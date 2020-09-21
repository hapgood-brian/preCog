message("Generating project for eon")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE eon_HEADERS
  "${CMAKE_SOURCE_DIR}/usr/share/murmur3/murmur3.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE eon_SOURCES
  "${CMAKE_SOURCE_DIR}/usr/share/murmur3/murmur3.c"
  "${CMAKE_SOURCE_DIR}/usr/share/lz4/lib/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM eon_SOURCES
  "${CMAKE_SOURCE_DIR}/usr/share/lz4/lib/lz4frame.c"
  "${CMAKE_SOURCE_DIR}/usr/share/lz4/lib/xxhash.c")

#-------------------------------------------------------------------------------
# Create library for libeon.dll
#-------------------------------------------------------------------------------

add_library(eon STATIC
  "${eon_HEADERS}"
  "${eon_SOURCES}")

#-------------------------------------------------------------------------------
# Choose C++17.
#-------------------------------------------------------------------------------

target_compile_features(eon PUBLIC cxx_std_17)

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

list(APPEND INCLUDE_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}")
if(WIN32)
  target_include_directories(eon PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/src/lz4/include"
    "${CMAKE_SOURCE_DIR}/src/lua//5.3.5"
    "${INCLUDE_DIRS}")
  add_definitions(/FI"eon/eon.h")
elseif(APPLE)
  target_include_directories(eon PRIVATE
    "/usr/local/include"
    "${INCLUDE_DIRS}")
  add_definitions(-include eon/eon.h)
elseif(UNIX)
  target_include_directories(eon PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/src/lz4/include"
    "${CMAKE_SOURCE_DIR}/src/lua//5.3.5"
    "/usr/local/include"
    "${INCLUDE_DIRS}")
  add_definitions(-include eon/eon.h)
endif()

#-------------------------------------------------------------------------------
# Add some stuff to make lz4 compile.
#-------------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE EQUAL "Debug")
  add_compile_definitions(LZ4_DEBUG=2)
endif()

#-------------------------------------------------------------------------------
# Warnings and bigobj.
#-------------------------------------------------------------------------------

# Setup command line options.
if(WIN32)
  target_compile_options(eon PRIVATE -W3 /bigobj)
endif()

#-------------------------------------------------------------------------------
# Cotire stuff: https://github.com/sakra/cotire/blob/master/MANUAL.md
#-------------------------------------------------------------------------------

if(WIN32 OR APPLE OR UNIX)
  # Exclude files that don't play well with unity builds.
  set_property(SOURCE src/gfx/ri/mesh.cpp
    PROPERTY COTIRE_EXCLUDED TRUE)
  set_property(SOURCE src/gfc/f64.cpp
    PROPERTY COTIRE_EXCLUDED TRUE)
  set_property(SOURCE src/gfc/f32.cpp
    PROPERTY COTIRE_EXCLUDED TRUE)
  set_property(SOURCE src/gfc/f16.cpp
    PROPERTY COTIRE_EXCLUDED TRUE)
  # Set the linked libraries.
  set_target_properties(eon PROPERTIES
    COTIRE_UNITY_LINK_LIBRARIES_INIT "NONE")
  # Set the multi-threading option...
  set_target_properties(eon PROPERTIES
    COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES
    "-j")
  # Setup properties on cotire.
  set_target_properties(eon PROPERTIES
    COTIRE_PREFIX_HEADER_IGNORE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}"
    COTIRE_PREFIX_HEADER_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
  # Tell cotire what the prefix header is.
  set_target_properties(eon PROPERTIES
    COTIRE_CXX_PREFIX_HEADER_INIT
    "${CMAKE_CURRENT_SOURCE_DIR}/include/eon/eon.h")
  # Run that bad boy.
  cotire(eon)
endif()
