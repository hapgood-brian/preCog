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

//================================================|=============================
//Ray3:{                                          |

  struct E_PUBLISH Ray3 final{

    //--------------------------------------------|-----------------------------
    //Operate:{                                   |

      /** \name Ray operators.
        *
        * These operators allow you to transform the ray and do other things.
        *
        * @{
        */

      e_forceinline Ray3 operator*( const Matrix4& m )const{
        Ray3 r;
        r.d = d * m;
        r.p = p * m;
        return r;
      }

      e_forceinline Ray3& operator*=( const Matrix4& m ){
        d = m * d;
        p = m * p;
        return *this;
      }

    //}:                                          |
    //Methods:{                                   |

      // http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection

      /** @{
        *
        * \name Ray methods.
        *
        * These methods allow you to do intersection tests and other things
        * with the ray.
        *
        * @{
        */

      /** Get closet distance between two rays.
        *
        * This routine will return the shortest distance between two rays.
        */

      f32 closestDistance( const Ray3& l2, self& aT, self& bT );

      /** \brief Perform a ray-disk intersection test.
        *
        * This routine will perform an intersection using this ray against the
        * given disk.
        *
        * \param O The origin of the disk.
        * \param N The normal of the disk.
        * \param r The radius of the disk.
        * \param out The output of the test (the collision point).
        *
        * \return Returns true if the ray intersected the disk otherwise false.
        */

      e_noinline bool intersectsDisk( const Point3& O, const Vector3& N, const self& r, Point3& out )const{
        self t;
        if( !intersectsPlane( O, N, t )){
          return false;
        }
        const Vector3 hit = p + d * t;
        const Vector3 v = hit - O;
        const self d2 = v.dot( v );
        return( d2.sqrt() <= r );
      }

      /** \brief Perform a ray-plane intersection test.
        *
        * This routine will perform an intersection test using this ray against
        * the plane provided.
        *
        * \param O The origin of the plane.
        * \param N The normal of the plane.
        * \param out The distance along the ray that the hit was found.
        *
        * \return Returns true if the ray intersected with the plane or false.
        */

      e_noinline bool intersectsPlane( const Vector3& O, const Vector3& N, self& out )const{
        const self denom = N.dot( d );
        if( denom.abs() <= T( 1e-6 )){
          out = 0;
          return false;
        }
        const Vector3 v = O-p;
        out = v.dot( N ) / denom;
        return( out >= 0 );
      }

      /** \brief Get point at distance t.
        *
        * This routine will return a point for the given t value.
        *
        * \param t The distance along the ray to get a point for.
        * \param bNormalize Normalize the direction before scaling by t.
        *
        * \return Returns a point at t distance.
        */

      e_noinline Point3 at( const self& t, const bool bNormalize=false )const{
        if( bNormalize ){
          return p+d.normalized()*t;
        }
        return p+d*t;
      }

    //}:                                          |
    //--------------------------------------------|-----------------------------

    e_forceinline Ray3( const self& x, const self& y, const self& z, const self& dx, const self& dy, const self& dz )
      : d( dx, dy, dz )
      , p(  x,  y,  z )
    {}

    constexpr Ray3( const float x, const float y, const float z, const float dx, const float dy, const float dz )
      : d( dx, dy, dz )
      , p(  x,  y,  z )
    {}

    e_forceinline Ray3( const Point3& a, const Vector3& b )
      : d( b )
      , p( a )
    {}

    e_forceinline Ray3( const Ray3& r )
      : d( r.d )
      , p( r.p )
    {}

    Ray3() = default;

    Vector3 d;
    Point3  p;
  };

//}:                                              |
//================================================|=============================
