--------------------------------------------------------------------------------
-- Create a new workspace.
--------------------------------------------------------------------------------

local project = workspace:new'cog'

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
   self:set_include_paths'usr/share'
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
  if platform.is'microsoft'then
    self:prefix'src/engine/include/eon/eon.h'
  elseif platform.is'apple'then
    self:prefix'src/engine/include/xcode-prefix.pch'
  end
  return self
end

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--------------------------------------------------------------------------------

project:new'startup'
  : startupIncludePaths()
  : prefixHeader()
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : find_includes'src/com/start/include'
  : find_sources'src/com/start/src'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for engine build (xcode)
--------------------------------------------------------------------------------

project:new'gfc'
  : gfcIncludePaths()
  : prefixHeader()
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : find_includes'src/engine/include'
  : find_sources'src/engine/src'
  : ignore'nedmalloc'
  : target'static'

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

project:new'pal'
  : palIncludePaths()
  : prefixHeader()
  : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
  : find_sources'src/pal/src/${PLATFORM}'
  : find_includes'src/pal/include'
  : target'static'

--------------------------------------------------------------------------------
-- Lua library.
--------------------------------------------------------------------------------

if platform.is'apple'then
  project:new'lua'
    : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
    : set_include_paths'/usr/local/include'
    : find_includes'src/lua/5.3.5/include'
    : find_sources'src/lua/5.3.5/src'
    : target'static'
elseif platform.is'microsoft'then
  project:new'lua'
    : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
    : find_includes'src/lua/5.3.5/include'
    : find_sources'src/lua/5.3.5/src'
    : set_include_paths'usr/share'
    : target'static'
end

--------------------------------------------------------------------------------
-- Create a new project under workspace to compile application.
--------------------------------------------------------------------------------

if platform.is'apple'then
  project:new'cog'
    : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
    : install[[
        [ "${CONFIGURATION}" == 'Debug' ] && {
          rm "../rel/macOS/${CONFIGURATION}/${TARGET_NAME}.a" 2> /dev/null
          cp "${TARGET_BUILD_DIR}/${TARGET_NAME}.a" "../rel/macOS/${CONFIGURATION}" 2> /dev/null ]
          echo "Copied product!"
        } || [ "${CONFIGURATION}" == 'Release' ] && {
          rm "../rel/macOS/${CONFIGURATION}/${TARGET_NAME}.a" 2> /dev/null
          cp "${TARGET_BUILD_DIR}/${TARGET_NAME}.a" "../rel/macOS/${CONFIGURATION}" 2> /dev/null ]
          echo "Copied product!"
        }
        exit 0
      ]]
    : set_include_paths'src/app/include,/usr/local/include'
    : find_includes'src/app/include'
    : find_sources'src/app/src'
    : link_with'libboost_filesystem.a,liblz4.a,Foundation.framework,libpal.a,libgfc.a,libstartup.a,liblua.a'
    : prefix'src/engine/include/xcode-prefix.pch'
    : target'console'
elseif platform.is'microsoft'then
  project:new'cog'
    : defines('_DEBUG=1, DEBUG=1','NDEBUG=1')
    : set_include_paths'src/app/include,usr/share'
    : find_includes'src/app/include'
    : find_sources'src/app/src'
    : target'console'
end

--------------------------------------------------------------------------------
-- Save all projects to tmp directory.
--------------------------------------------------------------------------------

platform.save( project, 'tmp' )
