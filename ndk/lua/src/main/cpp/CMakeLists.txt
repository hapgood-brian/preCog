cmake_minimum_required( VERSION 3.22.1 )
set( CMAKE_CXX_COMPILER clang++ )
set( CMAKE_C_COMPILER clang )
project( "lua" )
add_library(
  lua
  
  lauxlib.c
  liolib.c
  lopcodes.c
  lstate.c
  lobject.c
  lmathlib.c
  loadlib.c
  lvm.c
  lfunc.c
  lstrlib.c
  linit.c
  lstring.c
  lundump.c
  lctype.c
  ltable.c
  ldump.c
  loslib.c
  lgc.c
  lzio.c
  ldblib.c
  lutf8lib.c
  lmem.c
  lcorolib.c
  lcode.c
  ltablib.c
  lapi.c
  lbaselib.c
  ldebug.c
  lparser.c
  llex.c
  ltm.c
  ldo.c
)
target_include_directories( lua PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public/lua>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../public> )
target_compile_definitions( lua PUBLIC
  ANDROID=1
  NDEBUG=1 )
find_library(
  log-lib
  log
)
target_link_libraries( lua PUBLIC
  ${log-lib}