#include "dunepdlegacy/Overlays/MilliSliceFragmentWriter.hh"
#include <vector>
#include <stdint.h>
#include <memory>

// JCF, 9/29/14

// A new wrinkle with gcc 4.9.1 is that it gives an error if a signed
// and unsigned are compared as happens in Boosts's test_tools.hpp
// file; this error is therefore explicitly disabled for the duration
// of the file

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"

#define BOOST_TEST_MODULE(MilliSliceFragment_t)
 #include "cetlib/quiet_unit_test.hpp"


BOOST_AUTO_TEST_SUITE(MilliSliceFragment_test)

BOOST_AUTO_TEST_CASE(BaselineTest)
{
  const uint16_t CHANNEL_NUMBER = 123;
  const uint32_t MILLISLICE_BUFFER_WORDS = 2000;
  const uint32_t MICROSLICE_BUFFER_SIZE = 1024;
  const uint32_t NANOSLICE_BUFFER_SIZE = 128;
  const uint16_t SAMPLE1 = 0x1234;
  const uint16_t SAMPLE2 = 0xc3c3;
  const uint16_t SAMPLE3 = 0xbeef;
  const uint16_t SAMPLE4 = 0xfe87;
  const uint16_t SAMPLE5 = 0x5a5a;
  std::unique_ptr<dune::MicroSlice> microslice_ptr;
  std::shared_ptr<dune::MicroSliceWriter> microslice_writer_ptr;
  std::shared_ptr<dune::NanoSliceWriter> nanoslice_writer_ptr;
  uint16_t value;

  // *** Use a MilliSliceFragmentWriter to build up a MilliSlice, checking
  // *** that everything looks good as we go.

  artdaq::Fragment fragment(MILLISLICE_BUFFER_WORDS);
  dune::MilliSliceFragmentWriter
    millislice_writer(fragment, MILLISLICE_BUFFER_WORDS * sizeof(artdaq::RawDataType));
  BOOST_REQUIRE_EQUAL(millislice_writer.size(), sizeof(dune::MilliSlice::Header));
  BOOST_REQUIRE_EQUAL(millislice_writer.microSliceCount(), 0);
  microslice_ptr = millislice_writer.microSlice(0);
  BOOST_REQUIRE(microslice_ptr.get() == 0);
  microslice_ptr = millislice_writer.microSlice(999);
  BOOST_REQUIRE(microslice_ptr.get() == 0);

  // test a request for a microslice that is too large

  {
    bool threw_exception = false;

    try {
      millislice_writer.reserveMicroSlice(MILLISLICE_BUFFER_WORDS *
					  +                                         sizeof(artdaq::RawDataType));
    } catch (const cet::exception& ) {
      threw_exception = true;
    }

    BOOST_REQUIRE_EQUAL(threw_exception, true);
  }

  // resume the building of the microslices in the millislice
  microslice_writer_ptr = millislice_writer.reserveMicroSlice(MICROSLICE_BUFFER_SIZE);
  BOOST_REQUIRE(microslice_writer_ptr.get() != 0);
  BOOST_REQUIRE_EQUAL(millislice_writer.size(),
                      sizeof(dune::MilliSlice::Header) + MICROSLICE_BUFFER_SIZE);
  BOOST_REQUIRE_EQUAL(millislice_writer.microSliceCount(), 1);
  if (microslice_writer_ptr.get() != 0) {
    nanoslice_writer_ptr = microslice_writer_ptr->reserveNanoSlice(NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE(nanoslice_writer_ptr.get() != 0);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->size(),
                        sizeof(dune::MicroSlice::Header) + NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->nanoSliceCount(), 1);
    if (nanoslice_writer_ptr.get() != 0) {
      nanoslice_writer_ptr->setChannelNumber(CHANNEL_NUMBER);
      nanoslice_writer_ptr->addSample(SAMPLE1);
    }
  }

  microslice_writer_ptr = millislice_writer.reserveMicroSlice(MICROSLICE_BUFFER_SIZE);
  BOOST_REQUIRE(microslice_writer_ptr.get() != 0);
  BOOST_REQUIRE_EQUAL(millislice_writer.size(), sizeof(dune::MilliSlice::Header) +
                      sizeof(dune::MicroSlice::Header) + sizeof(dune::NanoSlice::Header) +
                      sizeof(uint16_t) + MICROSLICE_BUFFER_SIZE);
  BOOST_REQUIRE_EQUAL(millislice_writer.microSliceCount(), 2);
  if (microslice_writer_ptr.get() != 0) {
    nanoslice_writer_ptr = microslice_writer_ptr->reserveNanoSlice(NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE(nanoslice_writer_ptr.get() != 0);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->size(),
                        sizeof(dune::MicroSlice::Header) + NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->nanoSliceCount(), 1);
    if (nanoslice_writer_ptr.get() != 0) {
      nanoslice_writer_ptr->setChannelNumber(CHANNEL_NUMBER+1);
      nanoslice_writer_ptr->addSample(SAMPLE2);
    }

    nanoslice_writer_ptr = microslice_writer_ptr->reserveNanoSlice(NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE(nanoslice_writer_ptr.get() != 0);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->size(), sizeof(dune::MicroSlice::Header) +
                        sizeof(dune::MicroSlice::Header) + sizeof(uint16_t) +
                        NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->nanoSliceCount(), 2);
    if (nanoslice_writer_ptr.get() != 0) {
      nanoslice_writer_ptr->setChannelNumber(CHANNEL_NUMBER+1);
      nanoslice_writer_ptr->addSample(SAMPLE3);
    }
  }

  microslice_writer_ptr = millislice_writer.reserveMicroSlice(MICROSLICE_BUFFER_SIZE);
  BOOST_REQUIRE(microslice_writer_ptr.get() != 0);
  BOOST_REQUIRE_EQUAL(millislice_writer.size(), sizeof(dune::MilliSlice::Header) +
                      2*sizeof(dune::MicroSlice::Header) + 3*sizeof(dune::NanoSlice::Header) +
                      3*sizeof(uint16_t) + MICROSLICE_BUFFER_SIZE);
  BOOST_REQUIRE_EQUAL(millislice_writer.microSliceCount(), 3);
  if (microslice_writer_ptr.get() != 0) {
    nanoslice_writer_ptr = microslice_writer_ptr->reserveNanoSlice(NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE(nanoslice_writer_ptr.get() != 0);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->size(),
                        sizeof(dune::MicroSlice::Header) + NANOSLICE_BUFFER_SIZE);
    BOOST_REQUIRE_EQUAL(microslice_writer_ptr->nanoSliceCount(), 1);
    if (nanoslice_writer_ptr.get() != 0) {
      nanoslice_writer_ptr->setChannelNumber(CHANNEL_NUMBER+2);
      nanoslice_writer_ptr->addSample(SAMPLE4);
      nanoslice_writer_ptr->addSample(SAMPLE5);
    }
  }

  // *** Finish off the creation of the MilliSlice and verify that we can't
  // *** add any more MicroSlices after it is finalized

  int32_t size_diff = millislice_writer.finalize();
  BOOST_REQUIRE_EQUAL(size_diff, MILLISLICE_BUFFER_WORDS*sizeof(artdaq::RawDataType) -
                      sizeof(dune::MilliSlice::Header) -
                      3*sizeof(dune::MicroSlice::Header) - 
                      4*sizeof(dune::NanoSlice::Header) - 5*sizeof(uint16_t));

  {
    bool threw_exception = false;
    
    try {
      millislice_writer.reserveMicroSlice(MICROSLICE_BUFFER_SIZE);
    } catch (const cet::exception& ) {
      threw_exception = true;
    }
    
    BOOST_REQUIRE_EQUAL(threw_exception, true);
  }

  // *** Now we construct an instance of a read-only MicroSlice from
  // *** the fragment and verify that everything still looks good

  dune::MilliSliceFragment millislice(fragment);
  BOOST_REQUIRE_EQUAL(millislice.size(), sizeof(dune::MilliSlice::Header) +
                      3*sizeof(dune::MicroSlice::Header) +
                      4*sizeof(dune::NanoSlice::Header) + 5*sizeof(uint16_t));
  BOOST_REQUIRE_EQUAL(millislice.microSliceCount(), 3);

  microslice_ptr = millislice.microSlice(1);
  BOOST_REQUIRE(microslice_ptr.get() != 0);
  if (microslice_ptr.get() != 0) {
    BOOST_REQUIRE_EQUAL(microslice_ptr->nanoSliceCount(), 2);
    std::unique_ptr<dune::NanoSlice> nanoslice_ptr = microslice_ptr->nanoSlice(1);
    BOOST_REQUIRE(nanoslice_ptr.get() != 0);
    if (nanoslice_ptr.get() != 0) {
      BOOST_REQUIRE_EQUAL(nanoslice_ptr->sampleCount(), 1);
      BOOST_REQUIRE(nanoslice_ptr->sampleValue(0, value));
      BOOST_REQUIRE_EQUAL(value, SAMPLE3);
    }
  }

  microslice_ptr = millislice.microSlice(2);
  BOOST_REQUIRE(microslice_ptr.get() != 0);
  if (microslice_ptr.get() != 0) {
    BOOST_REQUIRE_EQUAL(microslice_ptr->nanoSliceCount(), 1);
    std::unique_ptr<dune::NanoSlice> nanoslice_ptr = microslice_ptr->nanoSlice(0);
    BOOST_REQUIRE(nanoslice_ptr.get() != 0);
    if (nanoslice_ptr.get() != 0) {
      BOOST_REQUIRE_EQUAL(nanoslice_ptr->sampleCount(), 2);
      BOOST_REQUIRE(nanoslice_ptr->sampleValue(0, value));
      BOOST_REQUIRE_EQUAL(value, SAMPLE4);
      BOOST_REQUIRE(nanoslice_ptr->sampleValue(1, value));
      BOOST_REQUIRE_EQUAL(value, SAMPLE5);
    }
  }
}

#if 0
BOOST_AUTO_TEST_CASE(TinyBufferTest)
{

  // *** Test a buffer that is too small for even the header

}

BOOST_AUTO_TEST_CASE(SmallBufferTest)
{

  // *** Test a buffer that is too small for two microslices

}

BOOST_AUTO_TEST_CASE(CopyTest)
{
}

BOOST_AUTO_TEST_CASE(BufferReuseTest)
{
}
#endif

BOOST_AUTO_TEST_SUITE_END()

#pragma GCC diagnostic pop
