//------------------------------------------------------------------------------
//       Copyright 2014-2020 Creepy Doll Games LLC. All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
// NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup time
  *     @{
  */

//================================================|=============================
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
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
