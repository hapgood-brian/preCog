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
//Point3:{                                        |

  struct Point3{

    //--------------------------------------------|-----------------------------
    //Statics:{                                   |

      /** \brief Number of tuples in this type.
        *
        * This type defines the number of tuples in this point type.
        */

      static constexpr u32 kTuples = 3;

      /** \brief Static zero point.
        *
        * This static instance defines the zero point or origin of the world.
        */

      static const Point3 kZero;

    //}:                                          |
    //Structs:{                                   |

      union{
        struct{
          self x; //!< X coordinate.
          self y; //!< Y coordinate.
          self z; //!< Z coordinate.
        };
        self a[ 3 ];
      };

    //}:                                          |
    //Operate:{                                   |

      /** \name Operators.
        *
        * These operators compare or assign, or index into the point.
        *
        * @{
        */

      /** \brief This is the equality operator.
        *
        * This routine will compare this point against another.
        *
        * \param P The pointer to compare with.
        *
        * \return Returns true if the two points (this and P) are equal.
        */

      e_forceinline bool operator==( const Point3& P )const{
        e_assert( valid() );
        if( x != P.x ){
          return false;
        }
        if( y != P.y ){
          return false;
        }
        if( z != P.z ){
          return false;
        }
        return true;
      }

      /** \brief This is the inequality operator.
        *
        * This routine will compare this point against another.
        *
        * \param P The pointer to compare with.
        *
        * \return Returns true if the two points (this and P) are unequal.
        */

      e_forceinline bool operator!=( const Point3& P )const{
        e_assert( valid() );
        if( x != P.x ){
          return true;
        }
        if( y != P.y ){
          return true;
        }
        if( z != P.z ){
          return true;
        }
        return false;
      }

      /** \brief Assignment operator.
        *
        * This routine will assign one point to another.
        *
        * \param p The point to assign to this.
        *
        * \return Returns the dereferenced this pointer.
        */

      e_forceinline Point3& operator=( const Point3& p ){
        x = p.x;
        y = p.y;
        z = p.z;
        e_assert( valid() );
        return *this;
      }

      /** \brief Bracket indexing operator.
        *
        * This routine will return the tuple at the nth place.
        *
        * \param i The index into the point. Must be between zero and kTuple.
        *
        * \return Returns the indexed value as a Math<> type.
        */

      e_forceinline self operator[]( const u32 i )const{
        e_assert( i < e_dimof( a ));
        e_assert( valid() );
        return a[ i ];
      }

      /** \brief Bracket indexing operator.
        *
        * This routine will return the tuple at the ith place.
        *
        * \param i The index into the point. Must be between zero and kTuple.
        *
        * \return Returns the indexed value as a Math<> type.
        */

      e_forceinline self& operator[]( const u32 i ){
        e_assert( i < e_dimof( a ));
        e_assert( valid() );
        return a[ i ];
      }

      /** \brief Negation operator.
        *
        * This routine will negate the point.
        *
        * \return Returns the negated point.
        */

      e_forceinline Point3 operator-()const{
        e_assert( valid() );
        return Point3( -x, -y, -z );
      }

      /** @}
        *
        * \name Arithmetic operators.
        *
        * These operators perform a variety of maths functions using the matrix
        * and vector types.
        *
        * @{
        */

      Point3 operator*( const Matrix4& )const;
      Point3 operator*( const Matrix3& )const;
      Point3 operator-( const Vector3& )const;
      Point3 operator+( const Vector3& )const;
      Point3 operator/( const Vector3& )const;
      Point3 operator*( const Vector3& )const;

      /** @}
        *
        * \name Self operators.
        *
        * These operators perform a variety of maths functions using the self
        * type.
        *
        * @{
        */

      e_forceinline Point3& operator-=( const self& scalar ){
        e_assert( valid() );
        x -= scalar;
        y -= scalar;
        z -= scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3& operator+=( const self& scalar ){
        e_assert( valid() );
        x += scalar;
        y += scalar;
        z += scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3& operator*=( const self& scalar ){
        e_assert( valid() );
        x *= scalar;
        y *= scalar;
        z *= scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3& operator/=( const self& scalar ){
        e_assert( valid() );
        x /= scalar;
        y /= scalar;
        z /= scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3 operator-( const self& scalar )const{
        e_assert( valid() );
        return Point3( x - scalar, y - scalar, z - scalar );
      }

      e_forceinline Point3 operator+( const self& scalar )const{
        e_assert( valid() );
        return Point3( x + scalar, y + scalar, z + scalar );
      }

      e_forceinline Point3 operator*( const self& scalar )const{
        e_assert( valid() );
        return Point3( x * scalar, y * scalar, z * scalar );
      }

      e_forceinline Point3 operator/( const self& scalar )const{
        e_assert( valid() );
        return Point3( x / scalar, y / scalar, z / scalar );
      }

      /** @}
        *
        * \name Scalar operators.
        *
        * These operators perform a variety of maths functions using the real
        * type.
        *
        * @{
        */

      e_forceinline Point3& operator-=( const float scalar ){
        e_assert( valid() );
        x -= scalar;
        y -= scalar;
        z -= scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3& operator+=( const float scalar ){
        e_assert( valid() );
        x += scalar;
        y += scalar;
        z += scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3& operator*=( const float scalar ){
        e_assert( valid() );
        x *= scalar;
        y *= scalar;
        z *= scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3& operator/=( const float scalar ){
        e_assert( valid() );
        x /= scalar;
        y /= scalar;
        z /= scalar;
        e_assert( valid() );
        return *this;
      }

      e_forceinline Point3 operator-( const float scalar )const{
        e_assert( valid() );
        return Point3( x - scalar, y - scalar, z - scalar );
      }

      e_forceinline Point3 operator+( const float scalar )const{
        e_assert( valid() );
        return Point3( x + scalar, y + scalar, z + scalar );
      }

      e_forceinline Point3 operator*( const float scalar )const{
        e_assert( valid() );
        return Point3( x * scalar, y * scalar, z * scalar );
      }

      e_forceinline Point3 operator/( const float scalar )const{
        e_assert( valid() );
        return Point3( x / scalar, y / scalar, z / scalar );
      }

      e_forceinline Point3& operator-=( const Vector3& V ){
        e_assert( valid() );
        *this=( *this - V );
        return *this;
      }

      e_forceinline Point3& operator+=( const Vector3& V ){
        e_assert( valid() );
        *this=( *this + V );
        return *this;
      }

      e_forceinline Point3& operator+=( const Point3& P ){
        e_assert( valid() );
        *this=( *this + P );
        return *this;
      }

      e_forceinline Point3 operator+( const Point3& P )const{
        e_assert( valid() );
        Point3 r;
        r.x = x+P.x;
        r.y = y+P.y;
        r.z = z+P.z;
        return r;
      }

      e_forceinline Point3 operator-( const Point3& P )const{
        e_assert( valid() );
        Point3 r;
        r.x = x-P.x;
        r.y = y-P.y;
        r.z = z-P.z;
        return r;
      }

      /** @} */

    //}:                                          |
    //Structs:{                                   |

      struct Interpolator{

        //----------------------------------------|-----------------------------
        //Methods:{                               |

          /** \brief Begin interpolation.
            *
            * This routine will kick off the interpolator by establishing the
            * duration and the start and end points.
            *
            * \param duration The number of seconds to interpolate for.
            * \param s The start point.
            * \param e The end point.
            */

          e_forceinline void begin( const self& duration, const Point3& s, const Point3& e ){
            m_fTimeStart = m_fTime;
            m_fTimeEnd   = m_fTime + duration;
            m_tStart     = s;
            m_tEnd       = e;
          }

          /** \brief Get the current time value.
            *
            * This routine will return the current parametric time used in the
            * interpolation.
            *
            * \return Returns a real parametric number (zero to one)
            * representing the current t value (time) for the interpolator.
            */

          e_forceinline self time()const{
            return saturate((( m_fTime - m_fTimeStart )/( m_fTimeEnd - m_fTimeStart )));
          }

          /** \brief Get the interpolated value.
            *
            * This routine will return the interpolation result.
            *
            * \return Returns a Point3 object containing the interpolated
            * result.
            */

          e_forceinline Point3 getValue()const{
            return Point3(
                self::lerp( m_tStart.x, m_tEnd.x, time() ),
                self::lerp( m_tStart.y, m_tEnd.y, time() ),
                self::lerp( m_tStart.z, m_tEnd.z, time() ));
          }

          /** \brief Check if finished.
            *
            * This routine checks to see if the interpolation is finished. If
            * it is it returns true otherwise false.
            *
            * \return Returns true if the interpolator is done or false.
            */

          e_forceinline bool isFinished()const{
            return( time()==k1 );
          }

          /** \brief Interpolator tick function.
            *
            * Call this function once per frame by the owner of the
            * interpolator.
            *
            * \param deltaTime The number of seconds that elapsed since the
            * last frame. Can and should be fractional.
            */

          e_forceinline void tick( const self& deltaTime ){
            m_fTime += deltaTime;
          }

        //}:                                      |
        //----------------------------------------|-----------------------------

        Interpolator()=default;

      private:

        e_var( Point3, t, Start );
        e_var( Point3, t, End );

        self m_fTimeStart = k0;
        self m_fTimeEnd   = k0;
        self m_fTime      = k0;
      };

    //}:                                          |
    //Methods:{                                   |

      e_forceinline void set( const self& X, const self& Y, const self& Z ){
        x = X;
        y = Y;
        z = Z;
        e_assert( valid() );
      }

      e_forceinline void set( const Point2& P, const self& Z ){
        x = P.x;
        y = P.y;
        z = Z;
        e_assert( valid() );
      }

      e_forceinline void set( const self& X, const self& Y ){
        x = X;
        y = Y;
        z = 0;
        e_assert( valid() );
      }

      e_forceinline void set( const Point2& P ){
        x = P.x;
        y = P.y;
        z = 0;
        e_assert( valid() );
      }

      e_forceinline void set( const self& X ){
        x = X;
        y = X;
        z = X;
        e_assert( valid() );
      }

      /** \brief Check if point is valid.
        *
        * This routine verifies that each component of the point is numerically
        * valid. It uses the math<self>::valid() call to do this.
        *
        * \return Returns true if the point is valid or false otherwise.
        */

      e_forceinline bool valid()const{
        if( !x.valid() ){
          return false;
        }
        if( !y.valid() ){
          return false;
        }
        if( !z.valid() ){
          return false;
        }
        return true;
      }

      /** \brief Get the distance from the origin squared.
        *
        * This routine is identical to length() except the result is squared.
        *
        * \return Returns the length squared to the origin.
        */

      e_forceinline self lengthSquared()const{
        e_assert( valid() );
        return x*x+y*y+z*z;
      }

      /** \brief Get the distance from origin.
        *
        * This routine will return the scalar value which is the length of the
        * vector from the point to the origin.
        *
        * \return Returns the distance from the point to the origin.
        */

      e_forceinline self length()const{
        e_assert( valid() );
        return sqrt( lengthSquared() );
      }

      /** \brief Get the distance between two points squared.
        *
        * This routine is faster than the regular distance() method because it
        * doesn't use the expensive square-root. If you do all your distance
        * checks in squared space then you can gain some pretty important
        * speed ups in your game code.
        *
        * \param O The point to get the distance from.
        *
        * \return Returns a scalar representing the distance between the point
        * O and this point.
        */

      e_forceinline self distanceSquared( const Point3& O )const{
        e_assert( valid() );
        const self& xx = x-O.x;
        const self& yy = y-O.y;
        const self& zz = z-O.z;
        return( xx*xx + yy*yy + zz*zz );
      }

      /** \brief Get the true distance between two points.
        *
        * This routine will calculate the distance between two points and
        * return the result.
        *
        * \param O The point to get the distance from.
        *
        * \return Returns the distance scalar.
        */

      e_forceinline self distance( const Point3& O )const{
        e_assert( valid() );
        return sqrt( distanceSquared( O ));
      }

      /** \brief Negate the point.
        *
        * This routine will negate the point in 3D space.
        */

      e_forceinline void negate(){
        e_assert( valid() );
        x = -x;
        y = -y;
        z = -z;
      }

      /** \brief Set to origin.
        *
        * This routine will zero out the point effectively setting it to the
        * origin location.
        */

      e_forceinline void clear(){
        x = y = z = k0;
      }

      /** \brief Interpolate between two points.
        *
        * This routine will interpolate between two points and store the result
        * in this point.
        *
        * \param a The starting point.
        * \param b The ending point.
        * \param in_t The parametric time value.
        */

      e_forceinline void lerp( const Point3& a, const Point3& b, const self& in_t ){
        e_assert( valid() );
        const self& t = in_t.saturated();
        x = self::lerp( a.x, b.x, t );
        y = self::lerp( a.y, b.y, t );
        z = self::lerp( a.z, b.z, t );
      }

      /** \brief Interpolate between this and point P.
        *
        * This routine will linearly interpolate between this point and the
        * given point P using a parametric t value.
        *
        * \param P The end point (this object is the start).
        * \param in_t The pareametric t value.
        */

      e_forceinline void lerp( const Point3& P, const self& in_t ){
        e_assert( valid() );
        const self& t = in_t.saturated();
        x = self::lerp( x, P.x, t );
        y = self::lerp( y, P.y, t );
        z = self::lerp( z, P.z, t );
      }

      e_forceinline Point3 lerped( const Point3& P, const self& in_t )const{
        e_assert( valid() );
        const self& t = in_t.saturated();
        return Point3(
          self::lerp( x, P.x, t ),
          self::lerp( y, P.y, t ),
          self::lerp( z, P.z, t )
        );
      }

      e_forceinline Point2 xy()const{
        e_assert( valid() );
        return Point2( x, y );
      }

    //}:                                          |
    //--------------------------------------------|-----------------------------

    /** \name Composite constructors.
      *
      * These constructors allow you to instantiate a 3D point from a variety
      * of other points, vectors and scalars.
      *
      * @{
      */

    /** \brief 2D point and scalar constructor.
      *
      * This constructor will build a 3D point from a 2D one and a scalar for
      * the Z (up) axis.
      *
      * \param P A point in the 2D ground plane at "scalar" z.
      * \param scalar The elevation of the ground plane in the local frame.
      */

    e_forceinline Point3( const Point2& P, const self& scalar )
        : x( P.x )
        , y( P.y )
        , z( scalar ){
      e_assert( valid() );
    }

    /** \brief 2D to 3D point conversion constructor.
      *
      * This constructor will build a 3D point from a 2D one and set the
      * elevation (z) to the origin (zero).
      *
      * \param P A point in the 2D ground plane at 0z.
      */

    e_forceinline Point3( const Point2& P )
        : x( P.x )
        , y( P.y )
        , z( 0 ){
      e_assert( valid() );
    }

    /** \brief Copy constructor.
      *
      * This constructor copies the contents of the input argument 'p' to this
      * 3D point.
      *
      * \param p The 3D point to copy into this.
      */

    e_forceinline Point3( const Point3& p )
        : x( p.x )
        , y( p.y )
        , z( p.z ){
      e_assert( valid() );
    }

    /** \brief Homogeneous vector to 3D point conversion constructor.
      *
      * This constructor will convert a point in homogeneous space to a 3D one
      * simply by stripping off the W component as if it was set to zero.
      *
      * \param v The homogeneous vector to assign to this.
      */

    Point3( const Vector4& v );

    /** \brief 3D vector to point conversion constructor.
      *
      * This constructor will convert a point in some space to this.
      *
      * \param v The vector to assign to this.
      */

    Point3( const Vector3& v );

    /** @}
      *
      * \name Scalar constructors.
      *
      * These constructors build a 3D point from a number of scalars so we can
      * instantiate one with only one scalar (applied to all three tuples), or
      * two scalars or even three.  The const expression forms of these combos
      * are also provided for optimal performance.
      *
      * @{
      */

    e_forceinline_always Point3( const self& X, const self& Y, const self& Z )
      : x( X )
      , y( Y )
      , z( Z )
    {}

    e_forceinline_always Point3( const self& X, const self& Y )
      : x( X )
      , y( Y )
      , z( 0 )
    {}

    e_forceinline_always Point3( const self& s )
      : x( s )
      , y( s )
      , z( s )
    {}

    e_forceinline_always Point3( const float X, const float Y, const float Z )
      : x( X )
      , y( Y )
      , z( Z )
    {}

    e_forceinline_always Point3( const float X, const float Y )
      : x( X )
      , y( Y )
      , z( 0 )
    {}

    e_forceinline_always Point3( const float s )
      : x( s )
      , y( s )
      , z( s )
    {}

    e_forceinline_always Point3()
      : x( 0 )
      , y( 0 )
      , z( 0 )
    {}

    /** @} */
  };

//}:                                              |
//================================================|=============================