--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local project = workspace:new'cog'
local BUILD_FRAMEWORK = false

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

project:new'startup'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include,src/engine/include'
  : find_include     'src/com/start/include'
  : find_source      'src/com/start/src'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'static'

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

if BUILD_FRAMEWORK then project:new'gfc'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : organization     'Brian Hapgood'
  : identifier       'com.creepydollgames.gfc'
  : team             'HE96RQ5ZY9'
  : set_include_path '/usr/local/include'
  : find_include     'src/engine/include'
  : find_source      'src/engine/src'
  : link_with        'libboost_filesystem.a,liblz4.a,Foundation.framework,libpal.a'
  : ignore           'nedmalloc'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'framework'
  : export_headers   'src/engine/include/eon/eon.h,src/engine/include/eon/gfc'
else project:new'gfc'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include'
  : find_include     'src/engine/include'
  : find_source      'src/engine/src'
  : ignore           'nedmalloc'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'static'
end

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

project:new'pal'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include'
  : find_include     'src/pal/include'
  : find_source      'src/pal/src/${PLATFORM}'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'static'

--------------------------------------------------------------------------------
-- Lua library.
--------------------------------------------------------------------------------

project:new'lua'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include'
  : find_include     'src/lua/5.3.5/include'
  : find_source      'src/lua/5.3.5/src'
  : target           'static'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

if BUILD_FRAMEWORK then project:new'cog'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path 'src/app/include,/usr/local/include'
  : find_include     'src/app/include'
  : find_source      'src/app/src'
  : link_with        'gfc.framework,libstartup.a,liblua.a'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'console'
else project:new'cog'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path 'src/app/include,/usr/local/include'
  : find_include     'src/app/include'
  : find_source      'src/app/src'
  : link_with        'libboost_filesystem.a,liblz4.a,Foundation.framework,libpal.a,libgfc.a,libstartup.a,liblua.a'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'console'
  : install[[
    [ ${CONFIGURATION} == 'Release' ] && {
      cp ${TARGET_BUILD_DIR}/${TARGET_NAME} /usr/local/bin
    }
  ]]
end

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

platform.save( project, 'tmp' )
