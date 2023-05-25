# ======================================================================
#  darkart main build file
#
#  cd .../path/to/build/directory
#  source ../path/to/dune-raw-data/ups/setup_for_development
#  buildtool
# ======================================================================


# use cmake 3.19 or later
cmake_minimum_required (VERSION 3.19)

project(dunepdlegacy VERSION 1.00.03 LANGUAGES CXX)

# cetbuildtools contains our cmake modules
find_package(cetbuildtools REQUIRED)

list(APPEND CMAKE_MODULE_PATH $ENV{CANVAS_ROOT_IO_DIR}/Modules $ENV{ART_DIR}/Modules)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
#  WERROR
# NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  EXTRA_DEFINITIONS -D_GLIBCXX_USE_NANOSLEEP #-march=native
)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_boost() # Needs to be first to cover for a missing transitive
                 # dependency from artdaq_core.
find_ups_product( artdaq_core )
find_ups_product( art )
find_ups_product( cetlib )
find_ups_product( gallery )
find_ups_product( messagefacility )

# have to make sure cmake doesn't select the system libraries
# define libsigc
cet_find_library( SIGC    NAMES sigc-2.0  PATHS ENV LIBSIGCPP_LIB NO_DEFAULT_PATH )

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)
include(CetTest)

# source
add_subdirectory(dunepdlegacy)

# test
add_subdirectory(test)

# run
add_subdirectory(run)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
