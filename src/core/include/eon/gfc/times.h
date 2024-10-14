//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup time
  *     @{
  */

//================================================+=============================
//Time functions:{                                |

  /** \brief Get time in nanoseconds.
    *
    * Uses the kernel's timing functions to return the number of nanoseconds
    * that have passed since the seventies!
    *
    * \return A 64-bit real number representing the number of nanoseconds
    * passed.
    */

  E_PUBLISH double e_nanoseconds();

  /** \brief Get time in microseconds.
    *
    * Uses the kernel's timing functions to return the number of microseconds
    * that have passed since the seventies!
    *
    * \return A 64-bit real number representing the number of microseconds
    * passed.
    */

  E_PUBLISH double e_microseconds();

  /** \brief Get time in milliseconds.
    *
    * Uses the kernel's timing functions to return the number of milliseconds
    * that have passed since the seventies!
    *
    * \return A 64-bit real number representing the number of milliseconds
    * passed.
    */

  E_PUBLISH double e_milliseconds();

  /** \brief Get time in seconds.
    *
    * Uses the kernel's timing functions to return the number of seconds
    * that have passed since the seventies!
    *
    * \return A 64-bit real number representing the number of seconds
    * passed.
    */

  E_PUBLISH double e_seconds();

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Constants:{                                     |

  namespace EON{

    namespace gfc{

      enum class Scale:u32{
        nanosecond,
        microsecond,
        millisecond,
        second,
        minute,
        hour,
        day,
        week,
        year,
      };

      template<Scale T> struct Time{
        E_PUBLISH static double nanoseconds ( const double x=1. ); //!< Ts in nanoseconds.
        E_PUBLISH static double microseconds( const double x=1. ); //!< Ts in microseconds.
        E_PUBLISH static double milliseconds( const double x=1. ); //!< Ts in milliseconds.
        E_PUBLISH static double seconds     ( const double x=1. ); //!< Ts in seconds.
        E_PUBLISH static double minutes     ( const double x=1. ); //!< Ts in minutes.
        E_PUBLISH static double hours       ( const double x=1. ); //!< Ts in hours.
        E_PUBLISH static double days        ( const double x=1. ); //!< Ts in days.
        E_PUBLISH static double weeks       ( const double x=1. ); //!< Ts in weeks.
        E_PUBLISH static double years       ( const double x=1. ); //!< Ts in years.
      };
    }
  }

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
