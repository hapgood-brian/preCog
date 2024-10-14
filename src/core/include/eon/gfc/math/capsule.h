//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

//================================================+=============================
//Capsule:{                                       |

  /** \brief Capsule class.
    *
    * This class defines the geometric capsulte.
    */

  struct E_PUBLISH Capsule final{

    //--------------------------------------------+-----------------------------
    //Methods:{                                   |
      //position:{                                |

        e_forceinline Point3 position()const{
          return m_tPos.xyz();
        }

      //}:                                        |
      //top:{                                     |

        e_forceinline Point3 top()const{
          return position() + axis();
        }

      //}:                                        |
      //length:{                                  |

        e_forceinline self length()const{
          return m_vDir.xyz().length();
        }

      //}:                                        |
      //radius:{                                  |

        e_forceinline self radius()const{
          return m_vDir.w;
        }

      //}:                                        |
      //height:{                                  |

        e_forceinline self height()const{
          return m_tPos.w;
        }

      //}:                                        |
      //axis:{                                    |

        e_forceinline Vector3 axis()const{
          return m_vDir.xyz();
        }

      //}:                                        |
    //}:                                          |
    //--------------------------------------------+-----------------------------

    Capsule( const Point3& start, const Point3& end, const self& radius )
      : m_vDir( e_normalize( Vector3( end - start )), radius )
      , m_tPos( start, Vector3( end-start ).length() )
    {}

    Capsule( const Ray3& ray, const self& radius )
      : m_vDir( e_normalize( ray.d ), radius )
      , m_tPos( ray.p, ray.d.length() )
    {}

    Capsule() = default;

  private:

    e_var( Vector4, v, Dir ) = Vector4( Vector3::kUp, 1/* radius */);
    e_var( Vector4, t, Pos ) = Vector4( 0, 0, 0, 1.f/* height */);
  };

//}:                                              |
//================================================+=============================
