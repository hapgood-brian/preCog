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

//================================================+=============================
//Interpolator:{                                  |

  /** \brief Interpolator class.
    *
    * This class will interpolate between two real values over time.
    */

  struct E_PUBLISH Interpolator final{

    //--------------------------------------------+-----------------------------
    //Methods:{                                   |

      /** \brief Begin interpolating.
        *
        * This routine will start interpolating between value s and e over N
        * seconds.
        *
        * \param N The number of seconds the interpolation should take.
        * \param s The starting value.
        * \param e The ending value.
        */

      e_forceinline void begin( const self& N, const self& s, const self& e ){
        m_fTimeStart = m_fTime;
        m_fTimeEnd   = m_fTime + N;
        m_fStart     = s;
        m_fEnd       = e;
      }

      /** \brief Get parametric time value.
        *
        * This routine returns the parameteric time or "t" value. It will
        * always have a value between 0 and 1.
        *
        * \return Returns a paremteric value between 0 and 1 representing the
        * interpolation between the start and end times.
        */

      e_forceinline self time()const{
        if( m_fStart < m_fEnd ){
          return saturate(( m_fTime - m_fTimeStart )/( m_fTimeEnd - m_fTimeStart ));
        }
        return 0.f;
      }

      /** \brief Get interpolated value.
        *
        * This routine uses the lerp() method of math<T> (f32 or f64 to the
        * rest of the world) to interpolate between the start and end values of
        * this interpolation class.
        *
        * \return Returns the interpolated value between the start and end
        * times provided to the constructor.
        */

      e_forceinline self getValue()const{
        if( m_fStart < m_fEnd ){
          return lerp( m_fStart, m_fEnd, time() );
        }
        return 1.f;
      }

      /** \brief Is finished.
        *
        * This routine returns true if the interpolator is finished or false.
        *
        * \return Returns a boolean value telling the caller whether the
        * interpolate has finished or not, in other words the "t" value is 1.
        */

      e_forceinline bool isFinished()const{
        return( time()==k1 );
      }

      /** \brief Tick the interpolator.
        *
        * This routine will increment the interpolator by some delta time
        * value.
        *
        * \param dt A fraction in seconds to increment the m_fTime
        * member variable. This is what time() uses to calculate the
        * parametric value.
        */

      e_forceinline void tick( const self& dt ){
        m_fTime += dt;
      }

    //}:                                          |
    //--------------------------------------------+-----------------------------

    Interpolator() = default;
  ~ Interpolator() = default;

  private:

    e_var( self, f, Start ) = 0;
    e_var( self, f, End   ) = 0;

    self m_fTimeStart = 0;
    self m_fTimeEnd   = 0;
    self m_fTime      = 0;
  };

//}:                                              |
//================================================+=============================
