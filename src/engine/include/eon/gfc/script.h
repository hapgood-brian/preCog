//------------------------------------------------------------------------------
//       Copyright 2014-2019 Creepy Doll Games LLC. All rights reserved.
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
  *     \addtogroup scripting
  *     @{
  */

//================================================|=============================
//Script:{                                        |

  namespace EON{

    namespace gfc{

      /** \brief Scripting object with grammar.
        *
        * This class is designed to facilitate the construction of grammars and
        * to use those grammars to parse and tokenize a source file in any
        * language.  This is the principal mechanism behind Swordlight's
        * ability to convert between any language and an EON block programming
        * language (BPL).
        */

      struct E_PUBLISH Script final:Resource{

        e_reflect_no_properties( Script, Resource );

        //----------------------------------------|-----------------------------
        //Structs:{                               |

          /** \brief Grammar resource.
            *
            * This resource is designed to encapsulate a language definition.
            * It uses a very simplisitic assembly syntax of tokens for
            * characters, whitepace, numbers and the like. STL regex are
            * supported everywhere.
            *
            * Syntax is made up of rules. Rules contain phrases which are
            * regexes.  For example, to define a function one would use the
            * following:
            *
            *   grammar main{
            *     integer{
            *       (\+|-)?[[:digit:]]+
            *     }->{
            *     }
            *   }
            */

          struct Grammar final:Resource{

            e_reflect_no_properties( Grammar, Resource );

            //------------------------------------|-----------------------------
            //Structs:{                           |
              //Rule:{                            |

                class Rule final{
                  e_var_string(  Name    );
                  e_var_string(  Pattern );
                  e_var_string(  Regex   );
                  e_var( u32, u, Token   ) = 0;
                };

              //}:                                |
            //}:                                  |
            //Methods:{                           |
              //set:{                             |

                void set( const string& source );

              //}:                                |
            //}:                                  |
            //------------------------------------|-----------------------------

            Grammar( const string& name )
              : Resource( name )
            {}

            virtual~Grammar() = default;
            Grammar() = default;

          private:

            e_var_string_map( Rule, Regex );
          };

        //}:                                      |
        //Methods:{                               |

          void parse( const string& rule, const string& text );

        //}:                                      |
        //----------------------------------------|-----------------------------

        explicit Script( const Grammar::handle& hGrammar )
          : m_hGrammar( hGrammar )
        {}

        virtual~Script() = default;
        Script() = default;

      private:

        virtual void onMatch( const string& ){}

        e_var_handle1( Grammar );
      };
    }
  }

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
