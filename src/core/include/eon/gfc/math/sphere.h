//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

//================================================+=============================
//Sphere:{                                        |

  struct E_PUBLISH Sphere final{

    //--------------------------------------------+-----------------------------
    //Operate:{                                   |

      e_forceinline Sphere operator*( const self& x ){
        Sphere s = *this;
        s.v.w *= x;
        return s;
      }

    //}:                                          |
    //Methods:{                                   |
      //toCenter:{                                |

        e_forceinline const Point3& toCenter()const{
          return reinterpret_cast<const Point3&>( v );
        }

        e_forceinline Point3& toCenter(){
          return reinterpret_cast<Point3&>( v );
        }

      //}:                                        |
      //toRadius:{                                |

        e_forceinline self toRadius()const{
          return v.w;
        }

        e_forceinline self& toRadius(){
          return v.w;
        }

      //}:                                        |
      //toBounds:{                                |

        AABB3 toBounds()const;

      //}:                                        |
      //Tests:{                                   |
        //intersects:{                            |

          bool intersects( const Ray3&, self& dist )const;
          bool intersects( const Sphere& )const;

        //}:                                      |
        //contains:{                              |

          bool contains( const Sphere& )const;

        //}:                                      |
      //}:                                        |
    //}:                                          |
    //--------------------------------------------+-----------------------------

    e_forceinline Sphere( const self& x, const self& y, const self& z, const self& radius )
      : v( x, y, z, radius )
    {}

    constexpr Sphere( const float x, const float y, const float z, const float radius )
      : v( x, y, z, radius )
    {}

    e_forceinline Sphere( const Point3& p, const self& radius )
      : v( p.x, p.y, p.z, radius )
    {}

    Sphere( const Sphere& s )
      : v( s.v )
    {}

    Sphere() = default;

  private:

    Vector4 v;
  };

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Circle:{                                        |

  struct Circle final{

    //--------------------------------------------+-----------------------------
    //Methods:{                                   |
      //toCenter:{                                |

        e_forceinline const Point2& toCenter()const{
          return *(Point2*)&v;
        }

        e_forceinline Point2& toCenter(){
          return *(Point2*)&v;
        }

      //}:                                        |
      //toRadius:{                                |

        e_forceinline T toRadius()const{
          return v.z;
        }

        e_forceinline T& toRadius(){
          return v.z;
        }

      //}:                                        |
      //toBounds:{                                |

        AABB2 toBounds()const;

      //}:                                        |
    //}:                                          |
    //--------------------------------------------+-----------------------------

    Circle( const self& x, const self& y, const self& radius )
      : v( x, y, radius )
    {}

    Circle( const Point2& p, const self& radius )
      : v( p.x, p.y, radius )
    {}

    Circle() = default;

  private:

    Vector3 v;
  };

//}:                                              |
//================================================+=============================
