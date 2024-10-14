//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup components
  *   @{
  */

//================================================+=============================
//IVisual:{                                       |

  namespace EON{

    namespace gfc{

      /** \brief Visual interface.
        *
        * This interface defines the update and rendering logic. Screens and
        * entities both derive from this interface.
        */

      struct E_PUBLISH IVisual{

        //----------------------------------------+-----------------------------
        //Actions:{                               |

          /** \name Update action.
            *
            * This routine will update the visual once per frame. It may or may
            * not be on the same thread as the render thread.
            *
            * @{
            */

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
            * \param dt The reciprocal number of seconds to have elapsed
            * since the last update (not necessarily per frame). You should
            * multiply all timed operations like interpolation by dt.
            */

          virtual void onTick( const f32& dt )=0;

          /** @}
            *
            * \name Rendering actions.
            *
            * These actions are called from the renderer in an immediate mode
            * or deferred mode depending on which stage of the rendering pipe
            * you're currently at.
            *
            * @{
            */

          /** \brief The engine's pre draw method.
            *
            * This routine is called in the lit pass. It is not deferred. It
            * renders to the background layer during G-buffer construction.
            */

          virtual void onPreDraw(){}

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
            \code
              virtual void onDraw(){
                const quat& Q = getRotation();
                if( m_hMesh ){
                  vec4x4 M;
                  Q.toMatrix( M );
                  IEngine::ri->setL2W( M );
                  e_drawMesh( m_hMesh );
                }
              }
            \endcode
            */

          virtual void onDraw()=0;

          /** \brief The engine's debug bounds method rendering function.
            *
            * This routine is responsible for rendering the bounding volume of
            * an IVisual interface.  The code will be called from the deferred
            * renderer  (or any other renderer of your choice even ones in C#)
            * and no depth buffer will be available so be sure to turn off the
            * depth buffering states before drawing. Everything done here is
            * unlit with no depth. Depth reading is on, writing is off.
            */

          virtual void onPostDraw(){}

          /** @} */

        //}:                                      |
        //----------------------------------------+-----------------------------

        virtual~IVisual() = default;
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
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

      struct E_PUBLISH Visual:Stream,IVisual{

        e_reflect_no_properties( Visual, Stream );

        //----------------------------------------+-----------------------------
        //Actions:{                               |

          virtual void onTick( const f32& )override{}
          virtual void onDraw()override{}

        //}:                                      |
        //----------------------------------------+-----------------------------

        Visual( const string& name )
          : Stream( name )
        {}

        Visual() = default;
      };
    }
  }

//}:                                              |
//================================================+=============================

/**   @}
  * @}
  */
