cmake_minimum_required( VERSION 3.22.1 )
set( CMAKE_CXX_COMPILER clang++ )
set( CMAKE_C_COMPILER clang )
project( "pal" )
add_library(
  pal
  STATIC
)
target_include_directories( pal PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/1.84.0>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public> )
target_compile_definitions( pal PUBLIC
  ANDROID=1
  NDEBUG=1 )
