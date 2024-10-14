//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
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
    template<> const i16 i16::k0       =  0;
    template<> const i16 i16::k1       =  1;
    template<> const i16 i16::k2       =  2;
    template<> const i16 i16::k3       =  3;
    template<> const i16 i16::k4       =  4;
    template<> const i16 i16::k5       =  5;
    template<> const i16 i16::k6       =  6;
    template<> const i16 i16::k7       =  7;
    template<> const i16 i16::k8       =  8;
    template<> const i16 i16::k9       =  9;
    template<> const i16 i16::k10      = 10;
    template<> const i16 i16::kPI      =  3;
    template<> const i16 i16::k2PI     = kPI * k2;
    template<> const i16 i16::k4PI     = kPI * k4;
    template<> const i16 i16::cm       = 1.f;
    template<> const i16 i16::mm       = cm / 10;
    template<> const i16 i16::m        =     100 * cm;
    template<> const i16 i16::dm       =      10 * m;
    template<> const i16 i16::km       =    1000 * m;
    template<> const i16 i16::Mm       =    1000 * km;
    template<> const i16 i16::g        =       1;
    template<> const i16 i16::kg       =    1000 * g;
    template<> const i16 i16::t        =    1000 * kg;
    template<> const i16 i16::kt       =    1000 * t;
    template<> const i16 i16::mt       =    1000 * kt;
    template<> const i16 i16::kMin     =-INT_MAX;
    template<> const i16 i16::kMax     = INT_MAX;
    template<> const i16 i16::kEpsilon = 1;
    template<> const i16 i16::kHalf    = 0;
  }

  /* Vector constants */

  namespace EON{
    template<> const vec4s vec4s::kForward ( 0, 1, 0 );
    template<> const vec4s vec4s::kUp      ( 0, 0, 1 );
    template<> const vec4s vec4s::kZero    ( 0, 0, 0 );
    template<> const vec4s vec4s::kRight   ( 1, 0, 0 );
    template<> const vec3s vec3s::kForward ( 0, 1, 0 );
    template<> const vec3s vec3s::kRight   ( 1, 0, 0 );
    template<> const vec3s vec3s::kUp      ( 0, 0, 1 );
    template<> const vec3s vec3s::kZero    ( 0, 0, 0 );
    template<> const vec2s vec2s::kRight   ( 1, 0 );
    template<> const vec2s vec2s::kUp      ( 0,-1 );
    template<> const vec2s vec2s::kZero    ( 0, 0 );
    template<> const pt3s  pt3s ::kZero    ( 0, 0, 0 );
    template<> const pt2s  pt2s ::kZero    ( 0, 0 );
    template<> const quats quats::kIdentity( 0, 0, 0, 1 );
  }

  /* Color constants */

  namespace EON{
    template<> const rgbas rgbas::kBlack  (   0, 255 );
    template<> const rgbas rgbas::kWhite  ( 255 );
    template<> const rgbas rgbas::kGrey   ( 127, 255 );
    template<> const rgbas rgbas::kOrange ( 255, .5f,   0, 255 );
    template<> const rgbas rgbas::kYellow ( 255, 255,   0, 255 );
    template<> const rgbas rgbas::kLime   ( 127, 255,   0, 255 );
    template<> const rgbas rgbas::kGreen  (   0, 255,   0, 255 );
    template<> const rgbas rgbas::kCyan   (   0, 255, 255, 255 );
    template<> const rgbas rgbas::kBlue   (   0,   0, 255, 255 );
    template<> const rgbas rgbas::kMagenta( 255,   0, 255, 255 );
    template<> const rgbas rgbas::kRed    ( 255,   0,   0, 255 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2s aabb2s::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2s aabb2s::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2s aabb2s::kZero  ( 0 );
    template<> const aabb3s aabb3s::kZero  ( 0 );
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Basic:{                                         |
  //[forwarding]:{                                |
    //vec3s:{                                     |
      //[operators]:{                             |
        //operator=:{                             |

          namespace EON{
            template<> vec3s& vec3s::operator=( const vec4s& V ){
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
          template<> vec3s::Vector3( const vec4s& V ){
            x = V.x;
            y = V.y;
            z = V.z;
          }
        }*/

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //smoothstep:{                                  |

    template<> i16 i16::smoothstep( const i16& edge2, const i16& x )const{
      const self& t = clamp(( x-value )/( edge2-value ), k0, k1 );
      return t*t*( k4-k2*t );
    }

  //}:                                            |
  //saturate[d]:{                                 |

    template<> i16 i16::saturated()const{
      return max( min( value, k1 ), k0 );
    }

    template<> void i16::saturate(){
      *this = max( min( value, k1 ), k0 );
    }

  //}:                                            |
  //iround:{                                      |

    namespace EON{
      template<> s32 i16::iround()const{
        return s32( value );
      }
    }

  //}:                                            |
  //rsqrt:{                                       |

#ifdef __APPLE__
  #pragma mark - Basic functions -
#endif

    namespace EON{
      template<> i16 i16::rsqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //sqrt:{                                        |

    namespace EON{
      template<> i16 i16::sqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //floor:{                                       |

    namespace EON{
      template<> i16 i16::floor()const{
        return value;
      }
    }

  //}:                                            |
  //ceil:{                                        |

    namespace EON{
      template<> i16 i16::ceil()const{
        return value;
      }
    }

  //}:                                            |
  //frac:{                                        |

    namespace EON{
      template<> i16 i16::frac( i16& i )const{
        return 0;
      }
    }

  //}:                                            |
  //exp:{                                         |

    namespace EON{
      template<> i16 i16::exp()const{
        return 0;
      }
    }

  //}:                                            |
  //pow:{                                         |

    namespace EON{
      template<> i16 i16::pow( const i16& y )const{
        s32 sum = 1;
        for( i16 i=0; i<y; ++i ){
          sum *= i16::cast( value );
        }
        return sum;
      }
    }

  //}:                                            |
  //atan2:{                                       |

    namespace EON{
      template<> i16 i16::atan2( const i16& y )const{
        return 0;
      }
    }

  //}:                                            |
  //atan:{                                        |

    namespace EON{
      template<> i16 i16::atan()const{
        return 0;
      }
    }

  //}:                                            |
  //tan:{                                         |

    namespace EON{
      template<> i16 i16::tan()const{
        return 0;
      }
    }

  //}:                                            |
  //asin:{                                        |

    namespace EON{
      template<> i16 i16::asin()const{
        return 0;
      }
    }

  //}:                                            |
  //sin:{                                         |

    namespace EON{
      template<> i16 i16::sin()const{
        return 0;
      }
    }

  //}:                                            |
  //acos:{                                        |

    namespace EON{
      template<> i16 i16::acos()const{
        return 0;
      }
    }

  //}:                                            |
  //cos:{                                         |

    namespace EON{
      template<> i16 i16::cos()const{
        return 0;
      }
    }

  //}:                                            |
  //mod:{                                         |

    namespace EON{
      template<> i16 i16::mod( const i16& y )const{
        return value % i16::cast( y );
      }
    }

  //}:                                            |
  //abs:{                                         |

    namespace EON{
      template<> i16 i16::abs()const{
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
    template<> bool i16::valid( const i16& t ){
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
          template<> pt3s pt3s::operator-( const vec3s& v )const{
            pt3s r;
            r.x = x-v.x;
            r.y = y-v.y;
            r.z = z-v.z;
            return r;
          }
        }

      //}:                                        |
      //operator+:{                               |

        namespace EON{
          template<> pt3s pt3s::operator+( const vec3s& v )const{
            pt3s r;
            r.x = x+v.x;
            r.y = y+v.y;
            r.z = z+v.z;
            return r;
          }
        }

      //}:                                        |
      //operator/:{                               |

        namespace EON{
          template<> pt3s pt3s::operator/( const vec3s& v )const{
            pt3s r;
            r.x = x/v.x;
            r.y = y/v.y;
            r.z = z/v.z;
            return r;
          }
        }

      //}:                                        |
      //operator*:{                               |

        namespace EON{
          template<> pt3s pt3s::operator*( const vec4x4s& M )const{
            return{};
          }
          template<> pt3s pt3s::operator*( const vec3x3s& M )const{
            return{};
          }
          template<> pt3s pt3s::operator*( const vec3s& v )const{
            pt3s r;
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
        template<> pt3s::Point3( const vec4s& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

      namespace EON{
        template<> pt3s::Point3( const vec3s& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

    //}:                                          |
    //Methods:{                                   |

      namespace EON{
        template<> pt2s pt2s::scaledToClip( const i16& scale )const{
          return{};
        }
      }

      namespace EON{
        template<> void pt2s::scaleToClip( const i16& scale ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Circle:{                                      |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb2s circles::toBounds()const{
          return aabb2s(
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
        template<> bool spheres::intersects( const ray3s& in_tRay, i16& out_fDistance )const{
          return false;
        }
      }

    //}:                                          |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb3s spheres::toBounds()const{
          return{};
        }
      }

    //}:                                          |
  //}:                                            |
  //Plane:{                                       |

    namespace EON{
      template<> void planes::set( const triangles& in_tTriangle ){
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
        template<> i16 quats::lengthSquared()const{
          return 0;
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void quats::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //operator::==:{                            |

        namespace EON{
          template<> bool quats::operator==( const quats& q )const{
            return( 0 == memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::!=:{                            |

        namespace EON{
          template<> bool quats::operator!=( const quats& q )const{
            return( 0 != memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quats::operator<( const quats& q )const{
            return( memcmp( a, q.a, sizeof( a )) < 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quats::operator<=( const quats& q )const{
            return( memcmp( a, q.a, sizeof( a )) <= 0 );
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quats::operator>( const quats& q )const{
            return( memcmp( a, q.a, sizeof( a )) > 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quats::operator>=( const quats& q )const{
            return( memcmp( a, q.a, 16 ) >= 0 );
          }
        }

      //}:                                        |
      //operator::+?:{                            |

        namespace EON{
          template<> quats& quats::operator+=( const quats& q ){
            x+=q.x;
            y+=q.y;
            z+=q.z;
            w+=q.w;
            return *this;
          }
        }

        namespace EON{
          template<> quats quats::operator+( const quats& q )const{
            return quats(
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
          template<> quats quats::operator*( const quats& q )const{
            return quats(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z
            );
          }
        }

        namespace EON{
          template<> quats quats::operator*( const i16& s )const{
            return quats(
              x * s,
              y * s,
              z * s,
              w * s
            );
          }
        }

        namespace EON{
          template<> quats& quats::operator*=( const quats& q ){
            const quats r(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z );
            *this = r;
            return *this;
          }
        }

        namespace EON{
          template<> quats& quats::operator*=( const i16& s ){
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
          template<> quats& quats::operator/=( const quats& q ){
            quats p( q );
            p.invert();
            (*this)*p;
            return *this;
          }
        }

        namespace EON{
          template<> quats& quats::operator/=( const i16& s ){
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
          template<> quats quats::operator/( const quats& q )const{
            quats p( q );
            p.invert();
            return (*this)*p;
          }
        }

        namespace EON{
          template<> quats quats::operator/( const i16& s )const{
            quats q;
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
        template<> void quats::setAxisAngle( const vec3s& axis, const i16& angleDeg ){
          const i16& a = rad( angleDeg );
          const vec3s& v = axis*sin( a / 2 );
          x = v.x;
          y = v.y;
          z = v.z;
          w = cos( a/2.f );
        }
      }

    //}:                                          |
    //asQuaternion:{                              |

      namespace EON{
        template<> quats vec3x3s::asQuaternion()const{
          return{};
        }
      }

    //}:                                          |
    //compare:{                                   |

      namespace EON{
        template<> bool quats::compare( const quats& q, const i16& tolerance )const{
          return true;
        }
      }

    //}:                                          |
    //rotate:{                                    |

      namespace EON{
        template<> void quats::rotate( const vec3s& axis, const i16& angleDeg ){
        }
      }

      namespace EON{
        template<> void quats::rotate( const vec2s& xyMouse ){
        }
      }

    //}:                                          |
    //length:{                                    |

      namespace EON{
        template<> i16 quats::length()const{
          return 0;
        }
      }

    //}:                                          |
    //normalize:{                                 |

      namespace EON{
        template<> quats& quats::normalize(){
          return *this;
        }
      }

    //}:                                          |
    //setEuler:{                                  |

      namespace EON{
        template<> void quats::setEuler( const vec3s& e ){
        }
      }

    //}:                                          |
    //getEuler:{                                  |

      namespace EON{
        template<> void quats::getEuler( i16& out_fX, i16& out_fY, i16& out_fZ, const bool bHomogeneous )const{
        }
      }

    //}:                                          |
    //randomize:{                                 |

      namespace EON{
        template<> void quats::randomize( const i16& x1, const i16& x2, const i16& x3 ){
        }
      }

    //}:                                          |
    //get?x?:{                                    |

      namespace EON{
        template<> vec4x4s quats::get4x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3s quats::get3x3()const{
          return{};
        }
      }

    //}:                                          |
    //dot:{                                       |

      namespace EON{
        template<> i16 quats::dot( const quats& q )const{
          return 0;
        }
      }

    //}:                                          |
    //lookat:{                                    |

      namespace EON{
        template<> void quats::lookat( const pt3s& srcPoint, const pt3s& dstPoint ){
        }
      }

    //}:                                          |
    //inverse:{                                   |

      namespace EON{
        template<> quats quats::inverse()const{
          return{};
        }
      }

    //}:                                          |
    //conjugate:{                                 |

      namespace EON{
        template<> quats quats::conjugate()const{
          return{};
        }
      }

    //}:                                          |
    //exp:{                                       |

      namespace EON{
        template<> quats quats::exp()const{
          return{};
        }
      }

    //}:                                          |
    //log:{                                       |

      namespace EON{
        template<> quats quats::log()const{
          return{};
        }
      }

    //}:                                          |
    //lerped:{                                    |

      namespace EON{
        template<> quats quats::lerped( const quats& p, const quats& q, const i16& fTime ){
          return{};
        }
      }

    //}:                                          |
    //lerp:{                                      |

      namespace EON{
        template<> quats& quats::lerp( const quats& p, const quats& q, const i16& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //slerpedNoInvert:{                           |

      namespace EON{
        template<> quats quats::slerpedNoInvert( const quats& p, const quats& q, const i16& fTime ){
          return{};
        }
      }

    //}:                                          |
    //slerp:{                                     |

      namespace EON{
        template<> quats& quats::slerp( const quats& p, const quats& q, const i16& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //squad:{                                     |

      namespace EON{
        template<> quats& quats::squad( const quats& q1, const quats& q2, const quats& a, const quats& b, const i16& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //align:{                                     |

      namespace EON{
        template<> quats& quats::align( const vec3s& up, const vec3s& norm ){
          return *this;
        }
      }

    //}:                                          |
  //}:                                            |
  //Matrices:{                                    |
    //vec4x4s:{                                    |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Matrices
  #pragma mark -
#endif

        namespace EON{
          template<> vec4x4s& vec4x4s::operator*=( const vec4x4s& B ){
            return *this;
          }
        }

        //}:                                      |
      //}:                                        |
      //transpose:{                               |

        namespace EON{
          template<> void vec4x4s::transpose(){
          }
        }

      //}:                                        |
      //setPerspectiveV2C:{                       |

        namespace EON{
          template<> void vec4x4s::setPerspectiveV2C( const i16& degFovY, const i16& aspect, const i16& zn, const i16& zf ){
          }
        }

      //}:                                        |
      //setAxisIdentity:{                         |

        namespace EON{
          template<> void vec4x4s::setAxisIdentity(){
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
          template<> void vec4x4s::setTranslation( const i16& x, const i16& y, const i16& z ){
            XX = 1; XY = 0; XZ = 0; XW = 0;
            YX = 0; YY = 1; YZ = 0; YW = 0;
            ZX = 0; ZY = 0; ZZ = 1; ZW = 0;
            WX = x; WY = y; WZ = z; WW = 1;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec4x4s::setScale( const i16& x, const i16& y, const i16& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

        namespace EON{
          template<> void vec4x4s::setScale( const i16& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec4x4s::setRotX( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec4x4s::setRotY( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec4x4s::setRotZ( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec4x4s::removeTranslation(){
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec4x4s::removeScale(){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec4x4s vec4x4s::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec4x4s::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x4s:{                                   |
      //[converter]:{                             |
        //vec4x4s:{                               |

          namespace EON{
            template<> template<> vec4x4s vec3x4s::as<vec4x4s>()const{
              return{};
            }
          }

        //}:                                      |
        //quats:{                                 |

          namespace EON{
            template<> template<> quats vec3x4s::as<quats>()const{
              return{};
            }
          }

        //}:                                      |
        //vec3x3s:{                               |

          namespace EON{
            template<> template<> vec3x3s vec3x4s::as<vec3x3s>()const{
              return{};
            }
          }

        //}:                                      |
        //pt3s:{                                  |

          namespace EON{
            template<> template<> pt3s vec3x4s::as<pt3s>()const{
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
            template<> vec3x4s& vec3x4s::operator*=( const vec3x4s& B ){
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
      //as<>:{                                    |

        namespace EON{
          template<> template<> vec3x3s vec4x4s::as<vec3x3s>()const{
            return{};
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec3x4s::removeTranslation(){
            WX = WY = WZ = 0;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec3x4s::setTranslation( const i16& x, const i16& y, const i16& z ){
            XX = 1; YX = 0; ZX = 0; WX = x;
            XY = 0; YY = 1; ZY = 0; WY = y;
            XZ = 0; YZ = 0; ZZ = 1; WZ = z;
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec3x4s::removeScale(){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x4s::setScale( const i16& x, const i16& y, const i16& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
          }
        }

        namespace EON{
          template<> void vec3x4s::setScale( const i16& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x4s::setRotX( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x4s::setRotY( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x4s::setRotZ( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec3x4s vec3x4s::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec3x4s::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x3s:{                                   |
      //setAxisAngle:{                            |

        namespace EON{
          template<> void vec3x3s::setAxisAngle( const vec3s& axis, const i16& angle ){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x3s::setScale( const i16& x, const i16& y, const i16& z ){
            XX = x; YX = 0; ZX = 0;
            XY = 0; YY = y; ZY = 0;
            XZ = 0; YZ = 0; ZZ = z;
          }
          template<> void vec3x3s::setScale( const i16& s ){
            XX = s; YX = 0; ZX = 0;
            XY = 0; YY = s; ZY = 0;
            XZ = 0; YZ = 0; ZZ = s;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x3s::setRotX( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x3s::setRotY( const i16& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x3s::setRotZ( const i16& angleInDeg ){
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
        template<> bool frustums::intersects( const pt3s& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustums::intersects( const spheres& S )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustums::intersects( const aabb3s& B )const{
          return false;
        }
      }

    //}:                                          |
    //set:{                                       |

      namespace EON{
        template<> void frustums::set( const vec4x4s& inW2C ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //Vector4:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec4s vec4s::operator*( const vec4x4s& M )const{
            return{};
          }
          template<> vec4s vec4s::operator*( const vec3x3s& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector3:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec3s vec3s::operator*( const vec4x4s& M )const{
            return{};
          }
          template<> vec3s vec3s::operator*( const vec3x3s& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector2:{                                   |
      //[operators]:{                             |
        //vec2s:{                                 |

          namespace EON{
            template<> vec2s  vec2s::operator+ ( const vec2s& v )const{ return vec2s( x+v.x, y+v.y ); }
            template<> vec2s  vec2s::operator- ( const vec2s& v )const{ return vec2s( x-v.x, y-v.y ); }
            template<> vec2s  vec2s::operator/ ( const vec2s& v )const{ return vec2s( x/v.x, y/v.y ); }
            template<> vec2s  vec2s::operator* ( const vec2s& v )const{ return vec2s( x*v.x, y*v.y ); }
            template<> vec2s& vec2s::operator+=( const vec2s& v ){ x+=v.x; y+=v.y; return*this; }
            template<> vec2s& vec2s::operator-=( const vec2s& v ){ x-=v.x; y-=v.y; return*this; }
            template<> vec2s& vec2s::operator/=( const vec2s& v ){ x/=v.x; y/=v.y; return*this; }
            template<> vec2s& vec2s::operator*=( const vec2s& v ){ x*=v.x; y*=v.y; return*this; }
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
        template<> vec4x4s qst3s::get4x4()const{
          return{};
        }
        template<> vec3x4s qst3s::get3x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3s qst3s::get3x3()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4s qst3s::get4x4RT()const{
          return{};
        }
        template<> vec3x4s qst3s::get3x4RT()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4s qst3s::get4x4ST()const{
          return{};
        }
        template<> vec3x4s qst3s::get3x4ST()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4s qst3s::get4x4T()const{
          return{};
        }
        template<> vec3x4s qst3s::get3x4T()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4s qst3s::get4x4R()const{
          return{};
        }
        template<> vec3x4s qst3s::get3x4R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3s qst3s::get3x3R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4s qst3s::get4x4S()const{
          return{};
        }
        template<> vec3x4s qst3s::get3x4S()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3s qst3s::get3x3S()const{
          return{};
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void qst3s::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //Multiply:{                                |

        namespace EON{
          template<> pt3s qst3s::operator*( const pt3s& P )const{
            return{};
          }
        }

        namespace EON{
          template<> void qst3s::operator*=( const qst3s& L2W ){
          }
        }

      //}:                                        |
      //Array:{                                   |

        namespace EON{
          template<> i16 qst3s::operator[]( const u32 i )const{
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
  //aabb3s:{                                      |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Bounding volumes
  #pragma mark -
#endif

      //http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
      namespace EON{
        template<> bool aabb3s::intersects( const ray3s& inRay, i16& outDst )const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb3s::contains( const pt3s& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3s::contains( const aabb3s& B )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3s::contains( const triangles& t )const{
          return false;
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb3s::empty()const{
          return false;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb3s::valid()const{
          return true;
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb3s::reset(){
        }
      }

    //}:                                          |
  //}:                                            |
  //aabb2s:{                                      |
    //scaledToClip:{                              |

      namespace EON{
        template<> aabb2s aabb2s::scaledToClip( const i16& scale )const{
          return{};
        }
      }

    //}:                                          |
    //scaleToClip:{                               |

      namespace EON{
        template<> void aabb2s::scaleToClip( const i16& scale ){
        }
      }

    //}:                                          |
    //scaleBack:{                                 |

      namespace EON{
        template<> void aabb2s::scaleBack(){
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb2s::empty()const{
          return true;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb2s::valid()const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb2s::contains( const pt2s& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2s::contains( const triangles& t )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2s::contains( const aabb2s& B )const{
          return false;
        }
      }

    //}:                                          |
    //intersects:{                                |

      namespace EON{
        template<> bool aabb2s::intersects( const ray2s& r )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2s::intersects( const aabb2s& b )const{
          return false;
        }
      }

    //}:                                          |
    //clipAgainst:{                               |

      namespace EON{
        template<> bool aabb2s::clipAgainst( const aabb2s& b ){
          return false;
        }
      }

    //}:                                          |
    //circle:{                                    |

      namespace EON{
        template<> circles aabb2s::circle()const{
          return{};
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb2s::reset(){
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

    namespace{
      std::mt19937 kGen32;
    }

    template<> i16 e_rand<i16>( const i16 start, const i16 end ){
      return 0;
    }

    template<> i16 e_rand<i16>(){
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
        template<> i16 rgbas::multiply( const i16& base, const i16& blend ){
          return( base*blend );
        }
      }

    //}:                                          |
    //lighten:{                                   |

      namespace EON{
        template<> i16 rgbas::lighten( const i16& base, const i16& blend ){
          return max( base, blend );
        }
      }

    //}:                                          |
    //darken:{                                    |

      namespace EON{
        template<> i16 rgbas::darken( const i16& base, const i16& blend ){
          return min( base, blend );
        }
      }

    //}:                                          |
    //average:{                                   |

      namespace EON{
        template<> i16 rgbas::average( const i16& base, const i16& blend ){
          return( base+blend )*kHalf;
        }
      }

    //}:                                          |
    //linearDodge:{                               |

      namespace EON{
        template<> i16 rgbas::linearDodge( const i16& base, const i16& blend ){
          return self::saturate( base+blend );
        }
      }

    //}:                                          |
    //linearBurn:{                                |

      namespace EON{
        template<> i16 rgbas::linearBurn( const i16& base, const i16& blend ){
          return self::saturate( base-blend );
        }
      }

    //}:                                          |
    //difference:{                                |

      namespace EON{
        template<> i16 rgbas::difference( const i16& base, const i16& blend ){
          return abs( base-blend );
        }
      }

    //}:                                          |
    //negation:{                                  |

      namespace EON{
        template<> i16 rgbas::negation( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          return( k1 - abs( k1 - base - blend ));
        }
      }

    //}:                                          |
    //screen:{                                    |

      namespace EON{
        template<> i16 rgbas::screen( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          return 1.f-(( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //exclusion:{                                 |

      namespace EON{
        template<> i16 rgbas::exclusion( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          return( base+blend-k2*base*blend );
        }
      }

    //}:                                          |
    //overlay:{                                   |

      namespace EON{
        template<> i16 rgbas::overlay( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          return( base <kHalf ) ?( 2.f * base * blend ) :( 1.f - 2.f*( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //hardLight:{                                 |

      namespace EON{
        template<> i16 rgbas::hardLight( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          return( blend <kHalf ) ?( k2 * blend * base ) :( k1 - k2 *( k1 - blend ) *( k1 - base ));
        }
      }

    //}:                                          |
    //softLight:{                                 |

      namespace EON{
        template<> i16 rgbas::softLight( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          if( blend < kHalf ){
            return( k2 * base * blend + base*base *( k1 - k2 * blend ));
          }
          return( sqrt( base ) * ( k2 * blend - k1 ) + k2 * base * ( k1 - blend ));
        }
      }

    //}:                                          |
    //softDodge:{                                 |

      namespace EON{
        template<> i16 rgbas::softDodge( const i16& base, const i16& blend ){
          return(( base + blend ) < 1.f ) ? (( base * .5f ) / ( 1.f - blend )) : ( 1.f - (( .5f *( 1.f - blend )) / base ));
        }
      }

    //}:                                          |
    //colorDodge:{                                |

      namespace EON{
        template<> i16 rgbas::colorDodge( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //colorBurn:{                                 |

      namespace EON{
        template<> i16 rgbas::colorBurn( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          if( blend > k0 ){
            return max(( k1 -(( k1 - base ) / blend )), k0 );
          }
          return blend;
        }
      }

    //}:                                          |
    //linearLight:{                               |

      namespace EON{
        template<> i16 rgbas::linearLight( const i16& base, const i16& blend ){
          return( blend < kHalf ) ? linearBurn( base, k2*blend ) : linearDodge( base, k2*( blend-kHalf ));
        }
      }

    //}:                                          |
    //vividLight:{                                |

      namespace EON{
        template<> i16 rgbas::vividLight( const i16& base, const i16& blend ){
          return( blend < kHalf ) ? colorBurn( base,( k2 * blend )) : colorDodge( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //pinLight:{                                  |

      namespace EON{
        template<> i16 rgbas::pinLight( const i16& base, const i16& blend ){
          return( blend < kHalf ) ? darken( base,( k2 * blend )) : lighten( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //hardMix:{                                   |

      namespace EON{
        template<> i16 rgbas::hardMix( const i16& base, const i16& blend ){
          return( vividLight( base, blend ) < kHalf ) ? k0 : k1;
        }
      }

    //}:                                          |
    //reflection:{                                |

      namespace EON{
        template<> i16 rgbas::reflect( const i16& in_fBase, const i16& in_fBlend ){
          const i16& blend = self::saturate( in_fBlend );
          const i16& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base*base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //glow:{                                      |

      namespace EON{
        template<> i16 rgbas::glow( const i16& base, const i16& blend ){
          if( base != k1 ){
            return min( blend*blend/( k1 - base ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //pheonix:{                                   |

      namespace EON{
        template<> i16 rgbas::phoenix( const i16& base, const i16& blend ){
          return min( base, blend ) - max( base, blend ) + k1;
        }
      }

    //}:                                          |
    //setHSV:{                                    |

      namespace EON{
        template<> void rgbas::setHSV( const i16& H, const i16& S, const i16& V ){
        }
      }

    //}:                                          |
    //hsv:{                                       |

      namespace EON{
        //http://www.tech-faq.com/hsv.html
        template<> hsvas rgbas::hsv()const{
          return{};
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    namespace EON{
      template<> void rgbas::set( const u32 inBGRA ){
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
      template<> bool triangles::intersects( const vec4x4s& L2W, const ray3s& ray, pt3s& hit, vec3s& norm )const{
        return false;
      }
      template<> bool triangles::intersects( const qst3s& L2W, const ray3s& ray, pt3s& hit, vec3s& norm )const{
        return false;
      }
      template<> bool triangles::intersects( const ray3s& ray, pt3s& hit, vec3s& norm )const{
        return false;
      }
    }

  //}:                                            |
  //toBounds2D:{                                  |

    namespace EON{
      template<> aabb2s triangles::toBounds2D()const{
        return{};
      }
    }

  //}:                                            |
  //toBounds:{                                    |

    namespace EON{
      template<> aabb3s triangles::toBounds()const{
        return{};
      }
    }

  //}:                                            |
  //draw:{                                        |

    namespace EON{
      template<> bool triangles::draw( IRasterizer& rasterizer, const bool bTestPrimitive )const{
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
          template<> i16::Quadtree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Quadtree: methods -
#endif

          namespace EON{
            template<> i16::Quadtree::Leaf::Result i16::Quadtree::Leaf::intersects(
                  const Quadtree* pQuadtree
                , const vec4x4s& L2W
                , const Ray3& r )const{
              return nullptr;
            }
            template<> i16::Quadtree::Leaf::Result i16::Quadtree::Leaf::intersects(
                  const Quadtree* pQuadtree
                , const vec4x4s& L2W
                , const Ray3& r
                , const u32 page )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i16::Quadtree::Leaf::Status i16::Quadtree::Leaf::addPolygon( Quadtree* pQuadtree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i16::Quadtree::Leaf::clear(){
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
          template<> i16::Octree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Octree: methods -
#endif

          namespace EON{
            template<> i16::Octree::Leaf::Result i16::Octree::Leaf::intersects( const Octree* pOctree, const vec4x4s& L2W, const Ray3& r )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i16::Octree::Leaf::Status i16::Octree::Leaf::addPolygon( Octree* pOctree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i16::Octree::Leaf::clear(){
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
