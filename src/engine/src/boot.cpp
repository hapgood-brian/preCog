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

/* System services */

#include"sha1.h"
#include<eon/eon.h>
#if e_compiling( web )
  #include<sys/sysctl.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<unistd.h>
  #include<signal.h>
  #include<fcntl.h>
#elif e_compiling( osx )||e_compiling( ios )||e_compiling( linux )
  #include<boost/filesystem.hpp>
  #include<sys/sysctl.h>
  #include<sys/types.h>
  #include<sys/fcntl.h>
  #include<sys/stat.h>
  #include<unistd.h>
  #include<signal.h>
#elif e_compiling( microsoft )
  #include<boost/filesystem.hpp>
#endif
#include<sstream>
#include<variant>

/* Engine services */

using namespace EON;
using namespace gfc;

#define USE_UNIT_TESTS 0

//================================================|=============================
//Externs:{                                       |

  #if e_compiling( microsoft )
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_LEAN
    #define VC_EXTRA_LEAN
    #define NOMINMAX
    #include<windows.h>
    #include<shlobj_core.h>
  #endif

  extern f32 g_fScale;
  extern f32 g_fW;
  extern f32 g_fH;

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Private:{                                       |
  //Cvars:{                                       |

    namespace{
      using cvar = std::variant<gfc::string,double,float,bool,u64,s64,u32,s32,u16,s16,u8,s8>;
      e_noinline gfc::hashmap<u64,cvar>& cvars(){
        static gfc::hashmap<u64,cvar> mCvars;
        return mCvars;
      }
    }

  //}:                                            |
  //Anon:{                                        |

    namespace{
      deque<std::function<void()>> s_dLambdas;
      pt2 s_tCursor;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Functions:{                                     |
  //e_singleton:{                                 |

    #undef e_singleton

    const std::pair<bool,vp> e_singleton( const std::size_t _sizeof, const u64 clsid ){
      static/* __thread */char singleton[ sizeof( hashmap<u64,cp> )];
      static/* __thread */bool gate = false;
      if( !gate ){
        new( singleton )hashmap<u64,cp>;
        gate = true;
      }
      auto& s = *reinterpret_cast<hashmap<u64,cp>*>( singleton );
      std::pair<bool,vp> result;
      result.first = s.find( clsid );
      if( !result.first ){
        s.setBy( clsid,
          [&]( cp& cptr ){
            result.second = cptr = cp( e_malloc( _sizeof ));
          }
        );
      }else{
        result.second = vp( s[ clsid ]);
      }
      return result;
    }

  //}:                                            |
  //e_getenv:{                                    |

    string e_getenv( ccp pEnv ){
      string s;
      #if e_compiling( microsoft )
        char envvar[260];
        if( !strcmp( pEnv, "HOME" )){
          SHGetFolderPathA( NULL, 0x0028, NULL, 0, envvar );
          s = envvar;
          s.replace( "\\", "/" );
        }else if( !strcmp( pEnv, "PWD" )){
          u32 dwResult = GetCurrentDirectoryA( e_dimof( envvar ), envvar );
          e_assert( dwResult );
          s = envvar;
          s.replace( "\\", "/" );
        }else{
          size_t size;
          switch( getenv_s( &size, envvar, e_dimof( envvar ), pEnv )){
            case ERANGE:/**/{
              char* pEnvVar = new char[ size ];
              size_t size = 0;
              if( getenv_s( &size, envvar, size, pEnv )){
                e_unreachable( "couldn't get environment variable" );
              }else{
                s = pEnvVar;
              }
              delete[] pEnvVar;
              break;
            }
            case EINVAL:
              e_unreachable( "couldn't get environment variable" );
              break;
            default:
              s = envvar;
              break;
          }
        }
      #else
        s = getenv( pEnv );
      #endif
      e_assert( !s.empty() );
      return s;
    }

  //}:                                            |
  //e_when:{                                      |

    u64 e_when(){
      return Object::gameTurn;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//IEngine:{                                       |
  //Private:{                                     |
    //[globals]:{                                 |

      namespace{
        f64 s_fLastTime = e_seconds();
      }

    //}:                                          |
    //copyDir:{                                   |

      namespace{
        #if !e_compiling( web )
          bool copyDir( const boost::filesystem::path& source, const boost::filesystem::path& destination ){
            try{
              if( !boost::filesystem::is_directory( source )){
                return false;
              }
              if( boost::filesystem::is_directory( destination.c_str() )){
                return false;
              }
              if( !boost::filesystem::create_directory( destination )){
                return false;
              }
            }
            catch( const boost::filesystem::filesystem_error& ){
              return false;
            }
            for( boost::filesystem::directory_iterator file( source ); file != boost::filesystem::directory_iterator(); ++file ){
              try{
                boost::filesystem::path current( file->path() );
                if( boost::filesystem::is_directory( current )){
                  if( !copyDir( current, destination / current.filename() )){
                    return false;
                  }
                }else{
                  boost::filesystem::copy_file( current, destination / current.filename() );
                }
              }
              catch( const boost::filesystem::filesystem_error& ){
              }
            }
            return true;
          }
        #endif
      }

    //}:                                          |
  //}:                                            |
  //Structs:{                                     |
    //Cvar:{                                      |
      //set:{                                     |

        #define CVAR_IMPL_SETTER(type)                                          \
        template<> bool IEngine::Cvar::set<type>(                               \
            const string& key                                                   \
          , const type& value ){                                                \
          cvars().set( key.hash(), value );                                     \
          return true;                                                          \
        }                                                                       \

        CVAR_IMPL_SETTER( string );
        CVAR_IMPL_SETTER( double );
        CVAR_IMPL_SETTER( float  );
        CVAR_IMPL_SETTER( bool   );
        CVAR_IMPL_SETTER( u64    );
        CVAR_IMPL_SETTER( s64    );
        CVAR_IMPL_SETTER( u32    );
        CVAR_IMPL_SETTER( s32    );
        CVAR_IMPL_SETTER( u16    );
        CVAR_IMPL_SETTER( s16    );
        CVAR_IMPL_SETTER( u8     );
        CVAR_IMPL_SETTER( s8     );

      //}:                                        |
      //get:{                                     |

        #define CVAR_IMPL_GETTER(type)                                          \
        template<> type IEngine::Cvar::get<type>( const string& key ){          \
          cvar c;                                                               \
          if( cvars().find( key.hash(), c )){                                   \
            return std::get<type>( c );                                         \
          }                                                                     \
          return ((type)0);                                                     \
        }                                                                       \

        CVAR_IMPL_GETTER( string );
        CVAR_IMPL_GETTER( double );
        CVAR_IMPL_GETTER( float  );
        CVAR_IMPL_GETTER( bool   );
        CVAR_IMPL_GETTER( u64    );
        CVAR_IMPL_GETTER( s64    );
        CVAR_IMPL_GETTER( u32    );
        CVAR_IMPL_GETTER( s32    );
        CVAR_IMPL_GETTER( u16    );
        CVAR_IMPL_GETTER( s16    );
        CVAR_IMPL_GETTER( u8     );
        CVAR_IMPL_GETTER( s8     );

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Globals:{                                     |

    vector<Prefab::handle>     IEngine::prefabs;
    strings                    IEngine::args;
    string_pairs               IEngine::envs;

  //}:                                            |
  //Methods:{                                     |
    //[hashing]:{                                 |
      //hash128:{                                 |

        u128 IEngine::hash128( cvp pData, const u32 size ){
          return 0;
        }

      //}:                                        |
    //}:                                          |
    //[debug]:{                                   |
      //isDebugging:{                             |

        bool IEngine::isDebugging(){
          #if e_compiling( microsoft )
            return IsDebuggerPresent();
          #elif e_compiling( osx )
            //https://developer.apple.com/library/mac/qa/qa1361/_index.html
            static bool bDebugging = false;
            static bool bGate = false;
            if( !bGate ){
              kinfo_proc info;
              info.kp_proc.p_flag = 0;
              s32 mib[ 4 ]={
                CTL_KERN,
                KERN_PROC,
                KERN_PROC_PID,
                getpid(),
              };
              size_t size = sizeof( info );
              sysctl( mib, e_dimof( mib ), &info, &size, nullptr, 0 );
              bDebugging=( 0!=( info.kp_proc.p_flag & P_TRACED ));
              bGate = true;
            }
            return bDebugging;
          #else
            return false;
          #endif
        }

      //}:                                        |
    //}:                                          |
    //[SHA1]:{                                    |

      string IEngine::sha1of( const stream& st ){
        string sha1;
        st.query( [&]( ccp pBuffer ){
          sha1 = sha1of( pBuffer, st.bytes() );
        });
        return sha1;
      }

      string IEngine::sha1of( cvp pBuf, const u64 size ){
        string s;
        SHA1 sha1;
        sha1.addBytes( ccp( pBuf ), size );
        const u8* digest = sha1.getDigest();
        for( u32 i=0; i<20; ++i ){
          s.catf( "%02X", digest[ i ]);
        }
        e_free( digest );
        return s;
      }

      string IEngine::sha1of( const string& s ){
        if( !s.empty() ){
          return sha1of( ccp( s ), s.len() );
        }
        return nullptr;
      }

    //}:                                          |
    //[app]:{                                     |
    //}:                                          |
    //fload:{                                     |

      stream IEngine::fload( const string& path ){
        stream out;
        FILE* f = e_fopen( path, "r" );
        if( ! f ){
          DEBUG_BREAK
        }else{
          fseek( f, 0, SEEK_END );
          const s64 result = ftell( f );
          fseek( f, 0, SEEK_SET );
          if( !result ){
            DEBUG_BREAK
          }else{
            cp pBuffer = out.alloc( result );
            fread( pBuffer, 1, result, f );
          }
          fclose( f );
        }
        return out;
      }

    //}:                                          |
    //mkdir:{                                     |

      bool IEngine::mkdir( const string& directories ){
        #if !e_compiling( web )
          if( !directories.empty() ){
            boost::system::error_code ec;
            return boost::filesystem::create_directories( directories.path().os().c_str(), ec );
          }
        #endif
        return false;
      }

    //}:                                          |
    //rm:{                                        |

      bool IEngine::rm( const string& path ){
        #if e_compiling( osx )
          ::system( "rm -rf " + path );
        #elif !e_compiling( web )
          if( !path.empty() ){
            const strings paths{
              toPackagePath() + "/" + path,
              path
            };
            bool bRemoved = false;
            paths.foreachs(
              [&]( const string& path ){
                boost::system::error_code ec;
                if( dexists( path )){
                  bRemoved=( 0 != boost::filesystem::remove_all( path.os().c_str(), ec ));
                }
                if( fexists( path )){
                  bRemoved = boost::filesystem::remove( path.os().c_str(), ec );
                }
                return !bRemoved;
              }
            );
            return bRemoved;
          }
        #endif
        return false;
      }

    //}:                                          |
    //cp:{                                        |

      void IEngine::copy( const string& source, const string& target ){
        #if !e_compiling( web )
          if( source.empty() || target.empty() ){
            e_assert( !"Source or target cannot be empty!" );
            return;
          }
          boost::system::error_code ec;
          if( fexists( source )){
            boost::filesystem::copy_file( source.os().c_str(), target.os().c_str(), ec );
            return;
          }
          e_assert( dexists( source ));
          rm( target );
          #if e_compiling( osx )
            string src = source.os();
            string dst = target.os();
            src.replace( "//", "/" );
            dst.replace( "//", "/" );
            copyDir( boost::filesystem::path( src ), boost::filesystem::path( dst ));
          #elif e_compiling( microsoft )
            string src = source.os();
            string dst = target.os();
            src.replace( "\\\\", "\\" );
            dst.replace( "\\\\", "\\" );
            copyDir(
                boost::filesystem::path( src.c_str() )
              , boost::filesystem::path( dst.c_str() )
            );
          #endif
        #endif
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//IPlugin:{                                       |
  //Private:{                                     |
    //[globals]:{                                 |

      namespace{
        hashmap<u64,IPlugin*> s_mPlugins;
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //spawn:{                                     |

      int IPlugin::spawn( const strings& args ){
        const string& name = args[ 0 ].filename();
        const u64 hash = name.hash();
        if( s_mPlugins.find( hash )){
          IPlugin* pPlugin = s_mPlugins[ hash ];
          if( pPlugin ){
            return pPlugin->main( args );
          }
        }
        return -1;
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    IPlugin::IPlugin( const string& name )
        : m_sName( name ){
      s_mPlugins.setBy( e_hashstr64( name ), [this]( IPlugin*& out ){
        out = this;
      });
    }

  //}:                                            |
  //Dtor:{                                        |

    IPlugin::~IPlugin(){
      s_mPlugins.erase( e_hashstr64( m_sName ));
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
