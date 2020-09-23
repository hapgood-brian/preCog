--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local EON_DIRECTORY = 'src/core/include'
local project       = workspace:new'cog'
local WIN_SDK       = '10.0.18362.0'
local USE_STARTUP   = 1
local USE_CORE      = 1
local USE_LZ4       = 1
local USE_LUA       = 1
local USE_PAL       = 1

--------------------------------------------------------------------------------
-- Generating for Visual Studio 2019.
--------------------------------------------------------------------------------

print'Generating for Visual Studio 2019.'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

if USE_STARTUP then project:new'startup'
  : winsdk( WIN_SDK )
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths([[
      usr/share/boost/1.71.0,]]
    ..EON_DIRECTORY )
  : find_sources'src/common/start'
  : prefix'eon/eon.h'
  : target'static'
end

--------------------------------------------------------------------------------
-- Setup the build settings for lz4.
--------------------------------------------------------------------------------

if USE_LZ4 then project:new'lz4'
  : winsdk( WIN_SDK )
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lz4/include'
  : find_sources'src/lz4/src'
  : target'static'
end

--------------------------------------------------------------------------------
-- Setup the build settings for lua.
--------------------------------------------------------------------------------

if USE_LUA then project:new'lua'
  : winsdk( WIN_SDK )
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lua/5.3.5/lua'
  : find_sources'src/lua/5.3.5/src'
  : target'static'
end

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

if USE_CORE then project:new'eon'
  : winsdk( WIN_SDK )
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths([[
    usr/share/boost/1.71.0,
    src/lz4/include,]]
    ..EON_DIRECTORY )
  : find_includes'src/core/include'
  : find_sources'src/core/src'
  : skip_unity'f32.cpp'
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
  : winsdk( WIN_SDK )
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths([[
    usr/share/boost/1.71.0,]]
    ..EON_DIRECTORY )
  : find_includes'src/pal/include'
  : find_sources'src/pal/src/win'
  : prefix'eon/eon.h'
  : target'static'
end

--------------------------------------------------------------------------------
-- Generate cog executable project.
--------------------------------------------------------------------------------

project:new'cog'
  : winsdk( WIN_SDK )
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths([[
    usr/share/boost/1.71.0,]]
    ..EON_DIRECTORY )
  : find_includes'src/applications/include'
  : find_sources'src/applications/src'
  : link_with'eon,lua,pal,startup'
  : prefix'eon/eon.h'
  : target'application'

--------------------------------------------------------------------------------
-- Save out the project for this platform.
--------------------------------------------------------------------------------

platform.save( project )
