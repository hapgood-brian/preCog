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
#include<ws.h>

using namespace EON;
using namespace gfc;

//------------------------------------------------|-----------------------------
//Private:{                                       |
  //Workspace:{                                   |

    namespace{
      ccp kWorkspace = "local workspace=class'workspace'{\n"
        //----------------------------------------|-----------------------------
        //new:{                                   |

          "  new = function(self,name)\n"
          "    return class'workspace'{\n"
          "      m_tProjects = {},\n"
          "      m_sName = name,\n"
          "      new = function(self,label)\n"
          "        local t=class'project'{\n"
          "          target = function(self,build)\n"
          "            self.m_build = build\n"
          "            return self\n"
          "          end,\n"
          "          harden = function(self,hardenedRuntime)\n"
          "            self.m_hardenedRuntime = hardenedRuntime\n"
          "            return self\n"
          "          end,\n"
          "          install = function(self,script)\n"
          "            self.m_installScript = script\n"
          "            return self\n"
          "          end,\n"
          "          defines = function(self,dbg,rel)\n"
          "            self.m_definesRel = rel\n"
          "            self.m_definesDbg = dbg\n"
          "            return self\n"
          "          end,\n"
          "          link_with = function(self,libs)\n"
          "            self.m_linkWith = libs\n"
          "            return self\n"
          "          end,\n"
          "          skip_unity = function(self,files)\n"
          "            self.m_skipUnity = files\n"
          "            return self\n"
          "          end,\n"
          "          disable = function(self,options)\n"
          "            self.m_disableOpts = options\n"
          "            return self\n"
          "          end,\n"
          "          export_headers = function(self,dirsAndFiles)\n"
          "            self.m_exportHeaders = dirsAndFiles\n"
          "            return self\n"
          "          end,\n"
          "          set_include_paths = function(self,paths)\n"
          "            self.m_includePaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          deployment = function(self,version)\n"
          "            self.m_deployTo = version\n"
          "            return self\n"
          "          end,\n"
          "          find_resources = function(self,paths)\n"
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
          "          set_plist_path = function(self,path)\n"
          "            self.m_plistPath = path\n"
          "            return self\n"
          "          end,\n"
          "          find_libraries = function(self,paths)\n"
          "            self.m_libraryPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          find_frameworks = function(self,paths)\n"
          "            self.m_frameworkPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          find_sources = function(self,paths)\n"
          "            self.m_srcPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          find_includes = function(self,paths)\n"
          "            self.m_incPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          ignore = function(self,regex)\n"
          "            self.m_ignore = regex\n"
          "            return self\n"
          "          end,\n"
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

          "save = function(self)\n"
          "  out.save(out.generate(self),'tmp')\n"
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
              "  return nil\n"
            #endif
          "end,\n"

        //}:                                      |
        //----------------------------------------|-----------------------------
      "}\n";
    }

  //}:                                            |
  //Globals:{                                     |

    Workspace::Flags Workspace::bmp;

  //}:                                            |
//}:                                              |
//Methods:{                                       |

  namespace{
    int generate( const string& cgf ){
      Lua::handle hLua = e_new( Lua );
      const auto& st = e_fload( cgf );
      if( st.empty() ){
        return-1;
      }
      st.query(
        [&]( ccp pBuffer ){
          auto& lua = hLua.cast();
          string sBuffer( pBuffer );
          sBuffer.replace( "${RELEASE}", "release" );
          sBuffer.replace( "${DEBUG}", "debug" );
          lua.sandbox( kWorkspace );
          lua.sandbox( kPlatform );
          #if e_compiling( osx )
            sBuffer.replace( "${PLATFORM}", "osx" );
          #elif e_compiling( linux )
            sBuffer.replace( "${PLATFORM}", "linux" );
          #elif e_compiling( microsoft )
            sBuffer.replace( "${PLATFORM}", "win" );
          #endif
          string target = "local options = {";
          if( Workspace::bmp->bXcode11 ){
            target << "\n  xcode11 = true,";
          }else{
            target << "\n  xcode11 = false,";
          }
          if( Workspace::bmp->bVS2019 ){
            target << "\n  vs2019 = true,";
          }else{
            target << "\n  vs2019 = false,";
          }
          target << "\n}\n";
          lua.sandbox( target + sBuffer );
        }
      );
      e_msgf( "ok" );
      return 0;
    }
  }

//}:                                              |
//------------------------------------------------|-----------------------------

int IEngine::main( const strings& args ){
  e_msgf( "Cog build system v1.2.0" );
  #if e_compiling( osx )
    Workspace::bmp->bXcode11 = 1;
  #elif e_compiling( microsoft )
    Workspace::bmp->bVS2019 = 1;
  #endif
  auto it = args.getIterator()+1;
  while( it ){
    switch( **it ){
      case'-':
        e_msgf( "Detected switch %s", ccp( *it ));
        if( it->hash() == e_hashstr64_const( "--unity" )){
          Workspace::bmp->bUnity = 1;
          break;
        }
        if( it->hash() == e_hashstr64_const( "--vs2019" )){
          Workspace::bmp->bVS2019 = 1;
          break;
        }
        if( it->hash() == e_hashstr64_const( "--xcode11" )){
          Workspace::bmp->bXcode11 = 1;
          break;
        }
        if( it->hash() == e_hashstr64_const( "--help" )){
          e_msgf( "Help coming soon..." );
          return 0;
        }
        break;
      default:
        return generate( *it );
    }
    ++it;
  }
  if( !fexists( "cogfile.lua" )){
    e_msgf( "  Usage cog [cogfile.lua]" );
  }else{
    return generate( "cogfile.lua" );
  }
  return 0;
}
