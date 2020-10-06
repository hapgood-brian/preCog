--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local EON_DIRECTORY = 'src/core/include'
local project       = workspace:new'cog'
local USE_STARTUP   = 1
local USE_GFC       = 1
local USE_LZ4       = 1
local USE_LUA       = 1
local USE_PAL       = 1
local USE_COG       = 1

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local WINSDK_VERSION = '10.0.18362.0'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

if USE_STARTUP then project:new'startup'
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths('usr/share/boost/1.71.0,'..EON_DIRECTORY )
  : find_sources'src/common/start'
  : winsdk( WINSDK_VERSION )
  : prefix'eon/eon.h'
  : target'static'
end

--------------------------------------------------------------------------------
-- Setup the build settings for lz4.
--------------------------------------------------------------------------------

if USE_LZ4 then project:new'lz4'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lz4/include'
  : find_sources'src/lz4/src'
  : winsdk( WINSDK_VERSION )
  : target'static'
end

--------------------------------------------------------------------------------
-- Setup the build settings for lua.
--------------------------------------------------------------------------------

if USE_LUA then project:new'lua'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lua/5.3.5/lua'
  : find_sources'src/lua/5.3.5/src'
  : winsdk( WINSDK_VERSION )
  : target'static'
end

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

if USE_GFC then project:new'gfc'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths([[
  usr/share/boost/1.71.0,
  src/lz4/include,]]..EON_DIRECTORY )
  : find_includes'src/core/include'
  : find_sources'src/core/src'
  : skip_unity'f32.cpp'
  : winsdk( WINSDK_VERSION )
  : prefix'eon/eon.h'
  : target'static'
end

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

if USE_PAL then project:new'pal'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths('usr/share/boost/1.71.0,'..EON_DIRECTORY )
  : find_includes'src/pal/include'
  : find_sources'src/pal/src/win'
  : winsdk( WINSDK_VERSION )
  : prefix'eon/eon.h'
  : target'static'
end

--------------------------------------------------------------------------------
-- Generate cog executable project.
--------------------------------------------------------------------------------

if USE_COG then project:new'cog'
  : set_include_paths([[
  src/applications/cog/include,
  usr/share/boost/1.71.0,
  src/lua/5.3.5,]]..EON_DIRECTORY )
  : find_includes'src/applications/cog/include'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : find_sources'src/applications/cog/src'
  : link_with'eon,lz4,lua,pal,startup'
  : target'application'
  : winsdk( WINSDK_VERSION )
  : prefix'eon/eon.h'
end

--------------------------------------------------------------------------------
-- Finally save off the script.
--------------------------------------------------------------------------------

platform.save( project )
