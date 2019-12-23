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
  local eon = workspace:declare'eon'
  local lib = eon.projects:declare'eon'
  lib : include'usr/engine/include'
      : sources'usr/engine/src'
      : target'framework'
  out : generate( eon )
  return
end

--------------------------------------------------------------------------------
-- Define every project on the 64-bit windows platform.
--------------------------------------------------------------------------------

if platform.is'win64'then
  print'Constructing eon workspace for Visual Studio'
  return
end

--------------------------------------------------------------------------------
-- Just print an error!
--------------------------------------------------------------------------------

print'Unknown platform'
