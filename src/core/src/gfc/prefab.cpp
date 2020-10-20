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

//================================================|=============================
//Prefab:{                                        |
  //Extends:{                                     |

    e_extends( Prefab );

  //}:                                            |
  //Methods:{                                     |
    //serialize:{                                 |

      void Prefab::serialize( Writer& fs )const{
        const auto useTracing = e_getCvar( bool, "USE_PREFAB_TRACING" );
        super::serialize( fs );
        fs.version( PREFAB_VERSION );
        fs.write<u32>( m_vFiles.size() );
        m_vFiles.foreach(
          [&]( const File::handle& F ){
            if( useTracing ){
              e_msgf(
                "  to prefab: \"%s\" at base:size %llu:%llu"
                , ccp( F->toName() )
                , F->toBase()
                , F->toSize()
              );
            }
            fs.write( F->toBase()  );
            fs.write( F->toSize()  );
            fs.write( F->toFlags() );
            fs.pack(  F->toName()  );
          }
        );
        const_cast<self*>( this )->m_uBase = fs.toStream().tell();
        fs.write<u64>( m_uBase );
      }

      s64 Prefab::serialize( Reader& fs ){
        const auto useTracing = e_getCvar( bool, "USE_PREFAB_TRACING" );
        super::serialize( fs );
        fs.version( PREFAB_VERSION );
        const auto n = fs.read<u32>();
        if( useTracing ){
          e_msgf( "PrefabFileCount: %u", n );
        }
        if( n ){
          File F;
          for( u32 i=0; i<n; ++i ){
            fs.read( F.toBase() );
            fs.read( F.toSize() );
            fs.read( F.toFlags() );
            fs.unpack( F.toName() );
            if( useTracing ){
              e_msgf(
                "  from prefab: \"%s\" at base:size %llu:%llu"
                , ccp( F.toName() )
                , F.toBase()
                , F.toSize()
              );
            }
            m_vFiles.push( e_new<File>( F ));
          }
          fs.read( m_uBase );
        }
        return UUID;
      }

    //}:                                          |
    //get:{                                       |

      vector<Prefab::handle> Prefab::get( const string& path ){
        vector<Prefab::handle> prefabs;
        IEngine::dir( path,
          [&](  const string& dir
              , const string& name
              , const bool isDirectory ){
            if( name.ext().tolower().hash() != ".prefab"_64 ){
              return;
            }
            const auto& path = dir + name;
            Prefab::handle hPrefab = e_load<Prefab>( path );
            if( !hPrefab ){
              if( !e_fexists( path )){
                e_errorf( 933344445
                  , "Prefab does not exist: \"%s\""
                  , ccp( path )
                );
              }else{
                e_errorf( 820233334
                  , "Couldn't load \"%s\""
                  , ccp( path )
                );
              }
              return;
            }
            auto& prefab = hPrefab.cast();
            prefabs.push( hPrefab );
            prefab.setPath( path );
            prefab.toFiles().foreach(
              [&]( File::handle& F ){
                F->toBase() += prefab.toBase();
              }
            );
          }
        );
        return prefabs;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Structs:{                                       |
  //Data:{                                        |

    struct Data final:Prefab::File{
      e_reflect_no_properties( Data, Prefab::File )
      e_var( stream, st, Stream );
    };

  //}:                                            |
//}:                                              |
//API:{                                           |
  //e_unpackage:{                                 |

    void e_unpackage( const string& path ){
    }

  //}:                                            |
  //e_package:{                                   |

    bool e_package( const string& path ){

      //------------------------------------------------------------------------
      // Define the Prefab file with stream included.
      //------------------------------------------------------------------------

      auto hPrefab = e_new<Prefab>();
      auto& prefab = hPrefab.cast();

      //------------------------------------------------------------------------
      // Collect all the streams.
      //------------------------------------------------------------------------

      const auto bResult = IEngine::dir( path,
        [&]( const string& d, const string& f, bool isDirectory ){
          const auto& spec = d + "/" + f;
          auto hData = e_new<Data>();
          auto& data = hData.cast();
          auto st = e_fload( spec );
          if( st.empty() ){
            return;
          }
          data.setStream( std::move( st ));
          data.setSize( st.bytes() );
          data.setName( spec );
          prefab
            . toFiles()
            . push( hData.as<Prefab::File>() )
          ;
        }
      );
      auto startingAt = 30ull/* magic number */;
      prefab.toFiles().foreach(
        [&]( Prefab::File::handle& hFile ){
          auto& f = hFile.cast();
          f.setBase( startingAt );
          startingAt += f.toSize();
        }
      );

      //------------------------------------------------------------------------
      // Now we have a list of files, their directories and streams so we can
      // now just write the prefab header and all the files.
      //------------------------------------------------------------------------

      auto pFs = std::make_shared<Writer>(
        path.basename() + ".prefab", 0 );
      prefab.setBase( startingAt );
      prefab.preSerialize(  *pFs );
      prefab.serialize(     *pFs );
      prefab.postSerialize( *pFs );
      prefab.toFiles().foreach(
        [&]( Prefab::File::handle& hFile ){
          auto hData = hFile.as<Data>();
          auto& data = hData.cast();
          const auto& st = data.toStream();
          pFs->write( st );
        }
      );
      if( e_mkdir( path.path() )){
        pFs->save( "Prefab" );
        return true;
      }
      return false;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
