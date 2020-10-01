# Cog -- A build system for Xcode 12 that you can ship with.

## OVERVIEW

Cog is a free build system from Creepy Doll Games. It supports Xcode 12 out of
the box and fills in the gaps where Cmake falls down on macOS. For completeness
it also supports Linux, Ninja file generation, and Visual Studio 2019 support.

## Lua

The first thing is the tool is native C++ running off a cut down version of my
game engine. That comes with built in support and integration for the latest
version of Lua. As such it just made sense to follow Premake's example and just
use it. There's some really huge advantages to this decision because a lot of
work went into the Lua integration for my new product (to be announced) so
there's an easy path to supporting texture manipulation, compression, mesh
importing and processing, etc.
