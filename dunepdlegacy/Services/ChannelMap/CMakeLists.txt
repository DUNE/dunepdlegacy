
# Jingbo Wang, John Freeman, with mods from Tom Junk
# 2017

install_fhicl()
install_headers()
install_source()

file(GLOB channel_map_file *ChannelMap*.txt)
install_fw( LIST ${channel_map_file} )

# Add plugin for each service


simple_plugin(PdspChannelMapService "service"
              ${ART_PERSISTENCY_ROOTDB}
              ${ART_FRAMEWORK_SERVICES_REGISTRY}
              ${persistency_lib}
              ${SQLITE3}
              messagefacility::MF_MessageLogger
              ${CETLIB_LIBS}
              ${ROOT_BASIC_LIB_LIST}
)


simple_plugin(IcebergChannelMapService "service"
              ${ART_PERSISTENCY_ROOTDB}
              ${ART_FRAMEWORK_SERVICES_REGISTRY}
              ${persistency_lib}
              ${SQLITE3}
              messagefacility::MF_MessageLogger
              ${CETLIB_LIBS}
              ${ROOT_BASIC_LIB_LIST}
)

