//------------------------------------------------------------------------------
//           Copyright 2014-2018 Creepy Doll Games. All rights reserved.
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

#include"glue.h"
#include<pthread.h>

using namespace gfc;

static string       s_sPackagePath;
static Thread       s_tMainThread;
static string       s_sPackageDir;
static android_app* s_pState = 0;

//================================================|=============================
//AppGlue:{                                       |
  //Methods:{                                     |
    //main:{                                      |

      void AppGlue::main(){
        e_log( "AppGlue::main", "Setting up view" );
        m_pState->userData = &m_tView;
        m_pState->onAppCmd = View::onAppCmd;
        m_pState->onInputEvent = View::onInputEvent;
        m_tView.setApp( m_pState );
        m_tView.setSensorManager( ASensorManager_getInstance() );
        m_tView.setAccelerometerSensor(
            (ASensor*)ASensorManager_getDefaultSensor( m_tView.toSensorManager(),
            ASENSOR_TYPE_ACCELEROMETER ));
        m_tView.setSensorEventQueue(
            ASensorManager_createEventQueue(
                m_tView.toSensorManager(),
                m_pState->looper,
                LOOPER_ID_USER,
                NULL,
                NULL ));
        for(;;){
          int ident;
          int events;
          android_poll_source* pSource;
          while(( ident=ALooper_pollAll( 0, NULL, &events, (void**)&pSource)) >= 0 ){
            if( pSource ){
              pSource->process( m_pState, pSource );
            }
            if( ident == LOOPER_ID_USER ){
              if( m_tView.toAccelerometerSensor() ){
                ASensorEvent event;
                while( ASensorEventQueue_getEvents( m_tView.toSensorEventQueue(), &event, 1) > 0 ){
                  listen<IController>::trigger(
                      &IController::onTilt,
                      event.acceleration.x,
                      event.acceleration.y,
                      event.acceleration.z );
                }
              }
            }
            if( m_pState->destroyRequested != 0 ){
              e_log( "EON", "Destroy requested by OS" );
              IEngine::ri.purge();
              IEngine::exit();
              return;
            }
          }

          if( !m_tView.toDisplay() ){
            continue;
          }

          // Just fill the screen with a color.
          IEngine::tick();
          IEngine::draw();
          eglSwapBuffers( m_tView.toDisplay(), m_tView.toSurface() );
          e_sreset();
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
  //[paths]:{                                     |
    //setPackagePath:{                            |

      void IEngine::setPackagePath( const gfc::string& dir ){
        if( dir.empty() ){
          s_sPackagePath.clear();
        }else{
          s_sPackagePath = dir;
          while( s_sPackagePath.right( 1 )=="/" ){
            s_sPackagePath.trim( 1 );
          }
        }
      }

    //}:                                          |
    //toPackagePath:{                             |

      gfc::string IEngine::toPackagePath(){
        return s_sPackageDir;
      }

    //}:                                          |
    //toResourcePath:{                            |

      gfc::string IEngine::toResourcePath(){
        return s_sPackageDir;
      }

    //}:                                          |
    //toBundlePath:{                              |

      gfc::string IEngine::toBundlePath(){
        return "";
      }

    //}:                                          |
    //toRedPath:{                                 |

      string IEngine::toRedPath(){
        string redPath;
        if( !s_sPackagePath.empty() ){
          if(( s_sPackagePath[0] == '.' )&&( s_sPackagePath[1] == '/' )){
            redPath.catf( "%s/%s/.android/", toResourcePath().c_str(), s_sPackagePath.c_str()+2 );
          }else{
            redPath = s_sPackagePath+"/.android/";
          }
        }else{
          redPath = toResourcePath();
        }
        return redPath;
      }

    //}:                                          |
  //}:                                            |
  //toNativeData:{                                |

    cvp IEngine::toNativeData(){
      return s_pState;
    }

  //}:                                            |
  //isMainThread:{                                |

    bool IEngine::isMainThread(){
      return s_tMainThread.isCurrent();
    }

  //}:                                            |
  //tempPath:{                                    |

    gfc::string IEngine::tempPath(){
      //there is no temp directory on android.
      return 0;
    }

  //}:                                            |
  //fopen:{                                       |

    static int android_read( void* cookie, char* buf, int size ){
      return AAsset_read( (AAsset*)cookie, buf, size );
    }

    static int android_write( void*, const char*, int ){
      return -1;
    }

    static fpos_t android_seek( void* cookie, fpos_t offset, int whence ){
      return AAsset_seek( (AAsset*) cookie, offset, whence );
    }

    static int android_close( void* cookie ){
      AAsset_close( (AAsset*)cookie );
      return 0;
    }

    FILE* IEngine::fopen( const string& path, ccp mode ){
      AAsset* a = AAssetManager_open( s_pState->activity->assetManager, path, 0 );
      if( !a ){
        e_logf( "EON", "Failed to open %s", path.c_str() );
        return 0;
      }
      return funopen( a,
          android_read,
          android_write,
          android_seek,
          android_close );
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
