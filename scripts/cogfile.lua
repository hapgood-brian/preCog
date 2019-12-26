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
  : include_path '/usr/local/include,src/engine/include'
  : scan_include 'src/com/start/include'
  : scan_source  'src/com/start/src'
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
  : organization 'Brian Hapgood'                       --> Ignored by windows.
  : identifier   'com.creepydollgames.eon'             --> For macOS, iOS and android.
  : team         'HE96RQ5ZY9'                          --> Apple team ID.
  : include_path '/usr/local/include'                  --> Header search paths.
  : scan_include 'src/engine/include'                  --> scan this for headers.
  : scan_source  'src/engine/src'                      --> Scan this for sources.
  : link_with    'libboost_filesystem.a,liblz4.a,Foundation.framework,libpal.a'
  : scan_res     'src/engine/res'                      --> scan this for resources.
  : ignore       'nedmalloc'
  : prefix       'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target       'framework'

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
  : include_path '/usr/local/include,src/engine/include'
  : scan_include 'src/pal/include'
  : scan_source  'src/pal/src/${PLATFORM}'
  : prefix       'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target       'static'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

project
  : new 'cog'
  : defines(
    '_DEBUG=1, DEBUG=1',
    'NDEBUG=1' )
  : include_path '/usr/local/include,src/engine/include'
  : scan_include 'src/app/include'
  : scan_source  'src/app/src'
  : link_with    'eon.framework'
  : prefix       'src/engine/include/xcode-prefix.pch'
  : target       'console'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

platform.save( project, 'tmp' )
