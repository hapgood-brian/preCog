# Cog -- A AAA project generator for Ninja, Emscripten, Xcode 12, Visual Studio 2019, you can ship with.

Works by scanning your directory structure with a simplistic Lua script and
creating static/shared libraries, console/windowed applications. Inspired by
cmake and premake.

## INSTALLATION

To install Cog on your system choose the installer for your platform, found
in releases in GitHub. For macOS, this will be a .pkg. For Windows, a .msi.
For Linux a Ubuntu snap package.

It's the same on Linux except exchange "macOS" for "linux". On Windows you will
need to copy cog.exe from the bin/win64 directory to somewhere that's in your
path or edit the environment variables to include the <COG_ROOT>/bin/win64
directory.

## OVERVIEW

Cog is a free build system from Creepy Doll Games. It supports Xcode 12 out of
the box and fills in the gaps where Cmake falls down on macOS. For completeness
it also supports Linux, Ninja file generation, and Visual Studio 2019 support.

### So why Cog?

The reason for writing Cog is I got really frustrated with cmake's support for
Xcode. It's stuck in the past, WAAAY in the past, and it's method of generating
xcproj's and no xcworkspaces is long in the tooth too. Also, with the advent of
Xcode 12 the build system cmake uses will soon go away. So something had to be
done, because I really wanted the power of cmake with the elegance of premake
and target all the build systems and IDEs I care about.

* Visual Studio 2019
* Emscripten
* Ninja
* Xcode 12 (11 is available too)

I want to add qmake and cmake to that list too, so I can generate Android
projects too.

### Lua

The first thing is the tool is native C++ running off a cut down version of my
game engine: EON. It comes with an integration of next to latest version of Lua
5.4.0. As such it just made sense to follow Premake's example and just use
that. There's some really huge advantages to this decision because Lua is easy
to use and just works. As shown by premake 5. A lot of work went into the
engine side so there's an easy path to supporting texture manipulation,
compression, mesh importing and processing, etc.

#### Examples

##### Your first Cog script.

To get a cog project up and running is very simple. Create a text file in the
root of your project called cogfile.lua. This follows the same paradigm as make
and cmake. In it write the following Lua code:

```
print'Hello, Cogger!'
```

Save the file and type `cog`! Instantly the program will display the version
and 'Hello, Cogger!' Not very useful, but it follows in a grand old tradition.

##### Your first *REAL* Cog script.

Now that we've done due dilligence and shown that Cog simply wraps Lua 5.3 at
this time, we'll make our next script. This one will be so fliipin' simple
you'll have a heart attack or die laughing, one of the two. I expect you have
an existing codebase that you want to port from Cmake, Qmake, Make, Xcode or
Visual Studio over to Cog. Yay! My very first victim!! Mwuhahahaha. It's quite
easy to accomplish this goal and the lines of code few.

Let's say we have an include directory, a src directory and a lib directory
full of third party code. At the end of the day what we want to do is generate
a Visual Studio 2019 solution on Windows, a great Xcode 12 workspace on macOS
targeting that OS, iOS or iPadOS, and Ninja on Linux. That's a tall order.
Let's see how Cog does:

###### macOS

```
-- Create a variable to hold a workspace named 'myProject'

local wsp = workspace:new 'myProject'
      wsp : new 'myApplication'
          : find_includes 'include'
          : find_libraries 'lib'
          : find_sources 'src'
          : target 'console'
```

So, now we have a much better script. The first thing it does is create a
workspcae object named myProject and assigns it to the variable wsp, which of
course is short for "wasp", no I'm kidding, it's short for "workspace". At the
command line you'll want to run this command in the same directory as your cog
file.

```
$ cog
```

That's it. Immediately, the program will create a new directory called "tmp",
generate myProject.xcworkspace and it's dependent myApplication.xcodeproj. If
you open up the workspace file by double clicking it in finder or typing at the
command line:

```
$ open tmp/myProject.xcworkspace
```

Then you'll have a lovely window. Hit Command+B and your program is produced!

## Getting Started

Things couldn't be more simpler to get things running. If you want to just use
Cog then look in the bin/ folder and the platform of your choice. I have
compiled up three versions for Linux, macOS and Windows. Should work on any
64-bit flavor of the OS. The Linux version has been tried and tested with
Ubuntu 20.04. The macOS version has been tried and tested with Catalina. The
latter has been code signed and relies on the Foundation and CoreFoundation
frameworks, which are part of the OS so no worries there. The Windows version
has not been signed. It's expensive to do so! LOL

	
-Brian Hapgood
