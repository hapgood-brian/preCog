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
#include<regex>

using namespace EON;
using namespace gfc;
using namespace ai;
using namespace fs;

//================================================|=============================
//Externs:{                                       |

  void verifyPBX( const string& path );

//}:                                              |
//Structs:{                                       |
  //Workspace:{                                   |

#ifdef __APPLE__
  #pragma mark - Structures -
#endif

    struct Workspace final:Object{

      e_reflect_no_properties( Workspace, Object );

      //------------------------------------------|-----------------------------
      //Structs:{                                 |

        struct Project:Object{

          e_reflect_no_properties( Project, Object );

          //--------------------------------------|-----------------------------
          //Structs:{                             |

            struct File final:string{
              File( const string& s )
                : string( s )
              {}
              File( string&& s )
                : string( std::move( s ))
              {}
              File() = default;
            ~ File() = default;

            private:

              e_var_string( BuildID ) = string::resourceId();
              e_var_string( RefID   ) = string::resourceId();
              e_var_bool(   Public  ) = false;
            };

          //}:                                    |
          //Aliases:{                             |

            using Files = vector<File>;

          //}:                                    |
          //Classes:{                             |

            enum class Source:u32{
              kNone,
              kStoryboard,
              kSharedlib,
              kStaticlib,
              kFramework,
              kXcasset,
              kLproj,
              kPlist,
              kXib,
              kRtf,
              kPng,
              kHpp,
              kCpp,
              kMm,
              kInl,
              kH,
              kC,
              kM,
              kPrefab,
              kEon,
              kMax
            };

          //}:                                    |
          //--------------------------------------|-----------------------------

        private:

          e_var_array(  Files,    Sources, Source::kMax );
          e_var(        Files, v, PublicHeaders         );
          e_var(        Files, v, EmbedFiles            );
          e_var(        Files, v, LibFiles              );
          e_var_handle( Object,   Generator             );
          e_var_string(           DisableOptions        );
          e_var_string(           IncludePaths          );
          e_var_string(           PrefixHeader          );
          e_var_string(           IgnoreParts           );
          e_var_string(           Deployment            ) = "10.15";
          e_var_string(           DefinesRel            ) = "NDEBUG, RELEASE";
          e_var_string(           DefinesDbg            ) = "_DEBUG, DEBUG";
          e_var_string(           SkipUnity             );
          e_var_string(           LinkWith              );
          e_var_string(           TeamName              );
          e_var_string(           Language              ) = "c++17";
          e_var_string(           FrameworkPaths        );
          e_var_string(           LibraryPaths          );
          e_var_string(           IncPath               );
          e_var_string(           SrcPath               );
          e_var_string(           PlistPath             );
          e_var_string(           ResPath               );
          e_var_string(           OrgName               );
          e_var_string(           TypeID                );
          e_var_string(           Build                 );
          e_var_string(           Label                 );
          e_var_bool(             HardenedRuntime       );
        };

        struct Xcode final:Project{

          e_reflect_no_properties( Xcode, Project );

          //--------------------------------------|-----------------------------
          //Aliases:{                             |

            using Unity = array<array<Files,5>,4>;

          //}:                                    |
          //Methods:{                             |

            virtual void serialize( Writer& )const override;

          //}:                                    |
          //--------------------------------------|-----------------------------

        private:

          e_var_string( ReleaseBuildConfiguration ) = string::resourceId();
          e_var_string( ReleaseNativeBuildConfig  ) = string::resourceId();
          e_var_string( DebugBuildConfiguration   ) = string::resourceId();
          e_var_string( DebugNativeBuildConfig    ) = string::resourceId();
          e_var_string( BuildConfigurationList    ) = string::resourceId();
          e_var_string( BuildNativeTarget         ) = string::resourceId();
          e_var_string( EmbedFrameworks           ) = string::resourceId();
          e_var_string( FrameworkNativeTarget     ) = string::resourceId();
          e_var_string( PublicHeadersBuildPhase   ) = string::resourceId();
          e_var_string( ResourcesBuildPhase       ) = string::resourceId();
          e_var_string( FrameworkBuildPhase       ) = string::resourceId();
          e_var_string( HeadersBuildPhase         ) = string::resourceId();
          e_var_string( SourcesBuildPhase         ) = string::resourceId();
          e_var_string( VariantBuildPhase         ) = string::resourceId();
          e_var_string( ProductFileRef            ) = string::resourceId();
          e_var_string( ProjectObject             ) = string::resourceId();
          e_var_string( ReferencesGroup           ) = string::resourceId();
          e_var_string( ResourcesGroup            ) = string::resourceId();
          e_var_string( CodeGroup                 ) = string::resourceId();
          e_var_string( FrameworkGroup            ) = string::resourceId();
          e_var_string( ProductsGroup             ) = string::resourceId();
          e_var_string( IncludeGroup              ) = string::resourceId();
          e_var_string( ResGroup                  ) = string::resourceId();
          e_var_string( SrcGroup                  ) = string::resourceId();
          e_var_string( MainGroup                 ) = string::resourceId();
          e_var_string( ProductBundleId           );
          e_var1(    a, Unity                     );

          void writePBXBuildFileSection(            Writer& )const;
          void writePBXCopyFilesBuildPhaseSection(  Writer& )const;
          void writePBXFileReferenceSection(        Writer& )const;
          void writePBXFrameworksBuildPhaseSection( Writer& )const;
          void writePBXGroupSection(                Writer& )const;
          void writePBXNativeTargetSection(         Writer& )const;
          void writePBXProjectSection(              Writer& )const;
          void writePBXResourcesBuildPhaseSection(  Writer& )const;
          void writePBXHeadersBuildPhaseSection(    Writer& )const;
          void writePBXSourcesBuildPhaseSection(    Writer& )const;
          void writePBXVariantGroupSection(         Writer& )const;
          void writeXCBuildConfigurationSection(    Writer& )const;
          void writeXCConfigurationListSection(     Writer& )const;
        };

        struct MSVC final:Project{

          e_reflect_no_properties( MSVC, Project );

          //--------------------------------------|-----------------------------
          //Methods:{                             |

            virtual void serialize( Writer& )const override;

          //}:                                    |
          //--------------------------------------|-----------------------------
        };

      //}:                                        |
      //Methods:{                                 |

        virtual void serialize( Writer& )const override;

      //}:                                        |
      //------------------------------------------|-----------------------------

    private:

      e_var_handle_vector1( Project );
      e_var_string(         TypeID  );
      e_var_string(         Name    );
    };

  //}:                                            |
  //Generator:{                                   |

    struct Generator final:Object{

      e_reflect_no_properties( Generator, Object );

      //------------------------------------------|-----------------------------
      //Aliases:{                                 |

        using Source = Workspace::Project::Source;

      //}:                                        |
      //Methods:{                                 |

        e_noinline void xcodeSortingHat( const string& in_path ){
          const auto& path = Workspace::Project::File( in_path );
          const auto& ext = path.ext().tolower();
          switch( ext.hash() ){

            //------------------------------------------------------------------
            // Platform specific file types.
            //------------------------------------------------------------------

            #if e_compiling( osx )
              case e_hashstr64_const( ".framework" ):
                m_pProject->inSources( Source::kFramework ).push( path );
                break;
              case e_hashstr64_const( ".storyboard" ):
                m_pProject->inSources( Source::kStoryboard ).push( path );
                break;
              case e_hashstr64_const( ".xcassets" ):
                m_pProject->inSources( Source::kXcasset ).push( path );
                break;
              case e_hashstr64_const( ".prefab" ):
                m_pProject->inSources( Source::kPrefab ).push( path );
                break;
              case e_hashstr64_const( ".eon" ):
                m_pProject->inSources( Source::kEon ).push( path );
                break;
              case e_hashstr64_const( ".lproj" ):
                m_pProject->inSources( Source::kLproj ).push( path );
                break;
              case e_hashstr64_const( ".plist" ):
                m_pProject->inSources( Source::kPlist ).push( path );
                break;
              case e_hashstr64_const( ".xib" ):
                m_pProject->inSources( Source::kXib ).push( path );
                break;
              case e_hashstr64_const( ".rtf" ):
                m_pProject->inSources( Source::kRtf ).push( path );
                break;
              case e_hashstr64_const( ".dylib" ):
                m_pProject->inSources( Source::kSharedlib ).push( path );
                break;
              case e_hashstr64_const( ".a" ):
                m_pProject->inSources( Source::kStaticlib ).push( path );
                break;
              case e_hashstr64_const( ".mm" ):
                m_pProject->inSources( Source::kMm ).push( path );
                break;
              case e_hashstr64_const( ".m" ):
                m_pProject->inSources( Source::kM ).push( path );
                break;
            #elif e_compiling( microsoft )
              case e_hashstr64_const( ".lib" ):
                m_pProject->inSources( Source::kStaticlib ).push( path );
                break;
            #endif

            //------------------------------------------------------------------
            // Source and header file types.
            //------------------------------------------------------------------

            case e_hashstr64_const( ".png" ):
              m_pProject->inSources( Source::kPng ).push( path );
              break;
            case e_hashstr64_const( ".inl" ):
              m_pProject->inSources( Source::kInl ).push( path );
              break;
            case e_hashstr64_const( ".hpp" ):
            case e_hashstr64_const( ".hxx" ):
            case e_hashstr64_const( ".hh" ):
              m_pProject->inSources( Source::kHpp ).push( path );
              break;
            case e_hashstr64_const( ".cpp" ):
            case e_hashstr64_const( ".cxx" ):
            case e_hashstr64_const( ".cc" ):
              m_pProject->inSources( Source::kCpp ).push( path );
              break;
            case e_hashstr64_const( ".h" ):
              m_pProject->inSources( Source::kH ).push( path );
              break;
            case e_hashstr64_const( ".c" ):
              m_pProject->inSources( Source::kC ).push( path );
              break;
            default:
              #if 0
                e_warnsf( "Ignoring %s!", ccp( path ));
              #endif
              return;
          }
          #if 0
            e_msgf( "  Found %s", ccp( path ));
          #endif
        }

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
                        switch( f.hash() ){
                          case e_hashstr64_const( ".DS_Store" ):
                            return;
                        }
                        if( isDirectory ){
                          const auto& d_ext = f.tolower().ext();
                          if( !d_ext.empty() ){
                            xcodeSortingHat( d+f );
                          }
                        }else{
                          xcodeSortingHat( d+f );
                        }
                      }
                    );
                  }else{
                    xcodeSortingHat( innerPath );
                  }
                }
              );
            }
          }
          return true;
        }

      //}:                                        |
      //------------------------------------------|-----------------------------

      Generator( Workspace::Project* pProject )
        : m_pProject( pProject )
      {}

      Generator() = default;
    ~ Generator() = default;

    private:

      Workspace::Project* m_pProject = nullptr;
    };

  //}:                                            |
//}:                                              |
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_extends( Workspace::Project );
  e_extends( Workspace::Xcode );
  e_extends( Workspace );
  e_extends( Generator );

//}:                                              |
//Private:{                                       |
  //findFramework:{                               |

    namespace{
      string anon_findFramework( const Workspace::Project::File& f ){
        if( *f == '~' ){
          return f.os();
        }
        if( *f == '.' ){
          return f.os();
        }
        if( *f == '/' ){
          return f.os();
        }
        return( "../" + f ).os();
      }
    }

  //}:                                            |
  //writeFileReference:{                          |

    namespace{
      void anon_writeFileReference( Writer& fs, const Workspace::Project::Files& files, const string& projectType ){
        auto paths = files;
        paths.sort(
          []( const Workspace::Project::File& a, const Workspace::Project::File& b ){
            return( a.filename().tolower() < b.filename().tolower() );
          }
        );
        files.foreach(
          [&]( const Workspace::Project::File& f ){
            fs << "    "
              + f.toRefID()
              + " = {isa = PBXFileReference; lastKnownFileType = "
              + projectType
              + "; path = ../"
              + f
              + "; name = "
              + f.filename()
              + "; sourceTree = \"<group>\"; };\n"
            ;
          }
        );
      }
    }

  //}:                                            |
  //ignoreFile?:{                                 |

    namespace{
      bool anon_ignoreFile( const string& regex, const string& s ){
        if( regex.empty() ){
          return false;
        }
        const std::regex r( regex.c_str() );
        const std::string var( s );
        auto it = var.cbegin();
        std::smatch sm;
        if( std::regex_search( it, var.cend(), sm, r )){
          return true;
        }
        return false;
      }
    }

  //}:                                            |
  //saveProject:{                                 |

    namespace{
      void anon_saveProject( const string& filename, const Workspace::Project& proj ){
        switch( proj.toTypeID().hash() ){
          case e_hashstr64_const( "pbx" ):
            { auto* ss = strdup( filename.path() );
              auto* ee = strchr( ss, 0 )-2;
              while( ee > ss ){
                if( *ee == '/' ){
                  break;
                }
                --ee;
              }
              *ee = 0;
              const string dirName = string( ss, ee ) + "/" + proj.toLabel() + ".xcodeproj";
              free( cp( ss ));
              e_rm( dirName );
              e_md( dirName );
              fs::Writer fs( dirName + "/project.pbxproj", fs::kTEXT );
              proj.serialize( fs );
              fs.save();
            }
            break;
        }
      }
    }

  //}:                                            |
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
      #if e_compiling( osx )
        void lua_gather( lua_State* L, Workspace::Xcode& p ){
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
                    Workspace::Project::File f( header );
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
      #elif e_compiling( microsoft )
        void lua_gather( lua_State* L, Workspace::MSVC& p ){
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
      #endif
      void lua_gather( lua_State* L, Workspace::Projects& v ){
        lua_pushnil( L );
        while( lua_next( L, -2 )){
          const string& key = lua_tostring( L, -2 );
          #if e_compiling( osx )
            Workspace::Xcode::handle hProject = e_new( Workspace::Xcode );
          #elif e_compiling( microsoft )
            Workspace::MSVC::handle hProject = e_new( Workspace::MSVC );
          #endif
          auto& p = hProject.cast();
          p.setLabel( key );
          lua_gather( L, p );
          v.push( hProject.as<Workspace::Project>() );
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
        if( hObject.isa<Workspace>() ){
          const auto& workspace = hObject.as<Workspace>().cast();
          if( workspace.toName().empty() ){
            lua_pushboolean( L, false );
            return 1;
          }
          #if e_compiling( osx )
            const auto& dirName = path + "/" + workspace.toName() + ".xcworkspace";
            e_rm( dirName );
            e_md( dirName );
            { fs::Writer fs( dirName + "/contents.xcworkspacedata", fs::kTEXT );
              workspace.serialize( fs );
              fs.save();
            }
            //verifyPBX( path + "/eon.xcodeproj/project.pbxproj" );
          #elif e_compiling( microsoft )
            const auto& dirName = path + "/" + workspace.toName() + ".sln";
            { fs::Writer fs( dirName, fs::kTEXT );
              workspace.serialize( fs );
              fs.save();
            }
            //verifySolution( dirName );
          #endif
        }
        lua_pushboolean( L, true );
        return 1;
      }
    }

  //}:                                            |
//}:                                              |
//Methods:{                                       |
  //[workspace]:{                                 |
    //serialize:{                                 |

#ifdef __APPLE__
  #pragma mark - Methods -
#endif

      void Workspace::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Generate workspace on macOS.
        //----------------------------------------------------------------------

        #if e_compiling( osx )

          switch( m_sTypeID.hash() ){

            //------------------------------------------------------------------
            // Makefile workspace; means a Makefile file.
            //------------------------------------------------------------------

            case e_hashstr64_const( "makefile" ):
              break;

            //------------------------------------------------------------------
            // Cmake workspace; means a root CMakeLists.txt with cotire.
            //------------------------------------------------------------------

            case e_hashstr64_const( "cmake" ):
              break;

            //------------------------------------------------------------------
            // XML workspace; means a .xcworkspace package.
            //------------------------------------------------------------------

            case e_hashstr64_const( "xml" ):
              fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
              fs << "<Workspace\n";
              fs << "  version = \"1.0\">\n";

              //----------------------------------------------------------------
              // Construct xcodeproj's for libraries.
              //----------------------------------------------------------------

              fs << "  <Group\n";
              fs << "    location = \"container:\"\n";
              fs << "    name = \"Libraries\">\n";
              auto it = m_vProjects.getIterator();
              while( it ){
                const auto& proj = it->cast();
                switch( proj.toBuild().tolower().hash() ){
                  case e_hashstr64_const( "framework" ):
                  case e_hashstr64_const( "shared" ):
                  case e_hashstr64_const( "static" ):
                    fs << "  <FileRef\n";
                    fs << "    location = \"group:" + proj.toLabel() + ".xcodeproj\">\n";
                    fs << "  </FileRef>\n";
                    anon_saveProject( fs.toFilename(), proj );
                    break;
                }
                ++it;
              }
              fs << "  </Group>\n";

              //----------------------------------------------------------------
              // Construct xcodeproj's for libraries.
              //----------------------------------------------------------------

              fs << "  <Group\n";
              fs << "    location = \"container:\"\n";
              fs << "    name = \"Apps\">\n";
              it = m_vProjects.getIterator();
              while( it ){
                const auto& proj = it->cast();
                switch( proj.toBuild().tolower().hash() ){
                  case e_hashstr64_const( "application" ):
                  case e_hashstr64_const( "console" ):
                    fs << "  <FileRef\n";
                    fs << "    location = \"group:"+proj.toLabel()+".xcodeproj\">\n";
                    fs << "  </FileRef>\n";
                    anon_saveProject( fs.toFilename(), proj );
                    break;
                }
                ++it;
              }
              fs << "  </Group>\n";
              fs << "</Workspace>\n";
              break;
          }
        #endif

        //----------------------------------------------------------------------
        // Generate Visual Studio 2019 on Windows 10.
        //----------------------------------------------------------------------

        #if e_compiling( microsoft )
          // TODO: Implement visual studio solution.
        #endif
      }

    //}:                                          |
  //}:                                            |
  //[project]:{                                   |
    //serialize:{                                 |

      #if e_compiling( osx )

        void Workspace::Xcode::serialize( Writer& fs )const{

          //--------------------------------------------------------------------
          // Populate build files across unity space.
          //--------------------------------------------------------------------

          if(( IEngine::args.size() == 3 ) && ( IEngine::args[ 2 ].hash() == e_hashstr64_const( "--unity" ))){
            u32 i = 0;
            inSources( Source::kCpp ).foreach(
              [&]( const File& f ){
                const_cast<Xcode*>( this )->m_aUnity[ 0/* c++ */][ ++i % 5 ].push( f );
              }
            );
            inSources( Source::kMm ).foreach(
              [&]( const File& f ){
                const_cast<Xcode*>( this )->m_aUnity[ 1/* obj-c++ */][ ++i % 5 ].push( f );
              }
            );
            inSources( Source::kC ).foreach(
              [&]( const File& f ){
                const_cast<Xcode*>( this )->m_aUnity[ 2/* c */][ ++i % 5 ].push( f );
              }
            );
            inSources( Source::kM ).foreach(
              [&]( const File& f ){
                const_cast<Xcode*>( this )->m_aUnity[ 3/* obj-c */][ ++i % 5 ].push( f );
              }
            );
            const string& output = "tmp/";
            //
            //  C++ unity files.
            //
            if( !inSources( Source::kCpp ).empty() ){
              const auto disableUnity=( nullptr != toDisableOptions().tolower().find( "unity" ));
              const_cast<Xcode*>( this )->inSources( Source::kCpp ).clear();
              for( u32 i=0; i<5; ++i ){
                if(( m_aUnity[ 0/* c++ */][ i ].size() < 2 )|| disableUnity ){
                  const_cast<Xcode*>( this )->inSources( Source::kCpp ).pushVector( m_aUnity[ 0/* c++ */][ i ]);
                  continue;
                }
                Writer cpp( output + string::resourceId() + ".cpp", kTEXT );
                const_cast<Xcode*>( this )->inSources( Source::kCpp ).push( cpp.toFilename() );
                m_aUnity[ 0/* cpp++ */][ i ].foreach(
                  [&]( const File& f ){
                    const auto& unity = toSkipUnity();
                    const auto& skipUnity = unity.splitAtCommas();
                    bool bSkip = false;
                    skipUnity.foreachs(
                      [&]( const string& skip ){
                        if( strstr( f, skip )){
                          bSkip = true;
                          return false;
                        }
                        return true;
                      }
                    );
                    if( bSkip ){
                      const_cast<Xcode*>( this )->inSources( Source::kCpp ).push( f );
                      printf( "Skipped %s from unity build...\n", ccp( f ));
                    }else{
                      cpp.write( "#include\"../" + f + "\"\n" );
                    }
                  }
                );
                cpp.save();
              }
            }
            //
            //  C unity files.
            //
            if( !inSources( Source::kC ).empty() ){
              const_cast<Xcode*>( this )->inSources( Source::kC ).clear();
              for( u32 i=0; i<5; ++i ){
                if( m_aUnity[ 2/* c */][ i ].size() < 2 ){
                  const_cast<Xcode*>( this )->inSources( Source::kC ).pushVector( m_aUnity[ 2/* c */][ i ]);
                  continue;
                }
                Writer c( output + string::resourceId() + ".c", kTEXT );
                const_cast<Xcode*>( this )->inSources( Source::kC ).push( c.toFilename() );
                m_aUnity[ 2/* c */][ i ].foreach(
                  [&]( const File& f ){
                    auto unity = toSkipUnity();
                    unity.replace( "\n", "" );
                    unity.replace( " ", "" );
                    const auto& skipUnity = unity.splitAtCommas();
                    bool bSkip = false;
                    skipUnity.foreachs(
                      [&]( const string& skip ){
                        if( strstr( f, skip )){
                          bSkip = true;
                          return false;
                        }
                        return true;
                      }
                    );
                    if( bSkip ){
                      const_cast<Xcode*>( this )->inSources( Source::kC ).push( f );
                      printf( "Skipped %s from unity build...\n", ccp( f ));
                    }else{
                      c.write( "#include\"../" + f + "\"\n" );
                    }
                  }
                );
                c.save();
              }
            }
            //
            //  Objective-C++ unity files
            //
            if( !inSources( Source::kMm ).empty() ){
              const_cast<Xcode*>( this )->inSources( Source::kMm ).clear();
              for( u32 i=0; i<5; ++i ){
                if( m_aUnity[ 1/* mm */][ i ].size() < 2 ){
                  const_cast<Xcode*>( this )->inSources( Source::kMm ).pushVector( m_aUnity[ 1/* mm */][ i ]);
                  continue;
                }
                Writer mm( output + string::resourceId() + ".mm", kTEXT );
                const_cast<Xcode*>( this )->inSources( Source::kMm ).push( mm.toFilename() );
                m_aUnity[ 1/* mm */][ i ].foreach(
                  [&]( const File& f ){
                    auto unity = toSkipUnity();
                    unity.replace( "\n", "" );
                    unity.replace( " ", "" );
                    const auto& skipUnity = unity.splitAtCommas();
                    bool bSkip = false;
                    skipUnity.foreachs(
                      [&]( const string& skip ){
                        if( strstr( f, skip )){
                          bSkip = true;
                          return false;
                        }
                        return true;
                      }
                    );
                    if( bSkip ){
                      const_cast<Xcode*>( this )->inSources( Source::kMm ).push( f );
                      printf( "Skipped %s from unity build...\n", ccp( f ));
                    }else{
                      mm.write( "#include\"../" + f + "\"\n" );
                    }
                  }
                );
                mm.save();
              }
            }
            //
            //  Objective-C unity files.
            //
            if( !inSources( Source::kM ).empty() ){
              const_cast<Xcode*>( this )->inSources( Source::kM ).clear();
              for( u32 i=0; i<5; ++i ){
                if( m_aUnity[ 3/* m */][ i ].size() < 2 ){
                  const_cast<Xcode*>( this )->inSources( Source::kM ).pushVector( m_aUnity[ 3/* m */][ i ]);
                  continue;
                }
                Writer m( output + string::resourceId() + ".m", kTEXT );
                const_cast<Xcode*>( this )->inSources( Source::kM ).push( m.toFilename() );
                m_aUnity[ 3/* m */][ i ].foreach(
                  [&]( const File& f ){
                    auto unity = toSkipUnity();
                    unity.replace( "\n", "" );
                    unity.replace( " ", "" );
                    const auto& skipUnity = unity.splitAtCommas();
                    bool bSkip = false;
                    skipUnity.foreachs(
                      [&]( const string& skip ){
                        if( strstr( f, skip )){
                          bSkip = true;
                          return false;
                        }
                        return true;
                      }
                    );
                    if( bSkip ){
                      const_cast<Xcode*>( this )->inSources( Source::kM ).push( f );
                      printf( "Skipped %s from unity build...\n", ccp( f ));
                    }else{
                      m.write( "#include\"../" + f + "\"\n" );
                    }
                  }
                );
                m.save();
              }
            }
          }

          //--------------------------------------------------------------------
          // Save Xcode project to pbx format bundle.
          //--------------------------------------------------------------------

          fs << "// !$*UTF8*$!\n";
          fs << "{\n";
          fs << "  archiveVersion = 1;\n";
          fs << "  classes = {\n";
          fs << "  };\n";
          fs << "  objectVersion = 50;\n";
          fs << "  objects = {\n";
          writePBXBuildFileSection(            fs );//ok
          writePBXCopyFilesBuildPhaseSection(  fs );
          writePBXFileReferenceSection(        fs );//ok
          writePBXFrameworksBuildPhaseSection( fs );
          writePBXGroupSection(                fs );
          writePBXNativeTargetSection(         fs );
          writePBXProjectSection(              fs );
          writePBXResourcesBuildPhaseSection(  fs );
          writePBXHeadersBuildPhaseSection(    fs );
          writePBXSourcesBuildPhaseSection(    fs );
          writePBXVariantGroupSection(         fs );
          writeXCBuildConfigurationSection(    fs );
          writeXCConfigurationListSection(     fs );
          fs << "  };\n";
          fs << "  rootObject = " + m_sProjectObject + " /* Project object */;\n";
          fs << "}\n";
        }
      #elif e_compiling( microsoft )
        void Workspace::MSVC::serialize( Writer& fs )const{
        }
      #endif

    //}:                                          |
    //write*:{                                    |

      #if e_compiling( osx )

        void Workspace::Xcode::writePBXBuildFileSection( Writer& fs )const{
          fs << "\n    /* Begin PBXBuildFile section */\n";

            //------------------------------------------------------------------
            // Linking with.
            //------------------------------------------------------------------

            Files files;
            if( !toLinkWith().empty() ){
              files.clear();
              const auto& libs = toLinkWith().splitAtCommas();
              libs.foreach(
                [&]( const string& lib ){
                  if( lib.empty() ){
                    return;
                  }
                  if( *lib == '.' ){
                    files.push( File( lib.os() ));
                  }else if( *lib == '/' ){
                    files.push( File( lib.os() ));
                  }else if( *lib == '~' ){
                    files.push( File( lib.os() ));
                  }else{
                    files.push( File(( "../" + lib ).os() ));
                  }
                }
              );
              const_cast<Xcode*>( this )->setLibFiles( files );
              files.foreach(
                [&]( File& file ){
                  if( file.empty() ){
                    return;
                  }
                  fs << "    "
                    + file.toBuildID()
                    + " /* "
                    + file.filename()
                    + " in Frameworks */ = {isa = PBXBuildFile; fileRef = "
                    + file.toRefID()
                    + " /* "
                    + file.filename()
                    + " in Frameworks */; };\n"
                  ;
                }
              );
            }

            //------------------------------------------------------------------
            // Resource files.
            //------------------------------------------------------------------

            files.clear();
            files.pushVector( inSources( Source::kXcasset ));
            files.pushVector( inSources( Source::kPrefab  ));
            files.pushVector( inSources( Source::kLproj   ));
            files.pushVector( inSources( Source::kPlist   ));
            files.foreach(
              [&]( File& file ){
                if( file.empty() ){
                  return;
                }
                fs << "    "
                  + file.toBuildID()
                  + " /* "
                  + file.filename()
                  + " in Resources */ = {isa = PBXBuildFile; fileRef = "
                  + file.toRefID()
                  + " /* "
                  + file.filename()
                  + " */; };\n"
                ;
              }
            );

            //------------------------------------------------------------------
            // Source files.
            //------------------------------------------------------------------

            files.clear();
            files.pushVector( inSources( Source::kCpp ));
            files.pushVector( inSources( Source::kMm  ));
            files.pushVector( inSources( Source::kM   ));
            files.pushVector( inSources( Source::kC   ));
            files.foreach(
              [&]( File& file ){
                if( file.empty() ){
                  return;
                }
                if( anon_ignoreFile( toIgnoreParts(), file )){
                  e_msgf( "Ignoring header %s because regex = \"%s\"", ccp( file.filename() ), ccp( toIgnoreParts() ));
                  return;
                }
                fs << "    "
                  + file.toBuildID()
                  + " /* "
                  + file.filename()
                  + " in Sources */ = {isa = PBXBuildFile; fileRef = "
                  + file.toRefID()
                  + " /* "
                  + file.filename()
                  + " */; };\n"
                ;
              }
            );
          fs << "    /* End PBXBuildFile section */\n";
        }
        void Workspace::Xcode::writePBXCopyFilesBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXCopyFilesBuildPhase section */\n";
          if( toBuild().hash() == e_hashstr64_const( "application" )){
            if( !toEmbedFiles().empty() ){
              fs << "    " + toEmbedFrameworks() + " /* Embed Frameworks */ = {\n";
              fs << "      isa = PBXCopyFilesBuildPhase;\n";
              fs << "      buildActionMask = 2147483647;\n";
              fs << "      dstPath = \"\";\n";
              fs << "      dstFolderSpec = 10;\n";
              fs << "      files = (\n";
              toEmbedFiles().foreach(
                [&]( const File& file ){
                  fs << "    "
                    + file.toBuildID()
                    + " /* "
                    + file.filename()
                    + " in Embed Frameworks */,\n"
                  ;
                }
              );
              fs << "      );\n";
              fs << "      name = \"Embed Frameworks\";\n";
              fs << "      runOnlyForDeploymentPostProcessing = 0;\n";
              fs << "    };\n";
            }
          }
          fs << "    /* End PBXCopyFilesBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXFileReferenceSection( Writer& fs )const{
          fs << "\n    /* Begin PBXFileReference section */\n";
          anon_writeFileReference( fs, inSources( Source::kXcasset ), "folder.assetcatalog" );
          anon_writeFileReference( fs, inSources( Source::kPrefab  ), "file"                );
          anon_writeFileReference( fs, inSources( Source::kLproj   ), "folder"              );
          anon_writeFileReference( fs, inSources( Source::kPlist   ), "text.plist.xml"      );
          anon_writeFileReference( fs, inSources( Source::kHpp     ), "sourcecode.cpp.h"    );
          anon_writeFileReference( fs, inSources( Source::kInl     ), "sourcecode.cpp.h"    );
          anon_writeFileReference( fs, inSources( Source::kH       ), "sourcecode.c.h"      );
          anon_writeFileReference( fs, inSources( Source::kCpp     ), "sourcecode.cpp.cpp"  );
          anon_writeFileReference( fs, inSources( Source::kMm      ), "sourcecode.cpp.objc" );
          anon_writeFileReference( fs, inSources( Source::kM       ), "sourcecode.c.objc"   );
          anon_writeFileReference( fs, inSources( Source::kC       ), "sourcecode.c.c"      );
          toPublicHeaders().foreach(
            [&]( const File& f ){
              string lastKnownFileType;
              switch( f.tolower().ext().hash() ){
                case e_hashstr64_const( ".h" ):
                  lastKnownFileType = "sourcecode.c.h";
                  break;
                default:
                  lastKnownFileType = "folder";
                  break;
              }
              fs << "    "
                + f.toRefID()
                + " = {isa = PBXFileReference; lastKnownFileType = "
                + lastKnownFileType
                + "; path = ../"
                + f
                + "; name = "
                + f.filename()
                + "; sourceTree = \"<group>\"; };\n"
              ;
            }
          );
          if( toBuild().hash() == e_hashstr64_const( "application" )){
            toEmbedFiles().foreach(
              [&]( const File& f ){
                string lastKnownFileType;
                switch( f.ext().tolower().hash() ){
                  case e_hashstr64_const( ".framework" ):
                    lastKnownFileType = "wrapper.framework";
                    break;
                  case e_hashstr64_const( ".dylib" ):
                    lastKnownFileType = "\"compiled.mach-o.dylib\"";
                    break;
                  default:
                    return;
                }
                fs << "    "
                  + f.toRefID()
                  + " = {isa = PBXFileReference; lastKnownFileType = "
                  + lastKnownFileType
                  + "; path = "
                  + anon_findFramework( f )
                  + "; name = "
                  + f.filename()
                  + "; sourceTree = \"<group>\"; };\n"
                ;
              }
            );
          }
          toLibFiles().foreach(
            [&]( const File& f ){
              string lastKnownFileType;
              switch( f.ext().tolower().hash() ){
                case e_hashstr64_const( ".framework" ):
                  lastKnownFileType = "wrapper.framework";
                  break;
                case e_hashstr64_const( ".dylib" ):
                  lastKnownFileType = "\"compiled.mach-o.dylib\"";
                  break;
                case e_hashstr64_const( ".a" ):
                  lastKnownFileType = "archive.ar";
                  break;
              }
              fs << "    "
                + f.toRefID()
                + " = {isa = PBXFileReference; lastKnownFileType = "
                + lastKnownFileType
                + "; path = "
                + f.os()
                + "; name = "
                + f.filename();
              if( toBuild().hash() != e_hashstr64_const( "application" )){
                fs << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
              }else{
                fs << "; sourceTree = \"<group>\"; };\n";
              }
            }
          );
          switch( toBuild().hash() ){
            case e_hashstr64_const( "framework" ):
              fs << "    "
                + m_sProductFileRef
                + " /* "
                + toLabel()
                + ".framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = "
                + toLabel()
                + ".framework; sourceTree = BUILT_PRODUCTS_DIR; };\n";
              break;
            case e_hashstr64_const( "static" ):
              fs << "    "
                + m_sProductFileRef
                + " /* lib"
                + toLabel()
                + ".a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = lib"
                + toLabel()
                + ".a; sourceTree = BUILT_PRODUCTS_DIR; };\n";
              break;
            case e_hashstr64_const( "application" ):
              fs << "    "
                + m_sProductFileRef
                + " /* "
                + toLabel()
                + " */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = "
                + toLabel()
                + "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
              break;
            case e_hashstr64_const( "console" ):
              fs << "    "
                + m_sProductFileRef
                + " /* "
                + toLabel()
                + " */ = {isa = PBXFileReference; explicitFileType = compiled.mach-o.executable; includeInIndex = 0; path = "
                + toLabel()
                + "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
              break;
          }
          fs << "    /* End PBXFileReference section */\n";
        }
        void Workspace::Xcode::writePBXFrameworksBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXFrameworksBuildPhase section */\n";
          if( !toLibFiles().empty() ){
            fs << "    " + m_sFrameworkBuildPhase + " /* frameworks */ = {\n"
                + "      isa = PBXFrameworksBuildPhase;\n"
                + "      buildActionMask = 2147483647;\n"
                + "      files = (\n";
            toLibFiles().foreach(
              [&]( const File& f ){
                fs << "        " + f.toBuildID() + " /* " + f.filename() + " */,\n";
              }
            );
            fs << string( "      );\n" )
              + "      runOnlyForDeploymentPostprocessing = 0;\n"
              + "    };\n"
            ;
          }
          fs << "    /* End PBXFrameworksBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXGroupSection( Writer& fs )const{
          fs << "\n    /* Begin PBXGroup section */\n";

            //------------------------------------------------------------------
            // Main top-level group.
            //------------------------------------------------------------------

            fs << "    " + m_sMainGroup + " = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n";
            if( !toLibFiles().empty() ){
              fs << "        " + m_sFrameworkGroup + " /* Frameworks */,\n";
            }
            fs << "        " + m_sProductsGroup  + " /* Products */,\n"
                + "        " + m_sCodeGroup + " /* Code */,\n"
                + "      );\n"
                + "      sourceTree = \"<group>\";\n"
                + "    };\n";

            //------------------------------------------------------------------
            // Products group.
            //------------------------------------------------------------------

            fs << "    " + m_sProductsGroup + " /* Products */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n"
                + "        " + m_sProductFileRef + " /* " + toLabel() + ".framework */,\n"
                + "      );\n"
                + "      name = Products;\n"
                + "      sourceTree = \"<group>\";\n"
                + "    };\n";
            Files files;
            if( !toIncPath().empty() ){
              fs << "    " + m_sIncludeGroup + " /* include */ = {\n"
                  + "      isa = PBXGroup;\n"
                  + "      children = (\n";
              files.pushVector( inSources( Source::kHpp ));
              files.pushVector( inSources( Source::kInl ));
              files.pushVector( inSources( Source::kH   ));
              files.sort(
                []( const File& a, const File& b ){
                  return( a.filename().tolower() < b.filename().tolower() );
                }
              );
              files.foreach(
                [&]( const File& file ){
                  fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
                }
              );
              fs << "      );\n";
              fs << "      path = \"\";\n";
              fs << "      name = " + toIncPath().basename() + ";\n";
              fs << "      sourceTree = \"<group>\";\n";
              fs << "    };\n";
              if( !toLibFiles().empty() || !toEmbedFiles().empty() ){
                fs << "    " + m_sFrameworkGroup + " /* Frameworks */ = {\n"
                    + "      isa = PBXGroup;\n"
                    + "      children = (\n";
                toLibFiles().foreach(
                  [&]( const File& f ){
                    fs << "        " + f.toRefID() + " /* " + f.filename() + " */,\n";
                  }
                );
                toEmbedFiles().foreach(
                  [&]( const File& f ){
                    fs << "        " + f.toRefID() + " /* " + f.filename() + " */,\n";
                  }
                );
                fs << string( "      );\n" )
                  + "      name = Frameworks;\n"
                  + "      sourceTree = \"<group>\";\n"
                  + "    };\n"
                ;
              }
            }

            //------------------------------------------------------------------
            // Resources group.
            //------------------------------------------------------------------

            files.clear();
            files.pushVector( inSources( Source::kXcasset ));
            files.pushVector( inSources( Source::kPrefab  ));
            files.pushVector( inSources( Source::kLproj   ));
            fs << "    " + m_sCodeGroup + " /* Code */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n"
                + "        " + m_sReferencesGroup + " /* references */,\n";
            if( !files.empty() ){
              fs << "        " + m_sResourcesGroup + " /* resources */,\n";
            }
            fs << "        " + m_sIncludeGroup + " /* include */,\n"
                + "        " + m_sSrcGroup + " /* src */,\n"
                + "      );\n"
                + "      name = Code;\n"
                + "      sourceTree = \"<group>\";\n"
                + "    };\n";
            if( !files.empty() ){
              fs << "    " + m_sResourcesGroup + " /* resources */ = {\n"
                  + "      isa = PBXGroup;\n"
                  + "      children = (\n";
              files.sort(
                []( const File& a, const File& b ){
                  return( a.filename().tolower() < b.filename().tolower() );
                }
              );
              files.foreach(
                [&]( const File& file ){
                  fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
                }
              );
              fs << "      );\n";
              fs << "      name = resources;\n";
              fs << "      sourceTree = \"<group>\";\n";
              fs << "    };\n";
            }

            //------------------------------------------------------------------
            // Exporting public headers from framework.
            //------------------------------------------------------------------

            if( !toPublicHeaders().empty() ){
              fs << "    " + m_sReferencesGroup + " /* references */ = {\n"
                  + "      isa = PBXGroup;\n"
                  + "      children = (\n";
              files.clear();
              files.pushVector( toPublicHeaders() );
              files.foreach(
                [&]( const File& file ){
                  fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
                }
              );
              fs << "      );\n";
              fs << "      path = \"\";\n";
              fs << "      name = references;\n";
              fs << "      sourceTree = \"<group>\";\n";
              fs << "    };\n";
            }
            fs << "    " + m_sSrcGroup + " /* src */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n";

            //------------------------------------------------------------------
            // Adding source files.
            //------------------------------------------------------------------

            files.clear();
            files.pushVector( inSources( Source::kCpp ));
            files.pushVector( inSources( Source::kMm  ));
            files.pushVector( inSources( Source::kC   ));
            files.pushVector( inSources( Source::kM   ));
            files.sort(
              []( const File& a, const File& b ){
                return( a.filename().tolower() < b.filename().tolower() );
              }
            );
            files.foreach(
              [&]( const File& file ){
                fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      path = \"\";\n";
            fs << "      name = src;\n";// + toSrcPath().basename() + ";\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
          fs << "    /* End PBXGroup section */\n";
        }
        void Workspace::Xcode::writePBXNativeTargetSection( Writer& fs )const{
          fs << "\n    /* Begin PBXNativeTarget section */\n";
          fs << "    " + m_sFrameworkNativeTarget + " /* framework_test */ = {\n"
              + "      isa = PBXNativeTarget;\n"
              + "      buildConfigurationList = " + m_sBuildNativeTarget + " /* Build configuration list for PBXNativeTarget \"" + toLabel() + "\" */;\n"
              + "      buildPhases = (\n"
              + "        " + m_sFrameworkBuildPhase + " /* frameworks */,\n"
              + "        " + m_sResourcesBuildPhase + " /* resources */,\n"
              + "        " + m_sHeadersBuildPhase   + " /* include */,\n"
              + "        " + m_sSourcesBuildPhase   + " /* src */,\n";
          if( !toEmbedFrameworks().empty() ){
            fs << "        " + m_sEmbedFrameworks + " /* Embed Frameworks */,\n";
          }
          fs << string( "      );\n" )
              + "      buildRules = (\n"
              + "      );\n"
              + "      dependencies = (\n"
              + "      );\n"
              + "      name = " + toLabel() + ";\n"
              + "      productName = " + toLabel() + ";\n";
          switch( toBuild().hash() ){
            case e_hashstr64_const( "framework" ):
              fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".framework */;\n";
              fs << "      productType = \"com.apple.product-type.framework\";\n";
              break;
            case e_hashstr64_const( "static" ):
              fs << "      productReference = " + m_sProductFileRef + " /* lib" + toLabel() + ".a */;\n";
              fs << "      productType = \"com.apple.product-type.library.static\";\n";
              break;
            case e_hashstr64_const( "application" ):
              fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".app */;\n";
              fs << "      productType = \"com.apple.product-type.application\";\n";
              break;
            case e_hashstr64_const( "console" ):
              fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + " */;\n";
              fs << "      productType = \"com.apple.product-type.tool\";\n";
              break;
          }
          fs << "    };\n";
          fs << "    /* End PBXNativeTarget section */\n";
        }
        void Workspace::Xcode::writePBXProjectSection( Writer& fs )const{
          fs << "\n    /* Begin PBXProject section */\n";
          fs << "    " + m_sProjectObject + " /* Project object */ = {\n"
              + "      isa = PBXProject;\n"
              + "      attributes = {\n"
              + "        LastUpgradeCheck = 1120;\n"
              + "        ORGANIZATIONNAME = \"" + toOrgName() + "\";\n"
              + "        TargetAttributes = {\n"
              + "          " + m_sFrameworkNativeTarget + " = {\n"
              + "            CreatedOnToolsVersion = 11.2.1;\n"
              + "          };\n"
              + "        };\n"
              + "      };\n"
              + "      buildConfigurationList = " + m_sBuildConfigurationList + " /* Build configuration list for PBXProject \"" + toLabel() + "\" */;\n"
              + "      compatibilityVersion = \"Xcode 9.3\";\n"
              + "      developmentRegion = en;\n"
              + "      hasScannedForEncodings = 0;\n"
              + "      knownRegions = (\n"
              + "        en,\n"
              + "        Base,\n"
              + "      );\n"
              + "      mainGroup = " + m_sMainGroup + ";\n"
              + "      productRefGroup = " + m_sProductsGroup + " /* Products */;\n"
              + "      projectDirPath = \"\";\n"
              + "      projectRoot = \"\";\n"
              + "      targets = (\n"
              + "        " + m_sFrameworkNativeTarget + " /* " + toLabel() + " */,\n"
              + "      );\n"
              + "    };\n";
          fs << "    /* End PBXProject section */\n";
        }
        void Workspace::Xcode::writePBXHeadersBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXHeadersBuildPhase section */\n";
          fs << "    " + m_sHeadersBuildPhase + " /* Headers */ = {\n"
              + "      isa = PBXHeadersBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          Files files;
          files.pushVector( toPublicHeaders() );
          files.foreach(
            [&]( const File& f ){
              fs << "        " + f.toBuildID() + " /* " + f + " in Reources */,\n";
            }
          );
          fs << "      );\n";
          fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
          fs << "    };\n";
          fs << "    /* End PBXHeadersBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXResourcesBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXResourcesBuildPhase section */\n";
          fs << "    " + m_sResourcesBuildPhase + " /* Resources */ = {\n"
              + "      isa = PBXResourcesBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          Files files;
          files.pushVector( inSources( Source::kXcasset ));
          files.pushVector( inSources( Source::kPrefab  ));
          files.pushVector( inSources( Source::kLproj   ));
          files.pushVector( inSources( Source::kPlist   ));
          files.foreach(
            [&]( const File& f ){
              if( f.empty() ){
                return;
              }
              fs << "        " + f.toBuildID() + " /* " + f + " in Reources */,\n";
            }
          );
          fs << "      );\n";
          fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
          fs << "    };\n";
          fs << "    /* End PBXResourcesBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXVariantGroupSection( Writer& fs )const{
          fs << "\n    /* Begin PBXVariantGroup section */\n";
          Files files;
          files.pushVector( inSources( Source::kStoryboard ));
          files.foreach(
            [&]( const File& f ){
              if( f.empty() ){
                return;
              }
              fs << "    " + f.toBuildID() + " /* " + f.filename() + " */ = {\n"
                  + "      isa = PBXSourcesBuildPhase;\n"
                  + "      buildActionMask = 2147483647;\n"
                  + "      files = (\n";
              fs << "        " + f.toRefID() + " /* " + f + " in Sources */,\n";
              fs << "      );\n";
              fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
              fs << "    };\n";
            }
          );
          fs << "    /* End PBXVariantGroup section */\n";
        }
        void Workspace::Xcode::writePBXSourcesBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXSourcesBuildPhase section */\n";
          fs << "    " + m_sSourcesBuildPhase + " /* Sources */ = {\n"
              + "      isa = PBXSourcesBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          Files files;
          files.pushVector( inSources( Source::kCpp ));
          files.pushVector( inSources( Source::kMm  ));
          files.pushVector( inSources( Source::kC   ));
          files.foreach(
            [&]( const File& f ){
              if( f.empty() ){
                return;
              }
              fs << "        " + f.toBuildID() + " /* " + f + " in Sources */,\n";
            }
          );
          fs << "      );\n";
          fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
          fs << "    };\n";
          fs << "    /* End PBXSourcesBuildPhase section */\n";
        }
        void Workspace::Xcode::writeXCBuildConfigurationSection( Writer& fs )const{
          fs << "\n    /* Begin XCBuildConfiguration section */\n";
          fs << "    " + m_sDebugBuildConfiguration + " /* Debug */ = {\n"
              + "      isa = XCBuildConfiguration;\n"
              + "      buildSettings = {\n"
              + "        ALWAYS_SEARCH_USER_PATHS = NO;\n"
              + "        CLANG_ANALYZER_NONNULL = YES;\n"
              + "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
              + "        CLANG_CXX_LANGUAGE_STANDARD = \"" + toLanguage() + "\";\n"
              + "        CLANG_CXX_LIBRARY = \"libc++\";\n"
              + "        CLANG_ENABLE_MODULES = YES;\n"
              + "        CLANG_ENABLE_OBJC_ARC = YES;\n"
              + "        CLANG_ENABLE_OBJC_WEAK = YES;\n"
              + "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
              + "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
              + "        CLANG_WARN_COMMA = YES;\n"
              + "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
              + "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
              + "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
              + "        CLANG_WARN_DOCUMENTATION_COMMENTS = YES;\n"
              + "        CLANG_WARN_EMPTY_BODY = YES;\n"
              + "        CLANG_WARN_ENUM_CONVERSION = YES;\n"
              + "        CLANG_WARN_INFINITE_RECURSION = YES;\n"
              + "        CLANG_WARN_INT_CONVERSION = YES;\n"
              + "        CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
              + "        CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
              + "        CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
              + "        CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
              + "        CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
              + "        CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
              + "        CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
              + "        CLANG_WARN_UNGUARDED_AVAILABILITY = YES_AGGRESSIVE;\n"
              + "        CLANG_WARN_UNREACHABLE_CODE = YES;\n"
              + "        CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;\n"
              + "        COPY_PHASE_STRIP = NO;\n"
              + "        CURRENT_PROJECT_VERSION = 1;\n"
              + "        DEBUG_INFORMATION_FORMAT = dwarf;\n"
              + "        ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
              + "        ENABLE_TESTABILITY = YES;\n"
              + "        GCC_C_LANGUAGE_STANDARD = gnu11;\n"
              + "        GCC_DYNAMIC_NO_PIC = NO;\n"
              + "        GCC_NO_COMMON_BLOCKS = YES;\n"
              + "        GCC_OPTIMIZATION_LEVEL = 0;\n"
              + "        GCC_PREPROCESSOR_DEFINITIONS = (\n"
              + "          \"$(inherited)\",\n";
          string dbgDefines = toDefinesDbg();
          const auto& vdbg = dbgDefines.splitAtCommas();
          vdbg.foreach(
            [&]( const string& define ){
              if( define.empty() ){
                return;
              }
              if( *define == '#' ){
                return;
              }
              fs << "          \"" + define + "\",\n";
            }
          );
          fs << string( "        );\n" )
              + "        GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
              + "        GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
              + "        GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
              + "        GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
              + "        GCC_WARN_UNUSED_FUNCTION = YES;\n"
              + "        GCC_WARN_UNUSED_VARIABLE = YES;\n";
          if( !toPrefixHeader().empty() ){
            fs << "        GCC_PRECOMPILE_PREFIX_HEADER = YES;\n";
            fs << "        GCC_PREFIX_HEADER = \"../" + toPrefixHeader() + "\";\n";
          }
          fs << string( "        MACOSX_DEPLOYMENT_TARGET = " + toDeployment() + ";\n" )
              + "        MTL_ENABLE_DEBUG_INFO = INCLUDE_SOURCE;\n"
              + "        MTL_FAST_MATH = YES;\n"
              + "        ONLY_ACTIVE_ARCH = YES;\n"
              + "        SDKROOT = macosx;\n"
              + "        VERSIONING_SYSTEM = \"apple-generic\";\n"
              + "        VERSION_INFO_PREFIX = \"\";\n"
              + "      };\n"
              + "      name = Debug;\n"
              + "    };\n";
          fs << "    " + m_sReleaseBuildConfiguration + " /* Release */ = {\n"
              + "      isa = XCBuildConfiguration;\n"
              + "      buildSettings = {\n"
              + "        ALWAYS_SEARCH_USER_PATHS = NO;\n"
              + "        CLANG_ANALYZER_NONNULL = YES;\n"
              + "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
              + "        CLANG_CXX_LANGUAGE_STANDARD = \"" + toLanguage() + "\";\n"
              + "        CLANG_CXX_LIBRARY = \"libc++\";\n"
              + "        CLANG_ENABLE_MODULES = YES;\n"
              + "        CLANG_ENABLE_OBJC_ARC = YES;\n"
              + "        CLANG_ENABLE_OBJC_WEAK = YES;\n"
              + "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
              + "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
              + "        CLANG_WARN_COMMA = YES;\n"
              + "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
              + "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
              + "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
              + "        CLANG_WARN_DOCUMENTATION_COMMENTS = YES;\n"
              + "        CLANG_WARN_EMPTY_BODY = YES;\n"
              + "        CLANG_WARN_ENUM_CONVERSION = YES;\n"
              + "        CLANG_WARN_INFINITE_RECURSION = YES;\n"
              + "        CLANG_WARN_INT_CONVERSION = YES;\n"
              + "        CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
              + "        CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
              + "        CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
              + "        CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
              + "        CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
              + "        CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
              + "        CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
              + "        CLANG_WARN_UNGUARDED_AVAILABILITY = YES_AGGRESSIVE;\n"
              + "        CLANG_WARN_UNREACHABLE_CODE = YES;\n"
              + "        CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;\n"
              + "        COPY_PHASE_STRIP = NO;\n"
              + "        CURRENT_PROJECT_VERSION = 1;\n"
              + "        DEBUG_INFORMATION_FORMAT = \"dwarf-with-dsym\";\n"
              + "        ENABLE_NS_ASSERTIONS = NO;\n"
              + "        ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
              + "        GCC_C_LANGUAGE_STANDARD = gnu11;\n"
              + "        GCC_NO_COMMON_BLOCKS = YES;\n"
              + "        GCC_OPTIMIZATION_LEVEL = fast;\n"
              + "        GCC_PREPROCESSOR_DEFINITIONS = (\n"
              + "          \"$(inherited)\",\n";
          string relDefines = toDefinesRel();
          const auto& vrel = relDefines.splitAtCommas();
          vrel.foreach(
            [&]( const string& define ){
              if( define.empty() ){
                return;
              }
              if( *define == '#' ){
                return;
              }
              fs << "          \"" + define + "\",\n";
            }
          );
          fs << string( "        );\n" )
              + "        GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
              + "        GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
              + "        GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
              + "        GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
              + "        GCC_WARN_UNUSED_FUNCTION = YES;\n"
              + "        GCC_WARN_UNUSED_VARIABLE = YES;\n";
          if( !toPrefixHeader().empty() ){
            fs << "        GCC_PRECOMPILE_PREFIX_HEADER = YES;\n";
            fs << "        GCC_PREFIX_HEADER = \"../" + toPrefixHeader() + "\";\n";
          }
          fs << string( "        MACOSX_DEPLOYMENT_TARGET = " + toDeployment() + ";\n" )
              + "        MTL_ENABLE_DEBUG_INFO = NO;\n"
              + "        MTL_FAST_MATH = YES;\n"
              + "        SDKROOT = macosx;\n"
              + "        VERSIONING_SYSTEM = \"apple-generic\";\n"
              + "        VERSION_INFO_PREFIX = \"\";\n"
              + "      };\n"
              + "      name = Release;\n"
              + "    };\n";
          fs << "    " + m_sDebugNativeBuildConfig + " /* Debug */ = {\n"
              + "      isa = XCBuildConfiguration;\n"
              + "      buildSettings = {\n"
              + "        CODE_SIGN_STYLE = Automatic;\n";
          if( !toTeamName().empty() ){
            fs << "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n";
          }
          fs << "        LIBRARY_SEARCH_PATHS = (\n";
          auto libraryPaths = toLibraryPaths().splitAtCommas();
          libraryPaths.foreach(
            [&]( const string& f ){
              fs << "          ../" + f + ",\n";
            }
          );
          fs << "        );\n";
          fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
          auto frameworkPaths = toFrameworkPaths().splitAtCommas();
          frameworkPaths.foreach(
            [&]( const string& f ){
              fs << "          ../" + f + ",\n";
            }
          );
          fs << "        );\n";
          fs << "        SYSTEM_HEADER_SEARCH_PATHS = (\n";
          strings paths;
          if( !toIncPath().empty() ){
            paths.push( "../" + toIncPath() );
          }
          if( !toIncludePaths().empty() ){
            const auto& syspaths = toIncludePaths().splitAtCommas();
            syspaths.foreach(
              [&]( const string& syspath ){
                if( syspath.empty() ){
                  return;
                }
                if( *syspath == '/' ){
                  paths.push( syspath );
                }else if( *syspath == '.' ){
                  paths.push( syspath );
                }else{
                  paths.push( "../" + syspath );
                }
              }
            );
          }
          paths.foreach(
            [&]( const string& path ){
              fs << "          " + path + ",\n";
            }
          );
          fs << "        );\n";
          switch( toBuild().hash() ){
            case e_hashstr64_const( "framework" ):
              fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../Frameworks\",\n";
              fs << "          \"@loader_path/Frameworks\",\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              fs << "          -L/usr/local/lib,\n";
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;
            case e_hashstr64_const( "static" ):
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        EXECUTABLE_PREFIX = lib;\n";
              break;
            case e_hashstr64_const( "application" ):
              fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
              fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_LDFLAGS = (\n";
              fs << "          -L/usr/local/lib,\n";
              fs << "        );\n";
              break;
            case e_hashstr64_const( "console" ):
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_LDFLAGS = (\n";
              fs << "          -L/usr/local/lib,\n";
              fs << "        );\n";
              break;
          }
          fs << "        SKIP_INSTALL = YES;\n";
          fs << "      };\n";
          fs << "      name = Debug;\n";
          fs << "    };\n";
          fs << "    " + m_sReleaseNativeBuildConfig + " /* Release */ = {\n"
              + "      isa = XCBuildConfiguration;\n"
              + "      buildSettings = {\n"
              + "        CODE_SIGN_STYLE = Automatic;\n";
          if( !toTeamName().empty() ){
            fs << "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n";
          }
          fs << "        LIBRARY_SEARCH_PATHS = (\n";
          libraryPaths.foreach(
            [&]( const string& f ){
              fs << "          ../" + f + ",\n";
            }
          );
          fs << "        );\n";
          fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
          frameworkPaths.foreach(
            [&]( const string& f ){
              fs << "          ../" + f + ",\n";
            }
          );
          fs << "        );\n";
          fs << "        SYSTEM_HEADER_SEARCH_PATHS = (\n";
          paths.foreach(
            [&]( const string& path ){
              fs << "          " + path + ",\n";
            }
          );
          fs << "        );\n";
          switch( toBuild().hash() ){
            case e_hashstr64_const( "framework" ):
              fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../Frameworks\",\n";
              fs << "          \"@loader_path/Frameworks\",\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              fs << "          -L/usr/local/lib,\n";
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;
            case e_hashstr64_const( "static" ):
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        EXECUTABLE_PREFIX = lib;\n";
              break;
            case e_hashstr64_const( "application" ):
              fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_LDFLAGS = (\n";
              fs << "          -L/usr/local/lib,\n";
              fs << "        );\n";
              break;
            case e_hashstr64_const( "console" ):
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_LDFLAGS = (\n";
              fs << "          -L/usr/local/lib,\n";
              fs << "        );\n";
              break;
          }
          fs << "        SKIP_INSTALL = YES;\n";
          fs << "      };\n";
          fs << "      name = Release;\n";
          fs << "    };\n";
          fs << "    /* End XCBuildConfiguration section */\n";
        }
        void Workspace::Xcode::writeXCConfigurationListSection( Writer& fs )const{
          fs << "\n    /* Begin XCConfigurationList section */\n";
          fs << "    " + m_sBuildConfigurationList + " /* Build configuration list for PBXProject \"" + toLabel() + "\" */ = {\n"
              + "      isa = XCConfigurationList;\n"
              + "      buildConfigurations = (\n"
              + "        " + m_sReleaseBuildConfiguration + " /* Release */,\n"
              + "        " + m_sDebugBuildConfiguration + " /* Debug */,\n"
              + "      );\n"
              + "      defaultConfigurationIsVisible = 0;\n"
              + "      defaultConfigurationName = Release;\n"
              + "    };\n";
          fs << "    " + m_sBuildNativeTarget + " /* Build configuration list for PBXNativeTarget \"" + toLabel() + "\" */ = {\n"
              + "      isa = XCConfigurationList;\n"
              + "      buildConfigurations = (\n"
              + "        " + m_sReleaseNativeBuildConfig + " /* Release */,\n"
              + "        " + m_sDebugNativeBuildConfig + " /* Debug */,\n"
              + "      );\n"
              + "      defaultConfigurationIsVisible = 0;\n"
              + "      defaultConfigurationName = Release;\n"
              + "    };\n";
          fs << "    /* End XCConfigurationList section */\n";
        }

      #endif

    //}:                                          |
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