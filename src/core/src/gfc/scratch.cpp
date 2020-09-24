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

using namespace EON;
using namespace gfc;

#include<stdio.h>

//================================================|=============================
//Scratch:{                                       |
  //Methods:{                                     |
    //alloc:{                                     |

      cp Scratch::alloc( u32 uSize, const u32 cAlignment, const u32 cPad ){
        ccp pEnd = m_tBuffer.end();
        e_assert( m_vOffset.m_cp + uSize <= pEnd );
        if( cAlignment > 0 ){
          if( sizeof( cp )==8 ){
            m_vOffset.m_u64 += u64( cAlignment-1 );
            m_vOffset.m_u64 &=~u64( cAlignment-1 );
          }else{
            m_vOffset.m_u32 += (cAlignment-1);
            m_vOffset.m_u32 &=~(cAlignment-1);
          }
        }
        cp result = m_vOffset.m_cp;
        e_assert( result != 0 );
        if( cPad > 0 ){
          uSize = (uSize + cPad - 1) & ~(cPad - 1);
        }
        m_vOffset.m_cp += uSize;
        if( m_vOffset.m_cp > pEnd ){
          u32 overBy = u32( m_vOffset.m_cp-pEnd );
          printf( "    Over the limit by %u bytes (%uM or %uK)!",
              overBy, overBy/1048576, overBy/1024 );
          DEBUG_BREAK
          return 0;
        }
        return result;
      }

    //}:                                          |
    //init:{                                      |

      void Scratch::init( const u32 cSize ){
        m_tBuffer.alloc( cSize );
        m_vOffset.m_vp = m_tBuffer.data();
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    Scratch::Scratch( const u32 cSize )
        : m_tBuffer( cSize ){
      if( cSize ){
        m_vOffset.m_vp = m_tBuffer.data();
      }else{
        m_vOffset.m_vp = 0;
      }
    }

    Scratch::Scratch()
        : m_tBuffer( 0 ){
      m_vOffset.m_vp = 0;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
