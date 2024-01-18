//------------------------------------------------------------------------------
//       Copyright 2014-2020 Creepy Doll Games LLC. All rights reserved.
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

struct BGRa;
struct RGBa;
struct HSVa;

//================================================+=============================
//RGBA10a2:{                                      |

  /** \brief A packed 32-bit color in RGB10A2 format.
    *
    * This structure defines a packed color for the RGB10A2 OpenGL format.
    * It is bit compatible with what GL expects for frame buffers. Use
    * with glReadPixels.
    */

  struct E_PUBLISH RGB10a2 final{

    //--------------------------------------------+-----------------------------
    //Operate:{                                   |

      e_forceinline RGB10a2& operator=( const RGBa& c ){
        r = u32(( c.r.clamped( 0, 4.01176470588235 )*255+kHalf ).floor() );
        g = u32(( c.g.clamped( 0, 4.01176470588235 )*255+kHalf ).floor() );
        b = u32(( c.b.clamped( 0, 4.01176470588235 )*255+kHalf ).floor() );
        a = u32(( c.a.clamped( 0, 0.01176470588235 )*255+kHalf ).floor() );
        return *this;
      }

      e_forceinline operator RGBa()const{
        return RGBa(
            r*0.00392156862745,
            g*0.00392156862745,
            b*0.00392156862745,
            a*0.00392156862745 );
      }

    //}:                                          |
    //--------------------------------------------+-----------------------------

    e_forceinline RGB10a2( const self& R, const self& G, const self& B, const self& A )
      : r( u32(( R.clamped( 0, 4.01176470588235f )*255+kHalf ).floor() ))
      , g( u32(( G.clamped( 0, 4.01176470588235f )*255+kHalf ).floor() ))
      , b( u32(( B.clamped( 0, 4.01176470588235f )*255+kHalf ).floor() ))
      , a( u32(( A.clamped( 0, 0.01176470588235f )*255+kHalf ).floor() ))
    {}

    e_forceinline RGB10a2( u32 R, u32 G, u32 B, u32 A )
      : r( R & 1023 )
      , g( G & 1023 )
      , b( B & 1023 )
      , a( A & 3 )
    {}

    e_forceinline RGB10a2( const u32 v )
      : all( v )
    {}

    e_forceinline RGB10a2()
      : all( 0 )
    {}

    union{
      struct{
        u32 r:10;
        u32 g:10;
        u32 b:10;
        u32 a:2;
      };
      u32 all;
    };
  };

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//BGRa:{                                          |

  /** \brief Native color format for little endian systems.
    *
    * This structure defines the BGRA format color. It is a 32-bit entity that
    * is easily convertible between the fat rgba format and the thin BGRa
    * format.
    */

  struct E_PUBLISH BGRa final{

    union{
      struct{
        u8 b; //!< Blue channel.
        u8 g; //!< Green channel.
        u8 r; //!< Red channel.
        u8 a; //!< Alpha channel.
      };
      u32 all; //!< All channels packed.
    };

    //--------------------------------------------+-----------------------------
    //Operate:{                                   |

      /** \brief RGBA assignment operator.
        *
        * This operator takes an rgba reference and assigns it to the BGRa
        * color by saturating each channel and multiplying by 255. An rgba type
        * is usually HDR so be warned that anything above 1 will be clamped in
        * this way when you convert it over.
        *
        * \param c The fat 128-bit RGBA format color.
        */

      e_forceinline void operator=( const RGBa& c ){
        r = u8( c.r.saturated()*255 );
        g = u8( c.g.saturated()*255 );
        b = u8( c.b.saturated()*255 );
        a = u8( c.a.saturated()*255 );
      }

      /** \brief RGBA conversion operator.
        *
        * This operator will convert the thin BGRa format color to the fat rgba
        * format color. The order of the channels will be seapped in the new
        * color.
        *
        * \return Returns a fat 128-bit color from this 32-bit one.
        */

      e_forceinline operator RGBa()const{
        return RGBa(
            r*0.00392156862745,
            g*0.00392156862745,
            b*0.00392156862745,
            a*0.00392156862745 );
      }

    //}:                                          |
    //--------------------------------------------+-----------------------------

    /** \brief Fat channel constructor.
      *
      * This constructor builds a 32-bit BGRa format color from four real
      * numbers.
      *
      * \param B The blue channel.
      * \param G The green channel.
      * \param R The red channel.
      * \param A The alpha channel.
      */

    e_forceinline BGRa( const self& B, const self& G, const self& R, const self& A )
      : b( u8( B.saturated()*255 ))
      , g( u8( G.saturated()*255 ))
      , r( u8( R.saturated()*255 ))
      , a( u8( A.saturated()*255 ))
    {}

    /** \brief Thin channel constructor.
      *
      * This constructor builds a 32-bit BGRa format color from four bytes.
      *
      * \param B The blue channel.
      * \param G The green channel.
      * \param R The red channel.
      * \param A The alpha channel.
      */

    e_forceinline BGRa( const u8 B, const u8 G, const u8 R, const u8 A=255 )
      : b( B )
      , g( G )
      , r( R )
      , a( A )
    {}

    /** \brief Construct color from 32-bit unsigned integer.
      *
      * This constructor will build a 32-bit BGRa format color from an
      * already packed integer value.
      *
      * \param v An already packed 32-bit unsigned int in BGRA format.
      */

    e_forceinline explicit BGRa( const u32 v )
      : all( v )
    {}

    /** \brief Default constructor.
      *
      * The default constructor will set the BGRa format color to black.
      */

    e_forceinline BGRa()
      : all( 0 )
    {}
  };

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//RGBi:{                                          |

  /** \brief Native color format for little endian systems.
    *
    * This structure defines the BGRA format color. It is a 32-bit entity that
    * is easily convertible between the fat rgba format and the thin RGBi
    * format.
    */

  struct E_PUBLISH RGBi final{

    //--------------------------------------------+-----------------------------
    //Structs:{                                   |

      union{
        struct{
          u8 r; //!< Blue channel.
          u8 g; //!< Green channel.
          u8 b; //!< Red channel.
          u8 a; //!< Alpha channel.
        };
        u32 all; //!< All channels packed.
      };

    //}:                                          |
    //Operate:{                                   |

      /** \brief RGBA assignment operator.
        *
        * This operator takes an rgba reference and assigns it to the RGBi
        * color by saturating each channel and multiplying by 255. An rgba type
        * is usually HDR so be warned that anything above 1 will be clamped in
        * this way when you convert it over.
        *
        * \param c The fat 128-bit RGBA format color.
        */

      e_forceinline void operator=( const RGBa& c ){
        r = u8( c.r.saturated()*255 );
        g = u8( c.g.saturated()*255 );
        b = u8( c.b.saturated()*255 );
        a = u8( c.a.saturated()*255 );
      }

      /** \brief RGBA conversion operator.
        *
        * This operator will convert the thin RGBi format color to the fat rgba
        * format color. The order of the channels will be seapped in the new
        * color.
        *
        * \return Returns a fat 128-bit color from this 32-bit one.
        */

      e_forceinline operator RGBa()const{
        return RGBa(
            r*0.00392156862745,
            g*0.00392156862745,
            b*0.00392156862745,
            a*0.00392156862745 );
      }

    //}:                                          |
    //--------------------------------------------+-----------------------------

    /** \brief Fat channel constructor.
      *
      * This constructor builds a 32-bit RGBi format color from four real
      * numbers.
      *
      * \param B The blue channel.
      * \param G The green channel.
      * \param R The red channel.
      * \param A The alpha channel.
      */

    e_forceinline RGBi( const self& B, const self& G, const self& R, const self& A )
      : b( u8( B.saturated()*255 ))
      , g( u8( G.saturated()*255 ))
      , r( u8( R.saturated()*255 ))
      , a( u8( A.saturated()*255 ))
    {}

    /** \brief Thin channel constructor.
      *
      * This constructor builds a 32-bit RGBi format color from four bytes.
      *
      * \param B The blue channel.
      * \param G The green channel.
      * \param R The red channel.
      * \param A The alpha channel.
      */

    e_forceinline RGBi( const u8 B, const u8 G, const u8 R, const u8 A=255 )
      : b( B )
      , g( G )
      , r( R )
      , a( A )
    {}

    /** \brief Construct color from 32-bit unsigned integer.
      *
      * This constructor will build a 32-bit RGBi format color from an
      * already packed integer value.
      *
      * \param v An already packed 32-bit unsigned int in BGRA format.
      */

    e_forceinline explicit RGBi( const u32 v )
      : all( v )
    {}

    /** \brief Default constructor.
      *
      * The default constructor will set the RGBi format color to black.
      */

    e_forceinline RGBi()
      : all( 0 )
    {}
  };

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//RGBa:{                                          |

  /** \brief Color class.
    *
    * This is the class for dealing with colors.
    */

  struct E_PUBLISH RGBa final{

    //--------------------------------------------+-----------------------------
    //Operate:{                                   |

      /** \brief Set by HSVa type.
        *
        * This operator will assign an HSVa color to this one.
        */

      e_forceinline RGBa& operator=( const struct HSVa& hsv ){
        set( hsv );
        return *this;
      }

      /** \brief Equality operator.
        *
        * This operator will compare two colours and return true if they are
        * equal and false otherwise.
        *
        * \param c The colour to compare this color with.
        *
        * \return Returns true if the two colors are the same or false.
        */

      e_forceinline bool operator==( const RGBa& c )const{
        if( r != c.r ){
          return false;
        }
        if( g != c.g ){
          return false;
        }
        if( b != c.b ){
          return false;
        }
        if( a != c.a ){
          return false;
        }
        return true;
      }

      /** \brief Inequality operator.
        *
        * This operator will compare two colours and return true if they are
        * unequal and false otherwise.
        *
        * \param c The colour to compare this color with.
        *
        * \return Returns true if the two colors are not the same or false.
        */

      e_forceinline bool operator!=( const RGBa& c )const{
        if( r != c.r ){
          return true;
        }
        if( g != c.g ){
          return true;
        }
        if( b != c.b ){
          return true;
        }
        if( a != c.a ){
          return true;
        }
        return false;
      }

      /** \brief Conversion to vec4 type.
        *
        * This operator converts the color to a 4 tuple vector.
        */

      e_forceinline operator Vector4()const{
        return Vector4( r, g, b, a );
      }

      /** \brief Addition operator.
        *
        * This routine will add the input color to this one.
        *
        * \param c The colour to add.
        *
        * \return Returns *this.
        */

      e_forceinline RGBa& operator+=( const RGBa& c ){
        r += c.r;
        g += c.g;
        b += c.b;
        a += c.a;
        return *this;
      }

      /** \brief Product operator.
        *
        * This routine will multiply two colours together.
        *
        * \param c The colour to multiply this color with.
        *
        * \return Returns the product of the two colors.
        */

      e_forceinline RGBa operator*( const RGBa& c )const{
        return RGBa( r*c.r, g*c.g, b*c.b, a*c.a );
      }

      e_forceinline RGBa& operator*=( const RGBa& c ){
        r *= c.r;
        g *= c.g;
        b *= c.b;
        a *= c.a;
        return *this;
      }

      /** \brief Product operator.
        *
        * This routine will multiply two colours together.
        *
        * \param scalar The scalar to multiply this color with.
        *
        * \return Returns the product of the two colors.
        */

      e_forceinline RGBa operator*( const self& scalar )const{
        return RGBa( r*scalar, g*scalar, b*scalar, a*scalar );
      }

      e_forceinline RGBa operator*( const float scalar )const{
        return RGBa( r*scalar, g*scalar, b*scalar, a*scalar );
      }

      e_forceinline RGBa& operator*=( const self& scalar ){
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
      }

      e_forceinline RGBa& operator*=( const float scalar ){
        r *= scalar;
        g *= scalar;
        b *= scalar;
        a *= scalar;
        return *this;
      }

      /** \brief Quotient operator.
        *
        * This routine will divide two colours.
        *
        * \param c The colour to divide this color with.
        *
        * \return Returns the quotient of the two colors.
        */

      e_forceinline RGBa operator/( const RGBa& c )const{
        RGBa result;
        if( c.r > 0 ){
          result.r = r/c.r;
        }else{
          result.r = 0;
        }
        if( c.g > 0 ){
          result.g = g/c.g;
        }else{
          result.g = 0;
        }
        if( c.b > 0 ){
          result.b = b/c.b;
        }else{
          result.b = 0;
        }
        result.a = a;
        return result;
      }

      /** \brief Quotient operator.
        *
        * This routine will divide the color by a scalar.
        *
        * \param scalar The scalar to scale this color by.
        *
        * \return Returns the scaled color.
        */

      e_forceinline RGBa operator/( const self& scalar )const{
        RGBa result;
        if( scalar > 0 ){
          result.r = r/scalar;
          result.g = g/scalar;
          result.b = b/scalar;
          result.a = a;
        }else{
          result = kBlack;
        }
        return result;
      }

      e_forceinline RGBa operator/( const float scalar )const{
        RGBa result;
        if( scalar > 0 ){
          result.r = r/scalar;
          result.g = g/scalar;
          result.b = b/scalar;
          result.a = a;
        }else{
          result = kBlack;
        }
        return result;
      }

      /** \brief Sum operator.
        *
        * This routine will add two colors together.
        *
        * \param c The colour to add this color to.
        *
        * \return Returns the sum of the two colors.
        */

      e_forceinline RGBa operator+( const RGBa& c )const{
        return RGBa( r+c.r, g+c.g, b+c.b );
      }

      /** \brief Difference operator.
        *
        * This routine will subtract one colour from another.
        */

      e_forceinline RGBa operator-( const RGBa& c )const{
        return RGBa( r-c.r, g-c.g, b-c.b );
      }

    //}:                                          |
    //Globals:{                                   |

      static const RGBa kBlack;
      static const RGBa kWhite;
      static const RGBa kGrey;
      static const RGBa kRed;
      static const RGBa kOrange;
      static const RGBa kYellow;
      static const RGBa kLime;
      static const RGBa kGreen;
      static const RGBa kCyan;
      static const RGBa kBlue;
      static const RGBa kMagenta;

    //}:                                          |
    //Structs:{                                   |

      union{
        self tuple[4];
        struct{
          self r;
          self g;
          self b;
          self a;
        };
      };

      /** \brief Color interpolation class.
        *
        * This interpolator is used to lerp between two colors.
        */

      struct E_PUBLISH Interpolator{

        //----------------------------------------+-----------------------------
        //Methods:{                               |

          e_forceinline void begin( const self& N, const RGBa& s, const RGBa& e ){
            m_fTimeStart = m_fTime;
            m_fTimeEnd   = m_fTime + N;
            m_cStart     = s;
            m_cEnd       = e;
          }

          e_forceinline self time()const{
            return self::saturate(( m_fTime-m_fTimeStart )/( m_fTimeEnd-m_fTimeStart ));
          }

          e_forceinline RGBa getValue()const{
            const self t = time();
            return RGBa(
                self::lerp( m_cStart.r, m_cEnd.r, t ),
                self::lerp( m_cStart.g, m_cEnd.g, t ),
                self::lerp( m_cStart.b, m_cEnd.b, t ),
                self::lerp( m_cStart.a, m_cEnd.a, t ));
          }

          e_forceinline bool isFinished()const{
            return( time()==k1 );
          }

          e_forceinline void tick( self dt ){
            m_fTime += dt;
          }

        //}:                                      |
        //----------------------------------------+-----------------------------

        Interpolator() = default;

      private:

        self m_fTimeStart = 0;
        self m_fTimeEnd   = 0;
        self m_fTime      = 0;

        e_var( RGBa, c, Start );
        e_var( RGBa, c, End );
      };

      /** \brief Channel enum class.
        *
        * This enumerated class type defines all the channels that this
        * slider object represents.
        */

      enum class Channel:u32{
        kR, //!< Red channel.
        kG, //!< Green channel.
        kB, //!< Blue channel.
        kA, //!< Alpha channel.
      };

    //}:                                          |
    //Methods:{                                   |

      /** \name Setters.
        *
        * These methods have everything to do with setting color values on this
        * object.
        *
        * @{
        */

      /** \brief Set from HSVa values.
        *
        * This routine will set the color based on the HSVa values provided.
        *
        * \param h Hue.
        * \param s Saturation.
        * \param v Value.
        */

      void setHSV( const self& h, const self& s, const self& v );

      /** \brief Set from HSVa values.
        *
        * This routine will set the color based on the HSVa values provided.
        *
        * \param c Hue.
        */

      e_forceinline void set( const HSVa& c ){
        setHSV( c.h, c.s, c.v );
      }

      /** \brief Colour setter from 32-bit unsigned BGRA.
        *
        * This routine will construct a new color from the BGRA value provided.
        \code
          rgba C;
          C.set( BGRa( 1.f, 0.f, 0.f, 1.f ).all );
        \endcode
        */

      void set( const u32 BGRa );

      /** @}
        *
        * \name Getters.
        *
        * These methods deal with accessing the color in a variety of ways.
        *
        * @{
        */

      /** \brief Return color component as three tuple vector.
        *
        * This routine will return a three-tuple vector containing just the
        * color component not the alpha.
        *
        * \return Returns a three-tuple vec3 containing the color. This is the
        * raw value and shouldn't be assumed it's in the RGB color space.
        */

      e_forceinline Vector3 bgr()const{
        return Vector3( b, g, r );
      }

      /** \brief Return color component as three tuple vector.
        *
        * This routine will return a three-tuple vector containing just the
        * color component not the alpha.
        *
        * \return Returns a three-tuple vec3 containing the color. This is the
        * raw value and shouldn't be assumed it's in the RGB color space.
        */

      e_forceinline Vector3 rgb()const{
        return Vector3( r, g, b );
      }

      /** \brief Get color in HSVa color space.
        *
        * This routine will convert the RGBa color to the HSVa space.
        */

      HSVa hsv()const;

      /** @}
        *
        * \name Saturation.
        *
        * These routines will saturate the color or in other words clamp it to
        * a parametric zero to one range.
        */

      /** \brief Get the saturated color.
        *
        * This routine will return the saturated color or in other words the
        * color with all color channels clamped between zero and one.
        *
        * \return Returns a clamped color in the range zero to one.
        */

      e_forceinline RGBa saturated()const{
        RGBa c( *this );
        c.saturate();
        return c;
      }

      /** \brief Set the saturated color.
        *
        * This routine will set the color to its saturated value.
        */

      e_forceinline void saturate(){
        r = self::saturate( r );
        g = self::saturate( g );
        b = self::saturate( b );
        a = self::saturate( a );
      }

      /** \brief Get color as greyscale.
        *
        * This routine will return the greyscale equivelent of this color.
        *
        * \return Returns a real number that represents the greyscale of this
        * color.
        */

      self greyscale()const{
        return rgb().dot( Vector3( .21f, .71f, .07f ));
      }

      /** @}
        *
        * \name The photoshop blends.
        *
        * These methods define the photoshop blends for this color. Use them to
        * more properly blend between two color channels or colors.
        *
        * @{
        */

      static self multiply   ( const self& base, const self& blend );
      static self lighten    ( const self& base, const self& blend );
      static self darken     ( const self& base, const self& blend );
      static self average    ( const self& base, const self& blend );
      static self linearDodge( const self& base, const self& blend );
      static self linearBurn ( const self& base, const self& blend );
      static self difference ( const self& base, const self& blend );
      static self negation   ( const self& base, const self& blend );
      static self screen     ( const self& base, const self& blend );
      static self exclusion  ( const self& base, const self& blend );
      static self overlay    ( const self& base, const self& blend );
      static self hardLight  ( const self& base, const self& blend );
      static self softLight  ( const self& base, const self& blend );
      static self softDodge  ( const self& base, const self& blend );
      static self colorDodge ( const self& base, const self& blend );
      static self colorBurn  ( const self& base, const self& blend );
      static self linearLight( const self& base, const self& blend );
      static self vividLight ( const self& base, const self& blend );
      static self pinLight   ( const self& base, const self& blend );
      static self hardMix    ( const self& base, const self& blend );
      static self reflect    ( const self& base, const self& blend );
      static self glow       ( const self& base, const self& blend );
      static self phoenix    ( const self& base, const self& blend );

      /** \brief Multiply two colours together.
        *
        * This routine will take this colour and the colour passed to produce a
        * final multiplied colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.multiply( b );
          a *= b;
        \endcode
        */

      e_forceinline void multiply( const RGBa& blend ){
        r = multiply( r, blend.r );
        g = multiply( g, blend.g );
        b = multiply( b, blend.b );
      }

      /** \brief Lighten two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final lightened colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.lighten( b );
          a *= b;
        \endcode
        */

      e_forceinline void lighten( const RGBa& blend ){
        r = lighten( r, blend.r );
        g = lighten( g, blend.g );
        b = lighten( b, blend.b );
      }

      /** \brief Darken two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final darkened colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.darken( b );
          a *= b;
        \endcode
        */

      e_forceinline void darken( const RGBa& blend ){
        r = darken( r, blend.r );
        g = darken( g, blend.g );
        b = darken( b, blend.b );
      }

      /** \brief Average two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final averaged colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.average( b );
          a *= b;
        \endcode
        */

      e_forceinline void average( const RGBa& blend ){
        r = average( r, blend.r );
        g = average( g, blend.g );
        b = average( b, blend.b );
      }

      /** \brief Linearly dodge two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final dodged colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.linearDodge( b );
          a *= b;
        \endcode
        */

      e_forceinline void linearDodge( const RGBa& blend ){
        r = linearDodge( r, blend.r );
        g = linearDodge( g, blend.g );
        b = linearDodge( b, blend.b );
      }

      /** \brief Linearly burn two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final burned colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.linearBurn( b );
          a *= b;
        \endcode
        */

      e_forceinline void linearBurn( const RGBa& blend ){
        r = linearBurn( r, blend.r );
        g = linearBurn( g, blend.g );
        b = linearBurn( b, blend.b );
      }

      /** \brief Get difference of two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final differenced colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.difference( b );
          a *= b;
        \endcode
        */

      e_forceinline void difference( const RGBa& blend ){
        r = difference( r, blend.r );
        g = difference( g, blend.g );
        b = difference( b, blend.b );
      }

      /** \brief Negation of two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final negated colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.negation( b );
          a *= b;
        \endcode
        */

      e_forceinline void negation( const RGBa& blend ){
        r = negation( r, blend.r );
        g = negation( g, blend.g );
        b = negation( b, blend.b );
      }

      /** \brief Perform screen operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final screened colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.screen( b );
          a *= b;
        \endcode
        */

      e_forceinline void screen( const RGBa& blend ){
        r = screen( r, blend.r );
        g = screen( g, blend.g );
        b = screen( b, blend.b );
      }

      /** \brief Perform exclusion operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final excluded colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.exclusion( b );
          a *= b;
        \endcode
        */

      e_forceinline void exclusion( const RGBa& blend ){
        r = exclusion( r, blend.r );
        g = exclusion( g, blend.g );
        b = exclusion( b, blend.b );
      }

      /** \brief Perform overlay operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final excluded colour. This is very expensive in a shader and in C++.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.overlay( b );
          a *= b;
        \endcode
        */

      e_forceinline void overlay( const RGBa& blend ){
        r = overlay( r, blend.r );
        g = overlay( g, blend.g );
        b = overlay( b, blend.b );
      }

      /** \brief Perform hard light operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final hard lit colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.hardLight( b );
          a *= b;
        \endcode
        */

      e_forceinline void hardLight( const RGBa& blend ){
        r = hardLight( r, blend.r );
        g = hardLight( g, blend.g );
        b = hardLight( b, blend.b );
      }

      /** \brief Perform soft light operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final soft lit colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.softLight( b );
          a *= b;
        \endcode
        */

      e_forceinline void softLight( const RGBa& blend ){
        r = softLight( r, blend.r );
        g = softLight( g, blend.g );
        b = softLight( b, blend.b );
      }

      /** \brief Perform soft dodge operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final soft dodged colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.softDodge( b );
          a *= b;
        \endcode
        */

      e_forceinline void softDodge( const RGBa& blend ){
        r = softDodge( r, blend.r );
        g = softDodge( g, blend.g );
        b = softDodge( b, blend.b );
      }

      /** \brief Perform colour dodge operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final colour dodged colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.colorDodge( b );
          a *= b;
        \endcode
        */

      e_forceinline void colorDodge( const RGBa& blend ){
        r = colorDodge( r, blend.r );
        g = colorDodge( g, blend.g );
        b = colorDodge( b, blend.b );
      }

      /** \brief Perform colour burn operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final colour burned colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.colorBurn( b );
          a *= b;
        \endcode
        */

      e_forceinline void colorBurn( const RGBa& blend ){
        r = colorBurn( r, blend.r );
        g = colorBurn( g, blend.g );
        b = colorBurn( b, blend.b );
      }

      /** \brief Perform linear-light operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final linearly lit colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.linearLight( b );
          a *= b;
        \endcode
        */

      e_forceinline void linearLight( const RGBa& blend ){
        r = linearLight( r, blend.r );
        g = linearLight( g, blend.g );
        b = linearLight( b, blend.b );
      }

      /** \brief Perform vivid-light operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final vividly lit colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.vividLight( b );
          a *= b;
        \endcode
        */

      e_forceinline void vividLight( const RGBa& blend ){
        r = vividLight( r, blend.r );
        g = vividLight( g, blend.g );
        b = vividLight( b, blend.b );
      }

      /** \brief Perform pin-light operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final pin lit colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.pinLight( b );
          a *= b;
        \endcode
        */

      e_forceinline void pinLight( const RGBa& blend ){
        r = pinLight( r, blend.r );
        g = pinLight( g, blend.g );
        b = pinLight( b, blend.b );
      }

      /** \brief Perform hard-mixed operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final hard mixed colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.hardMix( b );
          a *= b;
        \endcode
        */

      e_forceinline void hardMix( const RGBa& blend ){
        r = hardMix( r, blend.r );
        g = hardMix( g, blend.g );
        b = hardMix( b, blend.b );
      }

      /** \brief Perform reflect operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final reflected colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.reflect( b );
          a *= b;
        \endcode
        */

      e_forceinline void reflect( const RGBa& blend ){
        r = reflect( r, blend.r );
        g = reflect( g, blend.g );
        b = reflect( b, blend.b );
      }

      /** \brief Perform glow operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final glow colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.glow( b );
          a *= b;
        \endcode
        */

      e_forceinline void glow( const RGBa& blend ){
        r = glow( r, blend.r );
        g = glow( g, blend.g );
        b = glow( b, blend.b );
      }

      /** \brief Perform phoenix operation between two colours.
        *
        * This routine will take this colour and the colour passed to produce a
        * final phoenixed colour.
        *
        * \param blend The colour to blend into this one.
        *
        \code
          rgba a( .5f, .6f, .5f );
          rgba b( .4f, .1f, .0f );
          a.phoenix( b );
          a *= b;
        \endcode
        */

      e_forceinline void phoenix( const RGBa& blend ){
        r = phoenix( r, blend.r );
        g = phoenix( g, blend.g );
        b = phoenix( b, blend.b );
      }

      /** @} */

    //}:                                          |
    //--------------------------------------------+-----------------------------

    /** \brief Colour constructor.
      *
      * This routine will construct a new colour from its component parts.
      * \todo It takes R G B and A components not HSVa or CIExyz currently.
      */

    e_forceinline RGBa( const self& R, const self& G, const self& B, const self& A=1 )
      : r( R )
      , g( G )
      , b( B )
      , a( A )
    {}

    /** \brief Colour constructor.
      *
      * This routine will construct a new colour from its component parts.
      * \todo It takes R G B and A components not HSVa or CIExyz currently.
      */

    constexpr RGBa( const float R, const float G, const float B, const float A=1 )
      : r( R )
      , g( G )
      , b( B )
      , a( A )
    {}

    /** \brief Colour constructor.
      *
      * This routine will construct a new color from a vec3 and an alpha value.
      * It is useful for changing the alpha on an existing color.
      \code
        const rgba c = rgba( rgba::kBlack.rgb(), .5f );
      \endcode
      */

    e_forceinline explicit RGBa( const Vector3& V, const self& A )
      : r( V.x )
      , g( V.y )
      , b( V.z )
      , a( A )
    {}

    /** \brief Colour constructor.
      *
      * This routine will construct a color with two floats.
      */

    e_forceinline explicit RGBa( const self& C, const self& A )
      : r( T( C ))
      , g( T( C ))
      , b( T( C ))
      , a( A )
    {}

    /** \brief Colour constructor.
      *
      * This routine will construct a color with two floats.
      */

    constexpr explicit RGBa( const float C, const float A )
      : r( C )
      , g( C )
      , b( C )
      , a( A )
    {}

    /** \brief Colour constructor.
      *
      * This constructor will build a HDR parametric color from a fixed point
      * RGBA one.
      */

    e_forceinline RGBa( const BGRa& c )
      : r( cast( c.r )/255 )
      , g( cast( c.g )/255 )
      , b( cast( c.b )/255 )
      , a( cast( c.a )/255 )
    {}

    /** \brief Colour constructor.
      *
      * This routine will construct a new color from an HSVa type.
      *
      * \param hsv The hue, saturation and value of the incoming color.
      */

    e_forceinline RGBa( const HSVa& hsv )
        : a( hsv.a ){
      set( hsv );
    }

    /** \brief Colour constructor.
      *
      * This routine will construct a new color from a vec3 and an alpha value.
      * It is useful for changing the alpha on an existing color.
      *
      * \param v A four-tuple vector to load into the color.
      */

    e_forceinline RGBa( const Vector4& v )
      : r( v.x )
      , g( v.y )
      , b( v.z )
      , a( v.w )
    {}

    /** \brief Copy constructor.
      *
      * This constructor will copy the color from one to another.
      *
      * \param color The color to copy.
      */

    e_forceinline RGBa( const RGBa& color )
      : r( color.r )
      , g( color.g )
      , b( color.b )
      , a( color.a )
    {}

    /** \brief Scalar constructor.
      *
      * This constructor will assign all channels to a single value 't'. The
      * alpha value will be 1.
      *
      * \param scalar The scalar to load into all channels upon construction.
      */

    e_forceinline RGBa( const self& scalar )
      : r( scalar )
      , g( scalar )
      , b( scalar )
      , a( scalar )
    {}

    /** \brief Scalar constructor.
      *
      * This constructor will assign all channels to a single value 't'. The
      * alpha value will be 1.
      *
      * \param scalar The scalar to load into all channels upon construction.
      */

    constexpr RGBa( const float scalar )
      : r( scalar )
      , g( scalar )
      , b( scalar )
      , a( scalar )
    {}

    /** \brief Default colour constructor.
      *
      * The default constructor will create black with no transparency.
      */

    e_forceinline RGBa(){
      r=g=b=k0;
      a=k1;
    }
  };

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//HSVa:{                                          |

  /** \brief HSVa class.
    *
    * This class defines the HSVa color space.
    */

  struct E_PUBLISH HSVa final{
    self h = 0;
    self s = 0;
    self v = 0;
    self a = 1;
  };

//}:                                              |
//================================================+=============================
