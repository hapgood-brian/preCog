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
    //serialize:{                                 |

      void Workspace::MSVC::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Count the number of unity processors and double it. This is how many
        // files we'll compile at one time on different threads.
        //----------------------------------------------------------------------

        u32 kLimit = std::thread::hardware_concurrency();
        if( kLimit > toUnity().capacity() ){
            kLimit = toUnity().capacity();
        }
        kLimit <<= 1;

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        if( anon_isUnityBuild() ){
          u32 i = 0;
          unifyProject<MSVC>( fs, Type::kCpp, kLimit, 0, i );
          unifyProject<MSVC>( fs, Type::kC,   kLimit, 1, i );
          writeProject<MSVC>( fs, Type::kCpp, kLimit, 0 );
          writeProject<MSVC>( fs, Type::kC,   kLimit, 1 );
        }

        //----------------------------------------------------------------------
        // Save MSVC project to pbx format bundle.
        //----------------------------------------------------------------------

      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
