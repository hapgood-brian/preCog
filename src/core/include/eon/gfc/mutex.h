//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup multicore
  *     @{
  */

//================================================+=============================
//Mutex:{                                         |

namespace EON{

  namespace gfc{

    /** Thread mutex object.
      *
      * This class wraps mutex functionality on Windows and the *nixes including
      * iOS, OSX, Linux and Android.
      */

    struct E_PUBLISH Mutex final{

      //------------------------------------------+-----------------------------
      //Structs:{                                 |
        //LockGuard:{                             |

          struct LockGuard{

            LockGuard( Mutex& m )
                : m_atomicMutex( m ){
              m.acquire();
            }

          ~ LockGuard(){
              m_atomicMutex.release();
            }

          private:

            Mutex& m_atomicMutex;
          };

        //}:                                      |
      //}:                                        |
      //Methods:{                                 |

        /** \brief Acquire the mutex.
          *
          * Acquires ownership of the mutex. If another thread has ownership
          * the thread will block until it calls release.
          */

        bool acquire();

        /** \brief Release ownership.
          *
          * This routine will release ownership of the mutex and put it up for
          * grabs by another thread. This is the point where synchronization
          * can happen.
          */

        void release();

      //}:                                        |
      //------------------------------------------+-----------------------------

      /** \brief Create a new mutex object.
        *
        * Signals are a way of suspending thread execution until an event of some
        * kind occurs.
        */

      Mutex();

      /** \brief Retire the mutex from service.
        *
        * This routine is the antithesis of create and will destroy the mutex.
        */

    ~ Mutex();

    private:

      vp m_pNative = nullptr;
    };
  }
}

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
