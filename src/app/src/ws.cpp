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
      void anon_saveProject( const string& filename, const Workspace::VoidProject& proj ){
        if( Workspace::bmp->bXcode11 && e_isa<Workspace::Xcode>( &proj )){
          const auto& xcodeProj = static_cast<const Workspace::Xcode&>( proj );
          switch( xcodeProj.toTypeID().hash() ){
            case e_hashstr64_const( "pbx" ):/**/{
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
              const string dirName = string( ss, ee ) + "/" + xcodeProj.toLabel() + ".xcodeproj";
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
              auto it = m_vVoidProjects.getIterator();
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

              //----------------------------------------------------------------
              // Construct xcodeproj's for libraries.
              //----------------------------------------------------------------

              fs << "  <Group\n";
              fs << "    location = \"container:\"\n";
              fs << "    name = \"Apps\">\n";
              it = m_vVoidProjects.getIterator();
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
              break;
          }
        }

        //----------------------------------------------------------------------
        // Generate Visual Studio 2019 on Windows 10.
        //----------------------------------------------------------------------

        if( m_tFlags->bVS2019 ){
          // TODO: Implement visual studio solution.
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
