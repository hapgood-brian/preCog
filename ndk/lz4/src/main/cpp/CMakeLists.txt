cmake_minimum_required( VERSION 3.22.1 )
set( CMAKE_CXX_COMPILER clang++ )
set( CMAKE_C_COMPILER clang )
project( "lz4" )
add_library(
  lz4
  
  lz4hc.c
  lz4frame.c
  xxhash.c
  lz4.c
)
target_include_directories( lz4 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public> )
target_compile_definitions( lz4 PUBLIC
  ANDROID=1
  NDEBUG=1 )
find_library(
  log-lib
  log
)
target_link_libraries( lz4 PUBLIC
  ${log-lib}