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
    template<> const i64 i64::kPI      =  0;
    template<> const i64 i64::k2PI     = kPI * k2;
    template<> const i64 i64::k4PI     = kPI * k4;
    template<> const i64 i64::cm       =  1;
    template<> const i64 i64::mm       =  0;
    template<> const i64 i64::m        =  0;
    template<> const i64 i64::dm       =  0;
    template<> const i64 i64::km       =  0;
    template<> const i64 i64::Mm       =  0;
    template<> const i64 i64::g        =  0;
    template<> const i64 i64::kg       =  0;
    template<> const i64 i64::t        =  0;
    template<> const i64 i64::kt       =  0;
    template<> const i64 i64::mt       =  0;
    template<> const i64 i64::kMin     =-INT_MAX;
    template<> const i64 i64::kMax     = INT_MAX;
    template<> const i64 i64::kEpsilon =  0;
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
    template<> const rgbal rgbal::kBlack  ( 0, 1 );
    template<> const rgbal rgbal::kWhite  ( 1 );
    template<> const rgbal rgbal::kGrey   ( 0, 1 );
    template<> const rgbal rgbal::kOrange ( 1, 0, 0, 1 );
    template<> const rgbal rgbal::kYellow ( 1, 1, 0, 1 );
    template<> const rgbal rgbal::kLime   ( 0, 1, 0, 1 );
    template<> const rgbal rgbal::kGreen  ( 0, 1, 0, 1 );
    template<> const rgbal rgbal::kCyan   ( 0, 1, 1, 1 );
    template<> const rgbal rgbal::kBlue   ( 0, 0, 1, 1 );
    template<> const rgbal rgbal::kMagenta( 1, 0, 1, 1 );
    template<> const rgbal rgbal::kRed    ( 1, 0, 0, 1 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2l aabb2l::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2l aabb2l::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2l aabb2l::kZero  ( 0 );
    template<> const aabb3l aabb3l::kZero  ( 0 );
  }

//}:                                              |
//Methods:{                                       |
  //valid:{                                       |

    namespace EON{
      template<> bool i64::valid( const i64& ){
        return true;
      }
      template<> bool aabb3l::valid()const{
        return true;
      }
      template<> bool aabb2l::valid()const{
        return true;
      }
      template<> i64 i64::mod( const i64& i )const{
        return( value % i.value );
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
