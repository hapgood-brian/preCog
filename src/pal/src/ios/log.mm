//------------------------------------------------------------------------------
//       Copyright 2014-2018 Creepy Doll Games LLC. All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
// NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#import<UIKit/UIKit.h>

using namespace gfc;

#define SALLOC_SIZE 4096

#undef e_halt
#undef e_longlogf
#undef e_longlog
#undef e_logf
#undef e_log
#undef e_brk

//================================================|=============================
//tranlateCrtColors:{                                |

  namespace{
    void translateCrtColors( string& s ){
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
    }

    void stripCrtColors( string& s ){
      s.replace( "$(black)",       "" );
      s.replace( "$(red)",         "" );
      s.replace( "$(green)",       "" );
      s.replace( "$(brown)",       "" );
      s.replace( "$(blue)",        "" );
      s.replace( "$(purple)",      "" );
      s.replace( "$(cyan)",        "" );
      s.replace( "$(lightgrey)",   "" );
      s.replace( "$(darkgrey)",    "" );
      s.replace( "$(lightred)",    "" );
      s.replace( "$(lightgreen)",  "" );
      s.replace( "$(yellow)",      "" );
      s.replace( "$(lightblue)",   "" );
      s.replace( "$(lightpurple)", "" );
      s.replace( "$(lightcyan)",   "" );
      s.replace( "$(white)",       "" );
      s.replace( "$(off)",         "" );
    }
  }

//}:                                              |
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
    wrn += "$(off)";

    //--------------------------------------------------------------------------
    // Translate to CRT escape sequences.
    //--------------------------------------------------------------------------

    string crt = wrn;
    translateCrtColors( crt );
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
//e_errorf:{                                      |

  s32 e_errorf( const u32 code, ccp format,... ){
    va_list va;
    va_start( va, format );
      const s32 r = e_errorv( code, format, va );
    va_end( va );
    return r;
  }

//}:                                              |
//e_errorv:{                                      |

  s32 e_errorv( const u32 code, ccp format, va_list va ){

    //--------------------------------------------------------------------------
    // Format the warning string.
    //--------------------------------------------------------------------------

    string err;
    err += "$(lightred)ERROR $(yellow)";
    err.catv( format, va );
    err += "$(off)";

    //--------------------------------------------------------------------------
    // Translate to CRT escape sequences.
    //--------------------------------------------------------------------------

    string crt = err;
    translateCrtColors( crt );
    IEngine::getStdout() += crt + "\n";

    //--------------------------------------------------------------------------
    // Send to standard out, clear coloration if debuggingg.
    //--------------------------------------------------------------------------

    if( IEngine::isDebugging() ){
      stripCrtColors( err );
    }
    const s32 r = fprintf( stderr, "%s\n", err.c_str() );
  DEBUG_BREAK
    return r;
  }

//}:                                              |
//e_dbgf:{                                        |

  s32 e_dbgf( ccp format, ... ){
    va_list va;
    va_start( va, format );
      const s32 r=( 0 != e_errorv( 999, format, va )|| 1 );
      DEBUG_BREAK
    va_end( va );
    return r;
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
//e_msgf:{                                        |

  s32 e_msgv( ccp format, va_list va ){

    //--------------------------------------------------------------------------
    // Format the warning string.
    //--------------------------------------------------------------------------

    string msg;
    msg.catv( format, va );
    msg += "$(off)";

    //--------------------------------------------------------------------------
    // Translate to CRT escape sequences.
    //--------------------------------------------------------------------------

    string crt = msg;
    translateCrtColors( crt );
    IEngine::getStdout() += crt + "\n";

    //--------------------------------------------------------------------------
    // Send to standard out, clear coloration if debuggingg.
    //--------------------------------------------------------------------------

    if( IEngine::isDebugging() ){
      stripCrtColors( msg );
    }
    return fprintf( stdout, "%s\n", msg.c_str() );
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
    NSLogv([ NSString stringWithUTF8String:format ], va );
    return 1;
  }

//}:                                              |
//e_log:{                                         |

  s32 e_log( ccp msg ){
    if( msg && *msg ){
      NSLog( @"%s", msg );
      return s32( strlen( msg ));
    }
    return 0;
  }

//}:                                              |
//e_brk:{                                         |

  void e_brk( ccp msg ){
    NSLog( @"***************" );
    NSLog( @"*    BREAK    *" );
    NSLog( @"***************" );
    if( msg ){
      NSLog( @"[%u] %s", Thread::tid(), msg );
    }
    if( IEngine::isDebugging() ){
      __builtin_trap();
    }
    if( msg ){
      e_errorf( 999, "[%u] %s", Thread::tid(), msg );
    }
    exit( -1 );
  }

//}:                                              |
//e_hlt:{                                         |

  void e_hlt( ccp msg ){
    NSLog( @"**************" );
    NSLog( @"*    HALT    *" );
    NSLog( @"**************" );
    if( msg ){
      NSLog( @"[%u] %s", Thread::tid(), msg );
    }
    if( IEngine::isDebugging() ){
      __builtin_trap();
    }
    if( msg ){
      e_errorf( 999, "[%u] %s", Thread::tid(), msg );
    }
    exit( -1 );
  }

//}:                                              |
//================================================|=============================
//                                                                 vim:ft=objcpp
