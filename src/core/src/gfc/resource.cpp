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
using namespace fs;

//================================================+=============================
//Stream:{                                        |
  //Extends:{                                     |

#ifdef __APPLE__
  #pragma mark - Stream -
#endif

    e_extends( Stream );

  //}:                                            |
  //Operate:{                                     |
    //Assignment:{                                |

      Stream& Stream::operator=( const Stream& lvalue ){
        super::operator=( static_cast<const super&>( lvalue ));
        m_sComment = lvalue.m_sComment;
        m_sName    = lvalue.m_sName;
        m_sSHA1    = lvalue.m_sSHA1;
        m_sPath    = lvalue.m_sPath;
        return *this;
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //serialize:{                                 |

      /* Write streams */

      void Stream::preSerialize( Writer& fs )const{
        super::preSerialize( fs );
        fs.pack( m_sComment );
        fs.pack( m_sPath );
        fs.pack( m_sName );
        fs.pack( m_sSHA1 );
      }

      void Stream::serialize( Writer& fs )const{
        super::serialize( fs );
        fs.version( STREAM_VERSION );
      }

      void Stream::postSerialize( Writer& fs )const{
        super::postSerialize( fs );
      }

      /* Read streams */

      void Stream::preSerialize( Reader& fs ){
        super::preSerialize( fs );

        //----------------------------------------------------------------------
        // Read stream text fields.
        //----------------------------------------------------------------------

        m_sComment = fs.unpack();
        m_sPath    = fs.unpack();
        m_sName    = fs.unpack();
        m_sSHA1    = fs.unpack();

        //----------------------------------------------------------------------
        // Display tracing.
        //----------------------------------------------------------------------

        if( e_getCvar( bool, "USE_TRACING" )){
          e_msgf( "%s: ID:'%s' SHA1:$(green)%s"
            , classof()
            , ccp( m_sName )
            , ccp( m_sSHA1 )
          );
        }
      }

      s64 Stream::serialize( Reader& fs ){
        super::serialize( fs );
        fs.version( STREAM_VERSION );
        return UUID;
      }

      void Stream::postSerialize( Reader& fs ){
        super::postSerialize( fs );
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    Stream::Stream( const string& sName ){
      m_sName = sName;
      m_sSHA1.repeat( 40, '0' );
    }

    Stream::Stream( string&& sName ){
      m_sName = std::move( sName );
      m_sSHA1.repeat( 40, '0' );
    }

    Stream::Stream( const Stream& r ){
      m_sName = r.m_sName;
      m_sSHA1 = r.m_sSHA1;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
