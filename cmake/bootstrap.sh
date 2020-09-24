#!/bin/sh

OS=`uname`
echo "$OS"

#-------------------------------------------------------------------------------
# Clean all files depending on the platform.
#-------------------------------------------------------------------------------

[ "$1" = '--clean' ] && {
  rm cmake_install.cmake 2> /dev/null
  rm config.h 2> /dev/null
  rm CMakeCache.txt 2> /dev/null
  rm Makefile 2> /dev/null
  rm -rf build 2> /dev/null
  rm -rf CMakeFiles 2> /dev/null
  rm -rf usr 2> /dev/null
  rm -rf src 2> /dev/null
  rm -rf pal 2> /dev/null
  rm -rf share 2> /dev/null
  rm -rf .output 2> /dev/null
  rm -rf ../tmp 2> /dev/null
  [ "$OS" = 'Darwin' ] && {
    rm -rf cog.xcodeproj 2> /dev/null
    rm -rf CMakeScripts 2> /dev/null
    rm -rf cog.build 2> /dev/null
  } || [ "$OS" = 'Linux' ] && {
    rm cog.workspace 2> /dev/null
    rm cog.project 2> /dev/null
    rm build.ninja 2> /dev/null
    rm -rf .ninja_deps 2> /dev/null
    rm -rf .ninja_log 2> /dev/null
  } || {
    rm *.vcxproj
    rm *.vcxproj.filters
    rm *.vcxproj.user
    rm *.sln
    rm -rf Debug
    rm -rf Release
    rm -rf MinSizeRel
    rm -rf RelWithDebInfo
    rm -rf x64
    rm -rf .vs
  }
  exit 0
}

#-------------------------------------------------------------------------------
# Installation.
#-------------------------------------------------------------------------------

[ "$1" = '--install' ] && {
  [ "$OS" == 'Darwin' ] && {
    cp src/applications/cog ../bin/macOS/
    sudo cp ../bin/macOS/cog /usr/local/bin
    exit 0
  }

  [ "$OS" == 'Linux' ] && {
    cp src/applications/cog ../bin/linux/
    sudo cp ../bin/linux/cog /usr/local/bin
    exit 0
  }

  cp src/applications/Release/cog.exe ../bin/win64
  exit 0
}

#-------------------------------------------------------------------------------
# Apple macOS (Darwin)
#-------------------------------------------------------------------------------

[ "$OS" = 'Darwin' ] && {
  cmake .. -G "Unix Makefiles"
# cmake .. -G "Ninja"
# cmake .. -G "Xcode"
  make
  exit 0
}

#-------------------------------------------------------------------------------
# Generate project for Linux (using Ninja or Make).
#-------------------------------------------------------------------------------

[ "$OS" = 'Linux' ] && {
# sudo update-alternatives --config c++
# cmake .. -G "CodeLite - Ninja"
# cmake .. -G "Unix Makefiles"
  cmake .. -G "Ninja"
  ninja
  exit 0
}

#-------------------------------------------------------------------------------
# Microsoft Windows and Visual Studio 2019.
#-------------------------------------------------------------------------------

cmake .. -G "Visual Studio 16 2019" -Wno-dev
