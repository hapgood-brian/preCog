# Cog -- A build system for Xcode 11 that you can ship with.

OVERVIEW

Cog is a free build system from Creepy Doll Games. Currently it only supports
Xcode 9.3+ (but really the focus was Xcode 11) especially app bundles, which
for the life of me I couldn't get CMake to support easily and I'm using the
latest. I got so frustrated with CMake that I decided to write my own.

https://stackoverflow.com/questions/40664125/cmake-and-code-signing-in-xcode-8-for-ios-projects/45860228

We also have support in v1.1.8+ for Visual Studio 2019+ projects.

Don't get me wrong, I like CMake a lot, but I was in a bind. I had to make a
milestone and I was losing a lot of time Googling for a feature that seemed
possible but was a HUGE amount of work. For example, after writing Cog, I
examined the asset importer CMakeLists.txt and was amazed to see it was around
1000 lines of code long! Compare that with Lua and you're at 103 lines that
does the same job: make a static library.

So what makes Cog so special? Why do we need yet another build system? I mean,
there's Make, Cmake, Premake, Jam, etc. etc. ad nauseum and now Cog. Well,
let's take a look.

Lua

The first thing is the tool is native C++ running off a cut down version of our
game engine. That comes with built in support and integration for the latest
version of Lua. As such it just made sense to follow Premake's example and just
use it. There's some really huge advantages to this decision because a lot of
work went into the Lua integration for our new product (to be announced) so
there's an easy path to supporting texture manipulation, compression, mesh
importing and processing, etc.
