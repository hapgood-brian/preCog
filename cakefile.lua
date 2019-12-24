--------------------------------------------------------------------------------
-- Define every project on the mac platform.
--------------------------------------------------------------------------------

local eon = workspace:declare'eon'--> Will create eon.xcworkspace
local lib = eon:declare'eon'--> Will create eon.xcproj
      lib : identifier'com.creepydollgames.eon'
          : organization'Brian Hapgood'
          : include'src/engine/include'
          : sources'src/engine/src'
          : target'framework'
          : team'HE96RQ5ZY9'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

return platform.save( eon, 'tmp' )
