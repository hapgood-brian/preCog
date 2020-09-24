//------------------------------------------------------------------------------
//       Copyright 2014-2018 Creepy Doll Games LLC. All rights reserved.
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

#include<unistd.h>
#include<dirent.h>
#define _SVID_SOURCE
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>
#include<errno.h>
#include"glue.h"

using namespace EON;
using namespace gfc;

using OnOpenCompletion = std::function<void( const strings& paths )>;
using OnSaveCompletion = std::function<void( const string&  path )>;
using OnCancel         = std::function<void()>;
using OnOK             = std::function<void()>;

//================================================|=============================
//Glue data:{                                     |
  //Structs:{                                     |
    //ShmInfo:{                                   |

      class ShmInfo{
        e_var_ptr( char, Ptr  ) = nullptr;
        e_var_string(    Key  );
        e_var( u64,   u, Size ) = 0;
        e_var( s32,   s, Shm  ) =-1;
        e_var_bits(      Flags
          , bServer:1
        );
      };

    //}:                                          |
  //}:                                            |
  //Globals:{                                     |

    bool g_bTerminate = false;
    f32  g_fScale     = 0.f;
    f32  g_fW         = 0.f;
    f32  g_fH         = 0.f;

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
      u32                  s_tidMainThread = Thread::tid();
    }

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
      //toStreamPath:{                          |

        string IEngine::toStreamPath(){
          string bundlePath = toBundlePath();
          bundlePath += "Contents/Streams/";
          return bundlePath;
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
            //TODO: Code must be implemented for Linux.
          }
          return bundlePath;
        }

      //}:                                        |
      //toEonPath:{                               |

        string IEngine::toEonPath(){
          string eonPath;
          if( !s_sPackagePath.empty() ){
            if(( '.' == *s_sPackagePath )&&( '/' == s_sPackagePath[ 1 ])){
              eonPath += toStreamPath();
              eonPath += s_sPackagePath;
            }else{
              eonPath += s_sPackagePath;
            }
            eonPath += "/.eon/";
          }else{
            eonPath = toStreamPath();
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
          // TODO: Implement this method.
          return false;
        }

      //}:                                        |
      //ssync:{                                   |

        bool IEngine::ssync( const string& key ){
          // TODO: Implement this method.
          return false;
        }

      //}:                                        |
      //share:{                                   |

        //http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm-posix-producer.c
        //http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm-posix-consumer.c
        //http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm.html

        cp IEngine::share( const gfc::string& path, const u64 req_bytes, const bool bServer ){
          // TODO: Implement this method.
          return nullptr;
        }

      //}:                                        |
    //}:                                          |
    //[io]:{                                      |
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
          struct stat st;
          const auto ok = stat( path.os(), &st );
          if( !ok ){
            return S_ISDIR( st.st_mode );
          }
          return false;
        }

      //}:                                        |
      //fexists:{                                 |

        bool IEngine::fexists( const string& path ){
          struct stat st;
          const auto ok = stat( path.os(), &st );
          if( !ok ){
            return true;
          }
          return false;
        }

      //}:                                        |
      //fopen:{                                   |

        FILE* IEngine::fopen( const string& in_path, ccp mode ){
          const string& path = in_path.os();

          //--------------------------------------------------------------------
          // Recursively follow symbolic links. https://stackoverflow.com/questions/5525668/how-to-implement-readlink-to-find-the-path
          //--------------------------------------------------------------------

          struct stat st;
          lstat( path, &st );
          if( S_ISLNK( st.st_mode )){
            char buf[ PATH_MAX ];
            ssize_t len = readlink( path, buf, sizeof( buf )-1 );
            if( len == -1 ){
              return nullptr;
            }
            buf[ len ]=0;
            FILE* f = fopen( s_sPackagePath + "/" + buf, mode );
            if( !f ){
              f = fopen( buf, mode );
            }
            return f;
          }

          //--------------------------------------------------------------------
          // Create write dirctory and open the file.
          //--------------------------------------------------------------------

          return::fopen( path.os(), mode );
        }

      //}:                                        |
      //dir:{                                     |

        bool IEngine::dir( const string& cPath, const std::function<void( const string&, const string&, const bool )>& lambda ){
          if( cPath.empty() ){
            return false;
          }
          string path = cPath;
          if( '/' != *path.right( 1 )){
            path += '/';
          }
          DIR* D = opendir( path.os() );
          if( !D ){
            return false;
          }
          dirent* ent;
          while(( ent = readdir( D )) != nullptr ){
            const string& subpath = path + ent->d_name;
            DIR* tmp = opendir( subpath );
            if( tmp ){
              if(( *ent->d_name != '.' )&&( *ent->d_name != '_' )){
                lambda( path, ent->d_name, true );
                dir( subpath, lambda );
              }
              closedir( tmp );
            }else{
              lambda( path, ent->d_name, false );
            }
          }
          closedir( D );
          return true;
        }

      //}:                                        |
    //}:                                          |
    //runOnMainThread:{                           |

      void IEngine::runOnMainThread( const std::function<void()>& lambda ){}

    //}:                                          |
    //isMainThread:{                              |

      bool IEngine::isMainThread(){
        return( Thread::tid() == s_tidMainThread );
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
          cvArgs.foreach(
            [&]( const string& cArg ){
              string s( cArg );
              s.replace( " ", "\\ " );
              arg += " ";
              arg += s;
            }
          );
          system( e_xfs( "%s %s ", cmd.c_str(), arg.c_str() ));
        };
        syscall();
        lambda();
        return true;
      }

    //}:                                          |
    //spawn:{                                     |

      bool IEngine::spawn( const string& program, const strings& userArgs, const bool bBlocking, const std::function<void( const s32 )>& lambda ){
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
