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

#include<random>
#include<cmath>

using namespace EON;
using namespace gfc;

//================================================+=============================
//Constants:{                                     |

  //http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo3

#ifdef __APPLE__
  #pragma mark - Constants -
#endif

  /* Constants */

  namespace EON{
    template<> const i32 i32::k0       =  0;
    template<> const i32 i32::k1       =  1;
    template<> const i32 i32::k2       =  2;
    template<> const i32 i32::k3       =  3;
    template<> const i32 i32::k4       =  4;
    template<> const i32 i32::k5       =  5;
    template<> const i32 i32::k6       =  6;
    template<> const i32 i32::k7       =  7;
    template<> const i32 i32::k8       =  8;
    template<> const i32 i32::k9       =  9;
    template<> const i32 i32::k10      = 10;
    template<> const i32 i32::kPI      =  3;
    template<> const i32 i32::k2PI     = kPI * k2;
    template<> const i32 i32::k4PI     = kPI * k4;
    template<> const i32 i32::cm       = 1.f;
    template<> const i32 i32::mm       = cm / 10;
    template<> const i32 i32::m        =     100 * cm;
    template<> const i32 i32::dm       =      10 * m;
    template<> const i32 i32::km       =    1000 * m;
    template<> const i32 i32::Mm       =    1000 * km;
    template<> const i32 i32::g        =       1;
    template<> const i32 i32::kg       =    1000 * g;
    template<> const i32 i32::t        =    1000 * kg;
    template<> const i32 i32::kt       =    1000 * t;
    template<> const i32 i32::mt       =    1000 * kt;
    template<> const i32 i32::kMin     =-INT_MAX;
    template<> const i32 i32::kMax     = INT_MAX;
    template<> const i32 i32::kEpsilon = 1;
    template<> const i32 i32::kHalf    = 0;
  }

  /* Vector constants */

  namespace EON{
    template<> const vec4i vec4i::kForward ( 0, 1, 0 );
    template<> const vec4i vec4i::kUp      ( 0, 0, 1 );
    template<> const vec4i vec4i::kZero    ( 0, 0, 0 );
    template<> const vec4i vec4i::kRight   ( 1, 0, 0 );
    template<> const vec3i vec3i::kForward ( 0, 1, 0 );
    template<> const vec3i vec3i::kRight   ( 1, 0, 0 );
    template<> const vec3i vec3i::kUp      ( 0, 0, 1 );
    template<> const vec3i vec3i::kZero    ( 0, 0, 0 );
    template<> const vec2i vec2i::kRight   ( 1, 0 );
    template<> const vec2i vec2i::kUp      ( 0,-1 );
    template<> const vec2i vec2i::kZero    ( 0, 0 );
    template<> const pt3i  pt3i ::kZero    ( 0, 0, 0 );
    template<> const pt2i  pt2i ::kZero    ( 0, 0 );
    template<> const quati quati::kIdentity( 0, 0, 0, 1 );
  }

  /* Color constants */

  namespace EON{
    template<> const rgbai rgbai::kBlack  (   0, 255 );
    template<> const rgbai rgbai::kWhite  ( 255 );
    template<> const rgbai rgbai::kGrey   ( 127, 255 );
    template<> const rgbai rgbai::kOrange ( 255, .5f,   0, 255 );
    template<> const rgbai rgbai::kYellow ( 255, 255,   0, 255 );
    template<> const rgbai rgbai::kLime   ( 127, 255,   0, 255 );
    template<> const rgbai rgbai::kGreen  (   0, 255,   0, 255 );
    template<> const rgbai rgbai::kCyan   (   0, 255, 255, 255 );
    template<> const rgbai rgbai::kBlue   (   0,   0, 255, 255 );
    template<> const rgbai rgbai::kMagenta( 255,   0, 255, 255 );
    template<> const rgbai rgbai::kRed    ( 255,   0,   0, 255 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2i aabb2i::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2i aabb2i::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2i aabb2i::kZero  ( 0 );
    template<> const aabb3i aabb3i::kZero  ( 0 );
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Basic:{                                         |
  //[forwarding]:{                                |
    //vec3i:{                                     |
      //[operators]:{                             |
        //operator=:{                             |

          namespace EON{
            template<> vec3i& vec3i::operator=( const vec4i& V ){
              x = V.x;
              y = V.y;
              z = V.z;
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
      //Ctor:{                                    |

        /*namespace EON{
          template<> vec3i::Vector3( const vec4i& V ){
            x = V.x;
            y = V.y;
            z = V.z;
          }
        }*/

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //smoothstep:{                                  |

    template<> i32 i32::smoothstep( const i32& edge2, const i32& x )const{
      const self& t = clamp(( x-value )/( edge2-value ), k0, k1 );
      return t*t*( k3-k2*t );
    }

  //}:                                            |
  //saturate[d]:{                                 |

    template<> i32 i32::saturated()const{
      return max( min( value, k1 ), k0 );
    }

    template<> void i32::saturate(){
      *this = max( min( value, k1 ), k0 );
    }

  //}:                                            |
  //iround:{                                      |

    namespace EON{
      template<> s32 i32::iround()const{
        return value;
      }
    }

  //}:                                            |
  //rsqrt:{                                       |

#ifdef __APPLE__
  #pragma mark - Basic functions -
#endif

    namespace EON{
      template<> i32 i32::rsqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //sqrt:{                                        |

    namespace EON{
      template<> i32 i32::sqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //floor:{                                       |

    namespace EON{
      template<> i32 i32::floor()const{
        return value;
      }
    }

  //}:                                            |
  //ceil:{                                        |

    namespace EON{
      template<> i32 i32::ceil()const{
        return value;
      }
    }

  //}:                                            |
  //frac:{                                        |

    namespace EON{
      template<> i32 i32::frac( i32& i )const{
        return 0;
      }
    }

  //}:                                            |
  //exp:{                                         |

    namespace EON{
      template<> i32 i32::exp()const{
        return 0;
      }
    }

  //}:                                            |
  //pow:{                                         |

    namespace EON{
      template<> i32 i32::pow( const i32& y )const{
        s32 sum = 1;
        for( i32 i=0; i<y; ++i ){
          sum *= i32::cast( value );
        }
        return sum;
      }
    }

  //}:                                            |
  //atan2:{                                       |

    namespace EON{
      template<> i32 i32::atan2( const i32& y )const{
        return 0;
      }
    }

  //}:                                            |
  //atan:{                                        |

    namespace EON{
      template<> i32 i32::atan()const{
        return 0;
      }
    }

  //}:                                            |
  //tan:{                                         |

    namespace EON{
      template<> i32 i32::tan()const{
        return 0;
      }
    }

  //}:                                            |
  //asin:{                                        |

    namespace EON{
      template<> i32 i32::asin()const{
        return 0;
      }
    }

  //}:                                            |
  //sin:{                                         |

    namespace EON{
      template<> i32 i32::sin()const{
        return 0;
      }
    }

  //}:                                            |
  //acos:{                                        |

    namespace EON{
      template<> i32 i32::acos()const{
        return 0;
      }
    }

  //}:                                            |
  //cos:{                                         |

    namespace EON{
      template<> i32 i32::cos()const{
        return 0;
      }
    }

  //}:                                            |
  //mod:{                                         |

    namespace EON{
      template<> i32 i32::mod( const i32& y )const{
        return value % i32::cast( y );
      }
    }

  //}:                                            |
  //abs:{                                         |

    namespace EON{
      template<> i32 i32::abs()const{
        return::abs( value );
      }
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Validity:{                                      |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Validity
  #pragma mark -
#endif

  namespace EON{
    template<> bool i32::valid( const i32& t ){
      return true;
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Geometry:{                                      |
  //Points:{                                      |
    //[operators]:{                               |
      //operator-:{                               |

#ifdef __APPLE__
  #pragma mark - Points -
#endif

        namespace EON{
          template<> pt3i pt3i::operator-( const vec3i& v )const{
            pt3i r;
            r.x = x-v.x;
            r.y = y-v.y;
            r.z = z-v.z;
            return r;
          }
        }

      //}:                                        |
      //operator+:{                               |

        namespace EON{
          template<> pt3i pt3i::operator+( const vec3i& v )const{
            pt3i r;
            r.x = x+v.x;
            r.y = y+v.y;
            r.z = z+v.z;
            return r;
          }
        }

      //}:                                        |
      //operator/:{                               |

        namespace EON{
          template<> pt3i pt3i::operator/( const vec3i& v )const{
            pt3i r;
            r.x = x/v.x;
            r.y = y/v.y;
            r.z = z/v.z;
            return r;
          }
        }

      //}:                                        |
      //operator*:{                               |

        namespace EON{
          template<> pt3i pt3i::operator*( const vec4x4i& M )const{
            return pt3i(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> pt3i pt3i::operator*( const vec3x3i& M )const{
            return pt3i(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z
            );
          }
          template<> pt3i pt3i::operator*( const vec3i& v )const{
            pt3i r;
            r.x = x*v.x;
            r.y = y*v.y;
            r.z = z*v.z;
            return r;
          }
        }

      //}:                                        |
    //}:                                          |
    //[ctors]:{                                   |

      namespace EON{
        template<> pt3i::Point3( const vec4i& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

      namespace EON{
        template<> pt3i::Point3( const vec3i& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

    //}:                                          |
    //Methods:{                                   |

      namespace EON{
        template<> pt2i pt2i::scaledToClip( const i32& scale )const{
          return{};
        }
      }

      namespace EON{
        template<> void pt2i::scaleToClip( const i32& scale ){}
      }

    //}:                                          |
  //}:                                            |
  //Circle:{                                      |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb2i circlei::toBounds()const{
          return aabb2i(
            v.x - v.z,
            v.y - v.z,
            v.x + v.z,
            v.y + v.z
          );
        }
      }

    //}:                                          |
  //}:                                            |
  //Sphere:{                                      |
    //intersects:{                                |

      namespace EON{
        template<> bool spherei::intersects( const ray3i& in_tRay, i32& out_fDistance )const{
          const vec3i Q = this->v.xyz() - in_tRay.p;
          const i32& c = Q.length();
          const i32& v = Q.dot( in_tRay.d );
          const i32& d = toRadius()*toRadius() - (c*c - v*v);
          if( d < 0.f ){
            out_fDistance = -1.f;
            return false;
          }
          out_fDistance = (v - sqrt( d ));
          return true;
        }
      }

    //}:                                          |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb3i spherei::toBounds()const{
          return aabb3i(
            v.x - v.w,
            v.y - v.w,
            v.z - v.w,
            v.x + v.w,
            v.y + v.w,
            v.z + v.w
          );
        }
      }

    //}:                                          |
  //}:                                            |
  //Plane:{                                       |

    namespace EON{
      template<> void planei::set( const trianglei& in_tTriangle ){
      }
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Algebra:{                                       |
  //Quaternion:{                                  |
    //sqLength:{                                  |

#ifdef __APPLE__
  #pragma mark - Algebra -
#endif

      namespace EON{
        template<> i32 quati::lengthSquared()const{
          return 0;
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void quati::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //operator::==:{                            |

        namespace EON{
          template<> bool quati::operator==( const quati& q )const{
            return( 0 == memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::!=:{                            |

        namespace EON{
          template<> bool quati::operator!=( const quati& q )const{
            return( 0 != memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quati::operator<( const quati& q )const{
            return( memcmp( a, q.a, sizeof( a )) < 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quati::operator<=( const quati& q )const{
            return( memcmp( a, q.a, sizeof( a )) <= 0 );
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quati::operator>( const quati& q )const{
            return( memcmp( a, q.a, sizeof( a )) > 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quati::operator>=( const quati& q )const{
            return( memcmp( a, q.a, 16 ) >= 0 );
          }
        }

      //}:                                        |
      //operator::+?:{                            |

        namespace EON{
          template<> quati& quati::operator+=( const quati& q ){
            x+=q.x;
            y+=q.y;
            z+=q.z;
            w+=q.w;
            return *this;
          }
        }

        namespace EON{
          template<> quati quati::operator+( const quati& q )const{
            return quati(
              x+q.x,
              y+q.y,
              z+q.z,
              w+q.w
            );
          }
        }

      //}:                                        |
      //operator::*?:{                            |

        namespace EON{
          template<> quati quati::operator*( const quati& q )const{
            return quati(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z
            );
          }
        }

        namespace EON{
          template<> quati quati::operator*( const i32& s )const{
            return quati(
              x * s,
              y * s,
              z * s,
              w * s
            );
          }
        }

        namespace EON{
          template<> quati& quati::operator*=( const quati& q ){
            const quati r(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z );
            *this = r;
            return *this;
          }
        }

        namespace EON{
          template<> quati& quati::operator*=( const i32& s ){
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
          }
        }

      //}:                                        |
      //operator::/?:{                            |

        namespace EON{
          template<> quati& quati::operator/=( const quati& q ){
            quati p( q );
            p.invert();
            (*this)*p;
            return *this;
          }
        }

        namespace EON{
          template<> quati& quati::operator/=( const i32& s ){
            if( s != 0 ){
              x/=s;
              y/=s;
              z/=s;
              w/=s;
            }else{
              x = 0;
              y = 0;
              z = 0;
              w = 0;
            }
            return *this;
          }
        }

        namespace EON{
          template<> quati quati::operator/( const quati& q )const{
            quati p( q );
            p.invert();
            return (*this)*p;
          }
        }

        namespace EON{
          template<> quati quati::operator/( const i32& s )const{
            quati q;
            if( s != 0 ){
              q.x=x/s;
              q.y=y/s;
              q.z=z/s;
              q.w=w/s;
            }else{
              q.x = 0;
              q.y = 0;
              q.z = 0;
              q.w = 0;
            }
            return q;
          }
        }

      //}:                                        |
    //}:                                          |
    //setAxisAngle:{                              |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Quaternions
  #pragma mark -
#endif

      namespace EON{
        template<> void quati::setAxisAngle( const vec3i& axis, const i32& angleDeg ){
          const i32& a = rad( angleDeg );
          const vec3i& v = axis*sin( a / 2 );
          x = v.x;
          y = v.y;
          z = v.z;
          w = cos( a/2.f );
        }
      }

    //}:                                          |
    //asQuaternion:{                              |

      namespace EON{
        template<> quati vec3x3i::asQuaternion()const{
          return{};
        }
      }

    //}:                                          |
    //compare:{                                   |

      namespace EON{
        template<> bool quati::compare( const quati& q, const i32& tolerance )const{
          return true;
        }
      }

    //}:                                          |
    //rotate:{                                    |

      namespace EON{
        template<> void quati::rotate( const vec3i& axis, const i32& angleDeg ){
        }
      }

      namespace EON{
        template<> void quati::rotate( const vec2i& xyMouse ){
        }
      }

    //}:                                          |
    //length:{                                    |

      namespace EON{
        template<> i32 quati::length()const{
          return 0;
        }
      }

    //}:                                          |
    //normalize:{                                 |

      namespace EON{
        template<> quati& quati::normalize(){
          return *this;
        }
      }

    //}:                                          |
    //setEuler:{                                  |

      namespace EON{
        template<> void quati::setEuler( const vec3i& e ){
        }
      }

    //}:                                          |
    //getEuler:{                                  |

      namespace EON{
        template<> void quati::getEuler( i32& out_fX, i32& out_fY, i32& out_fZ, const bool bHomogeneous )const{
        }
      }

    //}:                                          |
    //randomize:{                                 |

      namespace EON{
        template<> void quati::randomize( const i32& x1, const i32& x2, const i32& x3 ){
        }
      }

    //}:                                          |
    //get?x?:{                                    |

      namespace EON{
        template<> vec4x4i quati::get4x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3i quati::get3x3()const{
          return{};
        }
      }

    //}:                                          |
    //dot:{                                       |

      namespace EON{
        template<> i32 quati::dot( const quati& q )const{
          return 0;
        }
      }

    //}:                                          |
    //lookat:{                                    |

      namespace EON{
        template<> void quati::lookat( const pt3i& srcPoint, const pt3i& dstPoint ){
        }
      }

    //}:                                          |
    //inverse:{                                   |

      namespace EON{
        template<> quati quati::inverse()const{
          return{};
        }
      }

    //}:                                          |
    //conjugate:{                                 |

      namespace EON{
        template<> quati quati::conjugate()const{
          return{};
        }
      }

    //}:                                          |
    //exp:{                                       |

      namespace EON{
        template<> quati quati::exp()const{
          return{};
        }
      }

    //}:                                          |
    //log:{                                       |

      namespace EON{
        template<> quati quati::log()const{
          return{};
        }
      }

    //}:                                          |
    //lerped:{                                    |

      namespace EON{
        template<> quati quati::lerped( const quati& p, const quati& q, const i32& fTime ){
          return{};
        }
      }

    //}:                                          |
    //lerp:{                                      |

      namespace EON{
        template<> quati& quati::lerp( const quati& p, const quati& q, const i32& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //slerpedNoInvert:{                           |

      namespace EON{
        template<> quati quati::slerpedNoInvert( const quati& p, const quati& q, const i32& fTime ){
          return{};
        }
      }

    //}:                                          |
    //slerp:{                                     |

      namespace EON{
        template<> quati& quati::slerp( const quati& p, const quati& q, const i32& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //squad:{                                     |

      namespace EON{
        template<> quati& quati::squad( const quati& q1, const quati& q2, const quati& a, const quati& b, const i32& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //align:{                                     |

      namespace EON{
        template<> quati& quati::align( const vec3i& up, const vec3i& norm ){
          return *this;
        }
      }

    //}:                                          |
  //}:                                            |
  //Matrices:{                                    |
    //vec4x4i:{                                    |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Matrices
  #pragma mark -
#endif

        namespace EON{
          template<> vec4x4i& vec4x4i::operator*=( const vec4x4i& B ){
            return *this;
          }
        }

        //}:                                      |
      //}:                                        |
      //transpose:{                               |

        namespace EON{
          template<> void vec4x4i::transpose(){
          }
        }

      //}:                                        |
      //setPerspectiveV2C:{                       |

        namespace EON{
          template<> void vec4x4i::setPerspectiveV2C( const i32& degFovY, const i32& aspect, const i32& zn, const i32& zf ){
          }
        }

      //}:                                        |
      //setAxisIdentity:{                         |

        namespace EON{
          template<> void vec4x4i::setAxisIdentity(){
            YX = ZX = WX = XY = WY = YY = 0;
            XZ = WZ = XW = YW = ZW = ZZ = 0;
            XX = 1;
            YZ = 1;
            ZY = 1;
            WW = 1;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec4x4i::setTranslation( const i32& x, const i32& y, const i32& z ){
            XX = 1; XY = 0; XZ = 0; XW = 0;
            YX = 0; YY = 1; YZ = 0; YW = 0;
            ZX = 0; ZY = 0; ZZ = 1; ZW = 0;
            WX = x; WY = y; WZ = z; WW = 1;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec4x4i::setScale( const i32& x, const i32& y, const i32& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

        namespace EON{
          template<> void vec4x4i::setScale( const i32& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec4x4i::setRotX( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec4x4i::setRotY( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec4x4i::setRotZ( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec4x4i::removeTranslation(){
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec4x4i::removeScale(){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec4x4i vec4x4i::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec4x4i::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x4i:{                                    |
      //[converter]:{                             |
        //vec4x4i:{                               |

          namespace EON{
            template<> template<> vec4x4i vec3x4i::as<vec4x4i>()const{
              return{};
            }
          }

        //}:                                      |
        //quati:{                                 |

          namespace EON{
            template<> template<> quati vec3x4i::as<quati>()const{
              return{};
            }
          }

        //}:                                      |
        //vec3x3i:{                               |

          namespace EON{
            template<> template<> vec3x3i vec3x4i::as<vec3x3i>()const{
              return{};
            }
          }

        //}:                                      |
        //pt3i:{                                  |

          namespace EON{
            template<> template<> pt3i vec3x4i::as<pt3i>()const{
              return{};
            }
          }

        //}:                                      |
      //}:                                        |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark - Matrices -
#endif

          namespace EON{
            template<> vec3x4i& vec3x4i::operator*=( const vec3x4i& B ){
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
      //as<>:{                                    |

        namespace EON{
          template<> template<> vec3x3i vec4x4i::as<vec3x3i>()const{
            return{};
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec3x4i::removeTranslation(){
            WX = WY = WZ = 0;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec3x4i::setTranslation( const i32& x, const i32& y, const i32& z ){
            XX = 1; YX = 0; ZX = 0; WX = x;
            XY = 0; YY = 1; ZY = 0; WY = y;
            XZ = 0; YZ = 0; ZZ = 1; WZ = z;
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec3x4i::removeScale(){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x4i::setScale( const i32& x, const i32& y, const i32& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
          }
        }

        namespace EON{
          template<> void vec3x4i::setScale( const i32& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x4i::setRotX( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x4i::setRotY( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x4i::setRotZ( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec3x4i vec3x4i::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec3x4i::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x3i:{                                   |
      //setAxisAngle:{                            |

        namespace EON{
          template<> void vec3x3i::setAxisAngle( const vec3i& axis, const i32& angle ){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x3i::setScale( const i32& x, const i32& y, const i32& z ){
            XX = x; YX = 0; ZX = 0;
            XY = 0; YY = y; ZY = 0;
            XZ = 0; YZ = 0; ZZ = z;
          }
          template<> void vec3x3i::setScale( const i32& s ){
            XX = s; YX = 0; ZX = 0;
            XY = 0; YY = s; ZY = 0;
            XZ = 0; YZ = 0; ZZ = s;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x3i::setRotX( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x3i::setRotY( const i32& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x3i::setRotZ( const i32& angleInDeg ){
          }
        }

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Frustum:{                                     |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark - Frustums -
#endif

      namespace EON{
        template<> bool frustumi::intersects( const pt3i& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustumi::intersects( const spherei& S )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustumi::intersects( const aabb3i& B )const{
          return false;
        }
      }

    //}:                                          |
    //set:{                                       |

      namespace EON{
        template<> void frustumi::set( const vec4x4i& inW2C ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //Vector4:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec4i vec4i::operator*( const vec4x4i& M )const{
            return{};
          }
          template<> vec4i vec4i::operator*( const vec3x3i& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector3:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec3i vec3i::operator*( const vec4x4i& M )const{
            return{};
          }
          template<> vec3i vec3i::operator*( const vec3x3i& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector2:{                                   |
      //[operators]:{                             |
        //vec2i:{                                 |

          namespace EON{
            template<> vec2i  vec2i::operator+ ( const vec2i& v )const{ return vec2i( x+v.x, y+v.y ); }
            template<> vec2i  vec2i::operator- ( const vec2i& v )const{ return vec2i( x-v.x, y-v.y ); }
            template<> vec2i  vec2i::operator/ ( const vec2i& v )const{ return vec2i( x/v.x, y/v.y ); }
            template<> vec2i  vec2i::operator* ( const vec2i& v )const{ return vec2i( x*v.x, y*v.y ); }
            template<> vec2i& vec2i::operator+=( const vec2i& v ){ x+=v.x; y+=v.y; return*this; }
            template<> vec2i& vec2i::operator-=( const vec2i& v ){ x-=v.x; y-=v.y; return*this; }
            template<> vec2i& vec2i::operator/=( const vec2i& v ){ x/=v.x; y/=v.y; return*this; }
            template<> vec2i& vec2i::operator*=( const vec2i& v ){ x*=v.x; y*=v.y; return*this; }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
  //QST:{                                         |
    //get?x?*:{                                   |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark QSTs
  #pragma mark -
#endif

      namespace EON{
        template<> vec4x4i qst3i::get4x4()const{
          return{};
        }
        template<> vec3x4i qst3i::get3x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3i qst3i::get3x3()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4i qst3i::get4x4RT()const{
          return{};
        }
        template<> vec3x4i qst3i::get3x4RT()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4i qst3i::get4x4ST()const{
          return{};
        }
        template<> vec3x4i qst3i::get3x4ST()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4i qst3i::get4x4T()const{
          return{};
        }
        template<> vec3x4i qst3i::get3x4T()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4i qst3i::get4x4R()const{
          return{};
        }
        template<> vec3x4i qst3i::get3x4R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3i qst3i::get3x3R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4i qst3i::get4x4S()const{
          return{};
        }
        template<> vec3x4i qst3i::get3x4S()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3i qst3i::get3x3S()const{
          return{};
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void qst3i::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //Multiply:{                                |

        namespace EON{
          template<> pt3i qst3i::operator*( const pt3i& P )const{
            return{};
          }
        }

        namespace EON{
          template<> void qst3i::operator*=( const qst3i& L2W ){
          }
        }

      //}:                                        |
      //Array:{                                   |

        namespace EON{
          template<> i32 qst3i::operator[]( const u32 i )const{
            return 0;
          }
        }

      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Bounds:{                                        |
  //aabb3i:{                                      |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Bounding volumes
  #pragma mark -
#endif

      //http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
      namespace EON{
        template<> bool aabb3i::intersects( const ray3i& inRay, i32& outDst )const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb3i::contains( const pt3i& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3i::contains( const aabb3i& B )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3i::contains( const trianglei& t )const{
          return false;
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb3i::empty()const{
          return false;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb3i::valid()const{
          return true;
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb3i::reset(){
        }
      }

    //}:                                          |
  //}:                                            |
  //aabb2i:{                                      |
    //scaledToClip:{                              |

      namespace EON{
        template<> aabb2i aabb2i::scaledToClip( const i32& scale )const{
          return{};
        }
      }

    //}:                                          |
    //scaleToClip:{                               |

      namespace EON{
        template<> void aabb2i::scaleToClip( const i32& scale ){
        }
      }

    //}:                                          |
    //scaleBack:{                                 |

      namespace EON{
        template<> void aabb2i::scaleBack(){
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb2i::empty()const{
          return true;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb2i::valid()const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb2i::contains( const pt2i& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2i::contains( const trianglei& t )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2i::contains( const aabb2i& B )const{
          return false;
        }
      }

    //}:                                          |
    //intersects:{                                |

      namespace EON{
        template<> bool aabb2i::intersects( const ray2i& r )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2i::intersects( const aabb2i& b )const{
          return false;
        }
      }

    //}:                                          |
    //clipAgainst:{                               |

      namespace EON{
        template<> bool aabb2i::clipAgainst( const aabb2i& b ){
          return false;
        }
      }

    //}:                                          |
    //circle:{                                    |

      namespace EON{
        template<> circlei aabb2i::circle()const{
          return{};
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb2i::reset(){
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Random:{                                        |
  //e_rand<>:{                                    |

#ifdef __APPLE__
  #pragma mark - Random -
#endif

    template<> i32 e_rand<i32>( const i32 start, const i32 end ){
      return 0;
    }

    template<> i32 e_rand<i32>(){
      return 0;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Color:{                                         |
  //Methods:{                                     |
    //multiply:{                                  |

#ifdef __APPLE__
  #pragma mark - Color -
#endif

      namespace EON{
        template<> i32 rgbai::multiply( const i32& base, const i32& blend ){
          return( base*blend );
        }
      }

    //}:                                          |
    //lighten:{                                   |

      namespace EON{
        template<> i32 rgbai::lighten( const i32& base, const i32& blend ){
          return max( base, blend );
        }
      }

    //}:                                          |
    //darken:{                                    |

      namespace EON{
        template<> i32 rgbai::darken( const i32& base, const i32& blend ){
          return min( base, blend );
        }
      }

    //}:                                          |
    //average:{                                   |

      namespace EON{
        template<> i32 rgbai::average( const i32& base, const i32& blend ){
          return( base+blend )*kHalf;
        }
      }

    //}:                                          |
    //linearDodge:{                               |

      namespace EON{
        template<> i32 rgbai::linearDodge( const i32& base, const i32& blend ){
          return self::saturate( base+blend );
        }
      }

    //}:                                          |
    //linearBurn:{                                |

      namespace EON{
        template<> i32 rgbai::linearBurn( const i32& base, const i32& blend ){
          return self::saturate( base-blend );
        }
      }

    //}:                                          |
    //difference:{                                |

      namespace EON{
        template<> i32 rgbai::difference( const i32& base, const i32& blend ){
          return abs( base-blend );
        }
      }

    //}:                                          |
    //negation:{                                  |

      namespace EON{
        template<> i32 rgbai::negation( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          return( k1 - abs( k1 - base - blend ));
        }
      }

    //}:                                          |
    //screen:{                                    |

      namespace EON{
        template<> i32 rgbai::screen( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          return 1.f-(( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //exclusion:{                                 |

      namespace EON{
        template<> i32 rgbai::exclusion( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          return( base+blend-k2*base*blend );
        }
      }

    //}:                                          |
    //overlay:{                                   |

      namespace EON{
        template<> i32 rgbai::overlay( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          return( base <kHalf ) ?( 2.f * base * blend ) :( 1.f - 2.f*( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //hardLight:{                                 |

      namespace EON{
        template<> i32 rgbai::hardLight( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          return( blend <kHalf ) ?( k2 * blend * base ) :( k1 - k2 *( k1 - blend ) *( k1 - base ));
        }
      }

    //}:                                          |
    //softLight:{                                 |

      namespace EON{
        template<> i32 rgbai::softLight( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          if( blend < kHalf ){
            return( k2 * base * blend + base*base *( k1 - k2 * blend ));
          }
          return( sqrt( base ) * ( k2 * blend - k1 ) + k2 * base * ( k1 - blend ));
        }
      }

    //}:                                          |
    //softDodge:{                                 |

      namespace EON{
        template<> i32 rgbai::softDodge( const i32& base, const i32& blend ){
          return(( base + blend ) < 1.f ) ? (( base * .5f ) / ( 1.f - blend )) : ( 1.f - (( .5f *( 1.f - blend )) / base ));
        }
      }

    //}:                                          |
    //colorDodge:{                                |

      namespace EON{
        template<> i32 rgbai::colorDodge( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //colorBurn:{                                 |

      namespace EON{
        template<> i32 rgbai::colorBurn( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          if( blend > k0 ){
            return max(( k1 -(( k1 - base ) / blend )), k0 );
          }
          return blend;
        }
      }

    //}:                                          |
    //linearLight:{                               |

      namespace EON{
        template<> i32 rgbai::linearLight( const i32& base, const i32& blend ){
          return( blend < kHalf ) ? linearBurn( base, k2*blend ) : linearDodge( base, k2*( blend-kHalf ));
        }
      }

    //}:                                          |
    //vividLight:{                                |

      namespace EON{
        template<> i32 rgbai::vividLight( const i32& base, const i32& blend ){
          return( blend < kHalf ) ? colorBurn( base,( k2 * blend )) : colorDodge( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //pinLight:{                                  |

      namespace EON{
        template<> i32 rgbai::pinLight( const i32& base, const i32& blend ){
          return( blend < kHalf ) ? darken( base,( k2 * blend )) : lighten( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //hardMix:{                                   |

      namespace EON{
        template<> i32 rgbai::hardMix( const i32& base, const i32& blend ){
          return( vividLight( base, blend ) < kHalf ) ? k0 : k1;
        }
      }

    //}:                                          |
    //reflection:{                                |

      namespace EON{
        template<> i32 rgbai::reflect( const i32& in_fBase, const i32& in_fBlend ){
          const i32& blend = self::saturate( in_fBlend );
          const i32& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base*base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //glow:{                                      |

      namespace EON{
        template<> i32 rgbai::glow( const i32& base, const i32& blend ){
          if( base != k1 ){
            return min( blend*blend/( k1 - base ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //pheonix:{                                   |

      namespace EON{
        template<> i32 rgbai::phoenix( const i32& base, const i32& blend ){
          return min( base, blend ) - max( base, blend ) + k1;
        }
      }

    //}:                                          |
    //setHSV:{                                    |

      namespace EON{
        template<> void rgbai::setHSV( const i32& H, const i32& S, const i32& V ){
        }
      }

    //}:                                          |
    //hsv:{                                       |

      namespace EON{
        //http://www.tech-faq.com/hsv.html
        template<> hsvai rgbai::hsv()const{
          return{};
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    namespace EON{
      template<> void rgbai::set( const u32 inBGRA ){
      }
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Triangle:{                                      |
  //intersects:{                                  |

#ifdef __APPLE__
  #pragma mark - Geometry -
#endif

    namespace EON{
      template<> bool trianglei::intersects( const vec4x4i& L2W, const ray3i& ray, pt3i& hit, vec3i& norm )const{
        return false;
      }
      template<> bool trianglei::intersects( const qst3i& L2W, const ray3i& ray, pt3i& hit, vec3i& norm )const{
        return false;
      }
      template<> bool trianglei::intersects( const ray3i& ray, pt3i& hit, vec3i& norm )const{
        return false;
      }
    }

  //}:                                            |
  //toBounds2D:{                                  |

    namespace EON{
      template<> aabb2i trianglei::toBounds2D()const{
        return{};
      }
    }

  //}:                                            |
  //toBounds:{                                    |

    namespace EON{
      template<> aabb3i trianglei::toBounds()const{
        return{};
      }
    }

  //}:                                            |
  //draw:{                                        |

    namespace EON{
      template<> bool trianglei::draw( IRasterizer& rasterizer, const bool bTestPrimitive )const{
        return false;
      }
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Quadtree:{                                      |
  //Structs:{                                     |
    //Leaf:{                                      |
      //Destruct:{                                |

        namespace EON{
          template<> i32::Quadtree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Quadtree: methods -
#endif

          namespace EON{
            template<> i32::Quadtree::Leaf::Result i32::Quadtree::Leaf::intersects( const Quadtree* pQuadtree, const vec4x4i& L2W, const Ray3& r )const{
              return nullptr;
            }
            template<> i32::Quadtree::Leaf::Result i32::Quadtree::Leaf::intersects( const Quadtree* pQuadtree, const vec4x4i& L2W, const Ray3& r, const u32 page )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i32::Quadtree::Leaf::Status i32::Quadtree::Leaf::addPolygon( Quadtree* pQuadtree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i32::Quadtree::Leaf::clear(){
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Octree:{                                        |
  //Structs:{                                     |
    //Leaf:{                                      |
      //Destruct:{                                |

        namespace EON{
          template<> i32::Octree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Octree: methods -
#endif

          namespace EON{
            template<> i32::Octree::Leaf::Result i32::Octree::Leaf::intersects( const Octree* pOctree, const vec4x4i& L2W, const Ray3& r )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i32::Octree::Leaf::Status i32::Octree::Leaf::addPolygon( Octree* pOctree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i32::Octree::Leaf::clear(){
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
