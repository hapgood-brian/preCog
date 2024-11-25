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
//_llvm_ver:{                                     |

  #if defined __clang__||defined __llvm__
    //Override standard library:{                 |

      #ifdef _LIBCPP_INLINE_VISIBILITY
      #undef _LIBCPP_INLINE_VISIBILITY
      #endif

      #define _LIBCPP_INLINE_VISIBILITY __attribute__((__visibility__("hidden"), __always_inline__,__nodebug__))

    //}:                                          |
    //Standard compiler options:{                 |

      #ifdef __EMSCRIPTEN__
        #define __compiling_web__ 1
      #endif
      #ifdef __linux__
        #define __compiling_linux__ 1
      #endif
      #define __compiling_clang__ 1
      #define __compiling_llvm__  1

    //}:                                          |
    //Restricted pointers:{                       |

      #define RESTRICT __restrict__

    //}:                                          |
    //Disable warnings:{                          |

      #pragma clang diagnostic ignored "-Wundefined-var-template"

    //}:                                          |
    //Detect C++11:{                              |

      #ifdef __cplusplus
        #if __cplusplus <= 201103L
          #define __compiling_cpp11__ 1
        #endif
      #endif

    //}:                                          |
    //Standard auto-detection:{                   |

      #if __INTEL_COMPILER
        #define __compiling_intel__    1
      #endif
      #ifdef __APPLE__
        #include<Availability.h>
        #if __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
          #define __compiling_mobile__ 1
          #define __compiling_apple__  1
          #define __compiling_metal__  1
          #define __compiling_mlsl__   1
          #define __compiling_ios__    1
        #elif __MAC_OS_X_VERSION_MIN_REQUIRED >= 1050
          #define __compiling_apple__ 1
          #define __compiling_metal__ 1
          #define __compiling_mlsl__  1
          #define __compiling_osx__   1
          #define __compiling_pc__    1
        #endif
      #endif

    //}:                                          |
    //Intrinsic SSE headers:{                     |

      #if e_compiling( arm64 )
        #include<arm_neon.h>
      #elif e_compiling( x64 )
        #ifdef __SSE4_1__
          #include<smmintrin.h>
        #else
          #include<x86intrin.h>
        #endif
      #endif

    //}:                                          |
    //Platform types:{                            |
      //e_declptrs:{                              |

        #ifdef __cplusplus

          /** \brief Engine ptr type definition macro.
            *
            * The e_declptrs macro is used to define pointer types with the
            * right naming convention.
            *
            * \param x The one letter name of the type.
            * \param y The base type.
            */

          #ifndef e_declptrs
          #define e_declptrs( x, y )                                            \
            typedef const y* c##x##p;                                           \
            typedef       y*    x##p
          #endif

          namespace EON{
            e_declptrs( v, void );
            e_declptrs( c, char );
            e_declptrs( l, long );
            e_declptrs( i, int  );
          }

        #endif

      //}:                                        |
      //e_declints:{                              |

        #ifdef __cplusplus

          /** \brief Engine integer type definition macro.
            *
            * The e_declints macro is used to define pointer types with the
            * right naming convention.
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

          #ifndef e_packed
          #define e_packed(T,...)                                                 \
            struct __attribute__((packed))T{__VA_ARGS__;}
          #endif

        #endif

      //}:                                        |
    //}:
    //Warnings:{                                  |

      #pragma clang diagnostic ignored "-Wunknown-pragmas"
      #ifndef __APPLE__
        #pragma clang diagnostic ignored "-Wdeprecated-copy"
      #endif

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
//                                                :
//                                                :
//                                                :
//================================================+=============================
//equates:{                                       |
// clang -dM -E -x c /dev/null > clang.h to generate a header with all macros.
// See http://clang.llvm.org/docs/LanguageExtensions.html#feature%5Fcheck.
//
// #define OBJC_NEW_PROPERTIES 1
// #define _LP64 1
// #define __APPLE_CC__ 5621
// #define __APPLE__ 1
// #define __ATOMIC_ACQUIRE 2
// #define __ATOMIC_ACQ_REL 4
// #define __ATOMIC_CONSUME 1
// #define __ATOMIC_RELAXED 0
// #define __ATOMIC_RELEASE 3
// #define __ATOMIC_SEQ_CST 5
// #define __BLOCKS__ 1
// #define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
// #define __CHAR16_TYPE__ unsigned short
// #define __CHAR32_TYPE__ unsigned int
// #define __CHAR_BIT__ 8
// #define __CONSTANT_CFSTRINGS__ 1
// #define __DBL_DENORM_MIN__ 4.9406564584124654e-324
// #define __DBL_DIG__ 15
// #define __DBL_EPSILON__ 2.2204460492503131e-16
// #define __DBL_HAS_DENORM__ 1
// #define __DBL_HAS_INFINITY__ 1
// #define __DBL_HAS_QUIET_NAN__ 1
// #define __DBL_MANT_DIG__ 53
// #define __DBL_MAX_10_EXP__ 308
// #define __DBL_MAX_EXP__ 1024
// #define __DBL_MAX__ 1.7976931348623157e+308
// #define __DBL_MIN_10_EXP__ (-307)
// #define __DBL_MIN_EXP__ (-1021)
// #define __DBL_MIN__ 2.2250738585072014e-308
// #define __DECIMAL_DIG__ 21
// #define __DYNAMIC__ 1
// #define __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ 1080
// #define __FINITE_MATH_ONLY__ 0
// #define __FLT_DENORM_MIN__ 1.40129846e-45F
// #define __FLT_DIG__ 6
// #define __FLT_EPSILON__ 1.19209290e-7F
// #define __FLT_EVAL_METHOD__ 0
// #define __FLT_HAS_DENORM__ 1
// #define __FLT_HAS_INFINITY__ 1
// #define __FLT_HAS_QUIET_NAN__ 1
// #define __FLT_MANT_DIG__ 24
// #define __FLT_MAX_10_EXP__ 38
// #define __FLT_MAX_EXP__ 128
// #define __FLT_MAX__ 3.40282347e+38F
// #define __FLT_MIN_10_EXP__ (-37)
// #define __FLT_MIN_EXP__ (-125)
// #define __FLT_MIN__ 1.17549435e-38F
// #define __FLT_RADIX__ 2
// #define __GCC_ATOMIC_BOOL_LOCK_FREE 2
// #define __GCC_ATOMIC_CHAR16_T_LOCK_FREE 2
// #define __GCC_ATOMIC_CHAR32_T_LOCK_FREE 2
// #define __GCC_ATOMIC_CHAR_LOCK_FREE 2
// #define __GCC_ATOMIC_INT_LOCK_FREE 2
// #define __GCC_ATOMIC_LLONG_LOCK_FREE 2
// #define __GCC_ATOMIC_LONG_LOCK_FREE 2
// #define __GCC_ATOMIC_POINTER_LOCK_FREE 2
// #define __GCC_ATOMIC_SHORT_LOCK_FREE 2
// #define __GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1
// #define __GCC_ATOMIC_WCHAR_T_LOCK_FREE 2
// #define __GNUC_MINOR__ 2
// #define __GNUC_PATCHLEVEL__ 1
// #define __GNUC_STDC_INLINE__ 1
// #define __GNUC__ 4
// #define __GXX_ABI_VERSION 1002
// #define __GXX_RTTI 1
// #define __INT16_TYPE__ short
// #define __INT32_TYPE__ int
// #define __INT64_C_SUFFIX__ LL
// #define __INT64_TYPE__ long long int
// #define __INT8_TYPE__ char
// #define __INTMAX_MAX__ 9223372036854775807L
// #define __INTMAX_TYPE__ long int
// #define __INTMAX_WIDTH__ 64
// #define __INTPTR_TYPE__ long int
// #define __INTPTR_WIDTH__ 64
// #define __INT_MAX__ 2147483647
// #define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
// #define __LDBL_DIG__ 18
// #define __LDBL_EPSILON__ 1.08420217248550443401e-19L
// #define __LDBL_HAS_DENORM__ 1
// #define __LDBL_HAS_INFINITY__ 1
// #define __LDBL_HAS_QUIET_NAN__ 1
// #define __LDBL_MANT_DIG__ 64
// #define __LDBL_MAX_10_EXP__ 4932
// #define __LDBL_MAX_EXP__ 16384
// #define __LDBL_MAX__ 1.18973149535723176502e+4932L
// #define __LDBL_MIN_10_EXP__ (-4931)
// #define __LDBL_MIN_EXP__ (-16381)
// #define __LDBL_MIN__ 3.36210314311209350626e-4932L
// #define __LITTLE_ENDIAN__ 1
// #define __LONG_LONG_MAX__ 9223372036854775807LL
// #define __LONG_MAX__ 9223372036854775807L
// #define __LP64__ 1
// #define __MACH__ 1
// #define __MMX__ 1
// #define __NO_INLINE__ 1
// #define __NO_MATH_INLINES 1
// #define __ORDER_BIG_ENDIAN__ 4321
// #define __ORDER_LITTLE_ENDIAN__ 1234
// #define __ORDER_PDP_ENDIAN__ 3412
// #define __PIC__ 2
// #define __POINTER_WIDTH__ 64
// #define __PRAGMA_REDEFINE_EXTNAME 1
// #define __PTRDIFF_TYPE__ long int
// #define __PTRDIFF_WIDTH__ 64
// #define __REGISTER_PREFIX__
// #define __SCHAR_MAX__ 127
// #define __SHRT_MAX__ 32767
// #define __SIG_ATOMIC_WIDTH__ 32
// #define __SIZEOF_DOUBLE__ 8
// #define __SIZEOF_FLOAT__ 4
// #define __SIZEOF_INT128__ 16
// #define __SIZEOF_INT__ 4
// #define __SIZEOF_LONG_DOUBLE__ 16
// #define __SIZEOF_LONG_LONG__ 8
// #define __SIZEOF_LONG__ 8
// #define __SIZEOF_POINTER__ 8
// #define __SIZEOF_PTRDIFF_T__ 8
// #define __SIZEOF_SHORT__ 2
// #define __SIZEOF_SIZE_T__ 8
// #define __SIZEOF_WCHAR_T__ 4
// #define __SIZEOF_WINT_T__ 4
// #define __SIZE_TYPE__ long unsigned int
// #define __SIZE_WIDTH__ 64
// #define __SSE2_MATH__ 1
// #define __SSE2__ 1
// #define __SSE3__ 1
// #define __SSE_MATH__ 1
// #define __SSE__ 1
// #define __SSP__ 1
// #define __SSSE3__ 1
// #define __STDC_HOSTED__ 1
// #define __STDC_VERSION__ 199901L
// #define __STDC__ 1
// #define __UINTMAX_TYPE__ long unsigned int
// #define __USER_LABEL_PREFIX__ _
// #define __VERSION__ "4.2.1 Compatible Apple LLVM 4.2 (clang-425.0.28)"
// #define __WCHAR_MAX__ 2147483647
// #define __WCHAR_TYPE__ int
// #define __WCHAR_WIDTH__ 32
// #define __WINT_TYPE__ int
// #define __WINT_WIDTH__ 32
// #define __amd64 1
// #define __amd64__ 1
// #define __apple_build_version__ 4250028
// #define __block __attribute__((__blocks__(byref)))
// #define __clang__ 1
// #define __clang_major__ 4
// #define __clang_minor__ 2
// #define __clang_patchlevel__ 0
// #define __clang_version__ "4.2 (clang-425.0.28)"
// #define __core2 1
// #define __core2__ 1
// #define __llvm__ 1
// #define __pic__ 2
// #define __strong
// #define __tune_core2__ 1
// #define __unsafe_unretained
// #define __weak __attribute__((objc_gc(weak)))
// #define __x86_64 1
// #define __x86_64__ 1
//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
