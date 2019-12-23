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
//Structs:{                                       |
  //Workspace:{                                   |

    struct Workspace final:Object{

      e_reflect_no_properties( Workspace, Object );

      //------------------------------------------|-----------------------------
      //Structs:{                                 |

        struct Project final:Object{

          e_reflect_no_properties( Project, Object );

          //--------------------------------------|-----------------------------
          //Structs:{                             |

            struct File final:string{
              File( const string& s )
                : string( s )
              {}
              File() = default;
            ~ File() = default;

            private:

              e_var_string( Identifier ) = string::resourceId();
            };

          //}:                                    |
          //Aliases:{                             |

            using Files = vector<File>;

          //}:                                    |
          //Classes:{                             |

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

          //}:                                    |
          //Methods:{                             |

            virtual void serialize( Writer& fs )const override;

            #if e_compiling( osx )
              void writePBXBuildFileSection(            Writer& fs )const;
              void writePBXCopyFilesBuildPhaseSection(  Writer& fs )const;
              void writePBXFileReferenceSection(        Writer& fs )const;
              void writePBXFrameworksBuildPhaseSection( Writer& fs )const;
              void writePBXGroupSection(                Writer& fs )const;
              void writePBXNativeTargetSection(         Writer& fs )const;
              void writePBXProjectSection(              Writer& fs )const;
              void writePBXResourcesBuildPhaseSection(  Writer& fs )const;
              void writePBXSourcesBuildPhaseSection(    Writer& fs )const;
              void writePBXVariantGroupSection(         Writer& fs )const;
              void writeXCBuildConfigurationSection(    Writer& fs )const;
              void writeXCConfigurationListSection(     Writer& fs )const;
            #endif

          //}:                                    |
          //--------------------------------------|-----------------------------

        private:

          e_var_string(         RootObject            ) = string::resourceId();
          e_var_string(         Framework             ) = string::resourceId();
          e_var_array(  Files,  Sources, Source::kMax );
          e_var_handle( Object, Generator             );
          e_var_string(         IncPath               );
          e_var_string(         SrcPath               );
          e_var_string(         ResPath               );
          e_var_string(         TypeID                );
          e_var_string(         Build                 );
          e_var_string(         Label                 );
        };

      //}:                                        |
      //Methods:{                                 |

        virtual void serialize( Writer& fs )const override;

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

        e_noinline void sortingHat( const string& in_path ){
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
        #if e_compiling( osx )
          const auto& dirName = path + "/" + workspace.toName() + ".xcworkspace";
          e_rm( dirName );
          e_md( dirName );
          { fs::Writer fs( dirName + "/contents.xcworkspacedata", fs::kTEXT );
            workspace.serialize( fs );
            fs.save();
          }
        #elif e_compiling( microsoft )
          const auto& dirName = path + "/" + workspace.toName() + ".sln";
          { fs::Writer fs( dirName, fs::kTEXT );
            workspace.serialize( fs );
            fs.save();
          }
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

      void Workspace::Project::serialize( Writer& fs )const{
        #if e_compiling( osx )
          fs << "// !$*UTF8*$!\n";
          fs << "{\n";
          fs << "  archiveVersion = 1;\n";
          fs << "  classes = {\n";
          fs << "  };\n";
          fs << "  objectVersion = 51;\n";
          fs << "  objects = {\n";
          writePBXBuildFileSection(            fs );
          writePBXCopyFilesBuildPhaseSection(  fs );
          writePBXFileReferenceSection(        fs );
          writePBXFrameworksBuildPhaseSection( fs );
          writePBXGroupSection(                fs );
          writePBXNativeTargetSection(         fs );
          writePBXProjectSection(              fs );
          writePBXResourcesBuildPhaseSection(  fs );
          writePBXSourcesBuildPhaseSection(    fs );
          writePBXVariantGroupSection(         fs );
          writeXCBuildConfigurationSection(    fs );
          writeXCConfigurationListSection(     fs );
          fs << "  };\n";
          fs << "  rootObject = " + m_sRootObject + "/* Project object */;\n";
          fs << "}\n";
        #elif e_compiling( microsoft )
        #endif
      }

    //}:                                          |
    //write*:{                                    |

      #if e_compiling( osx )

        void Workspace::Project::writePBXBuildFileSection( Writer& fs )const{
          fs << "\n    /* Begin PBXBuildFile section */\n";
            Files files;
            files.pushVector( m_aSources[ Source::kCpp ]);
            files.pushVector( m_aSources[ Source::kMm  ]);
            files.pushVector( m_aSources[ Source::kM   ]);
            files.pushVector( m_aSources[ Source::kC   ]);
            files.foreach(
              [&]( File& file ){
                fs << "    "
                  + file.toIdentifier()
                  + "/* "
                  + file.filename()
                  + " in Headers */ = {isa = PBXBuildFile; fileRef = "
                  + file.toIdentifier()
                  + " /* "
                  + file
                  + " */; settings = {ATTRIBUTES = (Public, ); }; };\n"
                ;
              }
            );
          fs << "    /* End PBXBuildFile section */\n";
        }

        void Workspace::Project::writePBXCopyFilesBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXCopyFilesBuildPhase section */\n";
          fs << "    /* End PBXCopyFilesBuildPhase section */\n";
        }

        namespace{
          void writeFileReference( Writer& fs, const Workspace::Project::Files& files, const string& projectType ){
            files.foreach(
              [&]( const Workspace::Project::File& file ){
                fs << "    "
                  + file.toIdentifier()
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

        void Workspace::Project::writePBXFileReferenceSection( Writer& fs )const{
          fs << "\n    /* Begin PBXFileReference section */\n";
          if( m_sBuild.hash() == e_hashstr64_const( "framework" )){
            writeFileReference( fs, m_aSources[ Source::kHpp ], "sourcecode.cpp.h"    );
            writeFileReference( fs, m_aSources[ Source::kInl ], "sourcecode.cpp.h"    );
            writeFileReference( fs, m_aSources[ Source::kH   ], "sourcecode.cpp.h"    );
            writeFileReference( fs, m_aSources[ Source::kCpp ], "sourcecode.cpp.cpp"  );
            writeFileReference( fs, m_aSources[ Source::kMm  ], "sourcecode.cpp.objc" );
            writeFileReference( fs, m_aSources[ Source::kM   ], "sourcecode.c.objc"   );
            writeFileReference( fs, m_aSources[ Source::kC   ], "sourcecode.c.c"      );
            fs << "    "
              + m_sFramework
              + " /* "
              + m_sLabel
              + ".framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = "
              + m_sLabel
              + ".framework; sourceTree = BUILT_PRODUCTS_DIR; };\n"
            ;
          }
          fs << "    /* End PBXFileReference section */\n";
        }

        void Workspace::Project::writePBXFrameworksBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXFrameworksBuildPhase section */\n";
          fs << "    /* End PBXFrameworksBuildPhase section */\n";
        }

        void Workspace::Project::writePBXGroupSection( Writer& fs )const{
          fs << "\n    /* Begin PBXGroup section */\n";
          fs << "    /* End PBXGroup section */\n";
        }

        void Workspace::Project::writePBXNativeTargetSection( Writer& fs )const{
          fs << "\n    /* Begin PBXNativeTarget section */\n";
          fs << "    /* End PBXNativeTarget section */\n";
        }

        void Workspace::Project::writePBXProjectSection( Writer& fs )const{
          fs << "\n    /* Begin PBXProject section */\n";
          fs << "    /* End PBXProject section */\n";
        }

        void Workspace::Project::writePBXResourcesBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXResourcesBuildPhase section */\n";
          fs << "    /* End PBXResourcesBuildPhase section */\n";
        }

        void Workspace::Project::writePBXSourcesBuildPhaseSection( Writer& fs )const{
          fs << "\n    /* Begin PBXSourcesBuildPhase section */\n";
          fs << "    /* End PBXSourcesBuildPhase section */\n";
        }

        void Workspace::Project::writePBXVariantGroupSection( Writer& fs )const{
          fs << "\n    /* Begin PBXVariantGroup section */\n";
          fs << "    /* End PBXVariantGroup section */\n";
        }

        void Workspace::Project::writeXCBuildConfigurationSection( Writer& fs )const{
          fs << "\n    /* Begin XCBuildConfiguration section */\n";
          fs << "    /* End XCBuildConfiguration section */\n";
        }

        void Workspace::Project::writeXCConfigurationListSection( Writer& fs )const{
          fs << "\n    /* Begin XCConfigurationList section */\n";
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
