echo "Building Ninja project for Linux."
precog ninja                                                                    \
  --clean                                                                       \
  --cross=arm,v6m,apple,darwin,macho                                            \
  --cvar=VERBOSE_LOGGING                                                        \
  -oninja
