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

using namespace EON;
using namespace gfc;
using namespace ai;

//================================================|=============================
//Structs:{                                       |
  //Workspace:{                                   |

    struct Workspace final:Object{

      e_reflect_no_properties( Workspace, Object );

      //------------------------------------------|-----------------------------
      //Structs:{                                 |

        struct Project final:Object{
          e_reflect_no_properties( Project, Object );
          e_var_handle( Object, Generator );
          e_var_string( IncPath );
          e_var_string( SrcPath );
          e_var_string( ResPath );
          e_var_string( TypeID );
          e_var_string( Build );
          e_var_string( Label );
        };

      //}:                                        |
      //------------------------------------------|-----------------------------

    private:

      e_var_handle_vector1( Project   );
      e_var_string(         TypeID    );
      e_var_string(         Name      );
    };

  //}:                                            |
  //Generator:{                                   |

    struct Generator:Object{

      e_reflect_no_properties( Generator, Object );

      //------------------------------------------|-----------------------------
      //Classes:{                                 |

        enum class Source:u32{
          kNone,
          #if e_compiling( osx )
            kSharedlib,
            kStaticlib,
            kFramework,
            kXcasset,
            kLproj,
            kPlist,
            kXib,
            kRtf,
          #elif e_compiling( microsoft )
            kStaticlib, //!< DLLs and static libraries.
          #endif
          kPng,
          kHpp,
          kCpp,
          kMm,
          kInl,
          kH,
          kC,
          kM,
          kMax
        };

      //}:                                        |
      //Methods:{                                 |

        e_noinline void sortingHat( const string& path ){
          const auto& ext = path.tolower().ext();
          switch( ext.hash() ){
            #if e_compiling( osx )
              case e_hashstr64_const( ".framework" ):
                m_aSources[ Source::kFramework ].push( path );
                break;
              case e_hashstr64_const( ".xcasset" ):
                m_aSources[ Source::kXcasset ].push( path );
                break;
              case e_hashstr64_const( ".lproj" ):
                m_aSources[ Source::kLproj ].push( path );
                break;
              case e_hashstr64_const( ".plist" ):
                m_aSources[ Source::kPlist ].push( path );
                break;
              case e_hashstr64_const( ".xib" ):
                m_aSources[ Source::kXib ].push( path );
                break;
              case e_hashstr64_const( ".rtf" ):
                m_aSources[ Source::kRtf ].push( path );
                break;
              case e_hashstr64_const( ".dylib" ):
                m_aSources[ Source::kSharedlib ].push( path );
                break;
              case e_hashstr64_const( ".a" ):
                m_aSources[ Source::kStaticlib ].push( path );
                break;
            #elif e_compiling( microsoft )
              case e_hashstr64_const( ".lib" ):
                m_aSources[ Source::kStaticlib ].push( path );
                break;
            #endif
            case e_hashstr64_const( ".png" ):
              m_aSources[ Source::kPng ].push( path );
              break;
            case e_hashstr64_const( ".inl" ):
              m_aSources[ Source::kInl ].push( path );
              break;
            case e_hashstr64_const( ".hpp" ):
            case e_hashstr64_const( ".hxx" ):
            case e_hashstr64_const( ".hh" ):
              m_aSources[ Source::kHpp ].push( path );
              break;
            case e_hashstr64_const( ".cpp" ):
            case e_hashstr64_const( ".cxx" ):
            case e_hashstr64_const( ".cc" ):
              m_aSources[ Source::kCpp ].push( path );
              break;
            case e_hashstr64_const( ".mm" ):
              m_aSources[ Source::kMm ].push( path );
              break;
            case e_hashstr64_const( ".h" ):
              m_aSources[ Source::kH ].push( path );
              break;
            case e_hashstr64_const( ".c" ):
              m_aSources[ Source::kC ].push( path );
              break;
            case e_hashstr64_const( ".m" ):
              m_aSources[ Source::kM ].push( path );
              break;
            default:
              e_warnsf( "Ignoring %s!", ccp( path ));
              break;
          }
        }

        bool addFiles(){
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
              IEngine::dir( paths[ i ],
                [this]( const string& d, const string& f, const bool isDirectory ){
                  switch( f.hash() ){
                    case e_hashstr64_const( ".DS_Store" ):
                      return;
                  }
                  if( isDirectory ){
                    const auto& d_ext = f.tolower().ext();
                    if( !d_ext.empty() ){
                      sortingHat( d+f );
                    }
                  }else{
                    sortingHat( d+f );
                  }
                }
              );
            }
          }
          return true;
        }

      //}:                                        |
      //------------------------------------------|-----------------------------

      Generator( const Workspace::Project* pProject )
        : m_pProject( pProject )
      {}

      Generator() = default;
    ~ Generator() = default;

    private:

      const Workspace::Project* m_pProject = nullptr;
      e_var_array( strings, Sources, Source::kMax );
    };

  //}:                                            |
//}:                                              |
//Extends:{                                       |

  e_extends( Workspace::Project );
  e_extends( Workspace );
  e_extends( Generator );

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
      void lua_gather( lua_State* L, Workspace::Project& p ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case e_hashstr64_const( "m_typeId" ):
              p.setTypeID( lua_tostring( L, -1 ));
              break;
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
      void lua_gather( lua_State* L, Workspace::Projects& v ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          Workspace::Project::handle hProject = e_new( Workspace::Project );
          Workspace::Project& p = hProject.cast();
          p.setLabel( key );
          lua_gather( L, p );
          v.push( hProject );
          Generator::handle hGenerator = e_new( Generator, &p );
          p.setGenerator( hGenerator.as<Object>() );
          hGenerator->addFiles();
          lua_pop( L, 1 );
        }
      }
      void lua_gather( lua_State* L, Workspace& w ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          switch( key.hash() ){
            case e_hashstr64_const( "m_tProjects" ):
              lua_gather( L, w.toProjects() );
              break;
            case e_hashstr64_const( "m_typeId" ):
              w.setTypeID( lua_tostring( L, -1 ));
              break;
            case e_hashstr64_const( "m_sName" ):
              w.setName( lua_tostring( L, -1 ));
              break;
          }
          lua_pop( L, 1 );
        }
      }
      // boolean = e_generate( workspace_table );
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
      // boolean = out.save( UUID, path );
      s32 onSave( lua_State* L ){
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
        if( Class::is<Workspace>( UUID )){
          lua_pushboolean( L, false );
          return 1;
        }
        Workspace::handle hWorkspace( UUID );
        const auto& workspace = hWorkspace.cast();
        if( workspace.toName().empty() ){
          lua_pushboolean( L, false );
          return 1;
        }
        const auto& dirName = path + "/" + workspace.toName() + ".xcworkspace";
        e_md( dirName );
        fs::Writer fs( dirName + "/contents.xcworkspacedata", fs::kTEXT );
        fs.save();
        lua_pushboolean( L, true );
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
