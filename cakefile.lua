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
-- Build the project cross platform and save.
--------------------------------------------------------------------------------

local eon = workspace:declare'eon'--> Will create eon.xcworkspace
local lib = eon:declare'eon'--> Will create eon.xcproj
      lib : include'usr/engine/include'
          : sources'usr/engine/src'
          : target'framework'
if~platform.save(eon,'tmp')then
  error"Couldn't save to platform."
  return false
end
return true
