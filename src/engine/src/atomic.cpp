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

#if !e_compiling( microsoft )
  #include<execinfo.h>
#endif
#include<atomic>

using namespace EON;
using namespace gfc;
using namespace atomic;

//================================================|=============================
//atomic:{                                        |
  //Defines:{                                     |

    #if e_compiling( osx )
      #define USE_SPINLOCK 0
      #define USE_LOGGING  0
      #define USE_YIELDS   1
    #elif e_compiling( microsoft )
      #define USE_SPINLOCK 1
      #define USE_LOGGING  0
      #define USE_YIELDS   4
    #endif

  //}:                                            |
  //Private:{                                     |

    #if USE_LOGGING
      namespace{
        __thread char msgBuffer[ 1024 ]{};
      }
    #endif

  //}:                                            |
  //Structs:{                                     |
    //ShareLockRecursive:{                        |
      //Methods:{                                 |
        //tryAcquireRead:{                        |

          bool ShareLockRecursive::tryAcquireRead(){
            decltype( m_tLock )::LockGuard lockGuard( m_tLock );
            // If current thread has already acquired the share lock then we're
            // allowed to increment the reader count.
            const auto tid = Thread::tid();
            bool bResult = m_atomicReads.query( tid, nullptr/* just query */);
            // If there are any write locks belonging to a different thread we
            // cannot acquire the read lock.
            if( !bResult ){
              // If the writer thread id is non-zero and not equal to the
              // current thread id we'll return false because we cannot
              // obtain a write lock; another thread obviously owns it.
              const auto oth = m_atomicWrites.tid();
              const auto tid = Thread::tid();
              if( oth && tid != oth ){
                // It's possible for oth to be invalid. Or in other words the
                // thread id 'oth' is not a living thread anymore. Test here.
                return false;
              }
            }
            // Ok, no write lock on another thread so acquire for reading.
            m_atomicReads.alter( tid,
              []( u32& out ){
                ++out;
              }
            );
            #if USE_LOGGING
              #if e_compiling( microsoft )
                sprintf_s( msgBuffer
                 , sizeof( msgBuffer )
              #else
                sprintf( msgBuffer
              #endif
                , "[%u (%p)] tryAcquireRead: reads=%u writes=%u (%u)"
                , tid
                , this
                , m_atomicReads.size()
                , m_atomicWrites.ownership()
                , m_atomicWrites.tid() );
              e_log( msgBuffer );
            #endif
            return true;
          }

        //}:                                      |
        //releaseRead:{                           |

          void ShareLockRecursive::releaseRead(){
            const auto tid = Thread::tid();
            m_atomicReads.condErase( tid,
              []( u32& refs ){
                if( refs == 1 ){// yes, erase.
                  return true;
                }
                --refs;// no, decrement.
                return false;
              }
            );
            #if USE_LOGGING
              #if e_compiling( microsoft )
                sprintf_s( msgBuffer
                 , sizeof( msgBuffer )
              #else
                sprintf( msgBuffer
              #endif
                , "[%u (%p)] releaseRead: reads=%u writes=%u (%u)"
                , tid
                , this
                , m_atomicReads.size()
                , m_atomicWrites.ownership()
                , m_atomicWrites.tid() );
              e_log( msgBuffer );
            #endif
          }

        //}:                                      |
        //acquireRead:{                           |

          void ShareLockRecursive::acquireRead(){
            for( u32 i=0; !tryAcquireRead(); ++i ){
              #if USE_YIELDS
                if( !( i % USE_YIELDS )){
                  Thread::yield();
                }
              #endif
            }
          }

        //}:                                      |
        //tryAcquire:{                            |

          bool ShareLockRecursive::tryAcquire(){
            const auto tid = Thread::tid();
            #if 0 // experimental attempt!
              u32 counts = 0;
              u32 theirs = 0;
              m_atomicReads.foreachKV(
                [&]( const u32 key, const u32& value ){
                  if( key == tid ){
                    counts += value;
                  }else{
                    theirs += value;
                  }
                }
              );
              bool bResult = false;
              if( !theirs ){
                bResult = m_atomicWrites.tryAcquire();
              }
              return bResult;
            #else
              if( !m_tLock.tryAcquire() ){
                return false;
              }
              // Count up the number of keys sharing this thread id and if that
              // count is nil return false again.
              const bool bEmpty = m_atomicReads.empty();
              if( !bEmpty ){
                u32 count = 0;
                m_atomicReads.foreachKV(
                  [&]( const u32 key, const u32& value ){
                    if( key == tid ){
                      count += value;
                    }
                  }
                );
                if( !count ){
                  m_tLock.release();
                  return false;
                }
              }
              // If the writer thread id is non-zero and not equal to the
              // current thread id we'll return false because we cannot
              // obtain a write lock; another thread obviously owns it.
              bool bResult = true;
              const auto oth = m_atomicWrites.tid();
              if( oth && oth != tid ){
                // It's possible for oth to be invalid. Or in other words the
                // thread id 'oth' is not a living thread anymore. Test here.
                bResult = false;
              }else{
                // If the current thread matches then we can increment the write
                // count and be on our way.
                m_atomicWrites.tryAcquire();
                bResult = true;
              }
              #if USE_LOGGING
                #if e_compiling( microsoft )
                  sprintf_s( msgBuffer
                   , sizeof( msgBuffer )
                #else
                  sprintf( msgBuffer
                #endif
                  , "[%u (%p)] tryAcquire: reads=%u writes=%u (%u)"
                  , tid
                  , this
                  , m_atomicReads.size()
                  , m_atomicWrites.ownership()
                  , m_atomicWrites.tid() );
                e_log( msgBuffer );
              #endif
              m_tLock.release();
              return bResult;
            #endif
          }

        //}:                                      |
        //acquire:{                               |

          void ShareLockRecursive::acquire(){
            for( u32 i=0; !tryAcquire(); ++i ){
              #if USE_YIELDS
                if( !( i % USE_YIELDS )){
                  Thread::yield();
                }
              #endif
            }
          }

        //}:                                      |
        //release:{                               |

          void ShareLockRecursive::release(){
            m_atomicWrites.release();
            #if USE_LOGGING
              const auto tid = Thread::tid();
              #if e_compiling( microsoft )
                sprintf_s( msgBuffer
                 , sizeof( msgBuffer )
              #else
                sprintf( msgBuffer
              #endif
                , "[%u (%p)] release: reads=%u writes=%u (%u)"
                , tid
                , this
                , m_atomicReads.size()
                , m_atomicWrites.ownership()
                , m_atomicWrites.tid() );
              e_log( msgBuffer );
            #endif
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
    //SpinlockRecursive:{                         |
      //Methods:{                                 |
        //tryAcquire:{                            |

          bool SpinlockRecursive::tryAcquire(){
            const auto tid = Thread::tid();
            auto lck = m_uLockTID.load();
            if( !m_uLockTID.compare_exchange_weak( lck, tid, std::memory_order_acquire )){
              return false;
            }
            ++m_uWrites;
            return true;
          }

        //}:                                      |
        //acquire:{                               |

          void SpinlockRecursive::acquire(){
            for( u32 i=0; !tryAcquire(); ++i ){
              #if USE_YIELDS
                if( !( i % USE_YIELDS )){
                  Thread::yield();
                }
              #endif
            }
          }

        //}:                                      |
        //release:{                               |

          void SpinlockRecursive::release(){
            if( --m_uWrites < 1 ){
              m_uLockTID.store( 0, std::memory_order_release );
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
    //Spinlock:{                                  |
      //Methods:{                                 |
        //tryAcquire:{                            |

          bool Spinlock::tryAcquire(){
            u32 uExpected{};
            const auto tid = Thread::tid();
            return( m_tLock.compare_exchange_weak(
                uExpected
              , tid
              , std::memory_order_acquire )
            );
          }

        //}:                                      |
        //acquire:{                               |

          void Spinlock::acquire(){
            for( u32 i=0; !tryAcquire(); ++i ){
              #if USE_YIELDS
                if( !( i % USE_YIELDS )){
                  Thread::yield();
                }
              #endif
            }
          }

        //}:                                      |
        //release:{                               |

          void Spinlock::release(){
            m_tLock.store( 0, std::memory_order_release );
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
