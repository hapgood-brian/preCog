local project = workspace:new'cog'

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

#include"lua/options.lua"

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

project:new'startup'
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths( 'usr/share/boost/1.71.0,'..EON_DIRECTORY )
  : prefix( EON_DIRECTORY..'/eon/eon.h' )
  : find_sources'src/common/start'
  : winsdk( WINSDK_VERSION )
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lz4.
--------------------------------------------------------------------------------

project:new'lz4'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lz4/include'
  : find_sources'src/lz4/src'
  : winsdk( WINSDK_VERSION )
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lua.
--------------------------------------------------------------------------------

project:new'lua'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lua/5.3.5/lua'
  : find_sources'src/lua/5.3.5/src'
  : winsdk( WINSDK_VERSION )
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

project:new'gfc'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths([[
    usr/share/boost/1.71.0,
    src/lz4/include,]]..EON_DIRECTORY )
  : prefix( EON_DIRECTORY..'/eon/eon.h' )
  : find_includes'src/core/include'
  : find_sources'src/core/src'
  : skip_unity'f32.cpp'
  : winsdk( WINSDK_VERSION )
  : target'static'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

project:new'pal'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths('usr/share/boost/1.71.0,'..EON_DIRECTORY )
  : prefix( EON_DIRECTORY..'/eon/eon.h' )
  : find_includes'src/pal/include'
  : find_sources'src/pal/src/win'
  : winsdk( WINSDK_VERSION )
  : target'static'

--------------------------------------------------------------------------------
-- Generate cog executable project.
--------------------------------------------------------------------------------

project:new'cog'
  : find_libraries[[
  $(SolutionDir)../lib/win64/boost/1.71.0,]]
  : set_include_paths([[
  src/applications/cog/include,
  usr/share/boost/1.71.0,
  src/lua/5.3.5,]]..EON_DIRECTORY )
  : find_includes'src/applications/cog/include'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : find_sources'src/applications/cog/src'
  : prefix( EON_DIRECTORY..'/eon/eon.h' )
  : link_with'gfc,lz4,lua,pal,startup,'
  : winsdk( WINSDK_VERSION )
  : target'console'

--------------------------------------------------------------------------------
-- Finally save off the script.
--------------------------------------------------------------------------------

platform.save( project )
