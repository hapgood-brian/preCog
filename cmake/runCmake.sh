OS=`uname`
[ "$OS" == 'Darwin' ] && {
  cmake .. -G Xcode -Wno-dev
  exit 0
}

[ "$OS" == 'Linux' ] && {
# cmake .. -G "CodeLite - Ninja" -Wno-dev
  cmake .. -G "Unix Makefiles"
  exit 0
}

cmake .. -G "Visual Studio 16 2019" -Wno-dev
