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

//================================================+=============================
//Memory:{                                        |

  #ifdef __cplusplus

    /** \name Aligned allocations.
      *
      * These engine functions let you allocate and free aligned memory crosss
      * platform.
      *
      * @{
      */

    /** \brief Aligned allocate.
      *
      * This routine will allocate a chunk of memory aligned to a particular
      * size and return a pointer to it.
      *
      * \param size The number of bytes to allocate.
      * \param align The alignment for the new block.
      *
      * \return Returns a pointer to the new aligned memory.
      */

    E_PUBLISH EON::vp e_amalloc( const size_t size, const size_t align );

    /** \brief Free an aligned allocation.
      *
      * This routine will free the memory previously allocated with e_amalloc.
      *
      * \param ptr The pointer to free up.
      */

    E_PUBLISH void e_afree( EON::cvp ptr );

    /** \brief Get CPU page size.
      *
      * This routine will return the size of a CPU virtual page in bytes.
      *
      * \return Returns the number of bytes in a page.
      */

    E_PUBLISH EON::u32 e_pagesize();

    /** @}
      *
      * \name Scratch memory allocation.
      *
      * These engine functions let you allocate memory from the current
      * thread's scratch memory pool. If you run out of memory the base pointer
      * will wrap.
      *
      * @{
      */

    /** \brief Test if memory in scratch memory.
      *
      * This routine will check if the pointer provided is in the scratch
      * buffer.
      */

    E_PUBLISH bool e_scratched( EON::ccp ptr );

    /** \brief Scratch memory allocator.
      *
      * This routine will allocate size bytes of memory on the scratch heap.
      * This heap is of a small fixed size and cannot be reallocated. You must
      * call e_sreset at the end of every frame. Thankfully this happens
      * automatically for you in the engine View class.
      */

    E_PUBLISH EON::cp e_salloc( const EON::u64 size );

    /** \brief Return scratch base pointer.
      *
      * This routine will return the base pointer to the scratch buffer. This
      * is useful if you want to obtain the e_msize of the buffer etc or
      * reallocate it.
      */

    E_PUBLISH EON::ccp e_scratch();

    /** \brief Scratch memory reset pointer.
      *
      * This routine will reset the memory pointer to the base of the scratch
      * buffer. It should be called once per frame, which it is from the
      * hardware View buffer.
      */

    E_PUBLISH void e_sreset();

    /** @}
      *
      * \name General memory allocation.
      *
      * These engine functions let you allocate and free memory from the
      * primary heap.
      *
      * @{
      */

    /** \brief Memory reallocator.
      *
      * This routine uses the header block to reallocate a block of aligned and
      * padded memory. This information is obtained from the memory header. The
      * file and line will only be modified if setdebuginfo is called.
      *
      * \param ptr The pointer to reallocate.
      * \param newsize The size of the new allocation.
      *
      * \return Returns a pointer to the optionally aligned and padded pointer.
      * It will keep the attributes of the original allocation.
      */

    E_PUBLISH EON::cp e_realloc( EON::cvp ptr, const size_t newsize );

    /** \brief Memory allocator.
      *
      * This is the engine memory allocator. It is meant for fast creation of
      * blocks of aligned, cache-friendly memory. All details of the allocation
      * will be saved in a header block including file, line and function for
      * debug builds. Every allocation will be logged.
      *
      * \param size The number of bytes to allocate.
      *
      * \return Return a pointer to the new memory block.
      */

    E_PUBLISH EON::cp e_malloc( const size_t size );

    /** \brief Memory deallocator.
      *
      * This routine will obliterate an old block of memory and return it to
      * the heap.
      *
      * \param ptr The memory block to obliterate.
      */

    E_PUBLISH void e_free( EON::cvp ptr );

    /** @}
      *
      * \name Memory utility functions.
      *
      * These engine functions let you randomize memory and other things.
      *
      * @{
      */

    /** \brief Memset randomly.
      *
      * This routine will memset a block of memory with random values.
      *
      * \param ptr A pointer to a writable block of memory.
      * \param size The number of bytes in the block.
      */

    E_PUBLISH void e_memrand( EON::cp ptr, const size_t size );

    /** \brief Memory validator.
      *
      * This routine will validate all allocated blocks in the process.
      */

    E_PUBLISH void e_memory_validator();

    /** @}
      *
      * \name Memory sanity functions.
      *
      * These functions let you determine the validity of a memory block in a
      * cross platform way.
      *
      * @{
      */

    /** \brief Check memory pointer!
      *
      * This routine will verify whether the given pointer is valid or not.
      */

    E_PUBLISH bool e_isbad( EON::cvp ptr );

    /** @} */

  #endif

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//e_memory_validator:{                            |

  #ifdef __cplusplus

    struct MemoryValidator final{
      MemoryValidator(){
        e_memory_validator();
      }
    ~ MemoryValidator(){
        e_memory_validator();
      }
    };

    #define e_memory_validator()                                                \
      MemoryValidator e_distinct(__memory_validator)                            \

  #endif

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
