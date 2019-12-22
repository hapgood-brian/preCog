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

#include<eon/eon.h>
#include<luacore.h>

using namespace EON;
using namespace gfc;
using namespace ai;

//------------------------------------------------|-----------------------------
//Strings:{                                       |
  //Workspace:{                                   |

    namespace{
      ccp kWorkspace =
        "local workspace=class'workspace'{\n"
        "  project = function(self,name )\n"
        "  end\n"
        "}\n"
      ;
    }

  //}:                                            |
  //Platform:{                                    |

    namespace{
      ccp kPlatform =
        "class'platform'{\n"
        "  is = function(self,name)\n"
        #if e_compiling( osx )
          "return name=='apple'\n"
        #elif e_compiling( microsoft )
          "return name=='win64'\n"
        #elif e_compiling( linux )
          "return name=='linux'\n"
        #else
          "return( name=='ios'or name=='android' )\n"
        #endif
        "  end\n"
        "}\n"
      ;
    }

  //}:                                            |
  //Target:{                                      |

    namespace{
      ccp kTarget =
        "class'Target'{\n"
        "}\n"
      ;
    }

  //}:                                            |
  //Build:{                                       |

    namespace{
      ccp kBuild =
        "class'Build'{\n"
        "}\n"
      ;
    }

  //}:                                            |
//}:                                              |
//Methods:{                                       |

  int IEngine::main( const strings& args ){
    if( args.size() == 1 ){
      e_msgf( "Juggle build system" );
      e_msgf( "\tUsage juggle cakefile.lua");
      return 0;
    }
    Lua::handle hLua = e_new( Lua );
    const auto& st = e_fload( args[ 1 ]);
    if( st.empty() ){
      return-1;
    }
    st.query(
      [&]( ccp s ){
        auto& lua = hLua.cast();
        lua.sandbox( kWorkspace );
        lua.sandbox( kPlatform );
        lua.sandbox( kTarget );
        lua.sandbox( kBuild );
        lua.sandbox( s );
      }
    );
    return-1;
  }

//}:                                              |
//------------------------------------------------|-----------------------------
