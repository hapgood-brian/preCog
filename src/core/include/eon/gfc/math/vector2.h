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
//Vector2:{                                       |

  /** \brief Two tuple vector.
    *
    * This is a 2D vector for controlling sprite movements and widgets etc.
    */

  struct E_PUBLISH Vector2{

    //--------------------------------------------|-----------------------------
    //Statics:{                                   |

      static const u32     kTuples = 2;
      static const Vector2 kRight;
      static const Vector2 kZero;
      static const Vector2 kUp;

    //}:                                          |
    //Structs:{                                   |

      union{
        struct{
          self x, y;
        };
        struct{
          self u, v;
        };
        self a[ 2 ];
      };

    //}:                                          |
    //Operate:{                                   |

      e_noinline self operator[]( const s32 i )const{
        switch( i ){
          case 0:
            return x;
          case 1:
            [[fallthrough]];
          default:
            return y;
        }
      }

      e_noinline self& operator[]( const s32 i ){
        switch( i ){
          case 0:
            return x;
          case 1:
            [[fallthrough]];
          default:
            return y;
        }
      }

      /** \name Self operators.
        *
        * These operators perform various math on the two tuple vector using
        * the embedded self type.
        *
        * @{
        */

      e_forceinline Vector2& operator=( const self& scalar ){
        x = y = scalar;
        return *this;
      }

      e_forceinline Vector2 operator+( const self& s )const{
        return Vector2( x+s, y+s );
      }

      e_forceinline Vector2 operator-( const self& s )const{
        return Vector2( x-s, y-s );
      }

      e_forceinline Vector2 operator/( const self& s )const{
        return Vector2( x/s, y/s );
      }

      e_forceinline Vector2 operator*( const self& s )const{
        return Vector2( x*s, y*s );
      }

      Vector2& operator+=( const self& s ){
        *this += Vector2( s, s );
        return *this;
      }

      Vector2& operator-=( const self& s ){
        *this -= Vector2( s, s );
        return *this;
      }

      Vector2& operator/=( const self& s ){
        *this /= Vector2( s, s );
        return *this;
      }

      Vector2& operator*=( const self& s ){
        *this *= Vector2( s, s );
        return *this;
      }

      /** @}
        *
        * \name Floating point operators,
        *
        * These operators perform various math operations on this two tuple
        * vector using the floating point type.
        *
        * @{
        */

      e_forceinline Vector2& operator=( const float scalar ){
        x = y = scalar;
        return *this;
      }

      e_forceinline Vector2 operator+( const float s )const{
        return Vector2( x+s, y+s );
      }

      e_forceinline Vector2 operator-( const float s )const{
        return Vector2( x-s, y-s );
      }

      e_forceinline Vector2 operator/( const float s )const{
        return Vector2( x/s, y/s );
      }

      e_forceinline Vector2 operator*( const float s )const{
        return Vector2( x*s, y*s );
      }

      Vector2& operator+=( const float s ){
        *this += Vector2( s, s );
        return *this;
      }

      Vector2& operator-=( const float s ){
        *this -= Vector2( s, s );
        return *this;
      }

      Vector2& operator/=( const float s ){
        *this /= Vector2( s, s );
        return *this;
      }

      Vector2& operator*=( const float s ){
        *this *= Vector2( s, s );
        return *this;
      }

      /** @} */

      e_forceinline bool operator==( const Vector2& v )const{
        return( 0==memcmp( this, &v, sizeof( Vector2 )));
      }

      e_forceinline bool operator!=( const Vector2& v )const{
        return( 0!=memcmp( this, &v, sizeof( Vector2 )));
      }

      e_forceinline Vector2& operator=( const Vector2& v ){
        x = v.x;
        y = v.y;
        return *this;
      }

      Vector2  operator+ ( const Vector2& v )const;
      Vector2  operator- ( const Vector2& v )const;
      Vector2  operator/ ( const Vector2& v )const;
      Vector2  operator* ( const Vector2& v )const;
      Vector2  operator+ ( const Point2&  p )const;
      Vector2  operator- ( const Point2&  p )const;
      Vector2  operator/ ( const Point2&  p )const;
      Vector2  operator* ( const Point2&  p )const;
      Vector2& operator+=( const Vector2& v );
      Vector2& operator-=( const Vector2& v );
      Vector2& operator/=( const Vector2& v );
      Vector2& operator*=( const Vector2& v );
      Vector2& operator+=( const Point2&  p );
      Vector2& operator-=( const Point2&  p );
      Vector2& operator/=( const Point2&  p );
      Vector2& operator*=( const Point2&  p );

      e_forceinline Vector2 operator-()const{
        return Vector2( -x, -y );
      }

    //}:                                          |
    //Methods:{                                   |

      e_forceinline bool valid()const{
        return( x.valid() && y.valid() );
      }

      e_forceinline void set( const self& X, const self& Y ){
        x = X;
        y = Y;
        e_assert( valid() );
      }

      e_forceinline self dotPerp( const Vector2& v )const{
        e_assert( valid() );
        return dot( v.perp() );
      }

      e_forceinline self dot( const Vector2& V )const{
        e_assert( valid() );
        return x*V.x+y*V.y;
      }

      e_forceinline self lengthSquared()const{
        e_assert( valid() );
        return( x*x + y*y );
      }

      e_forceinline Vector2 normalized()const{
        e_assert( valid() );
        Vector2 r = *this;
        r.normalize();
        return r;
      }

      e_forceinline Vector2 unitPerp()const{
        e_assert( valid() );
        Vector2 v = perp();
        v.normalize();
        return v;
      }

      e_forceinline Vector2 perp()const{
        e_assert( valid() );
        return Vector2( a[ 1 ], -a[ 0 ]);
      }

      e_forceinline self length()const{
        e_assert( valid() );
        return sqrt( x*x + y*y );
      }

      e_forceinline void normalize(){
        e_assert( valid() );
        const self& d=length();
        x /= d;
        y /= d;
      }

      e_forceinline self max()const{
        e_assert( valid() );
        return self::max( x, y );
      }

      e_forceinline self min()const{
        e_assert( valid() );
        return self::min( x, y );
      }

      e_forceinline void clear(){
        x=y=0;
      }

    //}:                                          |
    //--------------------------------------------|-----------------------------

    e_forceinline_always Vector2( const std::initializer_list<T>& ilist )
        : x( 0 )
        , y( 0 ){
      e_assert( ilist.size() <= 2 );
      u32 i = 0;
      for( auto& v : ilist ){
        a[ i++ ]=v;
      }
    }

    e_forceinline_always Vector2( const self& X, const self& Y )
        : x( X )
        , y( Y ){
      e_assert( valid() );
    }

    e_forceinline_always Vector2( const float X, const float Y )
      : x( X )
      , y( Y )
    {}

    e_forceinline_always Vector2( const self& s )
        : x( s )
        , y( s ){
      e_assert( valid() );
    }

    e_forceinline_always Vector2( const float s )
      : x( s )
      , y( s )
    {}

    e_forceinline_always Vector2( const Vector2& v )
        : x( v.x )
        , y( v.y ){
      e_assert( valid() );
    }

    e_forceinline_always Vector2( const Point2& p )
        : x( p.x )
        , y( p.y ){
      e_assert( valid() );
    }

    e_forceinline_always Vector2()
      : x( 0 )
      , y( 0 )
    {}
  };

//}:                                              |
//================================================|=============================
