
# comment out the *slice* tests for DUNE
# trj, 28 June 2018.  Old code

# cet_test(DUNE_NanoSlice_t USE_BOOST_UNIT
#   LIBRARIES dunepdlegacy::Overlays 
#   ${ARTDAQ-CORE_DATA}
# )

# cet_test(DUNE_MicroSlice_t USE_BOOST_UNIT
#   LIBRARIES dunepdlegacy::Overlays
#   ${ARTDAQ-CORE_DATA}
# )

# cet_test(DUNE_MilliSlice_t USE_BOOST_UNIT
#   LIBRARIES dunepdlegacy::Overlays
#   ${ARTDAQ-CORE_DATA}
# )

# cet_test(DUNE_MilliSliceFragment_t USE_BOOST_UNIT
#   LIBRARIES dunepdlegacy::Overlays
#   ${ARTDAQ-CORE_DATA} 
# )

cet_test(DUNE_FelixFragment_t USE_BOOST_UNIT
  LIBRARIES dunepdlegacy::Overlays
  ${ARTDAQ-CORE_DATA}
  pthread
)

cet_test(DUNE_FelixReorder_t USE_BOOST_UNIT
  LIBRARIES dunepdlegacy::Overlays
  ${ARTDAQ-CORE_DATA}
  pthread
)

cet_test(DUNE_FelixAvxReorder_t USE_BOOST_UNIT
  LIBRARIES dunepdlegacy::Overlays
  ${ARTDAQ-CORE_DATA}
  pthread
)

cet_test(DUNE_FelixDecode_t USE_BOOST_UNIT
  LIBRARIES dunepdlegacy::Overlays
  ${ARTDAQ-CORE_DATA}
  ${GALLERY_LIB}
  pthread
)
