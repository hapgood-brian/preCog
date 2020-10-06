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
//Globals:{                                       |

  strings EON::gfc::g_vIncludeStatements;

//}:                                              |
//Private:{                                       |
  //saveProject:{                                 |

    namespace{
      void anon_saveProject(
            const string& filename
          , const Workspace::Target& proj ){

        //----------------------------------------------------------------------
        // Save out the Xcode project.
        //----------------------------------------------------------------------

        if(( Workspace::bmp->bXcode11 || Workspace::bmp->bXcode12 ) && e_isa<Workspace::Xcode>( &proj )){
          #if e_compiling( microsoft )
            auto* ss =_strdup( filename.path() );
          #else
            auto* ss = strdup( filename.path() );
          #endif
          auto* ee = strchr( ss, 0 )-2;
          while( ee > ss ){
            if( *ee == '/' ){
              break;
            }
            --ee;
          }
          *ee = 0;
          const auto& xcodeProj = static_cast<const Workspace::Xcode&>( proj );
          const auto& dirPath = string( ss, ee )
            + "/" + xcodeProj.toLabel()
            + ".xcodeproj";
          free( cp( ss ));
          Writer fs( dirPath
            + "/project.pbxproj"
            , kTEXT );
          proj.serialize( fs );
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Ninja project for Unix, Linux and Android Studio.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bNinja && e_isa<Workspace::Ninja>( &proj )){
          const auto& ninja_target = static_cast<const Workspace::Ninja&>( proj );
          e_msgf( "Generating %s", ccp( filename ));
          Writer fs( filename, kTEXT );
          ninja_target.serialize( fs );
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Qmake project for Unix, Linux and Android Studio.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bQmake && e_isa<Workspace::Qmake>( &proj )){
          const auto& qmake_target = static_cast<const Workspace::Qmake&>( proj );
          e_msgf( "Generating %s", ccp( filename ));
          Writer fs( filename, kTEXT );
          qmake_target.serialize( fs );
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Visual Studio 2019 project.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bVS2019 && e_isa<Workspace::MSVC>( &proj )){
          const auto& dirPath = filename.path();
          const auto& vcxproj = static_cast<const Workspace::MSVC&>( proj );
          const auto& prjName = dirPath + vcxproj.toLabel() + ".vcxproj";
          Writer fs( prjName, kTEXT );
          proj.serialize( fs );
          fs.save();
        }
      }
    }

  //}:                                            |
//}:                                              |
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - (extends)  -
#endif

  e_specialized_extends( Workspace::Project<XCODE_PROJECT_SLOTS> );
  e_specialized_extends( Workspace::Project<NINJA_PROJECT_SLOTS> );
  e_specialized_extends( Workspace::Project<QMAKE_PROJECT_SLOTS> );
  e_specialized_extends( Workspace::Project<MSVC_PROJECT_SLOTS> );

  e_extends( Workspace );

//}:                                              |
//Methods:{                                       |
  //[workspace]:{                                 |
    //serializeSln2019:{                          |

#ifdef __APPLE__
  #pragma mark - (methods) -
#endif

      void Workspace::serializeSln2019( Writer& fs )const{
        if( m_tStates->bVS2019 && ( fs.toFilename().ext().tolower().hash() == ".sln"_64 )){

          //--------------------------------------------------------------------
          // Construct vcxproj's header.
          //--------------------------------------------------------------------

          fs << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
          fs << "# Visual Studio 16\n";
          fs << "VisualStudioVersion = 16.0.29709.97\n";
          fs << "MinimumVisualStudioVersion = 10.0.40219.1\n";

          //--------------------------------------------------------------------
          // Construct vcxproj's for libraries and applications.
          //--------------------------------------------------------------------

          // https://www.codeproject.com/Reference/720512/List-of-Visual-Studio-Project-Type-GUIDs
          const string& wsguid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"/* C++ GUID */;
          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<MSVC>() ){
              const auto& proj = it->as<MSVC>().cast();
              fs << "Project(\""
                + wsguid + "\") = \""
                + proj.toLabel()
                + "\", \""
                + proj.toLabel()
                + ".vcxproj\", \""
                + proj.toProjectGUID()
                + "\"\n";
              auto dependencies = proj.toLinkWith();
              if( !dependencies.empty() ){
                fs << "\tProjectSection(ProjectDependencies) = postProject\n";
                dependencies.replace( "\t", "" );
                dependencies.replace( "\n", "" );
                dependencies.replace( " ", "" );
                auto linkages = dependencies.splitAtCommas();
                auto i2 = m_vTargets.getIterator();
                while( i2 ){
                  if( i2->isa<MSVC>() ){
                    if( (*it)->UUID != (*i2)->UUID ){
                      auto i3 = linkages.getIterator();
                      while( i3 ){
                        const auto& A = (*i2).as<MSVC>()->toLabel().basename().tolower();
                        const auto& B = (*i3).basename().tolower();
                        if( A == B ){
                          const auto& guid = i2->as<MSVC>()->toProjectGUID();
                          fs << "\t\t" + guid + " = " + guid + "\n";
                          break;
                        }
                        ++i3;
                      }
                    }
                  }
                  ++i2;
                }
                fs << "\tEndProjectSection\n";
              }else{
                fs << "\tProjectSection(ProjectDependencies) = postProject\n";
                fs << "\tEndProjectSection\n";
              }
              fs << "EndProject\n";
              anon_saveProject( fs.toFilename(), proj );
            }
            ++it;
          }

          //--------------------------------------------------------------------
          // Write out the Global sections.
          //--------------------------------------------------------------------

          fs << "Global\n";
          fs << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
          fs << "\t\tDebug|x64 = Debug|x64\n";
          fs << "\t\tRelease|x64 = Release|x64\n";
          fs << "\tEndGlobalSection\n";
          fs << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
          it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<MSVC>() ){
              const auto& proj = it->as<MSVC>().cast();
              fs << "\t\t"
                + proj.toProjectGUID()
                + ".Debug|"+proj.toArchitecture()+".ActiveCfg = Debug|"+proj.toArchitecture()+"\n";
              fs << "\t\t"
                + proj.toProjectGUID()
                + ".Debug|"+proj.toArchitecture()+".Build.0 = Debug|"+proj.toArchitecture()+"\n";
              fs << "\t\t"
                + proj.toProjectGUID()
                + ".Release|"+proj.toArchitecture()+".ActiveCfg = Release|"+proj.toArchitecture()+"\n";
              fs << "\t\t"
                + proj.toProjectGUID()
                + ".Release|"+proj.toArchitecture()+".Build.0 = Release|"+proj.toArchitecture()+"\n";
            }
            ++it;
          }
          fs << "\tEndGlobalSection\n";
          fs << "\tGlobalSection(ExtensibilityGlobals) = postSolution\n";
          const auto& slnguid = string::guid();
          fs << "\t\tSolutionGuid = "
            + slnguid
            + "\n";
          fs << "\tEndGlobalSection\n";
          fs << "\tGlobalSection(ExtensibilityAddIns) = postSolution\n";
          fs << "\tEndGlobalSection\n";
          fs << "EndGlobal\n";

          //--------------------------------------------------------------------
          // We're done with this target so turn it off for the rest of the run.
          //--------------------------------------------------------------------

          const_cast<Workspace*>( this )->m_tStates->bVS2019 = 0;
          Workspace::bmp->bVS2019 = 0;
          return;
        }
      }

    //}:                                          |
    //serializeXcode:{                            |

      void Workspace::serializeXcode( Writer& fs )const{
        if(( m_tStates->bXcode11 || m_tStates->bXcode12 ) && ( fs.toFilename().ext().tolower().hash() == ".xcworkspacedata"_64 )){

          fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
          fs << "<Workspace\n";
          fs << "  version = \"1.0\">\n";

          //--------------------------------------------------------------------
          // Construct xcodeproj's for libraries.
          //--------------------------------------------------------------------

          fs << "  <Group\n";
          fs << "    location = \"container:\"\n";
          fs << "    name = \"Libraries\">\n";
          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Xcode>() ){
              const auto& proj = it->as<Xcode>().cast();
              switch( proj.toBuild().tolower().hash() ){
                case"framework"_64:
                  [[fallthrough]];
                case"shared"_64:
                  [[fallthrough]];
                case"static"_64:
                  fs << "  <FileRef\n";
                  fs << "    location = \"group:" + proj.toLabel() + ".xcodeproj\">\n";
                  fs << "  </FileRef>\n";
                  anon_saveProject( fs.toFilename(), proj );
                  break;
              }
            }
            ++it;
          }
          fs << "  </Group>\n";

          //--------------------------------------------------------------------
          // Construct xcodeproj's for applications.
          //--------------------------------------------------------------------

          fs << "  <Group\n";
          fs << "    location = \"container:\"\n";
          fs << "    name = \"Apps\">\n";
          it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Xcode>() ){
              const auto& proj = it->as<Xcode>().cast();
              switch( proj.toBuild().tolower().hash() ){
                case"application"_64:
                  [[fallthrough]];
                case"console"_64:
                  fs << "  <FileRef\n";
                  fs << "    location = \"group:"+proj.toLabel()+".xcodeproj\">\n";
                  fs << "  </FileRef>\n";
                  anon_saveProject( fs.toFilename(), proj );
                  break;
              }
            }
            ++it;
          }
          fs << "  </Group>\n";
          fs << "</Workspace>\n";

          //--------------------------------------------------------------------
          // We're done with this target so turn it off for the rest of the run.
          //--------------------------------------------------------------------

          const_cast<Workspace*>( this )->m_tStates->bXcode11 = 0;
          const_cast<Workspace*>( this )->m_tStates->bXcode12 = 0;
          Workspace::bmp->bXcode11 = 0;
          Workspace::bmp->bXcode12 = 0;
          return;
        }
      }

    //}:                                          |
    //serializeNinja:{                            |

      void Workspace::serializeNinja( Writer& fs )const{
        if( bmp->bNinja ){

          //--------------------------------------------------------------------
          // Setup main ninja file.
          //--------------------------------------------------------------------

          const string commentLine = "#---------------------------------------"
            "----------------------------------------\n";
          const string jokeLine = "#                   The best method for acc"
            "elerating a computer\n#                      is the one that boos"
            "ts it by 9.8 m/s2.\n";
          fs << commentLine
             << jokeLine
             << commentLine
             << "# GENERATED FILE DON'T EDIT IN ANY WAY SHAPE OR FORM SOMETHIN"
               "G BAD WILL HAPPEN!\n"
             << commentLine;
          fs << "ninja_required_version = 1.5\n\n";

          //--------------------------------------------------------------------
          // Handle Ninja targets.
          //--------------------------------------------------------------------

          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Ninja>() ){
              const auto& ninja_target = it->as<Ninja>().cast();
              switch( ninja_target.toBuild().tolower().hash() ){
                case"application"_64:
                  [[fallthrough]];
                case"shared"_64:
                  [[fallthrough]];
                case"static"_64:
                  [[fallthrough]];
                case"console"_64:
                  fs << "include " + ninja_target.toLabel() + ".rules\n";
                  anon_saveProject(
                    "tmp/"
                    + ninja_target.toLabel()
                    + ".rules"
                    , ninja_target );
                  break;
              }
            }
            ++it;
          }

          //--------------------------------------------------------------------
          // Add build statements.
          //--------------------------------------------------------------------

          if( !m_vTargets.empty() ){
            fs << "\n";
          }
          it = m_vTargets.getIterator();
          using PROJECT = Project<NINJA_PROJECT_SLOTS>;
          PROJECT::Files files;
          while( it ){
            const auto& hTarget = *it;
            if( hTarget.isa<Ninja>() ){

              //----------------------------------------------------------------
              // Add files.
              //----------------------------------------------------------------

              const auto& ninja_target = hTarget.as<Ninja>().cast();
              const auto& intermediate = ".intermediate/"
                + ninja_target
                . toLabel()
                . tolower();
              files.pushVector( ninja_target.inSources( Ninja::Type::kCpp ));
              files.pushVector( ninja_target.inSources( Ninja::Type::kC ));

              //----------------------------------------------------------------
              // Create the source file build step.
              //----------------------------------------------------------------

              fs << commentLine
                 << "# Project \""
                 << ninja_target.toLabel()
                 << "\"\n"
                 << commentLine
                 << "\n";
              const auto& bld = ninja_target.toBuild().tolower();
              const auto& tar = ninja_target.toLabel();
              files.foreach(
                [&]( const File& file ){

                  //------------------------------------------------------------
                  // Switch on the lowercase extension.
                  //------------------------------------------------------------

                  const auto& str = static_cast<const string&>( file );
                  const auto& ext = str.ext().tolower();
                  switch( ext.hash() ){

                    //----------------------------------------------------------
                    // C++ builds.
                    //----------------------------------------------------------

                    case".cpp"_64:
                      fs << "build "
                         << "../tmp/.intermediate/"
                         << tar
                         << "/"
                         << str.filename()
                         << ".o: CXX_"
                         << tar.toupper()
                         << " ../"
                         << str
                         << "\n";
                      fs << "  CXX_FLAGS = -std=gnu++17\n";
                      break;

                    //----------------------------------------------------------
                    // C builds.
                    //----------------------------------------------------------

                    case".c"_64:
                      fs << "build "
                         << "../tmp/.intermediate/"
                         << tar
                         << "/"
                         << str.filename()
                         << ".o: C_"
                         << tar.toupper()
                         << " ../"
                         << str
                         << "\n";
                      break;
                  }

                  //------------------------------------------------------------
                  // OBJECT files/directories.
                  //------------------------------------------------------------

                  if( !ninja_target.toIncludePaths().empty() ){
                    fs << "  OBJECT_DIR = ../tmp/.intermediate/"
                       << tar
                       << "\n";
                    fs << "  OBJECT_FILE_DIR = ../tmp/.intermediate/"
                       << tar
                       << "\n";
                    fs << "  TARGET_COMPILE_PDB = ../tmp/.intermediate/"
                       << tar
                       << "\n";
                    fs << "  TARGET_PDB = ../tmp/.intermediate/"
                       << tar
                       << ".pdb\n";
                    fs << "\n";
                  }
                }
              );

              //----------------------------------------------------------------
              // Handle the static/shared library build step.
              //----------------------------------------------------------------

              const auto& lwr = ninja_target.toLabel();
              const auto& upr = lwr.toupper();
              switch( bld.hash() ){
                case"shared"_64:/**/{
                  fs << commentLine
                     << "# "
                     << lwr
                     << " shared library\n"
                     << commentLine
                     << "\n"
                     << "build ../tmp/.output/lib"
                     << lwr
                  #if e_compiling( osx )
                     << ".dylib: SHARED_LIB_"
                  #elif e_compiling( microsoft )
                     << ".dll: SHARED_LIB_"
                  #else
                     << ".so: SHARED_LIB_"
                  #endif
                     << upr;
                  files.foreach(
                    [&]( const File& file ){
                      const auto& lbl = static_cast<const string&>( file );
                      const auto& ext = lbl.ext().tolower();
                      switch( ext.hash() ){
                        case".cpp"_64:
                          [[fallthrough]];
                        case".c"_64:
                          fs << " ../tmp/.intermediate/"
                             << ninja_target.toLabel()
                             << "/"
                             << lbl.filename()
                             << ".o";
                          break;
                      }
                    }
                  );
                  fs << "\n  OBJECT_DIR = ../tmp/.output"
                     << "\n  POST_BUILD = :"
                     << "\n  PRE_LINK = :"
                     << "\n  TARGET_FILE = ../tmp/.output/lib"
                     << lwr
                  #if e_compiling( osx )
                     << ".dylib"
                  #elif e_compiling( microsoft )
                     << ".dll"
                  #else
                     << ".so"
                  #endif
                     << "\n  TARGET_PDB = "
                     << lwr
                     << ".so.dbg\n"
                     << "default ../tmp/.output/lib"
                     << lwr
                  #if e_compiling( osx )
                     << ".dylib"
                  #elif e_compiling( microsoft )
                     << ".dll"
                  #else
                     << ".so"
                  #endif
                     << "\n\n";
                  break;
                }
                case"static"_64:
                  e_msgf( "  Creating %s", ccp( lwr.tolower() ));
                  fs << commentLine
                     << "# "
                     << lwr.mixedcase()
                     << " static library\n"
                     << commentLine
                     << "\n"
                     << "build ../tmp/.output/lib"
                     << lwr
                     << ".a: STATIC_LIB_"
                     << upr;
                  files.foreach(
                    [&]( const File& file ){
                      const auto& lbl = static_cast<const string&>( file );
                      const auto& ext = lbl.ext().tolower();
                      switch( ext.hash() ){
                        case".cpp"_64:
                          [[fallthrough]];
                        case".c"_64:
                          fs << " ../tmp/.intermediate/"
                             << ninja_target.toLabel()
                             << "/"
                             << lbl.filename()
                             << ".o";
                          break;
                      }
                    }
                  );
                  fs << "\n  OBJECT_DIR = ../tmp/.output"
                     << "\n  POST_BUILD = :"
                     << "\n  PRE_LINK = :"
                     << "\n  TARGET_FILE = ../tmp/.output/lib"
                     << lwr
                     << ".a"
                     << "\n  TARGET_PDB = "
                     << lwr
                     << ".a.dbg\n"
                     << "default ../tmp/.output/lib"
                     << lwr
                     << ".a\n\n";
                  break;
              }
            }
            files.clear();
            ++it;
          }

          //--------------------------------------------------------------------
          // We're done making sources, static and shared libaries so it's time
          // to produce the applications at the end.
          //--------------------------------------------------------------------

          it = m_vTargets.getIterator();
          while( it ){
            const auto& hTarget = *it;
            if( hTarget.isa<Ninja>() ){

              //----------------------------------------------------------------
              // Sort files.
              //----------------------------------------------------------------

              const auto& ninja_target = hTarget.as<Ninja>().cast();
              const auto& intermediate = ".intermediate/"
                + ninja_target
                . toLabel()
                . tolower();
              files.pushVector( ninja_target.inSources( Ninja::Type::kCpp ));
              files.pushVector( ninja_target.inSources( Ninja::Type::kC ));

              //----------------------------------------------------------------
              // Handle the console/application build step.
              //----------------------------------------------------------------

              const auto& lwr = ninja_target.toLabel();
              const auto& upr = lwr.toupper();
              const auto& bld = ninja_target.toBuild().tolower();
              const auto& tar = ninja_target.toLabel();
              switch( bld.hash() ){
                case"application"_64:
                  [[fallthrough]];
                case"console"_64:/**/{
                  fs << commentLine
                     << "# Applications\n"
                     << commentLine
                     << "\n"
                     << "build ../tmp/.output/"
                     << lwr;
                  if( bmp->bEmscripten ){
                     fs << ": WASM_LINKER_" << upr;
                  }else{
                    #if e_compiling( linux ) || e_compiling( osx )
                       fs << ": ELF_LINKER_" << upr;
                    #else
                       fs << ": PE_LINKER_" << upr;
                    #endif
                  }
                  const auto& libs = ninja_target.toLinkWith().splitAtCommas();
                  libs.foreach(
                    [&]( const string& lib ){
                      if( e_fexists( "/usr/lib/x86_64-linux-gnu/lib" + lib  + ".a" )){
                      }else if( e_fexists( "/usr/lib/lib"            + lib  + ".a" )){
                      }else if( e_fexists( "/usr/lib/"               + lib )){
                      }else if(( *lib != '/' )&&( *lib != '~' )&&( *lib != '.' )){
                        fs << " ../tmp/.output/" << lib;
                      }
                    }
                  );
                  const auto& intermediate = "../tmp/.intermediate/"
                    + ninja_target
                    . toLabel()
                    . tolower();
                  files.foreach(
                    [&]( const File& file ){
                      const auto& lbl = static_cast<const string&>( file );
                      const auto& ext = lbl.ext().tolower();
                      switch( ext.hash() ){
                        case".cpp"_64:
                          [[fallthrough]];
                        case".c"_64:
                          fs << " ../tmp/.intermediate/"
                             << ninja_target.toLabel()
                             << "/"
                             << lbl.filename()
                             << ".o";
                          break;
                      }
                    }
                  );
                  fs << "\n  LINK_LIBRARIES =";
                  libs.foreach(
                    [&]( const string& lib ){
                      if( e_fexists( "/usr/lib/x86_64-linux-gnu/lib" + lib  + ".a" )){
                            fs << " -L/usr/lib/x86_64-linux-gnu -l" << lib;
                      }else if( e_fexists( "/usr/lib/lib"            + lib  + ".a" )){
                                  fs << " -L/usr/lib/lib -l"        << lib;
                      }else if( e_fexists( "/usr/lib/"               + lib )){
                                  fs << " -l/usr/lib/"              << lib;
                      }else if(( *lib != '/' )&&( *lib != '~' )&&( *lib != '.' )){
                        fs << " ../tmp/.output/" << lib;
                      }else{
                        fs << " " << lib;
                      }
                    }
                  );
                  fs << "\n  TARGET_FILE = ../tmp/.output/"
                     << lwr.basename()
                     << "\n  OBJECT_DIR = ../tmp/.intermediate/"
                     << lwr.basename()
                     << "\n  TARGET_PDB = "
                     << lwr.basename()
                     << ".dbg"
                     << "\n  POST_BUILD = :"
                     << "\n  PRE_LINK = :"
                     << "\ndefault ../tmp/.output/"
                     << lwr.basename()
                     << "\n\n";
                  break;
                }
              }
            }
            files.clear();
            ++it;
          }

          //--------------------------------------------------------------------
          // We're done with this target so turn it off for the rest of the run.
          //--------------------------------------------------------------------

          const_cast<Workspace*>( this )->m_tStates->bNinja = 0;
          Workspace::bmp->bNinja = 0;
          return;
        }
      }

    //}:                                          |
    //serializeQmake:{                            |

      void Workspace::serializeQmake( Writer& fs )const{
        if( bmp->bQmake ){

          //--------------------------------------------------------------------
          // Setup main qmake file.
          //--------------------------------------------------------------------

          const string commentLine = "#---------------------------------------"
            "----------------------------------------\n";
          const string jokeLine = "#                   The best method for acc"
            "elerating a computer\n#                      is the one that boos"
            "ts it by 9.8 m/s2.\n";
          fs << commentLine
             << jokeLine
             << commentLine
             << "# GENERATED FILE DON'T EDIT IN ANY WAY SHAPE OR FORM SOMETHIN"
               "G BAD WILL HAPPEN!\n"
             << commentLine;

          //--------------------------------------------------------------------
          // Handle Qmake targets.
          //--------------------------------------------------------------------

          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Qmake>() ){
              const auto& qmake_target = it->as<Qmake>().cast();
              switch( qmake_target.toBuild().tolower().hash() ){
                case"application"_64:
                  [[fallthrough]];
                case"shared"_64:
                  [[fallthrough]];
                case"static"_64:
                  [[fallthrough]];
                case"console"_64:
                  fs << "include( " + qmake_target.toLabel() + ".pri )\n";
                  anon_saveProject(
                    "tmp/"
                    + qmake_target.toLabel()
                    + ".pri"
                    , qmake_target );
                  break;
              }
            }
            ++it;
          }

          //--------------------------------------------------------------------
          // Add build statements.
          //--------------------------------------------------------------------

          it = m_vTargets.getIterator();
          using PROJECT = Project<QMAKE_PROJECT_SLOTS>;
          PROJECT::Files files;
          while( it ){
            const auto& hTarget = *it;
            if( hTarget.isa<Qmake>() ){

              //----------------------------------------------------------------
              // Add files.
              //----------------------------------------------------------------

              const auto& qmake_target = hTarget.as<Qmake>().cast();
              const auto& intermediate = ".intermediate/"
                + qmake_target
                . toLabel()
                . tolower();
              files.pushVector( qmake_target.inSources( Qmake::Type::kCpp ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kCxx ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kHpp ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kHxx ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kInl ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kCC ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kHH ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kC ));
              files.pushVector( qmake_target.inSources( Qmake::Type::kH ));

              //----------------------------------------------------------------
              // Create the source file build step.
              //----------------------------------------------------------------

              fs << commentLine
                 << "# Project \""
                 << qmake_target.toLabel()
                 << "\"\n"
                 << commentLine
                 << "\n";
              const auto& bld = qmake_target.toBuild().tolower();
              const auto& tar = qmake_target.toLabel();
              auto sourceIndex = 0u;
              auto headerIndex = 0u;
              files.foreach(
                [&]( const File& file ){
                  const auto& str = static_cast<const string&>( file );
                  const auto& ext = str.ext().tolower();
                  switch( ext.hash() ){
                    case".hpp"_64:
                      [[fallthrough]];
                    case".hxx"_64:
                      [[fallthrough]];
                    case".hh"_64:
                      [[fallthrough]];
                    case".h"_64:
                      if( headerIndex ){
                        fs << "HEADERS << " << str.filename();
                      }else{
                        fs << "HEADERS = " << str.filename();
                      }
                      headerIndex++;
                      break;
                    case".cpp"_64:
                      [[fallthrough]];
                    case".cc"_64:
                      [[fallthrough]];
                    case".cxx"_64:
                      [[fallthrough]];
                    case".c"_64:
                      if( sourceIndex ){
                        fs << "SOURCES << " << str.filename();
                      }else{
                        fs << "SOURCES = " << str.filename();
                      }
                      sourceIndex++;
                      break;
                  }
                }
              );

              //----------------------------------------------------------------
              // Handle the static/shared library build step.
              //----------------------------------------------------------------

              const auto& lwr = qmake_target.toLabel();
              const auto& upr = lwr.toupper();
              switch( bld.hash() ){
                case"application"_64:
                  break;
                case"console"_64:
                  break;
                case"shared"_64:
                  break;
                case"static"_64:
                  break;
              }
            }
            files.clear();
            ++it;
          }

          //--------------------------------------------------------------------
          // We're done with this target so turn it off for the rest of the run.
          //--------------------------------------------------------------------

          const_cast<Workspace*>( this )->m_tStates->bQmake = 0;
          Workspace::bmp->bQmake = 0;
          return;
        }
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Generate Visual Studio 2019 on Windows 10.
        //----------------------------------------------------------------------

        serializeSln2019( fs );

        //----------------------------------------------------------------------
        // Generate workspace on macOS.
        //----------------------------------------------------------------------

        serializeXcode( fs );

        //----------------------------------------------------------------------
        // Generate build.ninja on Linux.
        //----------------------------------------------------------------------

        serializeNinja( fs );

        //----------------------------------------------------------------------
        // Generate pro file.
        //----------------------------------------------------------------------

        serializeQmake( fs );
      }

    //}:                                          |
    //isUnityBuild:{                              |

      bool Workspace::isUnityBuild(){
        auto it = IEngine::args.getIterator();
        while( it ){
          if( it->tolower().hash() == e_hashstr64_const( "--unity" )){
            return true;
          }
          ++it;
        }
        return false;
      }

    //}:                                          |
    //ignoreFile:{                                |

      bool Workspace::isIgnoreFile( const string& regex, const string& s ){
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

    //}:                                          |
    //cleanup:{                                   |

      void Workspace::cleanup()const{
        auto it = const_cast<Workspace*>( this )->m_vTargets.getIterator();
        while( it ){
          if( it->isa<Ninja>() ){
            it.erase();
            continue;
          }
          if( it->isa<Xcode>() ){
            if( !m_tStates->bXcode12 ){
              it.erase();
              continue;
            }
            if( !m_tStates->bXcode11 ){
              it.erase();
              continue;
            }
          }else if( it->isa<MSVC>() ){
            if( !m_tStates->bVS2019 ){
              it.erase();
              continue;
            }
          }
          ++it;
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
