//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

//================================================+=============================
//Ray2:{                                          |

  struct E_PUBLISH Ray2 final{

    e_forceinline Ray2( const self& x, const self& y, const self& dx, const self& dy )
      : d( dx, dy )
      , p( x, y )
    {}

    constexpr Ray2( const float x, const float y, const float dx, const float dy )
      : d( dx, dy )
      , p( x, y )
    {}

    e_forceinline Ray2( const Point2& a, const Vector2& b )
      : d( b )
      , p( a )
    {}

    e_forceinline Ray2( const Ray2& r )
      : d( r.d )
      , p( r.p )
    {}

    Ray2() = default;

    Vector2 d;
    Point2  p;
  };

//}:                                              |
//================================================+=============================
