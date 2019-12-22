message("Generating project for juggle")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE juggle_HEADERS "include/*.h")
file(GLOB_RECURSE juggle_SOURCES "src/*.cpp")

#-------------------------------------------------------------------------------
# Create library for libjuggle.a
#-------------------------------------------------------------------------------

add_executable(juggle
  "${juggle_HEADERS}"
  "${juggle_SOURCES}")

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

# Figure out what platform we're on.
if(WIN32)
  set(OS "win64")
elseif(APPLE)
  set(OS "macOS")
endif()

# Setup all the include directories per platform.
if(WIN32)
  target_include_directories(juggle PRIVATE
    "${CMAKE_SOURCE_DIR}/rel/${OS}/$(Configuration)/eon.framework"
    "${CMAKE_SOURCE_DIR}/usr/share/opencl/1.0/include"
    "${CMAKE_SOURCE_DIR}/usr/share/glew/2.1.0/include"
    "${CMAKE_SOURCE_DIR}/usr/share/lua/5.3.5/src"
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/usr/share/hash/include"
    "${CMAKE_SOURCE_DIR}/usr/share/xml/include"
    "${CMAKE_SOURCE_DIR}/usr/share/freeimage/3.18.0/Source"
    "${CMAKE_SOURCE_DIR}/usr/share"
    "${CMAKE_SOURCE_DIR}/usr/src/juggle/include"
    "${CMAKE_SOURCE_DIR}/usr/src/editors/include"
    "${CMAKE_SOURCE_DIR}/usr/src/export/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}")
elseif(APPLE)
  target_include_directories(juggle PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src/engine/include"
    "${CMAKE_SOURCE_DIR}"
    "/usr/local/include")
endif()

# Force include eon.h into every unity cxx.
if(MSVC)
  add_definitions(/FI"eon/eon.h")
elseif(XCODE)
  add_definitions(-include eon/eon.h)
endif()

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(juggle PRIVATE -W3)
endif()

#-------------------------------------------------------------------------------
# Cotire stuff: https://github.com/sakra/cotire/blob/master/MANUAL.md
#-------------------------------------------------------------------------------

if(WIN32 OR APPLE)
  # Exclude templatized files from unity.
  set_property(SOURCE src/view_schematic_block_input.cpp
    PROPERTY COTIRE_EXCLUDED TRUE)
  # Set the linked libraries.
  set_target_properties(juggle PROPERTIES
    COTIRE_UNITY_LINK_LIBRARIES_INIT "NONE")
  # Set the multi-threading option...
  set_target_properties(juggle PROPERTIES
    COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES
    "-j")
  # Setup properties on cotire.
  set_target_properties(juggle PROPERTIES
    COTIRE_PREFIX_HEADER_IGNORE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}"
    COTIRE_PREFIX_HEADER_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
  # Run that bad boy.
  cotire(juggle)
endif()

#-------------------------------------------------------------------------------
# Linking stuff.
#-------------------------------------------------------------------------------

# Link in the shared code and other EON libraries.
if(WIN32)
  target_link_directories(juggle_unity PUBLIC
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0/stage/lib"
    "${CMAKE_SOURCE_DIR}/usr/share/opencl/1.0/lib/x86_64")
  target_link_libraries(juggle_unity PUBLIC
    filesystem
    lua
    eonPAL
    lz4)
else(APPLE)
  find_framework(CoreFoundation)
  find_framework(Foundation)
  set_target_properties(juggle_unity PROPERTIES
    COMPILE_FLAGS "${FRAMEWORK_PATHS}"
    LINK_FLAGS "${FRAMEWORK_PATHS} ${FRAMEWORK_NAMES}")
  find_package(Boost REQUIRED COMPONENTS filesystem)
  find_library(lz4 REQUIRED PATHS
    "/usr/local/lib")
  target_link_libraries(juggle_unity PUBLIC
    ${Boost_LIBRARIES}
    ${lz4_LIBRARIES}
    lua
    eonPAL
    lz4)
endif()
