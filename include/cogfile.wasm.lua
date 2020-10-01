--------------------------------------------------------------------------------
-- Build options.
--
-- These little local variables control which projects get compiled (USE_*) and
-- also what directory to look in for the EON include headers. This project has
-- been cut down from the full EON experience so I can GPU3 it.
--------------------------------------------------------------------------------

local EON_DIRECTORY = 'src/core/include'
local project       = workspace:new'cog'
local USE_COG       = 1

--------------------------------------------------------------------------------
-- Generating for Ninja on Linux boxes.
--------------------------------------------------------------------------------

print'Generating for Emscripten.'

--------------------------------------------------------------------------------
-- Generate cog executable project.
--------------------------------------------------------------------------------

if USE_COG then project:new'cog'
  : defines( 'LUA_FLOAT_TYPE,_DEBUG=1,DEBUG=1'
  , 'LUA_FLOAT_TYPE,NDEBUG=1' )
  : set_include_paths([[
  src/applications/cog/include,
  usr/share/boost/1.71.0,
  src/lz4/include,
  src/pal/include,
  src/lua/5.3.5,]]
  ..EON_DIRECTORY )
  : find_includes[[
  src/applications/cog/include
  src/core/include]]
  : find_sources([[
  src/applications/cog/src
  src/lua/5.3.5/src,
  src/common/start,
  src/core/src,
  src/lz4/src,
  src/pal/src/]]..platform.name() )
  : prefix'src/core/include/eon/eon.h'
  : skip_unity'f32.cpp'
  : target'console'
end

--------------------------------------------------------------------------------
-- Save out the project for this platform.
--------------------------------------------------------------------------------

platform.save( project )
