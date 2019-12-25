--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local project = workspace:new 'eon'--> Will create eon.xcworkspace

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile engine framework.
--------------------------------------------------------------------------------

local eon     = project:new 'eon'     --> Will create eon.xcodeproj
local startup = project:new 'startup' --> Will create startup.xcodeproj

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

if platform:is 'apple' then
  eon
    : organization 'Brian Hapgood'                       --> Ignored by windows.
    : identifier   'com.creepydollgames.eon'             --> For macOS, iOS and android.
    : team         'HE96RQ5ZY9'                          --> Apple team ID.
    : headers      'src/engine/include'                  --> scan this for headers.
    : sources      'src/engine/src'                      --> Scan this for sources.
    : prefix       'src/engine/include/xcode-prefix.pch' --> Precompiled header.
    : ignore       'nedmalloc'                           --> Stripped from build.
    : target       'framework'
    : lang         'c++17'
end

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (visual studio)
--------------------------------------------------------------------------------

if platform:is 'win64' then
  eon
    : headers 'src/engine/include'               --> scan this for headers.
    : sources 'src/engine/src'                   --> Scan this for sources.
    : prefix  'src/engine/include/msvc-prefix.h' --> Precompiled header.
    : ignore  'nedmalloc'                        --> Stripped from build.
    : target  'shared'
    : lang    'c++17'
end

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

startup
  : headers 'src/com/start/include'
  : sources 'src/com/start/src'
  : target  'static'
  : lang    'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win.
--------------------------------------------------------------------------------

project:new 'pal'
  : headers 'src/pal/include'
  : sources 'src/pal/src/${PLATFORM}'
  : target  'static'
  : lang    'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

project:new 'cog'
  : headers 'src/app/include'
  : sources 'src/app/src'
  : target  'console'
  : lang    'c++17'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

return platform.save( project, 'tmp' )
