--------------------------------------------------------------------------------
-- Cakefile for testing.
--------------------------------------------------------------------------------

if platform == "Apple" then
  workspace "testing" {
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
