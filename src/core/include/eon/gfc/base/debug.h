//------------------------------------------------------------------------------
//       Copyright 2014-2020 Creepy Doll Games LLC. All rights reserved.
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

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup base
  *   @{
  *     \addtogroup macros
  *     @{
  */

//================================================+=============================
//Debug macros:{                                  |
  //DEBUG_ASSERT:{                                |
    //Microsoft compilers:{

      #if e_compiling( microsoft )
        #define e_assert( x, ... )                                              \
          DEBUG_ONLY(                                                           \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
        #define e_sanity_check( x, ... )                                        \
          SANITY_ONLY(                                                          \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
	  #define DEBUG_BREAK                                                       \
		DEBUG_ONLY( DebugBreak(); )
      #endif

    //}:                                          |
    //Google compilers:{                          |

      #if e_compiling( android )
        #define DEBUG_BREAK                                                     \
          __builtin_trap();
        #define e_assert( x, ... )                                              \
          DEBUG_ONLY(                                                           \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
        #define e_sanity_check( x, ... )                                        \
          SANITY_ONLY(                                                          \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
      #endif

    //}:                                          |
    //Apple compilers:{                           |

      #if e_compiling( ios )||e_compiling( osx )
        #if e_compiling( debug )
          #define DEBUG_BREAK                                                   \
            __builtin_trap();
        #else
          #define DEBUG_BREAK
        #endif
        #define e_assert( x, ... )                                              \
          DEBUG_ONLY(                                                           \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
        #define e_sanity_check( x, ... )                                        \
          SANITY_ONLY(                                                          \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
      #endif

    //}:                                          |
    //Linux compilers:{                           |

      #if e_compiling( linux ) || e_compiling( web )
        #if e_compiling( debug )
          #define DEBUG_BREAK                                                   \
            __builtin_trap();
        #else
          #define DEBUG_BREAK
        #endif
        #define e_assert( x, ... )                                              \
          DEBUG_ONLY(                                                           \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
        #define e_sanity_check( x, ... )                                        \
          SANITY_ONLY(                                                          \
            (void)((x)||!e_dbgf(                                                \
                __FILE__ "(" e_2str(__LINE__) "): " e_2str( x )                 \
                "\n" __VA_ARGS__)))
      #endif

    //}:                                          |
  //}:                                            |
  //SANITY_ONLY:{                                 |

    #if e_compiling( sanity )
      #define SANITY_ONLY(x) x
    #else
      #define SANITY_ONLY(x)
    #endif

  //}:                                            |
  //DEBUG_ONLY:{                                  |

    #if e_compiling( debug )
      #define DEBUG_ONLY(x) x
    #else
      #define DEBUG_ONLY(x)
    #endif

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Interface:{                                     |

  #ifdef __cplusplus

    extern"C"{

      E_PUBLISH EON::s32 e_warnsv( EON::ccp, va_list );
      E_PUBLISH EON::s32 e_msgv  ( EON::ccp, va_list );
      E_PUBLISH EON::s32 e_logv  ( EON::ccp, va_list );
      E_PUBLISH EON::s32 e_warnsf( EON::ccp, ... );
      E_PUBLISH EON::s32 e_msgf  ( EON::ccp, ... );
      E_PUBLISH EON::s32 e_logf  ( EON::ccp, ... );
      E_PUBLISH EON::s32 e_msg   ( EON::ccp );
      E_PUBLISH EON::s32 e_log   ( EON::ccp );
      E_PUBLISH void     e_break ( EON::ccp );
      E_PUBLISH void     e_breakf( EON::ccp );

      #define e_longlogf( X,... ) e_logf( __FILE__ "(" e_2str(__LINE__) "): " X,##__VA_ARGS__ )
      #define e_longlog(  X     ) e_logf( __FILE__ "(" e_2str(__LINE__) "): " X )
      #define e_logf(     X,... ) e_logf( X, ##__VA_ARGS__ )

      /** \brief An unreachable statement.
        *
        * This routine will display "unreachable" message and exit immediately.
        */

      #if e_compiling( debug )
        #define e_unreachablef( format,... )                                      \
          e_dbgf(__FILE__ "(" e_2str(__LINE__) "): " format, ##__VA_ARGS__);      \
          DEBUG_BREAK																                              \
          ::exit( -1 )
        #define e_unreachable( text )                                             \
          e_dbgf(__FILE__ "(" e_2str(__LINE__) "): " text );                      \
          DEBUG_BREAK																                              \
          ::exit( -1 )
      #else
        #define e_unreachablef( format,... )                                      \
          e_break(__FILE__ "(" e_2str(__LINE__) "): " format, ##__VA_ARGS__);     \
          ::exit( -1 )
        #define e_unreachable( text )                                             \
          e_break(__FILE__ "(" e_2str(__LINE__) "): " text );                     \
          ::exit( -1 )
      #endif
    }

  #endif

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
