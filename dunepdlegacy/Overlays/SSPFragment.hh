#ifndef artdaq_dune_Overlays_SSPFragment_hh
#define artdaq_dune_Overlays_SSPFragment_hh

#include "artdaq-core/Data/Fragment.hh"
//#include "artdaq/DAQdata/features.hh"
#include "cetlib_except/exception.h"
//#include "dune-artdaq/Generators/anlBoard/anlTypes.h"
#include "dunepdlegacy/Overlays/anlTypes.hh"

#include <ostream>
#include <vector>

// Implementation of "SSPFragment", an artdaq::Fragment overlay class
// used for pedagogical purposes

namespace dune {
  class SSPFragment;

  // Let the "<<" operator dump the SSPFragment's data to stdout
  std::ostream & operator << (std::ostream &, SSPFragment const &);
}

class dune::SSPFragment {
  public:

  // The SSPFragment represents its data through the adc_t type, which
  // is a typedef of an unsigned 32-bit integer.
  typedef unsigned int adc_t;

  // The "Metadata" struct is used to store info primarily related to
  // the upstream hardware environment from where the fragment came

  // "data_t" is a typedef of the fundamental unit of data the
  // metadata structure thinks of itself as consisting of; it can give
  // its size via the static "size_words" variable (
  // SSPFragment::Metadata::size_words )

  struct Metadata {
    //MillisliceHeader defined in anlTypes.hh
    typedef SSPDAQ::MillisliceHeader data_t;
    data_t sliceHeader;
    
    static size_t const size_words = 1ul; // Units of Metadata::data_t
  };

  static_assert (sizeof (Metadata) == Metadata::size_words * sizeof (Metadata::data_t), "SSPFragment::Metadata size changed");


  // The "Header" struct contains "metadata" specific to the fragment
  // which is not hardware-related

  // Header::data_t -- not to be confused with Metadata::data_t ! --
  // describes the standard size of a data type not just for the
  // header data, but ALSO the physics data beyond it; the size of the
  // header in units of Header::data_t is given by "size_words", and
  // the size of the fragment beyond the header in units of
  // Header::data_t is given by "event_size"

  // Notice only the first 28 bits of the first 32-bit unsigned
  // integer in the Header is used to hold the event_size ; this means
  // that you can't represent a fragment larger than 2**28 units of
  // data_t, or 1,073,741,824 bytes

  struct Header {
    typedef uint32_t data_t;

    typedef uint32_t event_size_t;  
    typedef uint32_t run_number_t;

    event_size_t event_size : 28;
    event_size_t unused_1   :  4;

    run_number_t run_number : 32;

    static size_t const size_words = 2ul; // Units of Header::data_t
  };

  static_assert (sizeof (Header) == Header::size_words * sizeof (Header::data_t), "SSPFragment::Header size changed");

  // The constructor simply sets its const private member "artdaq_Fragment_"
  // to refer to the artdaq::Fragment object

  SSPFragment(artdaq::Fragment const & f ) : artdaq_Fragment_(f) {}

  // const getter functions for the data in the header

  Header::event_size_t hdr_event_size() const { return header_()->event_size; } 
  Header::run_number_t hdr_run_number() const { return header_()->run_number; }
  static constexpr size_t hdr_size_words() { return Header::size_words; }

  // The number of ADC values describing data beyond the header
  size_t total_adc_values() const {
    return (hdr_event_size() - hdr_size_words()) * adcs_per_word_();
  }

  // Start of the ADC values, returned as a pointer to the ADC type
  adc_t const * dataBegin() const {
    return reinterpret_cast<adc_t const *>(header_() + 1);
  }

  // End of the ADC values, returned as a pointer to the ADC type
  adc_t const * dataEnd() const {
    return dataBegin() + total_adc_values();
  }

  protected:

  // Functions to translate between size (in bytes) of an ADC, size of
  // this fragment overlay's concept of a unit of data (i.e.,
  // Header::data_t) and size of an artdaq::Fragment's concept of a
  // unit of data (the artdaq::Fragment::value_type).

  static constexpr size_t adcs_per_word_() {
    return sizeof(Header::data_t) / sizeof(adc_t);
  }

  // header_() simply takes the address of the start of this overlay's
  // data (i.e., where the SSPFragment::Header object begins) and
  // casts it as a pointer to SSPFragment::Header

  Header const * header_() const {
    return reinterpret_cast<SSPFragment::Header const *>(artdaq_Fragment_.dataBeginBytes());
  }

private:

  artdaq::Fragment const & artdaq_Fragment_;
};

#endif /* artdaq_dune_Overlays_SSPFragment_hh */
