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
  *     \addtogroup macros
  *     @{
  */

//================================================+=============================
//Release macros:{                                |

  #if !e_compiling( debug )
    #define __compiling_release__ 1
    #define __compiling_sanity__  0
  #endif

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
