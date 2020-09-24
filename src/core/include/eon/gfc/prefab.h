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
  *   \addtogroup io
  *   @{
  *     \addtogroup prefab
  *     @{
  */

//================================================|=============================
//Prefab:{                                        |

  namespace EON{

    namespace gfc{

      struct E_PUBLISH Prefab final:Stream{

        e_reflect_no_properties( Prefab, Stream );

        #define PREFAB_VERSION u16(1)

        //----------------------------------------|-----------------------------
        //Structs:{                               |

          class File{
            e_var_string(  Name );
            e_var( u64, u, Base ) = 0;
            e_var( u64, u, Size ) = 0;
            e_var_bits(    Flags
              , bAlwaysLoad:1
              , bStreaming:1
            );
          };

        //}:                                      |
        //Methods:{                               |
          //serialize:{                           |

          virtual void serialize( fs::Writer& )const override;
          virtual s64  serialize( fs::Reader& )override;

          //}:                                    |
          //get:{                                 |

            /** \brief Get all prefabs in the project.
              *
              * This function will return a vector of prefab handles
              * representing all the databases connected to a project or in the
              * project path given.
              *
              * \param path The path to the prefab directory.
              *
              * \return A vector of prefab handles.
              */

            static vector<handle> get( const string& path );

          //}:                                    |
        //}:                                      |
        //----------------------------------------|-----------------------------

        explicit Prefab( const string& name )
          : Stream( name )
        {}

        Prefab() = default;

      private:

        e_var( u64, u, Base ) = 0ULL;
        e_var_vector1( File );
      };
    }
  }

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
