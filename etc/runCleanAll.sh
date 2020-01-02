OS=`uname`
rm config.h 2> /dev/null
rm CMakeCache.txt 2> /dev/null
rm Makefile 2> /dev/null
rm *.cmake 2> /dev/null
rm -rf build 2> /dev/null
rm -rf CMakeFiles 2> /dev/null
rm -rf usr 2> /dev/null
rm -rf src 2> /dev/null
rm -rf pal 2> /dev/null
rm -rf share 2> /dev/null
rm -rf .output 2> /dev/null
rm -rf ../tmp 2> /dev/null
[ "$OS" == 'Darwin' ] && {
  rm -rf juggle.xcodeproj 2> /dev/null
  rm -rf CMakeScripts 2> /dev/null
  rm -rf juggle.build 2> /dev/null
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
