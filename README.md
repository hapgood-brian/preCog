# Cog -- A AAA project generator for Ninja, Emscripten, Xcode 11-14, Visual Studio 2019-2022, you can ship with.

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

### Lua

The first thing is the tool comes with an integration of next to latest version of Lua
5.4.0. There's some really huge advantages to this decision because Lua is easy
to use and just works.

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
	
-Brian Hapgood
