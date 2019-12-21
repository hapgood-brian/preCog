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

/** \addtogroup engine
  * @{
  *   \addtogroup components
  *   @{
  */

//================================================|=============================
//IVisual:{                                       |

  namespace EON{

    namespace gfc{

      /** \brief Base component interface.
        *
        * Like all RED Engine Interfaces (REI) the IComponent defines a set of
        * pure virtual interfaces to be implemented by the Component class. The
        * I says it's pure and the removal of the letter denotes that the
        * implementation must be Component.
        *
        * The IComponent then is the most basic building block in all rendering
        * and updating functions of a game. The EON Engine is entirely
        * component based so whenever you need a new entity in your title, such
        * as a Steampunk Space Marine, you can easily add one by deriving your
        * own Component class.
        *
        * Whenever an object is derived from IComponent and some other Object
        * derived type then the new class is known as a soft component. Any
        * class that's derived from Component is known as a hard component.
        */

      struct IVisual{

        //----------------------------------------|-----------------------------
        //Actions:{                               |

          /** \brief The engine's principal update function.
            *
            * The onTick method is called at an entirely different framerate
            * than the render thread; in other words, it runs flat out for the
            * platform you're running on. For example, on iOS the framerate for
            * onTick is 30fps like then render thread (run from the app
            * delegate in update() method). On the PC it will be properly
            * multi-threaded and runs at a base four times the render thread
            * frequency or 120hz.
            *
            * \param deltaTime The reciprocal number of seconds to have elapsed
            * since the last update (not necessarily per frame). You should
            * multiply all timed operations like interpolation by deltaTime.
            */

          virtual void onTick( const f32& deltaTime )=0;

          /** \brief The engine's principal rendering function.
            *
            * The onDraw function is called once per frame at a guaranteed
            * frequency of 30fps. To maintain this frequency the onDraw
            * function may be called from a separate thread.
            *
            * The method is constant to avoid threading issues between the
            * render and update threads. Please don't use mutable to cheat
            * unless you want to take on the responsibility of making
            * everything thread safe.
            *
            * To render something from onDraw use the e_drawMesh() macro and
            * other methods of IEngine::ri.
            *
            \code
              const quat& Q = getRotation();
              if( m_hMesh ){
                vec4x4 M;
                Q.toMatrix( M );
                IEngine::ri->setL2W( M );
                e_drawMesh( m_hMesh );
              }
            \endcode
            */

          virtual void onDraw()=0;

        //}:                                          |
        //--------------------------------------------|-----------------------------

        virtual~IVisual() = default;
      };
    }
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Visual:{                                        |

  namespace EON{

    namespace gfc{

      /** \brief Base rendering component.
        *
        * Any scene, whether it be 2D or 3D, is made up of components. Through
        * components objects are ticked and drawn, which is everything they
        * need to do something interesting and "gamey" on the screen.
        *
        * A component is also said to be auto-referencing. When used in
        * conjunction with a AutoRef<> template they will automatically destroy
        * themselves when the AutoRef goes out of scope. All components should
        * be wrapped in this way to avoid memory leaks.
        */

      struct Visual:Resource,IVisual{

        e_reflect_no_properties( Visual, Resource );

        //----------------------------------------|-----------------------------
        //Operate:{                               |

          e_forceinline Visual& operator=( const Visual& lvalue ){
            super::operator=( static_cast<const super&>( lvalue ));
            return *this;
          }

        //}:                                      |
        //Actions:{                               |

          virtual void onTick( const f32& )override{}
          virtual void onDraw()override{}

        //}:                                      |
        //----------------------------------------|-----------------------------

        Visual( const string& name )
          : Resource( name )
        {}

        Visual() = default;
      };
    }
  }

//}:                                              |
//================================================|=============================

/**   @}
  * @}
  */
