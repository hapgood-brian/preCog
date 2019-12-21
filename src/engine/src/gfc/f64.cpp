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

#include<float.h>
#include<random>
#include<cmath>

using namespace EON;
using namespace gfc;

//================================================|=============================
//Constants:{                                     |

  //http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html#algo3

#ifdef __APPLE__
  #pragma mark - Constants -
#endif

  /* Constants */

  namespace EON{
    template<> const f64 f64::k0       =  0.;
    template<> const f64 f64::k1       =  1.;
    template<> const f64 f64::k2       =  2.;
    template<> const f64 f64::k3       =  3.;
    template<> const f64 f64::k4       =  4.;
    template<> const f64 f64::k5       =  5.;
    template<> const f64 f64::k6       =  6.;
    template<> const f64 f64::k7       =  7.;
    template<> const f64 f64::k8       =  8.;
    template<> const f64 f64::k9       =  9.;
    template<> const f64 f64::k10      = 10.;
    template<> const f64 f64::kPI      = 3.141592653589793f;
    template<> const f64 f64::k2PI     = kPI * k2;
    template<> const f64 f64::k4PI     = kPI * k4;
    template<> const f64 f64::cm       = 1.;
    template<> const f64 f64::mm       = cm / 10.;
    template<> const f64 f64::m        =     100. * cm;
    template<> const f64 f64::dm       =      10. * m;
    template<> const f64 f64::km       =    1000. * m;
    template<> const f64 f64::Mm       =    1000. * km;
    template<> const f64 f64::g        =       1.;
    template<> const f64 f64::kg       =    1000. * g;
    template<> const f64 f64::t        =    1000. * kg;
    template<> const f64 f64::kt       =    1000. * t;
    template<> const f64 f64::mt       =    1000. * kt;
    template<> const f64 f64::kMin     =-FLT_MAX;
    template<> const f64 f64::kMax     = FLT_MAX;
    template<> const f64 f64::kEpsilon = 0.001f;
  }

  /* Vector constants */

  namespace EON{
    template<> const vec4d vec4d::kForward ( 0., 1., 0. );
    template<> const vec4d vec4d::kUp      ( 0., 0., 1. );
    template<> const vec4d vec4d::kZero    ( 0., 0., 0. );
    template<> const vec4d vec4d::kRight   ( 1., 0., 0. );
    template<> const vec3d vec3d::kForward ( 0., 1., 0. );
    template<> const vec3d vec3d::kRight   ( 1., 0., 0. );
    template<> const vec3d vec3d::kUp      ( 0., 0., 1. );
    template<> const vec3d vec3d::kZero    ( 0., 0., 0. );
    template<> const vec2d vec2d::kRight   ( 1., 0. );
    template<> const vec2d vec2d::kUp      ( 0.,-1. );
    template<> const vec2d vec2d::kZero    ( 0., 0. );
    template<> const pt3d  pt3d ::kZero    ( 0., 0., 0. );
    template<> const pt2d  pt2d ::kZero    ( 0., 0. );
    template<> const quatd quatd::kIdentity( 0., 0., 0., 1. );
  }

  /* Color constants */

  namespace EON{
    template<> const rgbad rgbad::kBlack  ( 0., 1. );
    template<> const rgbad rgbad::kWhite  ( 1. );
    template<> const rgbad rgbad::kGrey   ( .5, 1. );
    template<> const rgbad rgbad::kOrange ( 1., .5, 0., 1. );
    template<> const rgbad rgbad::kYellow ( 1., 1., 0., 1. );
    template<> const rgbad rgbad::kLime   ( .5, 1., 0., 1. );
    template<> const rgbad rgbad::kGreen  ( 0., 1., 0., 1. );
    template<> const rgbad rgbad::kCyan   ( 0., 1., 1., 1. );
    template<> const rgbad rgbad::kBlue   ( 0., 0., 1., 1. );
    template<> const rgbad rgbad::kMagenta( 1., 0., 1., 1. );
    template<> const rgbad rgbad::kRed    ( 1., 0., 0., 1. );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2d aabb2d::kFullUV( 0., 0., 1., 1. );
    template<> const aabb2d aabb2d::kInvUV ( 0., 1., 1., 0. );
    template<> const aabb2d aabb2d::kZero  ( 0. );
    template<> const aabb3d aabb3d::kZero  ( 0. );
  }

  /* Random TLS */

  namespace{
    __thread std::mt19937* kGen64 = nullptr;
  }

//}:                                              |
//Private:{                                       |
  //line2line:{                                   |

    namespace{
      bool line2line( const pt2d& v0, const pt2d& v1, const pt2d& v2, const pt2d& v3 ){
        const f64& denom = (( v3.y-v2.y )*( v1.x-v0.x ))-(( v3.x-v2.x )*( v1.y-v0.y ));
        const f64& numer = (( v3.x-v2.x )*( v0.y-v2.y ))-(( v3.y-v2.y )*( v0.x-v2.x ));
        const f64& nume2 = (( v1.x-v0.x )*( v0.y-v2.y ))-(( v1.y-v0.y )*( v0.x-v2.x ));
        if( denom==0. ){
          if( numer==0. && nume2==0. ){
            return false;//coincident
          }
          return false;//parallel
        }
        const f64& ua = numer / denom;
        const f64& ub = nume2 / denom;
        return( ua >= 0.0f && ua <= 1. && ub >= 0.0f && ub <= 1. );
      }
    }

  //}:                                            |
  //line2rect:{                                   |

    namespace{
      bool line2rect( const pt2d v[2], const aabb2d& r ){
        const pt2d ll( r.min.x, r.max.y );
        const pt2d ur( r.max.x, r.min.y );
        const pt2d ul( r.min.x, r.min.y );
        const pt2d lr( r.max.x, r.max.y );
        for( u32 i=0; i<2; ++i ){
          if( v[i].x > ll.x && v[i].x < ur.x && v[i].y < ll.y && v[i].y > ur.y ){
            return true;
          }
        }
        if( line2line( v[0], v[1], ul, ll )){
          return true;
        }
        if( line2line( v[0], v[1], ll, lr )){
          return true;
        }
        if( line2line( v[0], v[1], ul, ur )){
          return true;
        }
        if( line2line( v[0], v[1], ur, lr )){
          return true;
        }
        return false;
      }
    }

  //}:                                            |
  //hue2RGB:{                                     |

    namespace{
      f64 hue2RGB( const f64& v1, const f64& v2, f64 vH ){
        if( vH < 0. ){
          vH += 1.;
        }
        if( vH > 1. ){
          vH -= 1.;
        }
        if( 6. * vH < 1. ){
          return v1 + (v2 - v1) * 6. * vH;
        }
        if( 2. * vH < 1. ){
          return v2;
        }
        if( 3. * vH < 2. ){
          return v1 + (v2 - v1) * (0.666666f - vH) * 6.;
        }
        return v1;
      }
    }

  //}:                                            |
  //imin3:{                                       |

    #if 0
      namespace{
        int imin3( const int x, const int y, const int z ){
          const int T = (x < y ? x : y);
          return( T < z ? T : z );
        }
      }
    #endif

  //}:                                            |
  //imax3:{                                       |

    #if 0
      namespace{
        int imax3( const int x, const int y, const int z ){
          const int T = (x > y ? x : y);
          return( T > z ? T : z );
        }
      }
    #endif

  //}:                                            |
  //gen:{                                         |

    namespace{
      std::mt19937& gen64(){
        if( !kGen64 ){
          kGen64 = new std::mt19937( u32( e_seconds() ));
        }
        return *kGen64;
      }
    }

  //}:                                            |
//}:                                              |
//Basic:{                                         |
  //smoothstep:{                                  |

    template<> f64 f64::smoothstep( const f64& edge2, const f64& x ){
      const self& t = (( x-value )/( edge2-value )).clamped( k1 );
      return t*t*( k3-k2*t );
    }

  //}:                                            |
  //saturate:{                                    |

    template<> f64 f64::saturated()const{
      return max( min( *this, k1 ), k0 );
    }

    template<> void f64::saturate(){
      *this = max( min( value, k1 ), k0 );
    }

  //}:                                            |
  //iround:{                                      |

    namespace EON{
      template<> s32 f64::iround()const{
        return s32( value+.5 );
      }
    }

  //}:                                            |
  //rsqrt:{                                       |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Basic functions
  #pragma mark -
#endif

    namespace EON{
      template<> f64 f64::rsqrt()const{
        return 0.;
      }
    }

  //}:                                            |
  //sqrt:{                                        |

    namespace EON{
      template<> f64 f64::sqrt()const{
        e_assert( value >= 0. );
        return::sqrt( value );
      }
    }

  //}:                                            |
  //floor:{                                       |

    namespace EON{
      template<> f64 f64::floor()const{
        return::floor( value );
      }
    }

  //}:                                            |
  //ceil:{                                        |

    namespace EON{
      template<> f64 f64::ceil()const{
        return::ceil( value );
      }
    }

  //}:                                            |
  //frac:{                                        |

    namespace EON{
      template<> f64 f64::frac( f64& i )const{
        i = s64( value );
        return ( value - i );
      }
    }

  //}:                                            |
  //exp:{                                         |

    namespace EON{
      template<> f64 f64::exp()const{
        return::exp( value );
      }
    }

  //}:                                            |
  //pow:{                                         |

    namespace EON{
      template<> f64 f64::pow( const f64& y )const{
        return::pow( value, y.value );
      }
    }

  //}:                                            |
  //atan2:{                                       |

    namespace EON{
      template<> f64 f64::atan2( const f64& y )const{
        return::atan2( value, y.value );
      }
    }

  //}:                                            |
  //atan:{                                        |

    namespace EON{
      template<> f64 f64::atan()const{
        return::atan( value );
      }
    }

  //}:                                            |
  //tan:{                                         |

    namespace EON{
      template<> f64 f64::tan()const{
        return::tan( value );
      }
    }

  //}:                                            |
  //asin:{                                        |

    namespace EON{
      template<> f64 f64::asin()const{
        return::asin( value );
      }
    }

  //}:                                            |
  //sin:{                                         |

    namespace EON{
      template<> f64 f64::sin()const{
        return::sin( value );
      }
    }

  //}:                                            |
  //acos:{                                        |

    namespace EON{
      template<> f64 f64::acos()const{
        return::acos( value );
      }
    }

  //}:                                            |
  //cos:{                                         |

    namespace EON{
      template<> f64 f64::cos()const{
        return::cos( value );
      }
    }

  //}:                                            |
  //mod:{                                         |

    namespace EON{
      template<> f64 f64::mod( const f64& y )const{
        return::fmod( value, y.value );
      }
    }

  //}:                                            |
  //abs:{                                         |

    namespace EON{
      template<> f64 f64::abs()const{
        return::fabs( value );
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Validity:{                                      |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Validity
  #pragma mark -
#endif

  namespace EON{
    template<> bool f64::valid( const f64& t ){
      //x != x is an odd property of NaNs.
      #if _MSC_VER == 1700
        if( !_finite( t.x )){
          return false;
        }
      #else
        if( std::isinf(double( t.value ))){
          return false;
        }
        if( std::isnan(double( t.value ))){
          return false;
        }
      #endif
      return true;
    }
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Geometry:{                                      |
  //Triangle:{                                    |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Geometry
  #pragma mark -
#endif

      namespace EON{
        template<> bool triangled::intersects( const ray3d& R, pt3d& hit, vec3d& norm )const{
          const vec3d& diff   = R.p - A;
          const vec3d& edge1  = B   - A;
          const vec3d& edge2  = C   - A;
          const vec3d& normal = edge1.cross( edge2 );
          f64 DdN = R.d.dot( normal );
          f64 sign;
          if( DdN > kEpsilon ){
            sign = 1.;
          }else if( DdN < -kEpsilon ){
            sign = -1.;
            DdN = -DdN;
          }else{
            return false;
          }
          const f64& DdQxE2 = sign*R.d.dot( diff.cross( edge2 ));
          if( DdQxE2 >= 0. ){
            const f64& DdE1xQ = sign*R.d.dot( edge1.cross( diff ));
            if( DdE1xQ >= 0. ){
              if( DdQxE2 + DdE1xQ <= DdN ){
                f64 QdN = -sign*diff.dot( normal );
                if( QdN >= 0. ){
                  const f64& inv = 1./ DdN;
                  const f64& par = QdN*inv;
                  hit.x = R.p.x+R.d.x*par;
                  hit.y = R.p.y+R.d.y*par;
                  hit.z = R.p.z+R.d.z*par;
                  norm.x = N.x;
                  norm.y = N.y;
                  norm.z = N.z;
                  return true;
                }
              }
            }
          }
          return false;
        }
      }

    //}:                                          |
    //toBounds2D:{                                |

      namespace EON{
        template<> aabb2d triangled::toBounds2D()const{
          aabb2d bounds;
          bounds += pt2d( A.x, A.y );
          bounds += pt2d( B.x, B.y );
          bounds += pt2d( C.x, C.y );
          return bounds;
        }
      }

    //}:                                          |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb3d triangled::toBounds()const{
          aabb3d bounds;
          bounds += A;
          bounds += B;
          bounds += C;
          return bounds;
        }
      }

    //}:                                          |
    //draw:{                                      |

      namespace EON{
        template<> bool triangled::draw( IRasterizer& rasterizer, const bool bTestPrimitive )const{
          #if /* DISABLES CODE */ (1)
            const u32 w = rasterizer.toW();
            const u32 h = rasterizer.toH();
            aabb2d b;
            b += pt2d( A.x, A.y );
            b += pt2d( B.x, B.y );
            b += pt2d( C.x, C.y );
            b.snap();
            const vec3d vs1( B.x-A.x, B.y-A.y );
            const vec3d vs2( C.x-A.x, C.y-A.y );

            //------------------------------------------------------------------
            // Test that triangled doesn't overwrite pixels.
            //------------------------------------------------------------------

            if( bTestPrimitive ){
              for( s32 y=b.min.y.as<s32>(); y<=b.max.y.as<s32>(); y++ ){
                for( s32 x=b.min.x.as<s32>(); x<=b.max.x.as<s32>(); x++ ){
                  if(( x >= 0 )&&( x < s32( w ))&&( y >= 0 )&&( y < s32( h ))){
                    const vec3d q( x-A.x, y-A.y );
                    const f64 s = (q.cross( vs2 ) / vs1.cross( vs2 )).length();
                    const f64 t = (vs1.cross( q ) / vs1.cross( vs2 )).length();
                    if(( s >= 0. ) && ( t >= 0. ) && ( s+t <= 1. )){
                      if( rasterizer.onTest( x, y )){
                        return false;
                      }
                    }
                  }
                }
              }
            }

            //------------------------------------------------------------------
            // Render triangled using rasterizer.
            //------------------------------------------------------------------

            for( s32 y=b.min.y.as<s32>(); y<=b.max.y.as<s32>(); y++ ){
              for( s32 x=b.min.x.as<s32>(); x<=b.max.x.as<s32>(); x++ ){
                if(( x >= 0 )&&( x < s32( w ))&&( y >= 0 )&&( y < s32( h ))){
                  const vec3d q( x-A.x, y-A.y );
                  const f64 s = (q.cross( vs2 ) / vs1.cross( vs2 )).length();
                  const f64 t = (vs1.cross( q ) / vs1.cross( vs2 )).length();
                  if(( s >= 0. ) && ( t >= 0. ) && ( s+t <= 1. )){
                    const f64& u = f64( x )/f64( w );
                    const f64& v = f64( y )/f64( h );
                    rasterizer.onDraw( x, y, vec2d( u, v ));
                  }
                }
              }
            }
          #else
            // 28.4 fixed-point coordinates
            const int Y1 = int( 16.0f * A.y+.5 );
            const int Y2 = int( 16.0f * B.y+.5 );
            const int Y3 = int( 16.0f * C.y+.5 );

            const int X1 = int( 16.0f * A.x+.5 );
            const int X2 = int( 16.0f * B.x+.5 );
            const int X3 = int( 16.0f * C.x+.5 );

            // Deltas
            const int DX12 = X1 - X2;
            const int DX23 = X2 - X3;
            const int DX31 = X3 - X1;

            const int DY12 = Y1 - Y2;
            const int DY23 = Y2 - Y3;
            const int DY31 = Y3 - Y1;

            // Fixed-point deltas
            const int FDX12 = DX12 << 4;
            const int FDX23 = DX23 << 4;
            const int FDX31 = DX31 << 4;

            const int FDY12 = DY12 << 4;
            const int FDY23 = DY23 << 4;
            const int FDY31 = DY31 << 4;

            // Bounding rectangle
            int minx = (imin3( X1, X2, X3 )+0xF) >> 4;
            int maxx = (imax3( X1, X2, X3 )+0xF) >> 4;
            int miny = (imin3( Y1, Y2, Y3 )+0xF) >> 4;
            int maxy = (imax3( Y1, Y2, Y3 )+0xF) >> 4;

            // Block size, standard 8x8 (must be power of two)
            const int q = 8;

            // Start in corner of 8x8 block
            minx &= ~(q - 1);
            miny &= ~(q - 1);

            // Clipping
            if( minx < 0 ){
              minx = 0;
            }
            if( miny < 0 ){
              miny = 0;
            }

            // Half-edge constants
            int C1 = DY12 * X1 - DX12 * Y1;
            int C2 = DY23 * X2 - DX23 * Y2;
            int C3 = DY31 * X3 - DX31 * Y3;

            // Correct for fill convention
            if( DY12 < 0 || (DY12 == 0 && DX12 > 0 )) C1++;
            if( DY23 < 0 || (DY23 == 0 && DX23 > 0 )) C2++;
            if( DY31 < 0 || (DY31 == 0 && DX31 > 0 )) C3++;

            //------------------------------------------------------------------
            // Test that triangled doesn't overwrite pixels.
            //------------------------------------------------------------------

            if( bTestPrimitive ){
              for( int y = miny; y < maxy; y += q ){
                for( int x = minx; x < maxx; x += q ){
                  // Corners of block
                  int x0 = x << 4;
                  int x1 = (x + q - 1) << 4;
                  int y0 = y << 4;
                  int y1 = (y + q - 1) << 4;

                  // Evaluate half-space functions
                  bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
                  bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
                  bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
                  bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
                  int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);

                  bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
                  bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
                  bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
                  bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
                  int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);

                  bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
                  bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
                  bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
                  bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
                  int c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

                  // Skip block when outside an edge
                  if( !a || !b || !c ){
                    continue;
                  }

                  // Accept whole block when totally covered
                  if( a == 0xF && b == 0xF && c == 0xF ){
                    for( int iy=0; iy<q; ++iy ){
                      const real h = real( rasterizer.toH() );
                      if( y+iy>=h ){
                        break;
                      }
                      for( int ix=x; ix<x+q; ++ix ){
                        const real w = real( rasterizer.toW() );
                        if( ix>=w ){
                          break;
                        }
                        if( ix<0 ){
                          continue;
                        }
                        if( rasterizer.onTest( ix, y+iy )){
                          return false;
                        }
                      }
                    }
                    continue;
                  }

                  // Partially covered block
                  int CY1 = C1 + DX12 * y0 - DY12 * x0;
                  int CY2 = C2 + DX23 * y0 - DY23 * x0;
                  int CY3 = C3 + DX31 * y0 - DY31 * x0;
                  for( int iy=y; iy<y+q; ++iy ){
                    const real h = real( rasterizer.toH() );
                    if( iy>=h ){
                      break;
                    }
                    int CX1 = CY1;
                    int CX2 = CY2;
                    int CX3 = CY3;
                    for( int ix=x; ix<x+q; ++ix ){
                      if( CX1 > 0 && CX2 > 0 && CX3 > 0 ){
                        const real w = real( rasterizer.toW() );
                        if( ix>=w ){
                          break;
                        }
                        if( ix<0 ){
                          continue;
                        }
                        if( rasterizer.onTest( ix, iy )){
                          return false;
                        }
                      }
                      CX1 -= FDY12;
                      CX2 -= FDY23;
                      CX3 -= FDY31;
                    }
                    CY1 += FDX12;
                    CY2 += FDX23;
                    CY3 += FDX31;
                  }
                }
              }
            }

            //------------------------------------------------------------------
            // Render triangled using rasterizer.
            //------------------------------------------------------------------

            // Loop through blocks
            for( int y = miny; y < maxy; y += q ){
              for( int x = minx; x < maxx; x += q ){
                // Corners of block
                int x0 = x << 4;
                int x1 = (x + q - 1) << 4;
                int y0 = y << 4;
                int y1 = (y + q - 1) << 4;

                // Evaluate half-space functions
                bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
                bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
                bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
                bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
                int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);

                bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
                bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
                bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
                bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
                int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);

                bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
                bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
                bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
                bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
                int c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

                // Skip block when outside an edge
                if( !a || !b || !c ){
                  continue;
                }

                // Accept whole block when totally covered
                if( a == 0xF && b == 0xF && c == 0xF ){
                  for( int iy=0; iy<q; ++iy ){
                    const real h = real( rasterizer.toH() );
                    const real v = real( y+iy )/h;
                    if( y+iy>=h ){
                      break;
                    }
                    for( int ix=x; ix<x+q; ++ix ){
                      const real w = real( rasterizer.toW() );
                      const real u = real( ix )/w;
                      if( ix>=w ){
                        break;
                      }
                      if( ix<0 ){
                        continue;
                      }
                      rasterizer.onDraw( ix, y+iy, vec2d( u, v ));
                    }
                  }
                  continue;
                }

                // Partially covered block
                int CY1 = C1 + DX12 * y0 - DY12 * x0;
                int CY2 = C2 + DX23 * y0 - DY23 * x0;
                int CY3 = C3 + DX31 * y0 - DY31 * x0;
                for( int iy=y; iy<y+q; ++iy ){
                  const real h = real( rasterizer.toH() );
                  const real v = real( iy )/h;
                  if( iy>=h ){
                    break;
                  }
                  int CX1 = CY1;
                  int CX2 = CY2;
                  int CX3 = CY3;
                  for( int ix=x; ix<x+q; ++ix ){
                    if( CX1 > 0 && CX2 > 0 && CX3 > 0 ){
                      const real w = real( rasterizer.toW() );
                      const real u = real( ix )/w;
                      if( ix>=w ){
                        break;
                      }
                      if( ix<0 ){
                        continue;
                      }
                      rasterizer.onDraw( ix, iy, vec2d( u, v ));
                    }
                    CX1 -= FDY12;
                    CX2 -= FDY23;
                    CX3 -= FDY31;
                  }
                  CY1 += FDX12;
                  CY2 += FDX23;
                  CY3 += FDX31;
                }
              }
            }
          #endif
          return true;
        }
      }

    //}:                                          |
  //}:                                            |
  //Circle:{                                      |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb2d circled::toBounds()const{
          return aabb2d(
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
        template<> bool sphered::intersects( const ray3d& in_tRay, f64& out_fDistance )const{
          const vec3d Q = this->v.xyz() - in_tRay.p;
          const f64& c = Q.length();
          const f64& v = Q.dot( in_tRay.d );
          const f64& d = toRadius()*toRadius() - (c*c - v*v);
          if( d < 0. ){
            out_fDistance = -1.;
            return false;
          }
          out_fDistance = (v - sqrt( d ));
          return true;
        }
      }

    //}:                                          |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb3d sphered::toBounds()const{
          return aabb3d(
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
  //planed:{                                       |

    namespace EON{
      template<> void planed::set( const triangled& in_tTriangle ){
        #if 0
          a() = in_tTriangle.N.x;
          b() = in_tTriangle.N.y;
          c() = in_tTriangle.N.z;
          d() =-(
              a() * in_tTriangle.A.x +
              b() * in_tTriangle.A.y +
              c() * in_tTriangle.A.z );
        #else
          a() = in_tTriangle.A.y*(in_tTriangle.B.z-in_tTriangle.C.z) + in_tTriangle.B.y*(in_tTriangle.C.z-in_tTriangle.A.z) + in_tTriangle.C.y*(in_tTriangle.A.z-in_tTriangle.B.z);
          b() = in_tTriangle.A.z*(in_tTriangle.B.x-in_tTriangle.C.x) + in_tTriangle.B.z*(in_tTriangle.C.x-in_tTriangle.A.x) + in_tTriangle.C.z*(in_tTriangle.A.x-in_tTriangle.B.x);
          c() = in_tTriangle.A.x*(in_tTriangle.B.y-in_tTriangle.C.y) + in_tTriangle.B.x*(in_tTriangle.C.y-in_tTriangle.A.y) + in_tTriangle.C.x*(in_tTriangle.A.y-in_tTriangle.B.y);
          d() =-(in_tTriangle.A.x*(in_tTriangle.B.y*in_tTriangle.C.z-in_tTriangle.C.y*in_tTriangle.B.z)+in_tTriangle.B.x*(in_tTriangle.C.y*in_tTriangle.A.z-in_tTriangle.A.y*in_tTriangle.C.z)+in_tTriangle.C.x*(in_tTriangle.A.y*in_tTriangle.B.z-in_tTriangle.B.y*in_tTriangle.A.z));
        #endif
      }
    }

  //}:                                            |
//}:                                              |
//Algebra:{                                       |
  //Quaternion:{                                  |
    //sqLength:{                                  |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Algebra
  #pragma mark -
#endif

      namespace EON{
        template<> f64 quatd::lengthSquared()const{
          return
            a[0]*a[0] + a[1]*a[1] +
            a[2]*a[2] + a[3]*a[3]
          ;
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void quatd::invert(){
          const f64& norm = lengthSquared();
          if( norm > 0. ){
            const f64& invNorm = 1./norm;
            x =  x * invNorm;
            y = -y * invNorm;
            z = -z * invNorm;
            w = -w * invNorm;
          }else{
            x = y = z = w = 0.;
          }
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //operator::==:{                            |

        namespace EON{
          template<> bool quatd::operator==( const quatd& q )const{
            return( 0 == memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::!=:{                            |

        namespace EON{
          template<> bool quatd::operator!=( const quatd& q )const{
            return( 0 != memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quatd::operator<( const quatd& q )const{
            return( memcmp( a, q.a, sizeof( a )) < 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quatd::operator<=( const quatd& q )const{
            return( memcmp( a, q.a, sizeof( a )) <= 0 );
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quatd::operator>( const quatd& q )const{
            return( memcmp( a, q.a, sizeof( a )) > 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quatd::operator>=( const quatd& q )const{
            return( memcmp( a, q.a, 16 ) >= 0 );
          }
        }

      //}:                                        |
      //operator::+?:{                            |

        namespace EON{
          template<> quatd& quatd::operator+=( const quatd& q ){
            x+=q.x;
            y+=q.y;
            z+=q.z;
            w+=q.w;
            return *this;
          }
        }

        namespace EON{
          template<> quatd quatd::operator+( const quatd& q )const{
            return quatd(
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
          template<> quatd quatd::operator*( const quatd& q )const{
            return quatd(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z
            );
          }
        }

        namespace EON{
          template<> quatd quatd::operator*( const f64& s )const{
            return quatd(
              x * s,
              y * s,
              z * s,
              w * s
            );
          }
        }

        namespace EON{
          template<> quatd& quatd::operator*=( const quatd& q ){
            quatd r(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z );
            *this = r;
            return *this;
          }
        }

        namespace EON{
          template<> quatd& quatd::operator*=( const f64& s ){
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
          template<> quatd& quatd::operator/=( const quatd& q ){
            quatd p( q );
            p.invert();
            (*this)*p;
            return *this;
          }
        }

        namespace EON{
          template<> quatd& quatd::operator/=( const f64& s ){
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
          template<> quatd quatd::operator/( const quatd& q )const{
            quatd p( q );
            p.invert();
            return (*this)*p;
          }
        }

        namespace EON{
          template<> quatd quatd::operator/( const f64& s )const{
            quatd q;
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
        template<> void quatd::setAxisAngle( const vec3d& axis, const f64& angleDeg ){
          const f64& a = rad( angleDeg );
          const vec3d& v = axis*sin( a / 2. );
          x = v.x;
          y = v.y;
          z = v.z;
          w = cos( a/2. );
        }
      }

    //}:                                          |
    //asQuaternion:{                              |

      namespace EON{
        template<> quatd vec3x3d::asQuaternion()const{
          // Use the Graphics Gems code, from
          // ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z
          // *NOT* the "Matrix and Quaternions FAQ", which has errors!

          // the trace is the sum of the diagonal elements; see
          // http://mathworld.wolfram.com/MatrixTrace.html
          f64 t = m[0][0] + m[1][1] + m[2][2];
          f64 x, y, z, w;

          // we protect the division by s by ensuring that s>=1
          if( t >= 0. ){ // |w| >= .5
            f64 s = sqrt( t + 1. ); // |s|>=1 ...
            w = 0.5 * s;
            s = 0.5 / s; // so this division isn't bad
            x = (m[2][1] - m[1][2]) * s;
            y = (m[0][2] - m[2][0]) * s;
            z = (m[1][0] - m[0][1]) * s;
          }else if(( m[0][0] > m[1][1] )&&( m[0][0] > m[2][2] )){
            f64 s = sqrt( 1. + m[0][0] - m[1][1] - m[2][2] ); // |s|>=1
            x = s * 0.5; // |x| >= .5
            s = 0.5 / s;
            y = (m[1][0] + m[0][1]) * s;
            z = (m[0][2] + m[2][0]) * s;
            w = (m[2][1] - m[1][2]) * s;
          }else if( m[1][1] > m[2][2] ){
            f64 s = sqrt( 1. + m[1][1] - m[0][0] - m[2][2] ); // |s|>=1
            y = s * 0.5; // |y| >= .5
            s = 0.5 / s;
            x = (m[1][0] + m[0][1]) * s;
            z = (m[2][1] + m[1][2]) * s;
            w = (m[0][2] - m[2][0]) * s;
          }else{
            f64 s = sqrt( 1. + m[2][2] - m[0][0] - m[1][1] ); // |s|>=1
            z = s * 0.5; // |z| >= .5
            s = 0.5 / s;
            x = (m[0][2] + m[2][0]) * s;
            y = (m[2][1] + m[1][2]) * s;
            w = (m[1][0] - m[0][1]) * s;
          }
          quatd q;
          q.x = x;
          q.y = y;
          q.z = z;
          q.w = w;
          return q;
        }
      }

    //}:                                          |
    //compare:{                                   |

      namespace EON{
        template<> bool quatd::compare( const quatd& q, const f64& tolerance )const{
          if( abs( x - q.x) > tolerance ){
            return false;
          }
          if( abs( y - q.y) > tolerance ){
            return false;
          }
          if( abs( z - q.z) > tolerance ){
            return false;
          }
          if( abs( w - q.w) > tolerance ){
            return false;
          }
          return true;
        }
      }

    //}:                                          |
    //rotate:{                                    |

      namespace EON{
        template<> void quatd::rotate( const vec3d& axis, const f64& angleDeg ){
          *this = quatd( axis, angleDeg ) * *this;
        }
      }

      namespace EON{
        template<> void quatd::rotate( const vec2d& xyMouse ){
          quatd q1 = *this, q2, q3;
          q2.setAxisAngle( vec3d( 0, 0,-1 ), xyMouse.x );
          q3.setAxisAngle( vec3d(-1, 0, 0 ), xyMouse.y );
          q1 = q2 * q1;
          q1 = q1 * q3;
          *this = q1;
        }
      }

    //}:                                          |
    //setEuler:{                                  |

      namespace EON{
        template<> void quatd::setEuler( const vec3d& e ){
          const f64& cos_z_2 = cos( .5*rad( e.z ));
          const f64& cos_y_2 = cos( .5*rad( e.y ));
          const f64& cos_x_2 = cos( .5*rad( e.x ));
          const f64& sin_z_2 = sin( .5*rad( e.z ));
          const f64& sin_y_2 = sin( .5*rad( e.y ));
          const f64& sin_x_2 = sin( .5*rad( e.x ));
          x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
          y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
          z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
          w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
        }
      }

    //}:                                          |
    //getEuler:{                                  |

      namespace EON{
        template<> void quatd::getEuler( f64& out_fX, f64& out_fY, f64& out_fZ, const bool bHomogeneous )const{
          const f64& sqw = w*w;
          const f64& sqx = x*x;
          const f64& sqy = y*y;
          const f64& sqz = z*z;
          if( bHomogeneous ){
            out_fZ = atan2( 2. * (x*y + z*w), sqx - sqy - sqz + sqw );
            out_fY = asin( -2. * (x*z - y*w) );
            out_fX = atan2( 2. * (y*z + x*w),-sqx - sqy + sqz + sqw );
          }else{
            out_fZ = atan2( 2. * (z*y + x*w), 1 - 2*( sqx + sqy ));
            out_fY = asin( -2. * (x*z - y*w));
            out_fX = atan2( 2. * (x*y + z*w), 1 - 2*( sqy + sqz ));
          }
          out_fX = out_fX.deg();
          out_fY = out_fY.deg();
          out_fZ = out_fZ.deg();
        }
      }

    //}:                                          |
    //randomize:{                                 |

      namespace EON{
        template<> void quatd::randomize( const f64& x1, const f64& x2, const f64& x3 ){
          const f64& Z = x1;
          const f64& o = k2PI * x2;
          const f64& r = sqrt( 1. - z * z );
          const f64& W = kPI * x3;
          const f64& sw = sin( w );
          x = sw * cos(o) * r;
          y = sw * sin(o) * r;
          z = sw * Z;
          w = cos( W );
        }
      }

    //}:                                          |
    //get?x?:{                                    |

      namespace EON{
        template<> vec4x4d quatd::get4x4()const{
          vec4x4d R( 0. );
          const f64& xx = x * x;
          const f64& xy = x * y;
          const f64& xz = x * z;
          const f64& xw = x * w;
          const f64& yy = y * y;
          const f64& yz = y * z;
          const f64& yw = y * w;
          const f64& zz = z * z;
          const f64& zw = z * w;
          R( 0, 0 ) = 1. - 2. * (yy + zz);
          R( 1, 0 ) =       2. * (xy - zw);
          R( 2, 0 ) =       2. * (xz + yw);
          R( 0, 1 ) =       2. * (xy + zw);
          R( 1, 1 ) = 1. - 2. * (xx + zz);
          R( 2, 1 ) =       2. * (yz - xw);
          R( 0, 2 ) =       2. * (xz - yw);
          R( 1, 2 ) =       2. * (yz + xw);
          R( 2, 2 ) = 1. - 2. * (xx + yy);
          R( 3, 3 ) = 1.;
          return R;
        }
      }

      namespace EON{
        template<> vec3x3d quatd::get3x3()const{
          vec3x3d R( 0. );
          const f64& xx = x * x;
          const f64& xy = x * y;
          const f64& xz = x * z;
          const f64& xw = x * w;
          const f64& yy = y * y;
          const f64& yz = y * z;
          const f64& yw = y * w;
          const f64& zz = z * z;
          const f64& zw = z * w;
          R( 0, 0 ) = 1. - 2. * (yy + zz);
          R( 1, 0 ) =       2. * (xy - zw);
          R( 2, 0 ) =       2. * (xz + yw);
          R( 0, 1 ) =       2. * (xy + zw);
          R( 1, 1 ) = 1. - 2. * (xx + zz);
          R( 2, 1 ) =       2. * (yz - xw);
          R( 0, 2 ) =       2. * (xz - yw);
          R( 1, 2 ) =       2. * (yz + xw);
          R( 2, 2 ) = 1. - 2. * (xx + yy);
          return R;
        }         
      }

    //}:                                          |
    //length:{                                    |

      namespace EON{
        template<> f64 quatd::length()const{
          return sqrt(
            a[0]*a[0] + a[1]*a[1] +
            a[2]*a[2] + a[3]*a[3]
          );
        }
      }

    //}:                                          |
    //dot:{                                       |

      namespace EON{
        template<> f64 quatd::dot( const quatd& q )const{
          return
            a[0]*q.a[0] + a[1]*q.a[1] +
            a[2]*q.a[2] + a[3]*q.a[3]
          ;
        }
      }

    //}:                                          |
    //normalize:{                                 |

      namespace EON{
        template<> quatd& quatd::normalize(){
          const f64& l = length();
          if( l > .00001 ){
            const f64& rcpLength=1./l;
            x *= rcpLength;
            y *= rcpLength;
            z *= rcpLength;
            w *= rcpLength;
          }else{
            x = y = z = w = 0.;
          }
          return *this;
        }
      }

    //}:                                          |
    //lookat:{                                    |

      namespace EON{
        template<> void quatd::lookat( const pt3d& srcPoint, const pt3d& dstPoint ){
          vec3d direction = dstPoint - srcPoint;
          direction.normalize();
          f64 dot = vec3d::kForward.dot( direction );
          if( f64( dot+1. ).abs() < .000001 ){
            *this = quatd( vec3d::kForward, deg( kPI ));
          }else if( f64( dot-1. ).abs() < .000001 ){
            *this = quatd();
          }else{
            f64 angle = deg( dot.acos() );
            vec3d axis = vec3d::kForward.cross( direction );
            axis.normalize();
            *this = quatd( axis, angle );
          }
        }
      }

    //}:                                          |
    //inverse:{                                   |

      namespace EON{
        template<> quatd quatd::inverse()const{
          quatd inv=*this;
          inv.invert();
          return inv;
        }
      }

    //}:                                          |
    //conjugate:{                                 |

      namespace EON{
        template<> quatd quatd::conjugate()const{
          return quatd( a[0],-a[1],-a[2],-a[3] );
        }
      }

    //}:                                          |
    //exp:{                                       |

      namespace EON{
        template<> quatd quatd::exp()const{
          const f64& a = ((vec3d*)this)->length();
          const f64& sina = sin( a );
          const f64& cosa = cos( a );
          quatd ret;
          ret.w = cosa;
          if( a > 0 ){
            ret.x = sina * x / a;
            ret.y = sina * y / a;
            ret.z = sina * z / a;
          }else{
            ret.x = 0;
            ret.y = 0;
            ret.z = 0;
          }
          return ret;
        }
      }

    //}:                                          |
    //log:{                                       |

      namespace EON{
        template<> quatd quatd::log()const{
          const f64& a = acos( w );
          const f64& sina = sin( a );
          quatd ret;
          ret.w = 0;
          if( sina > 0 ){
            ret.x = a*x/sina;
            ret.y = a*y/sina;
            ret.z = a*z/sina;
          }
          return ret;
        }
      }

    //}:                                          |
    //lerped:{                                    |

      namespace EON{
        template<> quatd quatd::lerped( const quatd& p, const quatd& q, const f64& fTime ){
          quatd result = p*(1-fTime) + q*fTime;
          result.normalize();
          return result;
        }
      }

    //}:                                          |
    //lerp:{                                      |

      namespace EON{
        template<> quatd& quatd::lerp( const quatd& p, const quatd& q, const f64& fTime ){
          *this = lerped( p, q, fTime );
          return *this;
        }
      }

    //}:                                          |
    //slerpedNoInvert:{                           |

      namespace EON{
        template<> quatd quatd::slerpedNoInvert( const quatd& p, const quatd& q, const f64& fTime ){
          const f64& _dot = p.dot( q );
          if(( _dot > -0.95 )&&( _dot < 0.95 )){
            const f64& angle = acos( _dot );
            return( p*sin( angle*( 1.-fTime ))+q*sin( angle*fTime ))/sin( angle );
          }
          // if the angle is small, use linear interpolation
          return lerped( p, q, fTime );
        }
      }

    //}:                                          |
    //slerp:{                                     |

      namespace EON{
        template<> quatd& quatd::slerp( const quatd& p, const quatd& q, const f64& fTime ){
          quatd q3;
          f64 _dot = p.dot( q );
          if( _dot < 0 ){
            _dot = -_dot;
            q3 = q;
            q3.negate();
          }else{
            q3 = q;
          }
          if( _dot < 0.95 ){
            const f64& angle = acos( _dot );
            *this=( p*sin( angle*( 1.-fTime )) + q3*sin( angle*fTime ))/sin( angle );
          }else{// if the angle is small, use linear interpolation
            *this=lerped( p, q3, fTime );
          }
          return *this;
        }
      }

    //}:                                          |
    //squad:{                                     |

      namespace EON{
        template<> quatd& quatd::squad( const quatd& q1, const quatd& q2, const quatd& a, const quatd& b, const f64& fTime ){
          const quatd& c = slerpedNoInvert( q1, q2, fTime );
          const quatd& d = slerpedNoInvert(  a,  b, fTime );
          *this = slerpedNoInvert( c, d, 2*fTime*( 1.-fTime ));
          return *this;
        }
      }

    //}:                                          |
    //align:{                                     |

      namespace EON{
        template<> quatd& quatd::align( const vec3d& up, const vec3d& norm ){
          vec3d w = up.cross( norm );
          *this = quatd( w.x, w.y, w.z, 1. + up.dot( norm ));
          return *this;
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //vec3d:{                                      |
      //[operators]:{                             |
        //operator=:{                             |

          namespace EON{
            template<> vec3d& vec3d::operator=( const vec4d& V ){
              x = V.x;
              y = V.y;
              z = V.z;
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
      //Ctor:{                                    |

        namespace EON{
          template<> vec3d::Vector3( const vec4d& V ){
            x = V.x;
            y = V.y;
            z = V.z;
          }
        }

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Matrices:{                                    |
    //vec4x4d:{                                   |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Matrices
  #pragma mark -
#endif

        namespace EON{
          template<> vec4x4d& vec4x4d::operator*=( const vec4x4d& B ){
            vec4x4d A = *this;
            for( u32 i=0; i<4; ++i ){
              for( u32 j=0; j<4; ++j ){
                f64 sum = 0;
                for( u32 e=0; e<4; ++e ){
                  sum += A.m[i][e] * B.m[e][j];
                }
                m[i][j]=sum;
              }
            }
            return *this;
          }
        }

        //}:                                      |
      //}:                                        |
      //mulTranspose:{                            |

        namespace EON{
          template<> vec4x4d& vec4x4d::mulTranspose( const vec4x4d& b ){
            Matrix4 a = *this;
            XX = a.XX * b.XX + a.YX * b.XY + a.ZX * b.XZ + a.WX * b.XW;
            XY = a.XY * b.XX + a.YY * b.XY + a.ZY * b.XZ + a.WY * b.XW;
            XZ = a.XZ * b.XX + a.YZ * b.XY + a.ZZ * b.XZ + a.WZ * b.XW;
            XW = a.XW * b.XX + a.YW * b.XY + a.ZW * b.XZ + a.WW * b.XW;
            YX = a.XX * b.YX + a.YX * b.YY + a.ZX * b.YZ + a.WX * b.YW;
            YY = a.XY * b.YX + a.YY * b.YY + a.ZY * b.YZ + a.WY * b.YW;
            YZ = a.XZ * b.YX + a.YZ * b.YY + a.ZZ * b.YZ + a.WZ * b.YW;
            YW = a.XW * b.YX + a.YW * b.YY + a.ZW * b.YZ + a.WW * b.YW;
            ZX = a.XX * b.ZX + a.YX * b.ZY + a.ZX * b.ZZ + a.WX * b.ZW;
            ZY = a.XY * b.ZX + a.YY * b.ZY + a.ZY * b.ZZ + a.WY * b.ZW;
            ZZ = a.XZ * b.ZX + a.YZ * b.ZY + a.ZZ * b.ZZ + a.WZ * b.ZW;
            ZW = a.XW * b.ZX + a.YW * b.ZY + a.ZW * b.ZZ + a.WW * b.ZW;
            WX = a.XX * b.WX + a.YX * b.WY + a.ZX * b.WZ + a.WX * b.WW;
            WY = a.XY * b.WX + a.YY * b.WY + a.ZY * b.WZ + a.WY * b.WW;
            WZ = a.XZ * b.WX + a.YZ * b.WY + a.ZZ * b.WZ + a.WZ * b.WW;
            WW = a.XW * b.WX + a.YW * b.WY + a.ZW * b.WZ + a.WW * b.WW;
            return *this;
          }
        }

      //}:                                        |
      //setPerspectiveV2C:{                       |

        namespace EON{
          template<> void vec4x4d::setPerspectiveV2C( const f64& degFOV, const f64& aspect, const f64& zn, const f64& zf ){
            // f = 1. / tanf( rad( degFOV )*.5 )
            // a = aspect ratio
            // +--         --+   +--        --+
            // | XX XY XZ XW |   | RR RR RR 0 |
            // | YX YY YZ YW | = | RR RR RR 0 |
            // | ZX ZY ZZ ZW | = | RR RR RR 0 |
            // | WX WY WZ WW |   | Tx Ty Tz 1 |
            // +--         --+   +--        --+
            // +--                                            --+
            // | f/a  0  0                 0                    |
            // | 0    f  0                 0                    |
            // | 0    0  -(zf+zn)/(zn-zf)  (-2.*zf*zn)/(zn-zf)  |
            // | 0    0  -1                0                    |
            // +--                                            --+
            const f64& f = 1. / tan( rad( degFOV )*.5 );
            memset( this, 0, sizeof( vec4x4d ));
            XX = f / aspect;
            YY = f;
            ZZ = -(zf+zn) / (zn-zf);
            ZW = (-2. * zf * zn) / (zn-zf);
            WZ = -1.;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec4x4d::setTranslation( const f64& x, const f64& y, const f64& z ){
            XX = 1; XY = 0; XZ = 0; XW = 0;
            YX = 0; YY = 1; YZ = 0; YW = 0;
            ZX = 0; ZY = 0; ZZ = 1; ZW = 0;
            WX = x; WY = y; WZ = z; WW = 1;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec4x4d::setScale( const f64& x, const f64& y, const f64& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

        namespace EON{
          template<> void vec4x4d::setScale( const f64& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec4x4d::setRotX( const f64& angleInDeg ){
            const f64& f = rad( angleInDeg );
            // [ 0 4      8 12 ]
            // [ 1 cos -sin 13 ]
            // [ 2 sin cos  14 ]
            // [ 3 7     11 15 ]
            t[10] = t[ 5] = cos( f );
            t[ 6] = sin( f );
            t[ 9] =-t[ 6];
            t[ 0] = t[15] = 1.;
            t[ 1] = t[ 2] = t[ 3] = t[ 4] =
            t[ 7] = t[ 8] = t[11] = t[12] =
            t[13] = t[14] = 0.;
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec4x4d::setRotY( const f64& angleInDeg ){
            const f64& f = rad( angleInDeg );
            // [ cos 4  -sin 12 ]
            // [ 1   5   9   13 ]
            // [ sin 6  cos  14 ]
            // [ 3   7  11   15 ]
            t[ 0] = t[10] = cos( f );
            t[ 2] = sin( f );
            t[ 8] =-t[ 2];
            t[ 5] = t[15] = 1.;
            t[ 1] = t[ 3] = t[ 4] = t[ 6] =
            t[ 7] = t[ 9] = t[11] = t[12] =
            t[13] = t[14] = 0.;
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec4x4d::setRotZ( const f64& angleInDeg ){
            const f64& f = rad( angleInDeg );
            // [ cos -sin 8 12 ]
            // [ sin cos  9 13 ]
            // [ 2   6   10 14 ]
            // [ 3   7   11 15 ]
            t[ 0] = t[ 5] = cos( f );
            t[ 1] = sin( f );
            t[ 4] =-t[ 1];
            t[10] = t[15] = 1.;
            t[ 2] = t[ 3] = t[ 6] = t[ 7] =
            t[ 8] = t[ 9] = t[11] = t[12] =
            t[13] = t[14] = 0.;
          }
        }

      //}:                                        |
      //setAxisIdentity:{                         |

        namespace EON{
          template<> void vec4x4d::setAxisIdentity(){
            YX = ZX = WX = XY = WY = YY = 0;
            XZ = WZ = XW = YW = ZW = ZZ = 0;
            XX = .01f;
            YZ = .01f;
            ZY = .01f;
            WW = 1;
          }
        }

      //}:                                        |
      //transpose:{                               |

        namespace EON{
          template<> void vec4x4d::transpose(){
            vec4x4d M=*this;
            XY=M.YX; XZ=M.ZX; XW=M.WX;
            YX=M.XY; YZ=M.ZY; YW=M.WY;
            ZX=M.XZ; ZY=M.YZ; ZW=M.WZ;
            WX=M.XW; WY=M.YW; WZ=M.ZW;
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace{
          bool invertMatrix( const double m[16], double inv[16] ){
            inv[ 0] = m[ 5] * m[10] * m[15] - 
              m[ 5] * m[11] * m[14] - 
              m[ 9] * m[ 6] * m[15] + 
              m[ 9] * m[ 7] * m[14] +
              m[13] * m[ 6] * m[11] - 
              m[13] * m[ 7] * m[10];
            inv[ 4] =-m[ 4] * m[10] * m[15] + 
              m[ 4] * m[11] * m[14] + 
              m[ 8] * m[ 6] * m[15] - 
              m[ 8] * m[ 7] * m[14] - 
              m[12] * m[ 6] * m[11] + 
              m[12] * m[ 7] * m[10];
            inv[ 8] = m[ 4] * m[ 9] * m[15] - 
              m[ 4] * m[11] * m[13] - 
              m[ 8] * m[ 5] * m[15] + 
              m[ 8] * m[ 7] * m[13] + 
              m[12] * m[ 5] * m[11] - 
              m[12] * m[ 7] * m[ 9];
            inv[12] = -m[4] * m[ 9] * m[14] + 
              m[ 4] * m[10] * m[13] +
              m[ 8] * m[ 5] * m[14] - 
              m[ 8] * m[ 6] * m[13] - 
              m[12] * m[ 5] * m[10] + 
              m[12] * m[ 6] * m[ 9];
            inv[ 1] =-m[ 1] * m[10] * m[15] + 
              m[ 1] * m[11] * m[14] + 
              m[ 9] * m[ 2] * m[15] - 
              m[ 9] * m[ 3] * m[14] - 
              m[13] * m[ 2] * m[11] + 
              m[13] * m[ 3] * m[10];
            inv[ 5] = m[ 0] * m[10] * m[15] - 
              m[ 0] * m[11] * m[14] - 
              m[ 8] * m[ 2] * m[15] + 
              m[ 8] * m[ 3] * m[14] + 
              m[12] * m[ 2] * m[11] - 
              m[12] * m[ 3] * m[10];
            inv[ 9] =-m[ 0] * m[9] * m[15] + 
              m[ 0] * m[11] * m[13] + 
              m[ 8] * m[ 1] * m[15] - 
              m[ 8] * m[ 3] * m[13] - 
              m[12] * m[ 1] * m[11] + 
              m[12] * m[ 3] * m[ 9];
            inv[13] = m[ 0] * m[ 9] * m[14] - 
              m[ 0] * m[10] * m[13] - 
              m[ 8] * m[ 1] * m[14] + 
              m[ 8] * m[ 2] * m[13] + 
              m[12] * m[ 1] * m[10] - 
              m[12] * m[ 2] * m[ 9];
            inv[ 2] = m[ 1] * m[ 6] * m[15] - 
              m[ 1] * m[ 7] * m[14] - 
              m[ 5] * m[ 2] * m[15] + 
              m[ 5] * m[ 3] * m[14] + 
              m[13] * m[ 2] * m[ 7] - 
              m[13] * m[ 3] * m[ 6];
            inv[ 6] =-m[ 0] * m[ 6] * m[15] + 
              m[ 0] * m[ 7] * m[14] + 
              m[ 4] * m[ 2] * m[15] - 
              m[ 4] * m[ 3] * m[14] - 
              m[12] * m[ 2] * m[ 7] + 
              m[12] * m[ 3] * m[ 6];
            inv[10] = m[ 0] * m[5] * m[15] - 
              m[ 0] * m[ 7] * m[13] - 
              m[ 4] * m[ 1] * m[15] + 
              m[ 4] * m[ 3] * m[13] + 
              m[12] * m[ 1] * m[ 7] - 
              m[12] * m[ 3] * m[ 5];
            inv[14] =-m[ 0] * m[ 5] * m[14] + 
              m[ 0] * m[ 6] * m[13] + 
              m[ 4] * m[ 1] * m[14] - 
              m[ 4] * m[ 2] * m[13] - 
              m[12] * m[ 1] * m[ 6] + 
              m[12] * m[ 2] * m[ 5];
            inv[ 3] =-m[ 1] * m[ 6] * m[11] + 
              m[ 1] * m[ 7] * m[10] + 
              m[ 5] * m[ 2] * m[11] - 
              m[ 5] * m[ 3] * m[10] - 
              m[ 9] * m[ 2] * m[ 7] + 
              m[ 9] * m[ 3] * m[ 6];
            inv[ 7] = m[ 0] * m[ 6] * m[11] - 
              m[ 0] * m[ 7] * m[10] - 
              m[ 4] * m[ 2] * m[11] + 
              m[ 4] * m[ 3] * m[10] + 
              m[ 8] * m[ 2] * m[ 7] - 
              m[ 8] * m[ 3] * m[ 6];
            inv[11] =-m[ 0] * m[ 5] * m[11] + 
              m[ 0] * m[ 7] * m[ 9] + 
              m[ 4] * m[ 1] * m[11] - 
              m[ 4] * m[ 3] * m[ 9] - 
              m[ 8] * m[ 1] * m[ 7] + 
              m[ 8] * m[ 3] * m[ 5];
            inv[15] = m[ 0] * m[ 5] * m[10] - 
              m[ 0] * m[ 6] * m[ 9] - 
              m[ 4] * m[ 1] * m[10] + 
              m[ 4] * m[ 2] * m[ 9] + 
              m[ 8] * m[ 1] * m[ 6] - 
              m[ 8] * m[ 2] * m[ 5];
            double det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
            if( !det ){
              return false;
            }
            det = 1. / det;
            for( u32 i=0; i<16; ++i ){
              inv[i] *= det;
            }
            return true;
          }
        }

        namespace EON{
          template<> void vec4x4d::invert(){
            const vec4x4d M = *this;
            if( !invertMatrix( (double*)M.t, (double*)t )){
              setIdentity();
            }
          }
          template<> vec4x4d vec4x4d::inverse()const{
            vec4x4d M;
            if( !invertMatrix( (double*)t, (double*)M.t )){
              M.setIdentity();
            }
            return M;
          }
        }

        namespace EON{
          template<> void vec4x4d::removeTranslation(){
            WX = WY = WZ = 0;
          }
        }

        namespace EON{
          template<> void vec4x4d::removeScale(){
            Vector3 vx = Vector3( XX, YX, ZX );
            Vector3 vy = Vector3( XY, YY, ZY );
            Vector3 vz = Vector3( XZ, YZ, ZZ );
            Vector3 v;
            v.x = 1./vx.length();
            v.y = 1./vy.length();
            v.z = 1./vz.length();
            XX *= v.x; YX *= v.x; ZX *= v.x;
            XY *= v.y; YY *= v.y; ZY *= v.y;
            XZ *= v.z; YZ *= v.z; ZZ *= v.z;
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x3d:{                                   |
      //setAxisAngle:{                            |

        namespace EON{
          template<> void vec3x3d::setAxisAngle( const vec3d& axis, const f64& angle ){
            const f64& cs          = cos( angle );
            const f64& sn          = sin( angle );
            const f64& oneMinusCos = k1-cs;
            const f64& x2          = axis.x*axis.x;
            const f64& y2          = axis.y*axis.y;
            const f64& z2          = axis.z*axis.z;
            const f64& xym         = axis.x*axis.y*oneMinusCos;
            const f64& xzm         = axis.x*axis.z*oneMinusCos;
            const f64& yzm         = axis.y*axis.z*oneMinusCos;
            const f64& xSin        = axis.x*sn;
            const f64& ySin        = axis.y*sn;
            const f64& zSin        = axis.z*sn;
            m[0][0] = x2*oneMinusCos + cs;
            m[0][1] = xym - zSin;
            m[0][2] = xzm + ySin;
            m[1][0] = xym + zSin;
            m[1][1] = y2*oneMinusCos + cs;
            m[1][2] = yzm - xSin;
            m[2][0] = xzm - ySin;
            m[2][1] = yzm + xSin;
            m[2][2] = z2*oneMinusCos + cs;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec3x3d::setScale( const f64& x, const f64& y, const f64& z ){
            XX = x; YX = 0; ZX = 0;
            XY = 0; YY = y; ZY = 0;
            XZ = 0; YZ = 0; ZZ = z;
          }
          template<> void vec3x3d::setScale( const f64& s ){
            XX = s; YX = 0; ZX = 0;
            XY = 0; YY = s; ZY = 0;
            XZ = 0; YZ = 0; ZZ = s;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x3d::setRotX( const f64& angleInDeg ){
            // [ 0   3    6 ]
            // [ 1 cos -sin ]
            // [ 2 sin  cos ]
            const f64& a = rad( angleInDeg );
            t[4] = t[8] = cos( a );
            t[5] = sin( a );
            t[7] =-t[5];
            t[0] = 1;
            t[3] = t[6] = t[1] = t[2] = 0;
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec3x3d::setRotY( const f64& angleInDeg ){
            // [ cos 3 -sin ]
            // [ 1   4    7 ]
            // [ sin 5  cos ]
            const f64& a = rad( angleInDeg );
            t[0] = t[8] = cos( a );
            t[2] = sin( a );
            t[6] =-t[2];
            t[4] = 1;
            t[1] = t[5] = t[7] = 0;
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec3x3d::setRotZ( const f64& angleInDeg ){
            // [ cos -sin 6 ]
            // [ sin  cos 7 ]
            // [ 2    5   8 ]
            const f64& a = rad( angleInDeg );
            t[0] = t[4] = cos( a );
            t[1] = sin( a );
            t[3] =-t[1];
            t[8] = 1;
            t[2] = t[5] = t[6] = t[7] = 0;
          }
        }

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Frustum:{                                     |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Frustums
  #pragma mark -
#endif

      namespace EON{
        template<> bool frustumd::intersects( const pt3d& P )const{
          const planed* next = m_aPlanes;
          const planed* end = next + 6;
          while( next < end ){
            const f64& t = next->distance( P );
            if( t < kEpsilon ){
              return false;
            }
            ++next;
          }
          return true;
        }
      }

      namespace EON{
        template<> bool frustumd::intersects( const sphered& S )const{
          const planed* next = m_aPlanes;
          const planed* end = next+6;
          while( next < end ){
            const f64 d = next->distance( S.toCenter() );
            if( d >= 2. * S.toRadius() ){
              return false;
            }
            ++next;
          }
          //fully inside.
          return true;
        }
      }

      namespace EON{
        template<> bool frustumd::intersects( const aabb3d& B )const{
          const planed* next = m_aPlanes;
          const planed* end = next+6;
          while( next < end ){
            pt3d a,b;
            if( next->Vector4::x >= 0. ){
              a.x=B.min.x;
              b.x=B.max.x;
            }else{
              b.x=B.min.x;
              a.x=B.max.x;
            }
            if( next->Vector4::y >= 0. ){
              a.y=B.min.y;
              b.y=B.max.y;
            }else{
              b.y=B.min.y;
              a.y=B.max.y;
            }
            if( next->Vector4::z >=0. ){
              a.z=B.min.z;
              b.z=B.max.z;
            }else{
              b.z=B.min.z;
              a.z=B.max.z;
            }
            int l1;
            { const f64& d = next->distance( a );
              if( d > kEpsilon ){
                l1 = 1;
              }else if( d < -kEpsilon ){
                l1 = -1;
              }else{
                l1 = 0;
              }
            }
            int l2;
            { const f64& d = next->distance( b );
              if( d > kEpsilon ){
                l2 = 1;
              }else if( d < -kEpsilon ){
                l2 = -1;
              }else{
                l2 = 0;
              }
            }
            if(( l1 == l2 )&&( l1 < 0 )){
              return false;
            }
            ++next;
          }
          return true;
        }
      }

    //}:                                          |
    //set:{                                       |

      namespace EON{
        template<> void frustumd::set( const vec4x4d& in_tW2C ){
          vec4x4d W2C = in_tW2C;
          const f64& xw = W2C[ 3];
          const f64& yw = W2C[ 7];
          const f64& zw = W2C[11];
          const f64& ww = W2C[15];

          const f64& xz = W2C[ 2];
          const f64& yz = W2C[ 6];
          const f64& zz = W2C[10];
          const f64& wz = W2C[14];

          // near
          m_aPlanes[0].x = xw - xz;
          m_aPlanes[0].y = yw - yz;
          m_aPlanes[0].z = zw - zz;
          m_aPlanes[0].w = ww - wz;

          // far
          m_aPlanes[1].x = xw + xz;
          m_aPlanes[1].y = yw + yz;
          m_aPlanes[1].z = zw + zz;
          m_aPlanes[1].w = ww + wz;

          const f64& xx = W2C[ 0];
          const f64& yx = W2C[ 4];
          const f64& zx = W2C[ 8];
          const f64& wx = W2C[12];

          // left
          m_aPlanes[2].x = xw - xx;
          m_aPlanes[2].y = yw - yx;
          m_aPlanes[2].z = zw - zx;
          m_aPlanes[2].w = ww - wx;

          // right
          m_aPlanes[3].x = xw + xx;
          m_aPlanes[3].y = yw + yx;
          m_aPlanes[3].z = zw + zx;
          m_aPlanes[3].w = ww + wx;

          const f64& xy = W2C[ 1];
          const f64& yy = W2C[ 5];
          const f64& zy = W2C[ 9];
          const f64& wy = W2C[13];

          // top
          m_aPlanes[4].x = xw + xy;
          m_aPlanes[4].y = yw + yy;
          m_aPlanes[4].z = zw + zy;
          m_aPlanes[4].w = ww + wy;

          // bottom
          m_aPlanes[5].x = xw - xy;
          m_aPlanes[5].y = yw - yy;
          m_aPlanes[5].z = zw - zy;
          m_aPlanes[5].w = ww - wy;
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //Vector4:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec4d vec4d::operator*( const vec4x4d& M )const{
            return vec4d(
              M.XX * x + M.YX * y + M.ZX * z + M.WX * w,
              M.XY * x + M.YY * y + M.ZY * z + M.WY * w,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ * w,
              M.XW * x + M.YW * y + M.ZW * z + M.WW * w
            );
          }
          template<> vec4d vec4d::operator*( const vec3x3d& M )const{
            return vec4d(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z,
              f64( 1. )
            );
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector3:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec3d vec3d::operator*( const vec4x4d& M )const{
            return vec3d(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> vec3d vec3d::operator*( const vec3x3d& M )const{
            return vec3d(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z
            );
          }
        }

      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Points:{                                      |
    //[operators]:{                               |
      //operator-:{                               |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Points
  #pragma mark -
#endif

        namespace EON{
          template<> pt3d pt3d::operator-( const vec3d& v )const{
            pt3d r;
            r.x = x-v.x;
            r.y = y-v.y;
            r.z = z-v.z;
            return r;
          }
        }

      //}:                                        |
      //operator+:{                               |

        namespace EON{
          template<> pt3d pt3d::operator+( const vec3d& v )const{
            pt3d r;
            r.x = x+v.x;
            r.y = y+v.y;
            r.z = z+v.z;
            return r;
          }
        }

      //}:                                        |
      //operator/:{                               |

        namespace EON{
          template<> pt3d pt3d::operator/( const vec3d& v )const{
            pt3d r;
            r.x = x/v.x;
            r.y = y/v.y;
            r.z = z/v.z;
            return r;
          }
        }

      //}:                                        |
      //operator*:{                               |

        namespace EON{
          template<> pt3d pt3d::operator*( const vec4x4d& M )const{
            return pt3d(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> pt3d pt3d::operator*( const vec3x3d& M )const{
            return pt3d(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z
            );
          }
          template<> pt3d pt3d::operator*( const vec3d& v )const{
            pt3d r;
            r.x = x*v.x;
            r.y = y*v.y;
            r.z = z*v.z;
            return r;
          }
        }

      //}:                                        |
    //}:                                          |
    //Methods:{                                   |

      namespace EON{
        template<> pt2d pt2d::scaledToClip( const f64& scale )const{
          pt2d p;
          p.x = (x/1024*2.-1.)*scale;
          p.y = (y/768*2.-1.)*scale;
          return p;
        }
      }

      namespace EON{
        template<> void pt2d::scaleToClip( const f64& scale ){
          x = (x/1024*2.-1.)*scale;
          y = (y/768*2.-1.)*scale;
        }
      }

    //}:                                          |
    //[ctors]:{                                   |

      namespace EON{
        template<> pt3d::Point3( const vec4d& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

      namespace EON{
        template<> pt3d::Point3( const vec3d& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

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
        template<> vec4x4d qst3d::get4x4()const{
          const vec4x4d& T = vec4x4d::getT( m_tPosition );
          const vec4x4d& R = toRotation().get4x4();
          vec4x4d out( m_fScale );
          out *= R;
          out *= T;
          return out;
        }
      }

      namespace EON{
        template<> vec3x3d qst3d::get3x3()const{
          vec3x3d out( m_fScale );
          return out * toRotation().get3x3();
        }
      }

      namespace EON{
        template<> vec4x4d qst3d::get4x4RT()const{
          const vec4x4d& T = vec4x4d::getT( m_tPosition );
          const vec4x4d& R = toRotation().get4x4();
          return vec4x4d( 1. ) * R * T;
        }
      }

      namespace EON{
        template<> vec4x4d qst3d::get4x4ST()const{
          const vec4x4d out( m_fScale );
          return out * vec4x4d::getT( m_tPosition );
        }
      }

      namespace EON{
        template<> vec4x4d qst3d::get4x4T()const{
          return vec4x4d::getT( m_tPosition );
        }
      }

      namespace EON{
        template<> vec4x4d qst3d::get4x4R()const{
          return toRotation().get4x4();
        }
      }

      namespace EON{
        template<> vec3x3d qst3d::get3x3R()const{
          return toRotation().get3x3();
        }
      }

      namespace EON{
        template<> vec4x4d qst3d::get4x4S()const{
          return vec4x4d( m_fScale );
        }
      }

      namespace EON{
        template<> vec3x3d qst3d::get3x3S()const{
          return vec3x3d( m_fScale );
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void qst3d::invert(){
          toPosition().negate();
          toRotation().invert();
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //Multiply:{                                |

        namespace EON{
          template<> pt3d qst3d::operator*( const pt3d& P )const{
            return P * get4x4();
          }
        }

        namespace EON{
          template<> void qst3d::operator*=( const qst3d& L2W ){
            x() += L2W.m_tPosition.x;
            y() += L2W.m_tPosition.y;
            z() += L2W.m_tPosition.z;
            quatd q = L2W.m_qRotation;
            q.normalize();
            m_qRotation *= q;
            m_qRotation.normalize();
            m_fScale *= L2W.m_fScale;
          }
        }

      //}:                                        |
      //Array:{                                   |

        namespace EON{
          template<> f64 qst3d::operator[]( const u32 i )const{
            e_assert( i < 7 );
            f64 euler[ 3 ];
            switch( i ){
              case 0:
              case 1:
              case 2:
                return m_tPosition[ i ];
              case 3:
              case 4:
              case 5:
                m_qRotation.getEuler( euler[ 0 ], euler[ 1 ], euler[ 2 ]);
                return euler[i-3];
            }
            return m_fScale;
          }
        }

      //}:                                        |
    //}:                                          |
  //}:                                            |
//}:                                              |
//Random:{                                        |
  //e_rand<>:{                                    |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Random
  #pragma mark -
#endif

    template<> f64 e_rand<f64>( const f64 start, const f64 end ){
      std::uniform_real_distribution<double> dis((double( start )), double( end ));
      return dis( gen64() );
    }

    template<> f64 e_rand<f64>(){
      std::uniform_real_distribution<double> dis;
      return dis( gen64() );
    }

    template<> double e_rand<double>(double start,double end ){
      std::uniform_real_distribution<double> dis( start, end );
      return dis( gen64() );
    }

    template<> double e_rand<double>(){
      std::uniform_real_distribution<double> dis;
      return dis( gen64() );
    }

    template<> u64 e_rand<u64>( const u64 start, const u64 end ){
      std::uniform_int_distribution<u64> dis( start, end );
      return dis( gen64() );
    }

    template<> u64 e_rand<u64>(){
      std::uniform_int_distribution<s64> dis;
      return dis( gen64() );
    }

    template<> s64 e_rand<s64>( const s64 start, const s64 end ){
      std::uniform_int_distribution<s64> dis( start, end );
      return dis( gen64() );
    }

    template<> s64 e_rand<s64>(){
      std::uniform_int_distribution<s64> dis;
      return dis( gen64() );
    }

  //}:                                            |
  //e_randunitvec*:{                              |

    vec2d e_randunitvec2d(){
      const f64& t = e_rand2pi();
      const f64& s = t.sin();
      const f64& c = t.cos();
      return vec2d( c, s );
    }

    vec3d e_randunitvec3d(){
      const f64& theta = e_rand2pi();
      const f64& r = e_randunit().sqrt();
      const f64& z = f64::sqrt( 1. - r*r )*( e_rand<u32>( 0, 1 ) ? -1. : 1. );
      return vec3d( r * theta.cos(), r * theta.sin(), z );
    }

  //}:                                            |
  //e_randunit:{                                  |

    f64 e_randunitd(){
      return e_rand<f64>( 0., 1. );
    }

  //}:                                            |
  //e_rand2pi:{                                   |

    f64 e_rand2pid(){
      return e_randpid()*2.;
    }

  //}:                                            |
  //e_randpi:{                                    |

    f64 e_randpid(){
      return e_randunitd() * f64::kPI;
    }

  //}:                                            |
//}:                                              |
//Bounds:{                                        |
  //aabb3d:{                                       |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Bounding volumes
  #pragma mark -
#endif

      //http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
      namespace EON{
        template<> bool aabb3d::intersects( const ray3d& inRay, f64& outDst )const{
          vec3d dirfrac;
          const pt3d& lb = min;
          const pt3d& rt = max;
          // r.dir is unit direction vector of ray
          const vec3d d = inRay.d.normalized();
          dirfrac.x = 1. / d.x;
          dirfrac.y = 1. / d.y;
          dirfrac.z = 1. / d.z;
          // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
          // r.org is origin of ray
          const f64 t1 = (lb.x - inRay.p.x)*dirfrac.x;
          const f64 t2 = (rt.x - inRay.p.x)*dirfrac.x;
          const f64 t3 = (lb.y - inRay.p.y)*dirfrac.y;
          const f64 t4 = (rt.y - inRay.p.y)*dirfrac.y;
          const f64 t5 = (lb.z - inRay.p.z)*dirfrac.z;
          const f64 t6 = (rt.z - inRay.p.z)*dirfrac.z;
          const f64 tmin = f64::max( f64::max( t1.min( t2 ), t3.min( t4 )), t5.min( t6 ));
          const f64 tmax = f64::min( f64::min( t1.max( t2 ), t3.max( t4 )), t5.max( t6 ));
          // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behind us
          if( tmax < 0. ){
            outDst = tmax;
            return false;
          }
          // if tmin > tmax, ray doesn't intersect AABB
          if( tmin > tmax ){
            outDst = tmax;
            return false;
          }
          outDst = tmin;
          return true;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb3d::contains( const pt3d& P )const{
          if( P.x < min.x ){
            return false;
          }
          if( P.y < min.y ){
            return false;
          }
          if( P.z < min.z ){
            return false;
          }
          if( P.x > max.x ){
            return false;
          }
          if( P.y > max.y ){
            return false;
          }
          if( P.z > max.z ){
            return false;
          }
          return true;
        }
      }

      namespace EON{
        template<> bool aabb3d::contains( const aabb3d& B )const{
          if( !contains( B.min )){
            return false;
          }
          if( !contains( B.max )){
            return false;
          }
          return true;
        }
      }

      namespace EON{
        template<> bool aabb3d::contains( const triangled& t )const{
          if( !contains( t.A )){
            return false;
          }
          if( !contains( t.B )){
            return false;
          }
          if( !contains( t.C )){
            return false;
          }
          return true;
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb3d::clear(){
          max.x = kMin;
          max.y = kMin;
          max.z = kMin;
          min.x = kMax;
          min.y = kMax;
          min.z = kMax;
        }
      }

    //}:                                          |
  //}:                                            |
  //aabb2d:{                                       |
    //scaledToClip:{                              |

      namespace EON{
        template<> aabb2d aabb2d::scaledToClip( const f64& scale )const{
          aabb2d b = *this;
          b.min.scaleToClip( scale );
          b.max.scaleToClip( scale );
          return b;
        }
      }

    //}:                                          |
    //scaleToClip:{                               |

      namespace EON{
        template<> void aabb2d::scaleToClip( const f64& scale ){
          min.scaleToClip( scale );
          max.scaleToClip( scale );
        }
      }

    //}:                                          |
    //scaleBack:{                                 |

      namespace EON{
        template<> void aabb2d::scaleBack(){
          const f64 cx = 1024;
          const f64 cy = 768;
          min.x = (min.x+1.)*.5*cx;
          min.y = (min.y+1.)*.5*cy;
          max.x = (max.x+1.)*.5*cx;
          max.y = (max.y+1.)*.5*cy;
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb2d::empty()const{
          e_assert( min.valid() );
          e_assert( max.valid() );
          if( min.x != kMax ){
            return false;
          }
          if( min.y != kMax ){
            return false;
          }
          if( max.x != kMin ){
            return false;
          }
          if( max.y != kMin ){
            return false;
          }
          return true;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb2d::valid()const{
          if( !min.valid() ){
            return false;
          }
          if( !max.valid() ){
            return false;
          }
          if( !empty() ){
            if( min.x > max.x ){
              return false;
            }
            if( min.y > max.y ){
              return false;
            }
          }
          return true;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb2d::contains( const pt2d& P )const{
          return(( P.x >= min.x )&&( P.y >= min.y )&&( P.x < max.x )&&( P.y < max.y ));
        }
      }

      namespace EON{
        template<> bool aabb2d::contains( const triangled& t )const{
          if( !contains( t.A.xy() )){
            return false;
          }
          if( !contains( t.B.xy() )){
            return false;
          }
          if( !contains( t.C.xy() )){
            return false;
          }
          return true;
        }
      }

      namespace EON{
        template<> bool aabb2d::contains( const aabb2d& B )const{
          if( B.empty() ){
            return false;
          }
          if( !contains( B.min )&& !contains( B.max )){
            return false;
          }
          return true;
        }
      }

    //}:                                          |
    //intersects:{                                |

      namespace EON{
        template<> bool aabb2d::intersects( const ray2d& r )const{
          const pt2d v[2]={ r.p, r.p + r.d };
          return line2rect( v, *this );
        }
      }

      namespace EON{
        template<> bool aabb2d::intersects( const aabb2d& b )const{

          //--------------------------------------------------------------------
          // Get all the corners.
          //--------------------------------------------------------------------

          const pt2d bA = b.min;
          const pt2d bB = pt2d( b.max.x, b.min.y );
          const pt2d bC = b.max;
          const pt2d bD = pt2d( b.min.x, b.max.y );

          //--------------------------------------------------------------------
          // Do any of b points appear in this?
          //--------------------------------------------------------------------

          if( contains( bA )){
            return true;
          }
          if( contains( bB )){
            return true;
          }
          if( contains( bC )){
            return true;
          }
          if( contains( bD )){
            return true;
          }

          //--------------------------------------------------------------------
          // Get all the edges.
          //--------------------------------------------------------------------

          const ray2d bEdge0( bA, bB-bA );
          const ray2d bEdge1( bB, bC-bB );
          const ray2d bEdge2( bC, bD-bC );
          const ray2d bEdge3( bD, bA-bD );
          const ray2d bDiag0( bA, bC-bA );
          const ray2d bDiag1( bB, bD-bB );

          //--------------------------------------------------------------------
          // Do any lines intersect this?
          //--------------------------------------------------------------------

          if( intersects( bEdge0 )){
            return true;
          }
          if( intersects( bEdge1 )){
            return true;
          }
          if( intersects( bEdge2 )){
            return true;
          }
          if( intersects( bEdge3 )){
            return true;
          }
          if( intersects( bDiag0 )){
            return true;
          }
          if( intersects( bDiag1 )){
            return true;
          }
          return false;
        }
      }

    //}:                                          |
    //clipAgainst:{                               |

      namespace EON{
        template<> bool aabb2d::clipAgainst( const aabb2d& b ){
          if( b.intersects( *this )){
            if( min.x < b.min.x ){
              min.x = b.min.x;
            }
            if( min.y < b.min.y ){
              min.y = b.min.y;
            }
            if( max.x > b.max.x ){
              max.x = b.max.x;
            }
            if( max.y > b.max.y ){
              max.y = b.max.y;
            }
            return valid();
          }
          return false;
        }
      }

    //}:                                          |
    //circled:{                                    |

      namespace EON{
        template<> circled aabb2d::circle()const{
          const self x = (max.x - min.x) * .5;
          const self y = (max.y - min.y) * .5;
          return Circle( min.x + x, min.y + y, x.max( y ));
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb2d::clear(){
          max.x = kMin;
          max.y = kMin;
          min.x = kMax;
          min.y = kMax;
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//Color:{                                         |
  //Methods:{                                     |
    //multiply:{                                  |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Color
  #pragma mark -
#endif

      namespace EON{
        template<> f64 rgbad::multiply( const f64& base, const f64& blend ){
          return( base*blend );
        }
      }

    //}:                                          |
    //lighten:{                                   |

      namespace EON{
        template<> f64 rgbad::lighten( const f64& base, const f64& blend ){
          return max( base, blend );
        }
      }

    //}:                                          |
    //darken:{                                    |

      namespace EON{
        template<> f64 rgbad::darken( const f64& base, const f64& blend ){
          return min( base, blend );
        }
      }

    //}:                                          |
    //average:{                                   |

      namespace EON{
        template<> f64 rgbad::average( const f64& base, const f64& blend ){
          return( base+blend )*kHalf;
        }
      }

    //}:                                          |
    //linearDodge:{                               |

      namespace EON{
        template<> f64 rgbad::linearDodge( const f64& base, const f64& blend ){
          return self::saturate( base+blend );
        }
      }

    //}:                                          |
    //linearBurn:{                                |

      namespace EON{
        template<> f64 rgbad::linearBurn( const f64& base, const f64& blend ){
          return self::saturate( base-blend );
        }
      }

    //}:                                          |
    //difference:{                                |

      namespace EON{
        template<> f64 rgbad::difference( const f64& base, const f64& blend ){
          return abs( base-blend );
        }
      }

    //}:                                          |
    //negation:{                                  |

      namespace EON{
        template<> f64 rgbad::negation( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          return( k1 - abs( k1 - base - blend ));
        }
      }

    //}:                                          |
    //screen:{                                    |

      namespace EON{
        template<> f64 rgbad::screen( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          return 1.-(( 1. - base )*( 1. - blend ));
        }
      }

    //}:                                          |
    //exclusion:{                                 |

      namespace EON{
        template<> f64 rgbad::exclusion( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          return( base+blend-k2*base*blend );
        }
      }

    //}:                                          |
    //overlay:{                                   |

      namespace EON{
        template<> f64 rgbad::overlay( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          return( base <kHalf ) ?( 2. * base * blend ) :( 1. - 2.*( 1. - base )*( 1. - blend ));
        }
      }

    //}:                                          |
    //hardLight:{                                 |

      namespace EON{
        template<> f64 rgbad::hardLight( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          return( blend <kHalf ) ?( k2 * blend * base ) :( k1 - k2 *( k1 - blend ) *( k1 - base ));
        }
      }

    //}:                                          |
    //softLight:{                                 |

      namespace EON{
        template<> f64 rgbad::softLight( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          if( blend < kHalf ){
            return( k2 * base * blend + base*base *( k1 - k2 * blend ));
          }
          return( sqrt( base ) * ( k2 * blend - k1 ) + k2 * base * ( k1 - blend ));
        }
      }

    //}:                                          |
    //softDodge:{                                 |

      namespace EON{
        template<> f64 rgbad::softDodge( const f64& base, const f64& blend ){
          return(( base + blend ) < 1. ) ? (( base * .5 ) / ( 1. - blend )) : ( 1. - (( .5 *( 1. - blend )) / base ));
        }
      }

    //}:                                          |
    //colorDodge:{                                |

      namespace EON{
        template<> f64 rgbad::colorDodge( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //colorBurn:{                                 |

      namespace EON{
        template<> f64 rgbad::colorBurn( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          if( blend > k0 ){
            return max(( k1 -(( k1 - base ) / blend )), k0 );
          }
          return blend;
        }
      }

    //}:                                          |
    //linearLight:{                               |

      namespace EON{
        template<> f64 rgbad::linearLight( const f64& base, const f64& blend ){
          return( blend < kHalf ) ? linearBurn( base, k2*blend ) : linearDodge( base, k2*( blend-kHalf ));
        }
      }

    //}:                                          |
    //vividLight:{                                |

      namespace EON{
        template<> f64 rgbad::vividLight( const f64& base, const f64& blend ){
          return( blend < kHalf ) ? colorBurn( base,( k2 * blend )) : colorDodge( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //pinLight:{                                  |

      namespace EON{
        template<> f64 rgbad::pinLight( const f64& base, const f64& blend ){
          return( blend < kHalf ) ? darken( base,( k2 * blend )) : lighten( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //hardMix:{                                   |

      namespace EON{
        template<> f64 rgbad::hardMix( const f64& base, const f64& blend ){
          return( vividLight( base, blend ) < kHalf ) ? k0 : k1;
        }
      }

    //}:                                          |
    //reflection:{                                |

      namespace EON{
        template<> f64 rgbad::reflect( const f64& in_fBase, const f64& in_fBlend ){
          const f64& blend = self::saturate( in_fBlend );
          const f64& base  = self::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base*base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //glow:{                                      |

      namespace EON{
        template<> f64 rgbad::glow( const f64& base, const f64& blend ){
          if( base != k1 ){
            return min( blend*blend/( k1 - base ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //pheonix:{                                   |

      namespace EON{
        template<> f64 rgbad::phoenix( const f64& base, const f64& blend ){
          return min( base, blend ) - max( base, blend ) + k1;
        }
      }

    //}:                                          |
    //setHSV:{                                    |

      namespace EON{
        template<> void rgbad::setHSV( const f64& h, const f64& s, const f64& v ){
          if( s == 0. ){
            r = g = b = v;
          }else{
            f64 v1 = 0;
            f64 v2 = 0;
            if( v < .5 ){
              v2 = v * (1. + s);
            }else{
              v2 = (v + s) - (s * v);
            }
            v1 = 2. * v - v2;
            r = hue2RGB( v1, v2, h + .333333 );
            g = hue2RGB( v1, v2, h );
            b = hue2RGB( v1, v2, h - .333333 );
          }
        }
      }

    //}:                                          |
    //hsv:{                                       |

      namespace EON{
        //https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
        template<> hsvad rgbad::hsv()const{
          hsvad result;
          const f64 fCMax = max( max( r, g ), b );
          const f64 fCMin = min( min( r, g ), b );
          const f64 fDelta = fCMax - fCMin;
          if( fDelta > 0. ){
            if( fCMax == r ){
              result.h = 60. * f64(( g - b ) / fDelta ).mod( 6 );
            }else if( fCMax == g ){
              result.h = 60. * ((( b - r ) / fDelta ) + 2.);
            }else if(fCMax == b){
              result.h = 60. * ((( r - g ) / fDelta ) + 4.);
            }
            if( fCMax > 0. ){
              result.s = fDelta / fCMax;
            }else{
              result.s = 0.;
            }
            result.v = fCMax;
          }else{
            result.h = 0.;
            result.s = 0.;
            result.v = fCMax;
          }
          if( result.h < 0. ){
            result.h = 360. + result.h;
          }
          return result;
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    namespace EON{
      template<> void rgbad::set( const u32 inBGRA ){
        const bgra v( inBGRA );
        r = f64( v.r )/255.;
        g = f64( v.g )/255.;
        b = f64( v.b )/255.;
        a = f64( v.a )/255.;
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
