//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#include"luacore.h"
#include"std.h"

using namespace EON;
using namespace gfc;

//================================================+=============================
//Structs:{                                       |
  //LuaCallback:{                                 |

    class LuaCallback{
      e_var( s64, ref, Class ) = 0;
      e_var_string( Name );
    };

  //}:                                            |
//}:                                              |
//Private:{                                       |
  //setLuaProperty:{                              |

#ifdef __APPLE__
  #pragma mark - Private -
#endif

    namespace{
      bool setLuaProperty( lua_State* L, const Object::prop_ptr& pProperty ){
        const_cast<Object::prop_ptr&>( pProperty )->tick();
        const u64 classid = pProperty->toType();

        //----------------------------------------------------------------------
        // Write string types.
        //----------------------------------------------------------------------

        bool bResult=( e_classid<string>() == classid );
        if( bResult ){
          if( lua_isstring( L, -1 )){
            pProperty->setValue( string( lua_tostring( L, -1 )));
          }
          return false;
        }

        //----------------------------------------------------------------------
        // Write real types.
        //----------------------------------------------------------------------

        /* 64 bit */

        bResult=(( e_classid<ssi_mass<f64>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi_mass<f64>>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<ssi<f64>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi<f64>>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<f64>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<f64>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<double>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<double>( lua_tonumber( L, -1 ));
          }
          return false;
        }

        /* 32 bit */

        bResult=(( e_classid<ssi_mass<f32>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi_mass<f32>>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<ssi<f32>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi<f32>>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<f32>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<f32>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<float>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<float>( lua_tonumber( L, -1 ));
          }
          return false;
        }

        /* 16 bit */

        bResult=(( e_classid<ssi_mass<f16>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi_mass<f16>>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<ssi<f16>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi<f16>>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<f16>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<f16>( lua_tonumber( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<half>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<half>( lua_tonumber( L, -1 ));
          }
          return false;
        }

        //----------------------------------------------------------------------
        // Write integer types.
        //----------------------------------------------------------------------

        /* 64 bit */

        bResult=(( e_classid<i64>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<i64>( lua_tointeger( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<s64>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<s64>( lua_tointeger( L, -1 ));
          }
          return false;
        }
        bResult=(( e_classid<u64>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<u64>( lua_tointeger( L, -1 ));
          }
          return false;
        }

        /* 32 bit */

        bResult=(( e_classid<ssi_mass<i32>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi_mass<i32>>( i32::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<ssi<i32>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi<i32>>( i32::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<i32>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<i32>( i32::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<s32>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<s32>( s32( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<u32>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<u32>( u32( lua_tointeger( L, -1 )));
          }
          return false;
        }

        /* 16 bit */

        bResult=(( e_classid<ssi_mass<i16>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi_mass<i16>>( i16::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<ssi<i16>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi<i16>>( i16::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<i16>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<i16>( i16::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<s16>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<s16>( s16( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<u16>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<u16>( u16( lua_tointeger( L, -1 )));
          }
          return false;
        }

        /* 8 bit */

        bResult=(( e_classid<ssi_mass<i8>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi_mass<i8>>( i8::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<ssi<i8>>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<ssi<i8>>( i8::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<i8>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<i8>( i8::cast( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<s8>() == classid ));
        if(( e_classid<s8>() == classid )){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<s8>( s8( lua_tointeger( L, -1 )));
          }
          return false;
        }
        bResult=(( e_classid<u8>() == classid ));
        if( bResult ){
          if( lua_isnumber( L, -1 )){
            pProperty->setValue<u8>( u8( lua_tointeger( L, -1 )));
          }
          return false;
        }

        //----------------------------------------------------------------------
        // Write boolean types.
        //----------------------------------------------------------------------

        bResult=( e_classid<bool>() == classid );
        if( bResult ){
          if( lua_isboolean( L, -1 )){
            pProperty->setValue<bool>( lua_toboolean( L, -1 ));
          }
          return false;
        }

        //----------------------------------------------------------------------
        // Write color types.
        //----------------------------------------------------------------------

        bResult=( e_classid<rgba>() == classid );
        if( bResult ){
          rgba c;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "r" );
          e_assert( lua_isnumber( L, -1 ));
          c.r = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "g" );
          e_assert( lua_isnumber( L, -1 ));
          c.g = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "b" );
          e_assert( lua_isnumber( L, -1 ));
          c.b = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "a" );
          e_assert( lua_isnumber( L, -1 ));
          c.a = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( c );
          return false;
        }

        //----------------------------------------------------------------------
        // Write quaternion types.
        //----------------------------------------------------------------------

        bResult=( e_classid<quat>() == classid );
        if( bResult ){
          e_assert( lua_istable(      L, -1 ));

          lua_getfield(               L, -1, "x" );
          e_assert( lua_isnumber(     L, -1 ));
          const f32 x = lua_tonumber( L, -1 );
          lua_pop(                    L,  1 );

          lua_getfield(               L, -1, "y" );
          e_assert( lua_isnumber(     L, -1 ));
          const f32 y = lua_tonumber( L, -1 );
          lua_pop(                    L,  1 );

          lua_getfield(               L, -1, "z" );
          e_assert( lua_isnumber(     L, -1 ));
          const f32 z = lua_tonumber( L, -1 );
          lua_pop(                    L,  1 );

          pProperty->setValue( quat( x, y, z ));
          return false;
        }

        //----------------------------------------------------------------------
        // Write vector types.
        //----------------------------------------------------------------------

        /* 128-bit */

        bResult=( e_classid<ssi_mass<vec4>>() == classid );
        if( bResult ){
          ssi_mass<vec4> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "w" );
          e_assert( lua_isnumber( L, -1 ));
          v.w = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<ssi<vec4>>() == classid );
        if( bResult ){
          ssi<vec4> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "w" );
          e_assert( lua_isnumber( L, -1 ));
          v.w = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<vec4>() == classid );
        if( bResult ){
          vec4 v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "w" );
          e_assert( lua_isnumber( L, -1 ));
          v.w = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }

        /* 96-bit */

        bResult=( e_classid<ssi<pt3>>() == classid );
        if( bResult ){
          ssi<pt3> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<pt3>() == classid );
        if( bResult ){
          pt3 v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<ssi_mass<vec3>>() == classid );
        if( bResult ){
          ssi_mass<vec3> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<ssi<vec3>>() == classid );
        if( bResult ){
          ssi<vec3> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<vec3>() == classid );
        if( bResult ){
          vec3 v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "z" );
          e_assert( lua_isnumber( L, -1 ));
          v.z = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }

        /* 64-bit */

        bResult=( e_classid<ssi_mass<vec2>>() == classid );
        if( bResult ){
          ssi_mass<vec2> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<ssi<vec2>>() == classid );
        if( bResult ){
          ssi<vec2> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<vec2>() == classid );
        if( bResult ){
          vec2 v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<ssi<pt2>>() == classid );
        if( bResult ){
          ssi<pt2> v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        bResult=( e_classid<pt2>() == classid );
        if( bResult ){
          pt2 v;
          e_assert( lua_istable(  L, -1 ));

          lua_getfield(           L, -1, "x" );
          e_assert( lua_isnumber( L, -1 ));
          v.x = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          lua_getfield(           L, -1, "y" );
          e_assert( lua_isnumber( L, -1 ));
          v.y = lua_tonumber(     L, -1 );
          lua_pop(                L,  1 );

          pProperty->setValue( v );
          return false;
        }
        return true;
      }
    }

  //}:                                            |
  //getLuaProperty:{                              |

    namespace{
      s32 getLuaProperty( lua_State* L, const Object::prop_ptr& pProperty ){
        const_cast<Object::prop_ptr&>( pProperty )->tick();
        const u64 classid = pProperty->toType();
        s32 iResult = 0;

        //----------------------------------------------------------------------
        // Write string types.
        //----------------------------------------------------------------------

        if( e_classid<string>() == classid ){
          pProperty->queryAs<string>( [&]( const string& value ){
            lua_pushstring( L, value.c_str() );
            iResult = 1;
          });
          return iResult;
        }

        //----------------------------------------------------------------------
        // Write real types.
        //----------------------------------------------------------------------

        /* 64 bit */

        if( e_classid<ssi_mass<f64>>() == classid ){
          pProperty->queryAs<ssi_mass<f64>>( [&]( const ssi_mass<f64>& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<f64>>() == classid ){
          pProperty->queryAs<ssi<f64>>( [&]( const ssi<f64>& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<f64>() == classid ){
          pProperty->queryAs<f64>( [&]( const f64& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<double>() == classid ){
          pProperty->queryAs<double>( [&]( const double& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }

        /* 32 bit */

        if( e_classid<ssi_mass<f32>>() == classid ){
          pProperty->queryAs<ssi_mass<f32>>( [&]( const ssi_mass<f32>& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<f32>>() == classid ){
          pProperty->queryAs<ssi<f32>>( [&]( const ssi<f32>& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<f32>() == classid ){
          pProperty->queryAs<f32>( [&]( const f32& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<float>() == classid ){
          pProperty->queryAs<float>( [&]( const float& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }

        /* 16 bit */

        if( e_classid<ssi_mass<f16>>() == classid ){
          pProperty->queryAs<ssi_mass<f16>>( [&]( const ssi_mass<f16>& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<f16>>() == classid ){
          pProperty->queryAs<ssi<f16>>( [&]( const ssi<f16>& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<f16>() == classid ){
          pProperty->queryAs<f16>( [&]( const f16& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<double>() == classid ){
          pProperty->queryAs<double>( [&]( const double& value ){
            lua_pushnumber( L, f32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }

        //----------------------------------------------------------------------
        // Write integer types.
        //----------------------------------------------------------------------

        /* 64 bit */

        if( e_classid<ssi_mass<i64>>() == classid ){
          pProperty->queryAs<ssi_mass<i64>>( [&]( const ssi_mass<i64>& value ){
            lua_pushinteger( L, i64::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<i64>>() == classid ){
          pProperty->queryAs<ssi<i64>>( [&]( const ssi<i64>& value ){
            lua_pushinteger( L, i64::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<i64>() == classid ){
          pProperty->queryAs<i64>( [&]( const i64& value ){
            lua_pushinteger( L, i64::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<s64>() == classid ){
          pProperty->queryAs<s64>( [&]( const s64& value ){
            lua_pushinteger( L, s64( value ));
            iResult = 1;
          });
          return iResult;
        }

        /* 32 bit */

        if( e_classid<ssi_mass<i32>>() == classid ){
          pProperty->queryAs<ssi_mass<i32>>( [&]( const ssi_mass<i32>& value ){
            lua_pushinteger( L, i32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<i32>>() == classid ){
          pProperty->queryAs<ssi<i32>>( [&]( const ssi<i32>& value ){
            lua_pushinteger( L, i32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<i32>() == classid ){
          pProperty->queryAs<i32>( [&]( const i32& value ){
            lua_pushinteger( L, i32::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<s32>() == classid ){
          pProperty->queryAs<s32>( [&]( const s32& value ){
            lua_pushinteger( L, s32( value ));
            iResult = 1;
          });
          return iResult;
        }

        /* 16 bit */

        if( e_classid<ssi_mass<i16>>() == classid ){
          pProperty->queryAs<ssi_mass<i16>>( [&]( const ssi_mass<i16>& value ){
            lua_pushinteger( L, i16::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<i16>>() == classid ){
          pProperty->queryAs<ssi<i16>>( [&]( const ssi<i16>& value ){
            lua_pushinteger( L, i16::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<i16>() == classid ){
          pProperty->queryAs<i16>( [&]( const i16& value ){
            lua_pushinteger( L, i16::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<s16>() == classid ){
          pProperty->queryAs<s16>( [&]( const s16& value ){
            lua_pushinteger( L, i16::cast( value ));
            iResult = 1;
          });
          return iResult;
        }

        /* 8 bit */

        if( e_classid<ssi_mass<i8>>() == classid ){
          pProperty->queryAs<ssi_mass<i8>>( [&]( const ssi_mass<i8>& value ){
            lua_pushinteger( L, i8::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<i8>>() == classid ){
          pProperty->queryAs<ssi<i8>>( [&]( const ssi<i8>& value ){
            lua_pushinteger( L, i8::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<i8>() == classid ){
          pProperty->queryAs<i8>( [&]( const i8& value ){
            lua_pushinteger( L, i8::cast( value ));
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<s8>() == classid ){
          pProperty->queryAs<s8>( [&]( const s8& value ){
            lua_pushinteger( L, i8::cast( value ));
            iResult = 1;
          });
          return iResult;
        }

        //----------------------------------------------------------------------
        // Write boolean types.
        //----------------------------------------------------------------------

        if( e_classid<bool>() == classid ){
          pProperty->queryAs<bool>( [&]( const bool& value ){
            lua_pushboolean( L, value );
            iResult = 1;
          });
          return iResult;
        }

        //----------------------------------------------------------------------
        // Write color types.
        //----------------------------------------------------------------------

        if( e_classid<rgba>() == classid ){
          pProperty->queryAs<rgba>( [&]( const rgba& value ){
            lua_createtable( L, 5, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.r ));
            lua_setfield(    L, -2, "r" );
            lua_pushnumber(  L, f32::cast( value.g ));
            lua_setfield(    L, -2, "g" );
            lua_pushnumber(  L, f32::cast( value.b ));
            lua_setfield(    L, -2, "b" );
            lua_pushnumber(  L, f32::cast( value.a ));
            lua_setfield(    L, -2, "a" );
            iResult = 1;
          });
          return iResult;
        }

        //----------------------------------------------------------------------
        // Write quaternion types.
        //----------------------------------------------------------------------

        if( e_classid<quat>() == classid ){
          pProperty->queryAs<quat>( [&]( const quat& value ){
            f32 x, y, z;
            value.getEuler( x, y, z );
            lua_createtable( L, 5, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( z ));
            lua_setfield(    L, -2, "z" );
            iResult = 1;
          });
          return iResult;
        }

        //----------------------------------------------------------------------
        // Write vector types.
        //----------------------------------------------------------------------

        /* 128-bit */

        if( e_classid<ssi_mass<vec4>>() == classid ){
          pProperty->queryAs<ssi_mass<vec4>>( [&]( const ssi_mass<vec4>& value ){
            lua_createtable( L, 5, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            lua_pushnumber(  L, f32::cast( value.w ));
            lua_setfield(    L, -2, "w" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<vec4>>() == classid ){
          pProperty->queryAs<ssi<vec4>>( [&]( const ssi<vec4>& value ){
            lua_createtable( L, 5, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            lua_pushnumber(  L, f32::cast( value.w ));
            lua_setfield(    L, -2, "w" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<vec4>() == classid ){
          pProperty->queryAs<vec4>( [&]( const vec4& value ){
            lua_createtable( L, 5, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            lua_pushnumber(  L, f32::cast( value.w ));
            lua_setfield(    L, -2, "w" );
            iResult = 1;
          });
          return iResult;
        }

        /* 96-bit */

        if( e_classid<ssi_mass<vec3>>() == classid ){
          pProperty->queryAs<ssi_mass<vec3>>( [&]( const ssi_mass<vec3>& value ){
            lua_createtable( L, 4, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<vec3>>() == classid ){
          pProperty->queryAs<ssi<vec3>>( [&]( const ssi<vec3>& value ){
            lua_createtable( L, 4, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<vec3>() == classid ){
          pProperty->queryAs<vec3>( [&]( const vec3& value ){
            lua_createtable( L, 4, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<pt3>>() == classid ){
          pProperty->queryAs<ssi<pt3>>( [&]( const ssi<pt3>& value ){
            lua_createtable( L, 4, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<pt3>() == classid ){
          pProperty->queryAs<pt3>( [&]( const pt3& value ){
            lua_createtable( L, 4, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            lua_pushnumber(  L, f32::cast( value.z ));
            lua_setfield(    L, -2, "z" );
            iResult = 1;
          });
          return iResult;
        }

        /* 64-bit */

        if( e_classid<ssi_mass<vec2>>() == classid ){
          pProperty->queryAs<ssi_mass<vec2>>( [&]( const ssi_mass<vec2>& value ){
            lua_createtable( L, 3, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<vec2>>() == classid ){
          pProperty->queryAs<ssi<vec2>>( [&]( const ssi<vec2>& value ){
            lua_createtable( L, 3, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<vec2>() == classid ){
          pProperty->queryAs<vec2>( [&]( const vec2& value ){
            lua_createtable( L, 3, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<ssi<pt2>>() == classid ){
          pProperty->queryAs<ssi<pt2>>( [&]( const ssi<pt2>& value ){
            lua_createtable( L, 3, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            iResult = 1;
          });
          return iResult;
        }
        if( e_classid<pt2>() == classid ){
          pProperty->queryAs<pt2>( [&]( const pt2& value ){
            lua_createtable( L, 3, 0 );
            lua_pushinteger( L, classid );
            lua_setfield(    L, -2, "__class" );
            lua_pushnumber(  L, f32::cast( value.x ));
            lua_setfield(    L, -2, "x" );
            lua_pushnumber(  L, f32::cast( value.y ));
            lua_setfield(    L, -2, "y" );
            iResult = 1;
          });
          return iResult;
        }
        return 0;
      }
    }

  //}:                                            |
  //setRecProperty:{                              |

    namespace{

      bool setRecProperty( lua_State* L, const string& inKey, const Object::prop_map& inProperties ){

        //----------------------------------------------------------------------
        // Isolate key for this pass. Can only continue when pDot is nullptr.
        //----------------------------------------------------------------------

        string key;
        ccp pDot = inKey.chr( '.' );
        if( pDot ){
          bool bResult = false;
          key = string( inKey.c_str(), pDot );
          inProperties.foreachs( [&]( const Object::prop_ptr& pProperty ){
            if( pProperty->isName( key )){
              pProperty->tick();
              if( setRecProperty( L, pDot+1, pProperty->toChildren() )){
                bResult = true;
              }
              return false;
            }
            return true;
          });
          return bResult;
        }
        key = inKey;

        //----------------------------------------------------------------------
        // Locate key in property list.
        //----------------------------------------------------------------------

        bool bResult = false;
        inProperties.foreachs( [&]( const Object::prop_ptr& pProperty ){
          if( pProperty->isName( key )){
            setLuaProperty( L, pProperty );
            return false;
          }
          return true;
        });
        return bResult;
      }
    }

  //}:                                            |
  //getRecProperty:{                              |

    namespace{

      s32 getRecProperty( lua_State* L, const string& inKey, const Object::prop_map& inProperties ){

        //----------------------------------------------------------------------
        // Isolate key for this pass. Can only continue when pDot is nullptr.
        //----------------------------------------------------------------------

        string key;
        ccp pDot = inKey.chr( '.' );
        if( pDot ){
          s32 iResult = 0;
          key = string( inKey.c_str(), pDot );
          inProperties.foreachs( [&]( const Object::prop_ptr& pProperty ){
            if( pProperty->isName( key )){
              if( getRecProperty( L, pDot+1, pProperty->toChildren() )){
                iResult = 1;
              }
              return false;
            }
            return true;
          });
          if( iResult ){
            return 1;
          }
        }
        key = inKey;

        //----------------------------------------------------------------------
        // Locate key in property list.
        //----------------------------------------------------------------------

        s32 iResult = 0;
        inProperties.foreachs( [&]( const Object::prop_ptr& pProperty ){
          if( pProperty->isName( key )){
            if( getLuaProperty( L, pProperty )){
              iResult = 1;
            }
            return false;
          }
          return true;
        });
        return iResult;
      }
    }

  //}:                                            |
  //calcVectors:{                                 |

    namespace{

      s32 calcVectors( lua_State* L, const std::function<s32( const vec3& v1, const vec3& v2 )>& lambda ){

        //--------------------------------------------------------------------
        // Validate inputs and bail conditions.
        //--------------------------------------------------------------------

        e_assert( lua_istable( L, -2 ));
        if( !lua_istable( L, -2 )){
          lua_pushnil( L );
          return 1;
        }

        //--------------------------------------------------------------------
        // Grab vector from 1st table.
        //--------------------------------------------------------------------

        lua_getfield(  L, -2, "x" );//+1
        const f32& x0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "y" );//+1
        const f32& y0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "z" );//+1
        const f32& z0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1

        //--------------------------------------------------------------------
        // Handle mod of scalar first.
        //--------------------------------------------------------------------

        f32 x1, y1, z1;
        if( lua_isnumber( L, -1 )){
          x1 = y1 = z1 = lua_tonumber( L, -1 );
        }else if( lua_istable( L, -1 )){
          lua_getfield(  L, -1, "x" );//+1
          x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "y" );//+1
          y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "z" );//+1
          z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
        }

        //--------------------------------------------------------------------
        // Create table and return summed up vectors in a table.
        //--------------------------------------------------------------------

        return lambda( vec3( x0, y0, z0 ), vec3( x1, y1, z1 ));
      }
    }

  //}:
  //calcPoints:{                                  |

    namespace{

      s32 calcPoints( lua_State* L, const std::function<s32( const pt3& v1, const pt3& v2 )>& lambda ){

        //--------------------------------------------------------------------
        // Validate inputs and bail conditions.
        //--------------------------------------------------------------------

        e_assert( lua_istable( L, -2 ));
        if( !lua_istable( L, -2 )){
          lua_pushnil( L );
          return 1;
        }

        //--------------------------------------------------------------------
        // Grab vector from 1st table.
        //--------------------------------------------------------------------

        lua_getfield(  L, -2, "x" );//+1
        const f32& x0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "y" );//+1
        const f32& y0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "z" );//+1
        const f32& z0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1

        //--------------------------------------------------------------------
        // Handle mod of scalar first.
        //--------------------------------------------------------------------

        f32 x1, y1, z1;
        if( lua_isnumber( L, -1 )){
          x1 = y1 = z1 = lua_tonumber( L, -1 );
        }else if( lua_istable( L, -1 )){
          lua_getfield(  L, -1, "x" );//+1
          x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "y" );//+1
          y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "z" );//+1
          z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
        }

        //--------------------------------------------------------------------
        // Create table and return summed up vectors in a table.
        //--------------------------------------------------------------------

        return lambda( pt3( x0, y0, z0 ), pt3( x1, y1, z1 ));
      }
    }

  //}:
  //calcColors:{                                  |

    namespace{

      s32 calcColors( lua_State* L, const std::function<s32( const rgba& v1, const rgba& v2 )>& lambda ){

        //--------------------------------------------------------------------
        // Validate inputs and bail conditions.
        //--------------------------------------------------------------------

        e_assert( lua_istable( L, -2 ));
        if( !lua_istable( L, -2 )){
          lua_pushnil( L );
          return 1;
        }

        //--------------------------------------------------------------------
        // Grab vector from 1st table.
        //--------------------------------------------------------------------

        lua_getfield(  L, -2, "r" );//+1
        const f32& x0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "g" );//+1
        const f32& y0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "b" );//+1
        const f32& z0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1
        lua_getfield(  L, -2, "a" );//+1
        const f32& w0 = lua_tonumber( L, -1 );
        lua_pop( L, 1 );//-1

        //--------------------------------------------------------------------
        // Handle mod of scalar first.
        //--------------------------------------------------------------------

        f32 x1, y1, z1, w1;
        if( lua_isnumber( L, -1 )){
          x1 = y1 = z1 = w1 = lua_tonumber( L, -1 );
        }else if( lua_istable( L, -1 )){
          lua_getfield(  L, -1, "r" );//+1
          x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "g" );//+1
          y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "b" );//+1
          z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "a" );//+1
          w1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
        }

        //--------------------------------------------------------------------
        // Create table and return summed up vectors in a table.
        //--------------------------------------------------------------------

        return lambda( rgba( x0, y0, z0, w0 ), rgba( x1, y1, z1, w1 ));
      }
    }

  //}:
  //getVector:{                                   |

    namespace{
      vec3 getVector( lua_State* L, const s32 index=-1 ){
        vec3 r;
        if( lua_istable( L, index )){

          // Get members of the vector.
          lua_getfield(  L, index, "x" );
          const f32& x = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          r.x = x;

          lua_getfield(  L, index, "y" );
          const f32& y = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          r.y = y;

          lua_getfield(  L, index, "z" );
          const f32& z = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          r.z = z;
        }
        return r;
      }
    }

  //}:                                            |
  //getPoint:{                                    |

    namespace{
      pt3 getPoint( lua_State* L, const s32 index=-1 ){
        pt3 r;
        if( lua_istable( L, index )){

          // Get members of the vector.
          lua_getfield(  L, index, "x" );
          const f32& x = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          r.x = x;

          lua_getfield(  L, index, "y" );
          const f32& y = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          r.y = y;

          lua_getfield(  L, index, "z" );
          const f32& z = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          r.z = z;
        }
        return r;
      }
    }

  //}:                                            |
  //getColor:{                                    |

    namespace{
      rgba getColor( lua_State* L, const s32 index=-1 ){
        rgba R;
        if( lua_istable( L, index )){

          // Get members of the vector.
          lua_getfield(  L, index, "r" );
          const f32& r = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          R.r = r;

          lua_getfield(  L, index, "g" );
          const f32& g = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          R.g = g;

          lua_getfield(  L, index, "b" );
          const f32& b = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          R.b = b;

          lua_getfield(  L, index, "a" );
          const f32& a = lua_tonumber( L, -1 );
          lua_pop( L, 1 );
          R.a = a;
        }
        return R;
      }
    }

  //}:                                            |
//}:                                              |
//Methods:{                                       |
  //[types]:{                                     |
    //lua_createUUID:{                            |

      template<typename T> s32 lua_createUUID( lua_State* L, const typename T::handle& hT ){

        //----------------------------------------------------------------------
        // Texture2D methods.
        //----------------------------------------------------------------------

        static const luaL_Reg methods[]={

          //--------------------------------------------------------------------
          // textureVariable:addref()
          //--------------------------------------------------------------------

          {"addref", []( lua_State* L )->s32{
            if( lua_istable( L, -1 )){
              lua_getfield( L, -1, "UUID" );
              Object::handle hObject( lua_tointeger( L, -1 ));
              if( hObject && hObject.isa<T>() ){
                hObject->addref();
              }
            }
            return 0;
          }},

          //--------------------------------------------------------------------
          // textureVariable:addref()
          //--------------------------------------------------------------------

          {"subref", []( lua_State* L )->s32{
            if( lua_istable( L, -1 )){
              lua_getfield( L, -1, "UUID" );
              Object::handle hObject( lua_tointeger( L, -1 ));
              if( hObject && hObject.isa<T>() ){
                const s64 sRefs = hObject->subref();
                if( !sRefs ){
                  Class::Factory::erase( hObject );
                }
                lua_pushinteger( L, sRefs );
                return 1;
              }
            }
            lua_pushnil( L );
            return 1;
          }},
        };

        //----------------------------------------------------------------------
        // Create table to wrap texture UUID and load up methods into it.
        //----------------------------------------------------------------------

        if( hT ){
          const T& t = hT.cast();
          lua_createtable( L, 2, 0 );
          luaL_setfuncs( L, methods, 0 );
          lua_call( L, 1, 0 );
          lua_pushinteger( L, e_classid<T>() );
          lua_setfield( L, -2, "__class" );
          lua_pushinteger( L, t.UUID );
          lua_setfield( L, -2, "UUID" );
          t.addref();
        }else{
          lua_pushnil( L );
        }
        return 1;
      }

    //}:                                          |
    //lua_createAABB2:{                           |

      s32 lua_createAABB2( lua_State* L, const f32& x, const f32& y, const f32& z, const f32& w ){
        // TODO: Create meta-table and fill with operators.
        // TODO: Create functions for maths.
        lua_createtable( L, 4, 0 );
        //luaL_setfuncs( L, methods, 0 );
        //lua_call( L, 1, 0 );
        lua_pushinteger( L, e_classid<aabb2>() );//+1
        lua_setfield(    L, -2, "__class" );//-1
        lua_pushnumber(  L, f32::cast( x ));//+1
        lua_setfield(    L, -2, "minx" );//-1
        lua_pushnumber(  L, f32::cast( y ));//+1
        lua_setfield(    L, -2, "miny" );//-1
        lua_pushnumber(  L, f32::cast( z ));//+1
        lua_setfield(    L, -2, "maxx" );//-1
        lua_pushnumber(  L, f32::cast( w ));//+1
        lua_setfield(    L, -2, "miny" );//-1
        return 1;
      }

    //}:                                          |
    //lua_createColor:{                           |

      s32 lua_createColor( lua_State* L, const f32& r, const f32& g, const f32& b, const f32& a ){
        // TODO: Create meta-table and fill with operators.
        // TODO: Create functions for maths.
        lua_createtable( L, 5, 0 );
        lua_pushinteger( L, e_classid<rgba>() );//+1
        lua_setfield(    L, -2, "__class" );//-1
        lua_pushnumber(  L, f32::cast( r ));//+1
        lua_setfield(    L, -2, "r" );//-1
        lua_pushnumber(  L, f32::cast( g ));//+1
        lua_setfield(    L, -2, "g" );//-1
        lua_pushnumber(  L, f32::cast( b ));//+1
        lua_setfield(    L, -2, "b" );//-1
        lua_pushnumber(  L, f32::cast( a ));//+1
        lua_setfield(    L, -2, "a" );//-1
        return 1;
      }

    //}:                                          |
    //lua_createQuat:{                            |

      s32 lua_createQuat( lua_State* L, const f32& x, const f32& y, const f32& z ){
        // TODO: Create meta-table and fill with operators.
        // TODO: Create functions for maths.
        lua_createtable( L, 4, 0 );
        //luaL_setfuncs( L, methods, 0 );
        //lua_call( L, 1, 0 );
        lua_pushinteger( L, e_classid<quat>() );//+1
        lua_setfield(    L, -2, "__class" );//-1
        lua_pushnumber(  L, f32::cast( x ));//+1
        lua_setfield(    L, -2, "x" );//-1
        lua_pushnumber(  L, f32::cast( y ));//+1
        lua_setfield(    L, -2, "y" );//-1
        lua_pushnumber(  L, f32::cast( z ));//+1
        lua_setfield(    L, -2, "z" );//-1
        return 1;
      }

    //}:                                          |
    //lua_createVec3:{                            |

      s32 lua_createVec3( lua_State* L, const f32& x, const f32& y, const f32& z ){
        // TODO: Create meta-table and fill with operators.
        // TODO: Create functions for maths.
        lua_createtable( L, 4, 0 );
        //luaL_setfuncs( L, methods, 0 );
        //lua_call( L, 1, 0 );
        lua_pushinteger( L, e_classid<vec3>() );//+1
        lua_setfield(    L, -2, "__class" );//-1
        lua_pushnumber(  L, f32::cast( x ));//+1
        lua_setfield(    L, -2, "x" );//-1
        lua_pushnumber(  L, f32::cast( y ));//+1
        lua_setfield(    L, -2, "y" );//-1
        lua_pushnumber(  L, f32::cast( z ));//+1
        lua_setfield(    L, -2, "z" );//-1
        return 1;
      }

    //}:                                          |
    //lua_createPt3:{                             |

      s32 lua_createPt3( lua_State* L, const f32& x, const f32& y, const f32& z ){
        // TODO: Create meta-table and fill with operators.
        // TODO: Create functions for maths.
        lua_createtable( L, 4, 0 );
        lua_pushinteger( L, e_classid<pt3>() );//+1
        lua_setfield(    L, -2, "__class" );//-1
        lua_pushnumber(  L, f32::cast( x ));//+1
        lua_setfield(    L, -2, "x" );//-1
        lua_pushnumber(  L, f32::cast( y ));//+1
        lua_setfield(    L, -2, "y" );//-1
        lua_pushnumber(  L, f32::cast( z ));//+1
        lua_setfield(    L, -2, "z" );//-1
        return 1;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//Actions:{                                       |
  //[scalar]:{                                    |
    //onSaturate:{                                |

#ifdef __APPLE__
  #pragma mark - Actions: Maths -
  #pragma mark Scalar maths
#endif

      namespace{
        s32 onSaturate( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.saturated() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onRad:{                                     |

      namespace{
        s32 onRad( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.rad() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onDeg:{                                     |

      namespace{
        s32 onDeg( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.deg() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onFloor:{                                   |

      namespace{
        s32 onFloor( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.floor() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onCeil:{                                    |

      namespace{
        s32 onCeil( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.ceil() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onRound:{                                   |

      namespace{
        s32 onRound( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.round() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onRsqrt:{                                   |

      namespace{
        s32 onRsqrt( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.rsqrt() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onSqrt:{                                    |

      namespace{
        s32 onSqrt( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            f32 x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.sqrt() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onAbs:{                                     |

      namespace{
        s32 onAbs( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.abs() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onExp:{                                     |

      namespace{
        s32 onExp( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.exp() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onSquared:{                                 |

      namespace{
        s32 onSquared( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.squared() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onCubed:{                                   |

      namespace{
        s32 onCubed( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.cubed() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onAtan2:{                                   |

      namespace{
        s32 onAtan2( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.atan( y )));
          return 1;
        }
      }

    //}:                                          |
    //onAtan:{                                    |

      namespace{
        s32 onAtan( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.atan() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onTan:{                                     |

      namespace{
        s32 onTan( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.tan() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onAcos:{                                    |

      namespace{
        s32 onAcos( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.acos() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onCos:{                                     |

      namespace{
        s32 onCos( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.cos() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onAsin:{                                    |

      namespace{
        s32 onAsin( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.asin() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onSin:{                                     |

      namespace{
        s32 onSin( lua_State* L ){
          if( lua_isnumber( L, -1 )){
            const f32& x = lua_tonumber( L, -1 );
            lua_pushnumber( L, f32::cast( x.sin() ));
          }else{
            lua_pushnil( L );
          }
          return 1;
        }
      }

    //}:                                          |
    //onClamp:{                                   |

      namespace{
        s32 onClamp( lua_State* L ){
          const f32& x = lua_tonumber( L, -3 );
          const f32& y = lua_tonumber( L, -2 );
          const f32& z = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.clamped( y, z )));
          return 1;
        }
      }

    //}:                                          |
    //onLerp:{                                    |

      namespace{
        s32 onLerp( lua_State* L ){
          const f32& x = lua_tonumber( L, -3 );
          const f32& y = lua_tonumber( L, -2 );
          const f32& z = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.lerp( y, z )));
          return 1;
        }
      }

    //}:                                          |
    //onCosLerp:{                                 |

      namespace{
        s32 onCosLerp( lua_State* L ){
          const f32& x = lua_tonumber( L, -3 );
          const f32& y = lua_tonumber( L, -2 );
          const f32& z = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.cosLerp( y, z )));
          return 1;
        }
      }

    //}:                                          |
    //onMax:{                                     |

      namespace{
        s32 onMax( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.max( y )));
          return 1;
        }
      }

    //}:                                          |
    //onMin:{                                     |

      namespace{
        s32 onMin( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.min( y )));
          return 1;
        }
      }

    //}:                                          |
    //onPow:{                                     |

      namespace{
        s32 onPow( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.pow( y )));
          return 1;
        }
      }

    //}:                                          |
    //onMod:{                                     |

      namespace{
        s32 onMod( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x.mod( y )));
          return 1;
        }
      }

    //}:                                          |
    //onMul:{                                     |

      namespace{
        s32 onMul( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x * y ));
          return 1;
        }
      }

    //}:                                          |
    //onDiv:{                                     |

      namespace{
        s32 onDiv( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x / y ));
          return 1;
        }
      }

    //}:                                          |
    //onAdd:{                                     |

      namespace{
        s32 onAdd( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x + y ));
          return 1;
        }
      }

    //}:                                          |
    //onSub:{                                     |

      namespace{
        s32 onSub( lua_State* L ){
          const f32& x = lua_tonumber( L, -2 );
          const f32& y = lua_tonumber( L, -1 );
          lua_pushnumber( L, f32::cast( x - y ));
          return 1;
        }
      }

    //}:                                          |
  //}:                                            |
  //[vector]:{                                    |
    //onSaturateVector:{                          |

#ifdef __APPLE__
  #pragma mark Vector maths
#endif

      namespace{
        s32 onSaturateVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L
            , e_saturate( v.x )
            , e_saturate( v.y )
            , e_saturate( v.z )
          );
        }
      }

    //}:                                          |
    //onRadVector:{                               |

      namespace{
        s32 onRadVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.rad(), v.y.rad(), v.z.rad() );
        }
      }

    //}:                                          |
    //onDegVector:{                               |

      namespace{
        s32 onDegVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.deg(), v.y.deg(), v.z.deg() );
        }
      }

    //}:                                          |
    //onFloorVector:{                             |

      namespace{
        s32 onFloorVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.floor(), v.y.floor(), v.z.floor() );
        }
      }

    //}:                                          |
    //onCeilVector:{                              |

      namespace{
        s32 onCeilVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.ceil(), v.y.ceil(), v.z.ceil() );
        }
      }

    //}:                                          |
    //onRoundVector:{                             |

      namespace{
        s32 onRoundVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.round(), v.y.round(), v.z.round() );
        }
      }

    //}:                                          |
    //onRsqrtVector:{                             |

      namespace{
        s32 onRsqrtVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.rsqrt(), v.y.rsqrt(), v.z.rsqrt() );
        }
      }

    //}:                                          |
    //onSqrtVector:{                              |

      namespace{
        s32 onSqrtVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.sqrt(), v.y.sqrt(), v.z.sqrt() );
        }
      }

    //}:                                          |
    //onAbsVector:{                               |

      namespace{
        s32 onAbsVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.abs(), v.y.abs(), v.z.abs() );
        }
      }

    //}:                                          |
    //onExpVector:{                               |

      namespace{
        s32 onExpVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.exp(), v.y.exp(), v.z.exp() );
        }
      }

    //}:                                          |
    //onSquaredVector:{                           |

      namespace{
        s32 onSquaredVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.squared(), v.y.squared(), v.z.squared() );
        }
      }

    //}:                                          |
    //onCubedVector:{                             |

      namespace{
        s32 onCubedVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.cubed(), v.y.cubed(), v.z.cubed() );
        }
      }

    //}:                                          |
    //onAtan2Vector:{                             |

      namespace{

        s32 onAtan2Vector( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable( L, -1 ));
          e_assert( lua_istable( L, -2 ));
          if( !lua_istable( L, -1 ) || !lua_istable( L, -2 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -1, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createVec3( L
            , f32::cast( x0.atan2( x1 ))
            , f32::cast( y0.atan2( y1 ))
            , f32::cast( z0.atan2( z1 ))
          );
        }
      }

    //}:                                          |
    //onAtanVector:{                              |

      namespace{
        s32 onAtanVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.atan(), v.y.atan(), v.z.atan() );
        }
      }

    //}:                                          |
    //onTanVector:{                               |

      namespace{
        s32 onTanVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.tan(), v.y.tan(), v.z.tan() );
        }
      }

    //}:                                          |
    //onAcosVector:{                              |

      namespace{
        s32 onAcosVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.acos(), v.y.acos(), v.z.acos() );
        }
      }

    //}:                                          |
    //onCosVector:{                               |

      namespace{
        s32 onCosVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.cos(), v.y.cos(), v.z.cos() );
        }
      }

    //}:                                          |
    //onAsinVector:{                              |

      namespace{
        s32 onAsinVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.asin(), v.y.asin(), v.z.asin() );
        }
      }

    //}:                                          |
    //onSinVector:{                               |

      namespace{
        s32 onSinVector( lua_State* L ){
          const vec3& v = getVector( L );
          return lua_createVec3( L, v.x.sin(), v.y.sin(), v.z.sin() );
        }
      }

    //}:                                          |
    //onClampVector:{                             |

      namespace{

        s32 onClampVector( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_isnumber( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_isnumber( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& min = lua_tonumber( L, -2 );
          const f32& max = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createVec3( L
            , f32::cast( x.clamped( min, max ))
            , f32::cast( y.clamped( min, max ))
            , f32::cast( z.clamped( min, max ))
          );
        }
      }

    //}:                                          |
    //onLerpVector:{                              |

      namespace{

        s32 onLerpVector( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_istable ( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_istable( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& t = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createVec3( L
            , f32::cast( x0.lerp( x1, t ))
            , f32::cast( y0.lerp( y1, t ))
            , f32::cast( z0.lerp( z1, t ))
          );
        }
      }

    //}:                                          |
    //onCosLerpVector:{                           |

      namespace{

        s32 onCosLerpVector( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_istable ( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_istable( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& t = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createVec3( L
            , f32::cast( x0.cosLerp( x1, t ))
            , f32::cast( y0.cosLerp( y1, t ))
            , f32::cast( z0.cosLerp( z1, t ))
          );
        }
      }

    //}:                                          |
    //onMaxVector:{                               |

      namespace{
        s32 onMaxVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x.max( v2.x ))
                , f32::cast( v1.y.max( v2.y ))
                , f32::cast( v1.z.max( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onMinVector:{                               |

      namespace{
        s32 onMinVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x.min( v2.x ))
                , f32::cast( v1.y.min( v2.y ))
                , f32::cast( v1.z.min( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onPowVector:{                               |

      namespace{
        s32 onPowVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x.pow( v2.x ))
                , f32::cast( v1.y.pow( v2.y ))
                , f32::cast( v1.z.pow( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onModVector:{                               |

      namespace{
        s32 onModVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x.mod( v2.x ))
                , f32::cast( v1.y.mod( v2.y ))
                , f32::cast( v1.z.mod( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onMulVector:{                               |

      namespace{

        s32 onMulVector( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable( L, -1 ));
          e_assert( lua_istable( L, -2 ));
          if( !lua_istable( L, -1 ) || !lua_istable( L, -2 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab the class identifiers.
          //--------------------------------------------------------------------

          lua_getfield( L, -2, "__class" );//+1
          const u64 classA = lua_tointeger( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield( L, -1, "__class" );//+1
          const u64 classB = lua_tointeger( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab point from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab point from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -1, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Create table and return summed up points in a table.
          //--------------------------------------------------------------------

          f32 x, y, z;
          if( classA == e_classid<quat>() ){
            if(( classB == e_classid<pt3>() )||( classB == e_classid<ssi<pt3>>() )){
              const quat   q( x0, y0, z0 );
              const pt3    p( x1, y1, z1 );
              const vec3x3 m( q.get3x3() );
              const pt3    r( p * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else if(( classB == e_classid<vec3>() )||( classB == e_classid<ssi<vec3>>() )){
              const quat   q( x0, y0, z0 );
              const vec3   v( x1, y1, z1 );
              const vec3x3 m( q.get3x3() );
              const vec3   r( v * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else{
              e_unreachable( "Unhandled type in Lua expression!" );
            }
          }else if( classB == e_classid<quat>() ){
            if(( classA == e_classid<pt3>() )||( classA == e_classid<ssi<pt3>>() )){
              const quat   q( x1, y1, z1 );
              const pt3    p( x0, y0, z0 );
              const vec3x3 m( q.get3x3() );
              const pt3    r( p * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else if(( classA == e_classid<vec3>() )||( classA == e_classid<ssi<vec3>>() )){
              const quat   q( x1, y1, z1 );
              const vec3   v( x0, y0, z0 );
              const vec3x3 m( q.get3x3() );
              const vec3   r( v * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else{
              e_unreachable( "Unhandled type in Lua expression!" );
            }
          }else{
            x = x0 * x1;
            y = y0 * y1;
            z = z0 * z1;
          }

          // Return new point table.
          lua_createVec3( L, x, y, z );
          return 1;
        }
      }

    //}:                                          |
    //onDivVector:{                               |

      namespace{
        s32 onDivVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x / v2.x )
                , f32::cast( v1.y / v2.y )
                , f32::cast( v1.z / v2.z )
              );
            }
          );
        }
      }

    //}:                                          |
    //onAddVector:{                               |

      namespace{
        s32 onAddVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x + v2.x )
                , f32::cast( v1.y + v2.y )
                , f32::cast( v1.z + v2.z )
              );
            }
          );
        }
      }

    //}:                                          |
    //onSubVector:{                               |

      namespace{
        s32 onSubVector( lua_State* L ){
          return calcVectors( L,
            [L]( const vec3& v1, const vec3& v2 )->s32{
              return lua_createVec3( L
                , f32::cast( v1.x - v2.x )
                , f32::cast( v1.y - v2.y )
                , f32::cast( v1.z - v2.z )
              );
            }
          );
        }
      }

    //}:                                          |
  //}:                                            |
  //[point]:{                                     |
    //onSaturatePoint:{                           |

#ifdef __APPLE__
  #pragma mark Point maths
#endif

      namespace{
        s32 onSaturatePoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L
            , e_saturate( v.x )
            , e_saturate( v.y )
            , e_saturate( v.z )
          );
        }
      }

    //}:                                          |
    //onRadPoint:{                                |

      namespace{
        s32 onRadPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.rad(), v.y.rad(), v.z.rad() );
        }
      }

    //}:                                          |
    //onDegPoint:{                                |

      namespace{
        s32 onDegPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.deg(), v.y.deg(), v.z.deg() );
        }
      }

    //}:                                          |
    //onFloorPoint:{                              |

      namespace{
        s32 onFloorPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.floor(), v.y.floor(), v.z.floor() );
        }
      }

    //}:                                          |
    //onCeilPoint:{                               |

      namespace{
        s32 onCeilPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.ceil(), v.y.ceil(), v.z.ceil() );
        }
      }

    //}:                                          |
    //onRoundPoint:{                              |

      namespace{
        s32 onRoundPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.round(), v.y.round(), v.z.round() );
        }
      }

    //}:                                          |
    //onRsqrtPoint:{                              |

      namespace{
        s32 onRsqrtPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.rsqrt(), v.y.rsqrt(), v.z.rsqrt() );
        }
      }

    //}:                                          |
    //onSqrtPoint:{                               |

      namespace{
        s32 onSqrtPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.sqrt(), v.y.sqrt(), v.z.sqrt() );
        }
      }

    //}:                                          |
    //onAbsPoint:{                                |

      namespace{
        s32 onAbsPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.abs(), v.y.abs(), v.z.abs() );
        }
      }

    //}:                                          |
    //onExpPoint:{                                |

      namespace{
        s32 onExpPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.exp(), v.y.exp(), v.z.exp() );
        }
      }

    //}:                                          |
    //onSquaredPoint:{                            |

      namespace{
        s32 onSquaredPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.squared(), v.y.squared(), v.z.squared() );
        }
      }

    //}:                                          |
    //onCubedPoint:{                              |

      namespace{
        s32 onCubedPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.cubed(), v.y.cubed(), v.z.cubed() );
        }
      }

    //}:                                          |
    //onAtan2Point:{                              |

      namespace{

        s32 onAtan2Point( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable( L, -1 ));
          e_assert( lua_istable( L, -2 ));
          if( !lua_istable( L, -1 ) || !lua_istable( L, -2 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -1, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createPt3( L
            , f32::cast( x0.atan2( x1 ))
            , f32::cast( y0.atan2( y1 ))
            , f32::cast( z0.atan2( z1 ))
          );
        }
      }

    //}:                                          |
    //onAtanPoint:{                               |

      namespace{
        s32 onAtanPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.atan(), v.y.atan(), v.z.atan() );
        }
      }

    //}:                                          |
    //onTanPoint:{                                |

      namespace{
        s32 onTanPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.tan(), v.y.tan(), v.z.tan() );
        }
      }

    //}:                                          |
    //onAcosPoint:{                               |

      namespace{
        s32 onAcosPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.acos(), v.y.acos(), v.z.acos() );
        }
      }

    //}:                                          |
    //onCosPoint:{                                |

      namespace{
        s32 onCosPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.cos(), v.y.cos(), v.z.cos() );
        }
      }

    //}:                                          |
    //onAsinPoint:{                               |

      namespace{
        s32 onAsinPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.asin(), v.y.asin(), v.z.asin() );
        }
      }

    //}:                                          |
    //onSinPoint:{                                |

      namespace{
        s32 onSinPoint( lua_State* L ){
          const pt3& v = getPoint( L );
          return lua_createPt3( L, v.x.sin(), v.y.sin(), v.z.sin() );
        }
      }

    //}:                                          |
    //onClampPoint:{                              |

      namespace{

        s32 onClampPoint( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_isnumber( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_isnumber( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& min = lua_tonumber( L, -2 );
          const f32& max = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createPt3( L
            , f32::cast( x.clamped( min, max ))
            , f32::cast( y.clamped( min, max ))
            , f32::cast( z.clamped( min, max ))
          );
        }
      }

    //}:                                          |
    //onLerpPoint:{                               |

      namespace{

        s32 onLerpPoint( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_istable ( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_istable( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& t = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createPt3( L
            , f32::cast( x0.lerp( x1, t ))
            , f32::cast( y0.lerp( y1, t ))
            , f32::cast( z0.lerp( z1, t ))
          );
        }
      }

    //}:                                          |
    //onCosLerpPoint:{                            |

      namespace{

        s32 onCosLerpPoint( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_istable ( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_istable( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& t = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createPt3( L
            , f32::cast( x0.cosLerp( x1, t ))
            , f32::cast( y0.cosLerp( y1, t ))
            , f32::cast( z0.cosLerp( z1, t ))
          );
        }
      }

    //}:                                          |
    //onMaxPoint:{                                |

      namespace{
        s32 onMaxPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x.max( v2.x ))
                , f32::cast( v1.y.max( v2.y ))
                , f32::cast( v1.z.max( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onMinPoint:{                                |

      namespace{
        s32 onMinPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x.min( v2.x ))
                , f32::cast( v1.y.min( v2.y ))
                , f32::cast( v1.z.min( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onPowPoint:{                                |

      namespace{
        s32 onPowPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x.pow( v2.x ))
                , f32::cast( v1.y.pow( v2.y ))
                , f32::cast( v1.z.pow( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onModPoint:{                                |

      namespace{
        s32 onModPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x.mod( v2.x ))
                , f32::cast( v1.y.mod( v2.y ))
                , f32::cast( v1.z.mod( v2.z ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onMulPoint:{                                |

      namespace{

        s32 onMulPoint( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable( L, -1 ));
          e_assert( lua_istable( L, -2 ));
          if( !lua_istable( L, -1 ) || !lua_istable( L, -2 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab the class identifiers.
          //--------------------------------------------------------------------

          lua_getfield( L, -1, "__class" );//+1
          const u64 classA = lua_tointeger( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield( L, -2, "__class" );//+1
          const u64 classB = lua_tointeger( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab point from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -1, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -1, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab point from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Create table and return summed up points in a table.
          //--------------------------------------------------------------------

          f32 x, y, z;
          if( classA == e_classid<quat>() ){
            if(( classB == e_classid<pt3>() )||( classB == e_classid<ssi<pt3>>() )){
              const quat   q( x0, y0, z0 );
              const pt3    p( x1, y1, z1 );
              const vec3x3 m( q.get3x3() );
              const pt3    r( p * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else if(( classB == e_classid<pt3>() )||( classB == e_classid<ssi<pt3>>() )){
              const quat   q( x0, y0, z0 );
              const pt3    v( x1, y1, z1 );
              const vec3x3 m( q.get3x3() );
              const pt3    r( v * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else{
              e_unreachable( "Unhandled type in Lua expression!" );
            }
          }else if( classB == e_classid<quat>() ){
            if(( classA == e_classid<pt3>() )||( classA == e_classid<ssi<pt3>>() )){
              const quat   q( x1, y1, z1 );
              const pt3    p( x0, y0, z0 );
              const vec3x3 m( q.get3x3() );
              const pt3    r( p * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else if(( classA == e_classid<pt3>() )||( classA == e_classid<ssi<pt3>>() )){
              const quat   q( x1, y1, z1 );
              const pt3    v( x0, y0, z0 );
              const vec3x3 m( q.get3x3() );
              const pt3    r( v * m );
              x = r.x;
              y = r.y;
              z = r.z;
            }else{
              e_unreachable( "Unhandled type in Lua expression!" );
            }
          }else{
            x = x0 * x1;
            y = y0 * y1;
            z = z0 * z1;
          }

          // Return new point table.
          lua_createPt3( L, x, y, z );
          return 1;
        }
      }

    //}:                                          |
    //onDivPoint:{                                |

      namespace{
        s32 onDivPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x / v2.x )
                , f32::cast( v1.y / v2.y )
                , f32::cast( v1.z / v2.z )
              );
            }
          );
        }
      }

    //}:                                          |
    //onAddPoint:{                                |

      namespace{
        s32 onAddPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x + v2.x )
                , f32::cast( v1.y + v2.y )
                , f32::cast( v1.z + v2.z )
              );
            }
          );
        }
      }

    //}:                                          |
    //onSubPoint:{                                |

      namespace{
        s32 onSubPoint( lua_State* L ){
          return calcPoints( L,
            [L]( const pt3& v1, const pt3& v2 )->s32{
              return lua_createPt3( L
                , f32::cast( v1.x - v2.x )
                , f32::cast( v1.y - v2.y )
                , f32::cast( v1.z - v2.z )
              );
            }
          );
        }
      }

    //}:                                          |
  //}:                                            |
  //[color]:{                                     |
    //onSaturateColor:{                           |

#ifdef __APPLE__
  #pragma mark Color maths
#endif

      namespace{
        s32 onSaturateColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L
            , e_saturate( v.r )
            , e_saturate( v.g )
            , e_saturate( v.b )
            , e_saturate( v.a )
          );
        }
      }

    //}:                                          |
    //onRadColor:{                                |

      namespace{
        s32 onRadColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onDegColor:{                                |

      namespace{
        s32 onDegColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onFloorColor:{                              |

      namespace{
        s32 onFloorColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.floor(), v.g.floor(), v.b.floor(), v.a.floor() );
        }
      }

    //}:                                          |
    //onCeilColor:{                               |

      namespace{
        s32 onCeilColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.ceil(), v.g.ceil(), v.b.ceil(), v.a.ceil() );
        }
      }

    //}:                                          |
    //onRoundColor:{                              |

      namespace{
        s32 onRoundColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.round(), v.g.round(), v.b.round(), v.a.round() );
        }
      }

    //}:                                          |
    //onRsqrtColor:{                              |

      namespace{
        s32 onRsqrtColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onSqrtColor:{                               |

      namespace{
        s32 onSqrtColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onAbsColor:{                                |

      namespace{
        s32 onAbsColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.abs(), v.g.abs(), v.b.abs(), v.a.abs() );
        }
      }

    //}:                                          |
    //onExpColor:{                                |

      namespace{
        s32 onExpColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.exp(), v.g.exp(), v.b.exp(), v.a.exp() );
        }
      }

    //}:                                          |
    //onSquaredColor:{                            |

      namespace{
        s32 onSquaredColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.squared(), v.g.squared(), v.b.squared(), v.a.squared() );
        }
      }

    //}:                                          |
    //onCubedColor:{                              |

      namespace{
        s32 onCubedColor( lua_State* L ){
          const rgba& v = getColor( L );
          return lua_createColor( L, v.r.cubed(), v.g.cubed(), v.b.cubed(), v.a.cubed() );
        }
      }

    //}:                                          |
    //onAtan2Color:{                              |

      namespace{
        s32 onAtan2Color( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onAtanColor:{                               |

      namespace{
        s32 onAtanColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onTanColor:{                                |

      namespace{
        s32 onTanColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onAcosColor:{                               |

      namespace{
        s32 onAcosColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onCosColor:{                                |

      namespace{
        s32 onCosColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onAsinColor:{                               |

      namespace{
        s32 onAsinColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onSinColor:{                                |

      namespace{
        s32 onSinColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onClampColor:{                              |

      namespace{

        s32 onClampColor( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_isnumber( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_isnumber( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "r" );//+1
          const f32& x = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "g" );//+1
          const f32& y = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "b" );//+1
          const f32& z = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "a" );//+1
          const f32& w = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& min = lua_tonumber( L, -2 );
          const f32& max = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createColor( L
            , f32::cast( x.clamped( min, max ))
            , f32::cast( y.clamped( min, max ))
            , f32::cast( z.clamped( min, max ))
            , f32::cast( w.clamped( min, max ))
          );
        }
      }

    //}:                                          |
    //onLerpColor:{                               |

      namespace{

        s32 onLerpColor( lua_State* L ){

          //--------------------------------------------------------------------
          // Validate inputs and bail conditions.
          //--------------------------------------------------------------------

          e_assert( lua_istable ( L, -3 ));
          e_assert( lua_istable ( L, -2 ));
          e_assert( lua_isnumber( L, -1 ));
          if( !lua_istable( L, -3 )||!lua_istable( L, -2 )||!lua_isnumber( L, -1 )){
            lua_pushnil( L );
            return 1;
          }

          //--------------------------------------------------------------------
          // Grab vector from 1st table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -3, "x" );//+1
          const f32& x0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "y" );//+1
          const f32& y0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& z0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -3, "z" );//+1
          const f32& w0 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab vector from 2nd table.
          //--------------------------------------------------------------------

          lua_getfield(  L, -2, "x" );//+1
          const f32& x1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "y" );//+1
          const f32& y1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          lua_getfield(  L, -2, "z" );//+1
          const f32& z1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1
          const f32& w1 = lua_tonumber( L, -1 );
          lua_pop( L, 1 );//-1

          //--------------------------------------------------------------------
          // Grab numbers.
          //--------------------------------------------------------------------

          const f32& t = lua_tonumber( L, -1 );

          //--------------------------------------------------------------------
          // Create table and return summed up vectors in a table.
          //--------------------------------------------------------------------

          return lua_createColor( L
            , f32::cast( x0.lerp( x1, t ))
            , f32::cast( y0.lerp( y1, t ))
            , f32::cast( z0.lerp( z1, t ))
            , f32::cast( w0.lerp( w1, t ))
          );
        }
      }

    //}:                                          |
    //onCosLerpColor:{                            |

      namespace{
        s32 onCosLerpColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onMaxColor:{                                |

      namespace{
        s32 onMaxColor( lua_State* L ){
          return calcColors( L,
            [L]( const rgba& v1, const rgba& v2 )->s32{
              return lua_createColor( L
                , f32::cast( v1.r.max( v2.r ))
                , f32::cast( v1.g.max( v2.g ))
                , f32::cast( v1.b.max( v2.b ))
                , f32::cast( v1.a.max( v2.a ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onMinColor:{                                |

      namespace{
        s32 onMinColor( lua_State* L ){
          return calcColors( L,
            [L]( const rgba& v1, const rgba& v2 )->s32{
              return lua_createColor( L
                , f32::cast( v1.r.min( v2.r ))
                , f32::cast( v1.g.min( v2.g ))
                , f32::cast( v1.b.min( v2.b ))
                , f32::cast( v1.a.min( v2.a ))
              );
            }
          );
        }
      }

    //}:                                          |
    //onPowColor:{                                |

      namespace{
        s32 onPowColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onModColor:{                                |

      namespace{
        s32 onModColor( lua_State* L ){
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //onMulColor:{                                |

      namespace{
        s32 onMulColor( lua_State* L ){
          return calcColors( L,
            [L]( const rgba& v1, const rgba& v2 )->s32{
              return lua_createColor( L
                , f32::cast( v1.r * v2.r )
                , f32::cast( v1.g * v2.g )
                , f32::cast( v1.b * v2.b )
                , f32::cast( v1.a * v2.a )
              );
            }
          );
        }
      }

    //}:                                          |
    //onDivColor:{                                |

      namespace{
        s32 onDivColor( lua_State* L ){
          return calcColors( L,
            [L]( const rgba& v1, const rgba& v2 )->s32{
              return lua_createColor( L
                , f32::cast( v1.r / v2.r )
                , f32::cast( v1.g / v2.g )
                , f32::cast( v1.b / v2.b )
                , f32::cast( v1.a / v2.a )
              );
            }
          );
        }
      }

    //}:                                          |
    //onAddColor:{                                |

      namespace{
        s32 onAddColor( lua_State* L ){
          return calcColors( L,
            [L]( const rgba& v1, const rgba& v2 )->s32{
              return lua_createColor( L
                , f32::cast( v1.r + v2.r )
                , f32::cast( v1.g + v2.g )
                , f32::cast( v1.b + v2.b )
                , f32::cast( v1.a + v2.a )
              );
            }
          );
        }
      }

    //}:                                          |
    //onSubColor:{                                |

      namespace{
        s32 onSubColor( lua_State* L ){
          return calcColors( L,
            [L]( const rgba& v1, const rgba& v2 )->s32{
              return lua_createColor( L
                , f32::cast( v1.r - v2.r )
                , f32::cast( v1.g - v2.g )
                , f32::cast( v1.b - v2.b )
                , f32::cast( v1.a - v2.a )
              );
            }
          );
        }
      }

    //}:                                          |
  //}:                                            |
  //[ctors]:{                                     |
    //onCtorColor:{                               |

#ifdef __APPLE__
  #pragma mark (constructors)
#endif

      namespace{
        s32 onCtorColor( lua_State* L ){
          const f32& r = lua_tonumber( L, -4 );
          const f32& g = lua_tonumber( L, -3 );
          const f32& b = lua_tonumber( L, -2 );
          const f32& a = lua_tonumber( L, -1 );
          return lua_createColor( L, r, g, b, a );
        }
      }

    //}:                                          |
    //onCtorQuat:{                                |

      namespace{
        s32 onCtorQuat( lua_State* L ){
          const f32& x = lua_tonumber( L, -3 );
          const f32& y = lua_tonumber( L, -2 );
          const f32& z = lua_tonumber( L, -1 );
          return lua_createQuat( L, x, y, z );
        }
      }

    //}:                                          |
    //onCtorVec3:{                                |

      namespace{
        s32 onCtorVec3( lua_State* L ){
          const f32& x = lua_tonumber( L, -3 );
          const f32& y = lua_tonumber( L, -2 );
          const f32& z = lua_tonumber( L, -1 );
          return lua_createVec3( L, x, y, z );
        }
      }

    //}:                                          |
    //onCtorPt3:{                                 |

      namespace{
        s32 onCtorPt3( lua_State* L ){
          const f32& x = lua_tonumber( L, -3 );
          const f32& y = lua_tonumber( L, -2 );
          const f32& z = lua_tonumber( L, -1 );
          return lua_createPt3( L, x, y, z );
        }
      }

    //}:                                          |
  //}:                                            |
  //[rand]:{                                      |
    //e_randvec3:{                                |

      namespace{
        s32 onRandVec3( lua_State* L ){
          const vec3& v = e_randunitvec3();
          return lua_createVec3( L, v.x, v.y, v.z );
        }
      }

    //}:                                          |
    //e_randunit:{                                |

      namespace{
        s32 onRandUnit( lua_State* L ){
          const f32& f = e_randunit();
          lua_pushnumber( L, f32::cast( f ));
          return 1;
        }
      }

    //}:                                          |
    //e_randse:{                                  |

      namespace{
        s32 onRandSE( lua_State* L ){
          if( lua_isnumber( L, -2 )){
            const f32& s = lua_tonumber( L, -2 );
            if( lua_isnumber( L, -1 )){
              const f32& e = lua_tonumber( L, -1 );
              const f32& f = e_rand( s, e );
              lua_pushnumber( L, f32::cast( f ));
              return 1;
            }
          }
          lua_pushnil( L );
          return 1;
        }
      }

    //}:                                          |
    //e_randpi:{                                  |

      namespace{
        s32 onRandPI( lua_State* L ){
          const f32& f = e_randpi();
          lua_pushnumber( L, f32::cast( f ));
          return 1;
        }
      }

    //}:                                          |
    //e_rand:{                                    |

      namespace{
        s32 onRand( lua_State* L ){
          const s64 i = e_rand<s64>();
          lua_pushinteger( L, i );
          return 1;
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//TableFu:{                                       |

  luaL_Reg standard[140]={
    //LuaNative (10):{                            |

      {"setmetatable",     Lua::setmetatable },
      {"getmetatable",     Lua::getmetatable },
      {"tostring",         Lua::tostring     },
      {"select",           Lua::select       },
      {"error",            Lua::error        },
      {"print",            Lua::print        },
      {"type",             Lua::type         },
      {"next",             Lua::next         },
      {"pairs",            Lua::pairs        },

    //}:                                          |
    //Vectors   (30):{                            |

      {"saturateVector", onSaturateVector },
      {"radVector",      onRadVector      },
      {"degVector",      onDegVector      },
      {"floorVector",    onFloorVector    },
      {"ceilVector",     onCeilVector     },
      {"roundVector",    onRoundVector    },
      {"rsqrtVector",    onRsqrtVector    },
      {"sqrtVector",     onSqrtVector     },
      {"absVector",      onAbsVector      },
      {"expVector",      onExpVector      },
      {"squaredVector",  onSquaredVector  },
      {"cubedVector",    onCubedVector    },
      {"atanVector",     onAtanVector     },
      {"tanVector",      onTanVector      },
      {"acosVector",     onAcosVector     },
      {"cosVector",      onCosVector      },
      {"asinVector",     onAsinVector     },
      {"sinVector",      onSinVector      },
      {"clampVector",    onClampVector    },
      {"lerpVector",     onLerpVector     },
      {"coslerpVector",  onCosLerpVector  },
      {"maxVector",      onMaxVector      },
      {"minVector",      onMinVector      },
      {"atan2Vector",    onAtan2Vector    },
      {"powVector",      onPowVector      },
      {"modVector",      onModVector      },
      {"mulVector",      onMulVector      },
      {"divVector",      onDivVector      },
      {"addVector",      onAddVector      },
      {"subVector",      onSubVector      },

    //}:                                          |
    //Points    (30):{                            |

      {"saturatePoint",  onSaturatePoint  },
      {"radPoint",       onRadPoint       },
      {"degPoint",       onDegPoint       },
      {"floorPoint",     onFloorPoint     },
      {"ceilPoint",      onCeilPoint      },
      {"roundPoint",     onRoundPoint     },
      {"rsqrtPoint",     onRsqrtPoint     },
      {"sqrtPoint",      onSqrtPoint      },
      {"absPoint",       onAbsPoint       },
      {"expPoint",       onExpPoint       },
      {"squaredPoint",   onSquaredPoint   },
      {"cubedPoint",     onCubedPoint     },
      {"atanPoint",      onAtanPoint      },
      {"tanPoint",       onTanPoint       },
      {"acosPoint",      onAcosPoint      },
      {"cosPoint",       onCosPoint       },
      {"asinPoint",      onAsinPoint      },
      {"sinPoint",       onSinPoint       },
      {"clampPoint",     onClampPoint     },
      {"lerpPoint",      onLerpPoint      },
      {"coslerpPoint",   onCosLerpPoint   },
      {"maxPoint",       onMaxPoint       },
      {"minPoint",       onMinPoint       },
      {"atan2Point",     onAtan2Point     },
      {"powPoint",       onPowPoint       },
      {"modPoint",       onModPoint       },
      {"mulPoint",       onMulPoint       },
      {"divPoint",       onDivPoint       },
      {"addPoint",       onAddPoint       },
      {"subPoint",       onSubPoint       },

    //}:                                          |
    //Colors    (30):{                            |

      {"saturateColor",  onSaturateColor  },
      {"radColor",       onRadColor       },
      {"degColor",       onDegColor       },
      {"floorColor",     onFloorColor     },
      {"ceilColor",      onCeilColor      },
      {"roundColor",     onRoundColor     },
      {"rsqrtColor",     onRsqrtColor     },
      {"sqrtColor",      onSqrtColor      },
      {"absColor",       onAbsColor       },
      {"expColor",       onExpColor       },
      {"squaredColor",   onSquaredColor   },
      {"cubedColor",     onCubedColor     },
      {"atanColor",      onAtanColor      },
      {"tanColor",       onTanColor       },
      {"acosColor",      onAcosColor      },
      {"cosColor",       onCosColor       },
      {"asinColor",      onAsinColor      },
      {"sinColor",       onSinColor       },
      {"clampColor",     onClampColor     },
      {"lerpColor",      onLerpColor      },
      {"coslerpColor",   onCosLerpColor   },
      {"maxColor",       onMaxColor       },
      {"minColor",       onMinColor       },
      {"atan2Color",     onAtan2Color     },
      {"powColor",       onPowColor       },
      {"modColor",       onModColor       },
      {"mulColor",       onMulColor       },
      {"divColor",       onDivColor       },
      {"addColor",       onAddColor       },
      {"subColor",       onSubColor       },

    //}:                                          |
    //Scalar    (30):{                            |

      {"saturate",       onSaturate       },
      {"rad",            onRad            },
      {"deg",            onDeg            },
      {"floor",          onFloor          },
      {"ceil",           onCeil           },
      {"round",          onRound          },
      {"rsqrt",          onRsqrt          },
      {"sqrt",           onSqrt           },
      {"abs",            onAbs            },
      {"exp",            onExp            },
      {"squared",        onSquared        },
      {"cubed",          onCubed          },
      {"atan",           onAtan           },
      {"tan",            onTan            },
      {"acos",           onAcos           },
      {"cos",            onCos            },
      {"asin",           onAsin           },
      {"sin",            onSin            },
      {"clamp",          onClamp          },
      {"lerp",           onLerp           },
      {"coslerp",        onCosLerp        },
      {"max",            onMax            },
      {"min",            onMin            },
      {"atan2",          onAtan2          },
      {"pow",            onPow            },
      {"mod",            onMod            },
      {"mul",            onMul            },
      {"div",            onDiv            },
      {"add",            onAdd            },
      {"sub",            onSub            },

    //}:                                          |
    //Ctors     ( 4):{                            |

      {"rgba",   onCtorColor  },
      {"quat",   onCtorQuat   },
      {"vec3",   onCtorVec3   },
      {"pt3",    onCtorPt3    },

    //}:                                          |
    //Math      ( 5):{                            |

      {"randvec3", onRandVec3 },
      {"randunit", onRandUnit },
      {"randse",   onRandSE   },
      {"randpi",   onRandPI   },
      {"rand",     onRand     },

    //}:                                          |
    //END       ( 1):{                            |

      {0,0}

    //}:                                          |
  };

//}:                                              |
//================================================+=============================
