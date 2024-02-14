//------------------------------------------------------------------------------
//                   Copyright 2022 Creepy Doll Software, Inc.
//                            All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#import<boost/stacktrace.hpp>
#import<Cocoa/Cocoa.h>
#import<execinfo.h>

using namespace EON;
using namespace gfc;

#define SALLOC_SIZE 4096

#undef e_halt
#undef e_longlogf
#undef e_longlog
#undef e_logf
#undef e_log
#undef e_break

//================================================+=============================
//Private:{                                       |
  //[logging]:{                                   |

    string& string::asCLI( string& s ){
      //http://tldp.org/HOWTO/Bash-Prompt-HOWTO/x329.html
      s.replace( "$(black)",       "\033[0;30m" );
      s.replace( "$(red)",         "\033[0;31m" );
      s.replace( "$(green)",       "\033[0;32m" );
      s.replace( "$(brown)",       "\033[0;33m" );
      s.replace( "$(blue)",        "\033[0;34m" );
      s.replace( "$(purple)",      "\033[0;35m" );
      s.replace( "$(cyan)",        "\033[0;36m" );
      s.replace( "$(lightgrey)",   "\033[0;37m" );
      s.replace( "$(darkgrey)",    "\033[1;30m" );
      s.replace( "$(lightred)",    "\033[1;31m" );
      s.replace( "$(lightgreen)",  "\033[1;32m" );
      s.replace( "$(yellow)",      "\033[1;33m" );
      s.replace( "$(lightblue)",   "\033[1;34m" );
      s.replace( "$(lightpurple)", "\033[1;35m" );
      s.replace( "$(lightcyan)",   "\033[1;36m" );
      s.replace( "$(white)",       "\033[1;37m" );
      s.replace( "$(off)",         "\033[0m"    );
      return s;
    }

    string& string::strip( string& s ){
      asCLI( s                  );
      s.erase( "\033[0;30m"     );
      s.erase( "\033[0;31m"     );
      s.erase( "\033[0;32m"     );
      s.erase( "\033[0;33m"     );
      s.erase( "\033[0;34m"     );
      s.erase( "\033[0;35m"     );
      s.erase( "\033[0;36m"     );
      s.erase( "\033[0;37m"     );
      s.erase( "\033[1;30m"     );
      s.erase( "\033[1;31m"     );
      s.erase( "\033[1;32m"     );
      s.erase( "\033[1;33m"     );
      s.erase( "\033[1;34m"     );
      s.erase( "\033[1;35m"     );
      s.erase( "\033[1;36m"     );
      s.erase( "\033[1;37m"     );
      s.erase( "\033[0m"        );
      return s;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Methods:{                                       |
  //e_stktrc:{                                    |

    void e_stktrc( const std::function<void( EON::ccp )>& lambda ){
      if( !lambda ){
        return;
      }
      auto buffer = (vp*)e_malloc( 1024*sizeof( vp )); // NB:Guaranteed to succeed.
      auto n_ptrs = backtrace( buffer, 1024 );
      auto strngs = backtrace_symbols(
          buffer
        , n_ptrs );
      if( strngs ){
        for( auto n=n_ptrs, i=0; i<n; ++i ){
          lambda( strngs[ i ]);
        }
      }
      e_free( vp( buffer ));
    }

  //}:                                            |
  //e_warnsv:{                                    |

    s32 e_warnsv( ccp format, va_list va ){

      //------------------------------------------------------------------------
      // Format the warning string.
      //------------------------------------------------------------------------

      string wrn = "  $(lightpurple)WARNING $(yellow)";
      wrn.catv( format, va );
      wrn += "$(off)";

      //------------------------------------------------------------------------
      // Translate to CRT escape sequences.
      //------------------------------------------------------------------------

      string crt = wrn;
      string con = crt;
      string::asCLI( crt );
      IEngine::getStdout() += crt + "\n";

      //------------------------------------------------------------------------
      // Send to standard out, clear coloration if debuggingg.
      //------------------------------------------------------------------------

      string::strip( crt );
      return fprintf( stdout
        , "%s\n"
        , ccp( crt )
      );
    }

  //}:                                            |
  //e_warnsf:{                                    |

    s32 e_warnsf( ccp format,... ){
      va_list va;
      va_start( va, format );
        const s32 r = e_warnsv( format, va );
      va_end( va );
      return r;
    }

  //}:                                            |
  //e_breakf:{                                    |

    s32 e_breakf( ccp format,... ){
      va_list va;
      va_start( va, format );
          string breakf;
          breakf.catv(
              format
            , va );
      va_end( va );
      e_break(
        breakf
      );
    }

  //}:                                            |
  //e_break:{                                     |

    void e_break( ccp msg ){
      static auto isBroke = false;
      if( !isBroke ){
        puts( "" );
        puts( "***************" );
        puts( "*    BREAK    *" );
        puts( "***************" );
      }
      if( msg )
        printf( "  [%u]  %s\n", Thread::tid(), msg );
      if( e_getCvar<bool>( "USE_BUILT_IN_TRAP"_64 ))
        __builtin_trap();
      string _msg( msg );
      string _stk;
      const auto&__stk = boost::stacktrace::stacktrace();
      const auto&__vec = __stk.as_vector();
      for( auto n=__vec.size(), i=0ul; i<n; ++i ){
        const auto& t = __vec[ i ];
        _stk << t.name().c_str() << "\n";
      }
      e_throwGpuAlert(
          _msg
        , _stk
      );
    }

  //}:                                            |
  //e_msgv:{                                      |

    s32 e_msgv( ccp format, va_list va ){

      //------------------------------------------------------------------------
      // Format the warning string.
      //------------------------------------------------------------------------

      string msg;
      msg.catv( format, va );
      auto con( msg );
      msg += "$(off)";
      auto crt( msg );

      //------------------------------------------------------------------------
      // Translate to CRT escape sequences.
      //------------------------------------------------------------------------

      IEngine::getStdout() += string::asCLI( crt );
      IEngine::getStdout() += "\n";

      //------------------------------------------------------------------------
      // Send to standard out, clear coloration if debuggingg.
      //------------------------------------------------------------------------

      if( e_isDebugging() )
        string::strip( con );
      const auto l = fprintf( stdout
        , "%s\n"
        , ccp( con ));
      const string path( "~/.eon/run.log" );
      auto* f = fopen( path.os(), "a" );
      if( f ){
        #if!e_compiling( debug )
          string::strip( con );
        #endif
        fprintf( f, "%s\n", ccp( con ));
        fclose( f );
        f = nullptr;
      }
      return l;
    }

  //}:                                            |
  //e_logf:{                                      |

    s32 e_logf( ccp format,... ){
      va_list va;
      va_start( va, format );
        const s32 r = e_logv( format, va );
      va_end( va );
      return r;
    }

  //}:                                            |
  //e_logv:{                                      |

    s32 e_logv( ccp format, va_list va ){
      const auto l=vprintf( format, va );
      if( !l )
        return 0;
      printf( "\n" );
      return l+1;
    }

  //}:                                            |
  //e_log:{                                       |

    s32 e_log( ccp msg ){
      if( msg && *msg ){
        puts( msg );
        return s32( strlen( msg ));
      }
      return 0;
    }

  //}:                                            |
  //e_halt:{                                      |

    void e_halt( ccp msg ){
      puts( "**************" );
      puts( "*    HALT    *" );
      puts( "**************" );
      if( msg ){
        printf( "  [%u]  %s\n", Thread::tid(), msg );
      }
      if( e_isDebugging() &&
             e_getCvar<bool>( "_BREAK_INTO_CODE"_64 )){
        __builtin_trap();
      }else{
        exit( -1 );
      }
    }

  //}:                                            |
  //e_msgf:{                                      |

    s32 e_msgf( ccp format,... ){
      va_list va;
      va_start( va, format );
        const s32 r = e_msgv( format, va );
      va_end( va );
      return r;
    }

  //}:                                            |
  //e_msg:{                                       |

    s32 e_msg( ccp text ){

      //------------------------------------------------------------------------
      // Format the warning string.
      //------------------------------------------------------------------------

      string msg( text );
      auto con( msg );
      msg += "$(off)";
      auto crt( msg );

      //------------------------------------------------------------------------
      // Translate to CRT escape sequences.
      //------------------------------------------------------------------------

      if( e_isDebugging() ){
        string::strip( con );
      }
      string::asCLI( crt );
      IEngine::getStdout() += crt + "\n";

      //------------------------------------------------------------------------
      // Send to standard out, clear coloration if debuggingg.
      //------------------------------------------------------------------------

      const auto l = fprintf( stdout
        , "%s\n"
        , ccp( con ));
      const string path( "~/.eon/run.log" );
      auto* f = fopen( path.os(), "a" );
      if( f ){
        #if!e_compiling( debug )
          string::strip( con );
        #endif
        fprintf( f, "%s\n", ccp( con ));
        fclose( f );
        f = nullptr;
      }
      return l;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                                 vim:ft=objcpp
