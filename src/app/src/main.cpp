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

#include<luacore.h>

using namespace EON;
using namespace gfc;
using namespace ai;

//------------------------------------------------|-----------------------------
//Strings:{                                       |
  //Workspace:{                                   |

    namespace{
      ccp kWorkspace = "local workspace=class'workspace'{\n"
        //----------------------------------------|-----------------------------
        //new:{                                   |

          "  new = function(self,name)\n"
          "    return class'workspace'{\n"
          "      m_tProjects = {},\n"
          "      m_typeId = 'xml',\n"
          "      m_sName = name,\n"
          "      new = function(self,label)\n"
          "        local t=class'project'{\n"
          "          target = function(self,build)\n"
          "            self.m_build = build\n"
          "            return self\n"
          "          end,\n"
          "          defines = function(self,dbg,rel)\n"
          "            self.m_definesRel = rel\n"
          "            self.m_definesDbg = dbg\n"
          "            return self\n"
          "          end,\n"
          "          link_with = function(self,libs)\n"
          "            self.m_frameworkLibs = libs\n"
          "            return self\n"
          "          end,\n"
          "          export_headers = function(self,dirsAndFiles)\n"
          "            self.m_exportHeaders = dirsAndFiles\n"
          "            return self\n"
          "          end,\n"
          "          set_include_path = function(self,paths)\n"
          "            self.m_includePaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          deployment = function(self,version)\n"
          "            self.m_deployTo = version\n"
          "            return self\n"
          "          end,\n"
          "          find_res = function(self,paths)\n"
          "            self.m_resPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          organization = function(self,name)\n"
          "            self.m_orgName = name\n"
          "            return self\n"
          "          end,\n"
          "          identifier = function(self,id)\n"
          "            self.m_bundleId = id\n"
          "            return self\n"
          "          end,\n"
          "          team = function(self,name)\n"
          "            self.m_teamName = name\n"
          "            return self\n"
          "          end,\n"
          "          prefix = function(self,header)\n"
          "            self.m_prefixHeader = header\n"
          "            return self\n"
          "          end,\n"
          "          lang = function(self,lang)\n"
          "            self.m_language = lang\n"
          "            return self\n"
          "          end,\n"
          "          find_source = function(self,paths)\n"
          "            self.m_srcPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          find_include = function(self,paths)\n"
          "            self.m_incPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          ignore = function(self,regex)\n"
          "            self.m_ignore = regex\n"
          "            return self\n"
          "          end,\n"
          "          m_typeId = 'pbx',\n"
          "          m_iBuild = build,\n"
          "        }\n"
          "        self.m_tProjects[label]=t\n"
          "        return t\n"
          "      end,\n"
          "    }\n"
          "  end,\n"

        //}:                                      |
        //----------------------------------------|-----------------------------
      "}\n";
    }

  //}:                                            |
  //Platform:{                                    |

    namespace{
      ccp kPlatform = "local platform=class'platform'{\n"
        //----------------------------------------|-----------------------------
        //save:{                                  |

          "save = function(self,path)\n"
          "  out.save(out.generate(self),path)\n"
          "end,\n"

        //}:                                      |
        //is:{                                    |

          "is = function(name)\n"
            #if e_compiling( osx )
              "  return name=='apple'\n"
            #elif e_compiling( microsoft )
              "  return name=='win64' or name=='microsoft'\n"
            #elif e_compiling( linux )
              "  return name=='linux'\n"
            #else
              "  return( name=='ios'or name=='android' )\n"
            #endif
          "end,\n"

        //}:                                      |
        //----------------------------------------|-----------------------------
      "}\n";
    }

  //}:                                            |
//}:                                              |
//Methods:{                                       |

  int IEngine::main( const strings& args ){
    e_msgf( "Cog build system:" );
    if( args.size() == 1 ){
      e_msgf( "  Usage cog [cogfile.lua]");
      return 0;
    }
    Lua::handle hLua = e_new( Lua );
    const auto& st = e_fload( args[ 1 ]);
    if( st.empty() ){
      return-1;
    }
    st.query(
      [&]( ccp pBuffer ){
        auto& lua = hLua.cast();
        string sBuffer( pBuffer );
        sBuffer.replace( "${RELEASE}", "'release'" );
        sBuffer.replace( "${DEBUG}", "'debug'" );
        lua.sandbox( kWorkspace );
        lua.sandbox( kPlatform );
        #if e_compiling( osx )
          sBuffer.replace( "${PLATFORM}", "osx" );
        #elif e_compiling( ios )
          sBuffer.replace( "${PLATFORM}", "ios" );
        #elif e_compiling( emscripten )
          sBuffer.replace( "${PLATFORM}", "web" );
        #elif e_compiling( android )
          sBuffer.replace( "${PLATFORM}", "android" );
        #elif e_compiling( linux )
          sBuffer.replace( "${PLATFORM}", "linux" );
        #elif e_compiling( microsoft )
          sBuffer.replace( "${PLATFORM}", "win" );
        #endif
        lua.sandbox( sBuffer );
      }
    );
    return-1;
  }

//}:                                              |
//------------------------------------------------|-----------------------------
