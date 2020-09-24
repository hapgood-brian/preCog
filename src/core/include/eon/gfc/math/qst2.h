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

//================================================|=============================
//QST2:{                                          |

  struct E_PUBLISH QST2 final{

    //--------------------------------------------|-----------------------------
    //Operate:{                                   |

      /** \brief In place translation operator.
        *
        * This routine will add a two-tuple vector to the position of this
        * 2D QST object.
        *
        * \param V The vector to add to the position of this QST.
        *
        * \return Returns the dereferenced this pointer.
        */

      e_forceinline QST2& operator+=( const Vector2& V ){
        x() += V.x;
        y() += V.y;
        return *this;
      }

      /** \brief Transform this QST in place.
        *
        * This routine will transform this QST by multiplying the components of
        * the input screen space L2W against this.
        *
        * \param L2W The screen space transform to multiply against this.
        *
        * \return Returns the dereferenced this pointer.
        */

      e_forceinline QST2& operator*=( const QST2& L2W ){
        x()         += L2W.m_tPosition.x;
        y()         += L2W.m_tPosition.y;
        m_vRotation *= L2W.m_vRotation;
        m_vScale.x  *= L2W.m_vScale.x;
        m_vScale.y  *= L2W.m_vScale.y;
        return *this;
      }

      /** \brief Multiply two QST2 transforms in place.
        *
        * This operator multiplies two QSTs as if they were normal matrices. It
        * adds the two positions together, concatenates the quaternions and
        * multiplies the scales. QSTs are an efficient representation because
        * of the fact that they only store two vectors. The downside is you
        * lose non-uniform scaling. One scalar for all three axis in other
        * words.
        *
        * \param L2W The local-to-world QST2 format transform.
        */

      e_forceinline QST2 operator*( const QST2& L2W )const{
        QST2 qst = *this;
        qst *= L2W;
        return qst;
      }

      /** \brief Multiply QST against a 2D point.
        *
        * This routine will multiply the input point against the current
        * translation.
        *
        * \return Returns a 2D point.
        */

      Point2 operator*( const Point2& )const;

    //}:                                          |
    //Methods:{                                   |
      //[components]:{                            |

        e_forceinline self x()const{
          e_assert( valid() );
          return toPosition().x;
        }

        e_forceinline self& x(){
          e_assert( valid() );
          return toPosition().x;
        }

        e_forceinline self y()const{
          e_assert( valid() );
          return toPosition().y;
        }

        e_forceinline self& y(){
          e_assert( valid() );
          return toPosition().y;
        }

        e_forceinline const Quaternion& q()const{
          e_assert( valid() );
          return m_vRotation;
        }

        e_forceinline Quaternion& q(){
          e_assert( valid() );
          return m_vRotation;
        }

        e_forceinline const Vector2& s()const{
          e_assert( valid() );
          return m_vScale;
        }

        e_forceinline Vector2& s(){
          e_assert( valid() );
          return m_vScale;
        }

        e_forceinline const Point2& t()const{
          e_assert( valid() );
          return m_tPosition;
        }

        e_forceinline Point2& t(){
          e_assert( valid() );
          return m_tPosition;
        }

      //}:                                        |
      //[rotation]:{                              |

        /** \brief Set the axis angle of the QST2.
          *
          * This routine will set the axis and angle of the rotation. Even
          * though this 2D QST uses a quaternion for it's rotations the Z axis
          * should always be zero.
          *
          * \param axis The 2D axis' of rotation.
          * \param angleDeg The angle on that axis in degrees.
          */

        e_forceinline void setAxisAngle( const Vector2& axis, const self& angleDeg ){
          toRotation().setAxisAngle( Vector3( axis, 0.f ), angleDeg );
          e_assert( valid() );
        }

        /** \brief Apply delta rotation to this QST.
          *
          * This routine will rotate the QST by the given axis and angle.
          *
          * \param axis The 2D axis of rotation.
          * \param angleDeg The angle of rotation in degrees.
          */

        e_forceinline void rotate( const Vector2& axis, const self& angleDeg ){
          toRotation().rotate( Vector3( axis, 0.f ), angleDeg );
          e_assert( valid() );
        }

        /** \brief Rotate the QST by delta mouse movement.
          *
          * This routine will rotate the QST by the amount specified in the
          * given delta mouse translation.
          *
          * \param xyMouse The mouse translation to apply.
          */

        e_forceinline void rotate( const Vector2& xyMouse ){
          toRotation().rotate( xyMouse );
          e_assert( valid() );
        }

      //}:                                        |
      //[position]:{                              |

        e_forceinline void setPosition( const self& X, const self& Y ){
          x() = X;
          y() = Y;
          e_assert( valid() );
        }

        e_forceinline void translate( const self& X, const self& Y ){
          x() += X;
          y() += Y;
          e_assert( valid() );
        }

        e_forceinline void translate( const Vector2& V ){
          x() += V.x;
          y() += V.y;
          e_assert( valid() );
        }

      //}:                                        |
      //[scale]:{                                 |

        e_forceinline void setScale( const self& sx, const self& sy ){
          m_vScale.x = sx;
          m_vScale.y = sy;
          e_assert( valid() );
        }

        e_forceinline void scale( const self& s ){
          m_vScale.x *= s;
          m_vScale.y *= s;
          e_assert( valid() );
        }

      //}:                                        |
      //setIdentity:{                             |

        e_forceinline void setIdentity(){
          m_vRotation.setIdentity();
          m_tPosition.zero();
          m_vScale.set( 1 );
        }

      //}:                                        |
      //asMatrix:{                                |

        Matrix3 get3x3()const;

      //}:                                        |
      //invert:{                                  |

        void invert();

      //}:                                        |
      //valid:{                                   |

        e_noinline bool valid()const{
          if( !m_tPosition.valid() ){
            return false;
          }
          if( !m_vRotation.valid() ){
            return false;
          }
          if( !m_vScale.valid() ){
            return false;
          }
          return true;
        }

      //}:                                        |
    //}:                                          |
    //--------------------------------------------|-----------------------------

    e_forceinline_always QST2( const self& tx, const self& ty, const self& rx, const self& ry, const self& sx, const self& sy )
      : m_tPosition( tx, ty )
      , m_vRotation( rx, ry )
      , m_vScale   ( sx, sy )
    {}

    e_forceinline_always QST2( const float tx, const float ty, const float rx, const float ry, const float sx, const float sy )
      : m_tPosition( tx, ty )
      , m_vRotation( rx, ry )
      , m_vScale   ( sx, sy )
    {}

    e_forceinline_always QST2( const std::initializer_list<T>& ilist ){
      e_assert( ilist.size() <= 6 );
      u32 i = 0;
      for( auto& v : ilist ){
        if( i < 2 ){
          m_tPosition[ i ]=v;
        }else if( i < 4 ){
          m_vRotation[ i-2 ]=v;
        }else{
          m_vScale[ i-4 ]=v;
        }
      }
    }

    e_forceinline_always QST2( const QST2& qst )
      : m_tPosition( qst.m_tPosition )
      , m_vRotation( qst.m_vRotation )
      , m_vScale   ( qst.m_vScale    )
    {}

    e_forceinline_always QST2()
      : m_vScale( 1 )
    {}

  private:

    e_var( Point2,  t, Position );
    e_var( Vector2, v, Rotation );
    e_var( Vector2, v, Scale    );
  };

//}:                                              |
//================================================|=============================
