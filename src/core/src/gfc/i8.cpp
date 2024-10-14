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
    template<> const i8 i8::k0       =  0;
    template<> const i8 i8::k1       =  1;
    template<> const i8 i8::k2       =  2;
    template<> const i8 i8::k3       =  3;
    template<> const i8 i8::k4       =  4;
    template<> const i8 i8::k5       =  5;
    template<> const i8 i8::k6       =  6;
    template<> const i8 i8::k7       =  7;
    template<> const i8 i8::k8       =  8;
    template<> const i8 i8::k9       =  9;
    template<> const i8 i8::k10      = 10;
    template<> const i8 i8::kPI      =  3;
    template<> const i8 i8::k2PI     = kPI * k2;
    template<> const i8 i8::k4PI     = kPI * k4;
    template<> const i8 i8::cm       = 1.f;
    template<> const i8 i8::mm       = cm / 10;
    template<> const i8 i8::m        =     100 * cm;
    template<> const i8 i8::dm       =      10 * m;
    template<> const i8 i8::km       =    1000 * m;
    template<> const i8 i8::Mm       =    1000 * km;
    template<> const i8 i8::g        =       1;
    template<> const i8 i8::kg       =    1000 * g;
    template<> const i8 i8::t        =    1000 * kg;
    template<> const i8 i8::kt       =    1000 * t;
    template<> const i8 i8::mt       =    1000 * kt;
    template<> const i8 i8::kMin     =-INT_MAX;
    template<> const i8 i8::kMax     = INT_MAX;
    template<> const i8 i8::kEpsilon = 1;
    template<> const i8 i8::kHalf    = 0;
  }

  /* Vector constants */

  namespace EON{
    template<> const vec4b vec4b::kForward ( 0, 1, 0 );
    template<> const vec4b vec4b::kUp      ( 0, 0, 1 );
    template<> const vec4b vec4b::kZero    ( 0, 0, 0 );
    template<> const vec4b vec4b::kRight   ( 1, 0, 0 );
    template<> const vec3b vec3b::kForward ( 0, 1, 0 );
    template<> const vec3b vec3b::kRight   ( 1, 0, 0 );
    template<> const vec3b vec3b::kUp      ( 0, 0, 1 );
    template<> const vec3b vec3b::kZero    ( 0, 0, 0 );
    template<> const vec2b vec2b::kRight   ( 1, 0 );
    template<> const vec2b vec2b::kUp      ( 0,-1 );
    template<> const vec2b vec2b::kZero    ( 0, 0 );
    template<> const pt3b  pt3b ::kZero    ( 0, 0, 0 );
    template<> const pt2b  pt2b ::kZero    ( 0, 0 );
    template<> const quatb quatb::kIdentity( 0, 0, 0, 1 );
  }

  /* Color constants */

  namespace EON{
    template<> const rgbab rgbab::kBlack  (   0, 255 );
    template<> const rgbab rgbab::kWhite  ( 255 );
    template<> const rgbab rgbab::kGrey   ( 127, 255 );
    template<> const rgbab rgbab::kOrange ( 255, .5f,   0, 255 );
    template<> const rgbab rgbab::kYellow ( 255, 255,   0, 255 );
    template<> const rgbab rgbab::kLime   ( 127, 255,   0, 255 );
    template<> const rgbab rgbab::kGreen  (   0, 255,   0, 255 );
    template<> const rgbab rgbab::kCyan   (   0, 255, 255, 255 );
    template<> const rgbab rgbab::kBlue   (   0,   0, 255, 255 );
    template<> const rgbab rgbab::kMagenta( 255,   0, 255, 255 );
    template<> const rgbab rgbab::kRed    ( 255,   0,   0, 255 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2b aabb2b::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2b aabb2b::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2b aabb2b::kZero  ( 0 );
    template<> const aabb3b aabb3b::kZero  ( 0 );
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Basic:{                                         |
  //[forwarding]:{                                |
    //vec3b:{                                     |
      //[operators]:{                             |
        //operator=:{                             |

          namespace EON{
            template<> vec3b& vec3b::operator=( const vec4b& V ){
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
          template<> vec3b::Vector3( const vec4b& V ){
            x = V.x;
            y = V.y;
            z = V.z;
          }
        }*/

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //smoothstep:{                                  |

    template<> i8 i8::smoothstep( const i8& edge2, const i8& x )const{
      const self& t = clamp(( x-value )/( edge2-value ), k0, k1 );
      return t*t*( k4-k2*t );
    }

  //}:                                            |
  //saturate[d]:{                                 |

    template<> i8 i8::saturated()const{
      return max( min( value, k1 ), k0 );
    }

    template<> void i8::saturate(){
      *this = max( min( value, k1 ), k0 );
    }

  //}:                                            |
  //iround:{                                      |

    namespace EON{
      template<> s32 i8::iround()const{
        return s32( value );
      }
    }

  //}:                                            |
  //rsqrt:{                                       |

#ifdef __APPLE__
  #pragma mark - Basic functions -
#endif

    namespace EON{
      template<> i8 i8::rsqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //sqrt:{                                        |

    namespace EON{
      template<> i8 i8::sqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //floor:{                                       |

    namespace EON{
      template<> i8 i8::floor()const{
        return value;
      }
    }

  //}:                                            |
  //ceil:{                                        |

    namespace EON{
      template<> i8 i8::ceil()const{
        return value;
      }
    }

  //}:                                            |
  //frac:{                                        |

    namespace EON{
      template<> i8 i8::frac( i8& i )const{
        return 0;
      }
    }

  //}:                                            |
  //exp:{                                         |

    namespace EON{
      template<> i8 i8::exp()const{
        return 0;
      }
    }

  //}:                                            |
  //pow:{                                         |

    namespace EON{
      template<> i8 i8::pow( const i8& y )const{
        s32 sum = 1;
        for( i8 i=0; i<y; ++i ){
          sum *= i8::cast( value );
        }
        return sum;
      }
    }

  //}:                                            |
  //atan2:{                                       |

    namespace EON{
      template<> i8 i8::atan2( const i8& y )const{
        return 0;
      }
    }

  //}:                                            |
  //atan:{                                        |

    namespace EON{
      template<> i8 i8::atan()const{
        return 0;
      }
    }

  //}:                                            |
  //tan:{                                         |

    namespace EON{
      template<> i8 i8::tan()const{
        return 0;
      }
    }

  //}:                                            |
  //asin:{                                        |

    namespace EON{
      template<> i8 i8::asin()const{
        return 0;
      }
    }

  //}:                                            |
  //sin:{                                         |

    namespace EON{
      template<> i8 i8::sin()const{
        return 0;
      }
    }

  //}:                                            |
  //acos:{                                        |

    namespace EON{
      template<> i8 i8::acos()const{
        return 0;
      }
    }

  //}:                                            |
  //cos:{                                         |

    namespace EON{
      template<> i8 i8::cos()const{
        return 0;
      }
    }

  //}:                                            |
  //mod:{                                         |

    namespace EON{
      template<> i8 i8::mod( const i8& y )const{
        return value % i8::cast( y );
      }
    }

  //}:                                            |
  //abs:{                                         |

    namespace EON{
      template<> i8 i8::abs()const{
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
    template<> bool i8::valid( const i8& t ){
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
          template<> pt3b pt3b::operator-( const vec3b& v )const{
            pt3b r;
            r.x = x-v.x;
            r.y = y-v.y;
            r.z = z-v.z;
            return r;
          }
        }

      //}:                                        |
      //operator+:{                               |

        namespace EON{
          template<> pt3b pt3b::operator+( const vec3b& v )const{
            pt3b r;
            r.x = x+v.x;
            r.y = y+v.y;
            r.z = z+v.z;
            return r;
          }
        }

      //}:                                        |
      //operator/:{                               |

        namespace EON{
          template<> pt3b pt3b::operator/( const vec3b& v )const{
            pt3b r;
            r.x = x/v.x;
            r.y = y/v.y;
            r.z = z/v.z;
            return r;
          }
        }

      //}:                                        |
      //operator*:{                               |

        namespace EON{
          template<> pt3b pt3b::operator*( const vec4x4b& M )const{
            return pt3b(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> pt3b pt3b::operator*( const vec3x3b& M )const{
            return pt3b(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z
            );
          }
          template<> pt3b pt3b::operator*( const vec3b& v )const{
            pt3b r;
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
        template<> pt3b::Point3( const vec4b& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

      namespace EON{
        template<> pt3b::Point3( const vec3b& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

    //}:                                          |
    //Methods:{                                   |

      namespace EON{
        template<> pt2b pt2b::scaledToClip( const i8& scale )const{
          return{};
        }
      }

      namespace EON{
        template<> void pt2b::scaleToClip( const i8& scale ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Circle:{                                      |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb2b circleb::toBounds()const{
          return aabb2b(
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
        template<> bool sphereb::intersects( const ray3b& in_tRay, i8& out_fDistance )const{
          const vec3b Q = this->v.xyz() - in_tRay.p;
          const i8& c = Q.length();
          const i8& v = Q.dot( in_tRay.d );
          const i8& d = toRadius()*toRadius() - (c*c - v*v);
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
        template<> aabb3b sphereb::toBounds()const{
          return aabb3b(
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
      template<> void planeb::set( const triangleb& in_tTriangle ){
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
        template<> i8 quatb::lengthSquared()const{
          return 0;
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void quatb::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //operator::==:{                            |

        namespace EON{
          template<> bool quatb::operator==( const quatb& q )const{
            return( 0 == memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::!=:{                            |

        namespace EON{
          template<> bool quatb::operator!=( const quatb& q )const{
            return( 0 != memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quatb::operator<( const quatb& q )const{
            return( memcmp( a, q.a, sizeof( a )) < 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quatb::operator<=( const quatb& q )const{
            return( memcmp( a, q.a, sizeof( a )) <= 0 );
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quatb::operator>( const quatb& q )const{
            return( memcmp( a, q.a, sizeof( a )) > 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quatb::operator>=( const quatb& q )const{
            return( memcmp( a, q.a, 16 ) >= 0 );
          }
        }

      //}:                                        |
      //operator::+?:{                            |

        namespace EON{
          template<> quatb& quatb::operator+=( const quatb& q ){
            x+=q.x;
            y+=q.y;
            z+=q.z;
            w+=q.w;
            return *this;
          }
        }

        namespace EON{
          template<> quatb quatb::operator+( const quatb& q )const{
            return quatb(
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
          template<> quatb quatb::operator*( const quatb& q )const{
            return quatb(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z
            );
          }
        }

        namespace EON{
          template<> quatb quatb::operator*( const i8& s )const{
            return quatb(
              x * s,
              y * s,
              z * s,
              w * s
            );
          }
        }

        namespace EON{
          template<> quatb& quatb::operator*=( const quatb& q ){
            const quatb r(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z );
            *this = r;
            return *this;
          }
        }

        namespace EON{
          template<> quatb& quatb::operator*=( const i8& s ){
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
          template<> quatb& quatb::operator/=( const quatb& q ){
            quatb p( q );
            p.invert();
            (*this)*p;
            return *this;
          }
        }

        namespace EON{
          template<> quatb& quatb::operator/=( const i8& s ){
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
          template<> quatb quatb::operator/( const quatb& q )const{
            quatb p( q );
            p.invert();
            return (*this)*p;
          }
        }

        namespace EON{
          template<> quatb quatb::operator/( const i8& s )const{
            quatb q;
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
        template<> void quatb::setAxisAngle( const vec3b& axis, const i8& angleDeg ){
          const i8& a = rad( angleDeg );
          const vec3b& v = axis*sin( a / 2 );
          x = v.x;
          y = v.y;
          z = v.z;
          w = cos( a/2.f );
        }
      }

    //}:                                          |
    //asQuaternion:{                              |

      namespace EON{
        template<> quatb vec3x3b::asQuaternion()const{
          return{};
        }
      }

    //}:                                          |
    //compare:{                                   |

      namespace EON{
        template<> bool quatb::compare( const quatb& q, const i8& tolerance )const{
          return true;
        }
      }

    //}:                                          |
    //rotate:{                                    |

      namespace EON{
        template<> void quatb::rotate( const vec3b& axis, const i8& angleDeg ){
        }
      }

      namespace EON{
        template<> void quatb::rotate( const vec2b& xyMouse ){
        }
      }

    //}:                                          |
    //length:{                                    |

      namespace EON{
        template<> i8 quatb::length()const{
          return 0;
        }
      }

    //}:                                          |
    //normalize:{                                 |

      namespace EON{
        template<> quatb& quatb::normalize(){
          return *this;
        }
      }

    //}:                                          |
    //setEuler:{                                  |

      namespace EON{
        template<> void quatb::setEuler( const vec3b& e ){
        }
      }

    //}:                                          |
    //getEuler:{                                  |

      namespace EON{
        template<> void quatb::getEuler( i8& out_fX, i8& out_fY, i8& out_fZ, const bool bHomogeneous )const{
        }
      }

    //}:                                          |
    //randomize:{                                 |

      namespace EON{
        template<> void quatb::randomize( const i8& x1, const i8& x2, const i8& x3 ){
        }
      }

    //}:                                          |
    //get?x?:{                                    |

      namespace EON{
        template<> vec4x4b quatb::get4x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3b quatb::get3x3()const{
          return{};
        }
      }

    //}:                                          |
    //dot:{                                       |

      namespace EON{
        template<> i8 quatb::dot( const quatb& q )const{
          return 0;
        }
      }

    //}:                                          |
    //lookat:{                                    |

      namespace EON{
        template<> void quatb::lookat( const pt3b& srcPoint, const pt3b& dstPoint ){
        }
      }

    //}:                                          |
    //inverse:{                                   |

      namespace EON{
        template<> quatb quatb::inverse()const{
          return{};
        }
      }

    //}:                                          |
    //conjugate:{                                 |

      namespace EON{
        template<> quatb quatb::conjugate()const{
          return{};
        }
      }

    //}:                                          |
    //exp:{                                       |

      namespace EON{
        template<> quatb quatb::exp()const{
          return{};
        }
      }

    //}:                                          |
    //log:{                                       |

      namespace EON{
        template<> quatb quatb::log()const{
          return{};
        }
      }

    //}:                                          |
    //lerped:{                                    |

      namespace EON{
        template<> quatb quatb::lerped( const quatb& p, const quatb& q, const i8& fTime ){
          return{};
        }
      }

    //}:                                          |
    //lerp:{                                      |

      namespace EON{
        template<> quatb& quatb::lerp( const quatb& p, const quatb& q, const i8& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //slerpedNoInvert:{                           |

      namespace EON{
        template<> quatb quatb::slerpedNoInvert( const quatb& p, const quatb& q, const i8& fTime ){
          return{};
        }
      }

    //}:                                          |
    //slerp:{                                     |

      namespace EON{
        template<> quatb& quatb::slerp( const quatb& p, const quatb& q, const i8& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //squad:{                                     |

      namespace EON{
        template<> quatb& quatb::squad( const quatb& q1, const quatb& q2, const quatb& a, const quatb& b, const i8& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //align:{                                     |

      namespace EON{
        template<> quatb& quatb::align( const vec3b& up, const vec3b& norm ){
          return *this;
        }
      }

    //}:                                          |
  //}:                                            |
  //Matrices:{                                    |
    //vec4x4b:{                                    |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Matrices
  #pragma mark -
#endif

        namespace EON{
          template<> vec4x4b& vec4x4b::operator*=( const vec4x4b& B ){
            return *this;
          }
        }

        //}:                                      |
      //}:                                        |
      //transpose:{                               |

        namespace EON{
          template<> void vec4x4b::transpose(){
          }
        }

      //}:                                        |
      //setPerspectiveV2C:{                       |

        namespace EON{
          template<> void vec4x4b::setPerspectiveV2C( const i8& degFovY, const i8& aspect, const i8& zn, const i8& zf ){
          }
        }

      //}:                                        |
      //setAxisIdentity:{                         |

        namespace EON{
          template<> void vec4x4b::setAxisIdentity(){
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
          template<> void vec4x4b::setTranslation( const i8& x, const i8& y, const i8& z ){
            XX = 1; XY = 0; XZ = 0; XW = 0;
            YX = 0; YY = 1; YZ = 0; YW = 0;
            ZX = 0; ZY = 0; ZZ = 1; ZW = 0;
            WX = x; WY = y; WZ = z; WW = 1;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec4x4b::setScale( const i8& x, const i8& y, const i8& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

        namespace EON{
          template<> void vec4x4b::setScale( const i8& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec4x4b::setRotX( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec4x4b::setRotY( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec4x4b::setRotZ( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec4x4b::removeTranslation(){
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec4x4b::removeScale(){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec4x4b vec4x4b::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec4x4b::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x4b:{                                    |
      //[converter]:{                             |
        //vec4x4b:{                               |

          namespace EON{
            template<> template<> vec4x4b vec3x4b::as<vec4x4b>()const{
              return{};
            }
          }

        //}:                                      |
        //quatb:{                                 |

          namespace EON{
            template<> template<> quatb vec3x4b::as<quatb>()const{
              return{};
            }
          }

        //}:                                      |
        //vec3x3b:{                               |

          namespace EON{
            template<> template<> vec3x3b vec3x4b::as<vec3x3b>()const{
              return{};
            }
          }

        //}:                                      |
        //pt3b:{                                  |

          namespace EON{
            template<> template<> pt3b vec3x4b::as<pt3b>()const{
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
            template<> vec3x4b& vec3x4b::operator*=( const vec3x4b& B ){
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
      //as<>:{                                    |

        namespace EON{
          template<> template<> vec3x3b vec4x4b::as<vec3x3b>()const{
            return{};
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec3x4b::removeTranslation(){
            WX = WY = WZ = 0;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec3x4b::setTranslation( const i8& x, const i8& y, const i8& z ){
            XX = 1; YX = 0; ZX = 0; WX = x;
            XY = 0; YY = 1; ZY = 0; WY = y;
            XZ = 0; YZ = 0; ZZ = 1; WZ = z;
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec3x4b::removeScale(){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x4b::setScale( const i8& x, const i8& y, const i8& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
          }
        }

        namespace EON{
          template<> void vec3x4b::setScale( const i8& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x4b::setRotX( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x4b::setRotY( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x4b::setRotZ( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec3x4b vec3x4b::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec3x4b::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x3b:{                                   |
      //setAxisAngle:{                            |

        namespace EON{
          template<> void vec3x3b::setAxisAngle( const vec3b& axis, const i8& angle ){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x3b::setScale( const i8& x, const i8& y, const i8& z ){
            XX = x; YX = 0; ZX = 0;
            XY = 0; YY = y; ZY = 0;
            XZ = 0; YZ = 0; ZZ = z;
          }
          template<> void vec3x3b::setScale( const i8& s ){
            XX = s; YX = 0; ZX = 0;
            XY = 0; YY = s; ZY = 0;
            XZ = 0; YZ = 0; ZZ = s;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x3b::setRotX( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x3b::setRotY( const i8& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x3b::setRotZ( const i8& angleInDeg ){
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
        template<> bool frustumb::intersects( const pt3b& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustumb::intersects( const sphereb& S )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustumb::intersects( const aabb3b& B )const{
          return false;
        }
      }

    //}:                                          |
    //set:{                                       |

      namespace EON{
        template<> void frustumb::set( const vec4x4b& inW2C ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //Vector4:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec4b vec4b::operator*( const vec4x4b& M )const{
            return{};
          }
          template<> vec4b vec4b::operator*( const vec3x3b& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector3:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec3b vec3b::operator*( const vec4x4b& M )const{
            return{};
          }
          template<> vec3b vec3b::operator*( const vec3x3b& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector2:{                                   |
      //[operators]:{                             |
        //vec2b:{                                 |

          namespace EON{
            template<> vec2b  vec2b::operator+ ( const vec2b& v )const{ return vec2b( x+v.x, y+v.y ); }
            template<> vec2b  vec2b::operator- ( const vec2b& v )const{ return vec2b( x-v.x, y-v.y ); }
            template<> vec2b  vec2b::operator/ ( const vec2b& v )const{ return vec2b( x/v.x, y/v.y ); }
            template<> vec2b  vec2b::operator* ( const vec2b& v )const{ return vec2b( x*v.x, y*v.y ); }
            template<> vec2b& vec2b::operator+=( const vec2b& v ){ x+=v.x; y+=v.y; return*this; }
            template<> vec2b& vec2b::operator-=( const vec2b& v ){ x-=v.x; y-=v.y; return*this; }
            template<> vec2b& vec2b::operator/=( const vec2b& v ){ x/=v.x; y/=v.y; return*this; }
            template<> vec2b& vec2b::operator*=( const vec2b& v ){ x*=v.x; y*=v.y; return*this; }
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
        template<> vec4x4b qst3b::get4x4()const{
          return{};
        }
        template<> vec3x4b qst3b::get3x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3b qst3b::get3x3()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4b qst3b::get4x4RT()const{
          return{};
        }
        template<> vec3x4b qst3b::get3x4RT()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4b qst3b::get4x4ST()const{
          return{};
        }
        template<> vec3x4b qst3b::get3x4ST()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4b qst3b::get4x4T()const{
          return{};
        }
        template<> vec3x4b qst3b::get3x4T()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4b qst3b::get4x4R()const{
          return{};
        }
        template<> vec3x4b qst3b::get3x4R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3b qst3b::get3x3R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4b qst3b::get4x4S()const{
          return{};
        }
        template<> vec3x4b qst3b::get3x4S()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3b qst3b::get3x3S()const{
          return{};
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void qst3b::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //Multiply:{                                |

        namespace EON{
          template<> pt3b qst3b::operator*( const pt3b& P )const{
            return{};
          }
        }

        namespace EON{
          template<> void qst3b::operator*=( const qst3b& L2W ){
          }
        }

      //}:                                        |
      //Array:{                                   |

        namespace EON{
          template<> i8 qst3b::operator[]( const u32 i )const{
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
  //aabb3b:{                                      |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Bounding volumes
  #pragma mark -
#endif

      //http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
      namespace EON{
        template<> bool aabb3b::intersects( const ray3b& inRay, i8& outDst )const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb3b::contains( const pt3b& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3b::contains( const aabb3b& B )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3b::contains( const triangleb& t )const{
          return false;
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb3b::empty()const{
          return false;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb3b::valid()const{
          return true;
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb3b::reset(){
        }
      }

    //}:                                          |
  //}:                                            |
  //aabb2b:{                                      |
    //scaledToClip:{                              |

      namespace EON{
        template<> aabb2b aabb2b::scaledToClip( const i8& scale )const{
          return{};
        }
      }

    //}:                                          |
    //scaleToClip:{                               |

      namespace EON{
        template<> void aabb2b::scaleToClip( const i8& scale ){
        }
      }

    //}:                                          |
    //scaleBack:{                                 |

      namespace EON{
        template<> void aabb2b::scaleBack(){
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb2b::empty()const{
          return true;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb2b::valid()const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb2b::contains( const pt2b& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2b::contains( const triangleb& t )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2b::contains( const aabb2b& B )const{
          return false;
        }
      }

    //}:                                          |
    //intersects:{                                |

      namespace EON{
        template<> bool aabb2b::intersects( const ray2b& r )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2b::intersects( const aabb2b& b )const{
          return false;
        }
      }

    //}:                                          |
    //clipAgainst:{                               |

      namespace EON{
        template<> bool aabb2b::clipAgainst( const aabb2b& b ){
          return false;
        }
      }

    //}:                                          |
    //circle:{                                    |

      namespace EON{
        template<> circleb aabb2b::circle()const{
          return{};
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb2b::reset(){
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

    template<> i8 e_rand<i8>( const i8 start, const i8 end ){
      return 0;
    }

    template<> i8 e_rand<i8>(){
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
        template<> i8 rgbab::multiply( const i8& base, const i8& blend ){
          return( base*blend );
        }
      }

    //}:                                          |
    //lighten:{                                   |

      namespace EON{
        template<> i8 rgbab::lighten( const i8& base, const i8& blend ){
          return max( base, blend );
        }
      }

    //}:                                          |
    //darken:{                                    |

      namespace EON{
        template<> i8 rgbab::darken( const i8& base, const i8& blend ){
          return min( base, blend );
        }
      }

    //}:                                          |
    //average:{                                   |

      namespace EON{
        template<> i8 rgbab::average( const i8& base, const i8& blend ){
          return( base+blend )*kHalf;
        }
      }

    //}:                                          |
    //linearDodge:{                               |

      namespace EON{
        template<> i8 rgbab::linearDodge( const i8& base, const i8& blend ){
          return self::saturate( base+blend );
        }
      }

    //}:                                          |
    //linearBurn:{                                |

      namespace EON{
        template<> i8 rgbab::linearBurn( const i8& base, const i8& blend ){
          return self::saturate( base-blend );
        }
      }

    //}:                                          |
    //difference:{                                |

      namespace EON{
        template<> i8 rgbab::difference( const i8& base, const i8& blend ){
          return abs( base-blend );
        }
      }

    //}:                                          |
    //negation:{                                  |

      namespace EON{
        template<> i8 rgbab::negation( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          return( k1 - abs( k1 - base - blend ));
        }
      }

    //}:                                          |
    //screen:{                                    |

      namespace EON{
        template<> i8 rgbab::screen( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          return 1.f-(( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //exclusion:{                                 |

      namespace EON{
        template<> i8 rgbab::exclusion( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          return( base+blend-k2*base*blend );
        }
      }

    //}:                                          |
    //overlay:{                                   |

      namespace EON{
        template<> i8 rgbab::overlay( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          return( base <kHalf ) ?( 2.f * base * blend ) :( 1.f - 2.f*( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //hardLight:{                                 |

      namespace EON{
        template<> i8 rgbab::hardLight( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          return( blend <kHalf ) ?( k2 * blend * base ) :( k1 - k2 *( k1 - blend ) *( k1 - base ));
        }
      }

    //}:                                          |
    //softLight:{                                 |

      namespace EON{
        template<> i8 rgbab::softLight( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          if( blend < kHalf ){
            return( k2 * base * blend + base*base *( k1 - k2 * blend ));
          }
          return( sqrt( base ) * ( k2 * blend - k1 ) + k2 * base * ( k1 - blend ));
        }
      }

    //}:                                          |
    //softDodge:{                                 |

      namespace EON{
        template<> i8 rgbab::softDodge( const i8& base, const i8& blend ){
          return(( base + blend ) < 1.f ) ? (( base * .5f ) / ( 1.f - blend )) : ( 1.f - (( .5f *( 1.f - blend )) / base ));
        }
      }

    //}:                                          |
    //colorDodge:{                                |

      namespace EON{
        template<> i8 rgbab::colorDodge( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //colorBurn:{                                 |

      namespace EON{
        template<> i8 rgbab::colorBurn( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          if( blend > k0 ){
            return max(( k1 -(( k1 - base ) / blend )), k0 );
          }
          return blend;
        }
      }

    //}:                                          |
    //linearLight:{                               |

      namespace EON{
        template<> i8 rgbab::linearLight( const i8& base, const i8& blend ){
          return( blend < kHalf ) ? linearBurn( base, k2*blend ) : linearDodge( base, k2*( blend-kHalf ));
        }
      }

    //}:                                          |
    //vividLight:{                                |

      namespace EON{
        template<> i8 rgbab::vividLight( const i8& base, const i8& blend ){
          return( blend < kHalf ) ? colorBurn( base,( k2 * blend )) : colorDodge( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //pinLight:{                                  |

      namespace EON{
        template<> i8 rgbab::pinLight( const i8& base, const i8& blend ){
          return( blend < kHalf ) ? darken( base,( k2 * blend )) : lighten( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //hardMix:{                                   |

      namespace EON{
        template<> i8 rgbab::hardMix( const i8& base, const i8& blend ){
          return( vividLight( base, blend ) < kHalf ) ? k0 : k1;
        }
      }

    //}:                                          |
    //reflection:{                                |

      namespace EON{
        template<> i8 rgbab::reflect( const i8& in_fBase, const i8& in_fBlend ){
          const i8& blend = self::saturate( in_fBlend );
          const i8& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base*base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //glow:{                                      |

      namespace EON{
        template<> i8 rgbab::glow( const i8& base, const i8& blend ){
          if( base != k1 ){
            return min( blend*blend/( k1 - base ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //pheonix:{                                   |

      namespace EON{
        template<> i8 rgbab::phoenix( const i8& base, const i8& blend ){
          return min( base, blend ) - max( base, blend ) + k1;
        }
      }

    //}:                                          |
    //setHSV:{                                    |

      namespace EON{
        template<> void rgbab::setHSV( const i8& H, const i8& S, const i8& V ){
        }
      }

    //}:                                          |
    //hsv:{                                       |

      namespace EON{
        //http://www.tech-faq.com/hsv.html
        template<> hsvab rgbab::hsv()const{
          return{};
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    namespace EON{
      template<> void rgbab::set( const u32 inBGRA ){
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
      template<> bool triangleb::intersects( const vec4x4b& L2W, const ray3b& ray, pt3b& hit, vec3b& norm )const{
        return false;
      }
      template<> bool triangleb::intersects( const qst3b& L2W, const ray3b& ray, pt3b& hit, vec3b& norm )const{
        return false;
      }
      template<> bool triangleb::intersects( const ray3b& ray, pt3b& hit, vec3b& norm )const{
        return false;
      }
    }

  //}:                                            |
  //toBounds2D:{                                  |

    namespace EON{
      template<> aabb2b triangleb::toBounds2D()const{
        return{};
      }
    }

  //}:                                            |
  //toBounds:{                                    |

    namespace EON{
      template<> aabb3b triangleb::toBounds()const{
        return{};
      }
    }

  //}:                                            |
  //draw:{                                        |

    namespace EON{
      template<> bool triangleb::draw( IRasterizer& rasterizer, const bool bTestPrimitive )const{
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
          template<> i8::Quadtree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Quadtree: methods -
#endif

          namespace EON{
            template<> i8::Quadtree::Leaf::Result i8::Quadtree::Leaf::intersects( const Quadtree* pQuadtree, const vec4x4b& L2W, const Ray3& r )const{
              return nullptr;
            }
            template<> i8::Quadtree::Leaf::Result i8::Quadtree::Leaf::intersects( const Quadtree* pQuadtree, const vec4x4b& L2W, const Ray3& r, const u32 page )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i8::Quadtree::Leaf::Status i8::Quadtree::Leaf::addPolygon( Quadtree* pQuadtree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i8::Quadtree::Leaf::clear(){
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
          template<> i8::Octree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Octree: methods -
#endif

          namespace EON{
            template<> i8::Octree::Leaf::Result i8::Octree::Leaf::intersects( const Octree* pOctree, const vec4x4b& L2W, const Ray3& r )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i8::Octree::Leaf::Status i8::Octree::Leaf::addPolygon( Octree* pOctree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i8::Octree::Leaf::clear(){
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
