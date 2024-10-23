--------------------------------------------------------------------------------
-- Create the 'wsp' object and name it; this is the final program name on disk.
--------------------------------------------------------------------------------

wsp = workspace:new'precog'

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local BOOST_DIRECTORY = 'usr/share/boost/1.84.0'
local EON_DIRECTORY   = 'src/core/include'
local WINSDK_VERSION  = '10.0.19041.0'

--------------------------------------------------------------------------------
-- Create a new wsp under workspace to compile startup code.
--------------------------------------------------------------------------------

wsp:new'startup'
  : defines{'_DEBUG=1,DEBUG=1','NDEBUG=1'}
  : set_include_paths( BOOST_DIRECTORY )
  : prefix'src/core/include/eon/eon.h'
  : find_sources'src/bootseq/start'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lz4.
--------------------------------------------------------------------------------

wsp:new'lz4'
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths'src/lz4/include'
  : find_sources'src/lz4/src'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lua.
--------------------------------------------------------------------------------

wsp:new'lua'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lua/5.4.4/lua'
  : find_sources'src/lua/5.4.4/src'
  : ignore'lua.c,luac.c'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

wsp:new'gfc'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths( BOOST_DIRECTORY..[[,
    src/lz4/include,]]
  ..EON_DIRECTORY )
  : find_sources'src/core/src,src/core/include'
  : prefix'src/core/include/eon/eon.h'
  : skip_unity'f32.cpp'
  : target'static'

--------------------------------------------------------------------------------
-- Create a new wsp under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

wsp:new'pal'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths(
    BOOST_DIRECTORY..','
  ..EON_DIRECTORY )
  : find_sources'src/pal/src/win,src/pal/include'
  : prefix'src/core/include/eon/eon.h'
  : target'static'

--------------------------------------------------------------------------------
-- Generate precog executable wsp.
--------------------------------------------------------------------------------

wsp:new'precog'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths( BOOST_DIRECTORY..[[,
    src/console/precog/include,
    src/lua/5.4.0,]]
  ..EON_DIRECTORY )
  : find_sources'src/console/precog/src,src/console/precog/include'
  : find_libraries(BOOST_DIRECTORY..'/stage/lib/')
  : link_with[[libboost_filesystem-vc143-mt-x64-1_84.lib,
      gfc.lib,
      lua.lib,
      pal.lib,
      lz4.lib,
      startup.lib]]
  : prefix'src/core/include/eon/eon.h'
  : target'console'

