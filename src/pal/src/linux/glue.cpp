//------------------------------------------------------------------------------
//       Copyright 2014-2017 Creepy Doll Games LLC. All rights reserved.
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

#include<GL/glx.h>
#include<GL/gl.h>
#include<X11/X.h>
#include<X11/keysym.h>

#define _SVID_SOURCE
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>
#include<unistd.h>
#include<dirent.h>
#include<errno.h>

#include"glue.h"
#include"view.h"

using namespace EON;
using namespace gfc;

using OnOpenCompletion = std::function<void( const strings& paths )>;
using OnSaveCompletion = std::function<void( const string&  path )>;
using OnCancel         = std::function<void()>;
using OnOK             = std::function<void()>;

//================================================|=============================
//Glue data:{                                     |
  //Structs:{                                     |

    class ShmInfo{
      e_var_ptr( char, Ptr  ) = nullptr;
      e_var_string(    Key  );
      e_var( u64,   u, Size ) = 0;
      e_var( s32,   s, Shm  ) =-1;
      e_var_bits(      Flags
          , bServer:1 );
    };

  //}:                                            |
  //Private:{                                     |

    namespace{
      hashmap<u64,ShmInfo> s_mSharedMemory;
      OnOpenCompletion     s_onOpenCompletion;
      OnSaveCompletion     s_onSaveCompletion;
      OnCancel             s_onCancel;
      OnOK                 s_onOK;
      string               s_sPackagePath;
      string               s_sTitle;
      string               s_stdOut;
      string               s_stdErr;
      u32                  s_uStartW         = 0;
      u32                  s_uStartH         = 0;
      bool                 s_bDoubleBuffered = true;
      bool                 s_bSheetIsOpen    = false;
      bool                 g_bTerminate;
    }

  //}:                                            |
  //Methods:{                                     |
    //onBackgroundDispatch:{                      |

#ifdef __APPLE__
  #pragma mark - Dispatchers -
#endif

      namespace{
        void onBackgroundDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onBackground();
          }
          listen<IPeer>::trigger( &ri::IScreen::onBackground );
        }
      }

    //}:                                          |
    //onForegroundDispatch:{                      |

      namespace{
        void onForegroundDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onForeground();
          }
          listen<IPeer>::trigger( &ri::IScreen::onForeground );
        }
      }

    //}:                                          |
    //onTerminateDispatch:{                       |

      namespace{

        void onTerminateDispatch(){

          //--------------------------------------------------------------------
          // First tell the app delegate we're dying.
          //--------------------------------------------------------------------

          g_bTerminate = true;

          //--------------------------------------------------------------------
          // Unshare all shared memory pages.
          //--------------------------------------------------------------------

          if( !s_mSharedMemory.empty() ){
            hashmap<u64,ShmInfo>::iterator it = s_mSharedMemory.getIterator();
            while( it ){
              IEngine::unshare( it->toKey() );
              ++it;
            }
            s_mSharedMemory.clear();
          }

          //--------------------------------------------------------------------
          // Notify active screen and engine peers.
          //--------------------------------------------------------------------

          // This is important because on Mac OSX the destructor is never called
          // for this class; always obliterate here.
          for(;;){
            IEngine::screen->onTerminating();
            if( IEngine::popScreen() ){
              continue;
            }
            IEngine::screen = 0;
            break;
          }

          // Tell the renderer that we've lost our context.
          IEngine::ri->toFlags()->bContextLost = 1;

          // Let engine peers know we're terminating everything.
          listen<IPeer>::trigger( &ri::IScreen::onTerminating );

          //--------------------------------------------------------------------
          // Kill the renderer, screen, then exit.
          //--------------------------------------------------------------------

          // Purge the renderer of everything.
          IEngine::ri.purge();

          // Blow away the scratch buffer.
          ccp pScratch = e_scratch();
          if( pScratch ){
            e_free( pScratch );
          }

          // Kill the prefabs.
          IEngine::prefabs.clear();

          // Log all handle leaks.
          #if e_compiling( debug )
            u32 count = 0;
            ClassFactory::foreach( [&count]( const Object* t ){
              e_logf( "%s(%d): UUID %lld has %d refs", t->file, t->line, t->UUID, t->refs() );
              ++count;
            });
            e_logf( "%u leaks", count );
            ++count;
          #endif

          // Optionally kill all child processes.
          #if e_compiling( osx )
            //killpg( getpgid( getpid() ), SIGKILL );
          #endif
        }
      }

    //}:                                          |
    //onSaveFileDispatch:{                        |

      namespace{
        void onSaveFileDispatch( const string& path ){
          if( IEngine::screen ){
            IEngine::screen->onSaveSheet( path );
          }
          listen<IPeer>::trigger( &ri::IScreen::onSaveSheet, path );
        }
      }

    //}:                                          |
    //onOpenFileDispatch:{                        |

      namespace{
        void onOpenFileDispatch( const strings& strings ){
          if( IEngine::screen ){
            IEngine::screen->onOpenFile( strings );
          }
          listen<IPeer>::trigger( &ri::IScreen::onOpenFile, strings );
        }
        void onOpenFileDispatch( string& s ){
          strings v;
          v.push( s );
          onOpenFileDispatch( v );
        }
      }

    //}:                                          |
    //onCancelDispatch:{                          |

      namespace{
        void onCancelDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onCancel();
          }
          listen<IPeer>::trigger( &ri::IScreen::onCancel );
        }
      }

    //}:                                          |
    //onYesNoDispatch:{                           |

      namespace{
        void onYesNoDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onAction( u64( ri::Action::kYesNo ));
          }
          listen<IPeer>::trigger( &ri::IScreen::onAction, u64( ri::Action::kYesNo ));
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//IEngine:{                                       |
  //Methods:{                                     |
    //[paths]:{                                   |
      //setPackagePath:{                          |

#ifdef __APPLE__
  #pragma mark - IEngine methods -
#endif

        void IEngine::setPackagePath( const string& dir ){
          if( dir.empty() ){
            s_sPackagePath.clear();
          }else{
            s_sPackagePath = dir.os();
            while( s_sPackagePath.back() == '/' ){
              s_sPackagePath.trim( 1 );
            }
          }
        }

      //}:                                        |
      //toResourcePath:{                          |

        string IEngine::toResourcePath(){
          string bundlePath = toBundlePath();
          bundlePath += "Contents/Resources/";
          if( dexists( bundlePath )){
            return bundlePath;
          }
          return nullptr;
        }

      //}:                                        |
      //toPackagePath:{                           |

        string IEngine::toPackagePath(){
          if( s_sPackagePath.empty() ){
            return string( "~/" ).os();
          }
          return s_sPackagePath + "/";
        }

      //}:                                        |
      //toHelpersPath:{                           |

        //https://developer.apple.com/library/mac/technotes/tn2206/_index.html#//apple_ref/doc/uid/DTS40007919-CH1-TNTAG20

        string IEngine::toHelpersPath(){
          return toBundlePath() + "Contents/Helpers/";
        }

      //}:                                        |
      //toPluginsPath:{                           |

        string IEngine::toPluginsPath(){
          return toBundlePath() + "Contents/PlugIns/";
        }

      //}:                                        |
      //toBundlePath:{                            |

        string IEngine::toBundlePath(){
          return nullptr;
        }

      //}:                                        |
      //toEonPath:{                               |

        string IEngine::toEonPath(){
          string eonPath;
          if( !s_sPackagePath.empty() ){
            if(( '.' == *s_sPackagePath )&&( '/' == s_sPackagePath[ 1 ])){
              eonPath += toResourcePath();
              eonPath += s_sPackagePath;
            }else{
              eonPath += s_sPackagePath;
            }
            eonPath += "/.eon/";
          }else{
            eonPath = toResourcePath();
          }
          return eonPath;
        }

      //}:                                        |
      //exit:{                                    |

        void IEngine::exit(){
          ::exit( 0 );
        }

      //}:                                        |
    //}:                                          |
    //[ipc]:{                                     |
      //unshare:{                                 |

        bool IEngine::unshare( const string& key ){
          return false;
        }

      //}:                                        |
      //ssync:{                                   |

        bool IEngine::ssync( const string& key ){
          return false;
        }

      //}:                                        |
      //share:{                                   |

        cp IEngine::share( const gfc::string& path, const u64 req_bytes, const bool bServer ){
          return nullptr;
        }

      //}:                                        |
    //}:                                          |
    //[io]:{                                      |
      //saveFileSheet:{                           |

        bool IEngine::saveFileSheet( const strings& vExts, ccp pBaseDir, ccp pPrompt, ccp pTitle, const std::function<void( const string& path )>& in_onCompletion, const std::function<void()>& in_onCancel ){
          return true;
        }

      //}:                                        |
      //openFileSheet:{                           |

        bool IEngine::openFileSheet( const strings& vStrings, ccp pBaseDir, bool bFiles, bool bDirs, const std::function<void( const strings& paths )>& in_onCompletion, const std::function<void()>& in_onCancel ){
          return true;
        }

      //}:                                        |
      //tempPath:{                                |

        string IEngine::tempPath(){
          return"/tmp";
        }

      //}:                                        |
      //homePath:{                                |

        string IEngine::homePath(){
          static string s_sHomePath;
          if( s_sHomePath.empty() ){
            s_sHomePath = e_getenv( "HOME" ) + "/";
          }
          return s_sHomePath;
        }

      //}:                                        |
      //dexists:{                                 |

        bool IEngine::dexists( const string& path ){
          return false;
        }

      //}:                                        |
      //fexists:{                                 |

        bool IEngine::fexists( const string& path ){
          return false;
        }

      //}:                                        |
      //fopen:{                                   |

        FILE* IEngine::fopen( const string& path, ccp mode ){
          if( strchr( mode, 'w' )){
            md( path );
          }
          return::fopen( path.os(), mode );
        }

      //}:                                        |
      //dir:{                                     |

        bool IEngine::dir( const string& cPath, const std::function<void( const string&, const string&, const bool )>& lambda ){
          string path = cPath;
          if( '/' != *path.right( 1 )){
            path += '/';
          }
          DIR* dir = opendir( path.os() );
          if( dir ){
            dirent* ent;
            while(( ent = readdir( dir )) != nullptr ){
              const string& subpath = path + ent->d_name;
              DIR* tmp = opendir( subpath );
              if( tmp ){
                if(( *ent->d_name != '.' )&&( *ent->d_name != '_' )){
                  lambda( path, ent->d_name, true );
                }
                closedir( tmp );
              }else{
                lambda( path, ent->d_name, false );
              }
            }
            closedir( dir );
          }
          return true;
        }

      //}:                                        |
    //}:                                          |
    //isMainThread:{                              |

      bool IEngine::isMainThread(){
        return false;
      }

    //}:                                          |
    //areYouSure:{                                |

      bool IEngine::areYouSure( const string& title, const string& body, const std::function<void()>& in_onOK, const std::function<void()>& in_onCancel ){
        return true;
      }

    //}:                                          |
    //stderr:{                                    |

      string& IEngine::getStderr(){
        return s_stdErr;
      }

    //}:                                          |
    //stdout:{                                    |

      string& IEngine::getStdout(){
        return s_stdOut;
      }

    //}:                                          |
    //system:{                                    |

      bool IEngine::system( const string& cmd, const strings& cvArgs, const std::function<void()>& lambda ){
        const auto syscall=[=](){
          string arg;
          cvArgs.foreach( [&]( const string& cArg ){
            string s( cArg );
            s.replace( " ", "\\ " );
            arg += " ";
            arg += s;
          });
          system( e_xfs( "%s %s ", cmd.c_str(), arg.c_str() ));
        };
        if( lambda ){
          std::thread( [=](){
            syscall();
            lambda();
          });
        }else{
          syscall();
        }
        return true;
      }

    //}:                                          |
    //spawn:{                                     |

      bool IEngine::spawn( const string& program, const strings& userArgs, const bool bBlocking, const std::function<void( const s32 )>& lambda ){
        if( !IEngine::isMainThread() ){
          return false;
        }
        const auto& onSpawn=[=]()->int{
          strings args;
          args.push( program );
          args.pushVector( userArgs );
          const int result = IPlugin::spawn( args );
          if( lambda ){
            lambda( result );
          }
          return result;
        };
        if( bBlocking ){
          return !onSpawn();
        }
        e_runAsync( onSpawn );
        return true;
      }

    //}:                                          |
    //exec:{                                      |

      bool IEngine::exec( const string& program, const strings& userArgs, const bool bBlocking, const std::function<void( const s32 )>& lambda ){
        return false;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
