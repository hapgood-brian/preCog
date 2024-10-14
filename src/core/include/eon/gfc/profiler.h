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
//BlockProfiler:{                                 |

  // If engine profiling enabled defines structures and e_profile.
  #if e_compiling( profiler )
    namespace EON{
      namespace gfc{
        struct E_PUBLISH BlockProfiler final{
          BlockProfiler( ccp, ccp, ... );
        ~ BlockProfiler();
        private:
          struct data{
            vector<data> m_vChildren;
            vector<data> m_vCallees;
            double       m_fStart = 0.;
            double       m_fStop  = 0.;
            string       m_sInfo;
          };
          static void printInfo( const data& d, const s32 tabs );
          static bool enabled();
          ccp            m_pFunction = nullptr;
          BlockProfiler* m_pParent   = nullptr;
          data*          m_pData     = nullptr;
          u8 m_aData[ sizeof( data )];
        };
      }
    }
    #define e_profile( format,... ) \
      ::EON::gfc::BlockProfiler e_distinct(__profile)(__PRETTY_FUNCTION__, format, ##__VA_ARGS__ )
  #else
    #define e_profile( format,... )
  #endif

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
