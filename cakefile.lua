--------------------------------------------------------------------------------
-- Define every project on the mac platform.
--
-- Declaration types:
--    framework
--    shared
--    static
--------------------------------------------------------------------------------

if platform:is'apple'then
  print'Constructing eon workspace for Xcode'
elseif platform.is'win64'then
  print'Constructing eon workspace for Visual Studio'
else
  error'Unknown platform!'
end

--------------------------------------------------------------------------------
-- Build the project cross platform.
--------------------------------------------------------------------------------

local eon = workspace:declare'eon'--> Will create eon.xcworkspace
local lib = eon:declare'eon'--> Will create eon.xcproj
      lib : include'src/engine/include'
          : sources'src/engine/src'
          : target'framework'

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

return platform.save(eon,'tmp')
