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

#include<shared_mutex>
#include<atomic>

//================================================|=============================
//Spinlock:{                                      |

#ifdef __APPLE__
  #pragma mark - spinlock -
#endif

  namespace EON{

    namespace gfc{

      namespace atomic{

        struct E_PUBLISH Spinlock final{

          //--------------------------------------|-----------------------------
          //Structs:{                             |
            //LockGuard:{                         |

              struct LockGuard{

                LockGuard( Spinlock& m )
                    : m_atomicSpinlock( m ){
                  m.acquire();
                }

              ~ LockGuard(){
                  m_atomicSpinlock.release();
                }

              private:

                Spinlock& m_atomicSpinlock;
              };

            //}:                                  |
          //}:                                    |
          //Methods:{                             |

            /** \brief Try to acquire the spinlock (non-recursive).
              *
              * This routine will acquire the spinlock.
              */

            bool tryAcquire();

            /** \brief Acquire the spinlock (non-recursive).
              *
              * This routine will acquire the spinlock.
              */

            void acquire();

            /** \brief Release the spinlock.
              *
              * This routine releases the acquired spinlock.
              */

            void release();

          //}:                                    |
          //--------------------------------------|-----------------------------

          Spinlock() = default;
        ~ Spinlock() = default;

        private:

          #if !e_compiling( web )
            std::atomic<u32> m_tLock{};
          #endif
        };
      }
    }
  }

//}:                                              |
//SpinlockRecursive:{                             |

#ifdef __APPLE__
  #pragma mark - recursive spinlock -
#endif

  namespace EON{

    namespace gfc{

      namespace atomic{

        struct E_PUBLISH SpinlockRecursive final{

          //--------------------------------------|-----------------------------
          //Structs:{                             |
            //LockGuard:{                         |

              struct LockGuard{

                e_forceinline LockGuard( SpinlockRecursive& m )
                    : m_atomicSpinlock( m ){
                  m.acquire();
                }

                e_forceinline~LockGuard(){
                  m_atomicSpinlock.release();
                }

              private:

                SpinlockRecursive& m_atomicSpinlock;
              };

            //}:                                  |
          //}:                                    |
          //Methods:{                             |

            /** \brief Try to acquire the spinlock (non-recursive).
              *
              * This routine will acquire the spinlock.
              */

            bool tryAcquire();

            /** \brief Acquire the spinlock (non-recursive).
              *
              * This routine will acquire the spinlock.
              */

            void acquire();

            /** \brief Release the spinlock.
              *
              * This routine releases the acquired spinlock.
              */

            void release();

            /** \brief Get the thread id.
              *
              * This routine returns the thread id for this lock.
              */

            e_forceinline u32 tid()const{
              return m_uLockTID.load();
            }

            /** \brief Get ownership count.
              *
              * This routine returns the ownership count.
              */

            e_forceinline s32 ownership()const{
              return m_uWrites.load();
            }

          //}:                                    |
          //--------------------------------------|-----------------------------

          SpinlockRecursive() = default;
        ~ SpinlockRecursive() = default;

        private:

          std::atomic<u32> m_uLockTID{};
          std::atomic<s32> m_uWrites{};
          Spinlock         m_tLock;
        };
      }
    }
  }

//}:                                              |
//SpinlockPool:{                                  |

#ifdef __APPLE__
  #pragma mark - table -
#endif

  namespace EON{

    namespace gfc{

      namespace atomic{

        template<typename K,typename T,int N=33,u32 PRIME=17> struct E_PUBLISH SpinlockPool final{

          //--------------------------------------|-----------------------------
          //Structs:{                             |

            struct Node final{
              u8  m_aData[ sizeof( T )];
              u32 m_uNext;
              K   m_uKey;
            };

          //}:                                    |
          //Methods:{                             |
            //foreach*:{                          |

              /** \brief For each key-value method.
                *
                * This routine will walk over all the keys in the table,
                * passing their values to the lambda provided. This version of
                * the method returns a boolean from the lambda. If that value
                * is false then it will break out of the loop and return true
                * from foreachsKV.
                *
                * \param lambda The callback to pass each key value pair to.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreachsKV( const std::function<bool( const K, T& )>& lambda ){
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    auto& node = m_aList[ next-1 ];
                    if( !lambda( node.m_uKey, *reinterpret_cast<T*>( node.m_aData ))){
                      return true;
                    }
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each key-value method.
                *
                * This routine will walk over all the keys in the table,
                * passing their values to the lambda provided. This version of
                * the method returns a boolean from the lambda. If that value
                * is false then it will break out of the loop and return true
                * from foreachsKV.
                *
                * \param lambda The callback to pass each key value pair to.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreachsKV( const std::function<bool( const K, const T& )>& lambda )const{
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    const auto& node = m_aList[ next-1 ];
                    if( !lambda( node.m_uKey, *reinterpret_cast<const T*>( node.m_aData ))){
                      return true;
                    }
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each key-value method.
                *
                * This routine will walk over all the keys in the table,
                * passing their values to the lambda provided.
                *
                * \param lambda The callback to pass each key value pair to.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreachKV( const std::function<void( const K, const T& )>& lambda )const{
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    const auto& node = m_aList[ next-1 ];
                    lambda( node.m_uKey, *reinterpret_cast<const T*>( node.m_aData ));
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each key-value method.
                *
                * This routine will walk over all the keys in the table,
                * passing their values to the lambda provided.
                *
                * \param lambda The callback to pass each key value pair to.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreachKV( const std::function<void( const K, T& )>& lambda ){
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    auto& node = m_aList[ next-1 ];
                    lambda( node.m_uKey, *reinterpret_cast<T*>( node.m_aData ));
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each value in the table.
                *
                * This routine will walk over all the keys in the table and
                * pass the value of them to the lambda provided.
                *
                * \param lambda The callback that receives the keyed value.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreachs( const std::function<bool( const T& )>& lambda )const{
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    const auto& node = m_aList[ next-1 ];
                    if( !lambda( *reinterpret_cast<const T*>( node.m_aData ))){
                      break;
                    }
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each value in the table.
                *
                * This routine will walk over all the keys in the table and
                * pass the value of them to the lambda provided.
                *
                * \param lambda The callback that receives the keyed value.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreach( const std::function<void( const T& )>& lambda )const{
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    const auto& node = m_aList[ next-1 ];
                    lambda( *reinterpret_cast<const T*>( node.m_aData ));
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each value in the table.
                *
                * This routine will walk over all the keys in the table and
                * pass the value of them to the lambda provided.
                *
                * \param lambda The callback that receives the keyed value.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreachs( const std::function<bool( T& )>& lambda ){
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    auto& node = m_aList[ next-1 ];
                    if( !lambda( *reinterpret_cast<T*>( node.m_aData ))){
                      break;
                    }
                    next = node.m_uNext;
                  }
                }
                return true;
              }

              /** \brief For each value in the table.
                *
                * This routine will walk over all the keys in the table and
                * pass the value of them to the lambda provided.
                *
                * \param lambda The callback that receives the keyed value.
                *
                * \return Returns false if the lambda is nullptr.
                */

              e_noinline bool foreach( const std::function<void( T& )>& lambda ){
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                for( u32 i=0; i<PRIME; ++i ){
                  auto next = m_pUsed[ i ];
                  while( next ){
                    auto& node = m_aList[ next-1 ];
                    lambda( *reinterpret_cast<T*>( node.m_aData ));
                    next = node.m_uNext;
                  }
                }
                return true;
              }

            //}:                                  |
            //erase*:{                            |

              /** \brief Conditional erase.
                *
                * This routine will erase the key in the table if the lambda
                * tells it too by returning true.
                *
                * \param key The key to erase.
                *
                * \param lambda The callback.
                *
                * \return Returns true if the key was erased and false
                * otherwise.
                */

              e_noinline bool condErase( const K key, const std::function<bool( T& )>& lambda ){
                if( !lambda ){
                  return false;
                }
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                if( !m_uSize ){
                  return false;
                }
                const auto ix = key % PRIME;
                u32& head = m_pUsed[ ix ];
                u32 next = head;
                u32 last = 0;
                while( next ){
                  auto& node = m_aList[ next-1 ];
                  if( node.m_uKey != key ){
                    last = next;
                    next = node.m_uNext;
                    continue;
                  }
                  if( lambda( *reinterpret_cast<T*>( node.m_aData ))){
                    if( last ){
                      auto& prev = m_aList[ last-1 ];
                      prev.m_uNext = node.m_uNext;
                    }else{
                      head = node.m_uNext;
                    }
                    reinterpret_cast<T*>( node.m_aData )->~T();
                    memset( node.m_aData, 0
                      , sizeof( node.m_aData ));
                    node.m_uNext = m_uFree;
                    node.m_uKey = 0;
                    m_uFree = next;
                    m_uSize--;
                    return true;
                  }
                }
                return false;
              }

              /** \brief Erase key.
                *
                * This routine will erase the node at the given key.
                *
                * \param key The key to erase.
                *
                * \return Returns true if the key was erased and false
                * otherwise.
                */

              e_noinline bool erase( const K key ){
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                const K ix = u32( key ) % PRIME;
                auto& head = m_pUsed[ ix ];
                u32 next = head;
                u32 prev = 0;
                while( next ){
                  auto& node = m_aList[ next-1 ];
                  if( node.m_uKey != key ){
                    prev = next;
                    next = node.m_uNext;
                    continue;
                  }
                  if( prev ){
                    auto& last = m_aList[ prev-1 ];
                    last.m_uNext = node.m_uNext;
                  }else{
                    head = node.m_uNext;
                  }
                  reinterpret_cast<T*>( node.m_aData )->~T();
                  memset( node.m_aData, 0
                    , sizeof( node.m_aData ));
                  node.m_uNext = m_uFree;
                  node.m_uKey = 0;
                  m_uFree = next;
                  m_uSize--;
                  return true;
                }
                return false;
              }

            //}:                                  |
            //query:{                             |

              /** \brief Try query key value.
                *
                * This routine will query the key in a thread safe manner and
                * pass it's value to the lambda provided.
                *
                * \param key The key of the value we want to query.
                *
                * \param lambda The callback that receives the value if it
                * exists (non-nullptr).
                *
                * \return Returns false if the key did not exist, and true
                * otherwise.
                */

              e_noinline bool tryQuery( const K key, const std::function<void( const T& )>& lambda )const{
                if( !m_tLock.tryAcquire() ){
                  return false;
                }
                const auto ix = u32( key ) % PRIME;
                u32 next = m_pUsed[ ix ];
                bool bResult = false;
                while( next ){
                  const auto& node = m_aList[ next-1 ];
                  if( node.m_uKey == key ){
                    if( lambda ){
                      lambda( *reinterpret_cast<const T*>( node.m_aData ));
                    }
                    bResult = true;
                    break;
                  }
                  next = node.m_uNext;
                }
                m_tLock.release();
                return bResult;
              }

              /** \brief Query key value.
                *
                * This routine will query the key in a thread safe manner and
                * pass it's value to the lambda provided.
                *
                * \param key The key of the value we want to query.
                *
                * \param lambda The callback that receives the value if it
                * exists.
                *
                * \return Returns false if the lambda was nullptr or the key
                * did not exist, and true otherwise.
                */

              e_noinline bool query( const K key, const std::function<void( const T& )>& lambda )const{
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                const auto ix = u32( key ) % PRIME;
                u32 next = m_pUsed[ ix ];
                bool bResult = false;
                while( next ){
                  const auto& node = m_aList[ next-1 ];
                  if( node.m_uKey == key ){
                    if( lambda ){
                      lambda( *reinterpret_cast<const T*>( node.m_aData ));
                    }
                    bResult = true;
                    break;
                  }
                  next = node.m_uNext;
                }
                return bResult;
              }

            //}:                                  |
            //count:{                             |

              /** \brief Counts the number of occurrences of t in the table.
                *
                * This routine will walk the entire tree looking for the given
                * value, and if found returns how many instances of that value
                * exist.
                *
                * \param t The value to look for.
                *
                * \return Returns the number of instances of t that exist in
                * the table object.
                */

              e_noinline u32 count( const T& t )const{
                u32 uCount = 0;
                foreach(
                  [&]( const T& tt ){
                    if( tt == t ){
                      ++uCount;
                    }
                  }
                );
                return uCount;
              }

            //}:                                  |
            //empty:{                             |

              /** \brief This routine return whether the container is empty.
                *
                * This routine will return true if there are no elements
                * allocated in this container or false.
                *
                * \return Returns true if there are no elements in the table
                * and false otherwise.
                */

              e_forceinline bool empty()const{
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                return !m_uSize;
              }

            //}:                                  |
            //alter:{                             |

              /** \brief Alter an element in the table by a new or existing key.
                *
                * This routine will alter an element in the table by it's key.
                * If the key is unknown a new element will be allocated for it.
                *
                * \param key The key whose value we wish to alter. If key
                * doesn't exist a new value will be allocated.
                *
                * \param lambda The callback function that may modify the value
                * in a thread safe way. The lambda must be very brief, short
                * sequences of code only, because we acquire a spinlock in the
                * table not a mutex.
                *
                * \return Returns false if the lambda was nullptr or there were
                * no more elements in the pool. In this case empty() will also
                * return true.
                */

              e_noinline bool alter( const K key, const std::function<void( T& )>& lambda ){
                if( !lambda ){
                  return false;
                }
                // Acquire the mutex or critical section.
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                // Walk to the end of the list searching.
                const auto ix = u32( key ) % PRIME;
                auto& head = m_pUsed[ ix ];
                auto next = head;
                while( next ){
                  auto& node = m_aList[ next-1 ];
                  if( node.m_uKey != key ){
                    next = node.m_uNext;
                    continue;
                  }
                  lambda( *reinterpret_cast<T*>( node.m_aData ));
                  return true;
                }
                // Calculate the root index into table. If no head just attach in
                // place.
                if( !m_uFree ){
                  return false;
                }
                // Pop next item of the free list.
                auto _free = m_uFree;
                auto& free = m_uFree;
                auto& node = m_aList[ free-1 ];
                free = node.m_uNext;
                // Push node onto used list.
                node.m_uNext = head;
                node.m_uKey = key;
                head = _free;
                // Allocate new node data.
                new( node.m_aData )T{};
                ++m_uSize;
                // Execute lambda.
                lambda( *reinterpret_cast<T*>( node.m_aData ));
                return true;
              }

            //}:                                  |
            //size:{                              |

              /** \brief Return the number of elements in or the size of, the
                * table.
                *
                * This routine return the size of the table in elements.
                *
                * \return Returns zero if the table is empty or the number of
                * elements.
                */

              u32 size()const{
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                return m_uSize;
              }

            //}:                                  |
            //lock:{                              |

              /** \brief Return spinlock.
                *
                * This routine returns the spinlock.
                */

              Spinlock& lock(){
                return m_tLock;
              }

            //}:                                  |
            //set:{                               |

              /** \brief Set a value by key and variadic template arguments.
                *
                * This routine will construct a new value for the given key and
                * passes the variadic arguments to the constructor of the new
                * value. Only one key is allowed but the same value can appear
                * many times if necessary.
                *
                * \param key The key to allocate the new value for.
                *
                * \param args The arguments to be forwarded to the value
                * constructor or type T.
                */

              template<typename...A> e_noinline bool set( const K key, A...args ){
                // Acquire the mutex or critical section.
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                // Walk to the end of the list searching.
                const auto ix = u32( key ) % PRIME;
                auto& head = m_pUsed[ ix ];
                auto next = head;
                while( next ){
                  auto& node = m_aList[ next-1 ];
                  if( node.m_uKey != key ){
                    next = node.m_uNext;
                    continue;
                  }
                  return false;
                }
                // Calculate the root index into table. If no head just attach in
                // place.
                if( !m_uFree ){
                  return false;
                }
                // Pop next item of the free list.
                auto _free = m_uFree;
                auto& free = m_uFree;
                auto& node = m_aList[ free-1 ];
                free = node.m_uNext;
                // Push node onto used list.
                node.m_uNext = head;
                node.m_uKey = key;
                head = _free;
                // Allocate new node data.
                new( node.m_aData )T( std::forward<A>( args )... );
                ++m_uSize;
                return true;
              }

              /** \brief This routine sets a copy on the table.
                *
                * This routine will set a new value by it's key using a copy of
                * the data provided.
                *
                * \param key The key to set on the table.
                *
                * \param data The data to copy into the new element.
                *
                * \return Returns false if there were no more preallocated
                * elements or key already added.
                */

              e_noinline bool set( const K key, const T& data ){
                // Acquire the mutex or critical section.
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                // Walk to the end of the list searching.
                const auto ix = u32( key ) % PRIME;
                auto& head = m_pUsed[ ix ];
                auto next = head;
                while( next ){
                  auto& node = m_aList[ next-1 ];
                  if( node.m_uKey != key ){
                    next = node.m_uNext;
                    continue;
                  }
                  return false;
                }
                // Calculate the root index into table. If no head just attach in
                // place.
                if( !m_uFree ){
                  return false;
                }
                // Pop next item of the free list.
                auto _free = m_uFree;
                auto& free = m_uFree;
                auto& node = m_aList[ free-1 ];
                free = node.m_uNext;
                // Push node onto used list.
                node.m_uNext = head;
                node.m_uKey = key;
                head = _free;
                // Allocate new node data.
                new( node.m_aData )T( data );
                ++m_uSize;
                return true;
              }

              /** \brief This routine sets a value into the table using move
                * semantics.
                *
                * This routine will set a new value by it's key.
                *
                * \param key The key to set on the table.
                *
                * \param data The data to move into the new element.
                *
                * \return Returns false if there were no more preallocated
                * elements.
                */

              e_noinline bool set( const K key, T&& data ){
                // Acquire the mutex or critical section.
                typename decltype( m_tLock )::LockGuard lockGuard( m_tLock );
                // Walk to the end of the list searching.
                const auto ix = u32( key ) % PRIME;
                auto& head = m_pUsed[ ix ];
                auto next = head;
                while( next ){
                  auto& node = m_aList[ next-1 ];
                  if( node.m_uKey != key ){
                    next = node.m_uNext;
                    continue;
                  }
                  return false;
                }
                // Calculate the root index into table. If no head just attach in
                // place.
                if( !m_uFree ){
                  return false;
                }
                // Pop next item of the free list.
                auto _free = m_uFree;
                auto& free = m_uFree;
                auto& node = m_aList[ free-1 ];
                free = node.m_uNext;
                // Push node onto used list.
                node.m_uNext = head;
                node.m_uKey = key;
                head = _free;
                // Allocate new node data.
                new( node.m_aData )T( std::move( data ));
                ++m_uSize;
                return true;
              }

            //}:                                  |
          //}:                                    |
          //--------------------------------------|-----------------------------

          SpinlockPool(){
            memset( m_pUsed, 0, sizeof( m_pUsed ));
            memset( m_aList, 0, sizeof( m_aList ));
            primeFree();
          }

        private:

          void primeFree(){
            for( u32 i=0; i<N; ++i ){
              m_aList[ i ].m_uNext = i+2;
            }
            m_aList[ N-1 ].m_uNext = 0;
            m_uFree = 1;
          }
          u32              m_pUsed[ PRIME ];
          Node             m_aList[ N ];
          u32              m_uFree = 0;
          u32              m_uSize = 0;
          mutable Spinlock m_tLock;
        };
      }
    }
  }

//}:                                              |
//ShareLock:{                                     |

#ifdef __APPLE__
  #pragma mark - sharelock -
#endif

  namespace EON{

    namespace gfc{

      namespace atomic{

        struct ShareLock final{

          //--------------------------------------|-----------------------------
          //Methods:{                             |

            /** \brief Try to acquire the share lock (non-recursive).
              *
              * This routine will acquire the share lock.
              */

            bool tryAcquireRead(){
              #if !e_compiling( web )
                return m_tLock.try_lock_shared();
              #else
                return true;
              #endif
            }

            /** \brief Acquire the share lock (non-recursive).
              *
              * This routine will acquire the share lock.
              */

            void acquireRead(){
              #if !e_compiling( web )
                m_tLock.lock_shared();
              #endif
            }

            /** \brief Release the share lock.
              *
              * This routine releases the acquired share lock.
              */

            void releaseRead(){
              #if !e_compiling( web )
                m_tLock.unlock_shared();
              #endif
            }

            /** \brief Try to acquire the share lock (non-recursive).
              *
              * This routine will acquire the share lock.
              */

            bool tryAcquire(){
              #if !e_compiling( web )
                return m_tLock.try_lock();
              #else
                return true;
              #endif
            }

            /** \brief Acquire the share lock (non-recursive).
              *
              * This routine will acquire the share lock.
              */

            void acquire(){
              #if !e_compiling( web )
                m_tLock.lock();
              #endif
            }

            /** \brief Release the share lock.
              *
              * This routine releases the acquired share lock.
              */

            void release(){
              #if !e_compiling( web )
                m_tLock.unlock();
              #endif
            }

          //}:                                    |
          //--------------------------------------|-----------------------------

          ShareLock() = default;
        ~ ShareLock() = default;

        private:

          #if !e_compiling( web )
            std::shared_mutex m_tLock;
          #endif
        };
      }
    }
  }

//}:                                              |
//ShareLockRecursive:{                            |

  namespace EON{

    namespace gfc{

      /** \brief Atomic operations.
        *
        * A structure containing the most common atomic operators for all
        * supported platforms.
        */

      namespace atomic{

        /** \brief Thread shared lock.
          *
          * This class implements a recursive read/write locking protocol for
          * heavily multi-threaded applications.
          */

        struct E_PUBLISH ShareLockRecursive final{

          //--------------------------------------|-----------------------------
          //Structs:{                             |
            //Atomic:{                            |

              template<typename T> struct E_PUBLISH Atomic:std::atomic<T>{
                using std::atomic<T>::atomic;
              };

            //}:                                  |
            //LockGuard:{                         |

              struct E_PUBLISH LockGuard final{

                //--------------------------------|-----------------------------
                //Operate:{                       |

                    e_forceinline operator bool()const{
                      return m_bTry;
                    }

                //}:                              |
                //--------------------------------|-----------------------------

                e_noinline LockGuard( const ShareLockRecursive* pSharedLock, const bool bShared, const bool bTry=false )
                    : m_pSharedLock( const_cast<ShareLockRecursive*>( pSharedLock ))
                    , m_bShared( bShared ){
                  if( bShared ){
                    if( bTry ){
                      m_bTry.store( m_pSharedLock->tryAcquireRead() );
                    }else{
                      m_pSharedLock->acquireRead();
                    }
                  }else{
                    if( bTry ){
                      m_bTry.store( m_pSharedLock->tryAcquire() );
                    }else{
                      m_pSharedLock->acquire();
                    }
                  }
                }
                LockGuard( const LockGuard& ) = delete;
                LockGuard( LockGuard&& ) = delete;
                LockGuard() = default;
                e_noinline~LockGuard(){
                  if( m_pSharedLock ){
                    if( m_bShared ){
                      m_pSharedLock->releaseRead();
                    }else{
                      m_pSharedLock->release();
                    }
                  }
                }

              private:

                ShareLockRecursive* m_pSharedLock = nullptr;
                Atomic<bool>        m_bShared     = false;
                Atomic<bool>        m_bTry        = false;
              };

            //}:                                  |
          //}:                                    |
          //Methods:{                             |

            /** \name Acquisition of lock for reading.
              *
              * These routines will lock for reading only.
              *
              * @{
              */

            /** \brief Try locking shared lock.
              *
              * This routine will attempt to acquire a read only lock. This is
              * the meat and potatoes of locking for reading.
              *   i) Attempt is made to acquire the read lock. It asks the
              *      question "are there any read locks already?" If there
              *      are any read locks on any thread return true.
              *  ii) It then asks if there are any write locks on the same
              *      thread as the current one. If so, it returns true.
              * iii) If there are any write locks on any other thread we must
              *      return false and immediately bail.
              *
              * \return Returns true if the shared lock was acquired and false
              * if you should try again later.
              */

            bool tryAcquireRead();

            /** \brief Unacquire shared acquire.
              *
              * This routine will unacquire a shared acquire whether it was acquireed
              * with acquireShared or tryAcquireShared.
              */

            void releaseRead();

            /** \brief Acquire shared read-only acquire.
              *
              * This routine will acquire a read-only acquire.
              */

            void acquireRead();

            /** @}
              *
              * \name Acquisition of lock for writing.
              *
              * These routines will lock for writing only.
              *
              * @{
              */

            /** \brief Try to acquire a read-write acquire.
              *
              * This routine will attempt to acquire a write acquire.
              *
              * \return Returns true if the acquire was acquired and false if you
              * should try again later.
              */

            bool tryAcquire();

            /** \brief Unacquire the shared acquire.
              *
              * This routine will unacquire the write only acquire.
              */

            void release();

            /** \brief Lock the object.
              *
              * This routine will acquire the shared acquire for writing.
              */

            void acquire();

            /** @} */

          //}:                                    |
          //--------------------------------------|-----------------------------

          ShareLockRecursive() = default;
         ~ShareLockRecursive() = default;

        private:

          SpinlockRecursive     m_atomicWrites;
          SpinlockPool<u32,u32> m_atomicReads;
          Spinlock              m_tLock;
        };

        /** \brief Macro to guard read-write lock.
          *
          * This macro will auto-acquire/release a write lock.
          *
          * \param lock The ShareLockRecursive object to lock and unlock.
          */

        #define e_guardw( lock )                                                \
          EON::gfc::atomic::ShareLockRecursive::LockGuard e_distinct(__L)(      \
              &lock, false, false )                                             \

        /** \brief Macro to guard read only lock.
          *
          * This macro will auto-acquire/release a read lock.
          *
          * \param lock The ShareLockRecursive object to lock and unlock.
          */

        #define e_guardr( lock )                                                \
          EON::gfc::atomic::ShareLockRecursive::LockGuard e_distinct(__L)(      \
              &lock, true, false )                                              \

        /** \brief Macro to guard read-write lock.
          *
          * This macro will auto-acquire/release a write lock.
          *
          * \param lock The ShareLockRecursive object to lock and unlock.
          */

        #define e_tryw( lock )                                                  \
          EON::gfc::atomic::ShareLockRecursive::LockGuard e_distinct(__L)(      \
              &lock, false, true );                                             \
          if( e_distinct(__L))                                                  \

        /** \brief Macro to guard read only lock.
          *
          * This macro will auto-acquire/release a read lock.
          *
          * \param lock The ShareLockRecursive object to lock and unlock.
          */

        #define e_tryr( lock )                                                  \
          EON::gfc::atomic::ShareLockRecursive::LockGuard e_distinct(__L)(      \
              &lock, true, true );                                              \
          if( e_distinct(__L))                                                  \

      }
    }
  }

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
