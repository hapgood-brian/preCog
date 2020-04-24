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
//Cone:{                                          |

  /** \brief Conical representation.
    *
    * This structure defines a conical volume in 3D space.
    */

  struct Cone final{

    //--------------------------------------------|-----------------------------
    //Operate:{                                   |

      e_forceinline Cone& operator=( const Cone& cone ){
        center = cone.center;
        radius = cone.radius;
        dir    = cone.dir;
        return *this;
      }

    //}:                                          |
    //Methods:{                                   |

      /** \brief Get center of the cone.
        *
        * The center of the cone is the same as the apex. It is called center()
        * to be compatible with the sphere class. A cone is only one quadrant
        * of a sphere with a radius and direction from the center of sphere.
        *
        * \return Returns a three-tuple point in space.
        */

      e_forceinline const Point3& toCenter()const{
        return center;
      }

      /** \brief Get direction of cone.
        *
        * This accessor returns the direction the cone is pointing. In many
        * ways the cone is similar to a ray and can be constructed with one.
        *
        * \return Returns a three-tuple direction vector.
        */

      e_forceinline const Vector3& toDir()const{
        return dir;
      }

      /** \brief Get height of the cone.
        *
        * This routine will returns the height of the cone as if it were
        * standing on the floor pointing up or with a direction of [0 0 1].
        *
        * \return A floating point real number representing the height of the
        * cone.
        */

      e_forceinline self toH()const{
        return dir.length();
      }

      /** \brief Get radius of cone.
        *
        * This routine returns the radius of the sphere the cone fits inside of
        * in real world units.
        *
        * \return A floating point real number representing the radius.
        */

      e_forceinline self toRadius()const{
        return radius;
      }

      /** \brief Get ray from cone.
        *
        * This routine converts the cone to a ray and returns a Ray3 structure
        * representing it.
        */

      e_forceinline Ray3 toRay()const{
        return Ray3( toCenter(), toDir() );
      }

    //}:                                          |
    //--------------------------------------------|-----------------------------

    /** \brief Ray and radius constructor.
      *
      * This routine will construct a cone from a three-tuple ray and a radius
      * of type T (a real number either 32-bit or 64-bit).
      *
      * \param ray A Ray3 structure reference representing the ray.
      * \param r The radius of the sphere this cone exists within.
      */

    e_forceinline Cone( const Ray3& ray, const self& r )
      : center( ray.p )
      , radius( r )
      , dir( ray.d )
    {}

    /** \brief Copy constructor.
      *
      * This constructor will copy the given cone into this one.
      *
      * \param cone The cone to copy into this.
      */

    e_forceinline Cone( const Cone& cone )
      : center( cone.center )
      , radius( cone.radius )
      , dir( cone.dir )
    {}

    /** \brief Default constructor.
      *
      * This routine will initialize an empty cone with zero radius, a center
      * at the origin, and no direction. Calling toH() will return zero;
      * so it is perfectly flat.
      */

    Cone() = default;

  private:

    Point3  center;
    self    radius = k0;
    Vector3 dir;
  };

//}:                                              |
//================================================|=============================
