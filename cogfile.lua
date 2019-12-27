--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local project = workspace:new'eon'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

project
  : new 'startup'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include,src/engine/include'
  : find_include 'src/com/start/include'
  : find_source  'src/com/start/src'
  : prefix       'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target       'static'

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

project
  : new 'eon'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : organization     'Brian Hapgood'                       --> Ignored by windows.
  : identifier       'com.creepydollgames.eon'             --> For macOS, iOS and android.
  : team             'HE96RQ5ZY9'                          --> Apple team ID.
  : set_include_path '/usr/local/include'
  : find_include     'src/engine/include'                  --> scan this for headers.
  : find_source      'src/engine/src'                      --> Scan this for sources.
  : link_with        'libboost_filesystem.a,liblz4.a,Foundation.framework,libpal.a'
  : ignore           'nedmalloc'
  : prefix           'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target           'framework'
  : export_headers   'src/engine/include/eon/eon.h,src/engine/include/eon/gfc'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

project
  : new 'pal'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include'
  : find_include     'src/pal/include'
  : find_source      'src/pal/src/${PLATFORM}'
  : prefix           'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target           'static'

--------------------------------------------------------------------------------
-- Lua library.
--------------------------------------------------------------------------------

project
  : new 'lua'
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

project
  : new 'cog'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : set_include_path '/usr/local/include,src/app/include'
  : find_include     'src/app/include'
  : find_source      'src/app/src'
  : link_with        'eon.framework,libstartup.a,liblua.a'
  : prefix           'src/engine/include/xcode-prefix.pch'
  : target           'console'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

platform.save( project, 'tmp' )
