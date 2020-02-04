message("Generating project for lz4")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE lz4_SOURCES "src/*.c")
file(GLOB_RECURSE lz4_HEADERS "src/*.h")

#-------------------------------------------------------------------------------
# Create library for liblz4.a
#-------------------------------------------------------------------------------

add_library(lz4 STATIC "${lz4_SOURCES}" "${lz4_HEADERS}")

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

# Setup all the include directories per platform.
target_include_directories(lz4 PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include",
  "${PROJECT_BINARY_DIR}")

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(lz4 PRIVATE -W3)
endif()
