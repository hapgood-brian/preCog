//------------------------------------------------------------------------------
//                   Copyright Lelu, Inc. All rights reserved.
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

#include<eon/eon.h>

#ifdef require
#undef require
#endif

/** \addtogroup language
  * @{
  */

//================================================|=============================
//Struct:{                                        |

  struct lua_State;
  struct luaL_Reg;

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Lua:{                                           |

  namespace EON{

    namespace gfc{

      /** \brief Game Lua engine.
        *
        * This structure encapsulates all functionality relating to Lua
        * scripting in the engine.
        */

      struct Lua:Object{

        e_reflect_no_properties( Lua, Object );

        //========================================|=============================
        //Methods:{                               |
          //Constructs:{                          |

            /** \brief Classify an engine class.
              *
              * This routine will generate a new Lua "class" metatable as if
              * "class" was called from Lua. The normal new() function in Lua
              * can be used to instantiate the new metatable.
              *
              * \param pTypename The name of the new class in Lua. A table will
              * be created in the sandbox with this name with a meta-table that
              * allows one to add methods, data and call a shortcut to
              * instantiation.
              *
              * \param pMethods An anonymous pointer to an array of luaL_Reg
              * structures that the code will load into the new class/table.
              */

            void classify( ccp pTypename, const luaL_Reg* pMethods );

            /** \brief Get field always.
              *
              * This routine will examine the table on the stack and look for a
              * field named by pField. If one exists it is returned otherwise a
              * table of that name will be created and returned on the stack.
              \code
                lua_getfield( L, -1, pField );
                if( lua_isnil( L, -1 )){
                  lua_pop( L, 1 );
                  lua_createtable( L, 0, 0 );
                  lua_pushvalue( L, -1 );
                  lua_setfield( L, -3, pField );
                }
              \endcode
              * \param L The lua state to use.
              * \param pField The name of the field to return. Expect it is a
              * table because if the field is missing a table of that name will
              * be created.
              */

            static bool getFieldAlways( lua_State* L, ccp pField );

            /** \brief Load and sandbox a Lua module.
              *
              * This routine is very much like sandbox() in that it sandboxes a
              * chunk of script or bytecode. The difference lies in where it
              * gets its data from, in this case off disk. The require keyword
              * is a standard part of the Lua language. Where standard Lua and
              * Lua::require diverge however is what the function considers a
              * valid file path. For a game on iOS and Mac OSX it is strictly
              * from the bundle's Resource folder.  A game on Windows it is the
              * read-only folder in "Program Files". On Android it is the
              * read-only APK.
              *
              * \param pFilename The name of the Lua file relative to the
              * platform specific resource folder.
              */

            void require( ccp pFilename );

            /** \brief Sandbox a new script.
              *
              * This routine will load a new chunk of Lua code into the sandbox
              * and run it immediately. A sandboxed chunk is like a file unto
              * itself so the local will work properly in file scope.
              *
              * \param pChunk A chunk of Lua code in text or bytecode format
              * that the the scripting system will execute.
              */

            bool sandbox( ccp pChunk );

            /** \brief Sandbox a new script.
              *
              * This routine will load a new chunk of Lua code into the sandbox
              * and run it immediately. A sandboxed chunk is like a file unto
              * itself so the local will work properly in file scope.
              */

            static bool sandbox( lua_State* L, ccp pScript );

          //}:                                    |
          //Sessions:{                            |

            /** \brief Object allocator.
              *
              * This routine allocates Lua objects in the engine heap.
              */

            static vp allocate( vp ud, vp ptr, size_t osize, size_t nsize );

            /** \brief Get UUID for Lua state.
              *
              * This routine will return the 64-bit UUID for the Lua::handle
              * associated with the given lua_State* pointer.
              */

            static s64 uuid( lua_State* );

            /** \brief Initialise the Lua engine.
              *
              * This routine will initialise the Lua engine by creating the
              * sandbox and connecting all secure Lua language features.
              */

            void initialise();

            /** \brief Destroy the Lua engine.
              *
              * This routine will tear down the Lua engine and destroy the
              * sandbox.  All data will be instantly obliterated.
              */

            void destroy();

            /** \brief The game entry point.
              *
              * This routine will start running the game logic. It looks for a
              * global function in the sandbox called main() and calls it.
              */

            void main();

            /** \brief The game exit wound.
              *
              * This routine will stop running the game logic. It is the
              * responsibility of the Lua exit() function to release all
              * resources and handles it might own back to the engine. The
              * engine expects the system to be restored to the same state
              * (memory wise) that it was in before Lua main() was called. This
              * is critical behavior. Any memory leaks will prevent a title
              * from safely transitioning between scenes.
              */

            void exit();

          //}:                                    |
          //Globals:{                             |
            //unref:{                             |

              void unref( int )const;

            //}:                                  |
            //find:{                              |

              //remember to luaL_unref() return values.
              int findInGlobal( ccp, ccp )const;
              int findInTable( int, ccp )const;
              int findInSandbox( ccp )const;
              int find( int, ccp )const;

            //}:                                  |
            //call:{                              |

              void call( const int ref, const int run, const f32& value );
              void call( const int ref, const int run, const s64 value );
              void call( const int ref, const int run );
              void call( const int run, const f32& value );
              void call( const int run, const s64 value );
              void call( const int run );

            //}:                                  |
            //set*:{                              |

              void set( ccp key, const string& value );
              void set( ccp key, ccp value );
              void set( ccp key, s64 value );
              void set( ccp key, f32 value );

            //}:                                  |
            //to*:{                               |

              string toString( ccp key )const;
              s64 toInteger( ccp key )const;
              ccp toCstring( ccp key )const;
              f32 toFloat( ccp key )const;

              e_forceinline lua_State* toL()const{
                return L;
              }

            //}:                                  |
          //}:                                    |
          //API:{                                 |

            static int collectgarbage( lua_State* );
            static int setmetatable  ( lua_State* );
            static int getmetatable  ( lua_State* );
            static int type          ( lua_State* );
            static int require       ( lua_State* );
            static int loadstring    ( lua_State* );
            static int error         ( lua_State* );
            static int print         ( lua_State* );
            static int tostring      ( lua_State* );
            static int select        ( lua_State* );
            static int next          ( lua_State* );
            static int pairs         ( lua_State* );

          //}:                                    |
        //}:                                      |
        //========================================|=============================

        Lua(){
          initialise();
        }

        virtual~Lua(){
          destroy();
        }

      private:

        lua_State* L = nullptr;

      public:

        static hashmap<u64,string> D;
      };
    }
  }

//}:                                              |
//================================================|=============================

/** @} */
