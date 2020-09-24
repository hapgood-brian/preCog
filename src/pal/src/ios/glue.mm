//------------------------------------------------------------------------------
//          Copyright 2014 RED Engine Games, LLC. All rights reserved.
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

#import"UIKit/UIKit.h"
#import<sys/stat.h>
#import<copyfile.h>
#import<dirent.h>
#import"view.h"
#import"glue.h"

using namespace ri;

//================================================|=============================
//Glue data:{                                     |
  //Private:{                                     |

    namespace{
      AppDelegate* s_pDelegate;
      string       s_sPackagePath;
      string       s_sTitle;
      string       s_stdOut;
      string       s_stdErr;
      bool         g_bTerminate;
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
          listen<IPeer>::trigger( &IScreen::onBackground );
        }
      }

    //}:                                          |
    //onForegroundDispatch:{                      |

      namespace{
        void onForegroundDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onForeground();
          }
          listen<IPeer>::trigger( &IScreen::onForeground );
        }
      }

    //}:                                          |
    //onActivationDispatch:{                      |

      namespace{
        void onActivationDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onActivating();
          }
          listen<IPeer>::trigger( &IScreen::onActivating );
        }
      }

    //}:                                          |
    //onResigningDispatch:{                       |

      namespace{
        void onResigningDispatch(){
          if( IEngine::screen ){
            IEngine::screen->onResigning();
          }
          listen<IPeer>::trigger( &IScreen::onResigning );
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
          // Notify active screen and engine peers.
          //--------------------------------------------------------------------

          // This is important because on Mac OSX the destructor is never called
          // for this class; always obliterate here.
          for(;;){
            IEngine::screen->onResigning();
            IEngine::screen->onTerminating();
            if( IEngine::popScreen() ){
              continue;
            }
            IEngine::screen = 0;
            break;
          }

          // Let engine peers know we're terminating everything.
          listen<IPeer>::trigger( &IScreen::onTerminating );

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
            Class::Factory::foreach( [&count]( const Object* t ){
              e_logf( "%s(%d): UUID %lld has %d refs", t->file, t->line, t->UUID, t->refs() );
              ++count;
            });
            e_logf( "%u leaks", count );
            ++count;
          #endif
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
//AppDelegate:{                                   |

#ifdef __APPLE__
  #pragma mark - AppDelegate -
#endif

  @implementation AppDelegate
    //applicationWillEnterForeground:{            |
    
      -(void)applicationWillEnterForeground:(UIApplication*)application{
        onForegroundDispatch();
      }

    //}:                                          |
    //applicationDidEnterBackground:{             |
    
      -(void)applicationDidEnterBackground:(UIApplication*)application{
        onBackgroundDispatch();
      }

    //}:                                          |
    //didFinishLaunchingWithOptions:{             |

      -(BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions{
        [ UIApplication sharedApplication ].idleTimerDisabled = YES;
        self.window=[[ UIWindow alloc ]initWithFrame:[[ UIScreen mainScreen ]bounds ]];
        self.viewController=[[ ViewController alloc ]initWithNibName:nil bundle:nil ];
        self.window.rootViewController = self.viewController;
        return YES;
      }

    //}:                                          |
    //applicationDidFinishLaunching:{             |

      -(void)applicationDidFinishLaunching:(UIApplication*)application{
      }

    //}:                                          |
    //applicationWillResignActive:{               |
    
      -(void)applicationWillResignActive:(UIApplication*)application{
        onResigningDispatch();
      }

    //}:                                          |
    //applicationDidBecomeActive:{                |
    
      -(void)applicationDidBecomeActive:(UIApplication*)application{
        onActivationDispatch();
      }

    //}:                                          |
    //applicationWillTerminate:{                  |
    
      -(void)applicationWillTerminate:(UIApplication*)application{
        onTerminateDispatch();
      }

    //}:                                          |
  @end

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

        //https://developer.apple.com/library/mac/documentation/CoreFoundation/Conceptual/CFBundles/BundleTypes/BundleTypes.html#//apple_ref/doc/uid/10000123i-CH101-SW1
        //https://developer.apple.com/library/mac/technotes/tn2206/_index.html#//apple_ref/doc/uid/DTS40007919-CH1-TNTAG20

        string IEngine::toBundlePath(){
          static char bundlePath[ 384 ]={ 0 };
          if( !*bundlePath ){
            strcpy( bundlePath, [[[NSBundle mainBundle] bundlePath] UTF8String] );
            strcat( bundlePath, "/" );
          }
          return bundlePath;
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
          return false;
        }

      //}:                                        |
      //openFileSheet:{                           |

        bool IEngine::openFileSheet( const strings& vStrings, ccp pBaseDir, bool bFiles, bool bDirs, const std::function<void( const strings& paths )>& in_onCompletion, const std::function<void()>& in_onCancel ){
          return false;
        }

      //}:                                        |
      //tempPath:{                                |

        string IEngine::tempPath(){
          NSString* tempDir = NSTemporaryDirectory();
          if( nil != tempDir ){
            return string([ tempDir UTF8String ]);
          }
          return homePath() + ".eon/";
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
          BOOL isDir = NO;
          if([[ NSFileManager defaultManager ]
              fileExistsAtPath:[[ NSString stringWithUTF8String:path.os().c_str() ] stringByExpandingTildeInPath ]
              isDirectory:&isDir ]){
            return( YES == isDir );
          }
          return false;
        }

      //}:                                        |
      //fexists:{                                 |

        bool IEngine::fexists( const string& path ){
          BOOL isDir = NO;
          if([[ NSFileManager defaultManager ]
              fileExistsAtPath:[[ NSString stringWithUTF8String:path.os().c_str() ]
              stringByExpandingTildeInPath ]
              isDirectory:&isDir ]){
            return( NO == isDir );
          }
          return false;
        }

      //}:                                        |
      //fopen:{                                   |

        FILE* IEngine::fopen( const string& path, ccp mode ){
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
        if([NSThread isMainThread]==YES ){
          return true;
        }
        return false;
      }

    //}:                                          |
    //areYouSure:{                                |

      bool IEngine::areYouSure( const string& title, const string& body, const std::function<void()>& in_onOK, const std::function<void()>& in_onCancel ){
        return false;
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
    //spawn:{                                     |

      bool IEngine::spawn( const string& program, const strings& userArgs, const bool bBlocking, const std::function<void( const s32 )>& lambda ){
        e_assert( IEngine::isMainThread() );
        strings args;
        args.push( program );
        args.pushVector( userArgs );
        int result = IPlugin::spawn( args );
        if( lambda ){
          lambda( result );
        }
        return result;
      }

    //}:                                          |
    //system:{                                    |

      bool IEngine::system( const gfc::string& cmd, const gfc::strings& args, const std::function<void()>& lambda ){
        return false;
      }

    //}:                                          |
    //exec:{                                      |

      bool IEngine::exec(
            const string& program
          , const strings& args
          , const bool bBlocking
          , const std::function<void( const s32 return_code )>& lambda ){
        return false;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                                vim: ft=objcpp
