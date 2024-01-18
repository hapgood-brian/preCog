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
// ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF
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

//================================================+=============================
//IThread:{                                       |

  #if !e_compiling( web )

    namespace EON{

      namespace gfc{

        /** \brief Abstract threading interface.
          *
          * The IThread class is based on the Java Thread class. The child
          * class must implement the run function, start the thread, and let it
          * go.
          */

        struct E_PUBLISH IThread{

          /** \brief Thread run function.
            *
            * When the thread first boots up it does a little initialisation
            * and then calls this run function. Exiting run will terminate the
            * thread.
            *
            * \return The thread exit code.
            */

          virtual int run(){
            return 0;
          }

          /** \brief Define virtual destructor.
            *
            * This routine simply defines the virtual destructor.
            */

          virtual~IThread() = default;
        };
      }
    }

  #endif

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Thread:{                                        |

  #if !e_compiling( web )

    namespace EON{

      namespace gfc{

        /** \brief Thread class.
          *
          * This class defines the engine thread based loosely on the Java
          * class of the same name. Derive your own class, implement the run
          * function and you're good to go. Please note that this class does
          * not use the C++11 std::thread class at all because this code
          * predates std::thread by a couple of years. The Thread::Farm class
          * uses the gfc::Thread rather than STL threads however. I just like
          * the syntax rather than the harder to read std::thread case.
          \code
            struct myThread:Thread{
              int run(){
                while( alive() ){
                }
                return 0;
              }
            };
            int main(){
              Thread* pThread = new( true )Thread;
              pThread->acquire();
              //deleted by class.
              return 0;
            }
          \endcode
          */

        struct E_PUBLISH Thread:IThread{

          using Lambda = std::function<void()>;

          //======================================+=============================
          //Structs:{                             |

            /** \brief Thread farm class.
              *
              * This class defines the thread farm. You add jobs to it and they
              * will execute on N number of threads.
              *
              * \tparam T The type of object to pass to each thread in the farm
              * to be processed in the background.
              * \tparam N The number of threads to launch for this farm.
              */

            template<typename T,s32 N=4> struct Farm final{

              using Lambda = std::function<void( const T& t )>;
              using Queue  = deque<T>;

              //----------------------------------+-----------------------------
              //Operate:{                         |

                Farm& operator=( const Farm& ) = delete;
                Farm& operator=( Farm&&      ) = delete;

              //}:                                |
              //Methods:{                         |

                e_forceinline void addJob( const T& t ){
                  if( !m_atQueue.find( t )){
                    m_atQueue.push( t );
                  }
                }

                e_noinline void clear(){
                  for( u32 i=0; i<N; ++i ){
                    m_aThreads[ i ] = nullptr;
                  }
                }

                e_forceinline bool empty()const{
                  return m_atQueue.empty();
                }

                e_forceinline void join()const{
                  while( !empty() ){
                    Thread::yield();
                  }
                }

              //}:                                |
              //----------------------------------+-----------------------------

              Farm( const Farm& ) = delete;
              Farm( Farm&&      ) = delete;

              Farm( const Queue& queue, const Lambda& lambda );
              explicit Farm( const Lambda& lambda );

              virtual ~Farm(){
                clear();
              }

            private:

              /* Private member vars */

              std::unique_ptr<Thread> m_aThreads[ N ]{};
              Queue                   m_atQueue;
            };

          //}:                                    |
          //Operate:{                             |

            /** \brief Copy operator.
              *
              * This operator will copy the old thread into this one. If the
              * assignee is in a running started state it will be shutdown,
              * assigned the new thread payload and restarted. This is
              * incredibly dangerous because the run method of both the source
              * and this threads may not be the same so the behavior is a bit
              * wonky.  Really all this operator does is say "shut down the
              * thread and relaunch it with the source thread's data ). The
              * assignment may take some time because obviously this thread has
              * to be terminated first, then assigned to the new thread.
              *
              * \param thread A const reference to the source thread we're
              * copying.
              */

            e_noinline Thread& operator=( const Thread& thread ){
              if( this != &thread ){
                bool bHasStarted=( nullptr != m_pThread );
                if( bHasStarted ){
                  release();
                }
                m_pPayload = thread.m_pPayload;
                m_pThread  = nullptr;
                m_tFlags.all = 0;
                m_uTID = 0;
                if( bHasStarted ){
                  start();
                }
              }
              return *this;
            }

            /** \brief Move operator.
              *
              * This move operator will move all the data from the source
              * thread provided to this one. If this thread is started we must
              * shut down the current thread first before assigning the new
              * thread's members. This operation could take a significant
              * amount of time and may hang if the run() method isn't
              * frequently checking the bIsDead flag.
              *
              * \param thread The source thread to move into this one.
              */

            e_noinline Thread& operator=( Thread&& thread ){
              if( this != &thread ){
                if( m_tFlags->bStarted ){
                  release();
                }
                m_pThread = thread.m_pThread;
                thread.m_pThread = nullptr;
                m_pPayload = thread.m_pPayload;
                thread.m_pPayload = nullptr;
                m_uTID = thread.m_uTID;
                thread.m_uTID = 0;
                m_tFlags.all = thread.m_tFlags.all;
                thread.m_tFlags.all = 0;
              }
              return *this;
            }

          //}:                                    |
          //Statics:{                               |

            /** \brief Foreach function to iterate over active threads.
              *
              * This routine will iterate over all the active threads in the
              * system and call the lambda function provided for each thread.
              *
              * \param lambda Callback function to be called per iteration of
              * the foreach function.
              */

            static void foreach(
                const std::function<void( const IThread* )>& lambda );

            /** \brief Check if thread exists.
              *
              * This routine checks to see if the given thread ID exists.
              */

            static bool exists( const u32 tid );

            /** \brief Return number of active threads.
              *
              * This routine will return the number of active threads at the
              * time of the call.
              */

            static s32 count();

            /** \brief Return total number of threads.
              *
              * This routine will return the total number of threads.
              */

            static s32 peak();

            /** \brief Cause the thread to sleep ms milliseconds.
              *
              * This routine can be called from any thread and will cause this
              * thread object to go to sleep for ms milliseconds.
              *
              * \param ms The number of milliseconds to suspend thread
              * execution for.
              */

            static void sleep( const s32 ms );

            /** \brief Get thread object.
              *
              * This routine will return a new Thread object for the current
              * thread.  It will be a new Thread object not an instance of
              * another.
              *
              * \return Returns a Thread object that wraps the current thread.
              */

            static Thread& context();

            /** \brief Force thread to yield.
              *
              * This routine will cause the thread to yield.
              */

            static void yield();

            /** \brief Get current thread id.
              *
              * \return This routine returns the current thread identifier.
              */

            static u32 tid();

            /** \brief Get unique id.
              *
              * This routine returns a unique id for the thread. It is not the
              * same as the platform thread id returned tid().
              *
              * \return Returns a byte value representing the active thread
              * similarly to the tid() method.
              */

            static u32 uid();

          //}:                                    |
          //Methods:{                             |

            /** \brief Start the thread.
              *
              * This routine will start the thread running. Do not ever start a
              * thread in a constructor because you'll run the risk of calling
              * a pure virtual otherwise known as a CRASH!
              */

            void start();

            /** \brief Is thread current?
              *
              * This routine will return true if the thread is current or false
              * if it's not. Current means we're currently running in this
              * thread.
              *
              * \return Returns true if the current thread TID matches the TID
              * of the running thread we're in or false.
              */

            bool isCurrent()const;

            /** \brief Acquire the thread.
              *
              * This routine will block until the thread terminates.
              *
              * \return Returns true if the thread was acquired (joined pthread
              * speak or signaled in Win32 land) or false if we tried to
              * acquire the thread when isCurrent() returns true.
              */

            bool acquire();

            /** \brief Release the thread.
              *
              * If the thread is released it's considered the same as returning
              * from the run function.
              */

            void release();

            /** \brief Restart the thread.
              *
              * This routine will release the thread, causing it to die and
              * reset all data members to defaults, and then call start. As
              * such you have the option to pass in a stack size for the new
              * thread.
              */

            e_forceinline void restart(){
              release();
              start();
            }

            /** \brief Pause the thread.
              *
              * This routine will pause the thread.
              */

            e_forceinline void pause(){
              m_tFlags->bPaused = 1;
            }

            /** \brief Unpause the thread.
              *
              * This routine will unpause the thread.
              */

            e_forceinline void unpause(){
              m_tFlags->bPaused = 0;
            }

            /** \brief Check if thread is alive.
              *
              * This routine will check if the thread is alive and return a
              * boolean result. This method will grow in the future with event
              * handling and other functionality so it's good practice to
              * always call alive() in your thread loop rather than just
              * checking the flags.
              *
              * \return Returns true if the thread is alive and
              * m_tFlags->bIsDead is zero or false otherwise.
              */

            bool alive()const;

            /** \brief Cause thread to delete on exit.
              *
              * This routine will force the thread to delete itself when it
              * dies.
              */

            e_forceinline Thread* autodelete(){
              m_tFlags->bDeleteOnExit = 1;
              return this;
            }

          //}:                                    |
          //======================================+=============================

          /** \brief Lambda constructor.
           *
           * This constructor is the same as the default constructor below but
           * will execute the given lambda as the first thing it does before
           * calling the run() method. This solves a problem in the Win32 build
           * of the engine where we need to set the thread TID of the main
           * thread before we run it so IEngine::isMainThread() will work
           * properly.
           *
           * \param lambda The function to call prior to run.
           */

          explicit Thread( const Lambda& lambda );

          /** \brief Move constructor.
            *
            * This constructor will move one thread into this one.
            *
            * \param rvalue The Rvalue to move into this thread.
            */

          Thread( Thread&& rvalue );

          /** \brief Copy constructor.
            *
            * This constructor will copy the old thread to this one. The thread
            * will not be started after the copy operation so you'll need to do
            * that yourself. This is unlike the move constructor or operator
            * which keeps the thread in a started running state after the
            * operation.
            *
            * \param thread The source thread object to copy from.
            */

          Thread( const Thread& thread );

          /** \brief Default constructor.
            *
            * The default constructor initializes the thread object with a
            * native pthread or C++11 std::thread depending on the
            * implementation. It is guaranteed to start immediately.
            */

          Thread();

          /** \brief Thread destructor.
            *
            * This routine will call release().
            */

          virtual~Thread();

        private:

          static void RedWorker( Thread* );
          Thread( std::thread* pThread );

          e_var_ptr( void,             Payload ) = nullptr;
          e_var_ptr( std::thread,      Thread  ) = nullptr;
          e_var( Lambda, on,           Lambda  );
          e_var( u32, u,               TID     ) = 0;
          e_var( u32, u,               UID     ) = 0;
          e_var_private_volatile_bits( Flags
            , bDeleteOnExit:1
            , bStarted:1
            , bCurrent:1
            , bIsDying:1
            , bPaused:1
            , bIsDead:1
          );
        };
      }
    }

  #endif

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Inlines:{                                       |

  #if !e_compiling( web )

    namespace EON{

      namespace gfc{

        /* Thread farm constructors */

        template<typename T,s32 N> e_noinline Thread::Farm<T,N>::Farm( const Queue& queue, const Lambda& lambda )
            : m_atQueue( queue ){
          if( !lambda ){
            e_unreachable( "Null lambda given!" );
          }
          for( u32 i=0; i<N; ++i ){
            if( queue.empty() ){
              break;
            }
            ( m_aThreads[ i ] = std::make_unique<Thread>(
              [=](){
                while( context().alive() ){
                  if( !m_atQueue.popfBy( lambda )){
                    break;
                  }
                }
              }
            ))->start();
          }
        }

        template<typename T,s32 N> e_noinline Thread::Farm<T,N>::Farm( const Lambda& lambda ){
          if( !lambda ){
            e_unreachable( "Null lambda given!" );
          }
          for( u32 i=0; i<N; ++i ){
            ( m_aThreads[ i ] = std::make_unique<Thread>(
              [=](){
                while( context().alive() ){
                  if( !m_atQueue.popfBy( lambda )){
                    yield();
                  }
                }
              }
            ))->start();
          }
        }

        /* Special for async */

        /** \brief Asynchronous foreach.
          *
          * This routine will take a deque of objects and divvy them up between
          * N threads. The lambda function will be called with each individual
          * element on various threads. First come first served so it's
          * possible that some background threads may starve.
          *
          * \tparam T The type of object to be passed to the lambda.
          * \tparam N The number of background threads to spawn.
          * \param lambda The callback function to be executed.
          */

        template<typename T,s32 N> e_noinline void e_forAsync( deque<T>& atQueue, const std::function<void( const T& )>& lambda ){
          if( !lambda ){
            e_unreachable( "Null lambda given!" );
          }
          if( atQueue.empty() ){
            return;
          }
          std::unique_ptr<Thread> apThreads[ N ]{};
          for( u32 i=0; i<N; ++i ){
            if( atQueue.empty() ){
              break;
            }
            ( apThreads[ i ] = std::make_unique<Thread>(
              [&](){
                while( Thread::context().alive() ){
                  if( !atQueue.popfBy( lambda )){
                    break;
                  }
                }
              }
            ))->start();
          }
          for( u32 i=0; i<N; ++i ){
            if( apThreads[ i ] ){
              apThreads[ i ]->acquire();
              apThreads[ i ] = nullptr;
            }
          }
        }

        /** \brief Asynchronous foreach.
          *
          * This routine will take a vector of objects and divvy them up
          * between N threads. The lambda function will be called with each
          * individual element on various threads. First come first served so
          * it's possible that some background threads may starve.
          *
          * \tparam T The type of object to be passed to the lambda.
          * \tparam N The number of background threads to spawn.
          * \param lambda The callback function to be executed.
          */

        template<typename T,s32 N> e_noinline void e_forAsync( const vector<T>& atVector, const std::function<void( const T& )>& lambda ){
          if( !lambda ){
            e_unreachable( "Null lambda given!" );
          }
          if( atVector.empty() ){
            return;
          }
          std::unique_ptr<Thread> apThreads[ N ]{};
          for( u32 i=0; i<N; ++i ){
            std::atomic<u32> ix{};
            ( apThreads[ i ] = std::make_unique<Thread>(
              [&](){
                while( Thread::context().alive() ){
                  const u32 j = ++ix;
                  if( j <= atVector.size() ){
                    atVector.query( j-1, lambda );
                    continue;
                  }
                  break;
                }
              }
            ))->start();
          }
          for( u32 i=0; i<N; ++i ){
            if( apThreads[ i ] ){
              apThreads[ i ]->acquire();
              apThreads[ i ] = nullptr;
            }
          }
        }

        /** \brief Asynchronous foreach.
          *
          * This routine will take a vector of objects and divvy them up
          * between N threads. The lambda function will be called with each
          * individual element on various threads. First come first served so
          * it's possible that some background threads may starve.
          *
          * \tparam T The type of object to be passed to the lambda.
          * \tparam N The number of background threads to spawn.
          * \param lambda The callback function to be executed.
          */

        template<typename T,s32 N> e_noinline void e_forAsync( vector<T>& atVector, const std::function<void( T& )>& lambda ){
          if( !lambda ){
            e_unreachable( "Null lambda given!" );
          }
          if( atVector.empty() ){
            return;
          }
          std::unique_ptr<Thread> apThreads[ N ]{};
          for( u32 i=0; i<N; ++i ){
            std::atomic<u32> ix{};
            ( apThreads[ i ] = std::make_unique<Thread>(
              [&](){
                while( Thread::context().alive() ){
                  const u32 j = ++ix;
                  if( j <= atVector.size() ){
                    atVector.alter( j-1, lambda );
                    continue;
                  }
                  break;
                }
              }
            ))->start();
          }
          for( u32 i=0; i<N; ++i ){
            if( apThreads[ i ] ){
              apThreads[ i ]->acquire();
              apThreads[ i ] = nullptr;
            }
          }
        }

        /** \brief Special run async
          *
          * This routine will launch a thread an execute the given lamdba in
          * the background. There is some overhead with launching the thread so
          * it's not ideal to be spawning tons of little tasks. Use the
          * e_forAsync() function instead.
          */

        e_forceinline void e_runAsync( const std::function<void()>& lambda ){
          ( new Thread( lambda ))->autodelete()->start();
        }
      }
    }

  #endif

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
