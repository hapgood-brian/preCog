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

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup multicore
  *     @{
  */

//================================================|=============================
//Mutex:{                                         |

namespace EON{

  namespace gfc{

    /** Thread mutex object.
      *
      * This class wraps mutex functionality on Windows and the *nixes including
      * iOS, OSX, Linux and Android.
      */

    struct E_PUBLISH Mutex final{

      //------------------------------------------|-----------------------------
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
      //------------------------------------------|-----------------------------

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
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
