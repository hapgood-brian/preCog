message("Generating project for eonStart")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE eonStart_SOURCES "src/*.cpp")

#-------------------------------------------------------------------------------
# Create library for libeonStart.a
#-------------------------------------------------------------------------------

add_library(eonStart STATIC "${eonStart_SOURCES}")

#-------------------------------------------------------------------------------
# Choose C++17.
#-------------------------------------------------------------------------------

target_compile_features(eonStart PUBLIC cxx_std_17)

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

# Setup all the include directories per platform.
if(WIN32)
  target_include_directories(eonStart PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/usr/src/engine/include"
    "${PROJECT_BINARY_DIR}")
elseif(ANDROID)
  target_include_directories(eonStart PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/usr/src/engine/include"
    "${PROJECT_BINARY_DIR}")
elseif(APPLE)
  target_include_directories(eonStart PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/src/engine/include"
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
  target_compile_options(eonStart PRIVATE -W3)
endif()
