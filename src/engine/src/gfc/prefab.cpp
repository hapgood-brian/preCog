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
//Prefab:{                                        |
  //Extends:{                                     |

    e_extends( Prefab );

  //}:                                            |
  //Methods:{                                     |
    //serialize:{                                 |

      void Prefab::serialize( Writer& fs )const{
        super::serialize( fs );
        fs.version( PREFAB_VERSION );
        fs.write( m_vFiles.size() );
        m_vFiles.foreach(
          [&]( const File& F ){
            fs.pack( F.toName() );
            fs.write( F.toBase() );
            fs.write( F.toSize() );
            fs.write( F.toFlags() );
          }
        );
      }

      s64 Prefab::serialize( Reader& fs ){
        super::serialize( fs );
        fs.version( PREFAB_VERSION );
        const u32 n = fs.read<u32>();
        if( n ){
          m_vFiles.resize( n );
          m_vFiles.alter( 0,
            [&]( File& _1st ){
              File* F = &_1st;
              for( u32 i=0; i<n; ++i ){
                fs.unpack( F->toName() );
                fs.read( F->toBase() );
                fs.read( F->toSize() );
                fs.read( F->toFlags() );
                ++F;
              }
            }
          );
        }
        return UUID;
      }

    //}:                                          |
    //get:{                                       |

      vector<Prefab::handle> Prefab::get( const string& path ){
        vector<Prefab::handle> prefabs;
        IEngine::dir( path,
          [&]( const string& dir, const string& name, const bool isDirectory ){
            if( name.ext().tolower() == ".prefab" ){
              const string& path = dir+name;
              Prefab::handle hPrefab = e_load<Prefab>( path );
              if( hPrefab ){
                Prefab& prefab = hPrefab.cast();
                prefabs.push( hPrefab );
                prefab.setPath( path );
              }
            }
          }
        );
        return prefabs;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
