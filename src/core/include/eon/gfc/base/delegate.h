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

#pragma once

#include"../srutil/delegate/delegate.hpp"
#include"../srutil/event/event.hpp"

/** \addtogroup engine
  * @{
  *   \addtogroup base
  *   @{
  *     \addtogroup macros
  *     @{
  */

//================================================|=============================
//Action templates:{                              |
  //Action producer:{                             |

    /** \brief Core action class.
      *
      * The action system is based on the "Impossibly Fast C++ Delegates" on
      * Code Project but significant enhanced. For example all controller events
      * will trigger onTouch etc. actions, the skeletal animation system will
      * trigger onBone() events etc. The speed of the system is critical.
      * Everything related to rendering, ticking entities and even controller
      * input is done through actions. It is the backbone of the EON Engine.
      *
      * Note that any IListen<> interface inherited by an object can only do so
      * with a single action. For example if a Component object or entity is
      * listening to an IScreen action it can only listen to one not two screen
      * instances.
      */

    template<typename IAbstract> struct action:boost::noncopyable{

      struct IListener:boost::noncopyable,IAbstract{};

      //------------------------------------------|-----------------------------
      //self_t:{                                  |

        typedef srutil::event_source<IListener*> self_t;

      //}:                                        |
      //self:{                                    |

        /** \brief Accessor to self object.
          *
          * \return This acessor simply returns a reference to the
          * srutil::event_source object. This represents the listener.
          */

        e_forceinline self_t& self()const{
          return m_tSelf;
        }

      //}:                                        |
      //------------------------------------------|-----------------------------

    private:

      mutable self_t m_tSelf;
    };

  //}:                                            |
  //Action consumer:{                             |

    /** \brief <b>The listener object.</b>
      *
      * This is the forward facing object that classes use to listen, that is
      * <i>register</i> for specific actions. One listener to one action stream
      * is the rule. This class is always the base class of any listener class.
      */

    template<typename T>struct listener:action<T>::IListener{

      //------------------------------------------|-----------------------------
      //Methods:{                                 |

        #define  _trigger_6 typename P, typename A, typename B, typename C, typename D, typename E, typename F
        #define  _trigger_5 typename P, typename A, typename B, typename C, typename D, typename E
        #define  _trigger_4 typename P, typename A, typename B, typename C, typename D
        #define  _trigger_3 typename P, typename A, typename B, typename C
        #define  _trigger_2 typename P, typename A, typename B
        #define  _trigger_1 typename P, typename A
        #define  _trigger_0 typename P

        template<_trigger_6> static e_forceinline bool conditional_trigger( P p, A a, B b, C c, D d, E e, F f ){
          return actions().self().conditional_emit( boost::bind( p, _1, a, b, c, d, e, f ));
        }

        template<_trigger_5> e_forceinline static bool conditional_trigger( P p, A a, B b, C c, D d, E e ){
          return actions().self().conditional_emit( boost::bind( p, _1, a, b, c, d, e ));
        }

        template<_trigger_4> e_forceinline static bool conditional_trigger( P p, A a, B b, C c, D d ){
          return actions().self().conditional_emit( boost::bind( p, _1, a, b, c, d ));
        }

        template<_trigger_3> e_forceinline static bool conditional_trigger( P p, A a, B b, C c ){
          return actions().self().conditional_emit( boost::bind( p, _1, a, b, c ));
        }

        template<_trigger_2> e_forceinline static bool conditional_trigger( P p, A a, B b ){
          return actions().self().conditional_emit( boost::bind( p, _1, a, b ));
        }

        template<_trigger_1> e_forceinline static bool conditional_trigger( P p, A a ){
          return actions().self().conditional_emit( boost::bind( p, _1, a ));
        }

        template<_trigger_0> e_forceinline static bool conditional_trigger( P p ){
          return actions().self().conditional_emit( boost::bind( p, _1 ));
        }

        template<_trigger_6> e_forceinline static void trigger( P p, A a, B b, C c, D d, E e, F f ){
          actions().self().emit( boost::bind( p, _1, a, b, c, d, e, f ));
        }

        template<_trigger_5> e_forceinline static void trigger( P p, A a, B b, C c, D d, E e ){
          actions().self().emit( boost::bind( p, _1, a, b, c, d, e ));
        }

        template<_trigger_4> e_forceinline static void trigger( P p, A a, B b, C c, D d ){
          actions().self().emit( boost::bind( p, _1, a, b, c, d ));
        }

        template<_trigger_3> e_forceinline static void trigger( P p, A a, B b, C c ){
          actions().self().emit( boost::bind( p, _1, a, b, c ));
        }

        template<_trigger_2> e_forceinline static void trigger( P p, A a, B b ){
          actions().self().emit( boost::bind( p, _1, a, b ));
        }

        template<_trigger_1> e_forceinline static void trigger( P p, A a ){
          actions().self().emit( boost::bind( p, _1, a ));
        }

        template<_trigger_0> e_forceinline static void trigger( P p ){
          actions().self().emit( boost::bind( p, _1 ));
        }

        e_forceinline void unbind(){
          m_tBinder.unbind();
        }

        e_forceinline void bind()const{
          m_tBinder.bind(
            actions().self(),
            const_cast<listener*>( this )
          );
        }

        e_forceinline void bind(){
          m_tBinder.bind(
            actions().self(),
            this
          );
        }

      //}:                                        |
      //------------------------------------------|-----------------------------

      e_forceinline listener( const bool bAutobind=false ){
        if( bAutobind ){
          m_tBinder.bind(
            actions().self(),
            this
          );
        }
      }

    protected:

      using super = action<T>;

    private:

      static action<T>& actions(){
        return e_singleton( action<T> );
      }

      typedef typename super::self_t::binder_type binder_t;
      mutable binder_t m_tBinder;
    };

  //}:
//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
