//------------------------------------------------------------------------------
//         Copyright 2015 Superluminal Games LLC.  All rights reserved.
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

#pragma once

//================================================|=============================
//CatmullRomSpline:{                              |

  /** \brief Catmull-Rom spline.
    *
    * This class defines catmull-rom splines using the excellent centripetal
    * form documented on Wikipedia. Based on the alpha value you'll get tighter
    * fitting through the control points or looser. An alpha value of 0.5 makes
    * it a centripetal spline, a value of 0.0 makes it a standard catmull-rom
    * spline, and a value of 1.0 makes it a loose chordal spline.
    *
    * If you want a payload with your control points, such as roation, then
    * pass your payload class as C and make sure all the standard vector
    * operators like * and + are supported.
    *
    * \tparam C A vector type such as pt3, pt2, vec4, vec3 or vec2.
    * \tparam A The alpha value used in the spline in fixed point.
    */

  template<typename C, CatmullRomAlpha A> struct E_PUBLISH CatmullRomSpline final{

    //--------------------------------------------|-----------------------------
    //Methods:{                                   |

      e_forceinline static self alpha(){
        return self( A )/1000.f;
      }

      /** \brief Insert control point.
        *
        * This routine will insert a new control point into the spline. The
        * final plot will go through this point guaranteed.
        *
        * \param cp The control point.
        */

      e_forceinline void insertControlPoint( const C& cp ){
        m_vControlPoints.push( cp );
      }

      /** \brief Test if renderable.
        *
        * This routine will return true if there are enough control points to
        * produce a proper spline or false if there's not.
        */

      e_forceinline bool isRenderable()const{
        return( !empty() && !( m_vControlPoints.size() % 4 ));
      }

      /** \brief Test if the control points are empty.
        *
        * This routine returns true if there are some control points and false
        * otherwise. Just because there are control points doesn't mean you
        * can produce a proper spline. You need at least 4 points for that.
        */

      e_forceinline bool empty()const{
        return m_vControlPoints.empty();
      }

      /** \brief Get control point count.
        *
        * This routine will return the number of control points.
        */

      e_forceinline u32 size()const{
        return m_vControlPoints.size();
      }

      /** \brief Produces a spline you can draw with.
        *
        * This routine will return a spline that's good enough to draw from.
        * You must have at least four control points for this to work so make
        * sure you call isRenderable() first.
        *
        * \param pointsPerSegment This value controls how many points will be
        * used to interpolate between each control point along the spline.
        *
        * \return Returns a gfc vector of points that represent all the points
        * along the spline.
        */

      e_noinline gfc::vector<C> toSpline( const u32 pointsPerSegment )const{
        gfc::vector<C> spline;
        if( pointsPerSegment > 0 ){
          gfc::vector<f32> knots = getKnotSequence();
          const u32 n = size();
          e_assert( n == knots.size() );
          spline.push( m_vControlPoints[ 0 ]);
          for( u32 i=0; ( i + 4 )<=n; ++i ){
            const C&    v0   = m_vControlPoints[ i + 0 ];
            const C&    v1   = m_vControlPoints[ i + 1 ];
            const C&    v2   = m_vControlPoints[ i + 2 ];
            const C&    v3   = m_vControlPoints[ i + 3 ];
            const self& t0   = knots[ i + 0 ];
            const self& t1   = knots[ i + 1 ];
            const self& t2   = knots[ i + 2 ];
            const self& t3   = knots[ i + 3 ];
            const self& rcpA = 1.f/( t1 - t0 );
            const self& rcpB = 1.f/( t2 - t1 );
            const self& rcpC = 1.f/( t3 - t2 );
            const self& rcpD = 1.f/( t2 - t0 );
            const self& rcpE = 1.f/( t3 - t1 );
            const self& d    = ( t2 - t1 )/self( pointsPerSegment );
            self t = t1;
            for( s32 j=pointsPerSegment-1; j>=0; --j, t+=d ){
              if( !j ){
                t = t2;
              }
              const C& A1 = ( t1 - t ) * rcpA * v0 + ( t - t0 ) * rcpA * v1;
              const C& A2 = ( t2 - t ) * rcpB * v1 + ( t - t1 ) * rcpB * v2;
              const C& A3 = ( t3 - t ) * rcpC * v2 + ( t - t2 ) * rcpC * v3;
              const C& B1 = ( t2 - t ) * rcpD * A1 + ( t - t0 ) * rcpD * A2;
              const C& B2 = ( t3 - t ) * rcpE * A2 + ( t - t1 ) * rcpE * A3;
              const C& C0 = ( t2 - t ) * rcpB * B1 + ( t - t1 ) * rcpB * B2;
              spline.push( C0 );
            }
          }
          spline.push( m_vControlPoints.back() );
        }
        return spline;
      }

    //}:                                          |
    //Getters:{                                   |

      e_forceinline const gfc::vector<C>& toControlPoints()const{
        return m_vControlPoints;
      }

    //}:                                          |
    //--------------------------------------------|-----------------------------

    CatmullRomSpline() = default;

  private:

    e_noinline self knotInSeq( const self& ti, const C& P0, const C& P1 )const{
      Maths squares = k0;
      for( u32 i=0; i<C::kTuples; ++i ){
        const self& x = P1[ i ] - P0[ i ];
        squares += x*x;
      }
      const self& a = alpha();
      return squares.pow( .5f * a ) + ti;
    }

    e_noinline gfc::vector<f32> getKnotSequence()const{
      gfc::vector<f32> knots;
      knots.reserve( size() );
      f32 knotT = 0;
      knots.push( knotT );
      for( u32 n=size()-1, i=0; i<n; ++i ){
        knotT = knotInSeq( knotT
            , m_vControlPoints[ i+0 ]
            , m_vControlPoints[ i+1 ]);
        knots.push( knotT );
      }
      return knots;
    }

    gfc::vector<C> m_vControlPoints;
  };

//}:                                              |
//================================================|=============================
