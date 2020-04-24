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

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Resource:{                                      |
  //Extends:{                                     |

    e_extends( Resource );

  //}:                                            |
  //Operate:{                                     |
    //Assignment:{                                |

      Resource& Resource::operator=( const Resource& lvalue ){
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

      void Resource::preSerialize( Writer& fs )const{
        super::preSerialize( fs );
        if( m_sSHA1.empty() ){
          string sha1;
          sha1.repeat( '0', 40 );
          e_assert( sha1.len() == 40 );
          fs.pack( sha1 );
        }else{
          e_assert( m_sSHA1.len() == 40 );
          fs.pack( m_sSHA1 );
        }
      }

      void Resource::serialize( Writer& fs )const{
        super::serialize( fs );
        fs.version( RESOURCE_VERSION );
        fs.pack( m_sComment );
        fs.pack( m_sName );
        fs.pack( m_sPath );
      }

      void Resource::postSerialize( Writer& fs )const{
        super::postSerialize( fs );
      }

      void Resource::preSerialize( Reader& fs ){
        super::preSerialize( fs );
        fs.unpack( m_sSHA1 );
        #if e_compiling( debug )
          const auto l = m_sSHA1.len();
          e_assert( 40 == l );
        #endif
        e_assert( !m_sSHA1.empty() );
      }

      s64 Resource::serialize( Reader& fs ){

        super::serialize( fs );

        //----------------------------------------------------------------------
        // Handle versioning appropriately.
        //----------------------------------------------------------------------

        fs.version( RESOURCE_VERSION );

        //----------------------------------------------------------------------
        // Stream in the names of this resource.
        //----------------------------------------------------------------------

        string comment;
        fs.unpack( comment );
        if( m_sComment.empty() && !comment.empty() ){
          m_sComment = comment;
        }
        string name;
        fs.unpack( name );
        if( m_sName.empty() && !name.empty() ){
          m_sName = name;
        }
        string path;
        fs.unpack( path );
        if( m_sPath.empty() && !path.empty() ){
          m_sPath = path;
        }

        //----------------------------------------------------------------------
        // Stream in the path of this resource.
        //----------------------------------------------------------------------

        if( m_sName.empty() ){
          if( m_sPath.empty() ){
            m_sName = string::resourceId();
          }else{
            m_sName = m_sPath.basename();
          }
        }
        if( m_sPath.empty() ){
          m_sPath = IEngine::toPackagePath();
        }
        return UUID;
      }

      void Resource::postSerialize( Reader& fs ){
        super::postSerialize( fs );
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
