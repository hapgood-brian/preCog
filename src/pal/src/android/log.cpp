//------------------------------------------------------------------------------
//          Copyright 2014 RED Engine Games, LLC. All rights reserved.
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

#if e_compiling( debug )

#include<android/log.h>

static __thread char s_aBuffer[4096];

using namespace gfc;

//================================================|=============================
//debug:{                                         |
  //logf:{                                        |

    int debug::logf( ccp tag, ccp format,... ){
      if( format ){
        va_list va;
        va_start( va, format );
        vsprintf( s_aBuffer, format, va );
        log( tag, s_aBuffer );
        va_end( va );
        return 1;
      }
      return 0;
    }

  //}:                                            |
  //log:{                                         |

    int debug::log( ccp _tag_, ccp msg ){
      if( msg && *msg ){
        if( _tag_ && *_tag_ ){
          __android_log_print( ANDROID_LOG_DEBUG, _tag_, "[%u] %s", Thread::tid(), msg );
        }else{
          __android_log_print( ANDROID_LOG_DEBUG, "RED", "[%u] %s", Thread::tid(), msg );
        }
        return 1;
      }
      return 0;
    }

  //}:                                            |
  //brk:{                                         |

    int debug::brk( ccp tag, ccp msg ){
      __android_log_write( ANDROID_LOG_ERROR, "", "***************" );
      __android_log_write( ANDROID_LOG_ERROR, "", "*    BREAK    *" );
      __android_log_write( ANDROID_LOG_ERROR, "", "***************" );
      log( tag, msg );
      __builtin_trap();
      return 1;
    }

  //}:                                            |
  //halt:{                                        |

    int debug::halt( ccp tag, ccp msg ){
      __android_log_write( ANDROID_LOG_ERROR, "", "**************" );
      __android_log_write( ANDROID_LOG_ERROR, "", "*    HALT    *" );
      __android_log_write( ANDROID_LOG_ERROR, "", "**************" );
      log( tag, msg );
      __builtin_trap();
      return 1;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================

#endif
