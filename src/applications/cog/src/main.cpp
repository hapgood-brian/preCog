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
using namespace fs;

//------------------------------------------------|-----------------------------
//Actions:{                                       |
  //onUnpackage:{                                 |

    namespace{
      void onUnpackage( const string& path ){
        e_unpackage( path );
      }
    }

  //}:                                            |
  //onPackage:{                                   |

    namespace{
      void onPackage( strings::const_iterator& it, const string& pkgName ){
        strings filesAndDirs;
        while( it ){
          filesAndDirs += *it;
          ++it;
        }
        e_package(
            filesAndDirs
          , pkgName
        );
      }
    }

  //}:                                            |
//}:                                              |
//Private:{                                       |
  //[workspace]:{                                 |

    namespace{
      constexpr ccp kWorkspace = "local workspace=class'workspace'{\n"
        //----------------------------------------|-----------------------------
        //new:{                                   |

          "  new = function(self,name)\n"
          "    return class'workspace'{\n"
          "      m_tProjects = {},\n"
          "      m_sName = name,\n"
          "      new = function(self,label)\n"
          "        local t=class'project'{\n"
          //--------------------------------------|-----------------------------
          //Microsoft:{                           |
          #if e_compiling( microsoft )
          "          winsdk = function(self,version)\n"
          "            self.m_winsdk = version\n"
          "            return self\n"
          "          end,\n"
          "          def = function(self,path)\n"
          "            self.m_def = path\n"
          "            return self\n"
          "          end,\n"
          #endif
          //}:                                    |
          //Common:{                              |
          "          target = function(self,build)\n"
          "            self.m_build = build\n"
          "            return self\n"
          "          end,\n"
          "          install = function(self,script)\n"
          "            self.m_installScript = script\n"
          "            return self\n"
          "          end,\n"
          "          defines = function(self,dbg,rev)\n"
          "            self.m_definesRel = rev\n"
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
          "          set_include_paths = function(self,paths)\n"
          "            self.m_includePaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          find_resources = function(self,paths)\n"
          "            self.m_resPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          find_libraries = function(self,paths)\n"
          "            self.m_libraryPaths = paths\n"
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
          "          prefix = function(self,header)\n"
          "            self.m_prefixHeader = header\n"
          "            return self\n"
          "          end,\n"
          //}:                                    |
          //Apple:{                               |
          #if e_compiling( osx )
          "          harden = function(self,hardenedRuntime)\n"
          "            self.m_hardenedRuntime = hardenedRuntime\n"
          "            return self\n"
          "          end,\n"
          "          deployment = function(self,version)\n"
          "            self.m_deployTo = version\n"
          "            return self\n"
          "          end,\n"
          "          enableARC = function(self,enable)\n"
          "            self.m_arcEnabled = enable\n"
          "            return self\n"
          "          end,\n"
          "          export_headers = function(self,dirsAndFiles)\n"
          "            self.m_exportHeaders = dirsAndFiles\n"
          "            return self\n"
          "          end,\n"
          "          export_refs = function(self,dirsAndFiles)\n"
          "            self.m_exportRefs = dirsAndFiles\n"
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
          "          lang = function(self,lang)\n"
          "            self.m_language = lang\n"
          "            return self\n"
          "          end,\n"
          "          set_plist_path = function(self,path)\n"
          "            self.m_plistPath = path\n"
          "            return self\n"
          "          end,\n"
          "          find_frameworks = function(self,paths)\n"
          "            self.m_frameworkPaths = paths\n"
          "            return self\n"
          "          end,\n"
          "          os_target = function(self,osTarget)\n"
          "            self.m_osTarget = osTarget\n"
          "            return self\n"
          "          end,\n"
          #endif
          //}:                                    |
          //--------------------------------------|-----------------------------
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
  //[platform]:{                                  |

    namespace{

      //------------------------------------------------------------------------
      // Get the platform name; for the user to query the build name.
      //------------------------------------------------------------------------

      string getPlatformName(){
        if( Workspace::bmp->bEmscripten ){
          return"  return'wasm'";
        }
        if( Workspace::bmp->bQmake ){
          return"  return'qmake'";
        }
        #if e_compiling( osx )
          return"  return'macos'";
        #elif e_compiling( microsoft )
          return"  return'win64'";
        #elif e_compiling( linux )
          return"  return'linux'";
        #endif
      }

      //------------------------------------------------------------------------
      // Get the platform class; for the user to query the platform.
      //------------------------------------------------------------------------

      string platformClass(){
        string out;
        out << "local platform=class'platform'{\n";

        //----------------------------------------|-----------------------------
        //builds:{                                |

          out << "builds=function()\n";
          out << "  return{ getBuildSystems() }\n";
          out << "end,\n";

        //}:                                      |
        //vendor:{                                |

          out << "vendor = function()\n";
          #if e_compiling( osx )
            out << "  return'apple'\n";
          #elif e_compiling( microsoft )
            out << "  return'microsoft'\n";
          #elif e_compiling( linux )
            out << "  return'community'\n";
          #endif
          out << "end,\n";

        //}:                                      |
        //save:{                                  |

          out << "save = function(self)\n";
          out << "  out.save(out.generate(self),'tmp')\n";
          out << "end,\n";

        //}:                                      |
        //name:{                                  |

          out << "name = function()\n";
          out << "  " << getPlatformName() << "\n";
          out << "end,\n";

        //}:                                      |
        //is:{                                    |

          out << "is = function(name)\n";
          #if e_compiling( osx )
            out << "  return name=='apple'\n";
          #elif e_compiling( microsoft )
            out << "  return name=='win64' or name=='microsoft'\n";
          #elif e_compiling( linux )
            out << "  return name=='linux'\n";
          #else
            out << "  return nil\n";
          #endif
          out << "end,\n";

        //}:                                      |
        //----------------------------------------|-----------------------------

        out << "}\n";
        return out;
      }
    }

  //}:                                            |
  //[globals]:{                                   |

    string            Workspace::gen;
    string            Workspace::ext;
    Workspace::States Workspace::bmp;

  //}:                                            |
  //generate:{                                    |

    namespace{

      int generate( const string& cgf ){

        //----------------------------------------------------------------------
        // Must always create the tmp directory.
        //----------------------------------------------------------------------

        if( IEngine::dexists( "tmp" )){
          IEngine::rm( "tmp" );
        }
        IEngine::mkdir( "tmp" );

        //----------------------------------------------------------------------
        // Generate template project and return.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bGenerate ){

          //--------------------------------------|-----------------------------
          //MaxPlugin:{                           |

            if( Workspace::bmp->bMaxPlugin ){

              //----------------------------------------------------------------
              // Write out the .DEF file.
              //----------------------------------------------------------------

              { Writer w( e_xfs( "tmp/%s.def", ccp( Workspace::gen )), kTEXT );
                w.write( e_xfs(
                    "LIBRARY %s.dlu\n"
                  , ccp( Workspace::gen )));
                w.write( "EXPORTS\n" );
                w.write( "  LibDescription   @1\n" );
                w.write( "  LibNumberClasses @2\n" );
                w.write( "  LibClassDesc     @3\n" );
                w.write( "  LibVersion       @4\n" );
                w.save();
              }

              //----------------------------------------------------------------
              // Write out the cogfile.lua and platform lua files.
              //----------------------------------------------------------------

              { Writer w( "tmp/cogfile.lua", kTEXT );
                w.write( "if platform.is'apple'then\n" );
                w.write( "  require'cogfile.xcode.lua'\n" );
                w.write( "elseif platform.is'microsoft'then\n" );
                w.write( "  require'cogfile.vs2019.lua'\n" );
                w.write( "elseif platform.is'linux'then\n" );
                w.write( "  require'cogfile.linux.lua'\n" );
                w.write( "end\n" );
                w.save();
              }
              { Writer w( "tmp/cogfile.xcode.lua", kTEXT );
                w.save();
              }
              { Writer w( "tmp/cogfile.linux.lua", kTEXT );
                w.save();
              }
              { Writer w( "tmp/cogfile.vs2019.lua", kTEXT );
                w.save();
              }
              return 0;
            }

          //}:                                    |
          //--------------------------------------|-----------------------------

          return-1;
        }

        //----------------------------------------------------------------------
        // Create Lua context, setup options on it, and run sandboxed script.
        //----------------------------------------------------------------------

        Lua::handle hLua = e_new<Lua>();
        const auto& st = e_fload( cgf );
        if( st.empty() ){
          return-1;
        }
        st.query(
          [&]( ccp pBuffer ){
            auto& lua = hLua.cast();
            lua.sandbox( platformClass() );
            lua.sandbox( kWorkspace );
            string sBuffer( pBuffer );
            sBuffer.replace( "${RELEASE}", "release" );
            sBuffer.replace( "${DEBUG}", "debug" );
            #if e_compiling( osx )
              sBuffer.replace( "${PLATFORM}", "macOS" );
            #elif e_compiling( linux )
              sBuffer.replace( "${PLATFORM}", "linux" );
            #elif e_compiling( microsoft )
              sBuffer.replace( "${PLATFORM}", "windows" );
            #endif
            string target = "local options={";
            if( Workspace::bmp->bXcode12 ){
              target << "\n  xcode12 = true,";
            }else{
              target << "\n  xcode12 = false,";
            }
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
            if( Workspace::bmp->bEmscripten ){
              target << "\n  emscripten = true,";
              target << "\n  wasm = true,";
            }else{
              target << "\n  emscripten = false,";
              target << "\n  wasm = false,";
            }
            if( Workspace::bmp->bQmake ){
              target << "\n  qmake = true,";
            }else{
              target << "\n  qmake = false,";
            }
            if( Workspace::bmp->bNinja ){
              target << "\n  ninja = true,";
            }else{
              target << "\n  ninja = false,";
            }
            target << "\n}\n";
            lua.sandbox( target + sBuffer );
          }
        );
        e_msgf( "ok" );
        return 0;
      }
    }

  //}:                                            |
//}:                                              |
//Globals:{                                       |
  //main:{                                        |

    int IEngine::main( const strings& args ){

      //------------------------------------------|-----------------------------
      //Versioning:{                              |
 
        //----------------------------------------------------------------------
        // Create the cogver file that lets us control versioning.
        //
        // REVISIONS:
        //
        //  1.4.6.0 Introduction of template generation; added for my new book:
        //  Metal, The Dark Arts.
        //
        //  1.4.7.0 Latest version with bug fixes.
        //
        //  1.4.7.1 Expanded command line arguments to support packaging multi-
        //  directories and multiple files. This will be really critical for a
        //  cross platform framework I'm planning with all platform shared lib
        //  and header files. Binary and much simpler than macOS and iOS etc.
        //
        //  1.4.9.0 Addeds support for TBD libraries.
        //
        //  1.5.0.0 Upgraded Lua to 5.4.0.
        //----------------------------------------------------------------------

        u8 major = 1;
        u8 minor = 5;
        u8 rev   = 0;
        u8 build = 1;

        //----------------------------------------------------------------------
        // Message out the version.
        //----------------------------------------------------------------------

        e_msgf( "Cog build system v%u.%u.%u.%u"
          , u32( major )
          , u32( minor )
          , u32( rev   )
          , u32( build )
        );

      //}:                                        |
      //------------------------------------------|-----------------------------
      //                                          :
      //                                          :
      //                                          :
      //------------------------------------------|-----------------------------
      //Options:{                                 |
        //Platform options:{                      |

          //--------------------------------------------------------------------
          // Setup platform specific options.
          //--------------------------------------------------------------------

          #if e_compiling( osx )
            Workspace::bmp->bXcode12 = 1;
          #elif e_compiling( microsoft )
            Workspace::bmp->bVS2019 = 1;
          #elif e_compiling( linux )
            Workspace::bmp->bNinja = 1;
          #endif

        //}:                                      |
        //Projects options:{                      |

          //--------------------------------------------------------------------
          // Run through all the arguments seting options as we go. Final non
          // long or short option will generate from arg as filename.
          //--------------------------------------------------------------------

          auto it = args.getIterator();
          while( ++it ){
            switch( **it ){

              //----------------------------------------------------------------
              // Long options.
              //----------------------------------------------------------------

              case'-':

                //--------------------------------------------------------------
                // Package up a directory.
                //--------------------------------------------------------------

                if( it->left( 10 ).tolower().hash() == "--package="_64 ){
                  const auto& pkgName = it->ltrimmed( 10 );
                  if( !++it ){
                    e_errorf( 81723, "missing directory name!" );
                    return-1;
                  }
                  onPackage( it, pkgName );
                  return 0;
                }

                //--------------------------------------------------------------
                // Generating from templates.
                //--------------------------------------------------------------

                if( it->left( 11 ).tolower().hash() == "--unpackage"_64 ){
                  if( !++it ){
                    e_errorf( 19283, "missing directory name!" );
                    return-1;
                  }
                  onUnpackage( *it );
                  return 0;
                }

                //--------------------------------------------------------------
                // Tweak output DLL (if there is one)  to be a 3D Studio Max
                // plugin.  This functionality is only available on Windows
                // because macOS has no support for 3D Studio Max. If it was
                // Maya this would be a very different proposition.
                //--------------------------------------------------------------

                #if e_compiling( microsoft )
                  if( it->trimmed( 4 ).tolower().hash() == "--maxplugin="_64 ){
                    Workspace::ext = it->ltrimmed( it->len() - 4 );
                    Workspace::bmp->bMaxPlugin = 1;
                    break;
                  }
                #endif

                //--------------------------------------------------------------
                // Collapse source files into Unity Build files.
                //--------------------------------------------------------------

                if( it->hash() == "--unity"_64 ){
                  Workspace::bmp->bUnity = 1;
                  break;
                }

                //--------------------------------------------------------------
                // Export to Ninja using emscripten and web assembly not C++.
                //--------------------------------------------------------------

                // Handle emscripten and wasm options.
                if(( it->hash() == "--emscripten"_64 )||( it->hash() == "--wasm"_64 )){
                  Workspace::bmp.all          = 0;
                  Workspace::bmp->bEmscripten = 1;
                  Workspace::bmp->bNinja      = 1;
                  break;
                }

                // Handle ninja option except on linux where it is the default.
                #if !e_compiling( linux )
                  if( it->hash() == "--ninja"_64 ){
                    Workspace::bmp.all     = 0;
                    Workspace::bmp->bNinja = 1;
                    break;
                  }
                #endif

                //--------------------------------------------------------------
                // Versioning saved back out on --version.
                //--------------------------------------------------------------

                if( it->left( 6 ).hash() == "--ver="_64 ){
                  cp  p = cp( it->c_str() + 6 );
                  cp  e = strchr( p, '.' );
                  u32 x = major;
                  u32 y = minor;
                  u32 z = rev;
                  u32 w = build;
                  if( e ){
                    *e = 0;
                    x = u32( atoi( p ));
                    p = e + 1;
                  }
                  e = strchr( p, '.' );
                  if( e ){
                    *e = 0;
                    y = u32( atoi( p ));
                    p = e + 1;
                  }
                  e = strchr( p, '.' );
                  if( e ){
                    z = u32( atoi( p ));
                    p = e + 1;
                  }
                  e = strchr( p, 0 );
                  if( e ){
                    w = u32( atoi( p ));
                  }
                  major = u8( x & 0xFF );
                  minor = u8( y & 0xFF );
                  rev   = u8( z & 0xFF );
                  build = u8( w & 0xFF );
                  { Writer fs( ".cog", kCOMPRESS|kNOEXT );
                    fs << major; // Major version
                    fs << minor; // Minor version
                    fs << rev;   // Revision version
                    fs << build; // Build version
                    fs.save( "Cog" ); // Makes an EON asset.
                  }
                  { Writer fs( "version.h", kTEXT );
                    fs << e_xfs(
                      "#define COG_BUILD_VERSION 0x%08x\n"
                      , u32( major << 24 )
                      | u32( minor << 16 )
                      | u32( rev   <<  8 )
                      | u32( build ));
                    fs << e_xfs(
                      "#define COG_BUILD_MAJOR    %u\n"
                      , u32( major ));
                    fs << e_xfs(
                      "#define COG_BUILD_MINOR    %u\n"
                      , u32( minor ));
                    fs << e_xfs(
                      "#define COG_BUILD_REVISION %u\n"
                      , u32( rev ));
                    fs << e_xfs(
                      "#define COG_BUILD_BUILD    %u\n"
                      , u32( build ));
                    fs.save();
                  }
                  break;
                }

                //--------------------------------------------------------------
                // Export a Qmake project.
                //--------------------------------------------------------------

                if( it->hash() == "--qmake"_64 ){
                  Workspace::bmp.all     = 0;
                  Workspace::bmp->bQmake = 1;
                  break;
                }

                //--------------------------------------------------------------
                // Export an Xcode 12 project instead of the default 11.
                //--------------------------------------------------------------

                #if e_compiling( osx )
                  if( it->hash() == "--xcode11"_64 ){
                    Workspace::bmp.all       = 0;
                    Workspace::bmp->bXcode11 = 1;
                    break;
                  }
                #endif

                //--------------------------------------------------------------
                // Help option.
                //--------------------------------------------------------------

                if( it->hash() == "--help"_64 ){
                  e_msgf( "  Usage cog [options] [cogfile.lua]" );
                  e_msgf( "    options:" );
                  e_msgf( "      --ver=major.minor.rev.build" );
                  e_msgf( "      --package=pkgname {file|dir} ..." );
                  e_msgf( "      --unpackage pkgname" );
                  e_msgf( "      --unity" );
                  #if e_compiling( microsoft )
                    e_msgf( "      --maxplugin={bmi|bmf|bms|dlb|dlc|dle|dlf"
                        "|dlh|dli|dlk|dlm|dlo|dlr|dls|dlt|dlu|dlv|flt|gup}" );
                  #elif e_compiling( osx )
                    e_msgf( "      --xcode11 (default is 12)" );
                  #endif
                  e_msgf( "      --emscripten \\__ Web Assembly" );
                  e_msgf( "      --wasm       /" );
                  #if !e_compiling( linux )
                    e_msgf( "      --ninja" );
                  #endif
                  e_msgf( "      --qmake" );
                  return 0;
                }
                break;

              //----------------------------------------------------------------
              // Everything else runs generator.
              //----------------------------------------------------------------

              default:
                if( !fexists( *it )){
                  e_warnsf( "  * %s not found; ignored!", ccp( *it ));
                }else{
                  return generate( *it );
                }
                break;
            }
          }

        //}:                                      |
      //}:                                        |
      //------------------------------------------|-----------------------------

      if( !fexists( "cogfile.lua" )){
        e_errorf( 98234, "cogfile.lua not found!" );
        return -1;
      }
      generate( "cogfile.lua" );
      return 0;
    }

  //}:                                            |
//}:                                              |
//------------------------------------------------|-----------------------------
