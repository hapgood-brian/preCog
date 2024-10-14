//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

using namespace EON;
using namespace gfc;

#include<stdio.h>

//================================================+=============================
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
//================================================+=============================
