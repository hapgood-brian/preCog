//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

//================================================+=============================
//Plane:{                                         |

  /** \brief The plane class.
    *
    * This structure defines the plane and everything you can do with one.
    */

  struct E_PUBLISH Plane final:Vector4{

    //--------------------------------------------+-----------------------------
    //Methods:{                                   |

      /** \brief Intersection test.
        *
        * This routine will perform an intersection test with a ray.
        */

      Vector3 intersects( const Ray3& r )const{
        const Vector3& V1   = Vector3( r.p );
        const Vector3& V2   = Vector3( r.p + r.d );
        const Vector3& diff = V1 - V2;
        const self&    den  = a()*diff.x + b()*diff.y + c()*diff.z;
        if( !den ){
          return( V1+V2 ) * 0.5f;
        }
        const self& u = (a()*V1.x + b()*V1.y + c()*V1.z + d() ) / den;
        return( V1 + u * ( V2 - V1 ));
      }

      /** \brief Get [signed] distance to point from plane.
        *
        * This routine returns the distance from the plane to the point P.
        *
        * \param P The point to measure.
        * \return Returns the distance to the point.
        */

      e_forceinline self signedDistance( const Point3& P )const{
        return
            a() * P.x +
            b() * P.y +
            c() * P.z +
            d();
      }

      /** \brief Get [signed] distance to point from plane.
        *
        * This routine returns the distance from the plane to the point P.
        *
        * \param P The point to measure.
        * \return Returns the distance to the point.
        */

      e_forceinline self distance( const Point3& P )const{
        return(
            a() * P.x +
            b() * P.y +
            c() * P.z +
            d()
        ).abs();
      }

      /** \brief Closest point on plane.
        *
        * This routine will return the closest point on a plane.
        */

      e_forceinline Point3 closest( const Point3& p )const{
        return( p - normal()*signedDistance( p ));
      }

      /** \brief Get normal of the plane.
        *
        * This routine will return the normal of the plane.
        *
        * \return Returns the normal of the plane.
        */

      e_forceinline Vector3 normal()const{
        return Vector4::xyz().normalized();
      }

      /** \brief Normalize the plane.
        *
        * This routine will normalize the plane.
        */

      e_forceinline void normalize(){
        const self& d = self( a()*a() + b()*b() + c()*c() ).sqrt();
        Vector4::x /= d;
        Vector4::y /= d;
        Vector4::z /= d;
        Vector4::w /= d;
      }

      /** \brief Normalize the plane.
        *
        * This routine will normalize the plane.
        */

      e_forceinline Plane normalized()const{
        Plane r( *this );
        r.normalize();
        return r;
      }

      /** \brief Set the plane from the given triangle.
        *
        * This routine will set the plane to the plane of the triangle t.
        *
        * \param t The triangle to load the plane from.
        */

      void set( const Triangle& t );

      /** \brief Set the plane from the given point and normal.
        *
        * This routine will construct a plane from a point and normal.
        */

      e_forceinline void set( const Point3& p, const Vector3& n ){
        const auto& normN = n.normalized();
        Vector4::x = normN.x;
        Vector4::y = normN.y;
        Vector4::z = normN.z;
        Vector4::w = -Vector3( p ).dot( normN );
      }

      /** \brief Get the A component.
        *
        * This routine will get the A component of the plane.
        *
        * \return Returns the A component of the plane.
        */

      e_forceinline const self& a()const{
        return Vector4::x;
      }

      /** \brief Get the A component.
        *
        * This routine will get the A component of the plane.
        *
        * \return Returns the A component of the plane.
        */

      e_forceinline self& a(){
        return Vector4::x;
      }

      /** \brief Get the B component.
        *
        * This routine will get the B component of the plane.
        *
        * \return Returns the B component of the plane.
        */

      e_forceinline const self& b()const{
        return Vector4::y;
      }

      /** \brief Get the B component.
        *
        * This routine will get the B component of the plane.
        *
        * \return Returns the B component of the plane.
        */

      e_forceinline self& b(){
        return Vector4::y;
      }

      /** \brief Get the C component.
        *
        * This routine will get the C component of the plane.
        *
        * \return Returns the C component of the plane.
        */

      e_forceinline const self& c()const{
        return Vector4::z;
      }

      /** \brief Get the C component.
        *
        * This routine will get the C component of the plane.
        *
        * \return Returns the C component of the plane.
        */

      e_forceinline self& c(){
        return Vector4::z;
      }

      /** \brief Get the D component.
        *
        * This routine will get the D component of the plane.
        *
        * \return Returns the D component of the plane.
        */

      e_forceinline const self& d()const{
        return Vector4::w;
      }

      /** \brief Get the D component.
        *
        * This routine will get the D component of the plane.
        *
        * \return Returns the D component of the plane.
        */

      e_forceinline self& d(){
        return Vector4::w;
      }

    //}:                                          |
    //--------------------------------------------+-----------------------------

    e_forceinline Plane( const Point3& p, const Vector3& n )
        : m_tCentroid( p ){
      set( p, n );
    }

    e_forceinline Plane( const Triangle& in_tTriangle )
        : m_tCentroid( in_tTriangle.center() ){
      set( in_tTriangle );
    }

    e_forceinline Plane( const Plane& p )
        : m_tCentroid( p.m_tCentroid ){
      Vector4::x = p.a();
      Vector4::y = p.b();
      Vector4::z = p.c();
      Vector4::w = p.d();
    }

    Plane() = default;

  private:

    e_var( Point3, t, Centroid );
  };

//}:                                              |
//================================================+=============================
