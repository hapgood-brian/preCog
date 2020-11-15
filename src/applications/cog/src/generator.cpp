//------------------------------------------------------------------------------
//       Copyright 2018-2020 Creepy Doll Games LLC. All rights reserved.
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
            m_pProject->toIncPath(),
            m_pProject->toResPath(),
            m_pProject->toSrcPath(),
          };
          for( u32 i=0; i<e_dimof( paths ); ++i ){
            if( !paths[ i ].empty() ){
              const auto& innerPaths = paths[ i ].splitAtCommas();
              innerPaths.foreach(
                [&]( const string& innerPath ){
                  if( IEngine::dexists( innerPath )){
                    // You have to spell bundles out by hand, you can't search
                    // for them until I figure out a way to modify dir().
                    if( innerPath.ext().tolower().hash() == ".xcassets"_64 ){
                      m_pProject->sortingHat( innerPath );
                      return;
                    }
                    // Scan for files in inner path directory.
                    e_msgf( "Scanning %s", ccp( innerPath ));
                    IEngine::dir( innerPath,
                      [this]( const string& d, const string& f, const bool isDirectory ){
                        #if e_compiling( osx )
                          switch( f.hash() ){
                            case".DS_Store"_64:
                              return;
                          }
                        #endif
                        if( isDirectory ){
                          const auto& d_ext = f.tolower().ext();
                          if( !d_ext.empty() ){
                            // NB: If the extension is non-empty then we must return. Otherwise
                            // we'll recurse through .swordlight packages and other Mac related
                            // bundles picking up way too many images.
                            return;
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
//Actions:{                                       |
  //onGenerate:{                                  |

#ifdef __APPLE__
  #pragma mark - Action -
#endif

    namespace{

      #if e_compiling( debug )
        void lua_printTable( lua_State* L, const int depth ){
          lua_pushnil( L );
          while( lua_next( L, -2 )){
            ccp key = lua_tostring( L, -2 );
            const auto& indent = string::spaces( depth*2 );
            if(lua_isstring( L, -1 ))
              printf("%s%s = '%s'\n", ccp( indent ), key, lua_tostring( L, -1 ));
            else if( lua_isinteger( L, -1 ))
              printf( "%s%s = %lld\n", ccp( indent ), key, lua_tointeger( L, -1 ));
            else if( lua_isnumber( L, -1 ))
              printf( "%s%s = %f\n", ccp( indent ), key, lua_tonumber( L, -1 ));
            else if( lua_istable( L, -1 )){
              printf( "%s%s{\n", ccp( indent ), key );
              lua_printTable( L, depth+1 );
              printf( "%s}\n", ccp( indent ));
            }
            lua_pop( L, 1 );
          }
        }
      #endif

      //------------------------------------------------------------------------
      // XCODE gathering function.
      //------------------------------------------------------------------------

      void lua_gather( lua_State* L, Workspace::Xcode& p ){
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
            case"m_arcEnabled"_64:/**/{
              const string& boolean = lua_tostring( L, -1 );
              switch( boolean.tolower().hash() ){
                case"false"_64:
                case"no"_64:
                  p.setEnableARC( false );
                  break;
                case"true"_64:
                case"yes"_64:
                  p.setEnableARC( true );
                  break;
              }
              break;
            }
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
            case"m_linkWith"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setLinkWith( s );
              break;
            }
            case"m_bundleId"_64:
              p.setProductBundleId( lua_tostring( L, -1 ));
              break;
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_ignore"_64:
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case"m_language"_64:
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case"m_teamName"_64:
              p.setTeamName( lua_tostring( L, -1 ));
              break;
            case"m_disableOpts"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              const auto& arc = s.tolower();
              if( arc.hash() == "arc"_64 ){
                p.setDisableOptions( arc );
                p.setEnableARC( false );
              }else{
                p.setDisableOptions( s );
              }
              break;
            }
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_exportHeaders"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
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
            case"m_exportRefs"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              const auto& refs = s.splitAtCommas();
              refs.foreach(
                [&]( const string& ref ){
                  if( ref.empty() ){
                    return;
                  }
                  Workspace::File f( ref );
                  p.toPublicRefs().push( f );
                }
              );
              break;
            }
            case"m_includePaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_plistPath"_64:
              p.setPlistPath( lua_tostring( L, -1 ));
              break;
            case"m_sdkVersion"_64:
              p.setSdkVersion( lua_tostring( L, -1 ));
              break;
            case"m_deployTo"_64:
              p.setDeployment( lua_tostring( L, -1 ));
              break;
            case"m_definesDbg"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesRel( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case"m_orgName"_64:
              p.setOrgName( lua_tostring( L, -1 ));
              break;
            case"m_incPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              p.setIncPath( s );
              break;
            }
            case"m_resPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setResPath( s );
              break;
            }
            case"m_srcPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSrcPath( s );
              break;
            }
            case"m_frameworkPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setFrameworkPaths( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setLibraryPaths( s );
              break;
            }
            case"m_osTarget"_64:
              p.setTargetOS( lua_tostring( L, -1 ));
              break;
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
              s.del( "\n" );
              p.setLinkWith( s );
              break;
            }
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_ignore"_64:
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case"m_language"_64:
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case"m_disableOpts"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              break;
            }
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_exportHeaders"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
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
              s.del( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_definesDbg"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesRel( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case"m_incPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              p.setIncPath( s );
              break;
            }
            case"m_srcPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSrcPath( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setLibraryPaths( s );
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
              s.del( "\n" );
              p.setLinkWith( s );
              break;
            }
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_ignore"_64:
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case"m_language"_64:
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case"m_disableOpts"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              break;
            }
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_exportHeaders"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
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
              s.del( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_definesDbg"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesRel( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesRel( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case"m_incPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              p.setIncPath( s );
              break;
            }
            case"m_srcPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSrcPath( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setLibraryPaths( s );
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
            case"m_prefixHeader"_64:
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case"m_skipUnity"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setSkipUnity( s );
              break;
            }
            case"m_includePaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setIncludePaths( s );
              break;
            }
            case"m_libraryPaths"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
              p.setLibraryPaths( s );
              break;
            }
            case"m_definesDbg"_64:
              if( Workspace::bmp->bUnity ){
                p.setDefinesDbg( "__compiling_unity__=1," + string( lua_tostring( L, -1 )));
              }else{
                p.setDefinesDbg( lua_tostring( L, -1 ));
              }
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case"m_definesRel"_64:
              if( Workspace::bmp->bUnity ){
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
            case"m_winsdk"_64:
              p.setWindowsSDK( lua_tostring( L, -1 ));
              break;
            case"m_build"_64:
              p.setBuild( lua_tostring( L, -1 ));
              break;
            case"m_incPaths"_64:
              p.setIncPath( lua_tostring( L, -1 ));
              break;
            case"m_resPaths"_64:
              p.setResPath( lua_tostring( L, -1 ));
              break;
            case"m_srcPaths"_64:
              p.setSrcPath( lua_tostring( L, -1 ));
              break;
            case"m_linkWith"_64:/**/{
              string s = lua_tostring( L, -1 );
              s.del( "\n" );
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

      void lua_gather( lua_State* L, Workspace::Targets& v ){
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
                , v
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
                , v
                , hGenerator
                , hNinja
              );
            }

            //------------------------------------------------------------------
            // Xcode workspaces; connectitive tissue to making targets.
            //------------------------------------------------------------------

            if( Workspace::bmp->bXcode11 || Workspace::bmp->bXcode12 ){
              auto hXcode = e_new<Workspace::Xcode>();
              auto hGenerator = e_new<Generator<Workspace::Xcode>>(
                reinterpret_cast<Workspace::Xcode*>( hXcode.pcast() ));
              lua_gatherAddFiles<Workspace::Xcode>( L
                , v
                , hGenerator
                , hXcode
              );
            }

            //------------------------------------------------------------------
            // Visual Studio solutions; connectitive tissue to making targets.
            //------------------------------------------------------------------

            if( Workspace::bmp->bVS2019 ){
              auto hMSVC = e_new<Workspace::MSVC>();
              auto hGenerator = e_new<Generator<Workspace::MSVC>>(
                reinterpret_cast<Workspace::MSVC*>( hMSVC.pcast() ));
              lua_gatherAddFiles<Workspace::MSVC>( L
                , v
                , hGenerator
                , hMSVC
              );
            }
          }
          lua_pop( L, 1 );
        }
      }

      void lua_gather( lua_State* L, Workspace& w ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case"m_tProjects"_64:
              lua_gather( L, w.toTargets() );
              break;
            case"m_sName"_64:
              w.setName( lua_tostring( L, -1 ));
              break;
          }
          lua_pop( L, 1 );
        }
      }

      s32 onGenerate( lua_State* L ){
        #if e_compiling( debug )
          lua_pushvalue( L, -1 );//+1
          lua_printTable( L, 0 );
          lua_pop( L, 1 );//-1
        #endif
        auto hWorkspace = e_new<Workspace>();
        auto& workspace = hWorkspace.cast();
        lua_pushvalue( L, -1 );//+1
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
    }

  //}:                                            |
  //onSave:{                                      |

    namespace{

      s32 onSave( lua_State* L ){

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

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
        bool bResult = false;

        //----------------------------------------------------------------------
        // Generate the workspace bundle for Xcode11.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bXcode11 || Workspace::bmp->bXcode12 ){
          const auto& xcworkspace = path + "/" + workspace.toName() + ".xcworkspace";
          e_rm( xcworkspace );
          e_md( xcworkspace );
          Writer fs( xcworkspace + "/contents.xcworkspacedata", kTEXT );
          workspace.serialize( fs );
          workspace.cleanup();
          fs.save();
          bResult = true;
        }

        //----------------------------------------------------------------------
        // Generate the solution XML for Visual Studio 2019.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bVS2019 ){
          const auto& sln = path + "/" + workspace.toName() + ".sln";
          Writer fs( sln, kTEXT );
          workspace.serialize( fs );
          workspace.cleanup();
          fs.save();
          bResult = true;
        }

        //----------------------------------------------------------------------
        // Generate the Qmake name.pro for all platforms.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bQmake ){
          const auto& build = path + "/" + workspace.toName() + ".pro";
          Writer fs( build, kTEXT );
          workspace.serialize( fs );
          workspace.cleanup();
          fs.save();
          bResult = true;
        }

        //----------------------------------------------------------------------
        // Generate the Ninja.build for Linux primarily.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bNinja ){
          const auto& build = path + "/build.ninja";
          Writer fs( build, kTEXT );
          workspace.serialize( fs );
          workspace.cleanup();
          fs.save();
          bResult = true;
        }

        //----------------------------------------------------------------------
        // Return result boolean to Lua.
        //----------------------------------------------------------------------

        lua_pushboolean( L, bResult );
        return 1;
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Tablefu:{                                       |

  luaL_Reg generators[3]={
    //Generators (1):{                            |

      {"generate", onGenerate },

    //}:                                          |
    //Savers     (1):{                            |

      {"save", onSave },

    //}:                                          |
    //END        (1):{                            |

      {0,0}

    //}:                                          |
  };

//}:                                              |
//================================================|=============================
