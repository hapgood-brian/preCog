#!/bin/sh

OS=`uname`
echo "$OS"

#-------------------------------------------------------------------------------
# Apple macOS (Darwin)
#-------------------------------------------------------------------------------

set CXX=/usr/bin/clang++
set CC=/usr/bin/clang

[ "$OS" = 'Darwin' ] && {
  cmake .. -G "Unix Makefiles"
# cmake .. -G "Ninja"
# cmake .. -G "Xcode"
  exit 0
}

#-------------------------------------------------------------------------------
# Generate project for Linux (using Ninja or Make).
#-------------------------------------------------------------------------------

[ "$OS" = 'Linux' ] && {
# cmake .. -G "CodeLite - Ninja"
# cmake .. -G "Unix Makefiles"
  cmake .. -G "Ninja"
  exit 0
}

#-------------------------------------------------------------------------------
# Microsoft Windows and Visual Studio 2019.
#-------------------------------------------------------------------------------

cmake .. -G "Visual Studio 16 2019" -Wno-dev
