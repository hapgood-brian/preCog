cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0079 NEW)

#-------------------------------------------------------------------------------
# The subdirlist macro.
#-------------------------------------------------------------------------------

macro(subdirList result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

#-------------------------------------------------------------------------------
# Include cotire and speed up development!
#-------------------------------------------------------------------------------

if(WIN32 OR APPLE)
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
  include(cotire)
endif()

#-------------------------------------------------------------------------------
# Setup some basics for macOS.
#-------------------------------------------------------------------------------

if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")
  set(CMAKE_OSX_SYSROOT macosx10.15)
endif()

# ------------------------------------------------------------------------------
# Setup some platform dependent options.
# ------------------------------------------------------------------------------

# Establish which platform directory to look in (rel).
if(WIN32)
  set(OS "win64")
elseif(APPLE)
  set(OS "macOS")
endif()

# Setup necessary #defines for using the engine. Establishes debug/release.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DBGUG} -DNDEBUG -DRELEASE -DDISABLE_PERF_MEASUREMENT")
set(CMAKE_CXX_FLAGS_DBGUG   "${CMAKE_CXX_FLAGS_DBGUG} -D_DBGUG -DDEBUG   -DDISABLE_PERF_MEASUREMENT")

#-------------------------------------------------------------------------------
# Setup the core EON project.
#-------------------------------------------------------------------------------

# Setup the minimum required version.
cmake_minimum_required(VERSION 3.16)

# Setup the project and versioning.
project(juggle VERSION 0.3.2)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

# Configure the version header.
configure_file(config.h.in config.h)

#-------------------------------------------------------------------------------
# Add libraries and defines.
#-------------------------------------------------------------------------------

add_subdirectory(src)

#-------------------------------------------------------------------------------
# Setup linking and dependencies.
#-------------------------------------------------------------------------------

target_link_libraries(juggle
  eonStart
  eonPAL
  eon)
add_dependencies(juggle
  eonStart
  eonPAL
  eon)

#-------------------------------------------------------------------------------
# Setup OS variable.
#-------------------------------------------------------------------------------

if(WIN32)
  set(OS "win64")
elseif(APPLE)
  set(OS "macOS")
endif()

# ------------------------------------------------------------------------------
# Setup debugging directory in Visual Studio.
# ------------------------------------------------------------------------------

if(MSVC)
  set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT all_unity)
endif()
