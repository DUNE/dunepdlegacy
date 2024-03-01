# ======================================================================
#  darkart main build file
#
#  cd .../path/to/build/directory
#  source ../path/to/dune-raw-data/ups/setup_for_development
#  buildtool
# ======================================================================


# use cmake 3.19 or later
cmake_minimum_required (VERSION 3.20 FATAL_ERROR)
# cmake_policy(VERSION 3.19)

find_package(cetmodules REQUIRED)
project(dunepdlegacy VERSION 1.01.00 LANGUAGES CXX)
set(dunepdlegacy_FW_DIR "files")
# cetbuildtools contains our cmake modules

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
find_package(nufinder REQUIRED)
# these are minimum required versions, not the actual product versions
find_package(Boost REQUIRED COMPONENTS system) # Needs to be first to cover for a missing transitive
                 # dependency from artdaq_core.
find_package( artdaq_core REQUIRED )
find_package( art REQUIRED )
find_package( cetlib REQUIRED )
find_package( gallery REQUIRED )
find_package( messagefacility REQUIRED )

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
# packaging utility
cet_cmake_config()
