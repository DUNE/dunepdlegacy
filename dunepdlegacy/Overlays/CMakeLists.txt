# project name
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

art_make(BASENAME_ONLY
               LIBRARY_NAME dunepdlegacy_Overlays
               LIB_LIBRARIES
               artdaq_core::artdaq-core_Data
               artdaq_core::artdaq-core_Utilities
  cetlib_except::cetlib_except
  fhiclcpp::fhiclcpp
  messagefacility::MF_MessageLogger
  messagefacility::headers
  dunepdlegacy::rce_dataaccess
  gallery::gallery
  canvas::canvas
  ROOT::Core
  art::Framework_Principal
  pthread
  z
  )

install_headers()
install_source()
