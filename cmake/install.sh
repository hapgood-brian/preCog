#/!/bin/sh
OS=`uname`

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
