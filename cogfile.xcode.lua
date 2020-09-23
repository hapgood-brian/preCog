--------------------------------------------------------------------------------
-- Create new solution from workspace object named 'eon'.
--------------------------------------------------------------------------------

local sln = workspace:new'eon'

--------------------------------------------------------------------------------
-- Generating for Visual Studio 2019.
--------------------------------------------------------------------------------

print'Generating for Xcode 11'

--------------------------------------------------------------------------------
-- Create a new sln under workspace to compile freestick.
--------------------------------------------------------------------------------

if false then
  VHACD_DEFINES={
      'CL_VERSION_1_1, OPENCL_FOUND, _DEBUG=1, DEBUG=1'
    , 'CL_VERSION_1_1, OPENCL_FOUND, NDEBUG=1'
  }
else
  VHACD_DEFINES={
      '_DEBUG=1, DEBUG=1'
    , 'NDEBUG=1'
  }
end
local vhacd = sln:new'vhacd'
  : set_include_paths'usr/share/v-hacd/src/VHACD_Lib/public'
  : find_includes'usr/share/v-hacd/src/VHACD_Lib/inc'
  : find_sources'usr/share/v-hacd/src/VHACD_Lib/src'
  : defines( VHACD_DEFINES )
  : target'static'

--------------------------------------------------------------------------------
-- Create a new sln under workspace to compile freestick.
--------------------------------------------------------------------------------

local freestick = sln:new'freestick'
  : disable'arc'
  : defines(
    'TARGET_OS_OSX, _DEBUG=1, DEBUG=1'
  , 'TARGET_OS_OSX, NDEBUG=1' )
  : set_include_paths'usr/share/freestick/inc'
  : find_includes'usr/share/freestick/inc'
  : find_sources[[
    usr/share/freestick/src/USB/platform/MacOSX,
    usr/share/freestick/src/USB/platform/iOS,
    usr/share/freestick/src/USB/common,
    usr/share/freestick/src/baseClasses,
    usr/share/freestick/src/3rdParty/EELog,
    usr/share/freestick/src/3rdParty/Mac,
    usr/share/freestick/src/FSDeviceInputEvent.cpp]]
  : target'static'

--------------------------------------------------------------------------------
-- Create a new sln under workspace to compile startup code.
--------------------------------------------------------------------------------

local startup = sln:new'startup'
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths[[
    /usr/local/include,
    include/eon]]
  : find_sources'src/startup'
  : prefix'include/xcode-prefix.pch'
  : target'static'

--------------------------------------------------------------------------------
-- Create a sln for the game foundation classes (gfc).
--------------------------------------------------------------------------------

local gfc = sln:new'gfc'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : set_include_paths[[
    /usr/local/include,
    include/eon,
    usr/share]]
  : prefix'include/xcode-prefix.pch'
  : find_sources[[
    usr/share/hash,
    src/gfc]]
  : target'static'

--------------------------------------------------------------------------------
-- Create a sln for the graphics library.
--------------------------------------------------------------------------------

local gfx = sln:new'gfx'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : set_include_paths[[
    usr/share/v-hacd/src/VHACD_Lib/public,
    usr/share/freestick/inc,
    usr/share,
    /usr/local/include,
    include/eon]]
  : prefix'include/xcode-prefix.pch'
  : find_sources'src/gfx'
  : target'static'

--------------------------------------------------------------------------------
-- Create a sln for the AI library.
--------------------------------------------------------------------------------

local ai = sln:new'ai'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : set_include_paths[[
    /Library/Frameworks/Mono.framework/Versions/Current/include/mono-2.0,
    /usr/local/include,
    usr/share/lua/5.3.5/src,
    usr/share/boost/1.71.0,
    usr/share,
    include/eon]]
  : prefix'include/xcode-prefix.pch'
  : find_sources'src/ai'
  : target'static'

--------------------------------------------------------------------------------
-- Create a new sln under workspace to compile startup code.
--------------------------------------------------------------------------------

local pal = sln:new'pal'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : set_include_paths[[
    /usr/local/include,
    include/eon]]
  : prefix'include/xcode-prefix.pch'
  : find_includes'src/pal/include'
  : find_sources'src/pal/src/osx'
  : target'static'

--------------------------------------------------------------------------------
-- Shim library.
--------------------------------------------------------------------------------

local shim = sln:new'shim'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : set_include_paths[[
    /usr/local/include,
    src/shim/include/osx,
    src/shim/include,
    include/eon]]
  : find_includes'src/shim/include/osx'
  : find_sources[[
    src/shim/src/pipeline.cpp,
    src/shim/src/osx]]
  : prefix'include/xcode-prefix.pch'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for the engine.
--
-- TODO: Instead of duplicating all the source and header files in this project
-- I should allow the project variables to be imported into find_sources() etc.
--
-- Think,
--    local eon = sln:new'eon'
--      : find_sources( shim,pal,ai,[[...]])
--------------------------------------------------------------------------------

local eon = sln:new'eon'
  --
  -- Setup signing credentials.
  --
  : organization'Brian Hapgood'
  : identifier'com.creepydollgames.eon'
  : team'HE96RQ5ZY9'
  --
  -- If you are using export_headers you don't need to double include here.
  --
  : export_headers[[
    include/eon/eon.h,
    include/eon/gfc,
    include/eon/gfx,
    include/eon/ai]]
  --
  -- Scan for libraries under the following path(s).
  --
  : find_libraries'lib/macOS'
  --
  -- Include headers from all sub-libraries.
  --
  : set_include_paths[[
    /Library/Frameworks/Mono.framework/Versions/Current/include/mono-2.0,
    /usr/local/include,
    usr/share/v-hacd/src/VHACD_Lib/public,
    usr/share/freestick/inc,
    src/shim/include/osx/Metal,
    src/shim/include/osx,
    src/shim/include,
    include/eon,
    usr/share]]
  --
  -- Include source files from all sub-libraries.
  --
  : find_includes'src/pal/include'
  : find_sources[[
    usr/share/hash,
    src/shim/src/pipeline.cpp,
    src/shim/src/osx,
    src/pal/src/osx,
    src/gfc,
    src/gfx,
    src/ai]]
  --
  -- We don't have any source files so we'll just pull in all the libraries we
  -- built previously and all system frameworks. System frameworks are seen by
  -- leaving off a path and the .framework extension.  Everything else has the
  -- full decoration of lib[name].a but no path.  The cog tool will scan for a
  -- library in the find_libraries clause above. Order doesn't matter.
  --
  : link_with[[
    CoreFoundation,
    GameController,
    ForceFeedback,
    Foundation,
    OpenCL,
    AppKit,
    IOKit,
    MetalPerformanceShaders,
    MetalKit,
    Metal,
    libboost_filesystem.a,
    libfreestick.a,
    libmono-2.0.a,
    libvhacd.a,
    libiconv.a,
    liblua.a,
    liblz4.a,
    libz.a]]
  --
  -- We're targeting a framework with this sln.
  --
  : prefix'include/xcode-prefix.pch'
  : target'framework'

--------------------------------------------------------------------------------
-- Save out the sln for this platform.
--------------------------------------------------------------------------------

platform.save( sln )
