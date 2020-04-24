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

#if e_compiling( microsoft )
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include<windows.h>
#else
  #include<pthread.h>
  #include<sched.h>
#endif

using namespace EON;
using namespace gfc;

//================================================|=============================
//Mutex:{                                         |
  //acquire:{                                     |

    bool Mutex::acquire(){
      #if e_compiling( microsoft )
        LPCRITICAL_SECTION pcs = reinterpret_cast<LPCRITICAL_SECTION>( m_pNative );
        EnterCriticalSection( pcs );
      #else
        pthread_mutex_t* pMutex = reinterpret_cast<pthread_mutex_t*>( m_pNative );
        pthread_mutex_lock( pMutex );
      #endif
      return true;
    }

  //}:                                            |
  //release:{                                     |

    void Mutex::release(){
      #if e_compiling( microsoft )
        LPCRITICAL_SECTION pcs = reinterpret_cast<LPCRITICAL_SECTION>( m_pNative );
        LeaveCriticalSection( pcs );
      #else
        pthread_mutex_t* pMutex = reinterpret_cast<pthread_mutex_t*>( m_pNative );
        pthread_mutex_unlock( pMutex );
      #endif
    }

  //}:                                            |
  //[ctor]:{                                      |

    Mutex::Mutex(){
      #if e_compiling( microsoft )
        LPCRITICAL_SECTION pcs = new CRITICAL_SECTION;
        ZeroMemory( pcs, sizeof( CRITICAL_SECTION ));
        InitializeCriticalSection( pcs );
        m_pNative = pcs;
      #else
        pthread_mutex_t* pMutex = new pthread_mutex_t;
        pthread_mutex_init( pMutex, NULL );
        m_pNative = pMutex;
      #endif
    }

  //}:                                            |
  //[dtor]:{                                      |

    Mutex::~Mutex(){
      #if e_compiling( microsoft )
        LPCRITICAL_SECTION pcs = reinterpret_cast<LPCRITICAL_SECTION>( m_pNative );
        e_assert( pcs, "Native pointer is nullptr!" );
        DeleteCriticalSection( pcs );
      #else
        pthread_mutex_t* pMutex = reinterpret_cast<pthread_mutex_t*>( m_pNative );
        pthread_mutex_destroy( pMutex );
        delete pMutex;
      #endif
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
