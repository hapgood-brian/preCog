#/!/bin/sh
OS=`uname`

[ "$OS" == 'Darwin' ] && {
  cp src/applications/cog ../bin/macOS/
  exit 0
}

[ "$OS" == 'Linux' ] && {
  cp src/applications/cog ../bin/linux/
  exit 0
}

cp src/applications/Release/cog.exe ../bin/win64
exit 0
