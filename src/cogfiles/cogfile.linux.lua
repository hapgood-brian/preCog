--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

#include<src/cogfiles/options.lua>

--------------------------------------------------------------------------------
-- Create the 'wsp' object and name it; this is the final program name on disk.
--------------------------------------------------------------------------------

wsp = workspace:new'cog'

--------------------------------------------------------------------------------
-- Create a new wsp under workspace to compile startup code.
--------------------------------------------------------------------------------

wsp:new'startup'
  : defines{'_DEBUG=1,DEBUG=1','NDEBUG=1'}
  : set_include_paths([[
    usr/share/boost/1.71.0,]]
  ..EON_DIRECTORY )
  : prefix'src/core/include/eon/eon.h'
  : find_sources'src/common/start'
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
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

wsp:new'gfc'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths([[
    usr/share/boost/1.71.0,
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
  : set_include_paths([[
    usr/share/boost/1.71.0,]]
  ..EON_DIRECTORY )
  : find_sources'src/pal/src/osx,src/pal/include'
  : prefix'src/core/include/eon/eon.h'
  : target'static'

--------------------------------------------------------------------------------
-- Generate cog executable wsp.
--------------------------------------------------------------------------------

wsp:new'cog'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths([[
    src/applications/include,
    usr/share/boost/1.71.0,
    src/lua/5.4.0,]]
  ..EON_DIRECTORY )
  : find_sources[[
      src/applications/cog/include,
      src/applications/cog/src,
    ]]
  -- Specify frameworks with no decoration and static libraries from other cog
  -- projects with full filename (pathing is allowed too).
  : link_with[[
      libstartup.a,
      libgfc.a,
      liblua.a,
      libpal.a,
      liblz4.a,
    ]]
  : prefix'src/core/include/eon/eon.h'
  : target'console'
