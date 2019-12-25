--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local project = workspace:new 'eon'--> Will create eon.xcworkspace or eon.sln

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

project:new 'startup' --> Will create startup.xcodeproj
  : deployment'10.15' --> Default macOS version.
  : defines(
      '_DEBUG=1, EON=1, DEBUG=1' --> debug
    , 'NDEBUG=1, EON=1' --> release
  )
  : headers 'src/com/start/include'
  : sources 'src/com/start/src'
  : prefix  'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target  'static'

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

project:new 'eon'                                      --> Will create eon.xcodeproj
  : organization 'Brian Hapgood'                       --> Ignored by windows.
  : identifier   'com.creepydollgames.eon'             --> For macOS, iOS and android.
  : team         'HE96RQ5ZY9'                          --> Apple team ID.
  : headers      'src/engine/include'                  --> scan this for headers.
  : sources      'src/engine/src'                      --> Scan this for sources.
  : prefix       'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : ignore       '\\bnedmalloc\\b'                     --> C++11 regex.
  : target       'framework'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win.
--------------------------------------------------------------------------------

project:new 'pal'
  : headers 'src/pal/include'
  : sources 'src/pal/src/${PLATFORM}'
  : prefix  'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target  'static'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

project:new 'cog'
  : headers 'src/app/include'
  : sources 'src/app/src'
  : prefix  'src/engine/include/xcode-prefix.pch' --> Precompiled header.
  : target  'console'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

return platform.save( project, 'tmp' )
