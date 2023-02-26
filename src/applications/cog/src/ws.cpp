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

#include"generators.h"
#include"luacore.h"
#include"std.h"
#include"ws.h"
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Externs:{                                       |

  void verifyPBX( const string& path );

//}:                                              |
//Globals:{                                       |

  strings EON::gfc::g_vIncludeStatements;

//}:                                              |
//Private:{                                       |
  //saveProject:{                                 |

    namespace{
      void anon_saveProject(
            const string& filename
          , const Workspace::Target& project ){

        //----------------------------------------------------------------------
        // Save out the Xcode project.
        //----------------------------------------------------------------------

        if(( Workspace::bmp->bXcode11 ||
             Workspace::bmp->bXcode12 ||
             Workspace::bmp->bXcode14 ) &&
             e_isa<Workspace::Xcode>( &project )){

          //--------------------------------------------------------------------
          // Write the PBX format project inside xcodeproj package.
          //--------------------------------------------------------------------

          auto* ss = strdup( filename.path() );
          auto* ee = strchr( ss, 0 )-2;
          while( ee > ss ){
            if( *ee == '/' ){
              break;
            }
            --ee;
          }
          *ee = 0;
          const auto& xcodeProj = static_cast<const
            Workspace::Xcode&>( project );
          const auto& dirPath = string( ss, ee )
            + "/" + xcodeProj.toLabel()
            + ".xcodeproj";
          free( cp( ss ));
          Writer fs( dirPath
            + "/project.pbxproj"
            , kTEXT );
          project.serialize( fs );
          fs.save();

          //----------------------------------------------------------------------
          // Write the entitlements file.
          //----------------------------------------------------------------------

          if((( xcodeProj.toBuild() == "application"_64 )&&
              // Entitlement generation affected here.
              xcodeProj.isDisableLibValidation() )){
            xcodeProj.saveEntitlements( dirPath );
          }
        }

        //----------------------------------------------------------------------
        // Save out the Ninja project for Unix, Linux and Android Studio. This
        // isn't used for NDK and gradle stuff; it writes out too many files.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bNinja &&
              e_isa<Workspace::Ninja>( &project )){
          const auto& ninja = static_cast<
            const Workspace::Ninja&>( project );
          Writer fs( filename, kTEXT );
          ninja.serialize( fs );
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Qmake project for Unix, Linux and Android Studio.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bQmake &&
              e_isa<Workspace::Qmake>( &project )){
          const auto& qmake =
              static_cast<const Workspace::Qmake&>( project );
          e_msgf( "Generating %s"
            , ccp( filename ));
          Writer fs( filename, kTEXT );
          qmake.serialize( fs );
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Visual Studio 2019 project.
        //----------------------------------------------------------------------

        if(( Workspace::bmp->bVS2019 ||
             Workspace::bmp->bVS2022 ) &&
               e_isa<Workspace::MSVC>( &project )){
          const auto& dirPath = filename.path();
          const auto& vcxproj = static_cast<const Workspace::MSVC&>( project );
          const auto& prjName = dirPath
            + vcxproj.toLabel()
            + ".vcxproj";
          Writer fs( prjName, kTEXT );
          project.serialize( fs );
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
  e_specialized_extends( Workspace::Project< MSVC_PROJECT_SLOTS> );

  e_extends( Workspace );

//}:                                              |
//Methods:{                                       |
  //[workspace]:{                                 |
    //serializeSln2022:{                          |

#ifdef __APPLE__
  #pragma mark - (methods) -
#endif

      void Workspace::serializeSln2022( Writer& fs )const{
        if( m_tStates->bVS2022 && ( fs.toFilename().ext().tolower().hash() == ".sln"_64 )){

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
              auto projDependencies = proj.toDependencies();
              if( !dependencies.empty() && !projDependencies ){
                dependencies << ",";
              }
              dependencies << projDependencies;
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

          const_cast<Workspace*>( this )
            ->m_tStates->bVS2022 = 0;
          bmp->bVS2022 = 0;
        }
      }

    //}:                                          |
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
              auto projDependencies = proj.toDependencies();
              if( !dependencies.empty() && !projDependencies ){
                dependencies << ",";
              }
              dependencies << projDependencies;
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
          bmp->bVS2019 = 0;
        }
      }

    //}:                                          |
    //serializeXcode:{                            |

      void Workspace::serializeXcode( Writer& fs )const{
        if(( m_tStates->bXcode11 ||
             m_tStates->bXcode12 ||
             m_tStates->bXcode14 )&&( fs
           . toFilename()
           . ext()
           . tolower()
           . hash() == ".xcworkspacedata"_64 )){
          fs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
          fs << "<Workspace\n";
          fs << "  version = \"1.0\">\n";

          //--------------------------------------------------------------------
          // Sort targets.
          //--------------------------------------------------------------------

          const auto& onSort = [](
                const auto& a
              , const auto& b )->bool{
            if( !a.template isa<Project<XCODE_PROJECT_SLOTS>>() )
              return false;
            if( !b.template isa<Project<XCODE_PROJECT_SLOTS>>() )
              return false;
            return(
                a.template as<Project<XCODE_PROJECT_SLOTS>>()->toLabel()
              < b.template as<Project<XCODE_PROJECT_SLOTS>>()->toLabel()
            );
          };
          auto& me = *const_cast<Workspace*>( this );
          me.m_vTargets.sort( onSort );

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
                default:
                  break;
              }
            }
            ++it;
          }
          fs << "  </Group>\n";

          //--------------------------------------------------------------------
          // Construct xcodeproj's for bundles.
          //--------------------------------------------------------------------

          fs << "  <Group\n";
          fs << "    location = \"container:\"\n";
          fs << "    name = \"Bundles\">\n";
          it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Xcode>() ){
              const auto& proj = it->as<Xcode>().cast();
              switch( proj.toBuild().tolower().hash() ){
                case"bundle"_64:
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
          bmp->bXcode11 = 0;
          bmp->bXcode12 = 0;
          bmp->bXcode14 = 0;
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
             << "# GENERATED FILE DO NOT EDIT IN ANY WAY SHAPE OR FORM SOMETHIN"
               "G BAD WILL HAPPEN\n"
             << commentLine
             << "\nninja_required_version = 1.5\n\n";

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

                    //----------------------------------------------------------
                    // Exclude everything else.
                    //----------------------------------------------------------

                    default:
                      e_msgf( "Skipping extension: %s", ccp( ext ));
                      return;
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
              files.pushVector( ninja_target.inSources( Ninja::Type::kCpp ));
              files.pushVector( ninja_target.inSources( Ninja::Type::kC ));

              //----------------------------------------------------------------
              // Handle the console/application build step.
              //----------------------------------------------------------------

              const auto& lwr = ninja_target.toLabel();
              const auto& upr = lwr.toupper();
              const auto& bld = ninja_target.toBuild().tolower();
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
                      if(( e_fexists( "/usr/lib/x86_64-linux-gnu/lib" + lib + ".a" ))||( e_fexists( "/usr/lib/x86_64-linux-gnu/lib" + lib + ".dylib" ))){
                            fs << " -L/usr/lib/x86_64-linux-gnu -l"  << lib;
                      }else if(( e_fexists( "/usr/lib/lib"            + lib + ".a" ))||( e_fexists( "/usr/lib/lib" + lib + ".dylib" ))){
                                  fs << " -L/usr/lib/lib -l"         << lib;
                      }else if( e_fexists( "/usr/lib/"                + lib )){
                                  fs << " -l/usr/lib/"               << lib;
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
          bmp->bNinja = 0;
        }
      }

    //}:                                          |
    //serializeQmake:{                            |

      void Workspace::serializeQmake( Writer& fs )const{
        if( bmp->bQmake ){

          //--------------------------------------------------------------------
          // Setup main .PRO file and create .PRI files.
          //--------------------------------------------------------------------

          const string commentLine = "#---------------------------------------"
            "----------------------------------------\n";
          const string jokeLine = "#                   The best method for acc"
            "elerating a computer\n#                      is the one that boos"
            "ts it by 9.8 m/s2.\n";
          fs << commentLine
             << jokeLine
             << commentLine
             << "# GENERATED FILE DO NOT EDIT IN ANY WAY SHAPE OR FORM SOMETHIN"
               "G BAD WILL HAPPEN\n"
             << commentLine;

          //------------------------------------------------------------------
          // Save PRI files.
          //------------------------------------------------------------------

          static constexpr u64 kOrder[4]{
            "static"_64, "shared"_64, "console"_64, "application"_64
          };
          fs << "\nTEMPLATE = subdirs\n";
          fs << "CONFIG += ordered\n";
          for( u32 i=0; i<4; ++i ){
            auto it = m_vTargets.getIterator();
            if( it ){
              while( it ){
                if( it->isa<Qmake>() ){
                  const auto& qmake_target = it->as<Qmake>().cast();
                  if( qmake_target.toBuild().tolower().hash() == kOrder[ i ]){
                    const auto& targetName = qmake_target.toLabel().tolower();
                    fs << "SUBDIRS +=";
                    fs <<  " "  << targetName << "\n";
                    e_md( "tmp/" + targetName );
                    anon_saveProject( "tmp/"
                      + targetName
                      + "/"
                      + targetName
                      + ".pro"
                      , qmake_target
                    );
                  }
                }
                ++it;
              }
            }
          }

          //--------------------------------------------------------------------
          // Save PRO files.
          //--------------------------------------------------------------------

          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Qmake>() ){
              const auto& qmake_target = it->as<Qmake>().cast();
              if( !qmake_target.toLinkWith().empty() ){
                const auto& qmake_name = qmake_target.toLabel().tolower();
                auto qmake_link = qmake_target.toLinkWith();
                qmake_link.erase( "\n" );
                qmake_link.erase( " " );
                const auto& links_with = qmake_link.splitAtCommas();
                strings dependencies;
                if( !links_with.empty() ){
                  auto i2 = links_with.getIterator();
                  while( i2 ){
                    string name = i2->tolower();
                    #if e_compiling( osx ) || e_compiling( linux )
                      if( name.left( 3 ).hash() == "lib"_64 ){
                        auto dep = name.filename();
                        dep.ltrim( 3 );
                        const auto& ext = dep.ext().tolower();
                        if( ext.hash() == ".a"_64 ){
                          dep.trim( 2 );
                        #if e_compiling( linux )
                          }else if( ext.hash() == ".so"_64 ){
                            dep.trim( 3 );
                        #elif e_compiling( osx )
                          }else if( ext.hash() == ".dylib"_64 ){
                            dep.trim( 6 );
                        #endif
                        }
                        dependencies.push( dep );
                      }
                    #elif e_compiling( microsoft )
                      switch( name.right( 4 ).hash() ){
                        case".lib"_64:
                          [[fallthrough]];
                        case".dll"_64:
                          dependencies.push( name.basename() );
                          break;
                      }
                    #endif
                    ++i2;
                  }
                }
                if( !dependencies.empty() ){
                  fs << qmake_name
                     << ".deps =";
                  auto i2 = dependencies.getIterator();
                  while( i2 ){
                    fs << " "
                       << *i2;
                    ++i2;
                  }
                  fs << "\n";
                }
              }
            }
            ++it;
          }
          fs << "\n"
             << commentLine
             << "# vim:ft=qmake\n"
             << commentLine;
          bmp->bQmake = 0;
          return;
        }
      }

    //}:                                          |
    //serializeGradle:{                           |

      void Workspace::serializeGradle( Writer& fs )const{

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

        //https://developer.android.com/studio/build
        if( !bmp->bGradle )
          return;

        //----------------------------------------------------------------------
        // Write the tmp/settings.gradle and tmp/build.gradle project files.
        //----------------------------------------------------------------------

        fs << "pluginManagement{\n"
           << "  repositories{\n"
           << "    gradlePluginPortal()\n"
           << "    mavenCentral()\n"
           << "    google()\n"
           << "  }\n"
           << "}\n";
        const auto& ndk_root
          = fs
          . toFilename()
          . path();
        fs << "rootProject.name = '"
           << toName()
           << "'\n";
        Writer rootPrj( ndk_root
          + "/build.gradle"
          , kTEXT );
        rootPrj << "plugins{\n";
        auto targets( m_vTargets );
        targets.sort(
          []( const auto& a, const auto& b ){
            return(
                a.template as<NDK>()->toLabel()
              < b.template as<NDK>()->toLabel()
            );
          }
        );
        // Only pull in the stuff we need; scan targets to figure that out; one
        // can only write an id line once, hence the 'ap' and 'lb' booleans.
        auto it = targets.getIterator();
        auto ap = false;
        auto lb = false;
        while( it ){
          if( it->isa<NDK>() ){
            const auto& build = it->as<NDK>()->toBuild().tolower();
            if( !ap && build == "application"_64 ){
              rootPrj << "  id 'com.android.application' version '7.4.1' apply false\n";
              ap = true;
            }else if( !lb && build == "shared"_64 ){
              rootPrj << "  id 'com.android.library' version '7.4.1' apply false\n";
              lb = true;
            }else if( !lb && build == "static"_64 ){
              rootPrj << "  id 'com.android.library' version '7.4.1' apply false\n";
              lb = true;
            }
          }
          ++it;
        }
        rootPrj << "}\n";
        rootPrj.save();

        //----------------------------------------------------------------------
        // Add all sub-directories to gradle.settings file. This is logically a
        // workspace/solution file. THe anchor of the project.
        //----------------------------------------------------------------------

        it = targets.getIterator();
        while( it ){
          if( it->isa<NDK>() ){
            const auto& ndk_proj = it->as<NDK>().cast();
            const auto& ndk_name = ndk_proj.toLabel();
            fs << "include('"
               << ndk_name
               << "')\n";
            const auto& ndk_path
              = ndk_root
              + "/"
              + ndk_name;
            if( !ndk_proj.inSources( NDK::Type::kCpp ).empty() )
              e_mkdir( ndk_path+ "/cpp" );
            if( !ndk_proj.inSources( NDK::Type::kHpp ).empty() ||
                !ndk_proj.inSources( NDK::Type::kH   ).empty() )
              e_mkdir( ndk_path + "/public" );
            if( !ndk_proj.inSources( NDK::Type::kC   ).empty() )
              e_mkdir( ndk_path + "/c" );
            #if 0 // TODO: Need a big switch on hashed extension here.
              e_mkdir( ndk_path + "/res" );
            #endif
            Writer subPrj( ndk_path
              + "/build.gradle"
              , kTEXT );
            ndk_proj
              . serialize( subPrj );
            subPrj.save();
          }
          ++it;
        }
        bmp->bGradle = 0;
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::serialize( Writer& fs )const{
        serializeSln2019( fs );
        serializeSln2022( fs );
        serializeXcode(   fs );
        serializeQmake(   fs );
        serializeNinja(   fs );
        serializeGradle(  fs );
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
        const_cast<self*>( this )->m_vTargets.clear();
      }

    //}:                                          |
    //again:{                                     |

      void Workspace::again(){
        m_vTargets.foreach(
          [this]( auto& hTarget ){

            //------------------------------------------------------------------
            // Bail conditions.
            //------------------------------------------------------------------

            if( !hTarget )
              return;

            //------------------------------------------------------------------
            // Xcode projects get reset here.
            //------------------------------------------------------------------

            if( m_tStates->bXcode11 ||
                m_tStates->bXcode12 ||
                m_tStates->bXcode14 ){
              auto hXcode = hTarget.template as<Xcode>();
              hXcode->reset();
            }
          }
        );
      }

    //}:                                          |
  //}:                                            |
  //getTargets:{                                  |

    strings Workspace::getTargets(){
      strings targets;
      if( bmp->osMac )
        targets.push( "macos" );
      if( bmp->osIphone )
        targets.push( "ios" );
      if( targets.empty() )
        targets.push( "macos" );
      return targets;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
