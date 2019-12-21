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

  //https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.types.__macro_half(v=vs.85).aspx
  //http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.ftml#algo3

#ifdef __APPLE__
  #pragma mark - Constants -
#endif

  /* Constants */

  namespace EON{
    template<> const f16 f16::k0       =  0.f;
    template<> const f16 f16::k1       =  1.f;
    template<> const f16 f16::k2       =  2.f;
    template<> const f16 f16::k3       =  3.f;
    template<> const f16 f16::k4       =  4.f;
    template<> const f16 f16::k5       =  5.f;
    template<> const f16 f16::k6       =  6.f;
    template<> const f16 f16::k7       =  7.f;
    template<> const f16 f16::k8       =  8.f;
    template<> const f16 f16::k9       =  9.f;
    template<> const f16 f16::k10      = 10.f;
    template<> const f16 f16::kPI      = 3.141592653589793f;
    template<> const f16 f16::k2PI     = kPI * k2;
    template<> const f16 f16::k4PI     = kPI * k4;
    template<> const f16 f16::cm       = 1.f;
    template<> const f16 f16::mm       = cm / 10.f;
    template<> const f16 f16::m        =     100.f * cm;
    template<> const f16 f16::dm       =      10.f * m;
    template<> const f16 f16::km       =    1000.f * m;
    template<> const f16 f16::Mm       =    1000.f * km;
    template<> const f16 f16::g        =       1.f;
    template<> const f16 f16::kg       =    1000.f * g;
    template<> const f16 f16::t        =    1000.f * kg;
    template<> const f16 f16::kt       =    1000.f * t;
    template<> const f16 f16::mt       =    1000.f * kt;
    template<> const f16 f16::kMin     =  -65504.f;
    template<> const f16 f16::kMax     =   65504.f;
    template<> const f16 f16::kEpsilon =    0.001f;
  }

  /* Vector constants */

  namespace EON{
    template<> const vec4h vec4h::kForward  = vec4h( 0.f, 1.f, 0.f );
    template<> const vec4h vec4h::kUp       = vec4h( 0.f, 0.f, 1.f );
    template<> const vec4h vec4h::kZero     = vec4h( 0.f, 0.f, 0.f );
    template<> const vec4h vec4h::kRight    = vec4h( 1.f, 0.f, 0.f );
    template<> const vec3h vec3h::kForward  = vec4h( 0.f, 1.f, 0.f );
    template<> const vec3h vec3h::kRight    = vec4h( 1.f, 0.f, 0.f );
    template<> const vec3h vec3h::kUp       = vec4h( 0.f, 0.f, 1.f );
    template<> const vec3h vec3h::kZero     = vec4h( 0.f, 0.f, 0.f );
    template<> const vec2h vec2h::kRight    = vec2h( 1.f, 0.f );
    template<> const vec2h vec2h::kUp       = vec2h( 0.f, 1.f );
    template<> const vec2h vec2h::kZero     = vec2h( 0.f, 0.f );
    template<> const pt3h  pt3h ::kZero     =  pt3h( 0.f, 0.f, 0.f );
    template<> const pt2h  pt2h ::kZero     =  pt2h( 0.f, 0.f);
    template<> const quath quath::kIdentity = quath( 0.f, 0.f, 0.f, 1.f );
  }

  /* Color constants */

  namespace EON{
    template<> const rgbah rgbah::kBlack  ( 0.f, 1.f );
    template<> const rgbah rgbah::kWhite  ( 1.f );
    template<> const rgbah rgbah::kGrey   ( .5f, 1.f );
    template<> const rgbah rgbah::kOrange ( 1.f, .5f, 0.f, 1.f );
    template<> const rgbah rgbah::kYellow ( 1.f, 1.f, 0.f, 1.f );
    template<> const rgbah rgbah::kLime   ( .5f, 1.f, 0.f, 1.f );
    template<> const rgbah rgbah::kGreen  ( 0.f, 1.f, 0.f, 1.f );
    template<> const rgbah rgbah::kCyan   ( 0.f, 1.f, 1.f, 1.f );
    template<> const rgbah rgbah::kBlue   ( 0.f, 0.f, 1.f, 1.f );
    template<> const rgbah rgbah::kMagenta( 1.f, 0.f, 1.f, 1.f );
    template<> const rgbah rgbah::kRed    ( 1.f, 0.f, 0.f, 1.f );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2h aabb2h::kFullUV( 0.f, 0.f, 1.f, 1.f );
    template<> const aabb2h aabb2h::kInvUV ( 0.f, 1.f, 1.f, 0.f );
    template<> const aabb2h aabb2h::kZero  ( 0.f );
    template<> const aabb3h aabb3h::kZero  ( 0.f );
  }

//}:                                              |
//Threads:{                                       |

  namespace{
    __thread std::mt19937* kGen16 = nullptr;
  }

//}:                                              |
//Private:{                                       |
  //line2line:{                                   |

    namespace{
      bool line2line( const pt2h& v0, const pt2h& v1, const pt2h& v2, const pt2h& v3 ){
        const f16 denom = ((v3.y-v2.y)*(v1.x-v0.x))-((v3.x-v2.x)*(v1.y-v0.y));
        const f16 numer = ((v3.x-v2.x)*(v0.y-v2.y))-((v3.y-v2.y)*(v0.x-v2.x));
        const f16 nume2 = ((v1.x-v0.x)*(v0.y-v2.y))-((v1.y-v0.y)*(v0.x-v2.x));
        if( denom == 0.f ){
          if(( numer == 0.f )&&( nume2 == 0.f )){
            return false;//coincident
          }
          return false;//parallel
        }
        const f16 ua = numer / denom;
        const f16 ub = nume2 / denom;
        return( ua >= 0.f && ua <= 1.f && ub >= 0.f && ub <= 1.f );
      }
    }

  //}:                                            |
  //line2rect:{                                   |

    namespace{
      bool line2rect( const pt2h v[ 2 ], const aabb2h& r ){
        const pt2h ll( r.min.x, r.max.y );
        const pt2h ur( r.max.x, r.min.y );
        const pt2h ul( r.min.x, r.min.y );
        const pt2h lr( r.max.x, r.max.y );
        for( u32 i=0; i<2; ++i ){
          if(( v[ i ].x > ll.x )&&( v[ i ].x < ur.x )&&( v[ i ].y < ll.y )&&( v[ i ].y > ur.y )){
            return true;
          }
        }
        if( line2line( v[ 0 ], v[ 1 ], ul, ll )){
          return true;
        }
        if( line2line( v[ 0 ], v[ 1 ], ll, lr )){
          return true;
        }
        if( line2line( v[ 0 ], v[ 1 ], ul, ur )){
          return true;
        }
        if( line2line( v[ 0 ], v[ 1 ], ur, lr )){
          return true;
        }
        return false;
      }
    }

  //}:                                            |
  //hue2RGB:{                                     |

    namespace{
      f16 hue2RGB( const f16& v1, const f16& v2, const f16& in_vH ){
        f16 vH = in_vH;
        if( vH < 0.f ){
          vH += f16::k1;
        }
        if( vH > 1.f ){
          vH -= f16::k1;
        }
        if( 6.f * vH < 1.f ){
          return v1+( v2 - v1 )*f16::k6*vH;
        }
        if( 2.f * vH < f16::k1 ){
          return v2;
        }
        if( 3.f * vH < f16::k2 ){
          return v1+( v2 - v1 )*( 0.666666f - vH ) * 6.f;
        }
        return v1;
      }
    }

  //}:                                            |
  //imin3:{                                       |

    #if 0
      namespace{
        s32 imin3( const s32 x, const s32 y, const s32 z ){
          const s32 T = (x < y ? x : y);
          return( T < z ? T : z );
        }
      }
    #endif

  //}:                                            |
  //imax3:{                                       |

    #if 0
      namespace{
        s32 imax3( const s32 x, const s32 y, const s32 z ){
          const s32 T = (x > y ? x : y);
          return( T > z ? T : z );
        }
      }
    #endif

  //}:                                            |
  //gen:{                                         |

    namespace{
      std::mt19937& gen16(){
        if( !kGen16 ){
          kGen16 = new std::mt19937( u32( e_seconds() ));
        }
        return *kGen16;
      }
    }

  //}:                                            |
//}:                                              |
//Basic:{                                         |
  //smoothstep:{                                  |

    template<> f16 f16::smoothstep( const f16& edge2, const f16& x ){
      const self& t = clamp(( x-value )/( edge2-value ), k0, k1 );
      return t*t*( k3-k2*t );
    }

  //}:                                            |
  //saturate[d]:{                                 |

    template<> f16 f16::saturated()const{
      return max( min( value, k1 ), k0 );
    }

    template<> void f16::saturate(){
      *this = max( min( value, k1 ), k0 );
    }

  //}:                                            |
  //iround:{                                      |

    namespace EON{
      template<> s32 f16::iround()const{
        return s32( value+.5f );
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
      template<> f16 f16::rsqrt()const{
        float v = value;
        float r;
        #ifdef __SSE3__
          _mm_store_ss( &r, _mm_rsqrt_ss( _mm_load_ss( &v )));
        #else // quake style for non-SSE platforms.
          float y;
          u32 i;
          y = v * 0.5f;
          i = *reinterpret_cast<const u32*>( &v );
          i = 0x5f3759df - ( i >> 1 );
          r = *reinterpret_cast<half*>( &i );
          r = r * ( 1.5f - r * r * y );
        #endif
        return r;
      }
    }

  //}:                                            |
  //sqrt:{                                        |

    namespace EON{
      template<> f16 f16::sqrt()const{
        e_assert( value >= k0 );
        float r;
        #ifdef __SSE3__
          float v = value;
          _mm_store_ss( &r, _mm_sqrt_ss( _mm_load_ss( &v )));
          return r;
        #else
          r=::sqrtf( value );
        #endif
        return r;
      }
    }

  //}:                                            |
  //floor:{                                       |

    namespace EON{
      template<> f16 f16::floor()const{
        return::floorf( value );
      }
    }

  //}:                                            |
  //ceil:{                                        |

    namespace EON{
      template<> f16 f16::ceil()const{
        return::ceilf( value );
      }
    }

  //}:                                            |
  //frac:{                                        |

    namespace EON{
      template<> f16 f16::frac( f16& i )const{
        i =f32::cast( s32( value ));
        return ( value - i );
      }
    }

  //}:                                            |
  //exp:{                                         |

    namespace EON{
      template<> f16 f16::exp()const{
        return::expf( value );
      }
    }

  //}:                                            |
  //pow:{                                         |

    namespace EON{
      template<> f16 f16::pow( const f16& y )const{
        return::powf( value, y.value );
      }
    }

  //}:                                            |
  //atan2:{                                       |

    namespace EON{
      template<> f16 f16::atan2( const f16& y )const{
        return::atan2f( value, y.value );
      }
    }

  //}:                                            |
  //atan:{                                        |

    namespace EON{
      template<> f16 f16::atan()const{
        return::atanf( value );
      }
    }

  //}:                                            |
  //tan:{                                         |

    namespace EON{
      template<> f16 f16::tan()const{
        return::tanf( value );
      }
    }

  //}:                                            |
  //asin:{                                        |

    namespace EON{
      template<> f16 f16::asin()const{
        return::asinf( value );
      }
    }

  //}:                                            |
  //sin:{                                         |

    namespace EON{
      template<> f16 f16::sin()const{
        return::sinf( value );
      }
    }

  //}:                                            |
  //acos:{                                        |

    namespace EON{
      template<> f16 f16::acos()const{
        return::acosf( value );
      }
    }

  //}:                                            |
  //cos:{                                         |

    namespace EON{
      template<> f16 f16::cos()const{
        return::cosf( value );
      }
    }

  //}:                                            |
  //mod:{                                         |

    namespace EON{
      template<> f16 f16::mod( const f16& y )const{
        return::fmodf( value, y.value );
      }
    }

  //}:                                            |
  //abs:{                                         |

    namespace EON{
      template<> f16 f16::abs()const{
        return::fabsf( value );
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
    template<> bool f16::valid( const f16& x ){
      return f32::valid( f32::cast( x ));
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
        template<> bool triangleh::intersects( const ray3h& R, pt3h& hit, vec3h& norm )const{
          const vec3h& diff   = R.p - A;
          const vec3h& edge1  = B - A;
          const vec3h& edge2  = C - A;
          const vec3h& normal = edge1.cross( edge2 );
          f16 DdN = R.d.dot( normal );
          f16 sign;
          if( DdN > kEpsilon ){
            sign = 1.f;
          }else if( DdN < -kEpsilon ){
            sign = -1.f;
            DdN  = -DdN;
          }else{
            return false;
          }
          const f16& DdQxE2 = sign*R.d.dot( diff.cross( edge2 ));
          if( DdQxE2 >= 0.f ){
            const f16& DdE1xQ = sign*R.d.dot( edge1.cross( diff ));
            if( DdE1xQ >= 0.f ){
              if( DdQxE2 + DdE1xQ <= DdN ){
                const f16& QdN = -sign*diff.dot( normal );
                if( QdN >= 0.f ){
                  const f16& inv = 1.f / DdN;
                  const f16& rayParameter = QdN*inv;
                  hit.x = R.p.x+R.d.x*rayParameter;
                  hit.y = R.p.y+R.d.y*rayParameter;
                  hit.z = R.p.z+R.d.z*rayParameter;
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
        template<> aabb2h triangleh::toBounds2D()const{
          aabb2h bounds;
          bounds += pt2h( A.x, A.y );
          bounds += pt2h( B.x, B.y );
          bounds += pt2h( C.x, C.y );
          return bounds;
        }
      }

    //}:                                          |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb3h triangleh::toBounds()const{
          aabb3h bounds;
          bounds += A;
          bounds += B;
          bounds += C;
          return bounds;
        }
      }

    //}:                                          |
    //draw:{                                      |

      namespace EON{
        template<> bool triangleh::draw( IRasterizer& rasterizer, const bool bTestPrimitive )const{
          #if /* DISABLES CODE */ (1)
            const u32 w = rasterizer.toW();
            const u32 h = rasterizer.toH();
            aabb2h b;
            b += pt2h( A.x, A.y );
            b += pt2h( B.x, B.y );
            b += pt2h( C.x, C.y );
            b.snap();
            const vec3h vs1( B.x-A.x, B.y-A.y, k0 );
            const vec3h vs2( C.x-A.x, C.y-A.y, k0 );

            //------------------------------------------------------------------
            // Test that triangleh doesn't overwrite pixels.
            //------------------------------------------------------------------

            if( bTestPrimitive ){
              for( s32 y=b.min.y.as<s32>(); y<=b.max.y.as<s32>(); y++ ){
                for( s32 x=b.min.x.as<s32>(); x<=b.max.x.as<s32>(); x++ ){
                  if(( x >= 0 )&&( x < s32( w ))&&( y >= 0 )&&( y < s32( h ))){
                    const vec3h q(( x-A.x.as<s32>() ),( y-A.y.as<s32>() ));
                    const f16 s = ( q.cross( vs2 ) / vs1.cross( vs2 )).length();
                    const f16 t = ( vs1.cross( q ) / vs1.cross( vs2 )).length();
                    if(( s >= 0.f ) && ( t >= 0.f )&&( s+t <= 1.f )){
                      if( rasterizer.onTest( x, y )){
                        return false;
                      }
                    }
                  }
                }
              }
            }

            //------------------------------------------------------------------
            // Render triangleh using rasterizer.
            //------------------------------------------------------------------

            for( s32 y=b.min.y.as<s32>(); y<=b.max.y.as<s32>(); y++ ){
              for( s32 x=b.min.x.as<s32>(); x<=b.max.x.as<s32>(); x++ ){
                if(( x >= 0 )&&( x < s32( w ))&&( y >= 0 )&&( y < s32( h ))){
                  const vec3h q(( x-A.x.as<s32>() ),( y-A.y.as<s32>() ));
                  const f16 s = ( q.cross( vs2 )/ vs1.cross( vs2 )).length();
                  const f16 t = ( vs1.cross( q )/ vs1.cross( vs2 )).length();
                  if(( s >= 0.f )&&( t >= 0.f ) && ( s+t <= 1.f )){
                    const f16 u = f32::cast( x )/f32::cast( w );
                    const f16 v = f32::cast( y )/f32::cast( h );
                    rasterizer.onDraw( x, y, vec2h( u, v ));
                  }
                }
              }
            }
          #else
            // 28.4 fixed-point coordinates
            const s32 Y1 = s32( 16.0f * A.y+.5h );
            const s32 Y2 = s32( 16.0f * B.y+.5h );
            const s32 Y3 = s32( 16.0f * C.y+.5h );

            const s32 X1 = s32( 16.0f * A.x+.5h );
            const s32 X2 = s32( 16.0f * B.x+.5h );
            const s32 X3 = s32( 16.0f * C.x+.5h );

            // Deltas
            const s32 DX12 = X1 - X2;
            const s32 DX23 = X2 - X3;
            const s32 DX31 = X3 - X1;

            const s32 DY12 = Y1 - Y2;
            const s32 DY23 = Y2 - Y3;
            const s32 DY31 = Y3 - Y1;

            // Fixed-point deltas
            const s32 FDX12 = DX12 << 4;
            const s32 FDX23 = DX23 << 4;
            const s32 FDX31 = DX31 << 4;

            const s32 FDY12 = DY12 << 4;
            const s32 FDY23 = DY23 << 4;
            const s32 FDY31 = DY31 << 4;

            // Bounding rectangle
            s32 minx = (imin3( X1, X2, X3 )+0xF) >> 4;
            s32 maxx = (imax3( X1, X2, X3 )+0xF) >> 4;
            s32 miny = (imin3( Y1, Y2, Y3 )+0xF) >> 4;
            s32 maxy = (imax3( Y1, Y2, Y3 )+0xF) >> 4;

            // Block size, standard 8x8 (must be power of two)
            const s32 q = 8;

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
            s32 C1 = DY12 * X1 - DX12 * Y1;
            s32 C2 = DY23 * X2 - DX23 * Y2;
            s32 C3 = DY31 * X3 - DX31 * Y3;

            // Correct for fill convention
            if( DY12 < 0 || (DY12 == 0 && DX12 > 0 )) C1++;
            if( DY23 < 0 || (DY23 == 0 && DX23 > 0 )) C2++;
            if( DY31 < 0 || (DY31 == 0 && DX31 > 0 )) C3++;

            //------------------------------------------------------------------
            // Test that triangleh doesn't overwrite pixels.
            //------------------------------------------------------------------

            if( bTestPrimitive ){
              for( s32 y = miny; y < maxy; y += q ){
                for( s32 x = minx; x < maxx; x += q ){
                  // Corners of block
                  const s32 x1 = ( x + q - 1 ) << 4;
                  const s32 y1 = ( y + q - 1 ) << 4;
                  const s32 x0 = ( x         ) << 4;
                  const s32 y0 = ( y         ) << 4;

                  // Evaluate half-space functions
                  const bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
                  const bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
                  const bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
                  const bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
                  const s32  a   = ( a00 << 0 )|( a10 << 1 )|( a01 << 2 )|( a11 << 3 );

                  const bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
                  const bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
                  const bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
                  const bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
                  const s32  b   = ( b00 << 0 )|( b10 << 1 )|( b01 << 2 )|( b11 << 3 );

                  const bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
                  const bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
                  const bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
                  const bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
                  const s32  c   = ( c00 << 0 )|( c10 << 1 )|( c01 << 2 )|( c11 << 3 );

                  // Skip block when outside an edge
                  if( !a || !b || !c ){
                    continue;
                  }

                  // Accept whole block when totally covered
                  if(( a == 0xF )&&( b == 0xF )&&( c == 0xF )){
                    for( s32 iy=0; iy<q; ++iy ){
                      const f16& h = rasterizer.toH();
                      if(( y + iy )>=h ){
                        break;
                      }
                      for( s32 ix=x; ix<( x + q ); ++ix ){
                        const f16& w = rasterizer.toW();
                        if( ix >= w ){
                          break;
                        }
                        if( ix < 0 ){
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
                  const s32 CY1 = C1 + DX12 * y0 - DY12 * x0;
                  const s32 CY2 = C2 + DX23 * y0 - DY23 * x0;
                  const s32 CY3 = C3 + DX31 * y0 - DY31 * x0;
                  for( s32 iy=y; iy<( y + q ); ++iy ){
                    const _f16& h = rasterizer.toH();
                    if( iy>=h ){
                      break;
                    }
                    s32 CX1 = CY1;
                    s32 CX2 = CY2;
                    s32 CX3 = CY3;
                    for( s32 ix=x; ix<x+q; ++ix ){
                      if( CX1 > 0 && CX2 > 0 && CX3 > 0 ){
                        const f16& w = rasterizer.toW();
                        if( ix >= w ){
                          break;
                        }
                        if( ix < 0 ){
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
            // Render triangleh using rasterizer.
            //------------------------------------------------------------------

            // Loop through blocks
            for( s32 y = miny; y < maxy; y += q ){
              for( s32 x = minx; x < maxx; x += q ){
                // Corners of block
                s32 x1 = ( x + q - 1 ) << 4;
                s32 y1 = ( y + q - 1 ) << 4;
                s32 x0 = ( x         ) << 4;
                s32 y0 = ( y         ) << 4;

                // Evaluate half-space functions
                const bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
                const bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
                const bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
                const bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
                const s32  a   = ( a00 << 0 )|( a10 << 1 )|( a01 << 2 )|( a11 << 3 );

                const bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
                const bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
                const bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
                const bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
                const s32  b   = ( b00 << 0 )|( b10 << 1 )|( b01 << 2 )|( b11 << 3 );

                const bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
                const bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
                const bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
                const bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
                const s32  c   = ( c00 << 0 )|( c10 << 1 )|( c01 << 2 )|( c11 << 3 );

                // Skip block when outside an edge
                if( !a || !b || !c ){
                  continue;
                }

                // Accept whole block when totally covered
                if(( a == 0xF )&&( b == 0xF )&&( c == 0xF )){
                  for( s32 iy=0; iy<q; ++iy ){
                    const f16& h = rasterizer.toH();
                    const f16& v = float( y+iy )/h;
                    if(( y + iy )>=h ){
                      break;
                    }
                    for( s32 ix=x; ix<( x + q ); ++ix ){
                      const f16& w = rasterizer.toW();
                      const f16& u = float( ix )/w;
                      if( ix>=w ){
                        break;
                      }
                      if( ix < 0 ){
                        continue;
                      }
                      rasterizer.onDraw( ix, y + iy, vec2h( u, v ));
                    }
                  }
                  continue;
                }

                // Partially covered block
                s32 CY1 = C1 + DX12 * y0 - DY12 * x0;
                s32 CY2 = C2 + DX23 * y0 - DY23 * x0;
                s32 CY3 = C3 + DX31 * y0 - DY31 * x0;
                for( s32 iy=y; iy<y+q; ++iy ){
                  const f16& h = rasterizer.toH();
                  const f16& v = float( iy )/h;
                  if( iy>=h ){
                    break;
                  }
                  s32 CX1 = CY1;
                  s32 CX2 = CY2;
                  s32 CX3 = CY3;
                  for( s32 ix=x; ix<( x + q ); ++ix ){
                    if(( CX1 > 0 )&&( CX2 > 0 )&&( CX3 > 0 )){
                      const f16& w = rasterizer.toW();
                      const f16& u = float( ix )/w;
                      if( ix>=w ){
                        break;
                      }
                      if( ix<0 ){
                        continue;
                      }
                      rasterizer.onDraw( ix, iy, vec2h( u, v ));
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
        template<> aabb2h circleh::toBounds()const{
          return aabb2h(
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
        template<> bool sphereh::intersects( const ray3h& in_ray, f16& out_distance )const{
          const vec3h q = v.xyz() - in_ray.p;
          const f16& c = q.length();
          const f16& v = q.dot( in_ray.d );
          const f16& d = toRadius()*toRadius()-( c*c - v*v );
          if( d < k0 ){
            out_distance = -1.f;
            return false;
          }
          out_distance=( v - sqrt( d ));
          return true;
        }
      }

    //}:                                          |
    //toBounds:{                                  |

      namespace EON{
        template<> aabb3h sphereh::toBounds()const{
          return aabb3h(
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
      template<> void planeh::set( const triangleh& in ){
        a() = in.A.y*(in.B.z-in.C.z) + in.B.y*(in.C.z-in.A.z) + in.C.y*(in.A.z-in.B.z);
        b() = in.A.z*(in.B.x-in.C.x) + in.B.z*(in.C.x-in.A.x) + in.C.z*(in.A.x-in.B.x);
        c() = in.A.x*(in.B.y-in.C.y) + in.B.x*(in.C.y-in.A.y) + in.C.x*(in.A.y-in.B.y);
        d() =-(in.A.x*(in.B.y*in.C.z-in.C.y*in.B.z)+in.B.x*(in.C.y*in.A.z-in.A.y*in.C.z)+in.C.x*(in.A.y*in.B.z-in.B.y*in.A.z));
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
        template<> f16 quath::lengthSquared()const{
          return
            a[0]*a[0] + a[1]*a[1] +
            a[2]*a[2] + a[3]*a[3]
          ;
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void quath::invert(){
          const f16 norm = lengthSquared();
          if( norm > 0.f ){
            const f16 invNorm = k1/norm;
            x =  x * invNorm;
            y = -y * invNorm;
            z = -z * invNorm;
            w = -w * invNorm;
          }else{
            x = y = z = w = 0.f;
          }
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //operator::==:{                            |

        namespace EON{
          template<> bool quath::operator==( const quath& q )const{
            return( 0 == memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::!=:{                            |

        namespace EON{
          template<> bool quath::operator!=( const quath& q )const{
            return( 0 != memcmp( a, q.a, sizeof( a )));
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quath::operator<( const quath& q )const{
            return( memcmp( a, q.a, sizeof( a )) < 0 );
          }
        }

      //}:                                        |
      //operator::<=:{                            |

        namespace EON{
          template<> bool quath::operator<=( const quath& q )const{
            return( memcmp( a, q.a, sizeof( a )) <= 0 );
          }
        }

      //}:                                        |
      //operator::<:{                             |

        namespace EON{
          template<> bool quath::operator>( const quath& q )const{
            return( memcmp( a, q.a, sizeof( a )) > 0 );
          }
        }

      //}:                                        |
      //operator::>=:{                            |

        namespace EON{
          template<> bool quath::operator>=( const quath& q )const{
            return( memcmp( a, q.a, 16 ) >= 0 );
          }
        }

      //}:                                        |
      //operator::+?:{                            |

        namespace EON{
          template<> quath& quath::operator+=( const quath& q ){
            x += q.x;
            y += q.y;
            z += q.z;
            w += q.w;
            return *this;
          }
        }

        namespace EON{
          template<> quath quath::operator+( const quath& q )const{
            return quath(
              x + q.x,
              y + q.y,
              z + q.z,
              w + q.w
            );
          }
        }

      //}:                                        |
      //operator::*?:{                            |

        namespace EON{
          template<> quath quath::operator*( const quath& q )const{
            return quath(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z
            );
          }
        }

        namespace EON{
          template<> quath quath::operator*( const f16& s )const{
            return quath(
              x * s,
              y * s,
              z * s,
              w * s
            );
          }
        }

        namespace EON{
          template<> quath& quath::operator*=( const quath& q ){
            const quath r(
              w*q.x + x*q.w + y*q.z - z*q.y,
              w*q.y + y*q.w + z*q.x - x*q.z,
              w*q.z + z*q.w + x*q.y - y*q.x,
              w*q.w - x*q.x - y*q.y - z*q.z );
            *this = r;
            return *this;
          }
        }

        namespace EON{
          template<> quath& quath::operator*=( const f16& s ){
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
          template<> quath& quath::operator/=( const quath& q ){
            quath p( q );
            p.invert();
            (*this)*p;
            return *this;
          }
        }

        namespace EON{
          template<> quath& quath::operator/=( const f16& s ){
            if( s != 0.f ){
              x/=s;
              y/=s;
              z/=s;
              w/=s;
            }else{
              x = 0.f;
              y = 0.f;
              z = 0.f;
              w = 0.f;
            }
            return *this;
          }
        }

        namespace EON{
          template<> quath quath::operator/( const quath& q )const{
            quath p( q );
            p.invert();
            return (*this)*p;
          }
        }

        namespace EON{
          template<> quath quath::operator/( const f16& s )const{
            quath q;
            if( s != 0.f ){
              q.x = x/s;
              q.y = y/s;
              q.z = z/s;
              q.w = w/s;
            }else{
              q.x = 0.f;
              q.y = 0.f;
              q.z = 0.f;
              q.w = 0.f;
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
        template<> void quath::setAxisAngle( const vec3h& axis, const f16& angleDeg ){
          const f16& a = rad( angleDeg );
          const vec3h v = axis*sin( a / k2 );
          x = v.x;
          y = v.y;
          z = v.z;
          w = cos( a/k2 );
        }
      }

    //}:                                          |
    //asQuaternion:{                              |

      namespace EON{
        template<> quath vec3x3h::asQuaternion()const{
          // Use the Graphics Gems code, from
          // ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z
          // *NOT* the "Matrix and Quaternions FAQ", which has errors!
          // The trace is the sum of the diagonal elements; see
          // http://mathworld.wolfram.com/MatrixTrace.ftml
          f16 t = m[0][0] + m[1][1] + m[2][2];
          f16 x, y, z, w;
          // we protect the division by s by ensuring that s>=1
          if( t >= 0.f ){ // |w| >= .5
            f16 s = sqrt( t + k1 ); // |s|>=1 ...
            w = kHalf * s;
            s = kHalf / s; // so this division isn't bad
            x = (m[2][1] - m[1][2]) * s;
            y = (m[0][2] - m[2][0]) * s;
            z = (m[1][0] - m[0][1]) * s;
          }else if(( m[0][0] > m[1][1] )&&( m[0][0] > m[2][2] )){
            f16 s = sqrt( k1 + m[0][0] - m[1][1] - m[2][2] ); // |s|>=1
            x = s * kHalf; // |x| >= .5
            s = kHalf / s;
            y = (m[1][0] + m[0][1]) * s;
            z = (m[0][2] + m[2][0]) * s;
            w = (m[2][1] - m[1][2]) * s;
          }else if( m[1][1] > m[2][2] ){
            f16 s = sqrt( k1 + m[1][1] - m[0][0] - m[2][2] ); // |s|>=1
            y = s * kHalf; // |y| >= .5
            s = kHalf / s;
            x = (m[1][0] + m[0][1]) * s;
            z = (m[2][1] + m[1][2]) * s;
            w = (m[0][2] - m[2][0]) * s;
          }else{
            f16 s = sqrt( k1 + m[2][2] - m[0][0] - m[1][1] ); // |s|>=1
            z = s * kHalf; // |z| >= .5
            s = kHalf / s;
            x = (m[0][2] + m[2][0]) * s;
            y = (m[2][1] + m[1][2]) * s;
            w = (m[1][0] - m[0][1]) * s;
          }
          quath q;
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
        template<> bool quath::compare( const quath& q, const f16& tolerance )const{
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
        template<> void quath::rotate( const vec3h& axis, const f16& angleDeg ){
          *this = quath( axis, angleDeg ) * *this;
        }
      }

      namespace EON{
        template<> void quath::rotate( const vec2h& xyMouse ){
          quath q1 = *this, q2, q3;
          q2.setAxisAngle( vec3h( k0, k0,-k1 ), xyMouse.x );
          q3.setAxisAngle( vec3h(-k1, k0, k0 ), xyMouse.y );
          q1 = q2 * q1;
          q1 = q1 * q3;
          *this = q1;
        }
      }

    //}:                                          |
    //setEuler:{                                  |

      namespace EON{
        template<> void quath::setEuler( const vec3h& e ){
          const f16& cos_z_2 = cos( kHalf*rad( e.z ));
          const f16& cos_y_2 = cos( kHalf*rad( e.y ));
          const f16& cos_x_2 = cos( kHalf*rad( e.x ));
          const f16& sin_z_2 = sin( kHalf*rad( e.z ));
          const f16& sin_y_2 = sin( kHalf*rad( e.y ));
          const f16& sin_x_2 = sin( kHalf*rad( e.x ));
          x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
          y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
          z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
          w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
        }
      }

    //}:                                          |
    //getEuler:{                                  |

      namespace EON{
        template<> void quath::getEuler( f16& out_x, f16& out_y, f16& out_z, const bool bHomogeneous )const{
          const f16& sqw = w*w;
          const f16& sqx = x*x;
          const f16& sqy = y*y;
          const f16& sqz = z*z;
          if( bHomogeneous ){
            out_z = atan2( k2 * (x*y + z*w), sqx - sqy - sqz + sqw );
            out_y = asin( -k2 * (x*z - y*w) );
            out_x = atan2( k2 * (y*z + x*w),-sqx - sqy + sqz + sqw );
          }else{
            out_z = atan2( k2 * (z*y + x*w), k1 - k2*( sqx + sqy ));
            out_y = asin( -k2 * (x*z - y*w));
            out_x = atan2( k2 * (x*y + z*w), k1 - k2*( sqy + sqz ));
          }
          out_x = out_x.deg();
          out_y = out_y.deg();
          out_z = out_z.deg();
        }
      }

    //}:                                          |
    //randomize:{                                 |

      namespace EON{
        template<> void quath::randomize( const f16& x1, const f16& x2, const f16& x3 ){
          const f16& Z = x1;
          const f16& o = k2PI * x2;
          const f16& r = sqrt( k1 - z * z );
          const f16& W = kPI * x3;
          const f16& sw = sin( w );
          x = sw * cos( o ) * r;
          y = sw * sin( o ) * r;
          z = sw * Z;
          w = cos( W );
        }
      }

    //}:                                          |
    //get?x?:{                                    |

      namespace EON{
        template<> vec4x4h quath::get4x4()const{
          vec4x4h R( k0 );
          const f16& xx = x * x;
          const f16& xy = x * y;
          const f16& xz = x * z;
          const f16& xw = x * w;
          const f16& yy = y * y;
          const f16& yz = y * z;
          const f16& yw = y * w;
          const f16& zz = z * z;
          const f16& zw = z * w;
          R( 0, 0 ) = k1 - k2 * (yy + zz);
          R( 1, 0 ) =      k2 * (xy - zw);
          R( 2, 0 ) =      k2 * (xz + yw);
          R( 0, 1 ) =      k2 * (xy + zw);
          R( 1, 1 ) = k1 - k2 * (xx + zz);
          R( 2, 1 ) =      k2 * (yz - xw);
          R( 0, 2 ) =      k2 * (xz - yw);
          R( 1, 2 ) =      k2 * (yz + xw);
          R( 2, 2 ) = k1 - k2 * (xx + yy);
          R( 3, 3 ) = k1;
          return R;
        }
      }

      namespace EON{
        template<> vec3x3h quath::get3x3()const{
          vec3x3h R( k0 );
          const f16& xx = x * x;
          const f16& xy = x * y;
          const f16& xz = x * z;
          const f16& xw = x * w;
          const f16& yy = y * y;
          const f16& yz = y * z;
          const f16& yw = y * w;
          const f16& zz = z * z;
          const f16& zw = z * w;
          R( 0, 0 ) = k1 - k2 * (yy + zz);
          R( 1, 0 ) =      k2 * (xy - zw);
          R( 2, 0 ) =      k2 * (xz + yw);
          R( 0, 1 ) =      k2 * (xy + zw);
          R( 1, 1 ) = k1 - k2 * (xx + zz);
          R( 2, 1 ) =      k2 * (yz - xw);
          R( 0, 2 ) =      k2 * (xz - yw);
          R( 1, 2 ) =      k2 * (yz + xw);
          R( 2, 2 ) = k1 - k2 * (xx + yy);
          return R;
        }         
      }

    //}:                                          |
    //length:{                                    |

      namespace EON{
        template<> f16 quath::length()const{
          return sqrt(
            a[0]*a[0] + a[1]*a[1] +
            a[2]*a[2] + a[3]*a[3]
          );
        }
      }

    //}:                                          |
    //dot:{                                       |

      namespace EON{
        template<> f16 quath::dot( const quath& q )const{
          return
            a[0]*q.a[0] + a[1]*q.a[1] +
            a[2]*q.a[2] + a[3]*q.a[3]
          ;
        }
      }

    //}:                                          |
    //normalize:{                                 |

      namespace EON{
        template<> quath& quath::normalize(){
          const f16& l = length();
          if( l > 0.00001f ){
            const f16& rcpLength=k1/l;
            x *= rcpLength;
            y *= rcpLength;
            z *= rcpLength;
            w *= rcpLength;
          }else{
            x = y = z = w = k0;
          }
          return *this;
        }
      }

    //}:                                          |
    //lookat:{                                    |

      namespace EON{
        template<> void quath::lookat( const pt3h& srcPoint, const pt3h& dstPoint ){
          vec3h direction = dstPoint - srcPoint;
          direction.normalize();
          const f16& dot = vec3h::kForward.dot( direction );
          if(( dot+k1 ).abs() < 0.000001f ){
            *this = quath( vec3h::kForward, deg( kPI ));
          }else if(( dot-k1 ).abs() < 0.000001f ){
            *this = quath();
          }else{
            const f16& angle = deg( dot.acos() );
            vec3h axis = vec3h::kForward.cross( direction );
            axis.normalize();
            *this = quath( axis, angle );
          }
        }
      }

    //}:                                          |
    //inverse:{                                   |

      namespace EON{
        template<> quath quath::inverse()const{
          quath inv=*this;
          inv.invert();
          return inv;
        }
      }

    //}:                                          |
    //conjugate:{                                 |

      namespace EON{
        template<> quath quath::conjugate()const{
          return quath( a[0],-a[1],-a[2],-a[3] );
        }
      }

    //}:                                          |
    //exp:{                                       |

      namespace EON{
        template<> quath quath::exp()const{
          const f16& a = ((vec3h*)this)->length();
          const f16& sina = sin( a );
          const f16& cosa = cos( a );
          quath ret;
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
        template<> quath quath::log()const{
          const f16& a = acos( w );
          const f16& sina = sin( a );
          quath ret;
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
        template<> quath quath::lerped( const quath& p, const quath& q, const f16& t ){
          quath result = p*( k1-t )+q*t;
          result.normalize();
          return result;
        }
      }

    //}:                                          |
    //lerp:{                                      |

      namespace EON{
        template<> quath& quath::lerp( const quath& p, const quath& q, const f16& t ){
          *this = lerped( p, q, t );
          return *this;
        }
      }

    //}:                                          |
    //slerpedNoInvert:{                           |

      namespace EON{
        template<> quath quath::slerpedNoInvert( const quath& p, const quath& q, const f16& fTime ){
          const f16& d = p.dot( q );
          if(( d > -0.95f )&&( d < 0.95f )){
            const f16& angle = acos( d );
            return( p*sin( angle*( k1-fTime ))+q*sin( angle*fTime ))/sin( angle );
          }
          // if the angle is small, use linear interpolation
          return lerped( p, q, fTime );
        }
      }

    //}:                                          |
    //slerp:{                                     |

      namespace EON{
        template<> quath& quath::slerp( const quath& p, const quath& q, const f16& fTime ){
          quath q3;
          f16 d = p.dot( q );
          if( d < 0.f ){
            d = -d;
            q3 = q;
            q3.negate();
          }else{
            q3 = q;
          }
          if( d < .95f ){
            const f16& angle = acos( d );
            *this=( p*sin( angle*( k1-fTime )) + q3*sin( angle*fTime ))/sin( angle );
          }else{// if the angle is small, use linear interpolation
            *this=lerped( p, q3, fTime );
          }
          return *this;
        }
      }

    //}:                                          |
    //squad:{                                     |

      namespace EON{
        template<> quath& quath::squad( const quath& q1, const quath& q2, const quath& a, const quath& b, const f16& t ){
          const quath& c = slerpedNoInvert( q1, q2, t );
          const quath& d = slerpedNoInvert( a,  b,  t );
          *this = slerpedNoInvert( c, d, k2*t*( k1-t ));
          return *this;
        }
      }

    //}:                                          |
    //align:{                                     |

      namespace EON{
        template<> quath& quath::align( const vec3h& up, const vec3h& norm ){
          const vec3h& r = up.cross( norm );
          *this = quath( r.x, r.y, r.z, k1 + up.dot( norm ));
          return *this;
        }
      }

    //}:                                          |
  //}:                                            |
  //Vectors:{                                     |
    //vec3h:{                                     |
      //[operators]:{                             |
        //operator=:{                             |

          namespace EON{
            template<> vec3h& vec3h::operator=( const vec4h& V ){
              x = V.x;
              y = V.y;
              z = V.z;
              return *this;
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Matrices:{                                    |
    //vec4x4h:{                                   |
      //[operators]:{                             |
        //operator*=:{                            |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Matrices
  #pragma mark -
#endif

        namespace EON{
          template<> vec4x4h& vec4x4h::operator*=( const vec4x4h& B ){
            vec4x4h A = *this;
            for( u32 i=0; i<4; ++i ){
              for( u32 j=0; j<4; ++j ){
                f16 sum( k0 );
                for( u32 e=0; e<4; ++e ){
                  sum += A.m[i][e]*B.m[e][j];
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
          template<> vec4x4h& vec4x4h::mulTranspose( const vec4x4h& b ){
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
          template<> void vec4x4h::setPerspectiveV2C( const f16& degFOV, const f16& aspect, const f16& zn, const f16& zf ){
            // f = 1.f / tanf( rad( degFOV )*.5h )
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
            // | 0    0  -(zf+zn)/(zn-zf)  (-2.f*zf*zn)/(zn-zf) |
            // | 0    0  -1                0                    |
            // +--                                            --+
            const f16& f = k1 / tan( rad( degFOV )*kHalf );
            memset( this, 0, sizeof( vec4x4h ));
            XX = f / aspect;
            YY = f;
            ZZ = -( zf+zn )/( zn-zf );
            ZW =  (-k2 * zf * zn )/( zn-zf );
            WZ = -k1;
          }
        }

      //}:                                        |
      //setTranslate:{                            |

        namespace EON{
          template<> void vec4x4h::setTranslation( const f16& x, const f16& y, const f16& z ){
            XX = 1; XY = 0; XZ = 0; XW = 0;
            YX = 0; YY = 1; YZ = 0; YW = 0;
            ZX = 0; ZY = 0; ZZ = 1; ZW = 0;
            WX = x; WY = y; WZ = z; WW = 1;
          }
        }

      //}:                                        |
      //setScale:{                                |

        namespace EON{
          template<> void vec4x4h::setScale( const f16& x, const f16& y, const f16& z ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = y; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = z; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

        namespace EON{
          template<> void vec4x4h::setScale( const f16& x ){
            XX = x; YX = 0; ZX = 0; WX = 0;
            XY = 0; YY = x; ZY = 0; WY = 0;
            XZ = 0; YZ = 0; ZZ = x; WZ = 0;
            XW = 0; YW = 0; ZW = 0; WW = 1;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec4x4h::setRotX( const f16& angleInDeg ){
            const f16& f = rad( angleInDeg );
            // [ 0 4      8 12 ]
            // [ 1 cos -sin 13 ]
            // [ 2 sin cos  14 ]
            // [ 3 7     11 15 ]
            t[10] = t[ 5] = cos( f );
            t[ 6] = sin( f );
            t[ 9] =-t[ 6];
            t[ 0] = t[15] = 1.f;
            t[ 1] = t[ 2] = t[ 3] = t[ 4] =
            t[ 7] = t[ 8] = t[11] = t[12] =
            t[13] = t[14] = 0.f;
          }
        }

      //}:                                        |
      //setRotY:{                                 |

        namespace EON{
          template<> void vec4x4h::setRotY( const f16& angleInDeg ){
            const f16& f = rad( angleInDeg );
            // [ cos 4  -sin 12 ]
            // [ 1   5   9   13 ]
            // [ sin 6  cos  14 ]
            // [ 3   7  11   15 ]
            t[ 0] = t[10] = cos( f );
            t[ 2] = sin( f );
            t[ 8] =-t[ 2];
            t[ 5] = t[15] = 1.f;
            t[ 1] = t[ 3] = t[ 4] = t[ 6] =
            t[ 7] = t[ 9] = t[11] = t[12] =
            t[13] = t[14] = 0.f;
          }
        }

      //}:                                        |
      //setRotZ:{                                 |

        namespace EON{
          template<> void vec4x4h::setRotZ( const f16& angleInDeg ){
            const f16& f = rad( angleInDeg );
            // [ cos -sin 8 12 ]
            // [ sin cos  9 13 ]
            // [ 2   6   10 14 ]
            // [ 3   7   11 15 ]
            t[ 0] = t[ 5] = cos( f );
            t[ 1] = sin( f );
            t[ 4] =-t[ 1];
            t[10] = t[15] = 1.f;
            t[ 2] = t[ 3] = t[ 6] = t[ 7] =
            t[ 8] = t[ 9] = t[11] = t[12] =
            t[13] = t[14] = 0.f;
          }
        }

      //}:                                        |
      //setAxisIdentity:{                         |

        namespace EON{
          template<> void vec4x4h::setAxisIdentity(){
            YX = ZX = WX = XY = WY = YY = 0.f;
            XZ = WZ = XW = YW = ZW = ZZ = 0.f;
            XX = .01f;
            YZ = .01f;
            ZY = .01f;
            WW = k1;
          }
        }

      //}:                                        |
      //transpose:{                               |

        namespace EON{
          template<> void vec4x4h::transpose(){
            vec4x4h M=*this;
            XY=M.YX; XZ=M.ZX; XW=M.WX;
            YX=M.XY; YZ=M.ZY; YW=M.WY;
            ZX=M.XZ; ZY=M.YZ; ZW=M.WZ;
            WX=M.XW; WY=M.YW; WZ=M.ZW;
          }
        }

      //}:                                        |
      //invert:{                                  |

        namespace{
          bool invertMatrix( const f16 m[16], f16 inv[16] ){
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
            f16 det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
            if( !det.as<bool>() ){
              return false;
            }
            det = 1.f / det;
            for( u32 i=0; i<16; ++i ){
              inv[i] *= det;
            }
            return true;
          }
        }

        namespace EON{
          template<> void vec4x4h::invert(){
            const vec4x4h M = *this;
            if( !invertMatrix( M.t, t )){
              setIdentity();
            }
          }
          template<> vec4x4h vec4x4h::inverse()const{
            vec4x4h M;
            if( !invertMatrix( t, M.t )){
              M.setIdentity();
            }
            return M;
          }
        }

        namespace EON{
          template<> void vec4x4h::removeTranslation(){
            WX = WY = WZ = 0.f;
          }
        }

        namespace EON{
          template<> void vec4x4h::removeScale(){
            vec3h vx = vec3h( XX, YX, ZX );
            vec3h vy = vec3h( XY, YY, ZY );
            vec3h vz = vec3h( XZ, YZ, ZZ );
            vec3h v;
            v.x = 1.f/vx.length();
            v.y = 1.f/vy.length();
            v.z = 1.f/vz.length();
            XX *= v.x; YX *= v.x; ZX *= v.x;
            XY *= v.y; YY *= v.y; ZY *= v.y;
            XZ *= v.z; YZ *= v.z; ZZ *= v.z;
          }
        }

      //}:                                        |
    //}:                                          |
    //vec3x3h:{                                   |
      //setAxisAngle:{                            |

        namespace EON{
          template<> void vec3x3h::setAxisAngle( const vec3h& axis, const f16& angle ){
            const f16& cs          = cos( angle );
            const f16& sn          = sin( angle );
            const f16& oneMinusCos = k1-cs;
            const f16& x2          = axis.x*axis.x;
            const f16& y2          = axis.y*axis.y;
            const f16& z2          = axis.z*axis.z;
            const f16& xym         = axis.x*axis.y*oneMinusCos;
            const f16& xzm         = axis.x*axis.z*oneMinusCos;
            const f16& yzm         = axis.y*axis.z*oneMinusCos;
            const f16& xSin        = axis.x*sn;
            const f16& ySin        = axis.y*sn;
            const f16& zSin        = axis.z*sn;
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
          template<> void vec3x3h::setScale( const f16& x, const f16& y, const f16& z ){
            XX = x; YX = 0; ZX = 0;
            XY = 0; YY = y; ZY = 0;
            XZ = 0; YZ = 0; ZZ = z;
          }
          template<> void vec3x3h::setScale( const f16& s ){
            XX = s; YX = 0; ZX = 0;
            XY = 0; YY = s; ZY = 0;
            XZ = 0; YZ = 0; ZZ = s;
          }
        }

      //}:                                        |
      //setRotX:{                                 |

        namespace EON{
          template<> void vec3x3h::setRotX( const f16& angleInDeg ){
            // [ 0   3    6 ]
            // [ 1 cos -sin ]
            // [ 2 sin  cos ]
            const f16& a = rad( angleInDeg );
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
          template<> void vec3x3h::setRotY( const f16& angleInDeg ){
            // [ cos 3 -sin ]
            // [ 1   4    7 ]
            // [ sin 5  cos ]
            const f16& a = rad( angleInDeg );
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
          template<> void vec3x3h::setRotZ( const f16& angleInDeg ){
            // [ cos -sin 6 ]
            // [ sin  cos 7 ]
            // [ 2    5   8 ]
            const f16& a = rad( angleInDeg );
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
        template<> bool frustumh::intersects( const pt3h& P )const{
          const Plane* next = m_aPlanes;
          const Plane* end = next + 6;
          while( next < end ){
            const f16& t = next->distance( P );
            if( t < kEpsilon ){
              return false;
            }
            ++next;
          }
          return true;
        }
      }

      namespace EON{
        template<> bool frustumh::intersects( const sphereh& S )const{
          const Plane* next = m_aPlanes;
          const Plane* end = next+6;
          while( next < end ){
            const f16& d = next->distance( S.toCenter() );
            if( d >= 2.f * S.toRadius() ){
              return false;
            }
            ++next;
          }
          //fully inside.
          return true;
        }
      }

      namespace EON{
        template<> bool frustumh::intersects( const aabb3h& B )const{
          const Plane* next = m_aPlanes;
          const Plane* end = next + 6;
          while( next < end ){
            pt3h a, b;
            if( next->Vector4::x >= 0.f ){
              a.x=B.min.x;
              b.x=B.max.x;
            }else{
              b.x=B.min.x;
              a.x=B.max.x;
            }
            if( next->Vector4::y >= 0.f ){
              a.y=B.min.y;
              b.y=B.max.y;
            }else{
              b.y=B.min.y;
              a.y=B.max.y;
            }
            if( next->Vector4::z >=0.f ){
              a.z=B.min.z;
              b.z=B.max.z;
            }else{
              b.z=B.min.z;
              a.z=B.max.z;
            }
            s32 l1;
            { const f16& d = next->distance( a );
              if( d > kEpsilon ){
                l1 = 1;
              }else if( d < -kEpsilon ){
                l1 = -1;
              }else{
                l1 = 0;
              }
            }
            s32 l2;
            { const f16& d = next->distance( b );
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
        template<> void frustumh::set( const vec4x4h& in ){
          vec4x4h W2C = in;
          const f16& xw = W2C[ 3];
          const f16& yw = W2C[ 7];
          const f16& zw = W2C[11];
          const f16& ww = W2C[15];

          const f16& xz = W2C[ 2];
          const f16& yz = W2C[ 6];
          const f16& zz = W2C[10];
          const f16& wz = W2C[14];

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

          const f16& xx = W2C[ 0];
          const f16& yx = W2C[ 4];
          const f16& zx = W2C[ 8];
          const f16& wx = W2C[12];

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

          const f16& xy = W2C[ 1];
          const f16& yy = W2C[ 5];
          const f16& zy = W2C[ 9];
          const f16& wy = W2C[13];

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
          template<> vec4h vec4h::operator*( const vec4x4h& M )const{
            return vec4h(
              M.XX * x + M.YX * y + M.ZX * z + M.WX * w,
              M.XY * x + M.YY * y + M.ZY * z + M.WY * w,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ * w,
              M.XW * x + M.YW * y + M.ZW * z + M.WW * w
            );
          }
          template<> vec4h vec4h::operator*( const vec3x3h& M )const{
            return vec4h(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z,
              k1
            );
          }
        }

      //}:                                        |
    //}:                                          |
    //Vector3:{                                   |
      //[operators]:{                             |

        namespace EON{
          template<> vec3h vec3h::operator*( const vec4x4h& M )const{
            return vec3h(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> vec3h vec3h::operator*( const vec3x3h& M )const{
            return vec3h(
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
          template<> pt3h pt3h::operator-( const vec3h& v )const{
            pt3h r;
            r.x = x-v.x;
            r.y = y-v.y;
            r.z = z-v.z;
            return r;
          }
        }

      //}:                                        |
      //operator+:{                               |

        namespace EON{
          template<> pt3h pt3h::operator+( const vec3h& v )const{
            pt3h r;
            r.x = x+v.x;
            r.y = y+v.y;
            r.z = z+v.z;
            return r;
          }
        }

      //}:                                        |
      //operator/:{                               |

        namespace EON{
          template<> pt3h pt3h::operator/( const vec3h& v )const{
            pt3h r;
            r.x = x/v.x;
            r.y = y/v.y;
            r.z = z/v.z;
            return r;
          }
        }

      //}:                                        |
      //operator*:{                               |

        namespace EON{
          template<> pt3h pt3h::operator*( const vec4x4h& M )const{
            return pt3h(
              M.XX * x + M.YX * y + M.ZX * z + M.WX,
              M.XY * x + M.YY * y + M.ZY * z + M.WY,
              M.XZ * x + M.YZ * y + M.ZZ * z + M.WZ
            );
          }
          template<> pt3h pt3h::operator*( const vec3x3h& M )const{
            return pt3h(
              M.XX * x + M.YX * y + M.ZX * z,
              M.XY * x + M.YY * y + M.ZY * z,
              M.XZ * x + M.YZ * y + M.ZZ * z
            );
          }
          template<> pt3h pt3h::operator*( const vec3h& v )const{
            pt3h r;
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
        template<> pt2h pt2h::scaledToClip( const f16& scale )const{
          pt2h p;
          p.x =( x/1024*k2-k1 )*scale;
          p.y =( y/768*k2-k1 )*scale;
          return p;
        }
      }

      namespace EON{
        template<> void pt2h::scaleToClip( const f16& scale ){
          x =( x/1024*k2-k1 )*scale;
          y =( y/768*k2-k1 )*scale;
        }
      }

    //}:                                          |
    //[ctors]:{                                   |

      namespace EON{
        template<> pt3h::Point3( const vec4h& v ){
          x = v.x;
          y = v.y;
          z = v.z;
        }
      }

      namespace EON{
        template<> pt3h::Point3( const vec3h& v ){
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
        template<> vec4x4h qst3h::get4x4()const{
          const vec4x4h& T = vec4x4h::getT( m_tPosition );
          const vec4x4h& R = toRotation().get4x4();
          vec4x4h out( m_fScale );
          out *= R;
          out *= T;
          return out;
        }
      }

      namespace EON{
        template<> vec3x3h qst3h::get3x3()const{
          vec3x3h out( m_fScale );
          return out * toRotation().get3x3();
        }
      }

      namespace EON{
        template<> vec4x4h qst3h::get4x4RT()const{
          const vec4x4h& T = vec4x4h::getT( m_tPosition );
          const vec4x4h& R = toRotation().get4x4();
          return vec4x4h( k1 ) * R * T;
        }
      }

      namespace EON{
        template<> vec4x4h qst3h::get4x4ST()const{
          const vec4x4h out( m_fScale );
          return out * vec4x4h::getT( m_tPosition );
        }
      }

      namespace EON{
        template<> vec4x4h qst3h::get4x4T()const{
          return vec4x4h::getT( m_tPosition );
        }
      }

      namespace EON{
        template<> vec4x4h qst3h::get4x4R()const{
          return toRotation().get4x4();
        }
      }

      namespace EON{
        template<> vec3x3h qst3h::get3x3R()const{
          return toRotation().get3x3();
        }
      }

      namespace EON{
        template<> vec4x4h qst3h::get4x4S()const{
          return vec4x4h( m_fScale );
        }
      }

      namespace EON{
        template<> vec3x3h qst3h::get3x3S()const{
          return vec3x3h( m_fScale );
        }
      }

    //}:                                          |
    //invert:{                                    |

      namespace EON{
        template<> void qst3h::invert(){
          toPosition().negate();
          toRotation().invert();
        }
      }

    //}:                                          |
    //[operators]:{                               |
      //Multiply:{                                |

        namespace EON{
          template<> pt3h qst3h::operator*( const pt3h& P )const{
            return P * get4x4();
          }
        }

        namespace EON{
          template<> void qst3h::operator*=( const qst3h& L2W ){
            x() += L2W.m_tPosition.x;
            y() += L2W.m_tPosition.y;
            z() += L2W.m_tPosition.z;
            quath q = L2W.m_qRotation;
            q.normalize();
            m_qRotation *= q;
            m_qRotation.normalize();
            m_fScale *= L2W.m_fScale;
          }
        }

      //}:                                        |
      //Array:{                                   |

        namespace EON{
          template<> f16 qst3h::operator[]( const u32 i )const{
            e_assert( i < 7 );
            f16 euler[3];
            switch( i ){
              case 0:
              case 1:
              case 2:
                return m_tPosition[ i ];
              case 3:
              case 4:
              case 5:
                m_qRotation.getEuler( euler[0], euler[1], euler[2] );
                return euler[ i-3 ];
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

    template<> f16 e_rand<f16>( const f16 start, const f16 end ){
      std::uniform_real_distribution<float> dis((half( start )),half( end ));
      return dis( gen16() );
    }

    template<> f16 e_rand<f16>(){
      std::uniform_real_distribution<float> dis;
      return dis( gen16() );
    }

    template<> half e_rand<half>(half start, half end ){
      std::uniform_real_distribution<float> dis( start, end );
      return dis( gen16() );
    }

    template<> half e_rand<half>(){
      std::uniform_real_distribution<float> dis;
      return dis( gen16() );
    }

    template<> u16 e_rand<u16>( const u16 start, const u16 end ){
      std::uniform_int_distribution<u16> dis( start, end );
      return dis( gen16() );
    }

    template<> u16 e_rand<u16>(){
      std::uniform_int_distribution<s16> dis;
      return dis( gen16() );
    }

    template<> s16 e_rand<s16>( const s16 start, const s16 end ){
      std::uniform_int_distribution<s16> dis( start, end );
      return dis( gen16() );
    }

    template<> s16 e_rand<s16>(){
      std::uniform_int_distribution<s16> dis;
      return dis( gen16() );
    }

  //}:                                            |
  //e_randunitvec*:{                              |

    vec2h e_randunitvec2h(){
      const f16& t = e_rand2pih();
      const f16& s = t.sin();
      const f16& c = t.cos();
      return vec2h( c, s );
    }

    vec3h e_randunitvec3h(){
      const f16& theta = e_rand2pi();
      const f16& r = e_randunith().sqrt();
      const f16& z = f16::sqrt( 1.f -r*r )*( e_rand<u32>( 0, 1 ) ? - 1.f : 1.f );
      return vec3h( r * theta.cos(), r * theta.sin(), z );
    }

  //}:                                            |
  //e_randunit:{                                  |

    f16 e_randunith(){
      return e_rand<f16>( 0.f , 1.f );
    }

  //}:                                            |
  //e_rand2pi:{                                   |

    f16 e_rand2pih(){
      return e_randpi()*2.f;
    }

  //}:                                            |
  //e_randpi:{                                    |

    f16 e_randpih(){
      return e_randunith() * f16::kPI;
    }

  //}:                                            |
//}:                                              |
//Bounds:{                                        |
  //aabb3h:{                                      |
    //intersects:{                                |

#ifdef __APPLE__
  #pragma mark -
  #pragma mark Bounding volumes
  #pragma mark -
#endif

      //http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
      namespace EON{
        template<> bool aabb3h::intersects( const ray3h& in, f16& out )const{
          vec3h dirfrac;
          const pt3h& lb = min;
          const pt3h& rt = max;
          // r.dir is unit direction vector of ray
          const vec3h d = in.d.normalized();
          dirfrac.x = 1.f / d.x;
          dirfrac.y = 1.f / d.y;
          dirfrac.z = 1.f / d.z;
          // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
          // r.org is origin of ray
          const f16& t1 =( lb.x - in.p.x )*dirfrac.x;
          const f16& t2 =( rt.x - in.p.x )*dirfrac.x;
          const f16& t3 =( lb.y - in.p.y )*dirfrac.y;
          const f16& t4 =( rt.y - in.p.y )*dirfrac.y;
          const f16& t5 =( lb.z - in.p.z )*dirfrac.z;
          const f16& t6 =( rt.z - in.p.z )*dirfrac.z;
          const f16& tmin = f16::max( f16::max( t1.min( t2 ), t3.min( t4 )), t5.min( t6 ));
          const f16& tmax = f16::min( f16::min( t1.max( t2 ), t3.max( t4 )), t5.max( t6 ));
          // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behind us
          if( tmax < 0.f ){
            out = tmax;
            return false;
          }
          // if tmin > tmax, ray doesn't intersect AABB
          if( tmin > tmax ){
            out = tmax;
            return false;
          }
          out = tmin;
          return true;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb3h::contains( const pt3h& P )const{
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
        template<> bool aabb3h::contains( const aabb3h& B )const{
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
        template<> bool aabb3h::contains( const triangleh& t )const{
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
    //empty:{                                     |

      namespace EON{
        template<> bool aabb3h::empty()const{
          if(( min != Point3( -kMax ))||( min == Point3( kMax ))){
            // No points added to the bounding box.
            return false;
          }
          if(( max != Point3( kMax ))||( max == Point3( -kMax ))){
            // No points added to the bounding box.
            return false;
          }
          return true;
        }
      }

    //}:                                          |
    //valid:{                                     |

      namespace EON{
        template<> bool aabb3h::valid()const{
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
            if( min.z > max.z ){
              return false;
            }
          }
          return true;
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb3h::clear(){
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
  //aabb2h:{                                      |
    //scaledToClip:{                              |

      namespace EON{
        template<> aabb2h aabb2h::scaledToClip( const f16& scale )const{
          aabb2h b = *this;
          b.min.scaleToClip( scale );
          b.max.scaleToClip( scale );
          return b;
        }
      }

    //}:                                          |
    //scaleToClip:{                               |

      namespace EON{
        template<> void aabb2h::scaleToClip( const f16& scale ){
          min.scaleToClip( scale );
          max.scaleToClip( scale );
        }
      }

    //}:                                          |
    //scaleBack:{                                 |

      namespace EON{
        template<> void aabb2h::scaleBack(){
          const f16& cx = 1024;
          const f16& cy = 768;
          min.x =( min.x+k1 )*kHalf*cx;
          min.y =( min.y+k1 )*kHalf*cy;
          max.x =( max.x+k1 )*kHalf*cx;
          max.y =( max.y+k1 )*kHalf*cy;
        }
      }

    //}:                                          |
    //empty:{                                     |

      namespace EON{
        template<> bool aabb2h::empty()const{
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
        template<> bool aabb2h::valid()const{
          if( !f16( min.x ).valid() ){
            return false;
          }
          if( !f16( min.y ).valid() ){
            return false;
          }
          if( !f16( max.x ).valid() ){
            return false;
          }
          if( !f16( max.y ).valid() ){
            return false;
          }
          if( min.x > max.x ){
            return false;
          }
          if( min.y > max.y ){
            return false;
          }
          return true;
        }
      }

    //}:                                          |
    //contains:{                                  |

      namespace EON{
        template<> bool aabb2h::contains( const pt2h& P )const{
          return(( P.x >= min.x )&&( P.y >= min.y )&&( P.x < max.x )&&( P.y < max.y ));
        }
      }

      namespace EON{
        template<> bool aabb2h::contains( const triangleh& t )const{
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
        template<> bool aabb2h::contains( const aabb2h& B )const{
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
        template<> bool aabb2h::intersects( const ray2h& r )const{
          const pt2h v[2]={ r.p, r.p + r.d };
          return line2rect( v, *this );
        }
      }

      namespace EON{
        template<> bool aabb2h::intersects( const aabb2h& b )const{

          //--------------------------------------------------------------------
          // Get all the corners.
          //--------------------------------------------------------------------

          const pt2h bA = b.min;
          const pt2h bB = pt2h( b.max.x, b.min.y );
          const pt2h bC = b.max;
          const pt2h bD = pt2h( b.min.x, b.max.y );

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

          const ray2h bEdge0( bA, bB-bA );
          const ray2h bEdge1( bB, bC-bB );
          const ray2h bEdge2( bC, bD-bC );
          const ray2h bEdge3( bD, bA-bD );
          const ray2h bDiag0( bA, bC-bA );
          const ray2h bDiag1( bB, bD-bB );

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
        template<> bool aabb2h::clipAgainst( const aabb2h& b ){
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
    //circleh:{                                   |

      namespace EON{
        template<> circleh aabb2h::circle()const{
          const f16& x = (max.x - min.x) * kHalf;
          const f16& y = (max.y - min.y) * kHalf;
          return circleh( min.x + x, min.y + y, x.max( y ));
        }
      }

    //}:                                          |
    //clear:{                                     |

      namespace EON{
        template<> void aabb2h::clear(){
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
        template<> f16 rgbah::multiply( const f16& base, const f16& blend ){
            return( base*blend );
        }
      }

    //}:                                          |
    //lighten:{                                   |

      namespace EON{
        template<> f16 rgbah::lighten( const f16& base, const f16& blend ){
          return max( base, blend );
        }
      }

    //}:                                          |
    //darken:{                                    |

      namespace EON{
        template<> f16 rgbah::darken( const f16& base, const f16& blend ){
          return min( base, blend );
        }
      }

    //}:                                          |
    //average:{                                   |

      namespace EON{
        template<> f16 rgbah::average( const f16& base, const f16& blend ){
          return( base+blend )*kHalf;
        }
      }

    //}:                                          |
    //linearDodge:{                               |

      namespace EON{
        template<> f16 rgbah::linearDodge( const f16& base, const f16& blend ){
          return f16::saturate( base+blend );
        }
      }

    //}:                                          |
    //linearBurn:{                                |

      namespace EON{
        template<> f16 rgbah::linearBurn( const f16& base, const f16& blend ){
          return f16::saturate( base-blend );
        }
      }

    //}:                                          |
    //difference:{                                |

      namespace EON{
        template<> f16 rgbah::difference( const f16& base, const f16& blend ){
          return abs( base-blend );
        }
      }

    //}:                                          |
    //negation:{                                  |

      namespace EON{
        template<> f16 rgbah::negation( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          return( k1 - abs( k1 - base - blend ));
        }
      }

    //}:                                          |
    //screen:{                                    |

      namespace EON{
        template<> f16 rgbah::screen( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          return k1-(( k1 - base )*( k1 - blend ));
        }
      }

    //}:                                          |
    //exclusion:{                                 |

      namespace EON{
        template<> f16 rgbah::exclusion( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          return( base+blend-k2*base*blend );
        }
      }

    //}:                                          |
    //overlay:{                                   |

      namespace EON{
        template<> f16 rgbah::overlay( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          return( base < kHalf )?( k2 * base * blend ):( k1 - k2 *( k1 - base )*( k1 - blend ));
        }
      }

    //}:                                          |
    //hardLight:{                                 |

      namespace EON{
        template<> f16 rgbah::hardLight( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          return( blend < kHalf )?( k2 * blend * base ):( k1 - k2 *( k1 - blend )*( k1 - base ));
        }
      }

    //}:                                          |
    //softLight:{                                 |

      namespace EON{
        template<> f16 rgbah::softLight( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          if( blend < kHalf ){
            return( k2 * base * blend )+( base*base )*( k1 - k2 * blend );
          }
          return( sqrt( base )*( k2 * blend - k1 )+ k2 * base * ( k1 - blend ));
        }
      }

    //}:                                          |
    //softDodge:{                                 |

      namespace EON{
        template<> f16 rgbah::softDodge( const f16& base, const f16& blend ){
          return(( base + blend ) < k1 )?(( base * kHalf ) / ( k1 - blend )):( k1 - (( kHalf*( k1 - blend )) / base ));
        }
      }

    //}:                                          |
    //colorDodge:{                                |

      namespace EON{
        template<> f16 rgbah::colorDodge( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //colorBurn:{                                 |

      namespace EON{
        template<> f16 rgbah::colorBurn( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          if( blend > k0 ){
            return max(( k1 -(( k1 - base ) / blend )), k0 );
          }
          return blend;
        }
      }

    //}:                                          |
    //linearLight:{                               |

      namespace EON{
        template<> f16 rgbah::linearLight( const f16& base, const f16& blend ){
          return( blend < kHalf ) ? linearBurn( base, k2*blend ) : linearDodge( base, k2*( blend-kHalf ));
        }
      }

    //}:                                          |
    //vividLight:{                                |

      namespace EON{
        template<> f16 rgbah::vividLight( const f16& base, const f16& blend ){
          return( blend < kHalf ) ? colorBurn( base,( k2 * blend )) : colorDodge( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //pinLight:{                                  |

      namespace EON{
        template<> f16 rgbah::pinLight( const f16& base, const f16& blend ){
          return( blend < kHalf ) ? darken( base,( k2 * blend )) : lighten( base,( k2 *( blend -kHalf )));
        }
      }

    //}:                                          |
    //hardMix:{                                   |

      namespace EON{
        template<> f16 rgbah::hardMix( const f16& base, const f16& blend ){
          return( vividLight( base, blend ) < kHalf ) ? k0 : k1;
        }
      }

    //}:                                          |
    //reflect:{                                   |

      namespace EON{
        template<> f16 rgbah::reflect( const f16& in_fBase, const f16& in_fBlend ){
          const f16& blend = f16::saturate( in_fBlend );
          const f16& base  = f16::saturate( in_fBase  );
          if( blend != k1 ){
            return min( base*base/( k1 - blend ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //glow:{                                      |

      namespace EON{
        template<> f16 rgbah::glow( const f16& base, const f16& blend ){
          if( base != k1 ){
            return min( blend*blend/( k1 - base ), k1 );
          }
          return k1;
        }
      }

    //}:                                          |
    //pheonix:{                                   |

      namespace EON{
        template<> f16 rgbah::phoenix( const f16& base, const f16& blend ){
          return min( base, blend ) - max( base, blend ) + k1;
        }
      }

    //}:                                          |
    //setHSV:{                                    |

      namespace EON{
        template<> void rgbah::setHSV( const f16& h, const f16& s, const f16& v ){
          if( s == k0 ){
            r = g = b = v;
          }else{
            f16 v1 = k0;
            f16 v2 = k0;
            if( v < kHalf ){
              v2 = v*( k1 + s );
            }else{
              v2 =( v + s )-( s * v );
            }
            v1 = 2.f * v - v2;
            r = hue2RGB( v1, v2, h + 0.333333f );
            g = hue2RGB( v1, v2, h );
            b = hue2RGB( v1, v2, h - 0.333333f );
          }
        }
      }

    //}:                                          |
    //hsv:{                                       |

      namespace EON{
        //https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
        template<> hsvah rgbah::hsv()const{
          hsvah result;
          const f16& cmax  = max3( r, g, b );
          const f16& cmin  = min3( r, g, b );
          const f16& delta = ( cmax - cmin );
          if( delta > 0.f ){
            if( cmax == r ){
              result.h = 60.f * (( g - b ) / delta ).mod( 6.f );
            }else if( cmax == g ){
              result.h = 60.f * ((( b - r ) / delta ) + 2.f );
            }else if(cmax == b){
              result.h = 0.f * ((( r - g ) / delta ) + 4.f );
            }
            if( cmax > 0.f ){
              result.s = delta / cmax;
            }else{
              result.s = 0.f;
            }
            result.v = cmax;
          }else{
            result.h = 0.f;
            result.s = 0.f;
            result.v = cmax;
          }
          if( result.h < 0.f ){
            result.h += 360.f;
          }
          return result;
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    namespace EON{
      template<> void rgbah::set( const u32 in_bgra ){
        const bgra v( in_bgra );
        r = half( v.r )/255;
        g = half( v.g )/255;
        b = half( v.b )/255;
        a = half( v.a )/255;
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
