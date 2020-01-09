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

  #if e_compiling( osx )
    void verifyPBX( const string& path );
  #endif

//}:                                              |
//Private:{                                       |
  //isUnityBuild:{                                |

    namespace{
      bool anon_isUnityBuild(){
        auto it = IEngine::args.getIterator();
        while( it ){
          if( it->tolower().hash() == e_hashstr64_const( "--unity" )){
            return true;
          }
          ++it;
        }
        return false;
      }
    }

  //}:                                            |
  //saveProject:{                                 |

    namespace{

      void anon_saveProject( const string& filename, const Workspace::Target& proj ){

        //----------------------------------------------------------------------
        // Save out the Xcode project.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bXcode11 && e_isa<Workspace::Xcode>( &proj )){
          #if e_compiling( microsoft )
            auto* ss = _strdup( filename.path() );
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
          const string& dirName = string( ss, ee ) + "/" + xcodeProj.toLabel() + ".xcodeproj";
          free( cp( ss ));
          if( !IEngine::dexists( dirName )){
            e_md( dirName );
          }
          fs::Writer fs( dirName + "/project.pbxproj", fs::kTEXT );
          proj.serialize( fs );
          fs.save();
        }

        //----------------------------------------------------------------------
        // Save out the Visual Studio 2019 project.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bVS2019 && e_isa<Workspace::MSVC>( &proj )){
          const auto& vcxproj = static_cast<const Workspace::MSVC&>( proj );
          const auto& dirPath = filename.path();
          const auto& dirName = dirPath + vcxproj.toLabel() + ".vcxproj";
          if( !IEngine::dexists( dirName )){
            e_md( dirName );
          }
          fs::Writer fs( dirName, fs::kTEXT );
          proj.serialize( fs );
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

  e_extends( Workspace::Project<17> );
  e_extends( Workspace::Project<8> );
  e_extends( Workspace::Project<0> );
  e_extends( Workspace );

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

        if( m_tFlags->bXcode11 ){

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
                case e_hashstr64_const( "shared" ):
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

        //----------------------------------------------------------------------
        // Generate Visual Studio 2019 on Windows 10.
        //----------------------------------------------------------------------

        if( m_tFlags->bVS2019 ){

          fs << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
          fs << "# Visual Studio 16\n";

          //--------------------------------------------------------------------
          // Construct vcxproj's for libraries and applications.
          //--------------------------------------------------------------------

          const auto& wsguid = string::guid();
          auto it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<MSVC>() ){
              const auto& proj = it->as<MSVC>().cast();
              fs << "Project(\""
                + wsguid + "\") = \""
                + proj.toLabel()
                + "\", \"tmp/"
                + proj.toLabel()
                + ".vcxproj\", \""
                + proj.toProjectGUID()
                + "\"\n";
              // ProjectSection(ProjectDependencies) + postProject
              // EndProjectSection
              fs << "EndProject\n";
              anon_saveProject( fs.toFilename(), proj );
            }
            ++it;
          }

          //--------------------------------------------------------------------
          // Write out the Global sections.
          //--------------------------------------------------------------------

          fs << "Global\n";
          fs << "  GlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
          fs << "    Debug|x64 = Debug|x64\n";
          fs << "    Release|x64 = Release|x64\n";
          fs << "  EndGlobalSection\n";
          fs << "  GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
          it = m_vTargets.getIterator();
          while( it ){
            if( it->isa<MSVC>() ){
              const auto& proj = it->as<MSVC>().cast();
              fs << "    "
                + proj.toProjectGUID()
                + ".Debug|x64.ActiveCfg = Debug|x64\n";
              fs << "    "
                + proj.toProjectGUID()
                + ".Release|x64.ActiveCfg = Release|x64\n";
            }
            ++it;
          }
          fs << "  EndGlobalSection\n";
          fs << "  GlobalSection(ExtensibilityGlobals) = postSolution\n";
          const auto& slnguid = string::guid();
          fs << "    SolutionGuid = "
            + slnguid
            + "\n";
          fs << "  EndGlobalSection\n";
          fs << "  GlobalSection(ExtensibilityAddIns) = postSolution\n";
          fs << "  EndGlobalSection\n";
          fs << "EndGlobal\n";

          //--------------------------------------------------------------------
          // We're done with this target so turn it off for the rest of the run.
          //--------------------------------------------------------------------

          const_cast<Workspace*>( this )->m_tFlags->bVS2019 = 0;
          Workspace::bmp->bVS2019 = 0;
          return;
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
