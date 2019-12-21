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
    template<> const i32 i32::kPI      =  0;
    template<> const i32 i32::k2PI     = kPI * k2;
    template<> const i32 i32::k4PI     = kPI * k4;
    template<> const i32 i32::cm       =  1;
    template<> const i32 i32::mm       =  0;
    template<> const i32 i32::m        =  0;
    template<> const i32 i32::dm       =  0;
    template<> const i32 i32::km       =  0;
    template<> const i32 i32::Mm       =  0;
    template<> const i32 i32::g        =  0;
    template<> const i32 i32::kg       =  0;
    template<> const i32 i32::t        =  0;
    template<> const i32 i32::kt       =  0;
    template<> const i32 i32::mt       =  0;
    template<> const i32 i32::kMin     =-INT_MAX;
    template<> const i32 i32::kMax     = INT_MAX;
    template<> const i32 i32::kEpsilon =  0;
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
    template<> const rgbai rgbai::kBlack  ( 0, 1 );
    template<> const rgbai rgbai::kWhite  ( 1 );
    template<> const rgbai rgbai::kGrey   ( 0, 1 );
    template<> const rgbai rgbai::kOrange ( 1, 0, 0, 1 );
    template<> const rgbai rgbai::kYellow ( 1, 1, 0, 1 );
    template<> const rgbai rgbai::kLime   ( 0, 1, 0, 1 );
    template<> const rgbai rgbai::kGreen  ( 0, 1, 0, 1 );
    template<> const rgbai rgbai::kCyan   ( 0, 1, 1, 1 );
    template<> const rgbai rgbai::kBlue   ( 0, 0, 1, 1 );
    template<> const rgbai rgbai::kMagenta( 1, 0, 1, 1 );
    template<> const rgbai rgbai::kRed    ( 1, 0, 0, 1 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2i aabb2i::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2i aabb2i::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2i aabb2i::kZero  ( 0 );
    template<> const aabb3i aabb3i::kZero  ( 0 );
  }

//}:                                              |
//Methods:{                                       |
  //valid:{                                       |

    namespace EON{
      template<> bool i32::valid( const i32& ){
        return true;
      }
      template<> bool aabb3i::valid()const{
        return true;
      }
      template<> bool aabb2i::valid()const{
        return true;
      }
      template<> i32 i32::mod( const i32& i )const{
        return( value % i.value );
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
