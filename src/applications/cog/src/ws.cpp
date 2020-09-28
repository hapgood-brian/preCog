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
          const auto& ninjaProj = static_cast<const Workspace::Ninja&>( proj );
          e_msgf( "Serializing %s", ccp( filename ));
          Writer fs( filename, kTEXT );
          ninjaProj.serialize( fs );
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

  e_specialized_extends( Workspace::Project<17> );
  e_specialized_extends( Workspace::Project<8> );
  e_specialized_extends( Workspace::Project<0> );
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
        if( bmp->bNinja && ( fs.toFilename().ext().tolower().hash() == ".ninja"_64 )){

          //--------------------------------------------------------------------
          // Setup main ninja file.
          //--------------------------------------------------------------------

          fs << "ninja_required_version = 1.5\n\n";

          //--------------------------------------------------------------------
          // Handle Ninja targets.
          //--------------------------------------------------------------------

          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<Ninja>() ){
              const auto& ninjaProj = it->as<Ninja>().cast();
              switch( ninjaProj.toBuild().tolower().hash() ){
                case"application"_64:
                  [[fallthrough]];
                case"static"_64:
                  [[fallthrough]];
                case"shared"_64:
                  [[fallthrough]];
                case"console"_64:
                  fs << "include " + ninjaProj.toLabel() + ".rules\n";
                  anon_saveProject(
                      "tmp/" + ninjaProj.toLabel() + ".rules"
                    , ninjaProj );
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
              const auto& target = hTarget.as<Ninja>().cast();
              files.pushVector( target.inSources( Ninja::Type::kCpp ));
              files.pushVector( target.inSources( Ninja::Type::kC   ));
              files.sort(
                []( const File& a, const File& b ){
                  return(
                      a.filename().tolower()
                    < b.filename().tolower()
                  );
                }
              );
              files.foreach(
                [&]( const File& file ){
                  const auto& ext = file.ext().tolower();
                  switch( ext.hash() ){
                    case".cpp"_64:
                      fs << "build ../"
                         << static_cast<const string&>( file )
                         << ".o: CXX_"
                         << target.toLabel().toupper()
                         << " "
                         << static_cast<const string&>( file )
                         << "\n";
                      fs << "  DEP_FILE = .intermediate/"
                         << target.toLabel().tolower()
                         << "/"
                         << file.basename()
                         << ".cpp.o.d\n";
                      fs << "  FLAGS = -std=gnu++17\n";
                      if( !target.toIncludePaths().empty() ){
                        fs << "\n";
                        fs << "  OBJECT_DIR = .intermediate/"
                           << target.toLabel().tolower()
                           << "\n";
                        fs << "  OBJECT_FILE_DIR = .intermediate/"
                           << target.toLabel().tolower()
                           << "\n";
                        fs << "  TARGET_COMPILE_PDB = .intermediate/"
                           << target.toLabel().tolower()
                           << "\n";
                        fs << "  TARGET_PDB = .intermediate/"
                           << target.toLabel().tolower()
                           << ".pdb\n";
                        fs << "\n";
                      }
                      break;
                  }
                }
              );
              files.clear();
            }
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
