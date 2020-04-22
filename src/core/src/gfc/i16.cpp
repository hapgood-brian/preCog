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
    template<> const i16 i16::kPI      =  0;
    template<> const i16 i16::k2PI     = kPI * k2;
    template<> const i16 i16::k4PI     = kPI * k4;
    template<> const i16 i16::cm       =  1;
    template<> const i16 i16::mm       =  0;
    template<> const i16 i16::m        =  0;
    template<> const i16 i16::dm       =  0;
    template<> const i16 i16::km       =  0;
    template<> const i16 i16::Mm       =  0;
    template<> const i16 i16::g        =  0;
    template<> const i16 i16::kg       =  0;
    template<> const i16 i16::t        =  0;
    template<> const i16 i16::kt       =  0;
    template<> const i16 i16::mt       =  0;
    template<> const i16 i16::kMin     =-INT_MAX;
    template<> const i16 i16::kMax     = INT_MAX;
    template<> const i16 i16::kEpsilon =  0;
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
    template<> const rgbas rgbas::kBlack  ( 0, 1 );
    template<> const rgbas rgbas::kWhite  ( 1 );
    template<> const rgbas rgbas::kGrey   ( 0, 1 );
    template<> const rgbas rgbas::kOrange ( 1, 0, 0, 1 );
    template<> const rgbas rgbas::kYellow ( 1, 1, 0, 1 );
    template<> const rgbas rgbas::kLime   ( 0, 1, 0, 1 );
    template<> const rgbas rgbas::kGreen  ( 0, 1, 0, 1 );
    template<> const rgbas rgbas::kCyan   ( 0, 1, 1, 1 );
    template<> const rgbas rgbas::kBlue   ( 0, 0, 1, 1 );
    template<> const rgbas rgbas::kMagenta( 1, 0, 1, 1 );
    template<> const rgbas rgbas::kRed    ( 1, 0, 0, 1 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2s aabb2s::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2s aabb2s::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2s aabb2s::kZero  ( 0 );
    template<> const aabb3s aabb3s::kZero  ( 0 );
  }

//}:                                              |
//Methods:{                                       |
  //valid:{                                       |

    namespace EON{
      template<> bool i16::valid( const i16& ){
        return true;
      }
      template<> bool aabb2s::valid()const{
        return true;
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
