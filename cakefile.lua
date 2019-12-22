--------------------------------------------------------------------------------
-- Cakefile for testing.
--------------------------------------------------------------------------------

if platform.is'Apple'then
  Workspace. "testing" {
    target "engine" {
      buildType"dylib"
    }
    target"name2"{
      build="shared"
    }
    target"app"{
    }
  }
end
