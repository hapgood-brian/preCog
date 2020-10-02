# Cog -- AAA project generator for Ninja, Emscripten, Xcode 12, Visual Studio 2019, you can ship with.

Works by scanning your directory structure with a simplistic Lua script and creating static/shared libraries, console/windowed applications. Inspired by cmake and premake.

## OVERVIEW

Cog is a free build system from Creepy Doll Games. It supports Xcode 12 out of
the box and fills in the gaps where Cmake falls down on macOS. For completeness
it also supports Linux, Ninja file generation, and Visual Studio 2019 support.

### So why Cog?

The reason for writing Cog is I got really frustrated with cmake's support for Xcode. It's stuck in the past, WAAAY in the past, and it's method of generating xcproj's and no xcworkspaces is long in the tooth too. Also, with the advent of Xcode 12 the build system cmake uses will soon go away. So something had to be done, because I really wanted the power of cmake with the elegance of premake and target all the build systems and IDEs I care about.

* Visual Studio 2019
* Emscripten
* Ninja
* Xcode 12 (11 is available too)

I want to add qmake and cmake to that list too, so I can generate Android projects too.

### Lua

The first thing is the tool is native C++ running off a cut down version of my
game engine: EON. It comes with an integration of next to latest
version of Lua 5.3.5.. As such it just made sense to follow Premake's example and just
use that. There's some really huge advantages to this decision because Lua is easy to use and just works. As shown by premake 5. A lot of work went into the engine side so
there's an easy path to supporting texture manipulation, compression, mesh
importing and processing, etc.

### EON

The EON engine is completely code modular so what made sense for the first iteration of Cog was to include the most basic module there is: GFC or the Game Foundation Classes. Most of this code is exposed to Lua so you can actually code up some great disk hacks and asset processing. It includes the universal engine file format, or .eon files, EON prefabs, memory management, an amazing little cvar system and much much more. My vision for Cog was to not only build projects but build assets you can use in C#, Lua and C++, the trifecta of languages supported by the engine. The only thing that's different between the big brother EON++ and the core EON that ships with Cog is the graphics and C# layers have been pulled out. Otherwise they're pretty much the same.

Look for EON++ in the near future as part of the Swordlight SDK.

## Getting Started

Things couldn't be more simpler to get things running. If you want to just use Cog then look in the bin/ folder and the platform of your choice. I have compiled up three versions for Linux, macOS and Windows. Should work on any 64-bit flavor of the OS. The Linux version has been tried and tested with Ubuntu 20.04. The macOS version has been tried and tested with Catalina. The latter has been code signed and relies on the Foundation and CoreFoundation frameworks, which are part of the OS so no worries there. The Windows version has not been signed. It's expensive to do so! LOL

### Compiling up the project

The principal method of compiling up your own fork of the project is to just pull it down from GitHub, and install the latest and greatest cmake and clang. The only two third party libraries that I use are Boost 1.71.0 and LZ4 for the file stream classes. These are included in the repository so you don't need to install anything.

To compile the project I have provided a little script called "cogger".

#### macOS Catalina

First of all make sure you have brew installed.

$ brew install cmake
$ chmod +x cogger
$ ./cogger install

That's it! It'll generate the cmake project for your platform and build the source. Once it's done it'll copy to your /usr/local/bin directory using sudo, so be prepared to type in your password :)

#### Ubuntu Linux

	$ sudo apt install cmake
	$ chmod +x cogger
	$ ./cogger install
	
That's about it. It'll copy the binary to the same place as on the Mac. If you don't want to install, just type

	$ ./cogger

And it will compile up everything into the cmake/src directory. Including project files.
