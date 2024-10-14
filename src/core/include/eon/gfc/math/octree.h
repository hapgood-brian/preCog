//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

//================================================+=============================
//Octree:{                                        |

	/** \brief Octree class.
		*
		* The frustum is generally used for culling objects in a scene. There is
		* one attached to the sg camera.
		*/

	struct E_PUBLISH Octree final{

    //--------------------------------------------+-----------------------------
    //Structs:{                                   |

      struct E_PUBLISH Leaf final{

        //----------------------------------------+-----------------------------
        //Structs:{                               |

          /** \brief Status enum.
            *
            * This enum class defines the return status when adding polygons.
            */

          enum class Status:s32{
            kFound =-1,
            kFalse = 0,
            kTrue  = 1,
          };

          /** \brief Leaf polygon.
            *
            * This structure will override the Maths<>::Triangle class. It adds
            * U and V coordinates.
            */

          using Polygon = Triangle;

          /** \brief Intersection test result.
            *
            * This structure contains the triangle, normals, intersection
            * point and nearest vertex index.
            */

          struct Intersection final{
            Polygon poly;
            Vector3 norm;
            Point3 hit;
          };

        //}:                                      |
        //Aliases:{                               |

          using Result = std::shared_ptr<Intersection>;

        //}:                                      |
        //Methods:{                               |

          /** \brief Intersection test.
            *
            * This routine will test if the given ray will intersect with a
            * triangle.
            *
            * \param pOctree The octree this leaf is a part of.
            * \param L2W Local to world transform.
            * \param r Ray to intersection test with.
            *
            * \return Returns a triangle pointer if the ray intersected it
            * otherwise nullptr is returned.
            */

          Result intersects( const Octree* pOctree, const Matrix4& L2W, const Ray3& r )const;

          /** \brief Add leaf to octree.
            *
            * This routine will add a new leaf to the octree.
            *
            * \param poly The leaf to add to the tree.
            */

          Status addPolygon( Octree* pOctree, const u32 depth, const Polygon& poly );

          /** \brief Clear the leaf.
            *
            * This routine will clear the leaf, all children and polygons.
            */

          void clear();

        //}:                                      |
        //----------------------------------------+-----------------------------

        Leaf( const AABB3& bounds )
          : m_bBounds( bounds )
        {}

        Leaf() = default;
      ~ Leaf();

      private:

        e_var_ptr_array( Leaf, Children, 8 );
        e_var_vector( u32,     Polygons    );
        e_var_ptr( Leaf,       Parent      ) = nullptr;
        e_var( AABB3, b,       Bounds      );
      };

    //}:                                          |
    //Aliases:{                                   |

      using Polygon = typename Leaf::Polygon;
      using Result  = typename Leaf::Result;

    //}:                                          |
    //Methods:{                                   |

      /** \brief Intersection test.
        *
        * This routine will test if the given ray will intersect with a
        * triangle.
        *
        * \param L2W Local to world transform.
        * \param r Ray to intersection test with.
        *
        * \return Returns a triangle pointer if the ray intersected it
        * otherwise nullptr is returned.
        */

      Result intersects( const Matrix4& L2W, const Ray3& r )const;

      /** \brief Add triangle to octree.
        *
        * This routine will add a triangle to the octree. It will tunnel down
        * as far as it can go until there's no more space to hold the polygon.
        * The minimum cube is one cm cubed.
        *
        * \param poly Triangle to add.
        *
        * \return Returns true if the triangle was without the starting bounds
        * of the octree.
        */

      e_forceinline typename Leaf::Status addPolygon( const Polygon& poly ){
        if( m_pRoot ){
          return m_pRoot->addPolygon( this, 0, poly );
        }
        return{};
      }

    //}:                                          |
    //--------------------------------------------+-----------------------------

    Octree( const AABB3& bounds )
      : m_pRoot( std::make_shared<Leaf>( bounds ))
    {}

    Octree() = default;

	private:

    e_var_vector1( Polygon );
    e_var_shared_ptr( Leaf, Root );
	};

//}:                                              |
//================================================+=============================
