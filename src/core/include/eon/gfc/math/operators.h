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
//Maths operators:{                               |
  //Comparison:{                                  |

    template<typename Y> e_forceinline bool operator<=( const Y& y )const{ return( value <= T( y )); }
    template<typename Y> e_forceinline bool operator< ( const Y& y )const{ return( value <  T( y )); }
    template<typename Y> e_forceinline bool operator>=( const Y& y )const{ return( value >= T( y )); }
    template<typename Y> e_forceinline bool operator> ( const Y& y )const{ return( value >  T( y )); }
    template<typename Y> e_forceinline bool operator==( const Y& y )const{ return( value == T( y )); }
    template<typename Y> e_forceinline bool operator!=( const Y& y )const{ return( value != T( y )); }

  //}:                                            |
  //Arithmetic:{                                  |
    //Scalar:{                                    |

      template<typename Y> e_forceinline self& operator+=( const Y& y ){ value += T( y ); return*this; }
      template<typename Y> e_forceinline self& operator-=( const Y& y ){ value -= T( y ); return*this; }
      template<typename Y> e_forceinline self& operator*=( const Y& y ){ value *= T( y ); return*this; }
      template<typename Y> e_forceinline self& operator/=( const Y& y ){ if( y ) value /= T( y ); return*this; }
      template<typename Y> e_forceinline self  operator+ ( const Y& y )const{ return( value + T( y )); }
      template<typename Y> e_forceinline self  operator- ( const Y& y )const{ return( value - T( y )); }
      template<typename Y> e_forceinline self  operator* ( const Y& y )const{ return( value * T( y )); }
      template<typename Y> e_forceinline self  operator/ ( const Y& y )const{
        if( y ){
          return( value / T( y ));
        }
        return 0.f;
      }

    //}:                                          |
    //Vector4:{                                   |

      e_forceinline Vector4 operator+( const Vector4& y )const{ return( Vector4( *this ) + y ); }
      e_forceinline Vector4 operator-( const Vector4& y )const{ return( Vector4( *this ) - y ); }
      e_forceinline Vector4 operator*( const Vector4& y )const{ return( Vector4( *this ) * y ); }

    //}:                                          |
    //Vector3:{                                   |

      e_forceinline Vector3 operator+( const Vector3& y )const{ return( Vector3( *this ) + y ); }
      e_forceinline Vector3 operator-( const Vector3& y )const{ return( Vector3( *this ) - y ); }
      e_forceinline Vector3 operator*( const Vector3& y )const{ return( Vector3( *this ) * y ); }

    //}:                                          |
    //Vector2:{                                   |

      e_forceinline Vector2 operator+( const Vector2& y )const{ return( Vector2( *this ) + y ); }
      e_forceinline Vector2 operator-( const Vector2& y )const{ return( Vector2( *this ) - y ); }
      e_forceinline Vector2 operator*( const Vector2& y )const{ return( Vector2( *this ) * y ); }

    //}:                                          |
    //Point3:{                                    |

      e_forceinline Point3 operator+( const Point3& y )const{ return( Point3( *this ) + y ); }
      e_forceinline Point3 operator-( const Point3& y )const{ return( Point3( *this ) - y ); }
      e_forceinline Point3 operator*( const Point3& y )const{
        Point3 p( *this );
        p.x *= y.x;
        p.y *= y.y;
        p.z *= y.z;
        return p;
      }

    //}:                                          |
    //Point2:{                                    |

      e_forceinline Point2 operator+( const Point2& y )const{ return( Point2( *this ) + y ); }
      e_forceinline Point2 operator-( const Point2& y )const{ return( Point2( *this ) - y ); }
      e_forceinline Point2 operator*( const Point2& y )const{ return( Point2( *this ) * y ); }

    //}:                                          |
  //}:                                            |
  //Conversion:{                                  |

    e_forceinline explicit operator   bool()const{ return value != 0; }
    e_forceinline explicit operator double()const{ return value; }
    e_forceinline explicit operator  float()const{ return value; }
    e_forceinline explicit operator   half()const{ return value; }
    e_forceinline explicit operator    u64()const{ return value; }
    e_forceinline explicit operator    s64()const{ return value; }
    e_forceinline explicit operator    u32()const{ return value; }
    e_forceinline explicit operator    s32()const{ return value; }
    e_forceinline explicit operator    u16()const{ return value; }
    e_forceinline explicit operator    s16()const{ return value; }
    e_forceinline explicit operator     u8()const{ return value; }
    e_forceinline explicit operator     s8()const{ return value; }

  //}:                                            |
  //Assignment:{                                  |

    e_forceinline self operator=( const float y ){
      value = y;
      return value;
    }

  //}:                                            |
  //Unary:{                                       |

    e_forceinline self operator-()const{
      return -value;
    }

    e_forceinline self operator+()const{
      return +value;
    }

    e_forceinline bool operator!()const{
      return !value;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
