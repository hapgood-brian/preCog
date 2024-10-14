//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include<boost/stacktrace.hpp>

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

    string& asCLI( string& s ){
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

    string& strip( string& s ){
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
      asCLI( crt );
      IEngine::getStdout() += crt + "\n";

      //------------------------------------------------------------------------
      // Send to standard out, clear coloration if debuggingg.
      //------------------------------------------------------------------------

      strip( crt );
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
  //e_break:{                                     |

    [[noreturn]] void e_break( ccp msg ){
      static auto isBroke = false;
      if( !isBroke ){
        puts( "" );
        puts( "***************" );
        puts( "*    BREAK    *" );
        puts( "***************" );
      }
      if( msg )
        printf( "  [%u]  %s\n", Thread::tid(), msg );
      string _msg( msg );
      string _stk;
      const auto&__stk = boost::stacktrace::stacktrace();
      const auto&__vec = __stk.as_vector();
      for( u32 n=__vec.size(), i=0u; i<n; ++i ){
        const auto& t = __vec[ i ];
        _stk << t.name().c_str() << "\n";
      }
      DebugBreak();
      exit( 0 );
    }

  //}:                                            |
  //e_breakf:{                                    |

    [[noreturn]] s32 e_breakf( ccp format,... ){
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

      IEngine::getStdout() += asCLI( crt );
      IEngine::getStdout() += "\n";

      //------------------------------------------------------------------------
      // Send to standard out, clear coloration if debuggingg.
      //------------------------------------------------------------------------

      if( IsDebuggerPresent() )
        strip( con );
      const auto l = fprintf( stdout
        , "%s\n"
        , ccp( con ));
      const string path( "~/.eon/run.log" );
      auto* f = fopen( path.os(), "a" );
      if( f ){
        #if!e_compiling( debug )
          strip( con );
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
      if( IsDebuggerPresent() ){
        DebugBreak();
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

      if( IsDebuggerPresent() )
        strip( con );
      asCLI( crt );
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
          strip( con );
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
