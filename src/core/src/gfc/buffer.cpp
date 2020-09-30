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

//================================================|=============================
//lockable:{                                      |
  //Operate:{                                     |
    //operator=:{                                 |

#ifdef __APPLE__
  #pragma mark - Lockable stream -
#endif

      lockable& lockable::operator=( lockable&& rvalue ){
        if( &rvalue != this ){
          m_uMaximum = rvalue.m_uMaximum;
          m_uStride = rvalue.m_uStride;
          m_uCount = rvalue.m_uCount;
          m_uTail = rvalue.m_uTail;
          m_pData = rvalue.m_pData;
          e_assert( m_uStride );
          rvalue.m_uMaximum = 0;
          rvalue.m_uCount   = 0;
          rvalue.m_uTail    = 0;
          rvalue.m_pData    = nullptr;
        }
        return *this;
      }

      lockable& lockable::operator=( const lockable& lvalue ){
        if( &lvalue != this ){
          unlock();
          m_uMaximum = lvalue.m_uMaximum;
          m_uStride = lvalue.m_uStride;
          m_uCount = lvalue.m_uCount;
          m_uTail = lvalue.m_uTail;
          e_assert( m_uStride );
          if( lvalue.m_pData ){
            memcpy( alloc( m_uCount ), lvalue.m_pData, bytes() );
          }
        }
        return *this;
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //realloc:{                                   |

      cp lockable::realloc( const u64 deltaBytes ){
        if( !m_uStride ){
          return nullptr;
        }
        const auto nextThreshold = ( m_uTail + deltaBytes ) / m_uStride;
        if( nextThreshold > m_uMaximum ){
          e_errorf( 736363
            , "Can't grow beyond our bounds! Increase capacity earlier: %llu (%llu) bytes out of %llu!"
            , m_uTail + deltaBytes
            , deltaBytes
            , avail() );
          return nullptr;
        }
        if( !m_pData ){
          m_pData = cp( e_malloc( bytes() ));
        }
        cp pBlk = m_pData + m_uTail;
        m_uTail += deltaBytes;
        return pBlk;
      }

    //}:                                          |
    //alloc:{                                     |

      cp lockable::alloc( const u64 bytes ){
        reset();
        return realloc( bytes );
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    lockable::lockable( lockable&& rvalue )
        : m_uMaximum( rvalue.m_uMaximum )
        , m_uStride(  rvalue.m_uStride  )
        , m_uCount(   rvalue.m_uCount   )
        , m_uTail(    rvalue.m_uTail    )
        , m_pData(    rvalue.m_pData    ){
      e_assert( m_uStride );
      rvalue.m_uMaximum = 0;
      rvalue.m_uCount   = 0;
      rvalue.m_uTail    = 0;
      rvalue.m_pData    = nullptr;
    }

    lockable::lockable( const lockable& lvalue )
        : m_uMaximum( lvalue.m_uMaximum )
        , m_uStride(  lvalue.m_uStride  )
        , m_uCount(   lvalue.m_uCount   )
        , m_uTail(    lvalue.m_uTail    ){
      e_assert( m_uStride );
      if( lvalue.m_pData ){
        m_pData = cp( e_malloc( bytes() ));
        memcpy( m_pData, lvalue.m_pData, bytes() );
      }
    }

    lockable::lockable( const u64 stride, const u64 cap )
        : m_uMaximum( cap )
        , m_uStride( stride )
        , m_uCount( cap ){
      e_assert( stride );
    }

  //}:                                            |
  //Dtor:{                                        |

    lockable::~lockable(){
      if( !bGpuMem ){
        e_free( m_pData );
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//buffer:{                                        |
  //Operate:{                                     |
    //operator=:{                                 |

#ifdef __APPLE__
  #pragma mark - Buffer -
#endif

      buffer& buffer::operator=( buffer&& rvalue ){
        if( &rvalue != this ){
          m_bDonated = rvalue.m_bDonated;
          rvalue.m_bDonated = 0;
          m_uStride = rvalue.m_uStride;
          rvalue.m_uStride = 0;
          m_uSize = rvalue.m_uSize;
          rvalue.m_uSize = 0;
          m_pData = rvalue.m_pData;
          rvalue.m_pData = nullptr;
        }
        return *this;
      }

      buffer& buffer::operator=( const buffer& b ){
        if( &b == this ){
          return *this;
        }
        if( b.m_pData != m_pData ){
          m_bDonated = b.m_bDonated;
          if(( m_uSize != b.m_uSize )||( m_uStride != b.m_uStride )){
            m_uStride = b.m_uStride;
            m_uSize   = b.m_uSize;
            alloc( m_uSize );
          }
          memcpy( m_pData, b.m_pData, bytes() );
        }
        return *this;
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //clearDataOnly:{                             |

      void buffer::clearDataOnly(){
        if( !m_bDonated ){
          e_free( m_pData );
        }
        m_pData = nullptr;
        m_bDonated = 0;
      }

    //}:                                          |
    //realloc:{                                   |

      bool buffer::realloc( const u32 size ){
        m_uSize = size;
        if( !m_bDonated ){
          m_pData = e_realloc( m_pData, bytes() );
          return true;
        }
        return false;
      }

    //}:                                          |
    //alloc:{                                     |

      void buffer::alloc( const u32 size ){
        clear();
        realloc( size );
      }

    //}:                                          |
    //clear:{                                     |

      void buffer::clear(){
        clearDataOnly();
        m_uSize = 0;
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    buffer::buffer( const buffer& b )
        : m_uSize( b.m_uSize )
        , m_uStride( b.m_uStride ){
      const u64 _bytes = bytes();
      if( b.m_bDonated ){
        m_pData = b.m_pData;
        m_bDonated = 1;
      }else{
        m_pData = e_malloc( _bytes );
        if( b.m_pData ){
          memcpy( m_pData, b.m_pData, _bytes );
        }
      }
    }

    buffer::buffer( buffer&& rvalue )
        : m_pData( rvalue.m_pData )
        , m_uSize( rvalue.m_uSize )
        , m_uStride( rvalue.m_uStride ){
      rvalue.m_uStride = 0;
      rvalue.m_uSize = 0;
      rvalue.m_pData = 0;
    }

    buffer::buffer( const u32 stride, const u32 size )
        : m_uSize( size )
        , m_uStride( stride ){
      m_pData = e_malloc( bytes() );
    }

    buffer::buffer( const u32 size )
        : m_uSize( size ){
      m_pData = e_malloc( bytes() );
    }

  //}:                                            |
  //Dtor:{                                        |

    buffer::~buffer(){
      clear();
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//stream:{                                        |
  //Operate:{                                     |
    //operator=:{                                 |

#ifdef __APPLE__
  #pragma mark - Stream -
#endif

      stream& stream::operator=( stream&& rvalue ){
        e_guardw( m_tLock );
        if( &rvalue != this ){
          m_aRefs.store( rvalue.m_aRefs.load() );
          rvalue.m_aRefs.store( 0 );
          m_uCapacity.store( rvalue.m_uCapacity.load() );
          rvalue.m_uCapacity.store( 0 );
          m_uStride.store( rvalue.m_uStride.load() );
          rvalue.m_uStride.store( 0 );
          e_assert( m_uStride );
          m_uSize.store( rvalue.m_uSize.load() );
          rvalue.m_uSize.store( 0 );
          if( !bDonated &&( m_pData != rvalue.m_pData )){
            e_free( m_pData );
          }
          m_pData = rvalue.m_pData;
          rvalue.m_pData = nullptr;
          all = rvalue.all;
          rvalue.all = 0;
        }
        e_sanity_check( !e_isbad( m_pData ));
        return *this;
      }

      stream& stream::operator=( const stream& lvalue ){
        e_guardr( lvalue.m_tLock );
        e_guardw( m_tLock );
        if( &lvalue == this ){
          return *this;
        }
        if( lvalue.empty() ){
          clear();
          return *this;
        }
        if( lvalue.m_pData != m_pData ){
          cp pBuffer = nullptr;
          if(( m_uStride != lvalue.m_uStride )||( m_uSize < lvalue.m_uSize )|| !m_pData ){
            // Assign the stride ready for allocating buffer.
            e_assert( lvalue.m_uStride, "Bad stride!" );
            m_uStride.store( lvalue.m_uStride.load() );
            // Allocate buffer so large enough for memcpy.
            pBuffer = alloc( lvalue.m_uSize );
            e_assert( pBuffer, "Dim alloc!" );
          }else{
            m_uSize.store( lvalue.m_uSize.load() );
            pBuffer = m_pData;
          }
          if( lvalue.m_pData ){
            memcpy( pBuffer, lvalue.m_pData, lvalue.bytes()+1 );
          }
          m_aRefs.store( lvalue.m_aRefs.load() );
          all = lvalue.all;
          bDonated = 0;
        }
        e_sanity_check( !e_isbad( m_pData ));
        return *this;
      }

    //}:                                          |
    //operator<<:{                                |

      stream& stream::operator<<( const string& s ){
        const auto n = s.len();
        if( n ){
          cp ptr = realloc( n );
          if( ptr ){
            memcpy( ptr, s.c_str(), n );
          }
        }
        return *this;
      }

      stream& stream::operator<<( ccp s ){
        if( s && *s ){
          const auto n = strlen( s );
          if( n ){
            cp ptr = realloc( n );
            if( ptr ){
              memcpy( ptr, s, n );
            }
          }
        }
        return *this;
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //clearDataOnly:{                             |

      void stream::clearDataOnly(){
        e_guardw( m_tLock );
        e_sanity_check( !e_isbad( m_pData ));
        if( !bDonated ){
          e_free( m_pData );
        }
        m_pData = nullptr;
        m_uCapacity = 0;
        m_uSize = 0;
      }

    //}:                                          |
    //realloc:{                                   |

      cp stream::realloc( const u64 deltaSize ){
        if( deltaSize ){
          e_sanity_check( !e_isbad( m_pData ));
          e_assert( deltaSize );
          if( !m_uStride ){
            // Fixes bug in std::vector using std::move that zeros out stride!
            m_uStride = 1;
          }
          const u64 cap = m_uSize + deltaSize + 1;
          if( cap > m_uCapacity ){
            m_uCapacity = ( cap + PARAGRAPH - 1 ) & ~( PARAGRAPH - 1 );
            if( !bDonated ){
              m_pData = e_realloc( m_pData, m_uCapacity * m_uStride );
            }else{
              cp pTemporary = e_malloc( m_uCapacity * m_uStride );
              memcpy( pTemporary, m_pData, bytes() );
              m_pData = pTemporary;
              bDonated = 0;
            }
            const auto count = m_uCapacity - m_uSize;
            if( count ){
              memset( m_pData + bytes(), 0, count * m_uStride );
            }
          }
        }
        cp pBlk = m_pData + bytes();
        if( !pBlk ){
          e_brk( "null result" );
        }
        m_uSize += deltaSize;
        return pBlk;
      }

    //}:                                          |
    //alloc:{                                     |

      cp stream::alloc( const u64 size ){
        e_guardw( m_tLock );
        clear();
        return realloc( size );
      }

    //}:                                          |
    //clear:{                                     |

      void stream::clear(){
        e_guardw( m_tLock );
        if( !bDonated ){
          e_free( m_pData );
        }
        m_pData = nullptr;
        m_uCapacity = 0;
        m_uSize = 0;
      }

    //}:                                          |
    //skip:{                                      |

      bool stream::skip( const u64 items ){
        e_guardw( m_tLock );
        if( m_uSize + items > m_uCapacity ){
          return false;
        }
        m_uSize += items;
        return true;
      }

    //}:                                          |
    //data:{                                      |

      bool stream::query( const u64 i, const std::function<void( ccp )>& lambda )const{
        e_guardr( m_tLock );
        if( i > m_uCapacity ){
          if( e_getCvar( bool, "USE_TRACING" )){
            e_msgf(
              "$(red)ERROR$(out) %llu > capacity (%llu)"
              , i
              , m_uCapacity.load()
            );
          }
          return false;
        }
        if( !m_pData ){
          return false;
        }
        if( !lambda ){
          return false;
        }
        e_sanity_check( !e_isbad( m_pData ));
        lambda( m_pData + i * m_uStride );
        return true;
      }

      bool stream::query( const std::function<void( ccp )>& lambda )const{
        e_guardr( m_tLock );
        if( !m_pData ){
          return false;
        }
        if( !lambda ){
          return false;
        }
        e_sanity_check( !e_isbad( m_pData ));
        lambda( m_pData );
        return true;
      }

      bool stream::alter( const std::function<void( cp )>& lambda ){
        e_guardr( m_tLock );
        if( !m_pData ){
          return false;
        }
        if( !lambda ){
          return false;
        }
        e_sanity_check( !e_isbad( m_pData ));
        lambda( m_pData );
        return true;
      }

    //}:                                          |
    //end:{                                       |

      void stream::end( const std::function<void( ccp )>& lambda )const{
        e_guardr( m_tLock );
        if( !m_pData ){
          return;
        }
        e_sanity_check( !e_isbad( m_pData ));
        if( lambda ){
          lambda( m_pData + bytes() );
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    stream::stream( stream&& rvalue )
        : all( rvalue.all )
        , m_uCapacity( rvalue.m_uCapacity.load() )
        , m_uStride( rvalue.m_uStride.load() )
        , m_uSize( rvalue.m_uSize.load() )
        , m_pData( rvalue.m_pData ){
      bDonated = rvalue.bDonated;
      rvalue.m_uCapacity = 0;
      rvalue.m_uStride   = 0;
      rvalue.m_uSize     = 0;
      rvalue.m_pData     = nullptr;
      rvalue.bDonated    = 0;
      rvalue.all         = 0;
      e_sanity_check( !e_isbad( m_pData ));
      e_assert( m_uStride );
    }

    stream::stream( const stream& lvalue )
        : all( lvalue.all )
        , m_uCapacity( lvalue.m_uCapacity.load() )
        , m_uStride( lvalue.m_uStride.load() )
        , m_uSize( lvalue.m_uSize.load() ){
      e_assert( m_uStride );
      if( lvalue.m_pData ){
        m_pData = e_malloc( m_uCapacity * m_uStride );
        memcpy(
            m_pData
          , lvalue.m_pData
          , m_uCapacity * m_uStride
        );
      }
    }

    stream::stream( cp pBuffer, const u64 stride, const u64 capacity )
        : m_uCapacity( capacity )
        , m_uStride( stride )
        , m_uSize( capacity )
        , m_pData( pBuffer ){
      e_sanity_check( !e_isbad( m_pData ));
      e_assert( stride );
      bDonated = 1;
    }

  //}:                                            |
  //Dtor:{                                        |

    stream::~stream(){
      clear();
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
