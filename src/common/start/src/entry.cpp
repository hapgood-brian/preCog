//------------------------------------------------------------------------------
//       Copyright 2014-2020 Creepy Doll Games LLC. All rights reserved.
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

#if !e_compiling( web )
  #include<boost/filesystem.hpp>
//#include<hash/MurmurHash3.h>
  #if e_compiling( osx )
    #include<sys/sysctl.h>
    #include<sys/stat.h>
    #include<unistd.h>
    #include<signal.h>
  #elif e_compiling( microsoft )
    extern"C"{
      bool        __stdcall CreateDirectoryA( const char*, void* );
      const char* __stdcall GetCommandLineA();
    }
  #endif
  #include<sstream>
#endif

using namespace EON;
using namespace gfc;

//================================================|=============================
//IEngine:{                                       |
  //Methods:{                                     |
    //[app]:{                                     |
      //main:{                                    |

        /* Define entry points for Windows and Android */

        #if e_compiling( android )
          namespace{
            vp s_pAndroidNativeState = nullptr;
          }
          void android_main( android_app* state ){
            // Save off the android native state.
            s_pAndroidNativeState = state;
            // Get the package path; can't do anything if it's null.
            if( !e_getCvar( bool, "COMPILE_EDITOR_PACKAGE" )){
              const string& streamPath = IEngine::toStreamPath();
              if( !streamPath.empty() ){
                // Load all prefabs and store locally for lifetime of program.
                IEngine::prefabs = Prefab::get( streamPath );
              }
            }
            // Finally we can enter the application entry-point.
            IEngine::main( IEngine::args );
          }
        #elif e_compiling( microsoft )
          int __stdcall WinMain( vp, vp, ccp, int ){
            // Before we can do anything with the engine we must load the args.
            { std::stringstream ss( GetCommandLineA() );
              std::string arg;
              while( ss >> arg ){
                string a( arg.c_str() );
                a.replace( "\\", "/" );
                if( *a == '"' ){
                  a.ltrim( 1 );
                  a.trim( 1 );
                }
                IEngine::args.push( a );
              }
            }
            IEngine::args.alter( 0,
              [&]( string& arg ){
                arg.replace( "\\", "/" );
              }
            );
            // Get the package path; can't do anything if it's null.
            if( !e_getCvar( bool, "COMPILE_EDITOR_PACKAGE" )){
              const string& streamPath = IEngine::toStreamPath();
              if( !streamPath.empty() ){
                // Load all prefabs and store locally for lifetime of program.
                IEngine::prefabs = Prefab::get( streamPath );
              }
            }
            // Run the game.
            return IEngine::main( IEngine::args );
          }
        #endif

        /* Define entry point for all the rest */

        #if !e_compiling( android )
          int main( s32 argc, cp argv[], cp envp[] ){
            // First of all let's construct our string pairs of env vars.
            for( u32 i=0; envp[ i ]; ++i ){
              cp L = envp[ i ];
              cp R = nullptr;
              if( e_strsplit( L, '=', R )){
                IEngine::envs.push( gfc::string_pair{ L, R });
                delete[] L;
              }
            }
            // Before we can do anything with the engine we must load the args.
            for( s32 i=0; i<argc; ++i ){
              const string a( argv[ i ]);
              // Change cvars on the command line.
              if( a.left( 7 ).tolower().hash() == "--setb="_64 ){
                const auto cvarName = a.ltrimmed( 7 );
                e_setCvar( cvarName, true );
                continue;
              }
              // Pass everything else to application.
              IEngine::args.push( a );
            }
            #if e_compiling( microsoft )
              IEngine::args.alter( 0, [&]( string& arg ){ arg.replace( "\\", "/" ); });
            #endif
            // Get the package path; can't do anything if it's null.
            if( !e_getCvar( bool, "COMPILE_EDITOR_PACKAGE" )){
              const auto& streamPath = IEngine::toStreamPath();
              if( !streamPath.empty() ){
                // Load all prefabs and store locally for lifetime of program.
                IEngine::prefabs = Prefab::get( streamPath );
                if( !IEngine::prefabs.empty() ){
                  e_msgf( "Prefabs loaded." );
                }
              }
            }
            // Run the game.
            return IEngine::main( IEngine::args );
          }
        #endif

      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
