//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup base
  *   @{
  *     \addtogroup compiler
  *     @{
  */

//================================================+=============================
//GCC:{                                           |

  #if( defined __GNUC__ || defined __GNUG__ ) && !defined __clang__
    //Standard compiler options:{                 |

      #ifdef __linux__
        #define __compiling_vulkan__ 1
        #define __compiling_linux__  1
        #define __compiling_pc__     1
      #endif
      #define __compiling_gcc__ 1

    //}:                                          |
    //Standard auto-detection:{                   |

      #ifdef ANDROID
        #define __compiling_android__ 1
        #define __compiling_mobile__  1
        #define __compiling_opengl__  1
      #endif

    //}:                                          |
    //Platform types:{                            |
      //e_declptrs:{                              |

        #ifdef __cplusplus

          /** \brief Engine ptr type definition macro.
            *
            * The e_declptrs macro is used to define pointer types with the right
            * naming convention.
            *
            * \param x The one letter name of the type.
            * \param y The base type.
            */

          #ifndef e_declptrs
          #define e_declptrs( x, y )                                              \
            typedef const y* c##x##p;                                             \
            typedef       y*    x##p
          #endif

          namespace EON{
            e_declptrs( l, long );
            e_declptrs( c, char );
            e_declptrs( i, int  );
            e_declptrs( v, void );
          }

        #endif

      //}:                                        |
      //e_declints:{                              |

        #ifdef __cplusplus

          /** \brief Engine integer type definition macro.
            *
            * The e_declints macro is used to define pointer types with the right
            * naming convention.
            *
            * \param x The name of the type.
            * \param y The base type.
            */

          #ifndef e_declints
          #define e_declints( bits )                                              \
            typedef   signed __int##bits s##bits;                                 \
            typedef unsigned __int##bits u##bits
          #endif

          #ifndef __int64
          #define __int64 long long
          #endif

          #ifndef __int32
          #define __int32 int
          #endif

          #ifndef __int16
          #define __int16 short
          #endif

          #ifndef __int8
          #define __int8 char
          #endif

          namespace EON{
            e_declints( 64 );
            e_declints( 32 );
            e_declints( 16 );
            e_declints(  8 );
          }

        #endif

      //}:                                        |
    //}:                                          |
    //Export:{                                    |

      #define E_PUBLISH
      #define E_REFLECT
      #define E_EXPORT
      #define E_IMPORT

    //}:                                          |
  #endif

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
