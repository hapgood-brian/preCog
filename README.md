# Cog -- A build system for Xcode 12 that you can ship with.

## OVERVIEW

Cog is a free build system from Creepy Doll Games. It supports Xcode 12 out of
the box and fills in the gaps where Cmake falls down on macOS. For completeness
it also supports Linux, Ninja file generation, and Visual Studio 2019 support.

## Lua

The first thing is the tool is native C++ running off a cut down version of my
game engine: EON. It comes with an integration of next to latest
version of Lua 5.3.5.. As such it just made sense to follow Premake's example and just
use that. There's some really huge advantages to this decision because Lua is easy to use and just works. As shown by premake 5. A lot of work went into the engine side so
there's an easy path to supporting texture manipulation, compression, mesh
importing and processing, etc.

## EON

The EON engine is completely code modular so what made sense for the first iteration of Cog was to include the most basic module there is: GFC or the Game Foundation Classes. Most of this code is exposed to Lua so you can actually code up some great disk hacks and asset processing. It includes the universal engine file format, or .eon files, EON prefabs, memory management, an amazing little cvar system and much much more. My vision for Cog was to not only build projects but build assets you can use in C#, Lua and C++, the trifecta of languages supported by the engine. The only thing that's different between the big brother EON++ and the core EON that ships with Cog is the graphics and C# layers have been pulled out. Otherwise they're pretty much the same.

Look for EON++ in the near future as part of the Swordlight SDK.
