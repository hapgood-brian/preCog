cmake_minimum_required( VERSION 3.22.1 )
set( CMAKE_CXX_COMPILER clang++ )
set( CMAKE_C_COMPILER clang )
project( "precog" )
add_library(
  precog
  CONSOLE
  generator.cpp
  ws.cpp
  ninja.cpp
  ndk.cpp
  luacore.cpp
  xcode.cpp
  msvc.cpp
  stdafx.cpp
  std.cpp
  qmake.cpp
  main.cpp
)
target_include_directories( precog PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/1.84.0>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/5.4.0>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public> )
target_compile_definitions( precog PUBLIC
  ANDROID=1
  NDEBUG=1 )
find_library(
  log-lib
  log
)
target_link_libraries( precog PUBLIC
  ${log-lib}