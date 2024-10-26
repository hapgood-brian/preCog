echo "Building Ninja project for Linux."
#precog ninja
#  --cross=x86_64,pc,linux,elf
#  --cvar=VERBOSE_LOGGING
#  -oninja
rm -rf ninja
precog ninja                                                                    \
  --cvar=VERBOSE_LOGGING                                                        \
  -oninja
