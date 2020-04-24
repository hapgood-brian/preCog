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
    template<> const i8 i8::kPI      =  0;
    template<> const i8 i8::k2PI     = kPI * k2;
    template<> const i8 i8::k4PI     = kPI * k4;
    template<> const i8 i8::cm       =  1;
    template<> const i8 i8::mm       =  0;
    template<> const i8 i8::m        =  0;
    template<> const i8 i8::dm       =  0;
    template<> const i8 i8::km       =  0;
    template<> const i8 i8::Mm       =  0;
    template<> const i8 i8::g        =  0;
    template<> const i8 i8::kg       =  0;
    template<> const i8 i8::t        =  0;
    template<> const i8 i8::kt       =  0;
    template<> const i8 i8::mt       =  0;
    template<> const i8 i8::kMin     =-CHAR_MAX;
    template<> const i8 i8::kMax     = CHAR_MAX;
    template<> const i8 i8::kEpsilon =  0;
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
    template<> const rgbab rgbab::kBlack  ( 0, 1 );
    template<> const rgbab rgbab::kWhite  ( 1 );
    template<> const rgbab rgbab::kGrey   ( 0, 1 );
    template<> const rgbab rgbab::kOrange ( 1, 0, 0, 1 );
    template<> const rgbab rgbab::kYellow ( 1, 1, 0, 1 );
    template<> const rgbab rgbab::kLime   ( 0, 1, 0, 1 );
    template<> const rgbab rgbab::kGreen  ( 0, 1, 0, 1 );
    template<> const rgbab rgbab::kCyan   ( 0, 1, 1, 1 );
    template<> const rgbab rgbab::kBlue   ( 0, 0, 1, 1 );
    template<> const rgbab rgbab::kMagenta( 1, 0, 1, 1 );
    template<> const rgbab rgbab::kRed    ( 1, 0, 0, 1 );
  }

  /* Bounds constants */

  namespace EON{
    template<> const aabb2b aabb2b::kFullUV( 0, 0, 1, 1 );
    template<> const aabb2b aabb2b::kInvUV ( 0, 1, 1, 0 );
    template<> const aabb2b aabb2b::kZero  ( 0 );
    template<> const aabb3b aabb3b::kZero  ( 0 );
  }

//}:                                              |
//Methods:{                                       |
  //valid:{                                       |

    namespace EON{
      template<> bool i8::valid( const i8& ){
        return true;
      }
      template<> bool aabb2b::valid()const{
        return true;
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
