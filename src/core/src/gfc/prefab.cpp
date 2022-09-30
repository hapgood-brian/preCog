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
            if( name.ext().tolower().hash() != ".index"_64 ){
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
//Private:{                                       |
  //isTextualStream:{                             |

    namespace{
      bool isTextualStream( const stream& st ){
        ccp e = st.data() + st.bytes();
        ccp p = st.data();
        while( p < e ){
          switch( *p ){
            case 0:
            case'\t':
            case'\r':
            case'\n':
              break;
            default:
              if( *p < ' ' ){
                return false;
              }
              break;
          }
          ++p;
        }
        return true;
      }
    }

  //}:                                            |
  //makeStream:{                                  |

    namespace{
      stream makeStream( const string& filespec ){
        string tmp_spec( filespec );
        tmp_spec.replace( "/", "_" );
        tmp_spec = tmp_spec.camelcase();
        const auto& path = ".intermediate/" + tmp_spec + ".eon";
        const auto& st = e_fload( filespec );
        if( st.empty() ){
          return{};
        }
        u32 flags = 0;
        if( !isTextualStream( st )){
          flags |= kCOMPRESS | kSHA1;
        }else{
          flags |= kTEXT;
        }
        { auto fs = std::make_unique<Writer>( path, flags );
          fs->write( st );
          if( flags & kTEXT ){
            fs->save( nullptr );
          }else{
            fs->save( "File" );
          }
        }
        return e_fload( path );
      }
    }

  //}:                                            |
//}:                                              |
//Structs:{                                       |
  //Data:{                                        |

    struct Data final:Prefab::File{
      e_reflect_no_properties( Data, Prefab::File )
      e_var( stream, st, Stream );
    };

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//API:{                                           |
  //e_loadFromPrefab:{                            |

    std::shared_ptr<stream> e_loadFromPrefab( const Prefab& prefab, Reader& fs, const string& name ){
      std::shared_ptr<stream>st = std::make_shared<stream>();
      prefab.toFiles().foreachs(
        [&]( const Prefab::File::handle& F ){
          if( F->isName( name )){
            fs.toStream().seek( prefab.toBase() + F->toBase() );
            cp pBuffer = st->alloc( F->toSize() );
            fs.read( pBuffer, F->toSize() );
            st->reset();
            return false;
          }
          return true;
        }
      );
      return st;
    }

  //}:                                            |
  //e_unpackage:{                                 |

    bool e_unpackage( const string& in_cPath ){

      //------------------------------------------------------------------------
      // We're going to go pretty low-level now; ignoring e_load<> etc. and just
      // get a stream to use.
      //------------------------------------------------------------------------

      auto r = std::make_unique<Reader>( in_cPath );
      auto& fs = r->load( "Prefab" );
      if( fs.isError() ){
        return false;
      }
      const auto clsid = fs.as<u64>();
      if( !Class::Factory::describe( clsid )){
        return false;
      }
      auto hPrefab = e_new<Prefab>( in_cPath );
      auto& prefab = hPrefab.cast();
      prefab.setName( in_cPath.basename() );
      prefab.setPath( in_cPath );
      prefab.preSerialize(  *r );
      prefab.serialize(     *r );
      prefab.postSerialize( *r );
      prefab.setBase( r->toStream().tell() );

      //------------------------------------------------------------------------
      // Run through all prefab filenames and rebuild directory structure.
      //------------------------------------------------------------------------

      prefab.toFiles().foreach(
        [&]( Prefab::File::handle& F ){
          const auto& file = F.cast();
          const auto& spec = file.toName();
          const auto& path = spec.path();
          e_msgf( "Unpacking %s", ccp( spec ));
          if( !path.empty() ){
            e_mkdir( path );
          }
          auto st = e_loadFromPrefab( prefab, fs, spec );
          if( !st->capacity() ){
            return;
          }
          FILE* f = e_fopen( spec, "w" );
          if( !f ){
            return;
          }
          fwrite( st->data(), 1, file.toSize(), f );
          fclose( f );
        }
      );
      return true;
    }

  //}:                                            |
  //e_package:{                                   |

    bool e_package( const strings& filesAndDirs, const string& pkgName ){

      //------------------------------------------------------------------------
      // Define the Prefab file with stream included.
      //------------------------------------------------------------------------

      auto hPrefab = e_new<Prefab>();
      auto& prefab = hPrefab.cast();

      //------------------------------------------------------------------------
      // Collect all the streams.
      //------------------------------------------------------------------------

      auto startingAt = 0;
      filesAndDirs.foreach(
        [&]( const string& path ){
          IEngine::dir( path,
            [&]( const string& d, const string& f, bool isDirectory ){
              if( isDirectory ){
                return;
              }
              const auto& spec = d + f;
              auto st = makeStream( spec );
              if( st.empty() ){
                return;
              }
              auto hData = e_new<Data>();
              auto& data = hData.cast();
              data.setSize( st.bytes() );//must come before std::move().
              data.toStream() = std::move( st );
              data.setName( spec );
              prefab
                . toFiles()
                . push( hData.as<Prefab::File>()
              );
            }
          );
          prefab.toFiles().foreach(
            [&]( Prefab::File::handle& hFile ){
              auto& f = hFile.cast( );
              f.setBase( startingAt );
              startingAt +=
                f.toSize()
              ;
            }
          );
        }
      );

      //------------------------------------------------------------------------
      // Now we have a list of files, their directories and streams so we can
      // now just write the prefab header and all the files.
      //------------------------------------------------------------------------

      const u32 flags = pkgName.empty()
        ? kSHA1
        | kCOMPRESS
        : kCOMPRESS;
      auto sfn = ( pkgName.empty() ? filesAndDirs[ 0 ].basename() : pkgName ) + ".prefab";
      sfn.replace(
          ".prefab.prefab"
        , ".prefab" );
      auto pFs = std::make_shared<Writer>( sfn, flags );
      prefab.setBase( startingAt );
      prefab.preSerialize(  *pFs );
      prefab.serialize(     *pFs );
      prefab.postSerialize( *pFs );
      prefab.toFiles().foreach(
        [&]( Prefab::File::handle& hFile ){
          auto hData = hFile.as<Data>();
          auto& data = hData.cast();
          const auto& st = data.toStream();
          pFs->write( st.data()
            , data.toSize()
          );
        }
      );
      if( !pFs->save( "Prefab" )){
        return false;
      }
      e_rd({ ".intermediate" });
      return true;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
