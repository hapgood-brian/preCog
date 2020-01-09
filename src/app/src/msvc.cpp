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
//}:                                              |
//Private:{                                       |
  //ignoreFile:{                                  |

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
//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //extFromSource<>:{                           |

      ccp Workspace::MSVC::extFromEnum( const Type e )const{
        switch( e ){
          case decltype( e )::kCpp:
            return ".cpp";
          case decltype( e )::kC:
            return ".c";
          default:
            return "";
        }
      }

    //}:                                          |
    //sortingHat:{                                |

      void Workspace::MSVC::sortingHat( const string& in_path ){

        //----------------------------------------------------------------------
        // Solutions need Windows GUIDs. If we're running on Windows then you
        // are guaranteed to have a GUID that's universally unique. If running
        // on the Mac or Linux, you're only guaranteed to have file uniqueness.
        //----------------------------------------------------------------------

        File path = in_path;
        path.setBuildID( string::guid() );
        path.setRefID(   string::guid() );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case e_hashstr64_const( ".lib" ):
            inSources( Type::kLib ).push( path );
            break;

          //--------------------------------------------------------------------
          // Source and header file types.
          //--------------------------------------------------------------------

          case e_hashstr64_const( ".png" ):
            inSources( Type::kPng ).push( path );
            break;
          case e_hashstr64_const( ".inl" ):
            inSources( Type::kInl ).push( path );
            break;
          case e_hashstr64_const( ".hpp" ):
          case e_hashstr64_const( ".hxx" ):
          case e_hashstr64_const( ".hh" ):
            inSources( Type::kHpp ).push( path );
            break;
          case e_hashstr64_const( ".cpp" ):
          case e_hashstr64_const( ".cxx" ):
          case e_hashstr64_const( ".cc" ):
            inSources( Type::kCpp ).push( path );
            break;
          case e_hashstr64_const( ".h" ):
            inSources( Type::kH ).push( path );
            break;
          case e_hashstr64_const( ".c" ):
            inSources( Type::kC ).push( path );
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

    //}:                                          |
    //serialize:{                                 |

      void Workspace::MSVC::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        if( !anon_isUnityBuild() ){
          const_cast<MSVC*>( this )->toUnity().resize( 1 );
        }else{
          u32 cores = std::thread::hardware_concurrency();
          u32 i=0;
          const_cast<MSVC*>( this )->toUnity().resize( cores );
          const_cast<MSVC*>( this )->unifyProject<MSVC>( fs, Type::kCpp, i );
          const_cast<MSVC*>( this )->unifyProject<MSVC>( fs, Type::kC,   i );
          writeProject<MSVC>( fs, Type::kCpp );
          writeProject<MSVC>( fs, Type::kC );
        }

        //----------------------------------------------------------------------
        // Save MSVC project to XML format file.
        //----------------------------------------------------------------------

      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
