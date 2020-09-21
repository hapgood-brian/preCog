message("Generating project for eonStartup")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE eonStartup_SOURCES "src/*.cpp")

#-------------------------------------------------------------------------------
# Create library for libeonStartup.a
#-------------------------------------------------------------------------------

add_library(eonStartup STATIC "${eonStartup_SOURCES}")

#-------------------------------------------------------------------------------
# Choose C++17.
#-------------------------------------------------------------------------------

target_compile_features(eonStartup PUBLIC cxx_std_17)

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

# Setup all the include directories per platform.
if(WIN32)
  target_include_directories(eonStartup PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/src/core/include"
    "${PROJECT_BINARY_DIR}")
elseif(ANDROID)
  target_include_directories(eonStartup PRIVATE
    "${CMAKE_SOURCE_DIR}/usr/share/boost/1.71.0"
    "${CMAKE_SOURCE_DIR}/src/core/include"
    "${PROJECT_BINARY_DIR}")
elseif(APPLE OR UNIX)
  target_include_directories(eonStartup PRIVATE
    "${CMAKE_SOURCE_DIR}/src/core/include"
    "${PROJECT_BINARY_DIR}"
    "/usr/local/include")
endif()

# Force include eon.h into every unity cxx.
if(MSVC)
  add_definitions(/FI"eon/eon.h")
elseif(APPLE OR UNIX)
  add_definitions(-include eon/eon.h)
endif()

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(eonStartup PRIVATE -W3)
endif()
