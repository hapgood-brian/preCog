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

#include<generators.h>
#include<luacore.h>
#include<std.h>
#include<ws.h>
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Externs:{                                       |

  void verifyPBX( const string& path );

//}:                                              |
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
                    e_msgf( "Scanning %s", ccp( innerPath ));
                    IEngine::dir( innerPath,
                      [this]( const string& d, const string& f, const bool isDirectory ){
                        #if e_compiling( osx )
                          switch( f.hash() ){
                            case e_hashstr64_const( ".DS_Store" ):
                              return;
                          }
                        #endif
                        if( isDirectory ){
                          const auto& d_ext = f.tolower().ext();
                          if( !d_ext.empty() ){
                            m_pProject->sortingHat( d+f );
                          }
                        }else{
                          m_pProject->sortingHat( d+f );
                        }
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

  e_extends( Generator<Workspace::Xcode> );
  e_extends( Generator<Workspace::MSVC> );

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
            const auto& spaces = string::spaces( depth*2 );
            if(lua_isstring( L, -1 ))
              printf("%s%s = '%s'\n", ccp( spaces ), key, lua_tostring( L, -1 ));
            else if( lua_isinteger( L, -1 ))
              printf( "%s%s = %lld\n", ccp( spaces ), key, lua_tointeger( L, -1 ));
            else if( lua_isnumber( L, -1 ))
              printf( "%s%s = %f\n", ccp( spaces ), key, lua_tonumber( L, -1 ));
            else if( lua_istable( L, -1 )){
              printf( "%s%s{\n", ccp( spaces ), key );
              lua_printTable( L, depth+1 );
              printf( "%s}\n", ccp( spaces ));
            }
            lua_pop( L, 1 );
          }
        }
      #endif

      void lua_gather( lua_State* L, Workspace::Xcode& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case e_hashstr64_const( "m_build" ):
              p.setBuild( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_installScript" ):
              p.setInstallScript( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_hardenedRuntime" ):/**/{
              const string& boolean = lua_tostring( L, -1 );
              switch( boolean.tolower().hash() ){
                case e_hashstr64_const( "false" ):
                case e_hashstr64_const( "no" ):
                  p.setHardenedRuntime( false );
                  break;
                case e_hashstr64_const( "true" ):
                case e_hashstr64_const( "yes" ):
                  p.setHardenedRuntime( true );
                  break;
              }
              break;
            }
            case e_hashstr64_const( "m_linkWith" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setLinkWith( s );
              break;
            }
            case e_hashstr64_const( "m_bundleId" ):
              p.setProductBundleId( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_prefixHeader" ):
              p.setPrefixHeader( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_ignore" ):
              p.setIgnoreParts( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_language" ):
              p.setLanguage( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_teamName" ):
              p.setTeamName( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_disableOpts" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setDisableOptions( s );
              break;
            }
            case e_hashstr64_const( "m_skipUnity" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setSkipUnity( s );
              break;
            }
            case e_hashstr64_const( "m_exportHeaders" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
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
            case e_hashstr64_const( "m_includePaths" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setIncludePaths( s );
              break;
            }
            case e_hashstr64_const( "m_plistPath" ):
              p.setPlistPath( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_deployTo" ):
              p.setDeployment( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_definesDbg" ):
              p.setDefinesDbg( lua_tostring( L, -1 ));
              #if e_compiling( debug )
                e_msgf( "DBG_DEFINES: %s", ccp( p.toDefinesDbg() ));
              #endif
              break;
            case e_hashstr64_const( "m_definesRel" ):
              p.setDefinesRel( lua_tostring( L, -1 ));
              #if e_compiling( debug )
                e_msgf( "REL_DEFINES: %s", ccp( p.toDefinesRel() ));
              #endif
              break;
            case e_hashstr64_const( "m_orgName" ):
              p.setOrgName( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_incPaths" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setIncPath( s );
              break;
            }
            case e_hashstr64_const( "m_resPaths" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setResPath( s );
              break;
            }
            case e_hashstr64_const( "m_srcPaths" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setSrcPath( s );
              break;
            }
            case e_hashstr64_const( "m_frameworkPaths" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setFrameworkPaths( s );
              break;
            }
            case e_hashstr64_const( "m_libraryPaths" ):/**/{
              string s = lua_tostring( L, -1 );
              s.replace( "\n", "" );
              s.replace( " ",  "" );
              p.setLibraryPaths( s );
              break;
            }
          }
          lua_pop( L, 1 );
        }
      }

      void lua_gather( lua_State* L, Workspace::MSVC& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case e_hashstr64_const( "m_build" ):
              p.setBuild( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_incPaths" ):
              p.setIncPath( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_resPaths" ):
              p.setResPath( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_srcPaths" ):
              p.setSrcPath( lua_tostring( L, -1 ));
              break;
          }
          lua_pop( L, 1 );
        }
      }

      template<typename T> void lua_gatherAddFiles( lua_State* L, Workspace::Targets& v, typename Generator<T>::handle& hGenerator, typename T::handle& hProject ){
        const string& key = lua_tostring( L, -2 );
        auto& p = hProject.cast();
        p.setLabel( key );
        lua_gather( L, p );
        v.push( hProject.as<Workspace::Target>() );
        p.setGenerator( hGenerator.as<Object>() );
        hGenerator->addFiles();
        p.setGenerator( 0 );
      }

      void lua_gather( lua_State* L, Workspace::Targets& v ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          if( Workspace::bmp->bXcode11 ){
            Workspace::Xcode::handle hXcode = e_new( Workspace::Xcode );
            Generator<Workspace::Xcode>::handle hGenerator = e_new( Generator<Workspace::Xcode>
              , reinterpret_cast<Workspace::Xcode*>( hXcode.pcast() ));
            lua_gatherAddFiles<Workspace::Xcode>( L, v, hGenerator, hXcode );
          }
          if( Workspace::bmp->bVS2019 ){
            Workspace::MSVC ::handle hMSVC  = e_new( Workspace::MSVC );
            Generator<Workspace::MSVC>::handle hGenerator = e_new( Generator<Workspace::MSVC>
              , reinterpret_cast<Workspace::MSVC*>( hMSVC.pcast() ));
            lua_gatherAddFiles<Workspace::MSVC>( L, v, hGenerator, hMSVC );
          }
          lua_pop( L, 1 );
        }
      }

      void lua_gather( lua_State* L, Workspace& w ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case e_hashstr64_const( "m_tProjects" ):
              lua_gather( L, w.toTargets() );
              break;
            case e_hashstr64_const( "m_sName" ):
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
        Workspace::handle hWorkspace = e_new( Workspace );
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
        if( classid.hash() != e_hashstr64_const( "workspace" )){
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

        if( workspace.toFlags()->bXcode11 ){
          const auto& xcworkspace = path + "/" + workspace.toName() + ".xcworkspace";
          e_rm( xcworkspace );
          e_md( xcworkspace );
          Writer fs( xcworkspace + "/contents.xcworkspacedata", kTEXT );
          workspace.serialize( fs );
          fs.save();
          bResult = true;
        }

        //----------------------------------------------------------------------
        // Generate the solution XML for Visual Studio 2019.
        //----------------------------------------------------------------------

        if( workspace.toFlags()->bVS2019 ){
          const auto& sln = path + "/" + workspace.toName() + ".sln";
          Writer fs( sln, kTEXT );
          workspace.serialize( fs );
          fs.save();
          bResult = true;
        }
        lua_pushboolean( L, bResult );
        return 1;
      }
    }

  //}:                                            |
//}:                                              |
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
