//------------------------------------------------------------------------------
//                    Copyright 2022 Creepy Doll Software LLC.
//                            All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//------------------------------------------------------------------------------

#include<generators.h>
#include<luacore.h>
#include<std.h>
#include<ws.h>
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Structs:{                                       |
  //Generator:{                                   |

    template<typename T> struct Generator final:Object{

      e_reflect_no_properties( Generator, Object );

      //------------------------------------------|-----------------------------
      //Aliases:{                                 |

        using Type = typename T::Type;

      //}:                                        |
      //Methods:{                                 |

        e_noinline bool addFiles(){
          if( !m_pProject ){
            return false;
          }
          const string paths[]{
            m_pProject->toResPath(),
            m_pProject->toSrcPath(),
          };
          for( u32 i=0; i<e_dimof( paths ); ++i ){
            if( !paths[ i ].empty() ){
              const auto& innerPaths = paths[ i ].splitAtCommas();
              innerPaths.foreach(
                [&]( const string& innerPath ){
                  if( IEngine::dexists( innerPath )){
                    e_msgf( "Scanning %s", ccp( innerPath ));
                    IEngine::dir( innerPath,
                      [this]( const string& d
                            , const string& f
                            , const bool isDirectory ){
                        switch( f.hash() ){
                          case".DS_Store"_64:
                            return;
                        }
                        if( isDirectory ){
                          const auto& d_ext = f.ext().tolower();
                          if( !d_ext.empty() ){
                            switch( d_ext.hash() ){
                              case".xcassets"_64:
                                break;
                              default:/**/{
                                // NB: If the extension is non-empty then we
                                // must return. Otherwise we'll recurse thru
                                // Lelu-XD packages and other Mac related
                                // bundles picking up way too many images.
                                return;
                              }
                            }
                          }
                        }
                        m_pProject->sortingHat( d + f );
                      }
                    );
                  }else{
                    m_pProject->sortingHat( innerPath );
                  }
                }
              );
            }
          }
          return true;
        }

      //}:                                        |
      //------------------------------------------|-----------------------------

      Generator( T* pProject )
        : m_pProject( pProject )
      {}

      Generator() = default;
    ~ Generator() = default;

    private:

      T* m_pProject = nullptr;
    };

  //}:                                            |
//}:                                              |
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_specialized_extends( Generator<Workspace::Xcode> );
  e_specialized_extends( Generator<Workspace::Ninja> );
  e_specialized_extends( Generator<Workspace::MSVC>  );

//}:                                              |
//Methods:{                                       |
  //lua_gather:{                                  |

#ifdef __APPLE__
  #pragma mark - Functions -
#endif

    namespace{

      //------------------------------------------------------------------------
      // Get string function.
      //------------------------------------------------------------------------

      string lua_gatherCleanFile( lua_State* L, const s32 ix ){
        string input = lua_tostring( L, ix );
        input.erase( "\n" );
        string result;
        cp  r = cp( input.c_str() );
        ccp e = input.end();
        while( r &&( r < e )){
          if((( r[0]=='-' )&&( r[1]=='-' ))||( *r == '#' )){
            r = string::skip_2eol( r );
          }else if(( *r == '"' )||( *r == '\'' )){
            r = strchr( r, *r );
          }else if( ' ' == *r ){
            ++r;
          }else{
            result.catf( "%c", *r++ );
          }
        }
        if( result.back() == ',' )
          result.trim( 1 );
        return result;
      }

      //------------------------------------------------------------------------
      // XCODE gathering function.
      //------------------------------------------------------------------------

      void lua_gather( lua_State* L, Workspace::Xcode& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){

            //------------------------------------|-----------------------------
            //Hardening:{                         |

              case"m_hardenedRuntime"_64:/**/{
                const string& boolean = lua_tostring( L, -1 );
                switch( boolean.tolower().hash() ){
                  case"false"_64:
                  case"no"_64:
                    p.setHardenedRuntime( false );
                    break;
                  case"true"_64:
                  case"yes"_64:
                    p.setHardenedRuntime( true );
                    break;
                }
                break;
              }

            //}:                                  |
            //Symbolics:{                         |

              case"m_loadAllSymbols"_64:/**/{
                const string& boolean = lua_tostring( L, -1 );
                switch( boolean.tolower().hash() ){
                  case"false"_64:
                  case"no"_64:
                    p.setLoadAllSymbols( false );
                    break;
                  case"true"_64:
                  case"yes"_64:
                    p.setLoadAllSymbols( true );
                    break;
                }
                break;
              }

            //}:                                  |
            //Universal:{                         |

              case"m_enableUniversal"_64:/**/{
                const string& boolean = lua_tostring( L, -1 );
                switch( boolean.tolower().hash() ){
                  case"false"_64:
                  case"no"_64:
                    p.setUniversalBinary( false );
                    break;
                  case"true"_64:
                  case"yes"_64:
                    p.setUniversalBinary( true );
                    break;
                }
                break;
              }

            //}:                                  |
            //DisableLibValidation:{              |

              case"m_disableLibValidation"_64:/**/{
                const string& boolean = lua_tostring( L, -1 );
                switch( boolean.tolower().hash() ){
                  case"false"_64:
                  case"no"_64:
                    p.setDisableLibValidation( false );
                    break;
                  case"true"_64:
                  case"yes"_64:
                    p.setDisableLibValidation( true );
                    break;
                }
                break;
              }

            //}:                                  |
            //AppleSilicon:{                      |

              case"m_enableAppleSilicon"_64:/**/{
                const string& boolean = lua_tostring( L, -1 );
                switch( boolean.tolower().hash() ){
                  case"false"_64:
                  case"no"_64:
                    p.setAppleSilicon( false );
                    break;
                  case"true"_64:
                  case"yes"_64:
                    p.setAppleSilicon( true );
                    break;
                }
                break;
              }

            //}:                                  |
            //Libs:{                              |

              case"m_libs"_64:
                p.setLibraryPaths( lua_gatherCleanFile( L, -1 ));
                break;

            //}:                                  |
            //EmbedSign:{                         |

              case"m_filesToEmbedAndSign"_64:/**/{
                const auto& s = lua_gatherCleanFile( L, -1 );
                e_msgf( "  Prepping to embed %s.", ccp( s ));
                p.setEmbedAndSign( s );
                break;
              }

              case"m_noEmbedAndSign"_64:/**/{
                p.setNoEmbedAndSign( true );
                break;
              }

              case"m_teamName"_64:
                p.setTeamName( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Installer:{                         |

              case"m_installScript"_64:
                p.setInstallScript( lua_gatherCleanFile( L, -1 ));
                break;

            //}:                                  |
            //LinkWith:{                          |

              case"m_linkWith"_64:/**/{

                //--------------------------------------------------------------
                // Name the libraries to link with via a single string.
                //--------------------------------------------------------------

                if( lua_isstring( L, -1 )){
                  p.setLinkWith( lua_gatherCleanFile( L, -1 ));
                  break;
                }

                //--------------------------------------------------------------
                // Name the libraries to link with via a single table. We will
                // pull the table apart, and build the m_sLinkWith string from
                // it.
                //--------------------------------------------------------------

                if( lua_istable( L, -1 )){
                  lua_pushnil( L );
                  while( lua_next( L, -2 )){
                    if( lua_isstring( L, -1 )){
                      p.toLinkWith() += lua_gatherCleanFile( L, -1 );
                      p.toLinkWith() += ",";
                    }else if( lua_istable( L, -1 )){
                      lua_getfield( L, -1, "label" );
                        if( lua_isstring( L, -1 )){
                          const string name = lua_tostring( L, -1 );
                          p.toLinkWith() += name;
                          p.toLinkWith() += ",";
                        }
                      lua_pop( L, 1 );
                    }
                    lua_pop( L, 1 );
                  }
                  break;
                }
              }

            //}:                                  |
            //Language:{                          |

              case"m_clanguage"_64:
                p.setLanguageC( lua_tostring( L, -1 ));
                break;

              case"m_language"_64:
                p.setLanguage( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Include:{                           |

            case"m_includePaths"_64:
              p.setIncludePaths( lua_gatherCleanFile( L, -1 ));
              break;

            //}:                                  |
            //Defines:{                           |

              case"m_definesDbg"_64:
                if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                  p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
                }else{
                  p.setDefinesDbg( lua_tostring( L, -1 ));
                }
                break;

              case"m_definesRel"_64:
                if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                  p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
                }else{
                  p.setDefinesRel( lua_tostring( L, -1 ));
                }
                break;

            //}:                                  |
            //Ignore:{                            |

              case"m_ignore"_64:
                p.setIgnoreParts( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Bundle:{                            |

              case"m_bundleId"_64:
                p.setProductBundleId( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Prefix:{                            |

              case"m_prefixHeader"_64:
                p.setPrefixHeader( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Export:{                            |

              case"m_exportHeaders"_64:/**/{
                const auto& strlist = lua_gatherCleanFile( L, -1 );
                const auto& headers = strlist.splitAtCommas();
                headers.foreach(
                  [&]( const string& header ){
                    if( header.empty() ){
                      return;
                    }
                    Workspace::File f( header );
                    f.setPublic( true );
                    p.toPublicHeaders().push( f );
                  }
                );
                break;
              }

              case"m_exportRefs"_64:/**/{
                const auto& str = lua_gatherCleanFile( L, -1 );
                const auto& ref = str.splitAtCommas();
                ref.foreach(
                  [&]( const string& r ){
                    if( r.empty() ){
                      return;
                    }
                    Workspace::File f( r );
                    p.toPublicRefs()
                      . push( f )
                    ;
                  }
                );
                break;
              }

            //}:                                  |
            //Deploy:{                            |

              case"m_deployTo"_64:
                p.setDeployment( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Build:{                             |

              case"m_build"_64:
                p.setBuild( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Unity:{                             |

              case"m_skipUnity"_64:
                p.setSkipUnity( lua_gatherCleanFile( L, -1 ));
                break;

              case"m_bUnity"_64:/**/{
                const string& boolean = lua_tostring( L, -1 );
                switch( boolean.tolower().hash() ){
                  case"false"_64:
                  case"no"_64:
                    p.setUnityBuild( false );
                    break;
                  case"true"_64:
                  case"yes"_64:
                    p.setUnityBuild( true );
                    break;
                }
                break;
              }

            //}:                                  |
            //Paths:{                             |

              case"m_resPaths"_64:
                p.setResPath( lua_gatherCleanFile( L, -1 ));
                break;

              case"m_srcPaths"_64:
                p.setSrcPath( lua_gatherCleanFile( L, -1 ));
                break;

              case"m_frameworkPaths"_64:
                p.setFrameworkPaths( lua_gatherCleanFile( L, -1 ));
                break;

              case"m_libraryPaths"_64:
                p.setFindLibsPaths( lua_gatherCleanFile( L, -1 ));
                break;

            //}:                                  |
            //ARC:{                               |
              //disable:{                         |

                case"m_disableOpts"_64:/**/{
                  string s = lua_tostring( L, -1 );
                  s.erase( "\n" );
                  const auto& arc = s.tolower();
                  if( arc.hash() == "arc"_64 ){
                    p.setDisableOptions( arc );
                    p.setEnableARC( false );
                  }else{
                    p.setDisableOptions( s );
                  }
                  break;
                }

              //}:                                |
              //enable:{                          |

                case"m_arcEnabled"_64:/**/{
                  const string& boolean = lua_tostring( L, -1 );
                  switch( boolean.tolower().hash() ){
                    case"false"_64:
                    case"no"_64:
                    case"0"_64:
                      p.setEnableARC( false );
                      break;
                    case"true"_64:
                    case"yes"_64:
                    case"1"_64:
                      p.setEnableARC( true );
                      break;
                  }
                  break;
                }

              //}:                                |
            //}:                                  |
            //Org:{                               |

              case"m_orgName"_64:
                p.setOrgName( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //Os:{                                |

              case"m_osTarget"_64:
                p.setTargetOS( lua_tostring( L, -1 ));
                break;

            //}:                                  |
            //------------------------------------|-----------------------------
          }
          lua_pop( L, 1 );
        }
      }

      //------------------------------------------------------------------------
      // NINJA gathering function.
      //------------------------------------------------------------------------

      void lua_gather( lua_State* L, Workspace::Ninja& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case"m_build"_64:
              p.setBuild( lua_tostring( L, -1 ));
              break;
            case"m_installScript"_64:
              p.setInstallScript( lua_tostring( L, -1 ));
              break;
            case"m_linkWith"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setLinkWith( s );
              break;
            }
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_ignore"_64:
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case"m_clanguage"_64:
              p.setLanguageC( lua_tostring( L, -1 ));
              break;
            case"m_language"_64:
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case"m_disableOpts"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              break;
            }
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_exportHeaders"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              const auto& headers = s.splitAtCommas();
              headers.foreach(
                [&]( const string& header ){
                  if( header.empty() ){
                    return;
                  }
                  Workspace::File f( header );
                  f.setPublic( true );
                  p.toPublicHeaders().push( f );
                }
              );
              break;
            }
            case"m_includePaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_definesDbg"_64:
              if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesRel( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case"m_srcPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setSrcPath( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setFindLibsPaths( s );
              break;
            }
          }
          lua_pop( L, 1 );
        }
      }

      //------------------------------------------------------------------------
      // QMAKE gathering function.
      //------------------------------------------------------------------------

      void lua_gather( lua_State* L, Workspace::Qmake& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case"m_build"_64:
              p.setBuild( lua_tostring( L, -1 ));
              break;
            case"m_installScript"_64:
              p.setInstallScript( lua_tostring( L, -1 ));
              break;
            case"m_linkWith"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setLinkWith( s );
              break;
            }
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_ignore"_64:
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case"m_clanguage"_64:
              p.setLanguageC( lua_tostring( L, -1 ));
              break;
            case"m_language"_64:
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case"m_disableOpts"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              break;
            }
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_exportHeaders"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              const auto& headers = s.splitAtCommas();
              headers.foreach(
                [&]( const string& header ){
                  if( header.empty() ){
                    return;
                  }
                  Workspace::File f( header );
                  f.setPublic( true );
                  p.toPublicHeaders().push( f );
                }
              );
              break;
            }
            case"m_includePaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_definesDbg"_64:
              if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesRel( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case"m_srcPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setSrcPath( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setFindLibsPaths( s );
              break;
            }
          }
          lua_pop( L, 1 );
        }
      }

      //------------------------------------------------------------------------
      // MSVC gathering function.
      //------------------------------------------------------------------------

      void lua_gather( lua_State* L, Workspace::MSVC& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case"m_ignore"_64:
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case"m_clanguage"_64:
              p.setLanguageC( lua_tostring( L, -1 ));
              break;

            case"m_language"_64:
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_includePaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setFindLibsPaths( s );
              break;
            }
            case"m_definesDbg"_64:
              if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( p.isUnityBuild() && Workspace::bmp->bUnity ){
                p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesRel( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case"m_def"_64:
              p.setDefinition( lua_tostring( L, -1 ));
              break;
            case"m_toolchain"_64:
              p.setPlatformTools( lua_tostring( L, -1 ));
              break;
            case"m_dependencies"_64:
              p.setDependencies( lua_tostring( L, -1 ));
              break;
            case"m_winsdk"_64:
              p.setWindowsSDK( lua_tostring( L, -1 ));
              break;
            case"m_build"_64:
              p.setBuild( lua_tostring( L, -1 ));
              break;
            case"m_resPaths"_64:
              p.setResPath( lua_tostring( L, -1 ));
              break;
            case"m_srcPaths"_64:
              p.setSrcPath( lua_tostring( L, -1 ));
              break;
            case"m_linkWith"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.erase( "\n" );
              p.setLinkWith( s );
              break;
            }
          }
          lua_pop( L, 1 );
        }
      }

      //------------------------------------------------------------------------
      // Gathering & add files function.
      //------------------------------------------------------------------------

      template<typename T> void lua_gatherAddFiles( lua_State* L
          , Workspace::Targets& v
          , typename Generator<T>::handle& hGenerator
          , typename T::handle& hProject ){
        const string& key = lua_tostring( L, -2 );
        auto& p = hProject.cast();
        p.setLabel( key );
        lua_gather( L, p );
        v.push( hProject.template as<Workspace::Target>() );
        p.setGenerator( hGenerator.template as<Object>() );
        hGenerator->addFiles();
        p.setGenerator( 0 );
      }

      //------------------------------------------------------------------------
      // Gathering function.
      //------------------------------------------------------------------------

      void lua_gather( lua_State* L, Workspace::Targets& targets ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string next( lua_tostring( L, -2 ));
          if( !next.empty() ){

            //------------------------------------------------------------------
            // Qmake workspaces; connectitive tissue to making targets.
            //------------------------------------------------------------------

            if( Workspace::bmp->bQmake ){
              auto hQmake = e_new<Workspace::Qmake>();
              auto hGenerator = e_new<Generator<Workspace::Qmake>>(
                reinterpret_cast<Workspace::Qmake*>( hQmake.pcast() ));
              lua_gatherAddFiles<Workspace::Qmake>( L
                , targets
                , hGenerator
                , hQmake
              );
            }

            //------------------------------------------------------------------
            // Ninja workspaces; connectitive tissue to making targets.
            //------------------------------------------------------------------

            if( Workspace::bmp->bNinja ){
              auto hNinja = e_new<Workspace::Ninja>();
              auto hGenerator = e_new<Generator<Workspace::Ninja>>(
                reinterpret_cast<Workspace::Ninja*>( hNinja.pcast() ));
              lua_gatherAddFiles<Workspace::Ninja>( L
                , targets
                , hGenerator
                , hNinja
              );
            }

            //------------------------------------------------------------------
            // Xcode workspaces; connectitive tissue to making targets.
            //------------------------------------------------------------------

            if( Workspace::bmp->bXcode11 ||
                Workspace::bmp->bXcode12 ||
                Workspace::bmp->bXcode14 ){
              auto hXcode = e_new<Workspace::Xcode>();
              auto hGenerator = e_new<Generator<Workspace::Xcode>>(
                reinterpret_cast<Workspace::Xcode*>( hXcode.pcast() ));
              lua_gatherAddFiles<Workspace::Xcode>( L
                , targets
                , hGenerator
                , hXcode
              );
            }

            //------------------------------------------------------------------
            // Visual Studio solutions; connectitive tissue to making targets.
            //------------------------------------------------------------------

            if( Workspace::bmp->bVS2022 ){
              auto hMSVC = e_new<Workspace::MSVC>();
              if( Workspace::bmp->bVSTools143 ){
                hMSVC->setPlatformTools( "v143" );
              }
              auto hGenerator = e_new<Generator<Workspace::MSVC>>(
                reinterpret_cast<Workspace::MSVC*>( hMSVC.pcast() ));
              lua_gatherAddFiles<Workspace::MSVC>( L
                , targets
                , hGenerator
                , hMSVC
              );
            }else if( Workspace::bmp->bVS2019 ){
              auto hMSVC = e_new<Workspace::MSVC>();
              auto hGenerator = e_new<Generator<Workspace::MSVC>>(
                reinterpret_cast<Workspace::MSVC*>( hMSVC.pcast() ));
              lua_gatherAddFiles<Workspace::MSVC>( L
                , targets
                , hGenerator
                , hMSVC
              );
            }
          }
          lua_pop( L, 1 );
        }
      }

      void lua_gather( lua_State* L, Workspace& w ){
        // https://pgl.yoyo.org/luai/i/lua_next
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string key( lua_tostring( L, -2 ));
          switch( key.hash() ){
            case"m_tProjects"_64:
              lua_pushvalue( L, -1 );
              lua_gather( L
                , w.toTargets() );
              lua_pop( L, 1 );
              break;
            case"m_sName"_64:
              w.setName( lua_tostring( L, -1 ));
              break;
          }
          // Removes 'value'; keeps 'key' for next iteration.
          lua_pop( L, 1 );
        }
      }
    }

#ifdef __APPLE__
  #pragma mark - Action -
#endif

    s32 onGenerate( lua_State* L ){
      auto hWorkspace = e_new<Workspace>();
      auto& workspace = hWorkspace.cast();
      lua_pushvalue( L, -1 );//+1
        if( !lua_istable( L, -1 ))
          e_errorf( 1001, "[issue] Must pass in a table" );
        lua_gather( L, workspace );
      lua_pop( L, 1 );//-1
      lua_getfield( L, -1, "__class" );//+1
        if( !lua_isstring( L, -1 )){
          lua_pop( L, 1 );//-1
          lua_pushnil( L );//+1
          return 1;
        }
        const string& classid = lua_tostring( L, -1 );
      lua_pop( L, 1 );//-1
      if( classid.hash() != "workspace"_64 ){
        lua_pushnil( L );//+1
        return 1;
      }
      lua_pushinteger( L, workspace.UUID );//+1
      workspace.addref();
      return 1;
    }

  //}:                                            |
//}:                                              |
//Actions:{                                       |
  //onSave:{                                      |

    s32 onSave( lua_State* L ){
      auto bResult = false;

      //------------------------------------------------------------------------
      // Bail conditions.
      //------------------------------------------------------------------------

      const string& path = lua_tostring( L, -1 );
      if( path.empty() ){
        lua_pushboolean( L, false );
        return 1;
      }
      const s64 UUID = lua_tointeger( L, -2 );
      if( !Class::Factory::valid( UUID )){
        lua_pushboolean( L, false );
        return 1;
      }
      Object::handle hObject = UUID;
      if( !hObject.isa<Workspace>() ){
        lua_pushboolean( L, false );
        return 1;
      }
      const auto& workspace = hObject.as<Workspace>().cast();
      if( workspace.toName().empty() ){
        lua_pushboolean( L, false );
        return 1;
      }

      //------------------------------------------------------------------------
      // Generate the workspace bundle for Xcode11.
      //------------------------------------------------------------------------

      if( Workspace::bmp->bXcode11 ||
          Workspace::bmp->bXcode12 ||
          Workspace::bmp->bXcode14 ){
        const auto& xcworkspace = path
          + "/"
          + workspace.toName()
          + ".xcworkspace";
        e_rm( xcworkspace );
        e_md( xcworkspace );
        Writer fs( xcworkspace
          + "/contents.xcworkspacedata"
          , kTEXT );
        workspace.serialize( fs );
        workspace.cleanup();
        bResult = true;
        fs.save();
      }

      //------------------------------------------------------------------------
      // Generate the solution XML for Visual Studio 2019.
      //------------------------------------------------------------------------

      if( Workspace::bmp->bVS2019 ||
          Workspace::bmp->bVS2022 ){
        const auto& sln = path + "/" + workspace.toName() + ".sln";
        Writer fs( sln, kTEXT );
        workspace.serialize( fs );
        workspace.cleanup();
        bResult = true;
        fs.save();
      }

      //------------------------------------------------------------------------
      // Generate the Qmake name.pro for all platforms.
      //------------------------------------------------------------------------

      if( Workspace::bmp->bQmake ){
        const auto& build = path + "/" + workspace.toName() + ".pro";
        Writer fs( build, kTEXT );
        workspace.serialize( fs );
        workspace.cleanup();
        bResult = true;
        fs.save();
      }

      //------------------------------------------------------------------------
      // Generate the Ninja.build for Linux primarily.
      //------------------------------------------------------------------------

      if( Workspace::bmp->bNinja ){
        const auto& build = path + "/build.ninja";
        Writer fs( build, kTEXT );
        workspace.serialize( fs );
        workspace.cleanup();
        bResult = true;
        fs.save();
      }

      //------------------------------------------------------------------------
      // Return result boolean to Lua.
      //------------------------------------------------------------------------

      lua_pushboolean( L, bResult );
      return 1;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================

#ifdef __APPLE__
  #pragma mark - Methods -
#endif

void Lua::save(){
  sandbox( L,
    "if wsp ~= nil then\n"
    "  local uuid = generate( wsp )\n"
    "  if uuid ~= nil then\n"
    "    save( uuid, 'tmp' )\n"
    "  end\n"
    "end\n"
  );
}
