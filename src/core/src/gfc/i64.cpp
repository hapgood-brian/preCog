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
    template<> const i64 i64::k0       =  0;
    template<> const i64 i64::k1       =  1;
    template<> const i64 i64::k2       =  2;
    template<> const i64 i64::k3       =  3;
    template<> const i64 i64::k4       =  4;
    template<> const i64 i64::k5       =  5;
    template<> const i64 i64::k6       =  6;
    template<> const i64 i64::k7       =  7;
    template<> const i64 i64::k8       =  8;
    template<> const i64 i64::k9       =  9;
    template<> const i64 i64::k10      = 10;
    template<> const i64 i64::kPI      =  3;
    template<> const i64 i64::k2PI     = kPI * k2;
    template<> const i64 i64::k4PI     = kPI * k4;
    template<> const i64 i64::cm       = 1.f;
    template<> const i64 i64::mm       = cm / 10;
    template<> const i64 i64::m        =     100 * cm;
    template<> const i64 i64::dm       =      10 * m;
    template<> const i64 i64::km       =    1000 * m;
    template<> const i64 i64::Mm       =    1000 * km;
    template<> const i64 i64::g        =       1;
    template<> const i64 i64::kg       =    1000 * g;
    template<> const i64 i64::t        =    1000 * kg;
    template<> const i64 i64::kt       =    1000 * t;
    template<> const i64 i64::mt       =    1000 * kt;
    template<> const i64 i64::kMin     =-INT_MAX;
    template<> const i64 i64::kMax     = INT_MAX;
    template<> const i64 i64::kEpsilon = 1;
    template<> const i64 i64::kHalf    = 0;
  }

  /* Vector constants */

  namespace EON{
    template<> const vec4l vec4l::kForward ( 0, 1, 0 );
    template<> const vec4l vec4l::kUp      ( 0, 0, 1 );
    template<> const vec4l vec4l::kZero    ( 0, 0, 0 );
    template<> const vec4l vec4l::kRight   ( 1, 0, 0 );
    template<> const vec3l vec3l::kForward ( 0, 1, 0 );
    template<> const vec3l vec3l::kRight   ( 1, 0, 0 );
    template<> const vec3l vec3l::kUp      ( 0, 0, 1 );
    template<> const vec3l vec3l::kZero    ( 0, 0, 0 );
    template<> const vec2l vec2l::kRight   ( 1, 0 );
    template<> const vec2l vec2l::kUp      ( 0,-1 );
    template<> const vec2l vec2l::kZero    ( 0, 0 );
    template<> const pt3l  pt3l ::kZero    ( 0, 0, 0 );
    template<> const pt2l  pt2l ::kZero    ( 0, 0 );
    template<> const quatl quatl::kIdentity( 0, 0, 0, 1 );
  }

  /* Color constants */

  namespace EON{
    template<> const rgbal rgbal::kBlack  (   0, 255 );
    template<> const rgbal rgbal::kWhite  ( 255 );
    template<> const rgbal rgbal::kGrey   ( 127, 255 );
    template<> const rgbal rgbal::kOrange ( 255, .5f,   0, 255 );
    template<> const rgbal rgbal::kYellow ( 255, 255,   0, 255 );
    template<> const rgbal rgbal::kLime   ( 127, 255,   0, 255 );
    template<> const rgbal rgbal::kGreen  (   0, 255,   0, 255 );
    template<> const rgbal rgbal::kCyan   (   0, 255, 255, 255 );
    template<> const rgbal rgbal::kBlue   (   0,   0, 255, 255 );
    template<> const rgbal rgbal::kMagenta( 255,   0, 255, 255 );
    template<> const rgbal rgbal::kRed    ( 255,   0,   0, 255 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2l aabb2l::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2l aabb2l::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2l aabb2l::kZero  ( 0 );
    template<> const aabb3l aabb3l::kZero  ( 0 );
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Basic:{                                         |
  //[forwarding]:{                                |
    //vec3l:{                                     |
      //[operators]:{                             |
        //operator=:{                             |

          namespace EON{
            template<> vec3l& vec3l::operator=( const vec4l& V ){
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
          template<> vec3l::Vector3( const vec4l& V ){
            x = V.x;
            y = V.y;
            z = V.z;
          }
        }*/

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //smoothstep:{                                  |

    template<> i64 i64::smoothstep( const i64& edge2, const i64& x )const{
      const self& t = clamp(( x-value )/( edge2-value ), k0, k1 );
      return t*t*( k3-k2*t );
    }

  //}:                                            |
  //saturate[d]:{                                 |

    template<> i64 i64::saturated()const{
      return max( min( value, k1 ), k0 );
    }

    template<> void i64::saturate(){
      *this = max( min( value, k1 ), k0 );
    }

  //}:                                            |
  //iround:{                                      |

    namespace EON{
      template<> s32 i64::iround()const{
        return s32( value );
      }
    }

  //}:                                            |
  //rsqrt:{                                       |

#ifdef __APPLE__
  #pragma mark - Basic functions -
#endif

    namespace EON{
      template<> i64 i64::rsqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //sqrt:{                                        |

    namespace EON{
      template<> i64 i64::sqrt()const{
        return 0;
      }
    }

  //}:                                            |
  //floor:{                                       |

    namespace EON{
      template<> i64 i64::floor()const{
        return value;
      }
    }

  //}:                                            |
  //ceil:{                                        |

    namespace EON{
      template<> i64 i64::ceil()const{
        return value;
      }
    }

  //}:                                            |
  //frac:{                                        |

    namespace EON{
      template<> i64 i64::frac( i64& i )const{
        return 0;
      }
    }

  //}:                                            |
  //exp:{                                         |

    namespace EON{
      template<> i64 i64::exp()const{
        return 0;
      }
    }

  //}:                                            |
  //pow:{                                         |

    namespace EON{
      template<> i64 i64::pow( const i64& y )const{
        s32 sum = 1;
        for( i64 i=0; i<y; ++i ){
          sum *= i64::cast( value );
        }
        return sum;
      }
    }

  //}:                                            |
  //atan2:{                                       |

    namespace EON{
      template<> i64 i64::atan2( const i64& y )const{
        return 0;
      }
    }

  //}:                                            |
  //atan:{                                        |

    namespace EON{
      template<> i64 i64::atan()const{
        return 0;
      }
    }

  //}:                                            |
  //tan:{                                         |

    namespace EON{
      template<> i64 i64::tan()const{
        return 0;
      }
    }

  //}:                                            |
  //asin:{                                        |

    namespace EON{
      template<> i64 i64::asin()const{
        return 0;
      }
    }

  //}:                                            |
  //sin:{                                         |

    namespace EON{
      template<> i64 i64::sin()const{
        return 0;
      }
    }

  //}:                                            |
  //acos:{                                        |

    namespace EON{
      template<> i64 i64::acos()const{
        return 0;
      }
    }

  //}:                                            |
  //cos:{                                         |

    namespace EON{
      template<> i64 i64::cos()const{
        return 0;
      }
    }

  //}:                                            |
  //mod:{                                         |

    namespace EON{
      template<> i64 i64::mod( const i64& y )const{
        return value % i64::cast( y );
      }
    }

  //}:                                            |
  //abs:{                                         |

    namespace EON{
      template<> i64 i64::abs()const{
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
    template<> bool i64::valid( const i64& t ){
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
          template<> pt3l pt3l::operator-( const vec3l& v )const{
            pt3l r;
            r.x = x-v.x;
            r.y = y-v.y;
            r.z = z-v.z;
            return r;
          }
        }

      //}:                                        |
      //operator+:{                               |

        namespace EON{
          template<> pt3l pt3l::operator+( const vec3l& v )const{
            pt3l r;
            r.x = x+v.x;
            r.y = y+v.y;
            r.z = z+v.z;
            return r;
          }
        }

      //}:                                        |
      //operator/:{                               |

        namespace EON{
          template<> pt3l pt3l::operator/( const vec3l& v )const{
            pt3l r;
            r.x = x/v.x;
            r.y = y/v.y;
            r.z = z/v.z;
            return r;
          }
        }

      //}:                                        |
      //operator*:{                               |

        namespace EON{
          template<> pt3l pt3l::operator*( const vec4x4l& M )const{
            return pt3l(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> pt3l pt3l::operator*( const vec3x3l& M )const{
            return pt3l(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z
            );
          }
          template<> pt3l pt3l::operator*( const vec3l& v )const{
            pt3l r;
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
        template<> pt3l::Point3( const vec4l& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

      namespace EON{
        template<> pt3l::Point3( const vec3l& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

    //}:                                          |
    //Methods:{                                   |

      namespace EON{
        template<> pt2l pt2l::scaledToClip( const i64& scale )const{
          return{};
        }
      }

      namespace EON{
        template<> void pt2l::scaleToClip( const i64& scale ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Circle:{                                      |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb2l circlel::toBounds()const{
          return aabb2l(
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
        template<> bool spherel::intersects( const ray3l& in_tRay, i64& out_fDistance )const{
          const vec3l Q = this->v.xyz() - in_tRay.p;
          const i64& c = Q.length();
          const i64& v = Q.dot( in_tRay.d );
          const i64& d = toRadius()*toRadius() - (c*c - v*v);
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
        template<> aabb3l spherel::toBounds()const{
          return aabb3l(
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
      template<> void planel::set( const trianglel& in_tTriangle ){
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
        template<> i64 quatl::lengthSquared()const{
          return 0;
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void quatl::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //operator::==:{                            |

        namespace EON{
          template<> bool quatl::operator==( const quatl& q )const{
            return( 0 == memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::!=:{                            |

        namespace EON{
          template<> bool quatl::operator!=( const quatl& q )const{
            return( 0 != memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quatl::operator<( const quatl& q )const{
            return( memcmp( a, q.a, sizeof( a )) < 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quatl::operator<=( const quatl& q )const{
            return( memcmp( a, q.a, sizeof( a )) <= 0 );
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quatl::operator>( const quatl& q )const{
            return( memcmp( a, q.a, sizeof( a )) > 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quatl::operator>=( const quatl& q )const{
            return( memcmp( a, q.a, 16 ) >= 0 );
          }
        }

      //}:                                        |
      //operator::+?:{                            |

        namespace EON{
          template<> quatl& quatl::operator+=( const quatl& q ){
            x+=q.x;
            y+=q.y;
            z+=q.z;
            w+=q.w;
            return *this;
          }
        }

        namespace EON{
          template<> quatl quatl::operator+( const quatl& q )const{
            return quatl(
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
          template<> quatl quatl::operator*( const quatl& q )const{
            return quatl(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z
            );
          }
        }

        namespace EON{
          template<> quatl quatl::operator*( const i64& s )const{
            return quatl(
              x * s,
              y * s,
              z * s,
              w * s
            );
          }
        }

        namespace EON{
          template<> quatl& quatl::operator*=( const quatl& q ){
            const quatl r(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z );
            *this = r;
            return *this;
          }
        }

        namespace EON{
          template<> quatl& quatl::operator*=( const i64& s ){
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
          template<> quatl& quatl::operator/=( const quatl& q ){
            quatl p( q );
            p.invert();
            (*this)*p;
            return *this;
          }
        }

        namespace EON{
          template<> quatl& quatl::operator/=( const i64& s ){
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
          template<> quatl quatl::operator/( const quatl& q )const{
            quatl p( q );
            p.invert();
            return (*this)*p;
          }
        }

        namespace EON{
          template<> quatl quatl::operator/( const i64& s )const{
            quatl q;
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
        template<> void quatl::setAxisAngle( const vec3l& axis, const i64& angleDeg ){
          const i64& a = rad( angleDeg );
          const vec3l& v = axis*sin( a / 2 );
          x = v.x;
          y = v.y;
          z = v.z;
          w = cos( a/2.f );
        }
      }

    //}:                                          |
    //asQuaternion:{                              |

      namespace EON{
        template<> quatl vec3x3l::asQuaternion()const{
          return{};
        }
      }

    //}:                                          |
    //compare:{                                   |

      namespace EON{
        template<> bool quatl::compare( const quatl& q, const i64& tolerance )const{
          return true;
        }
      }

    //}:                                          |
    //rotate:{                                    |

      namespace EON{
        template<> void quatl::rotate( const vec3l& axis, const i64& angleDeg ){
        }
      }

      namespace EON{
        template<> void quatl::rotate( const vec2l& xyMouse ){
        }
      }

    //}:                                          |
    //length:{                                    |

      namespace EON{
        template<> i64 quatl::length()const{
          return 0;
        }
      }

    //}:                                          |
    //normalize:{                                 |

      namespace EON{
        template<> quatl& quatl::normalize(){
          return *this;
        }
      }

    //}:                                          |
    //setEuler:{                                  |

      namespace EON{
        template<> void quatl::setEuler( const vec3l& e ){
        }
      }

    //}:                                          |
    //getEuler:{                                  |

      namespace EON{
        template<> void quatl::getEuler( i64& out_fX, i64& out_fY, i64& out_fZ, const bool bHomogeneous )const{
        }
      }

    //}:                                          |
    //randomize:{                                 |

      namespace EON{
        template<> void quatl::randomize( const i64& x1, const i64& x2, const i64& x3 ){
        }
      }

    //}:                                          |
    //get?x?:{                                    |

      namespace EON{
        template<> vec4x4l quatl::get4x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3l quatl::get3x3()const{
          return{};
        }
      }

    //}:                                          |
    //dot:{                                       |

      namespace EON{
        template<> i64 quatl::dot( const quatl& q )const{
          return 0;
        }
      }

    //}:                                          |
    //lookat:{                                    |

      namespace EON{
        template<> void quatl::lookat( const pt3l& srcPoint, const pt3l& dstPoint ){
        }
      }

    //}:                                          |
    //inverse:{                                   |

      namespace EON{
        template<> quatl quatl::inverse()const{
          return{};
        }
      }

    //}:                                          |
    //conjugate:{                                 |

      namespace EON{
        template<> quatl quatl::conjugate()const{
          return{};
        }
      }

    //}:                                          |
    //exp:{                                       |

      namespace EON{
        template<> quatl quatl::exp()const{
          return{};
        }
      }

    //}:                                          |
    //log:{                                       |

      namespace EON{
        template<> quatl quatl::log()const{
          return{};
        }
      }

    //}:                                          |
    //lerped:{                                    |

      namespace EON{
        template<> quatl quatl::lerped( const quatl& p, const quatl& q, const i64& fTime ){
          return{};
        }
      }

    //}:                                          |
    //lerp:{                                      |

      namespace EON{
        template<> quatl& quatl::lerp( const quatl& p, const quatl& q, const i64& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //slerpedNoInvert:{                           |

      namespace EON{
        template<> quatl quatl::slerpedNoInvert( const quatl& p, const quatl& q, const i64& fTime ){
          return{};
        }
      }

    //}:                                          |
    //slerp:{                                     |

      namespace EON{
        template<> quatl& quatl::slerp( const quatl& p, const quatl& q, const i64& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //squad:{                                     |

      namespace EON{
        template<> quatl& quatl::squad( const quatl& q1, const quatl& q2, const quatl& a, const quatl& b, const i64& fTime ){
          return *this;
        }
      }

    //}:                                          |
    //align:{                                     |

      namespace EON{
        template<> quatl& quatl::align( const vec3l& up, const vec3l& norm ){
          return *this;
        }
      }

    //}:                                          |
  //}:                                            |
  //Matrices:{                                    |
    //vec4x4l:{                                    |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Matrices
  #pragma mark -
#endif

        namespace EON{
          template<> vec4x4l& vec4x4l::operator*=( const vec4x4l& B ){
            return *this;
          }
        }

        //}:                                      |
      //}:                                        |
      //transpose:{                               |

        namespace EON{
          template<> void vec4x4l::transpose(){
          }
        }

      //}:                                        |
      //setPerspectiveV2C:{                       |

        namespace EON{
          template<> void vec4x4l::setPerspectiveV2C( const i64& degFovY, const i64& aspect, const i64& zn, const i64& zf ){
          }
        }

      //}:                                        |
      //setAxisIdentity:{                         |

        namespace EON{
          template<> void vec4x4l::setAxisIdentity(){
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
          template<> void vec4x4l::setTranslation( const i64& x, const i64& y, const i64& z ){
            XX = 1; XY = 0; XZ = 0; XW = 0;
            YX = 0; YY = 1; YZ = 0; YW = 0;
            ZX = 0; ZY = 0; ZZ = 1; ZW = 0;
            WX = x; WY = y; WZ = z; WW = 1;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec4x4l::setScale( const i64& x, const i64& y, const i64& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

        namespace EON{
          template<> void vec4x4l::setScale( const i64& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec4x4l::setRotX( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec4x4l::setRotY( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec4x4l::setRotZ( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec4x4l::removeTranslation(){
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec4x4l::removeScale(){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec4x4l vec4x4l::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec4x4l::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x4l:{                                    |
      //[converter]:{                             |
        //vec4x4l:{                               |

          namespace EON{
            template<> template<> vec4x4l vec3x4l::as<vec4x4l>()const{
              return{};
            }
          }

        //}:                                      |
        //quatl:{                                 |

          namespace EON{
            template<> template<> quatl vec3x4l::as<quatl>()const{
              return{};
            }
          }

        //}:                                      |
        //vec3x3l:{                               |

          namespace EON{
            template<> template<> vec3x3l vec3x4l::as<vec3x3l>()const{
              return{};
            }
          }

        //}:                                      |
        //pt3l:{                                  |

          namespace EON{
            template<> template<> pt3l vec3x4l::as<pt3l>()const{
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
            template<> vec3x4l& vec3x4l::operator*=( const vec3x4l& B ){
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
      //as<>:{                                    |

        namespace EON{
          template<> template<> vec3x3l vec4x4l::as<vec3x3l>()const{
            return{};
          }
        }

      //}:                                        |
      //removeTranslation:{                       |

        namespace EON{
          template<> void vec3x4l::removeTranslation(){
            WX = WY = WZ = 0;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec3x4l::setTranslation( const i64& x, const i64& y, const i64& z ){
            XX = 1; YX = 0; ZX = 0; WX = x;
            XY = 0; YY = 1; ZY = 0; WY = y;
            XZ = 0; YZ = 0; ZZ = 1; WZ = z;
          }
        }

      //}:                                        |
      //removeScale:{                             |

        namespace EON{
          template<> void vec3x4l::removeScale(){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x4l::setScale( const i64& x, const i64& y, const i64& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
          }
        }

        namespace EON{
          template<> void vec3x4l::setScale( const i64& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x4l::setRotX( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x4l::setRotY( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x4l::setRotZ( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //inverse:{                                 |

        namespace EON{
          template<> vec3x4l vec3x4l::inverse()const{
            return{};
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace EON{
          template<> void vec3x4l::invert(){
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x3l:{                                   |
      //setAxisAngle:{                            |

        namespace EON{
          template<> void vec3x3l::setAxisAngle( const vec3l& axis, const i64& angle ){
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x3l::setScale( const i64& x, const i64& y, const i64& z ){
            XX = x; YX = 0; ZX = 0;
            XY = 0; YY = y; ZY = 0;
            XZ = 0; YZ = 0; ZZ = z;
          }
          template<> void vec3x3l::setScale( const i64& s ){
            XX = s; YX = 0; ZX = 0;
            XY = 0; YY = s; ZY = 0;
            XZ = 0; YZ = 0; ZZ = s;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x3l::setRotX( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x3l::setRotY( const i64& angleInDeg ){
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x3l::setRotZ( const i64& angleInDeg ){
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
        template<> bool frustuml::intersects( const pt3l& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustuml::intersects( const spherel& S )const{
          return false;
        }
      }

      namespace EON{
        template<> bool frustuml::intersects( const aabb3l& B )const{
          return false;
        }
      }

    //}:                                          |
    //set:{                                       |

      namespace EON{
        template<> void frustuml::set( const vec4x4l& inW2C ){
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //Vector4:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec4l vec4l::operator*( const vec4x4l& M )const{
            return{};
          }
          template<> vec4l vec4l::operator*( const vec3x3l& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector3:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec3l vec3l::operator*( const vec4x4l& M )const{
            return{};
          }
          template<> vec3l vec3l::operator*( const vec3x3l& M )const{
            return{};
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector2:{                                   |
      //[operators]:{                             |
        //vec2l:{                                 |

          namespace EON{
            template<> vec2l  vec2l::operator+ ( const vec2l& v )const{ return vec2l( x+v.x, y+v.y ); }
            template<> vec2l  vec2l::operator- ( const vec2l& v )const{ return vec2l( x-v.x, y-v.y ); }
            template<> vec2l  vec2l::operator/ ( const vec2l& v )const{ return vec2l( x/v.x, y/v.y ); }
            template<> vec2l  vec2l::operator* ( const vec2l& v )const{ return vec2l( x*v.x, y*v.y ); }
            template<> vec2l& vec2l::operator+=( const vec2l& v ){ x+=v.x; y+=v.y; return*this; }
            template<> vec2l& vec2l::operator-=( const vec2l& v ){ x-=v.x; y-=v.y; return*this; }
            template<> vec2l& vec2l::operator/=( const vec2l& v ){ x/=v.x; y/=v.y; return*this; }
            template<> vec2l& vec2l::operator*=( const vec2l& v ){ x*=v.x; y*=v.y; return*this; }
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
        template<> vec4x4l qst3l::get4x4()const{
          return{};
        }
        template<> vec3x4l qst3l::get3x4()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3l qst3l::get3x3()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4l qst3l::get4x4RT()const{
          return{};
        }
        template<> vec3x4l qst3l::get3x4RT()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4l qst3l::get4x4ST()const{
          return{};
        }
        template<> vec3x4l qst3l::get3x4ST()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4l qst3l::get4x4T()const{
          return{};
        }
        template<> vec3x4l qst3l::get3x4T()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4l qst3l::get4x4R()const{
          return{};
        }
        template<> vec3x4l qst3l::get3x4R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3l qst3l::get3x3R()const{
          return{};
        }
      }

      namespace EON{
        template<> vec4x4l qst3l::get4x4S()const{
          return{};
        }
        template<> vec3x4l qst3l::get3x4S()const{
          return{};
        }
      }

      namespace EON{
        template<> vec3x3l qst3l::get3x3S()const{
          return{};
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void qst3l::invert(){
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //Multiply:{                                |

        namespace EON{
          template<> pt3l qst3l::operator*( const pt3l& P )const{
            return{};
          }
        }

        namespace EON{
          template<> void qst3l::operator*=( const qst3l& L2W ){
          }
        }

      //}:                                        |
      //Array:{                                   |

        namespace EON{
          template<> i64 qst3l::operator[]( const u32 i )const{
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
  //aabb3l:{                                      |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Bounding volumes
  #pragma mark -
#endif

      //http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
      namespace EON{
        template<> bool aabb3l::intersects( const ray3l& inRay, i64& outDst )const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb3l::contains( const pt3l& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3l::contains( const aabb3l& B )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb3l::contains( const trianglel& t )const{
          return false;
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb3l::empty()const{
          return false;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb3l::valid()const{
          return true;
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb3l::reset(){
        }
      }

    //}:                                          |
  //}:                                            |
  //aabb2l:{                                      |
    //scaledToClip:{                              |

      namespace EON{
        template<> aabb2l aabb2l::scaledToClip( const i64& scale )const{
          return{};
        }
      }

    //}:                                          |
    //scaleToClip:{                               |

      namespace EON{
        template<> void aabb2l::scaleToClip( const i64& scale ){
        }
      }

    //}:                                          |
    //scaleBack:{                                 |

      namespace EON{
        template<> void aabb2l::scaleBack(){
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb2l::empty()const{
          return true;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb2l::valid()const{
          return false;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb2l::contains( const pt2l& P )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2l::contains( const trianglel& t )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2l::contains( const aabb2l& B )const{
          return false;
        }
      }

    //}:                                          |
    //intersects:{                                |

      namespace EON{
        template<> bool aabb2l::intersects( const ray2l& r )const{
          return false;
        }
      }

      namespace EON{
        template<> bool aabb2l::intersects( const aabb2l& b )const{
          return false;
        }
      }

    //}:                                          |
    //clipAgainst:{                               |

      namespace EON{
        template<> bool aabb2l::clipAgainst( const aabb2l& b ){
          return false;
        }
      }

    //}:                                          |
    //circle:{                                    |

      namespace EON{
        template<> circlel aabb2l::circle()const{
          return{};
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb2l::reset(){
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

    template<> i64 e_rand<i64>( const i64 start, const i64 end ){
      return 0;
    }

    template<> i64 e_rand<i64>(){
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
        template<> i64 rgbal::multiply( const i64& base, const i64& blend ){
          return( base*blend );
        }
      }

    //}:                                          |
    //lighten:{                                   |

      namespace EON{
        template<> i64 rgbal::lighten( const i64& base, const i64& blend ){
          return max( base, blend );
        }
      }

    //}:                                          |
    //darken:{                                    |

      namespace EON{
        template<> i64 rgbal::darken( const i64& base, const i64& blend ){
          return min( base, blend );
        }
      }

    //}:                                          |
    //average:{                                   |

      namespace EON{
        template<> i64 rgbal::average( const i64& base, const i64& blend ){
          return( base+blend )*kHalf;
        }
      }

    //}:                                          |
    //linearDodge:{                               |

      namespace EON{
        template<> i64 rgbal::linearDodge( const i64& base, const i64& blend ){
          return self::saturate( base+blend );
        }
      }

    //}:                                          |
    //linearBurn:{                                |

      namespace EON{
        template<> i64 rgbal::linearBurn( const i64& base, const i64& blend ){
          return self::saturate( base-blend );
        }
      }

    //}:                                          |
    //difference:{                                |

      namespace EON{
        template<> i64 rgbal::difference( const i64& base, const i64& blend ){
          return abs( base-blend );
        }
      }

    //}:                                          |
    //negation:{                                  |

      namespace EON{
        template<> i64 rgbal::negation( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          return( k1 - abs( k1 - base - blend ));
        }
      }

    //}:                                          |
    //screen:{                                    |

      namespace EON{
        template<> i64 rgbal::screen( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          return 1.f-(( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //exclusion:{                                 |

      namespace EON{
        template<> i64 rgbal::exclusion( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          return( base+blend-k2*base*blend );
        }
      }

    //}:                                          |
    //overlay:{                                   |

      namespace EON{
        template<> i64 rgbal::overlay( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          return( base <kHalf ) ?( 2.f * base * blend ) :( 1.f - 2.f*( 1.f - base )*( 1.f - blend ));
        }
      }

    //}:                                          |
    //hardLight:{                                 |

      namespace EON{
        template<> i64 rgbal::hardLight( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          return( blend <kHalf ) ?( k2 * blend * base ) :( k1 - k2 *( k1 - blend ) *( k1 - base ));
        }
      }

    //}:                                          |
    //softLight:{                                 |

      namespace EON{
        template<> i64 rgbal::softLight( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          if( blend < kHalf ){
            return( k2 * base * blend + base*base *( k1 - k2 * blend ));
          }
          return( sqrt( base ) * ( k2 * blend - k1 ) + k2 * base * ( k1 - blend ));
        }
      }

    //}:                                          |
    //softDodge:{                                 |

      namespace EON{
        template<> i64 rgbal::softDodge( const i64& base, const i64& blend ){
          return(( base + blend ) < 1.f ) ? (( base * .5f ) / ( 1.f - blend )) : ( 1.f - (( .5f *( 1.f - blend )) / base ));
        }
      }

    //}:                                          |
    //colorDodge:{                                |

      namespace EON{
        template<> i64 rgbal::colorDodge( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //colorBurn:{                                 |

      namespace EON{
        template<> i64 rgbal::colorBurn( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          if( blend > k0 ){
            return max(( k1 -(( k1 - base ) / blend )), k0 );
          }
          return blend;
        }
      }

    //}:                                          |
    //linearLight:{                               |

      namespace EON{
        template<> i64 rgbal::linearLight( const i64& base, const i64& blend ){
          return( blend < kHalf ) ? linearBurn( base, k2*blend ) : linearDodge( base, k2*( blend-kHalf ));
        }
      }

    //}:                                          |
    //vividLight:{                                |

      namespace EON{
        template<> i64 rgbal::vividLight( const i64& base, const i64& blend ){
          return( blend < kHalf ) ? colorBurn( base,( k2 * blend )) : colorDodge( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //pinLight:{                                  |

      namespace EON{
        template<> i64 rgbal::pinLight( const i64& base, const i64& blend ){
          return( blend < kHalf ) ? darken( base,( k2 * blend )) : lighten( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //hardMix:{                                   |

      namespace EON{
        template<> i64 rgbal::hardMix( const i64& base, const i64& blend ){
          return( vividLight( base, blend ) < kHalf ) ? k0 : k1;
        }
      }

    //}:                                          |
    //reflection:{                                |

      namespace EON{
        template<> i64 rgbal::reflect( const i64& in_fBase, const i64& in_fBlend ){
          const i64& blend = self::saturate( in_fBlend );
          const i64& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base*base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //glow:{                                      |

      namespace EON{
        template<> i64 rgbal::glow( const i64& base, const i64& blend ){
          if( base != k1 ){
            return min( blend*blend/( k1 - base ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //pheonix:{                                   |

      namespace EON{
        template<> i64 rgbal::phoenix( const i64& base, const i64& blend ){
          return min( base, blend ) - max( base, blend ) + k1;
        }
      }

    //}:                                          |
    //setHSV:{                                    |

      namespace EON{
        template<> void rgbal::setHSV( const i64& H, const i64& S, const i64& V ){
        }
      }

    //}:                                          |
    //hsv:{                                       |

      namespace EON{
        //http://www.tech-faq.com/hsv.html
        template<> hsval rgbal::hsv()const{
          return{};
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    namespace EON{
      template<> void rgbal::set( const u32 inBGRA ){
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
      template<> bool trianglel::intersects( const vec4x4l& L2W, const ray3l& ray, pt3l& hit, vec3l& norm )const{
        return false;
      }
      template<> bool trianglel::intersects( const qst3l& L2W, const ray3l& ray, pt3l& hit, vec3l& norm )const{
        return false;
      }
      template<> bool trianglel::intersects( const ray3l& ray, pt3l& hit, vec3l& norm )const{
        return false;
      }
    }

  //}:                                            |
  //toBounds2D:{                                  |

    namespace EON{
      template<> aabb2l trianglel::toBounds2D()const{
        return{};
      }
    }

  //}:                                            |
  //toBounds:{                                    |

    namespace EON{
      template<> aabb3l trianglel::toBounds()const{
        return{};
      }
    }

  //}:                                            |
  //draw:{                                        |

    namespace EON{
      template<> bool trianglel::draw( IRasterizer& rasterizer, const bool bTestPrimitive )const{
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
          template<> i64::Quadtree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Quadtree: methods -
#endif

          namespace EON{
            template<> i64::Quadtree::Leaf::Result i64::Quadtree::Leaf::intersects( const Quadtree* pQuadtree, const vec4x4l& L2W, const Ray3& r )const{
              return nullptr;
            }
            template<> i64::Quadtree::Leaf::Result i64::Quadtree::Leaf::intersects( const Quadtree* pQuadtree, const vec4x4l& L2W, const Ray3& r, const u32 page )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i64::Quadtree::Leaf::Status i64::Quadtree::Leaf::addPolygon( Quadtree* pQuadtree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i64::Quadtree::Leaf::clear(){
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
          template<> i64::Octree::Leaf::~Leaf(){
          }
        }

      //}:                                        |
      //Methods:{                                 |
        //intersects:{                            |

#ifdef __APPLE__
  #pragma mark - Octree: methods -
#endif

          namespace EON{
            template<> i64::Octree::Leaf::Result i64::Octree::Leaf::intersects( const Octree* pOctree, const vec4x4l& L2W, const Ray3& r )const{
              return nullptr;
            }
          }

        //}:                                      |
        //addPolygon:{                            |

          namespace EON{
            template<> i64::Octree::Leaf::Status i64::Octree::Leaf::addPolygon( Octree* pOctree, const u32 depth, const Polygon& poly ){
              return Status::kFalse;
            }
          }

        //}:                                      |
        //clear:{                                 |

          namespace EON{
            template<> void i64::Octree::Leaf::clear(){
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
