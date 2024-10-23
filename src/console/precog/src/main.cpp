//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#include"luacore.h"
#include"ws.h"

using namespace EON;
using namespace gfc;
using namespace fs;

//------------------------------------------------+-----------------------------
//Statics:{                                       |

  hashmap<u64,string> Workspace::File::filerefs;

//}:                                              |
//Private:{                                       |
  //[workspace]:{                                 |

    namespace{
      constexpr ccp kWorkspace = "workspace=class'workspace'{\n"
        //----------------------------------------+-----------------------------
        //new:{                                   |
          "  new=function(self,name)\n"
          "    return class'workspace'{\n"
          "      m_tProjects={},\n"
          "      m_sName=name,\n"
          "      new=function(self,label)\n"
          "        local t=class'project'{\n"
          //--------------------------------------+-----------------------------
          //Microsoft:{                           |
          "          dependencies=function(self,dependsOn)\n"
          "            self.m_dependencies=dependsOn\n"
          "          end,\n"
          "          winsdk=function(self,version)\n"
          "            self.m_winsdk=version\n"
          "            return self\n"
          "          end,\n"
          "          toolchain=function(self,version)\n"
          "            self.m_toolchain=version\n"
          "            return self\n"
          "          end,\n"
          //Creates a .def file for resources in a DLL.
          "          def=function(self,path)\n"
          "            self.m_def=path\n"
          "            return self\n"
          "          end,\n"
          //}:                                    |
          //Common:{                              |
            //set_include_paths:{                 |
            //TODO: Check this doesn't double up files like find_includes did.
            "          set_include_paths=function(self,paths)\n"
            "            if nil~=self.m_includePaths then\n"
            "              self.m_includePaths=self.m_includePaths..','..paths\n"
            "            else\n"
            "              self.m_includePaths=paths\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_resources:{                    |
            //TODO: Check this doesn't double up files like find_includes did.
            "          find_resources=function(self,paths)\n"
            "            if nil~=self.m_resPaths then\n"
            "              self.m_resPaths=self.m_resPaths..','..paths\n"
            "            else\n"
            "              self.m_resPaths=paths\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_libraries:{                    |
            "          find_libraries=function(self,paths)\n"
            "            if nil~=self.m_libraryPaths then\n"
            "              self.m_libraryPaths=self.m_libraryPaths..','..paths\n"
            "            else\n"
            "              self.m_libraryPaths=paths\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //find_sources:{                      |
            "          find_sources=function(self,paths)\n"
            "            if nil~=self.m_srcPaths then\n"
            "              self.m_srcPaths=self.m_srcPaths..','..paths\n"
            "            else\n"
            "              self.m_srcPaths=paths\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //skip_unity:{                        |
            "          skip_unity=function(self,files)\n"
            "            self.m_skipUnity=files\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //link_with:{                         |
            "          link_with=function(self,libs)\n"
            "            if type(libs)=='table'then\n"
            "              self.m_linkWith=libs\n"
            "            elseif type(libs)=='string'then\n"
            "              self.m_linkWith=libs\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //install:{                           |
            "          install=function(self,script)\n"
            "            self.m_installScript=script\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //defines:{                           |
            //         Usage: defines{...,...} or defines( debug, release )
            "          defines=function(self,t,r)\n"
            //           Process the debug build defines first.
            "            if type(t)=='table'then\n"
            "              if nil~=self.m_definesDbg then\n"
            "                self.m_definesDbg=self.m_definesDbg..','..t[1]\n"
            "              else\n"
            "                self.m_definesDbg=tostring(t[1])\n"
            "              end if nil~=self.m_definesRel then\n"
            "                self.m_definesRel=self.m_definesRel..','..t[2]\n"
            "              else\n"
            "                self.m_definesRel=t[2]\n"
            "              end\n"
            "            elseif type(t)=='string'then\n"
            "              if nil~=self.m_definesDbg then\n"
            "                self.m_definesDbg=self.m_definesDbg..','..t\n"
            "              else\n"
            "                self.m_definesDbg=t\n"
            "              end\n"
            "            elseif nil~=self.m_definesDbg then\n"
            "              self.m_definesDbg=self.m_definesDbg..','..t\n"
            "            else\n"
            "              self.m_definesDbg=tostring(t)\n"
            "            end\n"
            //           Process the release build defines next.
            "            if nil~=r then\n"
            "              if type(r)=='string'then\n"
            "                if nil~=self.m_definesRel then\n"
            "                  self.m_definesRel=self.m_definesRel..','..r\n"
            "                else\n"
            "                  self.m_definesRel=r\n"
            "                end\n"
            "              elseif nil~=self.m_definesRel then\n"
            "                self.m_definesRel=self.m_definesRel..','..r\n"
            "              else\n"
            "                self.m_definesRel=r\n"
            "              end\n"
            "            end\n"
            //           Return the self reference to caller so we can continue chaining.
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //disable:{                           |
            "          disable=function(self, options)\n"
            "            self.m_disableOpts=options\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //target:{                            |
            "          target=function(self,build)\n"
            "            self.m_build=build\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //prefix:{                            |
            "          prefix=function(self,header)\n"
            "            self.m_prefixHeader=header\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //ignore:{                            |
            "          ignore=function(self,regexes)\n"
            "            if nil~=self.m_ignore then\n"
            "              self.m_ignore=self.m_ignore..','..regexes\n"
            "            else\n"
            "              self.m_ignore=regexes\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
            //c_lang:{                            |
            //This is a shortcut function; for brevity (or not).
            "          c_lang=function(self,lang)\n"
            "            self.m_clanguage=lang\n"
            "            return self\n"
            "          end,\n"
            "          c_language=function(self,l)\n"
            "            return self:c_lang( l )\n"
            "          end,\n"
            //This is a shortcut function; for brevity (or not).
            "          lang=function(self,lang)\n"
            "            self.m_language=lang\n"
            "            return self\n"
            "          end,\n"
            "          language=function(self,l)\n"
            "            return self:lang( l )\n"
            "          end,\n"
            //}:                                  |
            //unity:{                             |
            "          unity=function(self,enable)\n"
            "            self.m_bUnity=enable\n"
            "          end,\n"
            //}:                                  |
            //libs:{                              |
            "          libs=function(self,paths)\n"
            "            if nil~=self.m_resPaths then\n"
            "              self.m_libs=self.m_libs..','..paths\n"
            "            else\n"
            "              self.m_libs=paths\n"
            "            end\n"
            "            return self\n"
            "          end,\n"
            //}:                                  |
          //}:                                    |
          //Apple:{                               |
          "          disableLibraryValidation=function(self,disable)\n"
          "            self.m_disableLibValidation=disable\n"
          "            return self\n"
          "          end,\n"
          "          enableJIT=function(self,enable)\n"
          "            self.m_enableJIT=enable\n"
          "            return self\n"
          "          end,\n"
          "          appleSilicon=function(self, enable)\n"
          "            self.m_enableAppleSilicon=enable\n"
          "            return self\n"
          "          end,\n"
          "          universal=function(self, enable)\n"
          "            self.m_enableUniversal=enable\n"
          "            return self\n"
          "          end,\n"
          "          not_embedded=function(self)\n"
          "            self.m_noEmbedAndSign=true\n"
          "            return self\n"
          "          end,\n"
          "          embed_and_sign=function(self,files)\n"
          "            self.m_filesToEmbedAndSign=files\n"
          "            return self\n"
          "          end,\n"
          "          load_all_symbols=function(self,enable)\n"
          "            self.m_loadAllSymbols=enable\n"
          "            return self\n"
          "          end,\n"
          "          harden=function(self,hardenedRuntime)\n"
          "            self.m_hardenedRuntime=hardenedRuntime\n"
          "            return self\n"
          "          end,\n"
          "          deployment=function(self,version)\n"
          "            self.m_deployTo=version\n"
          "            return self\n"
          "          end,\n"
          "          sdk_version=function(self,version)\n"
          "            self.m_sdkVersion=version\n"
          "            return self\n"
          "          end,\n"
          "          enableARC=function(self,enable)\n"
          "            self.m_arcEnabled=enable\n"
          "            return self\n"
          "          end,\n"
          "          export_headers=function(self,dirsAndFiles)\n"
          "            self.m_exportHeaders=dirsAndFiles\n"
          "            return self\n"
          "          end,\n"
          "          export_refs=function(self,dirsAndFiles)\n"
          "            self.m_exportRefs=dirsAndFiles\n"
          "            return self\n"
          "          end,\n"
          "          organization=function(self,name)\n"
          "            self.m_orgName=name\n"
          "            return self\n"
          "          end,\n"
          "          identifier=function(self,id)\n"
          "            self.m_bundleId=id\n"
          "            return self\n"
          "          end,\n"
          "          team=function(self,name)\n"
          "            self.m_teamName=name\n"
          "            return self\n"
          "          end,\n"
          "          find_frameworks=function(self,paths)\n"
          "            self.m_frameworkPaths=paths\n"
          "            return self\n"
          "          end,\n"
          "          os_target=function(self,osTarget)\n"
          "            self.m_osTarget=osTarget\n"
          "            return self\n"
          "          end,\n"
          //}:                                    |
          //--------------------------------------+-----------------------------
          "        }\n"
          "        self.m_tProjects[label]=t\n"
          "        t.label=label\n"
          "        return t\n"
          "      end,\n"
          "    }\n"
          "  end,\n"

        //}:                                      |
        //----------------------------------------+-----------------------------
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
        if( Workspace::bmp->bXcode11 ||
            Workspace::bmp->bXcode12 ||
            Workspace::bmp->bXcode15 ){
          return"  return'macos'";
        }
        if( Workspace::bmp->bVS2019 ||
            Workspace::bmp->bVS2022 ){
          return"  return'win64'";
        }
        if( Workspace::bmp->bNinja ){
          return"  return'linux'";
        }
        if( Workspace::bmp->bNDK ){
          return"  return'android'";
        }
        return nullptr;
      }

      //------------------------------------------------------------------------
      // Get the platform class; for the user to query the platform.
      //------------------------------------------------------------------------

      string platformClass(){
        string out;
        out << "platform=class'platform'{\n";

        //----------------------------------------+-----------------------------
        //builds:{                                |

          out << "builds=function()\n";
          out << "  return{ getBuildSystems() }\n";
          out << "end,\n";

        //}:                                      |
        //vendor:{                                |

          out << "vendor = function()\n";
          if( Workspace::bmp->bXcode11 ||
              Workspace::bmp->bXcode12 ||
              Workspace::bmp->bXcode15 ){
            out << "  return'apple'\n";
          }else if( Workspace::bmp->bXcode11 ||
                    Workspace::bmp->bXcode12 ||
                    Workspace::bmp->bXcode15 ){
            out << "  return'apple'\n";
          }else if( Workspace::bmp->bVS2019 ||
                    Workspace::bmp->bVS2022 ){
            out << "  return'microsoft'\n";
          }else{
            out << "  return'community'\n";
          }
          out << "end,\n";

        //}:                                      |
        //save:{                                  |

          out << "save = function(self)\n";
          out << "  out.save(out.generate(self),'";
          out << Workspace::out;
          out << "')\n";
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
        //----------------------------------------+-----------------------------

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

        if( !IEngine::dexists( Workspace::out ))
             IEngine::mkdir( Workspace::out );

        //----------------------------------------------------------------------
        // Generate template project and return.
        //----------------------------------------------------------------------

        if( Workspace::bmp->bGenerate ){

          //--------------------------------------+-----------------------------
          //MaxPlugin:{                           |

            if( Workspace::bmp->bMaxPlugin ){

              //--------------------------------------------------------------
              // Write out the .DEF file.
              //--------------------------------------------------------------

              { Writer w( e_xfs( "%s%s.def"
                  , ccp( Workspace::out )
                  , ccp( Workspace::gen ))
                  , kTEXT );
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

              { Writer w( Workspace::out + "cogfile.lua", kTEXT );
                w.write( "if platform.is'apple'then\n" );
                w.write( "  require'cogfile.xcode.lua'\n" );
                w.write( "elseif platform.is'microsoft'then\n" );
                w.write( "  require'cogfile.vs2019.lua'\n" );
                w.write( "elseif platform.is'linux'then\n" );
                w.write( "  require'cogfile.linux.lua'\n" );
                w.write( "end\n" );
                w.save();
              }
              { Writer w( Workspace::out + "cogfile.xcode.lua", kTEXT );
                w.save();
              }
              { Writer w( Workspace::out + "cogfile.linux.lua", kTEXT );
                w.save();
              }
              { Writer w( Workspace::out + "cogfile.vs2019.lua", kTEXT );
                w.save();
              }
              return 0;
            }

          //}:                                    |
          //--------------------------------------+-----------------------------

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
          sBuffer.replace( "${RELEASE}"
            , "release" );
          sBuffer.replace( "${DEBUG}"
            , "debug" );
          if( Workspace::bmp->bXcode11 ||
              Workspace::bmp->bXcode12 ||
              Workspace::bmp->bXcode15 ){
            if( Workspace::bmp->osMac ){
              sBuffer.replace( "${PLATFORM}"
                , "macos"
              );
            }else{
              sBuffer.replace( "${SUBPLATFORM}"
                , "ios" );
              sBuffer.replace( "${PLATFORM}"
                , "macos"
              );
            }
          }else if( Workspace::bmp->bGradle ){
            if( Workspace::bmp->bNDK ){
              sBuffer.replace( "${PLATFORM}"
                , "android"
              );
            }
          }else if( Workspace::bmp->bNinja ){
            sBuffer.replace( "${PLATFORM}"
              , "linux"
            );
          }else if( Workspace::bmp->bVS2019 ||
                    Workspace::bmp->bVS2022 ){
            sBuffer.replace( "${PLATFORM}"
              , "windows"
            );
          }
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
            if( Workspace::bmp->bXcode15 ){
              equ << "\n  xcode15 = true,";
            }else{
              equ << "\n  xcode15 = false,";
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
            if( Workspace::bmp->bNDK ){
              equ << "\n  ndk = true,";
            }else{
              equ << "\n  ndk = false,";
            }
            equ << "\n}\n";

            //------------------------------------------------------------------
            // Create the targeted script and sandbox it. Gets called twice if
            // generating for ios and macos simultaneously.
            //------------------------------------------------------------------

            const auto& targets = Workspace::getTargets();
            auto targetedScript = ( equ + pBuffer );
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
//Program:{                                       |
  //main:{                                        |

    int IEngine::main( const strings& args ){

      //------------------------------------------+-----------------------------
      //Versioning:{                              |

        //----------------------------------------------------------------------
        // 1.4.6.x  Introduction of template generation; added for my new book:
        // Metal, The Dark Arts.
        //----------------------------------------------------------------------
        // 1.4.7.x  Latest version with bug fixes.
        //----------------------------------------------------------------------
        // 1.4.8.x  Expanded command line arguments to support packaging multi-
        // directories and multiple files. This will be really critical for a
        // cross platform framework I'm planning with all platform shared lib
        // and header files. Binary and much simpler than macOS and iOS etc.
        //----------------------------------------------------------------------
        // 1.4.9.x  Addeds support for TBD libraries.
        //----------------------------------------------------------------------
        // 1.5.0.x  Upgraded Lua to 5.4.0.
        //----------------------------------------------------------------------
        // 1.5.1.x  Introducing load_all keyword for macOS. It enables all the
        // symbols of a static library to be exported from a framework. This'
        // needed by Swordlight so it only has to link against eon.framework.
        //----------------------------------------------------------------------
        // 1.5.2.x  Added --clean option instead of automatically deleting tmp.
        //----------------------------------------------------------------------
        // 1.5.3.x  Added Xcode project sorting and lots of other little tweaks
        // for compiling Swordlight and EON on Big Sur.  Added ability to turn
        // off universal builds.
        //----------------------------------------------------------------------
        // 1.5.4.x  Added more robust logic behind ignore().
        //----------------------------------------------------------------------
        // 1.5.5.x  Moved "--unity" switch out of the command line and into the
        // Lua scripts. It doesn't make sense to have unity builds for all
        // sub-projects, especially third party ones.
        //----------------------------------------------------------------------
        // 1.5.6.x  Added embedAndSign keyword to Xcode projects.
        //----------------------------------------------------------------------
        // 1.5.7.x  Added ability to use tables for all commands. Tables are a
        // excellent way to pass in a class instance to the generator instead
        // of a string.  Also added to this version is proper shared library
        // support.
        //----------------------------------------------------------------------
        // 1.5.8.x  Overhauled the plist handling in the Xcode backend and sign
        // setup. Lots of bug fixes and code refactoring.
        //----------------------------------------------------------------------
        // 1.6.0.x  Added support for macOS bundles. These are used as plugins
        // into the Player and Swordlight projects. Big version jump, because
        // bundles are a really major feature.
        //----------------------------------------------------------------------
        // 1.6.1.x  Added ability to set the library directory path from Lua.
        //----------------------------------------------------------------------
        // 1.6.2.x  Added toolchain keyword to Windows generator.
        //----------------------------------------------------------------------
        // 1.6.3.x  Added ignore filtering to Ninja serializer.
        //----------------------------------------------------------------------
        // 1.6.4.x  Added ability to disable embedding for Xcode bundles.
        //----------------------------------------------------------------------
        // 1.6.5.x  Added dependency syntax to Lua scripts.
        //----------------------------------------------------------------------
        // 1.6.6.x  Upgraded to latest Xcode.
        //----------------------------------------------------------------------
        // 1.6.7.x  Added proper PCH creation/using for MSVC.
        //----------------------------------------------------------------------
        // 1.6.8.x  Upgrading to 11.6 SDK and deployment target (Mac).
        //----------------------------------------------------------------------
        // 1.6.9.x  Added support for Visual Studio 2022.
        // 1.6.9.1  Added hooks for Windows into dependencies.
        // 1.6.9.2  Fixed problem with embedding dylibs.
        //----------------------------------------------------------------------
        // 1.7.0.x  Fixed a major bug finding macOS SDK root.
        //----------------------------------------------------------------------
        // 1.7.1.x  Large revamp for Windows: chiefly extended support for MSVC
        // 2022 and C++20. Also added LTSC whole program optimizations. Big
        // ver update because of all the changes for Windows that went in. The
        // macOS build will continue to be stable on 1.7.0.
        //----------------------------------------------------------------------
        // 1.7.2.x  Fixed a bug that causes cog to lockup with string equates.
        //----------------------------------------------------------------------
        // 1.7.3.x  Added support for Apple Silicon targets, not Universal
        // ones.
        //----------------------------------------------------------------------
        // 1.7.4.x  Added .eon files to list of possible resources.
        //----------------------------------------------------------------------
        // 1.7.5.x  Added first entitlements file.
        //----------------------------------------------------------------------
        // 1.7.6.x  Added "iOS iPadOS macOS" builds as command line option(s).
        // 1.7.6.1  Fixed a generation bug that chewed up the PBXNativeTarget.
        // 1.7.6.2  Fixed more bugs, this time testing in the debugger.
        // 1.7.6.6  Overhauled the generator for iOS (previous borked).
        // 1.7.6.7  Added overhaul of writePBXProjectSection (xcode.cpp).
        // 1.7.6.8  Forgot to increment an iterator.
        // 1.7.6.9  Approaching final code.
        //----------------------------------------------------------------------
        // 1.7.7.x  Intermediate build, working hard on ios path bug fixing.
        // 1.7.7.1  Nasty bug fixed in find_includes().
        // 1.7.7.2  Fixed some bugs in std generator.
        //----------------------------------------------------------------------
        // 1.7.8.x  Finally got iOS and macOS target working
        // This was a really big rewrite
        // Very happy with the outcome
        // 1.7.8.1  Fixed some generation bugs to do with bundles
        // 1.7.8.2  Hunted down and killed a lua_next() bug
        // 1.7.8.3  Fixed a nasty bug where `cog --clean` didn't work properly
        // 1.7.8.4  Fixed a nasty bug where `cog` resulted in empty targets
        // 1.7.8.5  Fixed a nasty bug where `cog` ignored `links_with`
        // 1.7.8.6  Fixed a nasty bug where `cog` resulted in red include group
        // 1.7.8.7  For some reason the frameworks didn't get included
        // 1.7.8.8  Big oops; fixed a bug where embedding was broken
        // 1.7.8.9  Fixed pulling in .xcassets packages.
        //----------------------------------------------------------------------
        // 1.7.9.x  Embedding entitlements into xcodeproj package, suppressing
        // "references" group if none exist.
        // 1.7.9.1  Building for universal.
        // 1.7.9.2  Removed find_includes; the functionality was duplicated by
        // find_sources. So, the .cpp files were duplicated because find_inc*
        // went through the exact same code path and blew everything all up.
        // This build also features new appending functionality for defines.
        // I also added the ability to pass a table into wsp:defines.
        // 1.7.9.3  Fixed a bunch of bugs in the embedded Lua code (above).
        // 1.7.9.4  More bug fixing in workspace:defines(); should be good.
        // 1.7.9.5  You can now dump anything passed to Lua::sandbox().
        // 1.7.9.6  To catch a bug; added DUMP_SCRIPT cvar.
        // 1.7.9.7  The Lua 'workspace' variable is global.
        // 1.7.9.8  Another debugging build.
        // 1.7.9.9  Fixed the bloody thing!
        // 1.7.9.10 Fixed a crash after generation.
        // 1.7.9.11 Fixed another crash.
        // 1.7.9.12 Fixed a bug in Xcode generator where nothing is written to
        // the .xcworkspace folders. We should see a ton of libraries, and the
        // the .xcodeproj's generated too. The problem was we were using src**
        // not the m_libraryPaths!
        // 1.7.9.13 Fixed another problem with the Xcode backendf generator. It
        // was stopping any .xcodeproj's from being generated again.
        // 1.7.9.14 Fixed a bad compilation with clang.
        // srcInc was missing.
        // 1.7.9.15 This is an attempt to get the defines working properly. At
        // the moment there's a lot of problems with it it isn't recognized by
        // the backend when I pass in a table. Bug juice!
        //----------------------------------------------------------------------
        // 1.8.0.x  A _huge_ milestone: lots of features!
        // 1.8.0.1  Added some more error messahes for #include<>; #include""
        // is completely illegal at the moment.
        // 1.8.0.2  Making it so I can generate for Ninja everywhere.
        // 1.8.0.3  Debugging the Ninja process.
        // 1.8.0.4  No more platform specifics.
        // 1.8.0.5  Added /utf-8 option.
        // 1.8.0.6  Squashed a crash.
        // 1.8.0.7  Addd Android support via NDK; applications and libraries.
        //----------------------------------------------------------------------
        // 1.8.1.x  Finished (mostly) Gradle/NDK generator.
        // 1.8.1.1  Added repositories to Gradle generator.
        // 1.8.1.2  Took out annoying C++ language logging.
        // 1.8.1.3  Adding framework path: /Library/Frameworks when looking for
        // frameworks to link with; this is cheifly for linking third party API
        // like Mono into the project.
        // 1.8.1.4  Fixed a bug that was stopping direct referencing of libs.
        // 1.8.1.5  Added support for managed frameworks like Python3.
        // 1.8.1.6  Recursive searching for managed frameworks.
        // 1.8.1.7  Fixing bugs made by previous [bad] version.
        // 1.8.1.8  Fixing double refs in dylib and frameworks.
        // 1.8.1.9  Program output strips lib/.a now; so much more reeadable.
        //----------------------------------------------------------------------
        // 1.8.2    Finished the next step in the preCog tools brill evolution.
        // There is better support for managed frameworks and the output was
        // trimmed and reformatted.
        // 1.8.2.1  Nasty bug found in the managed frameworks code. It creates
        // something aweful in the pathing department; see errors below.
        // WARNING: directory not found for option
        // '-L/Library/ManagedFrameworks/Python/Python3.framework/Versions/
        // 3.10/include/python3.10/3.10/include/python3.10'
        // WARNING: directory not found for option
        // '-F/Applications/Xcode.app/Contents/Developer/Platforms/
        // MacOSX.platform/Developer/SDKs/MacOSX13.1.sdk/
        // /Library/ManagedFrameworks/Python'
        // Framework not found Python3
        // 1.8.2.2  Fixed the first two errors, now working on the framework
        // not being found where I say it is.
        // 1.8.2.3  Fixed the last error; compiling it now to test––ok it does
        // not work for me still; no errors just the Framework not found issue.
        // 1.8.2.4  Fixed the "Framework not found" -F error.
        // 1.8.2.5  Adding support for /Xode.app/Contents/Developer/Frameworks.
        // 1.8.2.6  Now sorting frameworks by name in the 'Frameworks' group.
        //----------------------------------------------------------------------
        // 1.8.3.x  Good place to put a stake in the sand.
        // 1.8.3.1  Found some nasty evil and subtle bugs.
        // 1.8.3.2  Moving artifacts relative to products.
        // 1.8.3.3  Bundles need to go into products also.
        // 1.8.3.4  Product frameworks too.
        // 1.8.3.5  Product dylibs as well.
        //----------------------------------------------------------------------
        // 1.8.4.x  Added ability to turn on JIT.
        // 1.8.4.1  Added framrworks to the list of embeddables/signing.
        //----------------------------------------------------------------------
        // 1.8.5.x  Added help pages to --help and got rid of the -- 'optional'
        // tag for making xcode, visual studio, etc. It's a lot nicer now.
        // 1.8.5.1  Fixed some bugs and tested within Xcode.
        // 1.8.5.2  Fixed version.
        // 1.8.5.3  Fixed lockup when prebuilding Xcode workspace and projects.
        // 1.8.5.4  Fixed lockup when passing --* [anything] into Cog.
        //----------------------------------------------------------------------
        // 1.8.6.x  Big release: stable build after messing up before.
        // 1.8.6.1  Took out the error when you run without a cogfile.
        // 1.8.6.2  Bugfix: where a framework can be used only once.
        // 1.8.6.3  Bugfix: getting universal binaries working.
        // 1.8.6.4  Allow switching to unicode if command line (Windows only).
        // 1.8.6.5  Bugfix: getting the iOS generator working again.
        // 1.8.6.6  Added support for the lib/android folder (ndk).
        // 1.8.6.7  Fixed support for lib folder on Android.
        // 1.8.6.8  Fixed a moment of stupidity on macOS.
        // 1.8.6.9  Added better output for each project.
        //----------------------------------------------------------------------
        // 1.8.7.0  Updates in project settings; fix debug builds.
        // 1.8.7.1  Updates to support c++2b as well as an others.
        //----------------------------------------------------------------------
        // 1.8.8.0  Updating to Xcode 15.x
        // 1.8.8.1  Changing some logging.
        // 1.8.8.2  Changing more logging.
        // 1.8.8.3  Fixed bug in Lua scan.
        //----------------------------------------------------------------------
        // 1.8.9.x  Major bug fixing push.
        // 1.8.9.1  Major headers private.
        // 1.8.9.2  Fixed massive bug I introduced with 1.8.9; paths to dylibs.
        // 1.8.9.3  Fixed a nasty bug I inadvertantly introduced.
        // 1.8.9.4  Failed attempt to fix linking with dylibs.
        // 1.8.9.5  Another attempt to fix the same.
        // 1.8.9.6  Found the problem (in data) and plugged the hole.
        // 1.8.9.7  Refactored and cleaned up the lookfor() lambda.
        // 1.8.9.8  Big overhaul of the embedding/linking groups.
        // 1.8.9.9  Continuing overhaul; still not working yet.
        //----------------------------------------------------------------------
        // 1.9.0.x  Only need to get eon.framework linking with SisuXD program.
        // 1.9.9.1  Gets libs referencing the correct directories not tmp/.
        // 1.9.9.2  The rest of the bugs for this class of fixes.
        // 1.9.9.3  Fixing problems with anything non "archive".
        //----------------------------------------------------------------------
        // 2.0.0.x  A huge leap forward in technology.
        // 2.0.0.1  Fixed all library linker paths.
        // 2.0.0.2  All library updating PBX correctly.
        // 2.0.0.3  Skipping groups without content.
        // 2.0.0.4  Now linking products properly.
        // 2.0.0.5  All that's left is hard: target ownership in Xcode.
        // 2.0.0.6  Got proper referencing of "libfbxsdk.dylib/.a".
        // 2.0.0.7  System header paths working across the board.
        // 2.0.0.8  Finished except for framework and bundle embedding.
        // 2.0.0.9  Still no embedding but a LOT of other fixes.
        //----------------------------------------------------------------------
        // 2.0.1.x  Embedding finally working.
        // 2.0.1.1  Got Win64 version working( builds on macOS too ).
        // 2.0.1.2  Embedded frameworks still don't work; many fixes.
        // 2.0.1.3  Two bugs exist: If you reference a library (.a) from a
        // link_with{} (table form), you will have, eg, "lua.a" instead of
        // "liblua.a". That's a pain in the bum honestly, but not critical in
        // nature. The other bug is when you generate a .bundle or.framework
        // you won't get proper entries in the PBXFileReference section. So you
        // have no choice but to manually select those link items.
        //----------------------------------------------------------------------
        // 2.0.2.x  Most mature alpha release; still has those two bugs that I
        // know of, and others that I don't.
        // 2.0.2.1  Fixed the awkward generation in PBXBuildFile section.
        // 2.0.2.2  Fixed the unselected frameworks, dylibs, and bundles.
        // 2.0.2.3  Fixed the unfortunate bugs with character cases.
        //----------------------------------------------------------------------
        // 2.0.3.x  Fixed embedding bundles, last bug in alpha, so now beta.
        //----------------------------------------------------------------------
        // 2.0.4.x  Massive amounts of fixes in all departments of the PBX.
        // 2.0.4.1  Fixed a bug where the "Code" group was broken in Xcode.
        // 2.0.4.2  Hopefully fixes many info.plist problems in frameworks.
        //----------------------------------------------------------------------
        // 2.0.5.x  More awesome fixes a few bugs but I cn work around them.
        // 2.0.5.1  Bundles were not embedding in the application fixed now.
        // 2.0.5.2  Prefabs were renamed to Fablets fixing this tool for em.
        // 2.0.5.3  Adding a feature where we can generate a new project in
        // a completely independent-from-the-workspace/solution way.
        //----------------------------------------------------------------------
        // 2.0.6.0  Modifying the program so it can generate projects for any
        // platform precog supports on any platform it's compiled for.
        // 2.0.6.1  Added support for changing the default directory from tmp/
        // to [absolute|relative] path.
        // 2.0.7.2  Fixed Ninja target bugs.
        // 2.0.7.3  Fixed some build.ninja bugs.
        // 2.0.7.3  Ninja now links.
        //----------------------------------------------------------------------
        // 2.0.8.0  Lost to time.
        //----------------------------------------------------------------------
        // 2.0.9.0  Bug fix with shared libraries and Ninja.
        // 2.0.9.1  [patch] Adding ability to cross compile.
        // 2.0.9.2  [patch] Fixed a bug generating .so file.
        //----------------------------------------------------------------------
        // 2.0.10.0 Fixed a bug with ninja .so file.
        //----------------------------------------------------------------------
        // 2.0.11.0 Made a lot of fixes and polishing to cross platform stuff.
        // 2.0.11.1 [builds] Fixed a bug where Ninja console apps were borked.
        // 2.0.11.2 Fixed a log text problem.
        // 2.0.11.3 Missed a X compile bug.
        // 2.0.11.4 Finished fixing bugs for this revision.
        //----------------------------------------------------------------------
        // 2.0.12   New revision 12 begins in ernest. Want to get Xcode support
        // fully upgraded so I can say "precog Xcode" and all the signing etc.
        // will be up to the same levels as the hand crafted EON PBX projects.
        // 2.0.12.1 Need to revisit the help text and as a bit of future proof-
        // ing setting up the git-style help pages.
        //----------------------------------------------------------------------

        // Each has 256 steps: 0x00 thru 0xFF.
        static constexpr u8 major = 0x02; // Major version number [majrelease]
        static constexpr u8 minor = 0x00; // Minor version number [minrelease]
        static constexpr u8 rev   = 0x0C; // Revision
        static constexpr u8 build = 0x00; // Build
        static constexpr u8 patch = 0x03; // Patch

        //----------------------------------------------------------------------
        // Message out the version.
        //----------------------------------------------------------------------

        string title; title.catf(
          "Precog v%u.%u.%u"
            , major
            , minor
            , rev );
        if( build )
          title.catf( " (build %u)"
            , build );
        if( patch )
          title.catf(
            " patch #%u"
            , patch );
        if( args.size() == 1u )
          title << " --help";
        e_msg( title );
        if( args.size() == 1u )
          return 0;

      //}:                                        |
      //------------------------------------------+-----------------------------
      //                                          :
      //                                          :
      //                                          :
      //------------------------------------------+-----------------------------
      //Options:{                                 |
        //Projects options:{                      |

          //--------------------------------------------------------------------
          // Run through all the arguments seting options as we go. Final non
          // long or short option will generate from arg as filename.
          //--------------------------------------------------------------------

          auto it = args.getIterator();
          while( ++it ){
            const auto& key = it->tolower();
            if( key.left( 2 ).hash() == "-o"_64 ){
              Workspace::out = key.right( key.len()-2 ) + "/";
              continue;
            }
            switch(( key.hash() )){

              //----------------------------------------------------------------
              // Export a Qmake project.
              //----------------------------------------------------------------

              case"qmake"_64:
                Workspace::bmp->bQmake = 1;
                continue;

                //--------------------------------------------------------------
                // Export an Android gradle project.
                //--------------------------------------------------------------

                case"gradle"_64:
                  Workspace::bmp->bGradle = 1;// and Cmake...?
                  Workspace::bmp->bNDK    = 1;
                  continue;

                //--------------------------------------------------------------
                // Export a Visual Studio 2022 project instead of default 2019.
                //--------------------------------------------------------------

                case"vs2022=v143"_64:
                  Workspace::bmp->bVSTools143 = 1;
                  Workspace::bmp->bVS2022     = 1;
                  continue;
                case"vs2019"_64:
                  Workspace::bmp->bVS2019 = 1;
                  continue;
                case"vs2022"_64:
                  Workspace::bmp->bVSTools143 = 1;
                  Workspace::bmp->bVS2022     = 1;
                  continue;

                //--------------------------------------------------------------
                // Export to Ninja using emscripten and web assembly not C++.
                //--------------------------------------------------------------

                // Handle emscripten and wasm options.
                case"emscripten"_64:
                  [[fallthrough]];
                case"wasm"_64:
                  if(( it->hash() == "emscripten"_64 )||(
                       it->hash() == "wasm"_64 )){
                    Workspace::bmp->bEmscripten = 1;
                    Workspace::bmp->bNinja      = 1;
                  }
                  continue;

                // Handle ninja option except on linux where it is the default.
                case"ninja"_64:
                  Workspace::bmp->bNinja = 1;
                  continue;

              //----------------------------------------------------------------
              // Enable iOS builds with or without macOS support too. Note to
              // self: cog does not support AppleTV (tvOS) in any form.
              //----------------------------------------------------------------

              case"xcode=ios,macos"_64:
                [[fallthrough]];
              case"xcode=macos,ios"_64:
                [[fallthrough]];
              case"xcode=macos"_64:
                [[fallthrough]];
              case"xcode=ios"_64:
                [[fallthrough]];
              case"xcode"_64:/**/{
                string s( key );
                if( s.replace( "macos", "" )){
                  Workspace::bmp->bXcode12 = 1;
                  Workspace::bmp->osMac    = 1;
                }else if( s.replace( "ios", "" )){
                  Workspace::bmp->bXcode12 = 1;
                  Workspace::bmp->osIphone = 1;
                }else{
                  Workspace::bmp->bXcode12 = 1;
                  Workspace::bmp->osMac    = 1;
                }
                continue;
              }

              //----------------------------------------------------------------
              // Everything else runs generator.
              //----------------------------------------------------------------

              default:/**/{
                if( *key != '-' )
                  break;

                //--------------------------------------------------------------
                // Enable Unicode for Windows.
                //--------------------------------------------------------------

                if( it->tolower().hash() == "--utf16"_64 ){
                  Workspace::bmp->bUTF16 = 1;
                  continue;
                }

                //--------------------------------------------------------------
                // Cross compiling option.
                //--------------------------------------------------------------

                if( it->tolower().left( 8 ).hash() == "--cross="_64 ){
                  Workspace::crossCompileTriple = it->ltrimmed( 8 );
                  Workspace::bmp->bCrossCompile = 1;
                  continue;
                }

                if( it->left( 2 ).hash() == "-x"_64 ){
                  Workspace::crossCompileTriple = it->ltrimmed( 2 );
                  Workspace::bmp->bCrossCompile = 1;
                  continue;
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
                  IEngine::rm( Workspace::out );
                  continue;
                }

                //--------------------------------------------------------------
                // C++ langauge options.
                //--------------------------------------------------------------

                switch( key.hash() ){
                  case"--c++2b"_64:
                    [[fallthrough]];
                  case"--cxx2b"_64:
                    [[fallthrough]];
                  case"--cpp2b"_64:
                    [[fallthrough]];
                  case"--c++23"_64:
                    [[fallthrough]];
                  case"--cxx23"_64:
                    [[fallthrough]];
                  case"--cpp23"_64:
                    Workspace::wsp->setLanguage( "c++23"_64 );
                    break;
                  case"--c++20"_64:
                    [[fallthrough]];
                  case"--cxx20"_64:
                    [[fallthrough]];
                  case"--cpp20"_64:
                    Workspace::wsp->setLanguage( "c++20"_64 );
                    break;
                  case"--c++17"_64:
                    [[fallthrough]];
                  case"--cxx17"_64:
                    [[fallthrough]];
                  case"--cpp17"_64:
                    Workspace::wsp->setLanguage( "c++17"_64 );
                    break;
                  case"--c++14"_64:
                    [[fallthrough]];
                  case"--cxx14"_64:
                    [[fallthrough]];
                  case"--cpp14"_64:
                    Workspace::wsp->setLanguage( "c++14"_64 );
                    break;
                  case"--c++11"_64:
                    [[fallthrough]];
                  case"--cxx11"_64:
                    [[fallthrough]];
                  case"--cpp11"_64:/**/{
                    Workspace::wsp->setLanguage( "c++11"_64 );
                    break;
                  }
                }

                //--------------------------------------------------------------
                // Tweak output DLL (if there is one)  to be a 3D Studio Max
                // plugin.  This functionality is only available on Windows
                // because macOS has no support for 3D Studio Max. If it was
                // Maya this would be a very different proposition.
                //--------------------------------------------------------------

                if( it->trimmed( 4 ).tolower().hash() == "--maxplugin="_64 ){
                  Workspace::ext = it->ltrimmed( it->len() - 4 );
                  Workspace::bmp->bMaxPlugin = 1;
                  break;
                }

                //--------------------------------------------------------------
                // Handle the "--once:name" case. Only produces one new project
                // without touching the existing ones. It's awesome for getting
                // a new .vcxproj or .xcodeproj file/package, without having to
                // absolutely knacker the existing project. That way if there's
                // problems with a build, a corner case it doesn't support, you
                // can make a .bundle, .dylib, or .a like a charm.
                //--------------------------------------------------------------

                //--------------------------------------------------------------
                // Export an Xcode 1x project instead of the default 12.
                //--------------------------------------------------------------

                if( it->hash() == "--xcode-v15"_64 ){
                  Workspace::bmp.all       = 0;
                  Workspace::bmp->bXcode15 = 1;
                  break;
                }
                if( it->hash() == "--xcode-v12"_64 ){
                  Workspace::bmp.all       = 0;
                  Workspace::bmp->bXcode12 = 1;
                  break;
                }
                if( it->hash() == "--xcode-v11"_64 ){
                  Workspace::bmp.all       = 0;
                  Workspace::bmp->bXcode11 = 1;
                  break;
                }

                //--------------------------------------------------------------
                // Help option.
                //--------------------------------------------------------------

                if( it->hash() == "--help"_64 ||
                    it->hash() == "-h"_64 ||
                    it->hash() == "-?"_64 ){
                  e_msg( "  Usage \"precog* commands [globals|options|cvars] [cogfile.lua]\"" );
                  e_msg( "    Commands:" );
                  e_msg( "      xcode{=[macos|ios]}" );
                  e_msg( "      vs2022{=[v143]}" );
                  e_msg( "      emscripten" );
                  e_msg( "      gradle" );
                  e_msg( "      qmake" );
                  e_msg( "      ninja" );
                  e_msg( "      wasm" );
                  e_msg( "    Compiling:" );
                  e_msg( "      {--cross=|-x}<arch><sub>-<vendor>-<sys>-<env>" );
                  e_msg( "        arch   : x86_64 i386 arm, etc." );
                  e_msg( "        sub    : v6m, etc." );
                  e_msg( "        vendor : pc apple nvidia ibm, etc." );
                  e_msg( "        sys    : none linux win32 darwin cuda, etc." );
                  e_msg( "        env    : eabi gnu android macho elf, etc." );
                  e_msg( "      --{sse|neon}" );
                  e_msg( "    Globals:" );
                  e_msg( "      -opath" );
                  e_msg( "      --c{++|pp|xx}{20|17|14|11}" );
                  e_msg( "      --unity" );
                  e_msg( "      --clean" );
                  e_msg( "    Options:" );
                  e_msg( "      when \"xcode\"" );
                  e_msg( "        --xcode-v15" );
                  e_msg( "        --xcode-v12" );
                  e_msg( "        --xcode-v11" );
                  e_msg( "      when \"vs2022[=v143]\"" );
                  e_msg( "        --maxplugin=ext" );
                  e_msg( "      when \"ninja\"" );
                  e_msg( "      when \"emscripten\" \\__ Web Assembly" );
                  e_msg( "      when \"wasm\"       /" );
                  e_msg( "    Cvars:" );
                  e_msg( "      --cvar=ENABLE_PTHREADS" );
                  e_msg( "      --cvar=VERBOSE_LOGGING" );
                  e_msg( "      --cvar=SPEW" );
                  e_msg( "\n  * Relatively no relation to Minority Report" );
                  return 0;
                }
                break;
              }
            }
          }

        //}:                                      |
      //}:                                        |
      //------------------------------------------+-----------------------------

      if( !Workspace::bmp.all )
        return-1;
      generate( "cogfile.lua" );
      return 0;
    }

  //}:                                            |
//}:                                              |
//------------------------------------------------+-----------------------------
