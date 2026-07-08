
cet_transitive_paths(FHICL_DIR BINARY IN_TREE)
cet_test_env_prepend(FHICL_FILE_PATH "." ${TRANSITIVE_PATHS_WITH_FHICL_DIR})
cet_transitive_paths(LIBRARY_DIR BINARY IN_TREE)
cet_test_env_prepend(CET_PLUGIN_PATH ${TRANSITIVE_PATHS_WITH_LIBRARY_DIR})

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
