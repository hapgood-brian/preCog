cmake_minimum_required( VERSION 3.22.1 )
set( CMAKE_CXX_COMPILER clang++ )
set( CMAKE_C_COMPILER clang )
project( "gfc" )
add_library(
  gfc
  STATIC
  boot.cpp
  mutex.cpp
  thread.cpp
  sha1.cpp
  atomic.cpp
  stdafx.cpp
  mem.cpp
  i32.cpp
  serialize.cpp
  resource.cpp
  f64.cpp
  strings.cpp
  f16.cpp
  object.cpp
  timer.cpp
  history.cpp
  buffer.cpp
  scratch.cpp
  i64.cpp
  i8.cpp
  fs.cpp
  prefab.cpp
  f32.cpp
  class.cpp
  half.cpp
  i16.cpp
  murmur3.c
)
target_include_directories( gfc PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/1.84.0>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public> )
target_compile_definitions( gfc PUBLIC
  ANDROID=1
  NDEBUG=1 )
