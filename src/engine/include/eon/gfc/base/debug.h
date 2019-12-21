//------------------------------------------------------------------------------
//       Copyright 2014-2019 Creepy Doll Games LLC. All rights reserved.
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

//================================================|=============================
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
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Interface:{                                     |

  /** \brief Emit error with va_list.
    *
    * This routine will output an error and continue on it's merry way. If the
    * debugger is running this code then a trap will be emitted.
    *
    * \param codeError The error code to print to standard out and the engine
    * console window.
    *
    * \param format The formatted text.
    */

  EON::s32 e_errorv( const EON::u32 codeError, EON::ccp format, va_list );

  /** \brief Emit error with arguments.
    *
    * This routine will output an error and continue on it's merry way. If the
    * debugger is running this code then a trap will be emitted.
    *
    * \param codeError The error code to print to standard out and the engine
    * console window.
    *
    * \param format The formatted text.
    */

  EON::s32 e_errorf( const EON::u32 codeError, EON::ccp format, ... );
  EON::s32 e_warnsv( EON::ccp format, va_list );
  EON::s32 e_msgv  ( EON::ccp format, va_list );
  EON::s32 e_logv  ( EON::ccp format, va_list );
  EON::s32 e_warnsf( EON::ccp format, ... );
  EON::s32 e_msgf  ( EON::ccp format, ... );
  EON::s32 e_logf  ( EON::ccp format, ... );
  EON::s32 e_log   ( EON::ccp );

  /** \brief Debug this line.
    *
    * This routine is used by the debug assertion system to break at the line
    * of code in question. It is forced inline on Xcode always but in debug
    * MSVC you will trap here instead of on the line.
    *
    * \param format The text to be outputted to standard error just
    * before the application breaks or shuts down. It may NOT have formatting
    * tokens of the form $(lightblue)...$(off) in it.
    */

  EON::s32 e_dbgf( EON::ccp format, ... );

  /** \brief Software breakpoint.
    *
    * This routine does not return. It fires a software breakpoint if IEngine
    * isDebugging() returns true (Xcode or MSVC are running the process). If
    * not in the debugger the application will terminate. Either way it will
    * always output the text provided.
    *
    * \param termination_text The text to be outputted to standard error just
    * before the application breaks or shuts down. It may NOT have formatting
    * tokens of the form $(lightblue)...$(off) in it.
    */

  e_noreturn void e_brk( EON::ccp termination_text );

  /** \brief Halt execution.
    *
    * This is the same as e_brk() but instead of firing a breakpoint off it'll
    * simply terminate by calling exit().
    *
    * \param termination_text The text to be outputted to standard error just
    * before the application breaks or shuts down. It may NOT have formatting
    * tokens of the form $(lightblue)...$(off) in it.
    */

  e_noreturn void e_hlt( EON::ccp termination_text );

  #define e_longlogf( X,... ) e_logf( __FILE__ "(" e_2str(__LINE__) "): " X,##__VA_ARGS__ )
  #define e_longlog(  X     ) e_logf( __FILE__ "(" e_2str(__LINE__) "): " X )
  #define e_logf(     X,... ) e_logf( X, ##__VA_ARGS__ )
  #define e_hlt(      X     ) e_hlt( X )
  #define e_brk(      X     ) e_brk(  X )

  /** \brief An unreachable statement.
    *
    * This routine will display "unreachable" message and exit immediately.
    */

  #if e_compiling( debug )
    #define e_unreachablef( format,... )                                        \
      e_dbgf(__FILE__ "(" e_2str(__LINE__) "): " format, ##__VA_ARGS__);        \
      DEBUG_BREAK																\
      ::exit( -1 )
    #define e_unreachable( text )                                               \
      e_dbgf(__FILE__ "(" e_2str(__LINE__) "): " text );                        \
      DEBUG_BREAK																\
      ::exit( -1 )
  #else
    #define e_unreachablef( format,... )                                        \
      e_dbgf(__FILE__ "(" e_2str(__LINE__) "): " format, ##__VA_ARGS__);        \
      ::exit( -1 )
    #define e_unreachable( text )                                               \
      e_dbgf(__FILE__ "(" e_2str(__LINE__) "): " text );                        \
      ::exit( -1 )
  #endif

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
