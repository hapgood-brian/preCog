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

#include<generators.h>
#include<luacore.h>
#include<std.h>

using namespace EON;
using namespace gfc;
using namespace ai;

//================================================|=============================
//Actions:{                                       |
  //[properties]:{                                |
    //onGenerate:{                                |

#ifdef __APPLE__
  #pragma mark - Action -
#endif

      namespace{
        // boolean = e_generate( workspace_table );
        s32 onGenerate( lua_State* L ){
          lua_pop( L, 1 );
          return 0;
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//Tablefu:{                                       |

  luaL_Reg generators[2]={
    //Generators (1):{                            |

      {"generate", onGenerate },

    //}:                                          |
    //END        (1):{                            |

      {0,0}

    //}:                                          |
  };

//}:                                              |
//================================================|=============================
