//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup io
  *   @{
  *     \addtogroup fablet
  *     @{
  */

//================================================+=============================
//Prefab:{                                        |

  namespace EON{

    namespace gfc{

      struct E_PUBLISH Prefab final:Stream{

        e_reflect_no_properties( Prefab, Stream );

        #define PREFAB_VERSION u16(1)

        //----------------------------------------+-----------------------------
        //Structs:{                               |

          struct File:Object{
            e_reflect_no_properties( File, Object );
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

            /** \brief Get all fablets in the project.
              *
              * This function will return a vector of fablet handles
              * representing all the databases connected to a project or in the
              * project path given.
              *
              * \param path The path to the fablet directory.
              *
              * \return A vector of fablet handles.
              */

            static vector<handle> get( const string& path );

          //}:                                    |
        //}:                                      |
        //----------------------------------------+-----------------------------

        explicit Prefab( const string& name )
          : Stream( name )
        {}

        Prefab() = default;

      private:

        e_var( u64, u, Base ) = 0ULL;
        e_var_handle_vector1( File );
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//API:{                                           |

  /** \brief Unpackage a directory.
    *
    * This routine will unpackage a directory on disk and return a fablet for
    * you to save to disk.
    *
    * \param path The directory to write to. If the folder doesn't exist one
    * will be created.
    *
    * \return Returns true if the fablet was unpackaged and false otherwise.
    * Usually the return value is false if the path is incorrect/bad.
    */

  bool e_unpackage( const EON::gfc::string& path );

  /** \brief Package up a directory.
    *
    * This routine will package up a number of files and directories on disk
    * and return a fablet for you to save to disk. The files are always
    * specified in Unix terms, even on Windows, and will be converted to
    * Windows paths via the String::os() call later.
    *
    * \param filesAndDirs A vector of path strings containing files and
    * directories to scan and include in the final package.
    *
    * \param packageName The name of the package to create.
    *
    * \return Returns true if the package was written (a .fablet using EON's
    * nomenclature). Returns false if it wasn't.
    */

  bool e_package(
      const EON::gfc::strings& filesAndDirs
    , const EON::gfc::string& packageName );

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
