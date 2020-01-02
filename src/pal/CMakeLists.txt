message("Generating project for eonPAL")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB eonPAL_HEADERS "include/*.h")
if(WIN32)
  file(GLOB_RECURSE eonPAL_SOURCES "src/win/*.cpp")
elseif(EMSCRIPTEN)
  file(GLOB_RECURSE eonPAL_SOURCES "src/web/*.cpp")
elseif(APPLE)
  file(GLOB_RECURSE eonPAL_SOURCES "src/osx/*.mm")
elseif(UNIX)
  file(GLOB_RECURSE eonPAL_SOURCES "src/linux/*.cpp")
elseif(ANDROID)
  file(GLOB_RECURSE eonPAL_SOURCES "src/android/*.cpp")
endif()

#-------------------------------------------------------------------------------
# Create library for libeonPAL.a
#-------------------------------------------------------------------------------

add_library(eonPAL STATIC "${eonPAL_HEADERS}" "${eonPAL_SOURCES}")

#-------------------------------------------------------------------------------
# Choose C++17.
#-------------------------------------------------------------------------------

target_compile_features(eonPAL PUBLIC cxx_std_17)

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

# Setup all the include directories per platform.
if(WIN32)
  target_include_directories(eonPAL PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/lua/5.3.5/src"
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/usr/share"
    "${CMAKE_SOURCE_DIR}/src/engine/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}")
elseif(APPLE)
  target_include_directories(eonPAL PRIVATE
    "${CMAKE_SOURCE_DIR}/src/engine/include"
    "${CMAKE_SOURCE_DIR}/usr/share"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}"
    "/usr/local/include")
endif()

# Force include eon.h into every unity cxx.
if(MSVC)
  add_definitions(/FI"eon/eon.h")
elseif(APPLE)
  add_definitions(-include eon/eon.h)
endif()

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(eonPAL PRIVATE -W3)
endif()

#-------------------------------------------------------------------------------
# Cotire stuff: https://github.com/sakra/cotire/blob/master/MANUAL.md
#-------------------------------------------------------------------------------

if(WIN32 OR APPLE)
  # Set the linked libraries.
  set_target_properties(eonPAL PROPERTIES
    COTIRE_UNITY_LINK_LIBRARIES_INIT "NONE")
  # Set the multi-threading option...
  set_target_properties(eonPAL PROPERTIES
    COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES
    "-j")
  # Setup properties on cotire.
  set_target_properties(eonPAL PROPERTIES
    COTIRE_PREFIX_HEADER_IGNORE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}"
    COTIRE_PREFIX_HEADER_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
  # Tell cotire what the prefix header is.
  set_target_properties(eonPAL PROPERTIES
    COTIRE_CXX_PREFIX_HEADER_INIT
    "${CMAKE_SOURCE_DIR}/usr/src/engine/include/eon/eon.h")
  # Run that bad boy.
  cotire(eonPAL)
endif()
