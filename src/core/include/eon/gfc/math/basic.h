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

#pragma once

//================================================|=============================
//Operate:{                                       |
  //operator%:{                                   |

    e_forceinline self operator%( const self& x )const{
      return mod( x );
    }

  //}:                                            |
  //operator++:{                                  |

    e_forceinline self& operator++( s32 ){
      value += 1;
      return*this;
    }

    e_forceinline self& operator++(){
      value += 1;
      return*this;
    }

  //}:                                            |
  //operator--:{                                  |

    e_forceinline self& operator--( s32 ){
      value -= 1;
      return*this;
    }

    e_forceinline self& operator--(){
      value -= 1;
      return*this;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Methods:{                                       |
  //smoothstep:{                                  |

    e_forceinline friend self e_smoothstep( const self& edge0, const self& edge1, const self& x ){
      return edge0.smoothstep( edge1, x );
    }

    e_forceinline static self smoothstep( const self& edge0, const self& edge1, const self& x ){
      return edge0.smoothstep( edge1, x );
    }

    self smoothstep( const self& edge2, const self& x );

  //}:                                            |
  //parametric:{                                  |

    e_forceinline friend self e_parametric( const self& t, const self& x, const self& y, const self& z ){
      return( t-x )/( y-z );
    }

    e_forceinline static self parametric( const self& t, const self& x, const self& y, const self& z ){
      return( t-x )/( y-z );
    }

    e_forceinline self parametric( const self& a, const self& b, const self& c )const{
      return parametric( *this, a, b, c );
    }

    e_forceinline self& setParametric( const self& a, const self& b, const self& c ){
      value = parametric( *this, a, b, c );
      return *this;
    }

  //}:                                            |
  //square(d):{                                   |

    e_forceinline friend self e_square( const self& x ){
      return x.squared();
    }

    e_forceinline static self squared( const self& x ){
      return x.squared();
    }

    e_forceinline self squared()const{
      return value * value;
    }

    e_forceinline self& square(){
      value *= value;
      return *this;
    }

  //}:                                            |
  //cube(d):{                                     |

    e_forceinline friend self e_cube( const self& x ){
      return x.cubed();
    }

    e_forceinline static self cubed( const self& x ){
      return x.cubed();
    }

    e_forceinline self cubed()const{
      return value * value * value;
    }

    e_forceinline self& cube(){
      value *= value;
      value *= value;
      return *this;
    }

  //}:                                            |
  //mod:{                                         |

    e_forceinline friend self e_mod( const self& x, const self& y ){
      return x.mod( y );
    }

    e_forceinline static self mod( const self& x, const self& y ){
      return x.mod( y );
    }

    self mod( const self& y )const;

  //}:                                            |
  //frac:{                                        |

    /** \brief Get fractional and integral parts.
      *
      * This routine will return the fractional and integral parts of a real
      * number.
      *
      * \param x The real number to extract fractional and integral parts from.
      * \param out_integral Will be set the integral part of the number
      *
      * \return Returns the fractional part of the number.
      */

    e_forceinline friend self e_frac( const self& x, self& out_integral ){
      return x.frac( out_integral );
    }

    /** \brief Get fractional and integral parts.
      *
      * This routine will return the fractional and integral parts of a real
      * number.
      *
      * \param x The real number to extract fractional and integral parts from.
      * \param out_integral Will be set the integral part of the number
      *
      * \return Returns the fractional part of the number.
      */

    e_forceinline static self frac( const self& x, self& out_integral ){
      return x.frac( out_integral );
    }

    /** \brief Get fractional and integral parts.
      *
      * This routine will return the fractional part of a real number.
      *
      * \param x The real number to extract fractional and integral parts from.
      *
      * \return Returns the fractional part of the number.
      */

    e_forceinline friend self e_frac( const self& x ){
      return x.frac();
    }

    /** \brief Get fractional and integral parts.
      *
      * This routine will return the fractional part of a real number.
      *
      * \param x The real number to extract fractional and integral parts from.
      *
      * \return Returns the fractional part of the number.
      */

    e_forceinline static self frac( const self& x ){
      return x.frac();
    }

    /** \brief Get fractional and integral parts.
      *
      * This routine will return the fractional part of a real number.
      *
      * \return Returns the fractional part of the number.
      */

    e_forceinline self frac()const{
      self i;
      return frac( i );
    }

    /** \brief Get fractional and integral parts.
      *
      * This routine will return the fractional and integral parts of a real
      * number.
      *
      * \param out_integral Will be set the integral part of the number
      *
      * \return Returns the fractional part of the number.
      */

    self frac( self& out_integral )const;

  //}:                                            |
  //saturate:{                                    |

    e_forceinline friend bool e_is_saturated( const self& t ){
      return( t >= k0 )&&( t <= k1 );
    }

    e_forceinline static bool is_saturated( const self& t ){
      return( t >= k0 )&&( t <= k1 );
    }

    e_forceinline friend self e_saturate( const self& x ){
      return x.saturated();
    }

    e_forceinline static self saturate( const self& x ){
      return x.saturated();
    }

    self saturated()const;
    void saturate();

  //}:                                            |
  //clamp[ed]:{                                   |

    e_forceinline self clamped( const self& y, const self& z )const{
      self r( *this );
      r.clamp( y, z );
      return r;
    }

    e_forceinline self clamped( const self& z )const{
      self r( *this );
      r.clamp( 0, z );
      return r;
    }

    e_forceinline friend self e_clamp( const self& x, const self& y, const self& z ){
      return x.clamped( y, z );
    }

    e_forceinline static self clamp( const self& x, const self& y, const self& z ){
      return x.clamped( y, z );
    }

    e_forceinline friend self e_clamp( const self& x, const self& z ){
      return x.clamped( z );
    }

    e_forceinline void clamp( const self& y, const self& z ){
      *this = min( max( *this, y ), z );
    }

    e_forceinline void clamp( const self& z ){
      clamp( 0, z );
    }

  //}:                                            |
  //rad:{                                         |

    /** \brief Convert degrees to radians.
      *
      * This routine will convert the given value in degrees to radians.
      *
      * \param deg Angle in degrees.
      */

    e_forceinline friend self e_rad( const self& deg ){
      return deg.rad();
    }

    /** \brief Convert degrees to radians.
      *
      * This routine will convert the given value in degrees to radians.
      *
      * \param deg Angle in degrees.
      */

    e_forceinline static self rad( const self& deg ){
      return deg.rad();
    }

    /** \brief Convert degrees to radians.
      *
      * This routine will convert the given value in degrees to radians.
      */

    e_forceinline self rad()const{
      return( value * T( .0174532925199432957692369055556f ));
    }

  //}:                                            |
  //deg:{                                         |

    /** \brief Convert radians to degrees.
      *
      * This routine will convert the given value in radians to degrees.
      *
      * \param rad Angle in radians.
      */

    e_forceinline friend self e_deg( const self& rad ){
      return rad.deg();
    }

    /** \brief Convert radians to degrees.
      *
      * This routine will convert the given value in radians to degrees.
      *
      * \param rad Angle in radians.
      */

    e_forceinline static self deg( const self& rad ){
      return rad.deg();
    }

    /** \brief Convert radians to degrees.
      *
      * This routine will convert the given value in radians to degrees.
      */

    e_forceinline self deg()const{
      return( value * T( 57.295779513082320876798161804285f ));
    }

  //}:                                            |
  //max3:{                                        |

    e_forceinline friend self e_max3f( const self& x, const self& y, const self& z ){
      return x.max3( y, z );
    }

    e_forceinline static self max3( const self& x, const self& y, const self& z ){
      return x.max3( y, z );
    }

    e_forceinline self max3( const self& y, const self& z )const{
      self w = value;
      w=(  w < y ) ? y : w;
      w=(  w < z ) ? z : w;
      return w;
    }

  //}:                                            |
  //min3:{                                        |

    e_forceinline friend self e_min3f( const self& x, const self& y, const self& z ){
      return x.min3( y, z );
    }

    e_forceinline static self min3( const self& x, const self& y, const self& z ){
      return x.min3( y, z );
    }

    e_forceinline self min3( const self& y, const self& z )const{
      self w = value;
      w=(  w < y ) ? w : y;
      w=(  w < z ) ? w : z;
      return w;
    }

  //}:                                            |
  //max:{                                         |

    e_forceinline friend self e_maxf( const self& x, const self& y ){
      return x.max( y );
    }

    e_forceinline static self max( const self& x, const self& y ){
      return x.max( y );
    }

    e_forceinline self max( const self& y )const{
      return( *this < y ) ? y : *this;
    }

  //}:                                            |
  //min:{                                         |

    e_forceinline friend self e_minf( const self& x, const self& y ){
      return x.min( y );
    }

    e_forceinline static self min( const self& x, const self& y ){
      return x.min( y );
    }

    e_forceinline self min( const self& y )const{
      return(( *this < y ) ? *this : y );
    }

  //}:                                            |
  //floor/ceil:{                                  |

    e_forceinline friend self e_floor( const self& x ){
      return x.floor();
    }

    e_forceinline static self floor( const self& x ){
      return x.floor();
    }

    self floor()const;

    e_forceinline friend self e_ceil( const self& x ){
      return x.ceil();
    }

    e_forceinline static self ceil( const self& x ){
      return x.ceil();
    }

    self ceil()const;

  //}:                                            |
  //rsqrt:{                                       |

    e_forceinline friend self e_rsqrt( const self& x ){
      return x.rsqrt();
    }

    e_forceinline static self rsqrt( const self& x ){
      return x.rsqrt();
    }

    self rsqrt()const;

  //}:                                            |
  //sqrt:{                                        |

    e_forceinline friend self e_sqrt( const self& x ){
      return x.sqrt();
    }

    e_forceinline static self sqrt( const self& x ){
      return x.sqrt();
    }

    self sqrt()const;

  //}:                                            |
  //lerp:{                                        |

    //http://paulbourke.net/miscellaneous/interpolation/

    e_forceinline friend self e_lerp( const self& x, const self& y, const self& t ){
      return x.lerp( y, t );
    }

    e_forceinline static self lerp( const self& x, const self& y, const self& t ){
      return x.lerp( y, t );
    }

    e_forceinline self lerp( const self& y, const self& t )const{
      e_assert(( t >= k0 )&&( t <= k1 ));
      return value * ( k1 - t.value ) + y.value * t.value;
    }

    e_forceinline friend self e_cosLerp( const self& x, const self& y, const self& t ){
      x.cosLerp( y, t );
    }

    e_forceinline static self cosLerp( const self& x, const self& y, const self& t ){
      x.cosLerp( y, t );
    }

    e_forceinline self cosLerp( const self& y, const self& t )const{
      const self& t2 = ( k1-cos( t*kPI ))*kHalf;
      return( value * ( T( 1 ) - t2.value ) + y.value * t2.value );
    }

  //}:                                            |
  //round:{                                       |

    e_forceinline friend self e_round( const self& x ){
      return x.round();
    }

    e_forceinline static self round( const self& x ){
      return x.round();
    }

    e_forceinline self round()const{
      return( *this + kHalf ).floor();
    }

  //}:                                            |
  //exp:{                                         |

    e_forceinline friend self e_exp( const self& x ){
      return x.exp();
    }

    e_forceinline static self exp( const self& x ){
      return x.exp();
    }

    self exp()const;

  //}:                                            |
  //abs:{                                         |

    e_forceinline friend self e_abs( const self& x ){
      return x.abs();
    }

    e_forceinline static self abs( const self& x ){
      return x.abs();
    }

    self abs()const;

  //}:                                            |
  //pow:{                                         |

    e_forceinline friend self e_pow( const self& x, const self& y ){
      return x.pow( y );
    }

    e_forceinline static self pow( const self& x, const self& y ){
      return x.pow( y );
    }

    self pow( const self& y )const;

  //}:                                            |
  //atan/tan:{                                    |

    e_forceinline friend self e_atan2( const self& x, const self& y ){
      return x.atan2( y );
    }

    e_forceinline static self atan2( const self& x, const self& y ){
      return x.atan2( y );
    }

    e_forceinline friend self e_atan( const self& x ){
      return x.atan();
    }

    e_forceinline static self atan( const self& x ){
      return x.atan();
    }

    self atan2( const self& y )const;
    self atan()const;

    e_forceinline friend self e_tan( const self& x ){
      return x.tan();
    }

    e_forceinline static self tan( const self& x ){
      return x.tan();
    }

    self tan()const;

  //}:                                            |
  //acos/cos:{                                    |

    e_forceinline friend self e_acos( const self& x ){
      return x.acos();
    }

    e_forceinline static self acos( const self& x ){
      return x.acos();
    }

    self acos()const;

    e_forceinline friend self e_cos( const self& x ){
      return x.cos();
    }

    e_forceinline static self cos( const self& x ){
      return x.cos();
    }

    self cos()const;

  //}:                                            |
  //asin/sin:{                                    |

    e_forceinline friend self e_asin( const self& x ){
      return x.asin();
    }

    e_forceinline static self asin( const self& x ){
      return x.asin();
    }

    self asin()const;

    e_forceinline friend self e_sin( const self& x ){
      return x.sin();
    }

    e_forceinline static self sin( const self& x ){
      return x.sin();
    }

    self sin()const;

  //}:                                            |
  //iround:{                                      |

    /** \brief Get rounded integer.
      *
      * This routine returns the rounded integer form of member x.
      */

    e_forceinline friend s32 iround( const self& x ){
      return x.iround();
    }

    /** \brief Get rounded integer.
      *
      * This routine returns the rounded integer form of member x.
      */

    e_forceinline static s32 iround( const self& x ){
      return x.iround();
    }

    /** \brief Get rounded integer.
      *
      * This routine returns the rounded integer form of member x.
      */

    s32 iround()const;

  //}:                                            |
  //is_sse:{                                      |

    constexpr static bool is_sse(){
      #ifdef __SSE__
        return( sizeof( T ) == 4 );
      #else
        return false;
      #endif
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
