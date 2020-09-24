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
          e_rm( dirPath );
          e_md( dirPath );
          Writer fs( dirPath
            + "/project.pbxproj"
            , kTEXT );
          proj.serialize( fs );
          e_msgf(
            "  Saving %s"
            , ccp( fs.toFilename() ));
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Ninja project for Unix, Linux and Android Studio.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bNinja && e_isa<Workspace::Ninja>( &proj )){
          const auto& dirPath = filename.path();
          const auto& prjName = dirPath + "build.ninja";
          if( !dirPath.empty() ){
            e_rm( dirPath );
            e_md( dirPath );
          }
          Writer fs( prjName, kTEXT );
          proj.serialize( fs );
          e_msgf(
            "  Saving %s"
            , ccp( fs.toFilename() ));
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Visual Studio 2019 project.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bVS2019 && e_isa<Workspace::MSVC>( &proj )){
          const auto& dirPath = filename.path();
          const auto& vcxproj = static_cast<const Workspace::MSVC&>( proj );
          const auto& prjName = dirPath + vcxproj.toLabel() + ".vcxproj";
          if( !dirPath.empty() ){
            e_rm( dirPath );
            e_md( dirPath );
          }
          Writer fs( prjName, kTEXT );
          proj.serialize( fs );
          e_msgf(
            "  Saving %s"
            , ccp( fs.toFilename() ));
          fs.save();
        }
      }
    }

  //}:                                            |
//}:                                              |
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_specialized_extends( Workspace::Project<17> );
  e_specialized_extends( Workspace::Project<8> );
  e_specialized_extends( Workspace::Project<0> );
  e_extends( Workspace );

//}:                                              |
//Methods:{                                       |
  //[workspace]:{                                 |
    //isUnityBuild:{                              |

#ifdef __APPLE__
  #pragma mark - Methods -
#endif

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
    //serialize*:{                                |

      void Workspace::serializeXcode( Writer& fs )const{
        if( m_tFlags->bXcode11 && ( fs.toFilename().ext().tolower().hash() == e_hashstr64_const( ".xcworkspacedata" ))){

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
                case e_hashstr64_const( "framework" ):
                  [[fallthrough]];
                case e_hashstr64_const( "shared" ):
                  [[fallthrough]];
                case e_hashstr64_const( "static" ):
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
                case e_hashstr64_const( "application" ):
                  [[fallthrough]];
                case e_hashstr64_const( "console" ):
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

          const_cast<Workspace*>( this )->m_tFlags->bXcode11 = 0;
          Workspace::bmp->bXcode11 = 0;
          return;
        }
      }

      void Workspace::serializeSln2019( Writer& fs )const{
        if( m_tFlags->bVS2019 && ( fs.toFilename().ext().tolower().hash() == e_hashstr64_const( ".sln" ))){

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

          const_cast<Workspace*>( this )->m_tFlags->bVS2019 = 0;
          Workspace::bmp->bVS2019 = 0;
          return;
        }
      }

      void Workspace::serializeNinja( Writer& fs )const{
        if( m_tFlags->bNinja && ( fs.toFilename().ext().tolower().hash() == ".ninja"_64 )){

          //--------------------------------------------------------------------
          // We're done with this target so turn it off for the rest of the run.
          //--------------------------------------------------------------------

          const_cast<Workspace*>( this )->m_tFlags->bNinja = 0;
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
    //cleanup:{                                   |

      void Workspace::cleanup()const{
        auto it = const_cast<Workspace*>( this )->m_vTargets.getIterator();
        while( it ){
          if( it->isa<Xcode>() ){
            if( !m_tFlags->bXcode12 ){
              it.erase();
              continue;
            }
            if( !m_tFlags->bXcode11 ){
              it.erase();
              continue;
            }
          }else if( it->isa<MSVC>() ){
            if( !m_tFlags->bVS2019 ){
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
