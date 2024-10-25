#precog ninja                                                                    \
#  --cross=x86_64,pc,linux,elf                                                   \
#  --cvar=VERBOSE_LOGGING                                                        \
#  -oninja
echo "Building Ninja project for Linux."
rm -rf ninja
precog ninja                                                                    \
  --cross=x86_64,pc,linux,elf                                                   \
  --cvar=VERBOSE_LOGGING                                                        \
  -oninja
