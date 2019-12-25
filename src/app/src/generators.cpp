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
              kMax
            };

          //}:                                    |
          //--------------------------------------|-----------------------------

        private:

          e_var_array(  Files,  Sources, Source::kMax );
          e_var_handle( Object, Generator             );
          e_var_string(         TeamName              );
          e_var_string(         Language              ) = "c++17";
          e_var_string(         IncPath               );
          e_var_string(         SrcPath               );
          e_var_string(         EnvPath               );
          e_var_string(         ResPath               );
          e_var_string(         OrgName               );
          e_var_string(         TypeID                );
          e_var_string(         Build                 );
          e_var_string(         Label                 );
        };

        struct Xcode final:Project{

          e_reflect_no_properties( Xcode, Project );

          //--------------------------------------|-----------------------------
          //Methods:{                             |

            virtual void serialize( Writer& )const override;

          //}:                                    |
          //--------------------------------------|-----------------------------

        private:

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

          e_var_string( FrameworkGroup         ) = string::resourceId();
          e_var_string( ResourcesGroup         ) = string::resourceId();
          e_var_string( ProductsGroup          ) = string::resourceId();
          e_var_string( IncludeGroup           ) = string::resourceId();
          e_var_string( ResGroup               ) = string::resourceId();
          e_var_string( SrcGroup               ) = string::resourceId();
          e_var_string( MainGroup              ) = string::resourceId();

          e_var_string( BuildConfigurationList ) = string::resourceId();
          e_var_string( BuildNativeTarget      ) = string::resourceId();
          e_var_string( FrameworkNativeTarget  ) = string::resourceId();
          e_var_string( ResourcesBuildPhase    ) = string::resourceId();
          e_var_string( FrameworkBuildPhase    ) = string::resourceId();
          e_var_string( HeadersBuildPhase      ) = string::resourceId();
          e_var_string( SourcesBuildPhase      ) = string::resourceId();
          e_var_string( FrameworkFileRef       ) = string::resourceId();
          e_var_string( ProjectObject          ) = string::resourceId();

          e_var_string( ReleaseBuildConfiguration ) = string::resourceId();
          e_var_string( ReleaseNativeBuildConfig  ) = string::resourceId();
          e_var_string( DebugBuildConfiguration   ) = string::resourceId();
          e_var_string( DebugNativeBuildConfig    ) = string::resourceId();

          e_var_string( ProductBundleId        );
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
          const auto& ext = path.tolower().ext();
          switch( ext.hash() ){
            #if e_compiling( osx )
              case e_hashstr64_const( ".framework" ):
                m_pProject->inSources( Source::kFramework ).push( path );
                break;
              case e_hashstr64_const( ".xcasset" ):
                m_pProject->inSources( Source::kXcasset ).push( path );
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
            #elif e_compiling( microsoft )
              case e_hashstr64_const( ".lib" ):
                m_pProject->inSources( Source::kStaticlib ).push( path );
                break;
            #endif
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
            case e_hashstr64_const( ".mm" ):
              m_pProject->inSources( Source::kMm ).push( path );
              break;
            case e_hashstr64_const( ".h" ):
              m_pProject->inSources( Source::kH ).push( path );
              break;
            case e_hashstr64_const( ".c" ):
              m_pProject->inSources( Source::kC ).push( path );
              break;
            case e_hashstr64_const( ".m" ):
              m_pProject->inSources( Source::kM ).push( path );
              break;
            default:
              e_warnsf( "Ignoring %s!", ccp( path ));
              break;
          }
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
              IEngine::dir( paths[ i ],
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

  namespace{
    void anon_writeFileReference( Writer& fs, const Workspace::Project::Files& files, const string& projectType ){
      files.foreach(
        [&]( const Workspace::Project::File& file ){
          fs << "    "
            + file.toRefID()
            + " = {isa = PBXFileReference; lastKnownFileType = "
            + projectType
            + "; path = ../"
            + file
            + "; sourceTree = \"<group>\"; };\n"
          ;
        }
      );
    }
  }

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
              case e_hashstr64_const( "m_incPaths" ):
                p.setIncPath( lua_tostring( L, -1 ));
                break;
              case e_hashstr64_const( "m_bundleId" ):
                p.setProductBundleId( lua_tostring( L, -1 ));
                break;
              case e_hashstr64_const( "m_language" ):
                p.setLanguage( lua_tostring( L, -1 ));
                break;
              case e_hashstr64_const( "m_teamName" ):
                p.setTeamName( lua_tostring( L, -1 ));
                break;
              case e_hashstr64_const( "m_orgName" ):
                p.setOrgName( lua_tostring( L, -1 ));
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
          verifySolution( dirName );
        #endif
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
                    switch( proj.toTypeID().hash() ){

                      //--------------------------------------------------------
                      // Write out a PBX format Xcode 11 project.
                      //--------------------------------------------------------

                      case e_hashstr64_const( "pbx" ):
                        { auto* ss = strdup( fs.toFilename().path().c_str() );
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
                switch( proj.toLabel().tolower().hash() ){
                  case e_hashstr64_const( "app" ):
                    fs << "  <FileRef\n";
                    fs << "    location = \"group:"+proj.toLabel()+".xcodeproj\">\n";
                    fs << "  </FileRef>\n";
                    switch( proj.toTypeID().hash() ){

                      //--------------------------------------------------------
                      // Write out a PBX format Xcode 11 project.
                      //--------------------------------------------------------

                      case e_hashstr64_const( "pbx" ):
                        { const auto& dirName = fs.toFilename().path() + "/" + proj.toLabel() + ".xcodeproj";
                          e_rm( dirName );
                          e_md( dirName );
                          fs::Writer fs( dirName + "/project.pbxproj", fs::kTEXT );
                          proj.serialize( fs );
                          fs.save();
                        }
                        break;
                    }
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
            Files files;
            files.pushVector( inSources( Source::kHpp ));
            files.pushVector( inSources( Source::kInl ));
            files.pushVector( inSources( Source::kH   ));
            files.foreach(
              [&]( File& file ){
                fs << "    "
                  + file.toBuildID()
                  + " /* "
                  + file.filename()
                  + " in Headers */ = {isa = PBXBuildFile; fileRef = "
                  + file.toRefID()
                  + " /* "
                  + file.filename()
                  + " */;";
                if( file.isPublic() ){
                  fs << " settings = {ATTRIBUTES = (Public, ); };";
                }
                fs << " };\n";
              }
            );
            files.clear();
            files.pushVector( inSources( Source::kCpp ));
            files.pushVector( inSources( Source::kMm  ));
            files.pushVector( inSources( Source::kM   ));
            files.pushVector( inSources( Source::kC   ));
            files.foreach(
              [&]( File& file ){
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
          fs << "    /* End PBXCopyFilesBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXFileReferenceSection( Writer& fs )const{
          fs << "\n    /* Begin PBXFileReference section */\n";
          if( toBuild().hash() == e_hashstr64_const( "framework" )){
            anon_writeFileReference( fs, inSources( Source::kHpp ), "sourcecode.cpp.h"    );
            anon_writeFileReference( fs, inSources( Source::kInl ), "sourcecode.cpp.h"    );
            anon_writeFileReference( fs, inSources( Source::kH   ), "sourcecode.c.h"      );
            anon_writeFileReference( fs, inSources( Source::kCpp ), "sourcecode.cpp.cpp"  );
            anon_writeFileReference( fs, inSources( Source::kMm  ), "sourcecode.cpp.objc" );
            anon_writeFileReference( fs, inSources( Source::kM   ), "sourcecode.c.objc"   );
            anon_writeFileReference( fs, inSources( Source::kC   ), "sourcecode.c.c"      );
            fs << "    "
              + m_sFrameworkFileRef
              + " /* "
              + toLabel()
              + ".framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = "
              + toLabel()
              + ".framework; sourceTree = BUILT_PRODUCTS_DIR; };\n"
            ;
          }
          fs << "    /* End PBXFileReference section */\n";
        }
        void Workspace::Xcode::writePBXFrameworksBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXFrameworksBuildPhase section */\n";
          if( toBuild().hash() == e_hashstr64_const( "framework" )){
            fs << "    " + m_sFrameworkBuildPhase + " /* frameworks */ = {\n"
              + "      isa = PBXFrameworksBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n"
              + "      );\n"
              + "      runOnlyForDeploymentPostprocessing = 0;\n"
              + "    };\n"
            ;
          }
          fs << "    /* End PBXFrameworksBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXGroupSection( Writer& fs )const{
          fs << "\n    /* Begin PBXGroup section */\n";
          fs << "    " + m_sMainGroup + " = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n"
              + "        " + m_sResourcesGroup + " /* Resources */,\n"
              + "        " + m_sProductsGroup  + " /* Products */,\n"
              + "      );\n"
              + "      sourceTree = \"<group>\";\n"
              + "    };\n";
          fs << "    " + m_sResourcesGroup + " /* Resources */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n"
              + "        " + m_sIncludeGroup + " /* include */,\n"
              + "        " + m_sSrcGroup + " /* src */,\n"
              + "      );\n"
              + "      name = Resources;\n"
              + "      sourceTree = \"<group>\";\n"
              + "    };\n";
          fs << "    " + m_sProductsGroup + " /* Products */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n"
              + "        " + m_sFrameworkFileRef + " /* " + toLabel() + ".framework */,\n"
              + "      );\n"
              + "      name = Products;\n"
              + "      sourceTree = \"<group>\";\n"
              + "    };\n";
          fs << "    " + m_sIncludeGroup + " /* include */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n";
          Files files;
          files.pushVector( inSources( Source::kHpp ));
          files.pushVector( inSources( Source::kInl ));
          files.pushVector( inSources( Source::kH   ));
          files.foreach(
            [&]( const File& file ){
              fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
            }
          );
          fs << "      );\n";
          fs << "      path = ../" + toIncPath() + ";\n";
          fs << "      sourceTree = \"<group>\";\n";
          fs << "    };\n";
          fs << "    " + m_sSrcGroup + " /* src */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n";
          files.clear();
          files.pushVector( inSources( Source::kCpp ));
          files.pushVector( inSources( Source::kMm  ));
          files.pushVector( inSources( Source::kC   ));
          files.pushVector( inSources( Source::kM   ));
          files.foreach(
            [&]( const File& file ){
              fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
            }
          );
          fs << "      );\n";
          fs << "      path = ../" + toSrcPath() + ";\n";
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
              + "        " + m_sResourcesBuildPhase + " /* res */,\n"
              + "        " + m_sHeadersBuildPhase   + " /* include */,\n"
              + "        " + m_sSourcesBuildPhase   + " /* src */,\n"
              + "      );\n"
              + "      buildRules = (\n"
              + "      );\n"
              + "      dependencies = (\n"
              + "      );\n"
              + "      name = " + toLabel() + ";\n"
              + "      productName = " + toLabel() + ";\n"
              + "      productReference = " + m_sFrameworkFileRef + " /* " + toLabel() + ".framework */;\n"
              + "      productType = \"com.apple.product-type.framework\";\n"
              + "    };\n";
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
              + "      isa = PBXResourcesBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          Files files;
          files.pushVector( inSources( Source::kHpp ));
          files.pushVector( inSources( Source::kH   ));
          files.foreach(
            [&]( const File& f ){
              fs << "        " + f.toBuildID() + " /* " + f.filename() + " in Headers */,\n";
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
              + "      files = (\n"
              + "      );\n"
              + "      runOnlyForDeploymentPostprocessing = 0;\n"
              + "    };\n";
          fs << "    /* End PBXResourcesBuildPhase section */\n";
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
              fs << "        " + f.toBuildID() + " /* " + f + " in Sources */,\n";
            }
          );
          fs << "      );\n";
          fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
          fs << "    };\n";
          fs << "    /* End PBXSourcesBuildPhase section */\n";
        }
        void Workspace::Xcode::writePBXVariantGroupSection( Writer& fs )const{
          fs << "\n    /* Begin PBXVariantGroup section */\n";
          fs << "    /* End PBXVariantGroup section */\n";
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
              + "          \"DEBUG=1\",\n"
              + "          \"$(inherited)\",\n"
              + "        );\n"
              + "        GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
              + "        GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
              + "        GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
              + "        GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
              + "        GCC_WARN_UNUSED_FUNCTION = YES;\n"
              + "        GCC_WARN_UNUSED_VARIABLE = YES;\n"
              + "        MACOSX_DEPLOYMENT_TARGET = 10.15;\n"
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
              + "        GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
              + "        GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
              + "        GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
              + "        GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
              + "        GCC_WARN_UNUSED_FUNCTION = YES;\n"
              + "        GCC_WARN_UNUSED_VARIABLE = YES;\n"
              + "        MACOSX_DEPLOYMENT_TARGET = 10.15;\n"
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
              + "        CODE_SIGN_STYLE = Automatic;\n"
              + "        COMBINE_HIDPI_IMAGES = YES;\n"
              + "        DEFINES_MODULE = YES;\n"
              + "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n"
              + "        DYLIB_COMPATIBILITY_VERSION = 1;\n"
              + "        DYLIB_CURRENT_VERSION = 1;\n"
              + "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n"
              + "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toEnvPath() + "/Info.plist\";\n"
              + "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n"
              + "        LD_RUNPATH_SEARCH_PATHS = (\n"
              + "          \"$(inherited)\",\n"
              + "          \"@executable_path/../Frameworks\",\n"
              + "          \"@loader_path/Frameworks\",\n"
              + "        );\n"
              + "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n"
              + "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n"
              + "        SKIP_INSTALL = YES;\n"
              + "      };\n"
              + "      name = Debug;\n"
              + "    };\n";
          fs << "    " + m_sReleaseNativeBuildConfig + " /* Release */ = {\n"
              + "      isa = XCBuildConfiguration;\n"
              + "      buildSettings = {\n"
              + "        CODE_SIGN_STYLE = Automatic;\n"
              + "        COMBINE_HIDPI_IMAGES = YES;\n"
              + "        DEFINES_MODULE = YES;\n"
              + "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n"
              + "        DYLIB_COMPATIBILITY_VERSION = 1;\n"
              + "        DYLIB_CURRENT_VERSION = 1;\n"
              + "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n"
              + "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toEnvPath() + "/Info.plist\";\n"
              + "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n"
              + "        LD_RUNPATH_SEARCH_PATHS = (\n"
              + "          \"$(inherited)\",\n"
              + "          \"@executable_path/../Frameworks\",\n"
              + "          \"@loader_path/Frameworks\",\n"
              + "        );\n"
              + "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n"
              + "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n"
              + "        SKIP_INSTALL = YES;\n"
              + "      };\n"
              + "      name = Release;\n"
              + "    };\n";
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
