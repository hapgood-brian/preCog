message("Generating project for cog")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE cog_HEADERS "include/*.h")
file(GLOB_RECURSE cog_SOURCES "src/*.cpp")

#-------------------------------------------------------------------------------
# Create library for libcog.a
#-------------------------------------------------------------------------------

add_executable(cog
  "${cog_HEADERS}"
  "${cog_SOURCES}")

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
  target_include_directories(cog PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/usr/share"
    "${CMAKE_SOURCE_DIR}/src/engine/include"
    "${CMAKE_SOURCE_DIR}/src/lua/5.3.5"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}")
elseif(APPLE)
  target_include_directories(cog PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src/engine/include"
    "${CMAKE_SOURCE_DIR}"
    "/usr/local/include")
endif()

# Tell LUA to use 32-bit floats.
add_compile_definitions(cog LUA_FLOAT_TYPE=1)

# Force include eon.h into every unity cxx.
if(WIN32)
  add_definitions(/FI"eon/eon.h")
elseif(APPLE)
  add_definitions(-include eon/eon.h)
endif()

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(cog PRIVATE -W3)
endif()

#-------------------------------------------------------------------------------
# Cotire stuff: https://github.com/sakra/cotire/blob/master/MANUAL.md
#-------------------------------------------------------------------------------

if(WIN32 OR APPLE)
  # Exclude templatized files from unity.
  set_property(SOURCE src/view_schematic_block_input.cpp
    PROPERTY COTIRE_EXCLUDED TRUE)
  # Set the linked libraries.
  set_target_properties(cog PROPERTIES
    COTIRE_UNITY_LINK_LIBRARIES_INIT "NONE")
  # Set the multi-threading option...
  set_target_properties(cog PROPERTIES
    COTIRE_UNITY_SOURCE_MAXIMUM_NUMBER_OF_INCLUDES
    "-j")
  # Setup properties on cotire.
  set_target_properties(cog PROPERTIES
    COTIRE_PREFIX_HEADER_IGNORE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}"
    COTIRE_PREFIX_HEADER_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
  # Run that bad boy.
  cotire(cog)
endif()

#-------------------------------------------------------------------------------
# Linking stuff.
#-------------------------------------------------------------------------------

# Link in the shared code and other EON libraries.
if(WIN32)
  target_link_directories(cog_unity PUBLIC
    "${CMAKE_SOURCE_DIR}/lib/win64/boost/1.71.0")
  target_link_libraries(cog_unity PUBLIC
    eonStartup
    eonPAL
    eon
    lz4
    lua)
else(APPLE)
  find_framework(CoreFoundation)
  find_framework(Foundation)
  set_target_properties(cog_unity PROPERTIES
    COMPILE_FLAGS "${FRAMEWORK_PATHS}"
    LINK_FLAGS "${FRAMEWORK_PATHS} ${FRAMEWORK_NAMES}")
  find_package(Boost REQUIRED COMPONENTS filesystem)
  find_library(lz4 REQUIRED PATHS
    "/usr/local/lib")
  target_link_libraries(cog_unity PUBLIC
    ${Boost_LIBRARIES}
    ${lz4_LIBRARIES}
    eonStartup
    eonPAL
    eon
    lz4
    lua)
endif()
