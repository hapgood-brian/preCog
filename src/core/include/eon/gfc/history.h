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
  *     \addtogroup histories
  *     @{
  */

//================================================+=============================
//History:{                                       |

  namespace EON{

    namespace gfc{

      /** \brief History object.
        *
        * This object is designed to capture, compress and optionally transmit
        * all modifications to an object over time. It is the basis for the
        * undo/redo buffer and the animation timeline in the editor.
        */

      struct E_PUBLISH History final:Stream{

        e_reflect_no_properties( History, Stream );

        #define OBJECT_HISTORY_VERSION u16(1)

        //----------------------------------------+-----------------------------
        //Aliases:{                               |

          struct State;

          using StreamOut = std::function<void( fs::Writer& )>;
          using StreamIn  = std::function<void( fs::Reader& )>;
          using Changed   = std::function<void()>;
          using Mutex     = std::mutex;

          /** \brief Stream type.
            *
            * The stream type is a shared pointer to a standard gfc stream.
            */

          using Stream = std::shared_ptr<stream>;

          /** \brief Animation table.
            *
            * This type represents the game turn to property list translation.
            * The table contains a hash map full of States types, which map
            * game turns to a number of keyframes. Each keyframe contains a
            * vector of property shared pointers just like you would get if you
            * called getProperties() on an object.  Now the important part here
            * is that only the properties that changed on a particular keyframe
            * will be stored in the property vector.
            *
            * The timeline is initially created in the private init() method
            * which has the unenviable task of constructing the first key frame
            * of the history. Now because we only save off the properties that
            * change during a scan() there's absolutely no reason why we can't
            * serialize the deltas between the previous frame and the next. To
            * execute a seek() command you have to start from the beginning of
            * the history and work your way forward anyway! So undos are slow
            * as hell! In practice we're not talking a lot of data so this is
            * acceptable. In the forward playback case for animating properties
            * this is going to be an incredibly fast experience. Only rewind
            * suffers, which I can live with.
            */

          using Timeline = deque<State>;

          /** \brief Game turn for keying off of.
            *
            * This alias is for readability to so we know that this particular
            * 64-bit ordinal is for describing key times in the timeline.
            */

          using When = u64;

        //}:                                      |
        //Structs:{                               |

          /** \brief A history keyframe known locally as a history state.
            *
            * History states are a change in the object we're tracking at a
            * particular moment in time based on the engine game turn. This type
            * expresses that idea.
            */

          struct State final{

            //------------------------------------+-----------------------------
            //Operate:{                           |

              e_noinline bool operator==( const State& state )const{
                if( m_dataStream && state.m_dataStream ){
                  return( *m_dataStream.get() == *state.m_dataStream.get() );
                }
                return false;
              }

              e_noinline bool operator!=( const State& state )const{
                if( m_dataStream && state.m_dataStream ){
                  return( *m_dataStream.get() != *state.m_dataStream.get() );
                }
                return true;
              }

              e_noinline State& operator=( const State& lvalue ){
                m_timeAnimTick = lvalue.m_timeAnimTick;
                m_dataStream   = lvalue.m_dataStream;
                return *this;
              }

              e_noinline State& operator=( State&& rvalue ){
                m_timeAnimTick = rvalue.m_timeAnimTick;
                m_dataStream   = std::move( rvalue.m_dataStream );
                rvalue.m_timeAnimTick = 0;
                return *this;
              }

              e_forceinline State& operator=( const stream& lvalue ){
                m_dataStream = std::make_shared<stream>( lvalue );
                return *this;
              }

              e_forceinline State& operator=( stream&& rvalue ){
                m_dataStream = std::make_shared<stream>( std::move( rvalue ));
                return *this;
              }

              e_forceinline State& operator=( const Stream& lvalue ){
                m_dataStream = lvalue;
                return *this;
              }

              e_forceinline State& operator=( Stream&& rvalue ){
                m_dataStream = std::move( rvalue );
                return *this;
              }

              e_forceinline operator const stream&()const{
                return *m_dataStream.get();
              }

              e_forceinline operator stream&(){
                return *m_dataStream.get();
              }

              e_forceinline operator const Stream&()const{
                return m_dataStream;
              }

              e_forceinline operator Stream&(){
                return m_dataStream;
              }

              e_forceinline const stream* operator->()const{
                return m_dataStream.get();
              }

              e_forceinline stream* operator->(){
                return m_dataStream.get();
              }

            //}:                                  |
            //Methods:{                           |

              virtual void serialize( fs::Writer& fs )const;
              virtual void serialize( fs::Reader& fs );

              e_forceinline State clone()const{
                return State( m_timeAnimTick, *m_dataStream.get() );
              }

            //}:                                  |
            //------------------------------------+-----------------------------

            State( const u64 tick, const stream& lvalue )
              : m_timeAnimTick( tick )
              , m_dataStream( std::make_shared<stream>( lvalue ))
            {}

            State( const stream& lvalue )
              : m_dataStream( std::make_shared<stream>( lvalue ))
            {}

            State( stream&& rvalue )
              : m_dataStream( std::make_shared<stream>( std::move( rvalue )))
            {}

            State( const State& lvalue )
              : m_timeAnimTick( lvalue.m_timeAnimTick )
              , m_dataStream( lvalue.m_dataStream )
            {}

            State( State&& rvalue )
                : m_timeAnimTick( rvalue.m_timeAnimTick )
                , m_dataStream( std::move( rvalue.m_dataStream )){
              rvalue.m_timeAnimTick = 0;
            }

            State() = default;
          ~ State() = default;

          private:

            e_var( When, time, AnimTick ) = 0;
            e_var1(      data, Stream   );
          };

        //}:                                      |
        //Methods:{                               |

          /** \name Serialization.
            *
            * These methods are used to serialize the timeline out to a memory
            * stream and from there to the network or disk.
            *
            * @{
            */

          virtual void serialize( fs::Writer& fs )const override;
          virtual s64  serialize( fs::Reader& fs )override;

          /** @}
            *
            * \name History tracking and reconstruction.
            *
            * These methods have everything to do with recording the timeline
            * and reconstructing an object (via the renewing mechanism in the
            * engine) at a given game turn in time.
            *
            * @{
            */

          /** \brief Track a new object.
            *
            * This routine will blow away the timeline for the previously
            * tracked object and setup a new one for the incoming one.  Please
            * note that tracking an object will increase its reference count as
            * it only holds on to a weak reference via it's UUID and so can't
            * weather the object being ref-counted to zero underneath it.
            *
            * \param hObject The handle of the object we want to track.
            */

          void track( const Object::handle& hObject );

          /** \brief Get object handle.
            *
            * This routine will return the handle of the object we are
            * currently tracking.
            */

          Object::handle tracking()const;

          /** \brief Set current frame and rewrite object.
            *
            * This routine will set the current frame. Calling setFrame will
            * obliterate the contents of the associated object by streaming
            * data out of the timeline and into it.
            *
            * \param zeroBasedFrameIndex The frame index relative to the
            * creation (when) date of the associated object.
            */

          void seek( const When zeroBasedFrameIndex );

          /** \brief Fast forward the object state.
            *
            * This routine will fast forward the object state back to the
            * beginning of time or when it was first created.
            */

          void ff( const u64 delta=1 );

          /** \brief Rewind the object state.
            *
            * This routine will rewind the object state back to the beginning of
            * time or when it was first created.
            */

          void rw( const u64 delta=1 );

          /** \brief Replay recording at head.
            *
            * This routine will replay the recording at the head position.
            */

          void eraseChanges();

          /** \brief record a frame of data but don't increment.
            *
            * This routine will scan for changes at the head and not increment
            * the head and tail.
            */

          bool record();

          /** \brief Scan associated object for changes.
            *
            * This routine will scan the properties of the associated object
            * for any modifications that can be serialized. It will then
            * automatically create a new State in the timeline for you with
            * only those properties that actually changed. This is the basis of
            * the automatic creation of an undo/redo buffer in the editor as
            * well as allowing for the creation of object property animations
            * that you create with the animation timeline UI in Swordlight's
            * scene view.
            *
            * \return Returns true if a new key was added or false meaning no
            * properties changed or the object UUID was zero.
            */

          bool scan();

          /** @}
            *
            * \name Mutexes.
            *
            * These methods let you access the history object mutex.
            *
            * @{
            */

          /** \brief Get mutex.
            *
            * This routine will return the mutex object.
            *
            * \return Returns a reference to the mutex.
            */

          const Mutex& toMutex()const{
            return m_tMutex;
          }

          /** \brief Get mutex.
            *
            * This routine will return the mutex object.
            *
            * \return Returns a reference to the mutex.
            */

          Mutex& toMutex(){
            return m_tMutex;
          }

          /** @} */

          e_forceinline void sendChanges(){
            if( toStatus()->bDirty ){
                toStatus()->bDirty = 0;
              if( m_onChanged ){
                m_onChanged();
              }
            }
          }

        //}:                                      |
        //----------------------------------------+-----------------------------

        explicit History( const s64 uuid );
        virtual ~History();

        History() = default;

      private:

        /* Internal member functions */

        bool record( State& out );

        /* Engine facing vars */

        e_var( Timeline, d,  Timeline );
        e_var1(          on, Changed  );
        e_var( When,     u,  Head     ) = 0ULL;
        e_var( When,     u,  Tail     ) = 0ULL;
        e_var( s64,      s,  UUID     ) = 0;
        e_var_bits(          Flags
          , bScanning:1
          , bPlayback:1
          , bActive:1
        );
        Mutex m_tMutex;
      };
    }
  }

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
