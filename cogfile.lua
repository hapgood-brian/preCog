--------------------------------------------------------------------------------
-- Print out what we're generating for.
--------------------------------------------------------------------------------

print( 'Generating for '..platform.name()..'.' )

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local EON_DIRECTORY = 'src/core/include'
local project       = workspace:new'cog'
local USE_STARTUP   = 1
local USE_GFC       = 1
local USE_LZ4       = 1
local USE_LUA       = 1
local USE_PAL       = 1
local USE_COG       = 1

--------------------------------------------------------------------------------
-- Now include the appropriate target script based on the command line.
--------------------------------------------------------------------------------

require( 'include/cogfile.'..platform.name()..'.lua' )

--------------------------------------------------------------------------------
-- Finally save off the script.
--------------------------------------------------------------------------------

platform.save( project )
