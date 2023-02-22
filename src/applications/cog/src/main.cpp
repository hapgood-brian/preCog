//------------------------------------------------------------------------------
//                    Copyright 2022 Creepy Doll Software LLC.
//                            All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
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
            "          dependencies = function(self,dependsOn)\n"
            "            self.m_dependencies = dependsOn\n"
            "          end,\n"
            "          winsdk = function(self,version)\n"
            "            self.m_winsdk = version\n"
            "            return self\n"
            "          end,\n"
            "          toolchain = function(self,version)\n"
            "            self.m_toolchain = version\n"
            "            return self\n"
            "          end,\n"
            "          def = function(self,path)\n"
            "            self.m_def = path\n"
            "            return self\n"
            "          end,\n"
          #endif
          //}:                                    |
          //Common:{                              |
            //link_with:{                         |
            "          link_with = function(self,libs)\n"
            "            if type(libs)=='table'then\n"
            "              self.m_linkWith=libs\n"
            "            elseif type(libs)=='string'then\n"
            "              self.m_linkWith=libs\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //c_lang:{                            |
            "          c_lang = function(self,lang)\n"
            "            self.m_clanguage = lang\n"
            "            return self\n"
            "          end,\n"
            "          c_language = function(self,l)\n"
            "            return self:c_lang( l )\n"
            "          end,\n"
            "          lang = function(self,lang)\n"
            "            self.m_language = lang\n"
            "            return self\n"
            "          end,\n"
            "          language = function(self,l)\n"
            "            return self:lang( l )\n"
            "          end,\n"
            //}:                                  |
            //unity:{                             |
            "          unity = function(self,enable)\n"
            "            self.m_bUnity = enable\n"
            "          end,\n"
            //}:                                  |
            //libs:{                              |
            "          libs = function(self,dirs)\n"
            "            self.m_libs = dirs\n"
            "          end,\n"
            //}:                                  |
            //target:{                            |
            "          target = function(self,build)\n"
            "            self.m_build = build\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //install:{                           |
            "          install = function(self,script)\n"
            "            self.m_installScript = script\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //defines:{                           |
            "          defines = function(self,dbg,rel)\n"
            "            self.m_definesDbg = dbg\n"
            "            self.m_definesRel = rel\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //skip_unity:{                        |
            "          skip_unity = function(self,files)\n"
            "            self.m_skipUnity = files\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //disable:{                           |
            "          disable = function(self,options)\n"
            "            self.m_disableOpts = options\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //set_include_paths:{                 |
            "          set_include_paths = function(self,paths)\n"
            "            self.m_includePaths = paths\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_resources:{                    |
            "          find_resources = function(self,paths)\n"
            "            self.m_resPaths = paths\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_libraries:{                    |
            "          find_libraries = function(self,paths)\n"
            "            self.m_libraryPaths = paths\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_sources:{                      |
            "          find_sources = function(self,paths)\n"
            "            self.m_srcPaths = paths\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_includes:{                     |
            "          find_includes = function(self,paths)\n"
            "            self.m_incPaths = paths\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //ignore:{                            |
            "          ignore = function(self,regex)\n"
            "            self.m_ignore = regex\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //prefix:{                            |
            "          prefix = function(self,header)\n"
            "            self.m_prefixHeader = header\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
          //}:                                    |
          //Apple:{                               |
          #if e_compiling( osx )
          "          disableLibraryValidation = function(self,disable)\n"
          "            self.m_disableLibValidation = disable\n"
          "            return self\n"
          "          end,\n"
          "          appleSilicon = function(self, enable)\n"
          "            self.m_enableAppleSilicon = enable\n"
          "            return self\n"
          "          end,\n"
          "          universal = function(self, enable)\n"
          "            self.m_enableUniversal = enable\n"
          "            return self\n"
          "          end,\n"
          "          not_embedded = function(self)\n"
          "            self.m_noEmbedAndSign = true\n"
          "            return self\n"
          "          end,\n"
          "          embed_and_sign = function(self,files)\n"
          "            self.m_filesToEmbedAndSign = files\n"
          "            return self\n"
          "          end,\n"
          "          load_all_symbols = function(self,enable)\n"
          "            self.m_loadAllSymbols = enable\n"
          "            return self\n"
          "          end,\n"
          "          harden = function(self,hardenedRuntime)\n"
          "            self.m_hardenedRuntime = hardenedRuntime\n"
          "            return self\n"
          "          end,\n"
          "          deployment = function(self,version)\n"
          "            self.m_deployTo = version\n"
          "            return self\n"
          "          end,\n"
          "          sdk_version = function(self,version)\n"
          "            self.m_sdkVersion = version\n"
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
          "        }\n"
          "        self.m_tProjects[label]=t\n"
          "        t.label = label\n"
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
        out << "platform=class'platform'{\n";

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
            out << "  return( name=='apple' )\n";
          #elif e_compiling( microsoft )
            out << "  return( name=='win64' )or( name=='microsoft' )\n";
          #elif e_compiling( linux )
            out << "  return( name=='linux' )\n";
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

        if( !IEngine::dexists( "tmp" )){
          IEngine::mkdir( "tmp" );
        }

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

        const auto& createLua=[&]( const ccp pBuffer ){
          Lua::handle hLua = e_new<Lua>();
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
          return hLua;
        };

        //----------------------------------------------------------------------
        // Load the script and run it in Lua VM.
        //----------------------------------------------------------------------

        const auto& st = e_fload( cgf );
        if( st.empty() )
          return-1;
        st.query(
          [&]( ccp pBuffer ){
            auto hLua = createLua( pBuffer );
            auto& lua = *hLua;

            //------------------------------------------------------------------
            // Make local options.
            //------------------------------------------------------------------

            string equ = "local options={";
            if( Workspace::bmp->bXcode14 ){
              equ << "\n  xcode14 = true,";
            }else{
              equ << "\n  xcode14 = false,";
            }
            if( Workspace::bmp->bXcode12 ){
              equ << "\n  xcode12 = true,";
            }else{
              equ << "\n  xcode12 = false,";
            }
            if( Workspace::bmp->bXcode11 ){
              equ << "\n  xcode11 = true,";
            }else{
              equ << "\n  xcode11 = false,";
            }
            if( Workspace::bmp->bVS2022 ){
              equ << "\n  vs2022 = true,";
            }else{
              equ << "\n  vs2022 = false,";
            }
            if( Workspace::bmp->bVS2019 ){
              equ << "\n  vs2019 = true,";
            }else{
              equ << "\n  vs2019 = false,";
            }
            if( Workspace::bmp->bEmscripten ){
              equ << "\n  emscripten = true,";
              equ << "\n  wasm = true,";
            }else{
              equ << "\n  emscripten = false,";
              equ << "\n  wasm = false,";
            }
            if( Workspace::bmp->bQmake ){
              equ << "\n  qmake = true,";
            }else{
              equ << "\n  qmake = false,";
            }
            if( Workspace::bmp->bNinja ){
              equ << "\n  ninja = true,";
            }else{
              equ << "\n  ninja = false,";
            }
            equ << "\n}\n";

            //------------------------------------------------------------------
            // Create the targeted script and sandbox it. Gets called twice if
            // generating for ios and macos simultaneously.
            //------------------------------------------------------------------

            auto targetedScript =( equ + pBuffer );
            const auto& targets = Workspace::getTargets();
            auto it = targets.getIterator();
            while( it ){
              lua.initialise();
              lua.sandbox(
                platformClass() );
              lua.sandbox(
                kWorkspace );
              const auto& l
                = "__target = \""
                + *it
                + "\"\n"
                + targetedScript;
              if( !lua.sandbox( l )){
            DEBUG_BREAK
              }
              ++it;
            }
            lua.save();
          }
        );
        e_msgf(
          "ok" );
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
        //  1.4.6   Introduction of template generation; added for my new book:
        //  Metal, The Dark Arts.
        //----------------------------------------------------------------------
        //  1.4.7   Latest version with bug fixes.
        //----------------------------------------------------------------------
        //  1.4.8   Expanded command line arguments to support packaging multi-
        //  directories and multiple files. This will be really critical for a
        //  cross platform framework I'm planning with all platform shared lib
        //  and header files. Binary and much simpler than macOS and iOS etc.
        //----------------------------------------------------------------------
        //  1.4.9   Addeds support for TBD libraries.
        //----------------------------------------------------------------------
        //  1.5.0   Upgraded Lua to 5.4.0.
        //----------------------------------------------------------------------
        //  1.5.1   Introducing load_all keyword for macOS. It enables all the
        //  symbols of a static library to be exported from a framework. This'
        //  needed by Swordlight so it only has to link against eon.framework.
        //----------------------------------------------------------------------
        //  1.5.2   Added --clean option instead of automatically deleting tmp.
        //----------------------------------------------------------------------
        //  1.5.3   Added Xcode project sorting and lots of other little tweaks
        //  for compiling Swordlight and EON on Big Sur.  Added ability to turn
        //  off universal builds.
        //----------------------------------------------------------------------
        //  1.5.4   Added more robust logic behind ignore().
        //----------------------------------------------------------------------
        //  1.5.5   Moved "--unity" switch out of the command line and into the
        //  Lua scripts. It doesn't make sense to have unity builds for all
        //  sub-projects, especially third party ones.
        //----------------------------------------------------------------------
        //  1.5.6   Added embedAndSign keyword to Xcode projects.
        //----------------------------------------------------------------------
        //  1.5.7   Added ability to use tables for all commands. Tables are a
        //  excellent way to pass in a class instance to the generator instead
        //  of a string.  Also added to this version is proper shared library
        //  support.
        //----------------------------------------------------------------------
        //  1.5.8   Overhauled the plist handling in the Xcode backend and
        //  signing setup. Lots of bug fixes and code refactoring.
        //----------------------------------------------------------------------
        //  1.6.0   Added support for macOS bundles. These are used as plugins
        //  into the Player and Swordlight projects. Big version jump, because
        //  bundles are a really major feature.
        //----------------------------------------------------------------------
        //  1.6.1   Added ability to set the library directory path from Lua.
        //----------------------------------------------------------------------
        //  1.6.2   Added toolchain keyword to Windows generator.
        //----------------------------------------------------------------------
        //  1.6.3   Added ignore filtering to Ninja serializer.
        //----------------------------------------------------------------------
        //  1.6.4   Added ability to disable embedding for Xcode bundles.
        //----------------------------------------------------------------------
        //  1.6.5   Added dependency syntax to Lua scripts.
        //----------------------------------------------------------------------
        //  1.6.6   Upgraded to latest Xcode.
        //----------------------------------------------------------------------
        //  1.6.7   Added proper PCH creation/using for MSVC.
        //----------------------------------------------------------------------
        //  1.6.8   Upgrading to 11.6 SDK and deployment target (Mac).
        //----------------------------------------------------------------------
        //  1.6.9   Added support for Visual Studio 2022.
        //  1.6.9.1 Added hooks for Windows into dependencies.
        //  1.6.9.2 Fixed problem with embedding dylibs.
        //----------------------------------------------------------------------
        //  1.7.0   Fixed a major bug finding macOS SDK root.
        //----------------------------------------------------------------------
        //  1.7.1   Large revamp for Windows: chiefly extended support for MSVC
        //  2022 and C++20. Also added LTSC whole program optimizations. Big
        //  ver update because of all the changes for Windows that went in. The
        //  macOS build will continue to be stable on 1.7.0.
        //----------------------------------------------------------------------
        //  1.7.2   Fixed a bug that causes cog to lockup with string equates.
        //----------------------------------------------------------------------
        //  1.7.3   Added support for Apple Silicon targets, not Universal
        //  ones.
        //----------------------------------------------------------------------
        //  1.7.4   Added .eon files to list of possible resources.
        //----------------------------------------------------------------------
        //  1.7.5   Added first entitlements file.
        //----------------------------------------------------------------------
        //  1.7.6   Added "iOS iPadOS macOS" builds as command line option(s).
        //  1.7.6.1 Fixed a generation bug that chewed up the PBXNativeTarget.
        //  1.7.6.2 Fixed more bugs, this time testing in the debugger.
        //  1.7.6.6 Overhauled the generator for iOS (previous borked).
        //  1.7.6.7 Added overhaul of writePBXProjectSection (xcode.cpp).
        //  1.7.6.8 Forgot to increment an iterator.
        //  1.7.6.9 Approaching final code.
        //----------------------------------------------------------------------
        //  1.7.7   Intermediate build, working hard on ios path bug fixing.
        //  1.7.7.1 Nasty bug fixed in find_includes().
        //  1.7.7.2 Fixed some bugs in std generator.
        //----------------------------------------------------------------------
        //  1.7.8   Finally got iOS and macOS target working
        //  This was a really big rewrite
        //  Very happy with the outcome
        //  1.7.8.1 Fixed some generation bugs to do with bundles
        //  1.7.8.2 Hunted down and killed a lua_next() bug
        //  1.7.8.3 Fixed a nasty bug where `cog --clean` didn't work properly
        //  1.7.8.4 Fixed a nasty bug where `cog` resulted in empty targets
        //  1.7.8.5 Fixed a nasty bug where `cog` ignored `links_with`
        //  1.7.8.6 Fixed a nasty bug where `cog` resulted in red include group
        //  1.7.8.7 For some reason the frameworks didn't get included
        //  1.7.8.8 Big oops; fixed a bug where embedding was broken
        //  1.7.8.9 Fixed pulling in .xcassets packages.
        //----------------------------------------------------------------------
        //  1.7.9   Embedding entitlements into xcodeproj package, suppressing
        //  "references" group if none exist.
        //  1.7.9.1 Building for universal.
        //----------------------------------------------------------------------

        u8 major = 1;
        u8 minor = 7;
        u8 rev   = 9;
        u8 build = 0;

        //----------------------------------------------------------------------
        // Message out the version.
        //----------------------------------------------------------------------

        if( build ){
          e_msgf( "Cog pre-build system v%u.%u.%u (build %u)"
            , u32( major )
            , u32( minor )
            , u32( rev   )
            , u32( build )
          );
        }else{
          e_msgf( "Cog pre-build system v%u.%u.%u"
            , u32( major )
            , u32( minor )
            , u32( rev   )
          );
        }

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
                // Enable iOS builds with or without macOS support too. Note to
                // self: cog does not support AppleTV (tvOS) in any form.
                //--------------------------------------------------------------

                if( it->tolower().left( 8 )=="--xcode="_64 ){
                  auto targets = it->tolower().ltrimmed( 8 );
                  if( targets.replace( "macos", "" ))
                    Workspace::bmp->osMac = 1;
                  if( targets.replace( "ios", "" ))
                    Workspace::bmp->osIphone = 1;
                  if( Workspace::bmp->osIphone &&
                      Workspace::bmp->osMac ){
                    Workspace::bmp->allApple = 1;
                  }
                }

                //--------------------------------------------------------------
                // Enable unity builds.
                //--------------------------------------------------------------

                if( it->tolower().hash() == "--unity"_64 ){
                  Workspace::bmp->bUnity = 1;
                  continue;
                }

                //--------------------------------------------------------------
                // Delete tmp directory.
                //--------------------------------------------------------------

                if( it->tolower().hash() == "--clean"_64 ){
                  IEngine::rm( "tmp" );
                  continue;
                }

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
                // Export a Visual Studio 2022 project instead of default 2019.
                //--------------------------------------------------------------

                #if e_compiling( microsoft )
                  if( it->hash() == "--vs2022=v143"_64 ){
                    Workspace::bmp->bVSTools143 = 1;
                    Workspace::bmp->bVS2022     = 1;
                    break;
                  }
                  if( it->hash() == "--vs2022"_64 ){
                    Workspace::bmp->bVS2022 = 1;
                    break;
                  }
                #endif
                switch( it->hash() ){
                  case"--c++20"_64:
                  case"--cxx20"_64:
                  case"--cpp20"_64:
                    Workspace::bmp->uLanguage = 20;
                    break;
                  case"--c++17"_64:
                  case"--cxx17"_64:
                  case"--cpp17"_64:
                    Workspace::bmp->uLanguage = 17;
                    break;
                  case"--c++14"_64:
                  case"--cxx14"_64:
                  case"--cpp14"_64:
                    Workspace::bmp->uLanguage = 14;
                    break;
                  case"--c++11"_64:
                  case"--cxx11"_64:
                  case"--cpp11"_64:
                    Workspace::bmp->uLanguage = 11;
                    break;
                }

                //--------------------------------------------------------------
                // Handle the MT NO DLL case for compiling.
                //--------------------------------------------------------------

                #if e_compiling( microsoft )
                  switch( it->hash() ){
                    case"--mtdll=false"_64:
                      [[fallthrough]];
                    case"--mtdll=no"_64:
                      Workspace::bmp->bVSMTNoDLL = 1;
                      break;
                  }
                #endif


                //--------------------------------------------------------------
                // Export an Xcode 1x project instead of the default 12.
                //--------------------------------------------------------------

                #if e_compiling( osx )
                  if( it->hash() == "--xcode14"_64 ){
                    Workspace::bmp.all       = 0;
                    Workspace::bmp->bXcode12 = 1;
                    break;
                  }
                  if( it->hash() == "--xcode12"_64 ){
                    Workspace::bmp.all       = 0;
                    Workspace::bmp->bXcode12 = 1;
                    break;
                  }
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
//                e_msgf( "      --ver=major.minor.rev.build" );//TODO: <- Make this more useful or rip it out.
//                e_msgf( "      --package=pkgname {file|dir} ..." );//TODO: <- Remove all of it (it isn't useful at all).
//                e_msgf( "      --unpackage pkgname" );//TODO: <- Remove all of it too.
                  #ifdef __APPLE__
                    e_msgf( "      --xcode=[macos,ios]" );
                    e_msgf( "      --ndk" );
                  #endif
                  e_msgf( "      --unity" );
                  e_msgf( "      --clean" );
                  #if e_compiling( microsoft )
//                  e_msgf( "      --maxplugin={bmi|bmf|bms|dlb|dlc|dle|dlf"
//                      "|dlh|dli|dlk|dlm|dlo|dlr|dls|dlt|dlu|dlv|flt|gup}" );
                    e_msgf( "      --vs2022[=v143]" );
                    e_msgf( "      --c++{20|17|14|11} (default is 17)" );
                  #endif
                  e_msgf( "      --emscripten \\__ Web Assembly" );
                  e_msgf( "      --wasm       /" );
                  #if e_compiling( microsoft )
                    e_msgf( "      --mtdll=no" );
                  #endif
                  #if !e_compiling( linux )
                    e_msgf( "      --ninja" );
                  #endif
//                e_msgf( "      --qmake" );//TODO: <- Rip out all of cog's Qmake code.
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
