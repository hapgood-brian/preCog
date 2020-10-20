local project = workspace:new'cog'

#define OPTIONS_FILE options

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

#include"include/${OPTIONS_FILE}.lua"

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

project:new'startup'
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths([[
    /usr/local/boost_1_71_0,]]
  ..EON_DIRECTORY )
  : prefix'src/core/include/eon/eon.h'
  : find_sources'src/common/start'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lz4.
--------------------------------------------------------------------------------

project:new'lz4'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lz4/include'
  : find_sources'src/lz4/src'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lua.
--------------------------------------------------------------------------------

project:new'lua'
  : defines( 'LUA_FLOAT_TYPE,_DEBUG=1,DEBUG=1'
  , 'LUA_FLOAT_TYPE,NDEBUG=1' )
  : set_include_paths'src/lua/5.3.5/lua'
  : find_sources'src/lua/5.3.5/src'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

project:new'gfc'
  : defines( 'LUA_FLOAT_TYPE,_DEBUG=1,DEBUG=1'
  , 'LUA_FLOAT_TYPE,NDEBUG=1' )
  : set_include_paths([[
    /usr/local/boost_1_71_0,
    src/lz4/include,]]
  ..EON_DIRECTORY )
  : prefix'src/core/include/eon/eon.h'
  : find_includes'src/core/include'
  : find_sources'src/core/src'
  : skip_unity'f32.cpp'
  : target'static'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

project:new'pal'
  : find_sources( 'src/pal/src/'..platform.name() )
  : find_includes'src/pal/include'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths([[
    /usr/local/boost_1_71_0,
    src/pal/include,]]
  ..EON_DIRECTORY )
  : prefix'src/core/include/eon/eon.h'
  : target'static'

--------------------------------------------------------------------------------
-- Generate cog executable project.
--------------------------------------------------------------------------------

project:new'cog'
  : defines( 'LUA_FLOAT_TYPE,_DEBUG=1,DEBUG=1'
  , 'LUA_FLOAT_TYPE,NDEBUG=1' )
  : set_include_paths([[
    src/lua/5.3.5,
    src/applications/cog/include,
    /usr/local/boost_1_71_0,]]
  ..EON_DIRECTORY )
  : find_includes'src/applications/cog/include'
  : find_sources'src/applications/cog/src'
  : link_with[[
    pthread,
    libgfc.a,
    liblua.a,
    libpal.a,
    liblz4.a,
    libstartup.a]]
  : prefix'src/core/include/eon/eon.h'
  : target'console'

--------------------------------------------------------------------------------
-- Finally save off the script.
--------------------------------------------------------------------------------

platform.save( project )
