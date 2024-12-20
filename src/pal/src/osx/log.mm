//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#import<Cocoa/Cocoa.h>

using namespace EON;
using namespace gfc;

#define SALLOC_SIZE 4096

#undef e_halt
#undef e_longlogf
#undef e_longlog
#undef e_logf
#undef e_log
#undef e_brk

//================================================+=============================
//*CrtColors:{                                    |

  namespace{// http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x329.html
    void stripCrtColors( string& s ){

      s.erase( "$(black)"       );
      s.erase( "$(red)"         );
      s.erase( "$(green)"       );
      s.erase( "$(brown)"       );
      s.erase( "$(blue)"        );
      s.erase( "$(purple)"      );
      s.erase( "$(cyan)"        );
      s.erase( "$(lightgrey)"   );
      s.erase( "$(darkgrey)"    );
      s.erase( "$(lightred)"    );
      s.erase( "$(lightgreen)"  );
      s.erase( "$(yellow)"      );
      s.erase( "$(lightblue)"   );
      s.erase( "$(lightpurple)" );
      s.erase( "$(lightcyan)"   );
      s.erase( "$(white)"       );
      s.erase( ""         );

      s.erase( "\033[0;30m" );
      s.erase( "\033[0;31m" );
      s.erase( "\033[0;32m" );
      s.erase( "\033[0;33m" );
      s.erase( "\033[0;34m" );
      s.erase( "\033[0;35m" );
      s.erase( "\033[0;36m" );
      s.erase( "\033[0;37m" );
      s.erase( "\033[1;30m" );
      s.erase( "\033[1;31m" );
      s.erase( "\033[1;32m" );
      s.erase( "\033[1;33m" );
      s.erase( "\033[1;34m" );
      s.erase( "\033[1;35m" );
      s.erase( "\033[1;36m" );
      s.erase( "\033[1;37m" );
      s.erase( "\033[0m"    );
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//e_warnsf:{                                      |

  s32 e_warnsf( ccp format,... ){
    va_list va;
    va_start( va, format );
      const s32 r = e_warnsv( format, va );
    va_end( va );
    return r;
  }

//}:                                              |
//e_warnsv:{                                      |

  s32 e_warnsv( ccp format, va_list va ){

    //--------------------------------------------------------------------------
    // Format the warning string.
    //--------------------------------------------------------------------------

    string wrn;
    wrn += "$(lightpurple)WARNING $(yellow)";
    wrn.catv( format, va );
    wrn += "";

    //--------------------------------------------------------------------------
    // Translate to CRT escape sequences.
    //--------------------------------------------------------------------------

    string crt = wrn;
    stripCrtColors( crt );
    IEngine::getStdout() += crt + "\n";

    //--------------------------------------------------------------------------
    // Send to standard out, clear coloration if debuggingg.
    //--------------------------------------------------------------------------

    if( IEngine::isDebugging() ){
      stripCrtColors( wrn );
    }
    return fprintf( stdout, "%s\n", wrn.c_str() );
  }

//}:                                              |
//e_dbgf:{                                        |

  s32 e_dbgf( ccp format, ... ){
    va_list va;
    va_start( va, format );
      const s32 r = e_logv( format, va );
      DEBUG_BREAK
    va_end( va );
    return r;
  }

//}:                                              |
//e_msg:{                                         |

  s32 e_msg( ccp msg ){
    return e_msgf( msg );
  }

//}:                                              |
//e_msgf:{                                        |

  s32 e_msgf( ccp format,... ){
    va_list va;
    va_start( va, format );
      const s32 r = e_msgv( format, va );
    va_end( va );
    return r;
  }

//}:                                              |
//e_msgv:{                                        |

  s32 e_msgv( ccp format, va_list va ){

    //--------------------------------------------------------------------------
    // Format the warning string.
    //--------------------------------------------------------------------------

    string msg;
    msg.catv( format, va );
    msg += "";

    //--------------------------------------------------------------------------
    // Translate to CRT escape sequences.
    //--------------------------------------------------------------------------

    string crt = msg;
    stripCrtColors( crt );
    IEngine::getStdout() += crt + "\n";

    //--------------------------------------------------------------------------
    // Send to standard out, clear coloration if debuggingg.
    //--------------------------------------------------------------------------

    if( IEngine::isDebugging() ){
      stripCrtColors( crt );
    }
    return fprintf(
        stdout
      , "%s\n"
      , crt.c_str()
    );
  }

//}:                                              |
//e_logf:{                                        |

  s32 e_logf( ccp format,... ){
    va_list va;
    va_start( va, format );
      const s32 r = e_logv( format, va );
    va_end( va );
    return r;
  }

//}:                                              |
//e_logv:{                                        |

  s32 e_logv( ccp format, va_list va ){
    vprintf( format, va );
    puts( "" );
    return 1;
  }

//}:                                              |
//e_log:{                                         |

  s32 e_log( ccp msg ){
    if( msg && *msg ){
      puts( msg );
      return s32( strlen( msg ));
    }
    return 0;
  }

//}:                                              |
//e_brk:{                                         |

  void e_break( ccp msg ){
    puts("");
    puts( "***************" );
    puts( "*    BREAK    *" );
    puts( "***************" );
    if( msg ){
      printf( "[%u] %s\n", Thread::tid(), msg );
    }
    if( IEngine::isDebugging() )
      __builtin_trap();
    exit( -1 );
  }

//}:                                              |
//e_hlt:{                                         |

  void e_hlt( ccp msg ){
    puts("");
    puts( "**************" );
    puts( "*    HALT    *" );
    puts( "**************" );
    if( msg ){
      printf( "[%u] %s\n", Thread::tid(), msg );
    }
    if( IEngine::isDebugging() )
      __builtin_trap();
    exit( -1 );
  }

//}:                                              |
//================================================+=============================
//                                                                 vim:ft=objcpp
