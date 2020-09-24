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

#undef e_memory_validator

using namespace EON;
using namespace gfc;

#include<stdio.h>
#include<mutex>

#if e_compiling( profiler )

//================================================|=============================
//BlockProfiler:{                                 |
  //Private:{                                     |

    namespace{
      __thread BlockProfiler* __up = nullptr;
      std::mutex m;
    }

  //}:                                            |
  //Methods:{                                     |
    //printInfo:{                                 |

      void BlockProfiler::printInfo( const data& d, const s32 cTabs ){

        const s32 tabs = cTabs + 1;

        //----------------------------------------------------------------------
        // Figure out the inclusive and exclusive timings.
        //----------------------------------------------------------------------

        double inclusiveTime = 0.f;
        d.m_vChildren.foreach( [&]( const data& c ){
          inclusiveTime+=( c.m_fStop - c.m_fStart );
        });
        double calleeTime = 0.f;
        d.m_vCallees.foreach( [&]( const data& c ){
          calleeTime+=( c.m_fStop - c.m_fStart );
        });
        double exclusiveTime=( d.m_fStop - d.m_fStart );
        exclusiveTime -= inclusiveTime;
        exclusiveTime -= calleeTime;

        //----------------------------------------------------------------------
        // Render timings for self.
        //----------------------------------------------------------------------

        const u32 tid = Thread::tid();
        string spaces;
        for( u32 i=0; i<u32( tabs ); ++i ){
          spaces += "| ";
        }
        if( f32( exclusiveTime*1e-9f ).floor() > 0.f ){
          e_msgf( "%6u%s%.4fs : \"%s\"", tid, spaces.c_str(), exclusiveTime*1e-9f, d.m_sInfo.c_str() );
        }else if( f32( exclusiveTime*1e-6f ).floor() > 0.f ){
          e_msgf( "%6u%s%.4fms : \"%s\"", tid, spaces.c_str(), exclusiveTime*1e-6f, d.m_sInfo.c_str() );
        }else if( f32( exclusiveTime*.001f ).floor() > 0.f ){
          e_msgf( "%6u%s%.4fus : \"%s\"", tid, spaces.c_str(), exclusiveTime*.001f, d.m_sInfo.c_str() );
        }else{
          e_msgf( "%6u%s%.4fns : \"%s\"", tid, spaces.c_str(), exclusiveTime, d.m_sInfo.c_str() );
        }

        //----------------------------------------------------------------------
        // Render timings for children.
        //----------------------------------------------------------------------

        if( !d.m_vChildren.empty() ){
          if( f32( inclusiveTime*1e-9f ).floor() > 0.f ){
            e_msgf( "%6u%s| Children (%.4fs)", tid, spaces.c_str(), inclusiveTime*1e-9f );
          }else if( f32( inclusiveTime*1e-6f ).floor() > 0.f ){
            e_msgf( "%6u%s| Children (%.4fms)", tid, spaces.c_str(), inclusiveTime*1e-6f );
          }else if( f32( inclusiveTime*.001f ).floor() > 0.f ){
            e_msgf( "%6u%s| Children (%.4fus)", tid, spaces.c_str(), inclusiveTime*.001f );
          }else{
            e_msgf( "%6u%s| Children (%.4fns)", tid, spaces.c_str(), inclusiveTime );
          }
          for( u32 n=d.m_vChildren.size(), i=0; i<n; ++i ){
            printInfo( d.m_vChildren[ i ], tabs+1 );
          }
        }

        //----------------------------------------------------------------------
        // Render timings for callees.
        //----------------------------------------------------------------------

        if( !d.m_vCallees.empty() ){
          if( f32( calleeTime*1e-9f ).floor() > 0.f ){
            e_msgf( "%6u%s| Callees (%.4fs)", tid, spaces.c_str(), calleeTime*1e-9f );
          }else if( f32( calleeTime*1e-6f ).floor() > 0.f ){
            e_msgf( "%6u%s| Callees (%.4fms)", tid, spaces.c_str(), calleeTime*1e-6f );
          }else if( f32( calleeTime*.001f ).floor() > 0.f ){
            e_msgf( "%6u%s| Callees (%.4fus)", tid, spaces.c_str(), calleeTime*.001f );
          }else{
            e_msgf( "%6u%s| Callees (%.4fns)", tid, spaces.c_str(), calleeTime );
          }
          for( u32 n=d.m_vCallees.size(), i=0; i<n; ++i ){
            printInfo( d.m_vCallees[ i ], tabs+1 );
          }
        }
      }

    //}:                                          |
    //enabled:{                                   |

      bool BlockProfiler::enabled(){
        return IEngine::Cvar::get<bool>( "--profile=on" );
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    BlockProfiler::BlockProfiler( ccp pFunction, ccp format,... )
        : m_pFunction( pFunction )
        , m_pParent( __up ){
      e_memory_validator();
      __up = this;
      if( !enabled() ){
        return;
      }
      va_list va;
      va_start( va, format );
        const double start = e_nanoseconds();
        m_pData = new( m_aData )data;
        m_pData->m_sInfo.catv( format, va );
        m_pData->m_sInfo += " ";
        m_pData->m_sInfo += pFunction;
        const double stops = e_nanoseconds();
        m_pData->m_fStart = stops-( stops-start );
      va_end( va );
    }

  //}:                                            |
  //Dtor:{                                        |

    BlockProfiler::~BlockProfiler(){
      if( __up == this ){
        __up = m_pParent;
      }
      if( !enabled() ){
        return;
      }
      if( m_pData ){
        m_pData->m_fStop = e_nanoseconds();
        if( m_pParent ){
          if( strcmp( m_pFunction, m_pParent->m_pFunction )){
            m_pParent->m_pData->m_vChildren.push( std::move( *m_pData ));
          }else{
            m_pParent->m_pData->m_vCallees.push( std::move( *m_pData ));
          }
        }else{
          std::lock_guard<std::mutex>__guard( m );
          printInfo( *m_pData, 0 );
        }
        m_pData->~data();
        e_memory_validator();
        return;
      }
      e_unreachable( "that's just wrong" );
    }

  //}:                                            |
//}:                                              |
//================================================|=============================

#endif
