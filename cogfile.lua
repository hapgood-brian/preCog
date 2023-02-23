--------------------------------------------------------------------------------
-- Print out what we're generating for.
--------------------------------------------------------------------------------

print('Generating for '..platform.name()..'.')

--------------------------------------------------------------------------------
-- Now include the appropriate target script based on the command line.
--------------------------------------------------------------------------------

require('src/cogfiles/cogfile.'..platform.name()..'.lua')
