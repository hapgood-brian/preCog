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

//================================================|=============================
//Plane:{                                         |

  /** \brief The plane class.
    *
    * This structure defines the plane and everything you can do with one.
    */

  struct Plane final:Vector4{

    //--------------------------------------------|-----------------------------
    //Methods:{                                   |

      /** \brief Get distance to point from plane.
        *
        * This routine returns the distance from the plane to the point P.
        *
        * \param P The point to measure.
        * \return Returns the distance to the point.
        */

      e_forceinline self distance( const Point3& P )const{
        return
            a() * P.x +
            b() * P.y +
            c() * P.z +
            d();
      }

      /** \brief Get normal of the plane.
        *
        * This routine will return the normal of the plane.
        *
        * \return Returns the normal of the plane.
        */

      e_forceinline Vector3 normal()const{
        return Vector4::xyz().normalized();
      }

      /** \brief Set the plane from the given triangle.
        *
        * This routine will set the plane to the plane of the triangle t.
        *
        * \param t The triangle to load the plane from.
        */

      void set( const Triangle& t );

      /** \brief Get the A component.
        *
        * This routine will get the A component of the plane.
        *
        * \return Returns the A component of the plane.
        */

      e_forceinline const self& a()const{
        return Vector4::x;
      }

      /** \brief Get the A component.
        *
        * This routine will get the A component of the plane.
        *
        * \return Returns the A component of the plane.
        */

      e_forceinline self& a(){
        return Vector4::x;
      }

      /** \brief Get the B component.
        *
        * This routine will get the B component of the plane.
        *
        * \return Returns the B component of the plane.
        */

      e_forceinline const self& b()const{
        return Vector4::y;
      }

      /** \brief Get the B component.
        *
        * This routine will get the B component of the plane.
        *
        * \return Returns the B component of the plane.
        */

      e_forceinline self& b(){
        return Vector4::y;
      }

      /** \brief Get the C component.
        *
        * This routine will get the C component of the plane.
        *
        * \return Returns the C component of the plane.
        */

      e_forceinline const self& c()const{
        return Vector4::z;
      }

      /** \brief Get the C component.
        *
        * This routine will get the C component of the plane.
        *
        * \return Returns the C component of the plane.
        */

      e_forceinline self& c(){
        return Vector4::z;
      }

      /** \brief Get the D component.
        *
        * This routine will get the D component of the plane.
        *
        * \return Returns the D component of the plane.
        */

      e_forceinline const self& d()const{
        return Vector4::w;
      }

      /** \brief Get the D component.
        *
        * This routine will get the D component of the plane.
        *
        * \return Returns the D component of the plane.
        */

      e_forceinline self& d(){
        return Vector4::w;
      }

    //}:                                          |
    //--------------------------------------------|-----------------------------

    e_forceinline Plane( const Triangle& in_tTriangle ){
      set( in_tTriangle );
    }

    Plane() = default;
  };

//}:                                              |
//================================================|=============================
