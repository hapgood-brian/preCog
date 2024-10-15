--------------------------------------------------------------------------------
-- Create the 'wsp' object and name it; this is the final program name on disk.
--------------------------------------------------------------------------------

wsp = workspace:new'precog'

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local BOOST_DIRECTORY = 'usr/share/boost/1.84.0'
local EON_DIRECTORY   = 'src/core/include'

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
  : find_sources'src/pal/src/osx,src/pal/include'
  : prefix'src/core/include/eon/eon.h'
  : target'static'

--------------------------------------------------------------------------------
-- Generate precog executable wsp.
--------------------------------------------------------------------------------

wsp:new'precog'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths( BOOST_DIRECTORY..[[,
    src/applications/precog/include,
    src/lua/5.4.4,]]
  ..EON_DIRECTORY )
  : find_sources'src/applications/precog/src,src/applications/precog/include'
  -- Specify frameworks with no decoration and static libraries from other precog
  -- projects with full filename (pathing is allowed too).
  : link_with[[
      libstartup.a,
      liblua.a,
      libgfc.a,
      libpal.a,
      liblz4.a,
    ]]
  : prefix'src/core/include/eon/eon.h'
  : target'console'
