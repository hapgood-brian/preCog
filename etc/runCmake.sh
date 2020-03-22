OS=`uname`
[ "$OS" == 'Darwin' ] && {
  cmake .. -G Xcode -Wno-dev
} || {
  cmake .. -G "Visual Studio 16 2019" -Wno-dev
}
