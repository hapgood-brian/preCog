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
#include<ws.h>
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Externs:{                                       |

  void verifyPBX( const string& path );

//}:                                              |
//Private:{                                       |

  namespace{
    bool isUnityBuild(){
      auto it = IEngine::args.getIterator();
      while( it ){
        if( it->tolower().hash() == e_hashstr64_const( "--unity" )){
          return true;
        }
        ++it;
      }
      return false;
    }
  }

//}:                                              |
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_extends( Workspace::Xcode );

//}:                                              |
//Private:{                                       |
  //writeFileReference:{                          |

    namespace{
      void anon_writeFileReference( Writer& fs, const Workspace::Xcode::Files& files, const string& projectType ){
        auto paths = files;
        paths.sort(
          []( const Workspace::File& a, const Workspace::File& b ){
            return( a.filename().tolower() < b.filename().tolower() );
          }
        );
        files.foreach(
          [&]( const Workspace::File& f ){
            fs << "    "
              + f.toRefID()
              + " = {isa = PBXFileReference; lastKnownFileType = "
              + projectType
              + "; path = ../"
              + f
              + "; name = "
              + f.filename()
              + "; sourceTree = \"<group>\"; };\n"
            ;
          }
        );
      }
    }

  //}:                                            |
  //findFramework:{                               |

    namespace{
      string anon_findFramework( const Workspace::File& f ){
        if( *f == '~' ){
          return f.os();
        }
        if( *f == '.' ){
          return f.os();
        }
        if( *f == '/' ){
          return f.os();
        }
        return( "../" + f ).os();
      }
    }

  //}:                                            |
  //ignoreFile:{                                  |

    namespace{
      bool anon_ignoreFile( const string& regex, const string& s ){
        if( regex.empty() ){
          return false;
        }
        const std::regex r( regex.c_str() );
        const std::string var( s );
        auto it = var.cbegin();
        std::smatch sm;
        if( std::regex_search( it, var.cend(), sm, r )){
          return true;
        }
        return false;
      }
    }

  //}:                                            |
//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //extFromSource<>:{                           |

      ccp Workspace::Xcode::extFromEnum( const Type e )const{
        switch( e ){
          case decltype( e )::kCpp:
            return ".cpp";
          case decltype( e )::kMm:
            return ".mm";
          case decltype( e )::kC:
            return ".c";
          case decltype( e )::kM:
            return ".m";
          default:
            return "";
        }
      }

    //}:                                          |
    //sortingHat:{                                |

      void Workspace::Xcode::sortingHat( const string& in_path ){
        const auto& path = File( in_path );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case e_hashstr64_const( ".framework" ):
            inSources( Type::kFramework ).push( path );
            break;
          case e_hashstr64_const( ".storyboard" ):
            inSources( Type::kStoryboard ).push( path );
            break;
          case e_hashstr64_const( ".xcassets" ):
            inSources( Type::kXcasset ).push( path );
            break;
          case e_hashstr64_const( ".prefab" ):
            inSources( Type::kPrefab ).push( path );
            break;
          case e_hashstr64_const( ".lproj" ):
            inSources( Type::kLproj ).push( path );
            break;
          case e_hashstr64_const( ".plist" ):
            inSources( Type::kPlist ).push( path );
            break;
          case e_hashstr64_const( ".rtf" ):
            inSources( Type::kRtf ).push( path );
            break;
          case e_hashstr64_const( ".dylib" ):
            inSources( Type::kSharedlib ).push( path );
            break;
          case e_hashstr64_const( ".a" ):
            inSources( Type::kStaticlib ).push( path );
            break;
          case e_hashstr64_const( ".mm" ):
            inSources( Type::kMm ).push( path );
            break;
          case e_hashstr64_const( ".m" ):
            inSources( Type::kM ).push( path );
            break;

          //--------------------------------------------------------------------
          // Source and header file types.
          //--------------------------------------------------------------------

          case e_hashstr64_const( ".png" ):
            inSources( Type::kPng ).push( path );
            break;
          case e_hashstr64_const( ".inl" ):
            inSources( Type::kInl ).push( path );
            break;
          case e_hashstr64_const( ".hpp" ):
          case e_hashstr64_const( ".hxx" ):
          case e_hashstr64_const( ".hh" ):
            inSources( Type::kHpp ).push( path );
            break;
          case e_hashstr64_const( ".cpp" ):
          case e_hashstr64_const( ".cxx" ):
          case e_hashstr64_const( ".cc" ):
            inSources( Type::kCpp ).push( path );
            break;
          case e_hashstr64_const( ".h" ):
            inSources( Type::kH ).push( path );
            break;
          case e_hashstr64_const( ".c" ):
            inSources( Type::kC ).push( path );
            break;
          default:
            #if 0
              e_warnsf( "Ignoring %s!", ccp( path ));
            #endif
            return;
        }
        #if 0
          e_msgf( "  Found %s", ccp( path ));
        #endif
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::Xcode::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Count the number of unity processors and double it. This is how many
        // files we'll compile at one time on different threads.
        //----------------------------------------------------------------------

        const auto disableUnity =
            ( nullptr != toDisableOptions().tolower().find( "unity" ));
        u32 kLimit = std::thread::hardware_concurrency();
        if( kLimit > toUnity().capacity() ){
            kLimit = toUnity().capacity();
        }

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        if( isUnityBuild() ){
          u32 i = 0;
          unifyProject<Xcode>( fs, Type::kCpp );
          unifyProject<Xcode>( fs, Type::kMm  );
          unifyProject<Xcode>( fs, Type::kC   );
          unifyProject<Xcode>( fs, Type::kM   );
          writeProject<Xcode>( fs, Type::kCpp );
          writeProject<Xcode>( fs, Type::kMm  );
          writeProject<Xcode>( fs, Type::kC   );
          writeProject<Xcode>( fs, Type::kM   );
        }

        //----------------------------------------------------------------------
        // Save Xcode project to pbx format bundle.
        //----------------------------------------------------------------------

        fs << "// !$*UTF8*$!\n";
        fs << "{\n";
        fs << "  archiveVersion = 1;\n";
        fs << "  classes = {\n";
        fs << "  };\n";
        fs << "  objectVersion = 50;\n";
        fs << "  objects = {\n";
        writePBXBuildFileSection(             fs );
        writePBXCopyFilesBuildPhaseSection(   fs );
        writePBXFileReferenceSection(         fs );
        writePBXFrameworksBuildPhaseSection(  fs );
        writePBXGroupSection(                 fs );
        writePBXNativeTargetSection(          fs );
        writePBXProjectSection(               fs );
        writePBXResourcesBuildPhaseSection(   fs );
        writePBXShellScriptBuildPhaseSection( fs );
        writePBXHeadersBuildPhaseSection(     fs );
        writePBXSourcesBuildPhaseSection(     fs );
        writePBXVariantGroupSection(          fs );
        writeXCBuildConfigurationSection(     fs );
        writeXCConfigurationListSection(      fs );
        fs << "  };\n";
        fs << "  rootObject = " + m_sProjectObject + " /* Project object */;\n";
        fs << "}\n";
      }

    //}:                                          |
    //write*:{                                    |

      namespace{
        string normaliseInstallScript( const string& inScript ){
          string r( inScript );
          r.replace( "\"", "\\\"" );
          r.replace( "\n", "\\n" );
          return r;
        }
      }

      void Workspace::Xcode::writePBXShellScriptBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXShellScriptBuildPhase section */\n";
        fs << "    " + toShellScriptBuildPhase() + " /* ShellScript */ = {\n"
            + "      isa = PBXShellScriptBuildPhase;\n"
            + "      buildActionMask = 2147483647;\n"
            + "      files = (\n"
            + "      );\n"
            + "      inputFileListPaths = (\n"
            + "      );\n"
            + "      inputPaths = (\n"
            + "      );\n"
            + "      outputFileListPaths = (\n"
            + "      );\n"
            + "      outputPaths = (\n"
            + "      );\n"
            + "      runOnlyForDeploymentPostprocessing = 0;\n"
            + "      shellPath = /bin/sh;\n"
            + "      shellScript = \"" + normaliseInstallScript( toInstallScript() ) + "\";\n"
            + "    };\n";
        fs << "    /* End PBXShellScriptBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXBuildFileSection( Writer& fs )const{

        fs << "\n    /* Begin PBXBuildFile section */\n";

          //--------------------------------------------------------------------
          // Linking with.
          //--------------------------------------------------------------------

          Files files;
          if( !toLinkWith().empty() ){
            files.clear();
            const auto& libs = toLinkWith().splitAtCommas();
            libs.foreach(
              [&]( const string& lib ){
                if( lib.empty() ){
                  return;
                }
                if( *lib == '#' ){
                  return;
                }
                // Test whether the intent was to link with the system libs.
                if( lib.path().empty() && lib.ext().empty() ){
                  string path;
                  if( IEngine::dexists( "/Applications/Xcode.app" )){
                    path = e_xfs(
                      "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX%s.sdk/System/Library/Frameworks/"
                      , ccp( toDeployment() ));
                    if( lib.ext().empty() ){
                      path += lib + ".framework";
                    }else{
                      path += lib;
                    }
                    if( IEngine::dexists( path )){
                      e_msgf( "Found framework %s", ccp( path.basename() ));
                      files.push( File( path.os() ));
                    }
                  }
                  return;
                }
                if( *lib == '.' ){
                  files.push( File( lib.os() ));
                }else if( *lib == '/' ){
                  files.push( File( lib.os() ));
                }else if( *lib == '~' ){
                  files.push( File( lib.os() ));
                }else{
                  files.push( File(( "../" + lib ).os() ));
                }
              }
            );
            const_cast<Xcode*>( this )->setLibFiles( files );
            files.foreach(
              [&]( File& file ){
                if( file.empty() ){
                  return;
                }
                fs << "    "
                  + file.toBuildID()
                  + " /* "
                  + file.filename()
                  + " in Frameworks */ = {isa = PBXBuildFile; fileRef = "
                  + file.toRefID()
                  + " /* "
                  + file.filename()
                  + " in Frameworks */; };\n"
                ;
              }
            );
          }

          //--------------------------------------------------------------------
          // Resource files.
          //--------------------------------------------------------------------

          files.clear();
          files.pushVector( inSources( Type::kStoryboard ));
          files.pushVector( inSources( Type::kXcasset    ));
          files.pushVector( inSources( Type::kPrefab     ));
          files.pushVector( inSources( Type::kLproj      ));
          files.pushVector( inSources( Type::kPlist      ));
          files.foreach(
            [&]( File& file ){
              if( file.empty() ){
                return;
              }
              fs << "    "
                + file.toBuildID()
                + " /* "
                + file.filename()
                + " in Resources */ = {isa = PBXBuildFile; fileRef = "
                + file.toRefID()
                + " /* "
                + file.filename()
                + " */; };\n"
              ;
            }
          );

          //--------------------------------------------------------------------
          // Source files.
          //--------------------------------------------------------------------

          files.clear();
          files.pushVector( inSources( Type::kCpp ));
          files.pushVector( inSources( Type::kMm  ));
          files.pushVector( inSources( Type::kM   ));
          files.pushVector( inSources( Type::kC   ));
          files.foreach(
            [&]( File& file ){
              if( file.empty() ){
                return;
              }
              if( anon_ignoreFile( toIgnoreParts(), file )){
                e_msgf( "Ignoring header %s because regex = \"%s\"", ccp( file.filename() ), ccp( toIgnoreParts() ));
                return;
              }
              fs << "    "
                + file.toBuildID()
                + " /* "
                + file.filename()
                + " in Sources */ = {isa = PBXBuildFile; fileRef = "
                + file.toRefID()
                + " /* "
                + file.filename()
                + " */; };\n"
              ;
            }
          );
        fs << "    /* End PBXBuildFile section */\n";
      }

      void Workspace::Xcode::writePBXCopyFilesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXCopyFilesBuildPhase section */\n";
        if( toBuild().hash() == e_hashstr64_const( "application" )){
          if( !toEmbedFiles().empty() ){
            fs << "    " + toEmbedFrameworks() + " /* Embed Frameworks */ = {\n";
            fs << "      isa = PBXCopyFilesBuildPhase;\n";
            fs << "      buildActionMask = 2147483647;\n";
            fs << "      dstPath = \"\";\n";
            fs << "      dstFolderSpec = 10;\n";
            fs << "      files = (\n";
            toEmbedFiles().foreach(
              [&]( const File& file ){
                fs << "    "
                  + file.toBuildID()
                  + " /* "
                  + file.filename()
                  + " in Embed Frameworks */,\n"
                ;
              }
            );
            fs << "      );\n";
            fs << "      name = \"Embed Frameworks\";\n";
            fs << "      runOnlyForDeploymentPostProcessing = 0;\n";
            fs << "    };\n";
          }
        }
        fs << "    /* End PBXCopyFilesBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXFileReferenceSection( Writer& fs )const{
        fs << "\n    /* Begin PBXFileReference section */\n";
        anon_writeFileReference( fs, inSources( Type::kStoryboard ), "file.storyboard"     );
        anon_writeFileReference( fs, inSources( Type::kXcasset    ), "folder.assetcatalog" );
        anon_writeFileReference( fs, inSources( Type::kPrefab     ), "file"                );
        anon_writeFileReference( fs, inSources( Type::kLproj      ), "folder"              );
        anon_writeFileReference( fs, inSources( Type::kPlist      ), "text.plist.xml"      );
        anon_writeFileReference( fs, inSources( Type::kHpp        ), "sourcecode.cpp.h"    );
        anon_writeFileReference( fs, inSources( Type::kInl        ), "sourcecode.cpp.h"    );
        anon_writeFileReference( fs, inSources( Type::kH          ), "sourcecode.c.h"      );
        anon_writeFileReference( fs, inSources( Type::kCpp        ), "sourcecode.cpp.cpp"  );
        anon_writeFileReference( fs, inSources( Type::kMm         ), "sourcecode.cpp.objc" );
        anon_writeFileReference( fs, inSources( Type::kM          ), "sourcecode.c.objc"   );
        anon_writeFileReference( fs, inSources( Type::kC          ), "sourcecode.c.c"      );
        toPublicHeaders().foreach(
          [&]( const File& f ){
            string lastKnownFileType;
            switch( f.tolower().ext().hash() ){
              case e_hashstr64_const( ".h" ):
                lastKnownFileType = "sourcecode.c.h";
                break;
              default:
                lastKnownFileType = "folder";
                break;
            }
            fs << "    "
              + f.toRefID()
              + " = {isa = PBXFileReference; lastKnownFileType = "
              + lastKnownFileType
              + "; path = ../"
              + f
              + "; name = "
              + f.filename()
              + "; sourceTree = \"<group>\"; };\n"
            ;
          }
        );
        if( toBuild().hash() == e_hashstr64_const( "application" )){
          toEmbedFiles().foreach(
            [&]( const File& f ){
              string lastKnownFileType;
              switch( f.ext().tolower().hash() ){
                case e_hashstr64_const( ".framework" ):
                  lastKnownFileType = "wrapper.framework";
                  break;
                case e_hashstr64_const( ".dylib" ):
                  lastKnownFileType = "\"compiled.mach-o.dylib\"";
                  break;
                default:
                  return;
              }
              fs << "    "
                + f.toRefID()
                + " = {isa = PBXFileReference; lastKnownFileType = "
                + lastKnownFileType
                + "; path = "
                + anon_findFramework( f )
                + "; name = "
                + f.filename()
                + "; sourceTree = \"<group>\"; };\n"
              ;
            }
          );
        }
        toLibFiles().foreach(
          [&]( const File& f ){
            string lastKnownFileType;
            switch( f.ext().tolower().hash() ){
              case e_hashstr64_const( ".framework" ):
                lastKnownFileType = "wrapper.framework";
                break;
              case e_hashstr64_const( ".dylib" ):
                lastKnownFileType = "\"compiled.mach-o.dylib\"";
                break;
              case e_hashstr64_const( ".a" ):
                lastKnownFileType = "archive.ar";
                break;
            }
            fs << "    "
              + f.toRefID()
              + " = {isa = PBXFileReference; lastKnownFileType = "
              + lastKnownFileType
              + "; path = "
              + f.os()
              + "; name = "
              + f.filename();
            if( toBuild().hash() != e_hashstr64_const( "application" )){
              fs << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            }else{
              fs << "; sourceTree = \"<group>\"; };\n";
            }
          }
        );
        switch( toBuild().hash() ){
          case e_hashstr64_const( "framework" ):
            fs << "    "
              + m_sProductFileRef
              + " /* "
              + toLabel()
              + ".framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = "
              + toLabel()
              + ".framework; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case e_hashstr64_const( "static" ):
            fs << "    "
              + m_sProductFileRef
              + " /* lib"
              + toLabel()
              + ".a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = lib"
              + toLabel()
              + ".a; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case e_hashstr64_const( "application" ):
            fs << "    "
              + m_sProductFileRef
              + " /* "
              + toLabel()
              + " */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = "
              + toLabel()
              + "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case e_hashstr64_const( "console" ):
            fs << "    "
              + m_sProductFileRef
              + " /* "
              + toLabel()
              + " */ = {isa = PBXFileReference; explicitFileType = compiled.mach-o.executable; includeInIndex = 0; path = "
              + toLabel()
              + "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
        }
        fs << "    /* End PBXFileReference section */\n";
      }

      void Workspace::Xcode::writePBXFrameworksBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXFrameworksBuildPhase section */\n";
        if( !toLibFiles().empty() ){
          fs << "    " + m_sFrameworkBuildPhase + " /* frameworks */ = {\n"
              + "      isa = PBXFrameworksBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          toLibFiles().foreach(
            [&]( const File& f ){
              fs << "        " + f.toBuildID() + " /* " + f.filename() + " */,\n";
            }
          );
          fs << string( "      );\n" )
            + "      runOnlyForDeploymentPostprocessing = 0;\n"
            + "    };\n"
          ;
        }
        fs << "    /* End PBXFrameworksBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXGroupSection( Writer& fs )const{
        fs << "\n    /* Begin PBXGroup section */\n";

          //--------------------------------------------------------------------
          // Main top-level group.
          //--------------------------------------------------------------------

          fs << "    " + m_sMainGroup + " = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n";
          if( !toLibFiles().empty() ){
            fs << "        " + m_sFrameworkGroup + " /* Frameworks */,\n";
          }
          fs << "        " + m_sProductsGroup  + " /* Products */,\n"
              + "        " + m_sCodeGroup + " /* Code */,\n"
              + "      );\n"
              + "      sourceTree = \"<group>\";\n"
              + "    };\n";

          //--------------------------------------------------------------------
          // Products group.
          //--------------------------------------------------------------------

          fs << "    " + m_sProductsGroup + " /* Products */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n"
              + "        " + m_sProductFileRef + " /* " + toLabel() + ".framework */,\n"
              + "      );\n"
              + "      name = Products;\n"
              + "      sourceTree = \"<group>\";\n"
              + "    };\n";
          Files files;
          if( !toIncPath().empty() ){
            fs << "    " + m_sIncludeGroup + " /* include */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n";
            files.pushVector( inSources( Type::kHpp ));
            files.pushVector( inSources( Type::kInl ));
            files.pushVector( inSources( Type::kH   ));
            files.sort(
              []( const File& a, const File& b ){
                return( a.filename().tolower() < b.filename().tolower() );
              }
            );
            files.foreach(
              [&]( const File& file ){
                fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      path = \"\";\n";
            fs << "      name = " + toIncPath().basename() + ";\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
            if( !toLibFiles().empty() || !toEmbedFiles().empty() ){
              fs << "    " + m_sFrameworkGroup + " /* Frameworks */ = {\n"
                  + "      isa = PBXGroup;\n"
                  + "      children = (\n";
              toLibFiles().foreach(
                [&]( const File& f ){
                  fs << "        " + f.toRefID() + " /* " + f.filename() + " */,\n";
                }
              );
              toEmbedFiles().foreach(
                [&]( const File& f ){
                  fs << "        " + f.toRefID() + " /* " + f.filename() + " */,\n";
                }
              );
              fs << string( "      );\n" )
                + "      name = Frameworks;\n"
                + "      sourceTree = \"<group>\";\n"
                + "    };\n"
              ;
            }
          }

          //--------------------------------------------------------------------
          // Resources group.
          //--------------------------------------------------------------------

          files.clear();
          files.pushVector( inSources( Type::kStoryboard ));
          files.pushVector( inSources( Type::kXcasset    ));
          files.pushVector( inSources( Type::kPrefab     ));
          files.pushVector( inSources( Type::kLproj      ));
          fs << "    " + m_sCodeGroup + " /* Code */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n"
              + "        " + m_sReferencesGroup + " /* references */,\n";
          if( !files.empty() ){
            fs << "        " + m_sResourcesGroup + " /* resources */,\n";
          }
          fs << "        " + m_sIncludeGroup + " /* include */,\n"
              + "        " + m_sSrcGroup + " /* src */,\n"
              + "      );\n"
              + "      name = Code;\n"
              + "      sourceTree = \"<group>\";\n"
              + "    };\n";
          if( !files.empty() ){
            fs << "    " + m_sResourcesGroup + " /* resources */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n";
            files.sort(
              []( const File& a, const File& b ){
                return( a.filename().tolower() < b.filename().tolower() );
              }
            );
            files.foreach(
              [&]( const File& file ){
                fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      name = resources;\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
          }

          //--------------------------------------------------------------------
          // Exporting public headers from framework.
          //--------------------------------------------------------------------

          if( !toPublicHeaders().empty() ){
            fs << "    " + m_sReferencesGroup + " /* references */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n";
            files.clear();
            files.pushVector( toPublicHeaders() );
            files.foreach(
              [&]( const File& file ){
                fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      path = \"\";\n";
            fs << "      name = references;\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
          }
          fs << "    " + m_sSrcGroup + " /* src */ = {\n"
              + "      isa = PBXGroup;\n"
              + "      children = (\n";

          //--------------------------------------------------------------------
          // Adding source files.
          //--------------------------------------------------------------------

          files.clear();
          files.pushVector( inSources( Type::kCpp ));
          files.pushVector( inSources( Type::kMm  ));
          files.pushVector( inSources( Type::kC   ));
          files.pushVector( inSources( Type::kM   ));
          files.sort(
            []( const File& a, const File& b ){
              return( a.filename().tolower() < b.filename().tolower() );
            }
          );
          files.foreach(
            [&]( const File& file ){
              fs << "        " + file.toRefID() + " /* ../" + file + " */,\n";
            }
          );
          fs << "      );\n";
          fs << "      path = \"\";\n";
          fs << "      name = src;\n";// + toSrcPath().basename() + ";\n";
          fs << "      sourceTree = \"<group>\";\n";
          fs << "    };\n";
        fs << "    /* End PBXGroup section */\n";
      }

      void Workspace::Xcode::writePBXNativeTargetSection( Writer& fs )const{
        fs << "\n    /* Begin PBXNativeTarget section */\n";
        fs << "    " + m_sFrameworkNativeTarget + " /* framework_test */ = {\n"
            + "      isa = PBXNativeTarget;\n"
            + "      buildConfigurationList = " + m_sBuildNativeTarget + " /* Build configuration list for PBXNativeTarget \"" + toLabel() + "\" */;\n"
            + "      buildPhases = (\n"
            + "        " + m_sFrameworkBuildPhase   + " /* frameworks */,\n"
            + "        " + m_sResourcesBuildPhase   + " /* resources */,\n"
            + "        " + m_sHeadersBuildPhase     + " /* include */,\n"
            + "        " + m_sSourcesBuildPhase     + " /* src */,\n"
            + "        " + m_sShellScriptBuildPhase + " /* script */,\n";
        if( !toEmbedFrameworks().empty() ){
          fs << "        " + m_sEmbedFrameworks + " /* Embed Frameworks */,\n";
        }
        fs << string( "      );\n" )
            + "      buildRules = (\n"
            + "      );\n"
            + "      dependencies = (\n"
            + "      );\n"
            + "      name = " + toLabel() + ";\n"
            + "      productName = " + toLabel() + ";\n";
        switch( toBuild().hash() ){
          case e_hashstr64_const( "framework" ):
            fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".framework */;\n";
            fs << "      productType = \"com.apple.product-type.framework\";\n";
            break;
          case e_hashstr64_const( "static" ):
            fs << "      productReference = " + m_sProductFileRef + " /* lib" + toLabel() + ".a */;\n";
            fs << "      productType = \"com.apple.product-type.library.static\";\n";
            break;
          case e_hashstr64_const( "application" ):
            fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".app */;\n";
            fs << "      productType = \"com.apple.product-type.application\";\n";
            break;
          case e_hashstr64_const( "console" ):
            fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + " */;\n";
            fs << "      productType = \"com.apple.product-type.tool\";\n";
            break;
        }
        fs << "    };\n";
        fs << "    /* End PBXNativeTarget section */\n";
      }

      void Workspace::Xcode::writePBXProjectSection( Writer& fs )const{
        fs << "\n    /* Begin PBXProject section */\n";
        fs << "    " + m_sProjectObject + " /* Project object */ = {\n"
            + "      isa = PBXProject;\n"
            + "      attributes = {\n"
            + "        LastUpgradeCheck = 1120;\n"
            + "        ORGANIZATIONNAME = \"" + toOrgName() + "\";\n"
            + "        TargetAttributes = {\n"
            + "          " + m_sFrameworkNativeTarget + " = {\n"
            + "            CreatedOnToolsVersion = 11.2.1;\n"
            + "          };\n"
            + "        };\n"
            + "      };\n"
            + "      buildConfigurationList = " + m_sBuildConfigurationList + " /* Build configuration list for PBXProject \"" + toLabel() + "\" */;\n"
            + "      compatibilityVersion = \"Xcode 9.3\";\n"
            + "      developmentRegion = en;\n"
            + "      hasScannedForEncodings = 0;\n"
            + "      knownRegions = (\n"
            + "        en,\n"
            + "        Base,\n"
            + "      );\n"
            + "      mainGroup = " + m_sMainGroup + ";\n"
            + "      productRefGroup = " + m_sProductsGroup + " /* Products */;\n"
            + "      projectDirPath = \"\";\n"
            + "      projectRoot = \"\";\n"
            + "      targets = (\n"
            + "        " + m_sFrameworkNativeTarget + " /* " + toLabel() + " */,\n"
            + "      );\n"
            + "    };\n";
        fs << "    /* End PBXProject section */\n";
      }

      void Workspace::Xcode::writePBXHeadersBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXHeadersBuildPhase section */\n";
        fs << "    " + m_sHeadersBuildPhase + " /* Headers */ = {\n"
            + "      isa = PBXHeadersBuildPhase;\n"
            + "      buildActionMask = 2147483647;\n"
            + "      files = (\n";
        Files files;
        files.pushVector( toPublicHeaders() );
        files.foreach(
          [&]( const File& f ){
            fs << "        " + f.toBuildID() + " /* " + f + " in Reources */,\n";
          }
        );
        fs << "      );\n";
        fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
        fs << "    };\n";
        fs << "    /* End PBXHeadersBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXResourcesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXResourcesBuildPhase section */\n";
        fs << "    " + m_sResourcesBuildPhase + " /* Resources */ = {\n"
            + "      isa = PBXResourcesBuildPhase;\n"
            + "      buildActionMask = 2147483647;\n"
            + "      files = (\n";
        Files files;
        files.pushVector( inSources( Type::kStoryboard ));
        files.pushVector( inSources( Type::kXcasset    ));
        files.pushVector( inSources( Type::kPrefab     ));
        files.pushVector( inSources( Type::kLproj      ));
        files.pushVector( inSources( Type::kPlist      ));
        files.foreach(
          [&]( const File& f ){
            if( f.empty() ){
              return;
            }
            fs << "        " + f.toBuildID() + " /* " + f + " in Resources */,\n";
          }
        );
        fs << "      );\n";
        fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
        fs << "    };\n";
        fs << "    /* End PBXResourcesBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXVariantGroupSection( Writer& fs )const{
        fs << "\n    /* Begin PBXVariantGroup section */\n";
        fs << "    /* End PBXVariantGroup section */\n";
      }

      void Workspace::Xcode::writePBXSourcesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXSourcesBuildPhase section */\n";
        fs << "    " + m_sSourcesBuildPhase + " /* Sources */ = {\n"
            + "      isa = PBXSourcesBuildPhase;\n"
            + "      buildActionMask = 2147483647;\n"
            + "      files = (\n";
        Files files;
        files.pushVector( inSources( Type::kCpp ));
        files.pushVector( inSources( Type::kMm  ));
        files.pushVector( inSources( Type::kC   ));
        files.foreach(
          [&]( const File& f ){
            if( f.empty() ){
              return;
            }
            fs << "        " + f.toBuildID() + " /* " + f + " in Sources */,\n";
          }
        );
        fs << "      );\n";
        fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
        fs << "    };\n";
        fs << "    /* End PBXSourcesBuildPhase section */\n";
      }

      void Workspace::Xcode::writeXCBuildConfigurationSection( Writer& fs )const{
        fs << "\n    /* Begin XCBuildConfiguration section */\n";
        fs << "    " + m_sDebugBuildConfiguration + " /* Debug */ = {\n"
            + "      isa = XCBuildConfiguration;\n"
            + "      buildSettings = {\n"
            + "        ALWAYS_SEARCH_USER_PATHS = NO;\n"
            + "        CLANG_ANALYZER_NONNULL = YES;\n"
            + "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
            + "        CLANG_CXX_LANGUAGE_STANDARD = \"" + toLanguage() + "\";\n"
            + "        CLANG_CXX_LIBRARY = \"libc++\";\n"
            + "        CLANG_ENABLE_MODULES = YES;\n"
            + "        CLANG_ENABLE_OBJC_ARC = YES;\n"
            + "        CLANG_ENABLE_OBJC_WEAK = YES;\n"
            + "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
            + "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
            + "        CLANG_WARN_COMMA = YES;\n"
            + "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
            + "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
            + "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
            + "        CLANG_WARN_DOCUMENTATION_COMMENTS = YES;\n"
            + "        CLANG_WARN_EMPTY_BODY = YES;\n"
            + "        CLANG_WARN_ENUM_CONVERSION = YES;\n"
            + "        CLANG_WARN_INFINITE_RECURSION = YES;\n"
            + "        CLANG_WARN_INT_CONVERSION = YES;\n"
            + "        CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
            + "        CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
            + "        CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
            + "        CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
            + "        CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
            + "        CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
            + "        CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
            + "        CLANG_WARN_UNGUARDED_AVAILABILITY = YES_AGGRESSIVE;\n"
            + "        CLANG_WARN_UNREACHABLE_CODE = YES;\n"
            + "        CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;\n"
            + "        COPY_PHASE_STRIP = NO;\n"
            + "        CURRENT_PROJECT_VERSION = 1;\n"
            + "        DEBUG_INFORMATION_FORMAT = dwarf;\n"
            + "        ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
            + "        ENABLE_TESTABILITY = YES;\n"
            + "        GCC_C_LANGUAGE_STANDARD = gnu11;\n"
            + "        GCC_DYNAMIC_NO_PIC = NO;\n"
            + "        GCC_NO_COMMON_BLOCKS = YES;\n"
            + "        GCC_OPTIMIZATION_LEVEL = 0;\n"
            + "        GCC_PREPROCESSOR_DEFINITIONS = (\n"
            + "          \"$(inherited)\",\n";
        string dbgDefines = toDefinesDbg();
        const auto& vdbg = dbgDefines.splitAtCommas();
        vdbg.foreach(
          [&]( const string& define ){
            if( define.empty() ){
              return;
            }
            if( *define == '#' ){
              return;
            }
            fs << "          \"" + define + "\",\n";
          }
        );
        fs << string( "        );\n" )
            + "        GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
            + "        GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
            + "        GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
            + "        GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
            + "        GCC_WARN_UNUSED_FUNCTION = YES;\n"
            + "        GCC_WARN_UNUSED_VARIABLE = YES;\n";
        if( !toPrefixHeader().empty() ){
          fs << "        GCC_PRECOMPILE_PREFIX_HEADER = YES;\n";
          fs << "        GCC_PREFIX_HEADER = \"../" + toPrefixHeader() + "\";\n";
        }
        fs << string( "        MACOSX_DEPLOYMENT_TARGET = " + toDeployment() + ";\n" )
            + "        MTL_ENABLE_DEBUG_INFO = INCLUDE_SOURCE;\n"
            + "        MTL_FAST_MATH = YES;\n"
            + "        ONLY_ACTIVE_ARCH = YES;\n"
            + "        SDKROOT = macosx;\n"
            + "        VERSIONING_SYSTEM = \"apple-generic\";\n"
            + "        VERSION_INFO_PREFIX = \"\";\n"
            + "      };\n"
            + "      name = Debug;\n"
            + "    };\n";
        fs << "    " + m_sReleaseBuildConfiguration + " /* Release */ = {\n"
            + "      isa = XCBuildConfiguration;\n"
            + "      buildSettings = {\n"
            + "        ALWAYS_SEARCH_USER_PATHS = NO;\n"
            + "        CLANG_ANALYZER_NONNULL = YES;\n"
            + "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
            + "        CLANG_CXX_LANGUAGE_STANDARD = \"" + toLanguage() + "\";\n"
            + "        CLANG_CXX_LIBRARY = \"libc++\";\n"
            + "        CLANG_ENABLE_MODULES = YES;\n"
            + "        CLANG_ENABLE_OBJC_ARC = YES;\n"
            + "        CLANG_ENABLE_OBJC_WEAK = YES;\n"
            + "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
            + "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
            + "        CLANG_WARN_COMMA = YES;\n"
            + "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
            + "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
            + "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
            + "        CLANG_WARN_DOCUMENTATION_COMMENTS = YES;\n"
            + "        CLANG_WARN_EMPTY_BODY = YES;\n"
            + "        CLANG_WARN_ENUM_CONVERSION = YES;\n"
            + "        CLANG_WARN_INFINITE_RECURSION = YES;\n"
            + "        CLANG_WARN_INT_CONVERSION = YES;\n"
            + "        CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
            + "        CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
            + "        CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
            + "        CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
            + "        CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
            + "        CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
            + "        CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
            + "        CLANG_WARN_UNGUARDED_AVAILABILITY = YES_AGGRESSIVE;\n"
            + "        CLANG_WARN_UNREACHABLE_CODE = YES;\n"
            + "        CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;\n"
            + "        COPY_PHASE_STRIP = NO;\n"
            + "        CURRENT_PROJECT_VERSION = 1;\n"
            + "        DEBUG_INFORMATION_FORMAT = \"dwarf-with-dsym\";\n"
            + "        ENABLE_NS_ASSERTIONS = NO;\n"
            + "        ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
            + "        GCC_C_LANGUAGE_STANDARD = gnu11;\n"
            + "        GCC_NO_COMMON_BLOCKS = YES;\n"
            + "        GCC_OPTIMIZATION_LEVEL = fast;\n"
            + "        GCC_PREPROCESSOR_DEFINITIONS = (\n"
            + "          \"$(inherited)\",\n";
        string relDefines = toDefinesRel();
        const auto& vrel = relDefines.splitAtCommas();
        vrel.foreach(
          [&]( const string& define ){
            if( define.empty() ){
              return;
            }
            if( *define == '#' ){
              return;
            }
            fs << "          \"" + define + "\",\n";
          }
        );
        fs << string( "        );\n" )
            + "        GCC_WARN_64_TO_32_BIT_CONVERSION = YES;\n"
            + "        GCC_WARN_ABOUT_RETURN_TYPE = YES_ERROR;\n"
            + "        GCC_WARN_UNDECLARED_SELECTOR = YES;\n"
            + "        GCC_WARN_UNINITIALIZED_AUTOS = YES_AGGRESSIVE;\n"
            + "        GCC_WARN_UNUSED_FUNCTION = YES;\n"
            + "        GCC_WARN_UNUSED_VARIABLE = YES;\n";
        if( !toPrefixHeader().empty() ){
          fs << "        GCC_PRECOMPILE_PREFIX_HEADER = YES;\n";
          fs << "        GCC_PREFIX_HEADER = \"../" + toPrefixHeader() + "\";\n";
        }
        fs << string( "        MACOSX_DEPLOYMENT_TARGET = " + toDeployment() + ";\n" )
            + "        MTL_ENABLE_DEBUG_INFO = NO;\n"
            + "        MTL_FAST_MATH = YES;\n"
            + "        SDKROOT = macosx;\n"
            + "        VERSIONING_SYSTEM = \"apple-generic\";\n"
            + "        VERSION_INFO_PREFIX = \"\";\n"
            + "      };\n"
            + "      name = Release;\n"
            + "    };\n";
        fs << "    " + m_sDebugNativeBuildConfig + " /* Debug */ = {\n"
            + "      isa = XCBuildConfiguration;\n"
            + "      buildSettings = {\n"
            + "        CODE_SIGN_STYLE = Automatic;\n";
        if( !toTeamName().empty() ){
          fs << "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n";
        }
        fs << "        LIBRARY_SEARCH_PATHS = (\n";
        auto libraryPaths = toLibraryPaths().splitAtCommas();
        libraryPaths.foreach(
          [&]( const string& f ){
            fs << "          ../" + f + ",\n";
          }
        );
        fs << "        );\n";
        fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
        auto frameworkPaths = toFrameworkPaths().splitAtCommas();
        frameworkPaths.foreach(
          [&]( const string& f ){
            fs << "          ../" + f + ",\n";
          }
        );
        fs << "        );\n";
        fs << "        SYSTEM_HEADER_SEARCH_PATHS = (\n";
        strings paths;
        if( !toIncPath().empty() ){
          paths.push( "../" + toIncPath() );
        }
        if( !toIncludePaths().empty() ){
          const auto& syspaths = toIncludePaths().splitAtCommas();
          syspaths.foreach(
            [&]( const string& syspath ){
              if( syspath.empty() ){
                return;
              }
              if( *syspath == '/' ){
                paths.push( syspath );
              }else if( *syspath == '.' ){
                paths.push( syspath );
              }else{
                paths.push( "../" + syspath );
              }
            }
          );
        }
        paths.foreach(
          [&]( const string& path ){
            fs << "          " + path + ",\n";
          }
        );
        fs << "        );\n";
        switch( toBuild().hash() ){
          case e_hashstr64_const( "framework" ):
            fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
            fs << "        DEFINES_MODULE = YES;\n";
            fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
            fs << "        DYLIB_CURRENT_VERSION = 1;\n";
            fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
            fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
            fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
            fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
            fs << "          \"$(inherited)\",\n";
            fs << "          \"@executable_path/../Frameworks\",\n";
            fs << "          \"@loader_path/Frameworks\",\n";
            fs << "        );\n";
            fs << "        OTHER_LDFLAGS = (\n";
            fs << "          -L/usr/local/lib,\n";
            fs << "        );\n";
            fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
            break;
          case e_hashstr64_const( "static" ):
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
            fs << "        EXECUTABLE_PREFIX = lib;\n";
            break;
          case e_hashstr64_const( "application" ):
            fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
            fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
            fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
            fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
            fs << "        OTHER_LDFLAGS = (\n";
            fs << "          -L/usr/local/lib,\n";
            fs << "        );\n";
            break;
          case e_hashstr64_const( "console" ):
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
            fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
            fs << "        OTHER_LDFLAGS = (\n";
            fs << "          -L/usr/local/lib,\n";
            fs << "        );\n";
            break;
        }
        fs << "        SKIP_INSTALL = YES;\n";
        fs << "      };\n";
        fs << "      name = Debug;\n";
        fs << "    };\n";
        fs << "    " + m_sReleaseNativeBuildConfig + " /* Release */ = {\n"
            + "      isa = XCBuildConfiguration;\n"
            + "      buildSettings = {\n"
            + "        CODE_SIGN_STYLE = Automatic;\n";
        if( !toTeamName().empty() ){
          fs << "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n";
        }
        fs << "        LIBRARY_SEARCH_PATHS = (\n";
        libraryPaths.foreach(
          [&]( const string& f ){
            fs << "          ../" + f + ",\n";
          }
        );
        fs << "        );\n";
        fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
        frameworkPaths.foreach(
          [&]( const string& f ){
            fs << "          ../" + f + ",\n";
          }
        );
        fs << "        );\n";
        fs << "        SYSTEM_HEADER_SEARCH_PATHS = (\n";
        paths.foreach(
          [&]( const string& path ){
            fs << "          " + path + ",\n";
          }
        );
        fs << "        );\n";
        switch( toBuild().hash() ){
          case e_hashstr64_const( "framework" ):
            fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
            fs << "        DEFINES_MODULE = YES;\n";
            fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
            fs << "        DYLIB_CURRENT_VERSION = 1;\n";
            fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
            fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
            fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
            fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
            fs << "          \"$(inherited)\",\n";
            fs << "          \"@executable_path/../Frameworks\",\n";
            fs << "          \"@loader_path/Frameworks\",\n";
            fs << "        );\n";
            fs << "        OTHER_LDFLAGS = (\n";
            fs << "          -L/usr/local/lib,\n";
            fs << "        );\n";
            fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
            break;
          case e_hashstr64_const( "static" ):
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
            fs << "        EXECUTABLE_PREFIX = lib;\n";
            break;
          case e_hashstr64_const( "application" ):
            fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
            fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
            fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "/Info.plist\";\n";
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
            fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
            fs << "        OTHER_LDFLAGS = (\n";
            fs << "          -L/usr/local/lib,\n";
            fs << "        );\n";
            break;
          case e_hashstr64_const( "console" ):
            fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
            fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
            fs << "        OTHER_LDFLAGS = (\n";
            fs << "          -L/usr/local/lib,\n";
            fs << "        );\n";
            break;
        }
        fs << "        SKIP_INSTALL = YES;\n";
        fs << "      };\n";
        fs << "      name = Release;\n";
        fs << "    };\n";
        fs << "    /* End XCBuildConfiguration section */\n";
      }

      void Workspace::Xcode::writeXCConfigurationListSection( Writer& fs )const{
        fs << "\n    /* Begin XCConfigurationList section */\n";
        fs << "    " + m_sBuildConfigurationList + " /* Build configuration list for PBXProject \"" + toLabel() + "\" */ = {\n"
            + "      isa = XCConfigurationList;\n"
            + "      buildConfigurations = (\n"
            + "        " + m_sReleaseBuildConfiguration + " /* Release */,\n"
            + "        " + m_sDebugBuildConfiguration + " /* Debug */,\n"
            + "      );\n"
            + "      defaultConfigurationIsVisible = 0;\n"
            + "      defaultConfigurationName = Release;\n"
            + "    };\n";
        fs << "    " + m_sBuildNativeTarget + " /* Build configuration list for PBXNativeTarget \"" + toLabel() + "\" */ = {\n"
            + "      isa = XCConfigurationList;\n"
            + "      buildConfigurations = (\n"
            + "        " + m_sReleaseNativeBuildConfig + " /* Release */,\n"
            + "        " + m_sDebugNativeBuildConfig + " /* Debug */,\n"
            + "      );\n"
            + "      defaultConfigurationIsVisible = 0;\n"
            + "      defaultConfigurationName = Release;\n"
            + "    };\n";
        fs << "    /* End XCConfigurationList section */\n";
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
