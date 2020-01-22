--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local ws = workspace:new'cog'

--------------------------------------------------------------------------------
-- Include paths helper functions.
--------------------------------------------------------------------------------

local startupIncludePaths = function(self)
  if platform.is'microsoft'then
   self:set_include_paths'src/engine/include' 
  elseif platform.is'apple'then
   self:set_include_paths'/usr/local/include,src/engine/include' 
  end
  return self
end

local gfcIncludePaths = function(self)
  if platform.is'microsoft'then
    self:set_include_paths[[
      usr/share
    ]]
  elseif platform.is'apple'then
    self:set_include_paths'/usr/local/include'
  end
  return self
end

local palIncludePaths = function(self)
  if platform.is'microsoft'then
   self:set_include_paths'usr/share'
  elseif platform.is'apple'then
   self:set_include_paths'/usr/local/include'
  end
  return self
end

local prefixHeader = function(self)
  if platform.is'microsoft'then self
    : set_include_paths[[
      usr/share/boost/1.71.0,
      src/engine/include,
      src/app/include,
      src/lz4/include,
      src/lua/5.3.5]]
    : prefix'eon/eon.h'
  elseif platform.is'apple'then self
    : set_include_paths[[
      usr/share/boost/1.71.0,
      src/engine/include,
      src/app/include,
      src/lz4/include,
      src/lua/5.3.5]]
    : prefix'xcode-prefix.pch'
  end
  return self
end

--------------------------------------------------------------------------------
-- Create a new ws under workspace to compile startup code.
--------------------------------------------------------------------------------

local newStartup=(ws:new'startup'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : find_includes'src/com/start/include'
  : find_sources'src/com/start/src'
  : target'static')
startupIncludePaths(newStartup)
prefixHeader(newStartup)
newStartup = nil

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

local newGfc=(
  ws : new'gfc'
     : defines('LUA_FLOAT_TYPE=1,_DEBUG=1,DEBUG=1','LUA_FLOAT_TYPE=1,NDEBUG=1')
     : find_includes'src/engine/include'
     : find_sources'src/engine/src'
     : target'static')
gfcIncludePaths(newGfc)
prefixHeader(newGfc)
newGfc = nil

--------------------------------------------------------------------------------
-- Create a new ws under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win.  If
-- you name your platform specific directories like so then one line pulls in
-- the code for a specific platform.
--------------------------------------------------------------------------------

local newPal=(ws:new'pal'
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : find_sources'src/pal/src/${PLATFORM}'
  : find_includes'src/pal/include'
  : target'static')
palIncludePaths(newPal)
prefixHeader(newPal)
newPal = nil

--------------------------------------------------------------------------------
-- LZ4 library.
--------------------------------------------------------------------------------

ws:new'lz4'
  : set_include_paths'src/lz4/include'
  : find_includes'src/lz4/include'
  : find_sources'src/lz4/src'
  : target'static'

--------------------------------------------------------------------------------
-- Lua library.
--------------------------------------------------------------------------------

if platform.is'apple'then
  ws : new'lua'
     : defines('LUA_FLOAT_TYPE=1,_DEBUG=1,DEBUG=1','LUA_FLOAT_TYPE=1,NDEBUG=1')
     : set_include_paths'/usr/local/include'
     : find_includes'src/lua/5.3.5/lua'
     : find_sources'src/lua/5.3.5/src'
     : target'static'
elseif platform.is'microsoft'then
  ws : new'lua'
     : defines('LUA_FLOAT_TYPE=1,_DEBUG=1,DEBUG=1','LUA_FLOAT_TYPE=1,NDEBUG=1')
     : find_includes'src/lua/5.3.5/lua'
     : find_sources'src/lua/5.3.5/src'
     : set_include_paths[[
       src/lua/5.3.5/lua,
       usr/share]]
     : target'static'
end

--------------------------------------------------------------------------------
-- Create a new ws under workspace to compile application.
--------------------------------------------------------------------------------

if platform.is'apple'then
  ws:new'cog'
    :defines('LUA_FLOAT_TYPE=1,_DEBUG=1,DEBUG=1','LUA_FLOAT_TYPE=1,NDEBUG=1')
    :install[[
      [ "${CONFIGURATION}" == 'Debug' ] && {
        rm "../rel/macOS/${CONFIGURATION}/${TARGET_NAME}.a" 2> /dev/null
        cp "${TARGET_BUILD_DIR}/${TARGET_NAME}.a" "../rel/macOS/${CONFIGURATION}" 2> /dev/null ]
        echo "Copied product!"
      } || [ "${CONFIGURATION}" == 'Release' ] && {
        rm "../rel/macOS/${CONFIGURATION}/${TARGET_NAME}.a" 2> /dev/null
        cp "${TARGET_BUILD_DIR}/${TARGET_NAME}.a" "../rel/macOS/${CONFIGURATION}" 2> /dev/null ]
        echo "Copied product!"
      }
      exit 0]]
    : set_include_paths[[
        /usr/local/include,
        src/app/include
      ]]
    : find_includes'src/app/include'
    : find_sources'src/app/src'
    : link_with[[
        Foundation,
        libboost_filesystem.a,
        libstartup.a,
        liblz4.a,
        libpal.a,
        libgfc.a,
        liblua.a
        liblz4.a
      ]]
    : prefix'src/engine/include/xcode-prefix.pch'
    : target'console'
elseif platform.is'microsoft'then
  ws:new'cog'
    : defines('LUA_FLOAT_TYPE=1,_DEBUG=1,DEBUG=1','LUA_FLOAT_TYPE=1,NDEBUG=1')
    : prefix'eon/eon.h'
    : set_include_paths[[
        usr/share/boost/1.71.0,
        src/engine/include,
        src/app/include,
        src/lua/5.3.5
      ]]
    : find_libraries'lib/win64/boost/1.71.0'
    : find_includes'src/app/include'
    : find_sources'src/app/src'
    : target'console'
    : link_with[[
      startup.lib,
      pal.lib,
      gfc.lib,
      lua.lib,
      lz4.lib
    ]]
end

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

platform.save( ws )
