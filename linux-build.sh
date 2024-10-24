echo "Building Ninja project for Linux."
precog ninja                                                                    \
  --cross=arm,v6m,apple,darwin,macho                                            \
  --cvar=VERBOSE_LOGGING                                                        \
  --clean                                                                       \
  -oninja
