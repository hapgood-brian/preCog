--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local eonWorkspace = workspace:new'eon'--> Will create eon.xcworkspace

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile engine framework.
--------------------------------------------------------------------------------

local eonFramework = eonWorkspace:new'eon'--> Will create eon.xcodeproj

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

if platform.is 'apple' then
  eonFramework
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

if platform.is 'microsoft' then
  eonFramework
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
--[[
local start = eon:new'start'
      start : include'src/com/start/include'
            : sources'src/com/start/src'
            : target'static'
            : lang'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

local pal = eon:new'pal'
      pal : include'src/com/pal/include'
          : sources'src/com/pal/src'
          : target'static'
          : lang'c++17'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

local app = eon:new'cog'
      app : identifier'com.creepydollgames.cog'
          : organization'Brian Hapgood'
          : include'src/app/include'
          : sources'src/app/src'
          : team'HE96RQ5ZY9'
          : target'app'
          : lang'c++17'
]]
--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

return platform.save( eonWorkspace, 'tmp' )
