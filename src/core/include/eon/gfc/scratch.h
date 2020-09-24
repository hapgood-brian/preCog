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
  *     \addtogroup memory
  *     @{
  */

//================================================|=============================
//Scratch:{                                       |

  namespace EON{

    namespace gfc{

      /** \brief Core temporary memory APIs.
        *
        * The scratch buffer is a simple round-robin buffer that you must
        * continually allocate from.  The largest allocation is limited to the
        * maximum size of the entire pool.  If the head alloc overwrites the
        * tail that data will be lost. Thus any use of alloc must be under the
        * assumption that the data won't survive very long.
        */

      struct E_PUBLISH Scratch final{

        //----------------------------------------|-----------------------------
        //Operate:{                               |

          e_forceinline char operator[]( const int i )const{
            return ccp( m_tBuffer.data() )[ i ];
          }

          e_forceinline char& operator[]( const int i ){
            return cp( m_tBuffer.data() )[ i ];
          }

        //}:                                      |
        //Methods:{                               |

          /** \brief Allocate n bytes from scratch memory.
            *
            * The scratch allocator grabs memory in a ring-buffer. As soon as
            * an allocation hits the end of the buffer it wraps on itself.
            * \param size The number of bytes to allocate.
            * \param alignment Power of two alignment of memory.
            * \param pad Power of two padding; chiefly used for cache line
            * alignment.
            */

          cp alloc( const u32 size, u32 alignment=0, const u32 pad=0 );

          /** \brief Returns the start of the scratch buffer.
            *
            * \return Returns buffer::data.
            */

          e_forceinline ccp begin()const{
            return ccp( m_tBuffer.data() );
          }

          e_forceinline cp begin(){
            return cp( m_tBuffer.data() );
          }

          /** \brief Returns the end of the scratch buffer (const).
            *
            * \return Returns offset._cp.
            */

          e_forceinline ccp end()const{
            return m_vOffset.m_cp;
          }

          /** \brief Returns the end of the scratch buffer.
            *
            * \return Returns offset._cp.
            */

          e_forceinline cp end(){
            return m_vOffset.m_cp;
          }

          /** \brief Reset end() to begin().
            */

          e_forceinline void reset(){
            m_vOffset.m_vp = m_tBuffer.data();
          }

          /** \brief Get the size of the scratch buffer currently.
            *
            * \return Returns the number of bytes allocated so far.
            */

          e_forceinline u32 used()const{
            return u32( m_vOffset.m_cp-cp( m_tBuffer.data() ));
          }

          /** \brief Gets the available byte count.
            *
            * This routine will return the number of bytes left in the scratch
            * buffer before a reset is required. \todo Add a global scratch
            * buffer in the IEngine interface that is wiped/reset once a frame.
            */

          e_forceinline u32 avail()const{
            return u32( m_tBuffer.size() - used() );
          }

          /** \brief Initialize the scratch buffer.
            *
            * This routine will initialize the scratch buffer with the pointer
            * and size provided. This is how donorship can occur.
            */

          void init( const u32 size );

        //}:                                      |
        //----------------------------------------|-----------------------------

        /** \brief Scratch buffer constructor.
          *
          * Constructor allocates the full size of the ring buffer.
          *
          * \param size Number of bytes to allocate without alignment.
          */

        explicit Scratch( const u32 size );
        Scratch();

      private:

        buffer m_tBuffer;
        union Variant{
          u64 m_u64;
          u32 m_u32;
          cp  m_cp;
          vp  m_vp;
        } m_vOffset;
      };
    }
  }

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
