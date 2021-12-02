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

//http://www.monobjc.net/xcode-project-file-format.html

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
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_extends( Workspace::Xcode );

//}:                                              |
//Private:{                                       |
  //writeFileReference:{                          |

    namespace{
      void anon_writeFileReference( Writer& fs
          , const Workspace::Xcode::Files& files
          , const string& projectType ){
        auto paths = files;
        paths.sort(
          []( const Workspace::File& a, const Workspace::File& b ){
            return( a.filename().tolower() < b.filename().tolower() );
          }
        );
        paths.foreach(
          [&]( const Workspace::File& f ){
            fs << "    "
              + f.toFileRefID()
              + " = {isa = PBXFileReference; lastKnownFileType = "
              + projectType
              + "; name = "
              + f.filename()
              + "; path = ../"
              + f
              + "; sourceTree = \"<group>\"; };\n"
            ;
          }
        );
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

      bool Workspace::Xcode::sortingHat( const string& in_path ){
        const auto& path = File( in_path );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case".framework"_64:
            inSources( Type::kFramework ).push( path );
            break;
          case".bundle"_64:
            inSources( Type::kBundle ).push( path );
            break;
          case".storyboard"_64:
            inSources( Type::kStoryboard ).push( path );
            break;
          case".xcassets"_64:
            inSources( Type::kXcasset ).push( path );
            break;
          case".prefab"_64:
            inSources( Type::kPrefab ).push( path );
            break;
          case".lproj"_64:
            inSources( Type::kLproj ).push( path );
            break;
          case".plist"_64:
            inSources( Type::kPlist ).push( path );
            setPlistPath( path );
            break;
          case".rtf"_64:
            inSources( Type::kRtf ).push( path );
            break;
          case".dylib"_64:
            inSources( Type::kSharedlib ).push( path );
            break;
          case".a"_64:
            inSources( Type::kStaticlib ).push( path );
            break;
          case".mm"_64:
            inSources( Type::kMm ).push( path );
            break;
          case".m"_64:
            inSources( Type::kM ).push( path );
            break;

          //--------------------------------------------------------------------
          // Source and header file types.
          //--------------------------------------------------------------------

          case".png"_64:
            inSources( Type::kPng ).push( path );
            break;
          case".inl"_64:
            inSources( Type::kInl ).push( path );
            break;
          case".hpp"_64:
          case".hxx"_64:
          case".hh"_64:
            inSources( Type::kHpp ).push( path );
            break;
          case".cpp"_64:
          case".cxx"_64:
          case".cc"_64:
            inSources( Type::kCpp ).push( path );
            break;
          case".h"_64:
            inSources( Type::kH ).push( path );
            break;
          case".c"_64:
            inSources( Type::kC ).push( path );
            break;
          default:
            return false;
        }
        return true;
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::Xcode::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        const auto isUnity = isUnityBuild();
        if( !isUnity||!Workspace::bmp->bUnity ){
          writeProject<Xcode>( fs, Type::kCpp );
          writeProject<Xcode>( fs, Type::kMm );
          writeProject<Xcode>( fs, Type::kC );
          writeProject<Xcode>( fs, Type::kM );
        }else{
          const u32 cores = std::thread::hardware_concurrency();
          u32 i=0;
          const_cast<Xcode*>( this )->toUnity().resize( cores );
          const_cast<Xcode*>( this )->unifyProject<Xcode>( Type::kCpp, i );
          const_cast<Xcode*>( this )->unifyProject<Xcode>( Type::kMm,  i );
          const_cast<Xcode*>( this )->unifyProject<Xcode>( Type::kC,   i );
          const_cast<Xcode*>( this )->unifyProject<Xcode>( Type::kM,   i );
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
        if( Workspace::bmp->bXcode11 ){
          fs << "  objectVersion = 50;\n";// Version 9.3 compatible.
        }else if( Workspace::bmp->bXcode12 ){
          fs << "  objectVersion = 54;\n";
        }
        fs << "  objects = {\n";
        writePBXBuildFileSection(             fs );
        writePBXCopyFilesBuildPhaseSection(   fs );
        writePBXFileReferenceSection(         fs );
        writePBXFrameworksBuildPhaseSection(  fs );
        writePBXGroupSection(                 fs );
        writePBXHeadersBuildPhaseSection(     fs );
        writePBXNativeTargetSection(          fs );
        writePBXProjectSection(               fs );
        writePBXResourcesBuildPhaseSection(   fs );
        writePBXShellScriptBuildPhaseSection( fs );
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

      void Workspace::Xcode::writePBXBuildFileSection( Writer& out )const{

        out << "\n    /* Begin PBXBuildFile section */\n";

          //--------------------------------------------------------------------
          // Linking with.
          //--------------------------------------------------------------------

          Files files;
          if( !toLinkWith().empty() ){
            const auto& libs = toLinkWith().splitAtCommas();
            libs.foreach(
              [&]( const string& lib ){

                //--------------------------------------------------------------
                // Bail conditions.
                //--------------------------------------------------------------

                // Missing library string bails out.
                if( lib.empty() ){
                  return;
                }

                // Comment found at start of string, bail out.
                if( *lib == '#' ){
                  return;
                }

                //**************************************************************

                //--------------------------------------------------------------
                // Test whether the intent was to link with a TBD file.
                //--------------------------------------------------------------

                if( e_dexists( "/Applications/Xcode.app" )){
                  auto path = e_xfs(
                    "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX%s.sdk/usr/lib"
                    , ccp( toSdkVersion() ));
                  path += "/lib" + lib + ".tbd";
                  if( e_fexists( path )){
                    e_msgf( "Found library %s (tbd)"
                         , ccp( path.basename() ));
                    files.push( File( path.os() ));
                    return;
                  }
                }

                //**************************************************************

                //--------------------------------------------------------------
                // Link against system frameworks in the SDK.
                //--------------------------------------------------------------

                const auto wantsSystemFramework=( lib.path().empty() && lib.ext().empty() );
                if( wantsSystemFramework ){

                  //------------------------------------------------------------
                  // Test whether the intent was to link with the system libs.
                  //------------------------------------------------------------

                  const auto& rootLibraryPath = "/Library/Frameworks/" + lib + ".framework";
                  if( e_dexists( rootLibraryPath )){
                    e_msgf( "Found framework %s", ccp( lib.basename() ));
                    files.push( File( rootLibraryPath.os() ));
                    return;
                  }

                  //------------------------------------------------------------
                  // Test whether the intent was to link with the system libs.
                  //------------------------------------------------------------

                  const auto& homeLibraryPath = "~/Library/Frameworks/" + lib + ".framework";
                  if( e_dexists( homeLibraryPath )){
                    e_msgf( "Found framework %s", ccp( lib.basename() ));
                    files.push( File( homeLibraryPath.os() ));
                    return;
                  }

                  //------------------------------------------------------------
                  // Test whether it's a framework inside Xcode app bundle.
                  //------------------------------------------------------------

                  if( e_dexists( "/Applications/Xcode.app" )){
                    auto path = e_xfs(
                      "/Applications/Xcode.app/Contents/Developer/Platforms"
                      "/MacOSX.platform/Developer/SDKs/MacOSX%s.sdk/System"
                      "/Library/Frameworks/"
                      , ccp( toSdkVersion() ));
                    path += lib + ".framework";
                    if( e_dexists( path )){
                      e_msgf( "Found framework %s", ccp( path.basename() ));
                      files.push( File( path.os() ));
                      return;
                    }
                  }

                  //------------------------------------------------------------
                  // Test whether it's a project framework or bundle.
                  //------------------------------------------------------------

                  bool found = false;
                  Class::foreachs<Xcode>(
                    [&]( const Xcode& xcode ){
                      if( this == &xcode ){
                        return true;
                      }
                      if( lib == xcode.toLabel() ){
                        if( xcode.toLabel().ext().tolower() == ".dylib"_64 ){
                          const auto& label =
                              xcode.toLabel()
                            + "."
                            + xcode.toBuild();
                          e_msgf(
                            "Found dylibs %s"
                            , ccp( lib ));
                          File f( label.os() );
                          if( !isNoEmbedAndSign() ){
                            f.setEmbed( true );
                            f.setSign( true );
                            const_cast<Xcode*>( this )
                              -> toEmbedFiles().push( f )
                            ;
                          }
                          files.push( f );
                          found = true;
                          return false;
                        }
                        switch( xcode.toBuild().hash() ){
                          case"dylib"_64:
                            [[fallthrough]];
                          case"bundle"_64:/**/{
                            const auto& label =
                                xcode.toLabel()
                              + "."
                              + xcode.toBuild();
                            e_msgf(
                              "Found bundle %s"
                              , ccp( lib ));
                            File f( label.os() );
                            if( !isNoEmbedAndSign() ){
                              f.setEmbed( true );
                              f.setSign( true );
                              const_cast<Xcode*>( this )
                                -> toEmbedFiles().push( f )
                              ;
                            }
                            files.push( f );
                            found = true;
                            return false;
                          }
                          case"framework"_64:/**/{
                            const auto& label =
                                xcode.toLabel()
                              + "."
                              + xcode.toBuild();
                            e_msgf(
                              "Found framework %s"
                              , ccp( lib ));
                            File f( label.os() );
                            if( !isNoEmbedAndSign() ){
                              f.setEmbed( true );
                              f.setSign( true );
                              const_cast<Xcode*>( this )
                                -> toEmbedFiles().push( f )
                              ;
                            }
                            files.push( f );
                            found = true;
                            return false;
                          }
                        }
                      }
                      return true;
                    }
                  );
                  if( found ){
                    return;
                  }
                }

                //**************************************************************

                //--------------------------------------------------------------
                // Handle pathing directly to desired library. If we don't find
                // it here then the find_frameworks and find_library calls will
                // pick it up for us.
                //--------------------------------------------------------------

                const auto& osLib = lib.os();
                const auto& osExt = osLib.ext().tolower();
                auto embedAndSign = false;
                switch( osExt.hash() ){
                  case".dylib"_64:
                    embedAndSign = true;
                    [[fallthrough]];
                  case".a"_64:
                    switch( *osLib ){
                      case'~':
                        [[fallthrough]];
                      case'/':
                        [[fallthrough]];
                      case'.':
                        if( e_fexists( osLib )){
                          File f( osLib.os() );
                          files.push( File( osLib.os() ));
                          return;
                        }
                        break;
                      default:
                        if( e_fexists( osLib )){
                          File f( "../" + osLib.os() );
                          if( embedAndSign ){
                            f.setEmbed( true );
                            f.setSign(  true );
                            const_cast<Xcode*>( this )->toEmbedFiles().push( f );
                            e_msgf( "Found library %s (embed/sign)"
                              , ccp( lib.basename() ))
                            ;
                          }else{
                            e_msgf( "Found library %s"
                              , ccp( lib.basename() ))
                            ;
                          }
                          files.push( f );
                          return;
                        }
                        break;
                    }
                    break;
                  case".framework"_64:
                    [[fallthrough]];
                  case".bundle"_64:/**/{
                    switch( *osLib ){
                      case'~':
                        [[fallthrough]];
                      case'/':
                        [[fallthrough]];
                      case'.':
                        if( e_dexists( osLib )){
                          File f( osLib.os() );
                          files.push( File( osLib.os() ));
                          return;
                        }
                        break;
                      default:/**/{
                        if( e_dexists( osLib )){
                          e_msgf( "Found %s %s"
                            , ccp( osExt )
                            , ccp( lib.basename() ));
                          files.push( File( "../" + osLib ));
                          return;
                        }
                        break;
                      }
                    }
                    break;
                  }
                }

                //**************************************************************

                //--------------------------------------------------------------
                // If you specify just "name.framework" then we'll do a search
                // the find_frameworks() vector and embed and sign.
                //--------------------------------------------------------------

                const auto& fn = lib
                  . filename();
                const auto& ln = toFrameworkPaths()
                  . splitAtCommas();
                const ccp nm[]{
                  "Debug",
                  "Release"
                };
                for( u32 n=e_dimof( nm ), i=0; i<n; ++i ){
                  bool stop = false;
                  ln.foreachs(
                    [&]( const string& in_st ){
                      string st( in_st );
                      st.replace( "$(CONFIGURATION)", nm[ i ]);
                      // If the filenames don't match return.
                      if( !e_dexists( st + "/" + fn )){
                        return true;
                      }
                      if(( *st != '/' ) && ( *st != '~' ) && ( *st != '.' )){
                        st = "../" + st;
                      }
                      // Construct a file object for embedding later.
                      File file( st + "/" + fn );
                      file.setStrip( true );
                      file.setEmbed( true );
                      file.setSign(  true );
                      files.push(    file );

                      // Also add to embedding files vector.
                      const_cast<Xcode*>( this )->toEmbedFiles().push( file );
                      stop = true;
                      return!stop;
                    }
                  );
                  if( !stop ){
                    continue;
                  }
                  break;
                }

                //**************************************************************

                //--------------------------------------------------------------
                // Handle .a and .dylib extensions.
                //--------------------------------------------------------------

                const auto& libExt = lib.ext().tolower().hash();
                if(( libExt != ".framework"_64 )&&( libExt != ".bundle"_64 )){
                  File f( lib.os() );

                  //------------------------------------------------------------
                  // Searching for future (not compiled yet) libraries.
                  //------------------------------------------------------------

                  if( !e_fexists( lib )){
                    Class::foreachs<Xcode>(
                      [&]( const Xcode& xcode ){
                        if( lib.left( 3 ) == "lib"_64 ){
                          const auto& s = lib.ltrimmed( 3 ).basename();
                          if( xcode.toLabel() == s ){
                            switch( xcode.toBuild().hash() ){
                              case"shared"_64:
                                [[fallthrough]];
                              case"static"_64:/**/{
                                files.push( File( lib ));
                                e_msgf(
                                  "Found future library %s"
                                  , ccp( lib ));
                                break;
                              }
                            }
                            return false;
                          }
                        }
                        return true;
                      }
                    );
                    return;
                  }

                  //------------------------------------------------------------
                  // Existing libraries, frameworks and plugin bundles.
                  //------------------------------------------------------------

                  e_msgf( "Found library %s", ccp( lib ));
                  if( *lib == '.' ){
                    files.push( f );
                  }else if( *lib == '/' ){
                    files.push( f );
                  }else if( *lib == '~' ){
                    files.push( f );
                  }else{
                    files.push( File(( "../" + lib ).os() ));
                  }
                }
              }
            );

            //------------------------------------------------------------------
            // Set library files.
            //------------------------------------------------------------------

            const_cast<Xcode*>( this )->setLibFiles( files );

            //------------------------------------------------------------------
            // Write out the file and embedding line.
            //------------------------------------------------------------------

            const auto embedAndSign = toEmbedAndSign();
            const auto& vectorsSign = embedAndSign.splitAtCommas();
            { files.foreach(
                [&]( File& file ){
                  if( file.empty() ){
                    return;
                  }
                  const auto/* no & */fileExt = file.ext().tolower().hash();
                  vectorsSign.foreach(
                    [&]( const string& f ){
                      if( strstr( file, f )){
                        e_msgf( "  $(lightblue)Embedding $(off)%s"
                            , ccp( file.basename() ));
                        file.setEmbed( true );
                        file.setSign( true );
                      }
                    }
                  );
                  if( file.isEmbed() ){

                    //------------------------------------------------------------
                    // Reference in frameworks.
                    //------------------------------------------------------------

                    out << "    "
                      + file.toBuildID()
                      + " /* "
                      + file.filename();
                    if(( fileExt == ".framework"_64 )||( fileExt == ".dylib"_64 )){
                      out << " in Frameworks */ = {isa = PBXBuildFile; fileRef = ";
                    }else if( fileExt == ".bundle"_64 ){
                      out << " in PlugIns */ = {isa = PBXBuildFile; fileRef = ";
                    }else{
                      out << " */ = {isa = PBXBuildFile; fileRef = ";
                    }
                    out << file.toFileRefID()
                      + " /* "
                      + file.filename()
                      + " */; };\n";

                    //------------------------------------------------------------
                    // Reference in embedded frameworks.
                    //------------------------------------------------------------

                    out << "    "
                      + file.toEmbedID()
                      + " /* "
                      + file.filename();
                    if( fileExt == ".framework"_64 ){
                      out << " in Embed Frameworks */ = {isa = PBXBuildFile; fileRef = ";
                    }else if( fileExt == ".bundle"_64 ){
                      out << " in Embed Bundles */ = {isa = PBXBuildFile; fileRef = ";
                    }else if( fileExt == ".dylib"_64 ){
                      out << " in Embed Dylibs */ = {isa = PBXBuildFile; fileRef = ";
                    }else{
                      out << " */ = {isa = PBXBuildFile; fileRef = ";
                    }
                    out << file.toFileRefID()
                      + " /* "
                      + file.filename()
                      + " */; settings = {ATTRIBUTES = (";
                    if( file.isSign() ){
                      out << "CodeSignOnCopy, ";
                    }
                    if(( fileExt == ".framework"_64 ) && file.isStrip() ){
                      out << "RemoveHeadersOnCopy, ";
                    }
                    out << "); }; };\n";
                  }else{
                    switch( fileExt ){
                      case".framework"_64:
                        [[fallthrough]];
                      case".bundle"_64:
                        [[fallthrough]];
                      case".dylib"_64:
                        [[fallthrough]];
                      case".tbd"_64:
                        [[fallthrough]];
                      case".a"_64:
                        out << "    "
                          + file.toBuildID()
                          + " /* "
                          + file.filename();
                        break;
                      default:
                        e_errorf( 2987234, "Unhandled library type" );
                        return;
                    }
                    switch( fileExt ){
                      case".framework"_64:
                        out << " in Frameworks */ = {isa = PBXBuildFile; fileRef = ";
                        break;
                      case".tbd"_64:
                        out << " in TBDs */ = {isa = PBXBuildFile; fileRef = ";
                        break;
                      case".bundle"_64:
                        out << " in PlugIns */ = {isa = PBXBuildFile; fileRef = ";
                        break;
                      case".dylib"_64:
                        out << " in Dynamics */ = {isa = PBXBuildFile; fileRef = ";
                        break;
                      case".a"_64:
                        out << " in Statics */ = {isa = PBXBuildFile; fileRef = ";
                        break;
                    }
                    out << file.toFileRefID()
                      + " /* "
                      + file.filename()
                      + " */; };\n"
                    ;
                  }
                }
              );
            }
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
              out << "    "
                + file.toBuildID()
                + " /* "
                + file.filename()
                + " in Resources */ = {isa = PBXBuildFile; fileRef = "
                + file.toFileRefID()
                + " /* "
                + file.filename()
                + " */; };\n"
              ;
            }
          );

          //--------------------------------------------------------------------
          // CopyFile references.
          //--------------------------------------------------------------------

          files.clear();
          files.pushVector( toPublicRefs() );
          files.foreach(
            [&]( File& file ){
              if( file.empty() ){
                return;
              }
              out << "    "
                + file.toBuildID()
                + " /* "
                + file.filename()
                + " in CopyFiles */ = {isa = PBXBuildFile; fileRef = "
                + file.toFileRefID()
                + " /* "
                + file.filename()
                + " */; };\n"
              ;
            }
          );

          //--------------------------------------------------------------------
          // Headers files.
          //--------------------------------------------------------------------

          if( !toPublicHeaders().empty() ){
            files.clear();
            files.pushVector( toPublicHeaders() );
            files.foreach(
              [&]( File& file ){
                if( file.empty() ){
                  return;
                }
                out << "    "
                  + file.toBuildID()
                  + " /* "
                  + file.filename()
                  + " in Headers */ = {isa = PBXBuildFile; fileRef = "
                  + file.toFileRefID()
                  + " /* "
                  + file.filename()
                  + " */; settings = {ATTRIBUTES = (Public, ); }; };\n"
                ;
              }
            );
          }

          //--------------------------------------------------------------------
          // Ignore files.
          //--------------------------------------------------------------------

          const auto& onIgnore = [this]( vector<File>::iterator it ){
            while( it ){
              auto ok = false;
              { auto parts = toIgnoreParts();
                parts.erase( "\n" );
                parts.erase( "\t" );
                parts.erase( " " );
                const auto& splits = parts.splitAtCommas();
                splits.foreachs(
                  [&]( const string& split ){
                    if( isIgnoreFile( split, *it )){
                      e_msgf( "  Ignoring %s", ccp( it->filename() ));
                      ok = true;
                      return false;
                    }
                    return true;
                  }
                );
              }
              if( ok ){
                it.erase();
                continue;
              }
              ++it;
            }
          };
          onIgnore( const_cast<Xcode*>( this )
            -> inSources( Type::kCpp ).getIterator() );
          onIgnore( const_cast<Xcode*>( this )
            -> inSources( Type::kMm  ).getIterator() );
          onIgnore( const_cast<Xcode*>( this )
            -> inSources( Type::kC   ).getIterator() );
          onIgnore( const_cast<Xcode*>( this )
            -> inSources( Type::kM   ).getIterator() );

          //--------------------------------------------------------------------
          // Source files.
          //--------------------------------------------------------------------

          files.clear();
          files.pushVector( inSources( Type::kCpp ));
          files.pushVector( inSources( Type::kMm  ));
          files.pushVector( inSources( Type::kM   ));
          files.pushVector( inSources( Type::kC   ));

          //--------------------------------------------------------------------
          // Add files.
          //--------------------------------------------------------------------

          files.foreach(
            [&]( File& file ){
              if( file.empty() ){
                return;
              }
              out << "    "
                + file.toBuildID()
                + " /* "
                + file.filename()
                + " in Sources */ = {isa = PBXBuildFile; fileRef = "
                + file.toFileRefID()
                + " /* "
                + file.filename()
                + " */; };\n"
              ;
            }
          );
        out << "    /* End PBXBuildFile section */\n";
      }

      void Workspace::Xcode::writePBXCopyFilesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXCopyFilesBuildPhase section */\n";
        if(( toBuild() == "application"_64 )
         ||( toBuild() == "console"_64 )
         ||( toBuild() == "framework"_64 )
         ||( toBuild() == "bundle"_64 )){

          //--------------------------------------------------------------------
          // Local lambda function to embed files.
          //--------------------------------------------------------------------

          const auto& writePBXCopyFilesBuildPhase = [&fs](
                const auto& subfolderSpec
              , const auto& files
              , const auto& id
              , const auto& comment
              , const std::function<void( const File& )>& lbuild
              , const std::function<void()>& lambda ){
            if( !files.empty() ){
              fs << "    " + id + " /* " + comment + " */ = {\n";
              fs << "      isa = PBXCopyFilesBuildPhase;\n";
              fs << "      buildActionMask = 2147483647;\n";
              fs << "      dstPath = \"\";\n";
              fs << "      dstSubfolderSpec = " + subfolderSpec + ";\n";
              fs << "      files = (\n";
              files.foreach(
                [&]( const File& file ){
                  lbuild( file );
                }
              );
              fs << "      );\n";
              if( lambda ){
                lambda();
              }
              fs << "      runOnlyForDeploymentPostProcessing = 0;\n";
              fs << "    };\n";
            }
          };

          //--------------------------------------------------------------------
          // Copy references into resources folder.
          //--------------------------------------------------------------------

          writePBXCopyFilesBuildPhase(
              string( "7"/* CopyFiles */)
            , toPublicRefs()
            , toCopyReferences()
            , string( "CopyFiles" )
            , [&]( const File& f ){
                fs << "        ";
                fs << f.toBuildID();
                fs << " /* "
                  + f.filename()
                  + " in CopyFiles */,\n"
                ;
              }
            , nullptr
          );

          //--------------------------------------------------------------------
          // Copy embedded frameworks and dylibs etc into the Frameworks folder.
          //--------------------------------------------------------------------

          writePBXCopyFilesBuildPhase(
              string( "13"/* PlugIns CopyFiles */)
            , toEmbedFiles()
            , toEmbedPlugIns()
            , string( "CopyFiles" )
            , [&]( const File& f ){
                switch( f.ext().tolower().hash() ){
                  case".bundle"_64:
                    fs << "        ";
                    fs << f.toEmbedID();
                    fs << " /* "
                      + f.filename()
                      + " in CopyFiles */,\n";
                    break;
                }
              }
            , nullptr
          );

          //--------------------------------------------------------------------
          // Copy embedded frameworks and dylibs etc into the Frameworks folder.
          // DB0FA58B2758022D00CA287A /* Embed Frameworks */ = {
          //   isa = PBXCopyFilesBuildPhase;
          //   buildActionMask = 2147483647;
          //   dstPath = "";
          //   dstSubfolderSpec = 10;
          //   files = (
          //      DB0FA58C2758023000CA287A /* libfbxsdk.dylib in Embed Frameworks */,
          //      DB0FA58A2758022D00CA287A /* bfg.framework in Embed Frameworks */,
          //   );
          //   name = "Embed Frameworks";
          //   runOnlyForDeploymentPostprocessing = 0;
          // };
          //--------------------------------------------------------------------

          writePBXCopyFilesBuildPhase(
              string( "10"/* Frameworks */)
            , toEmbedFiles()
            , toEmbedFrameworks()
            , string( "Embed Frameworks" )
            , [&]( const File& f ){
                switch( f.ext().tolower().hash() ){
                  case".framework"_64:
                    [[fallthrough]];
                  case".dylib"_64:
                    fs << "        ";
                    fs << f.toEmbedID();
                    fs << " /* "
                      + f.filename()
                      + " in Embed Frameworks */,\n";
                    break;
                }
              }
            , [&](){
              fs << "      name = \"Embed Frameworks\";\n";
            }
          );
        }
        fs << "    /* End PBXCopyFilesBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXFileReferenceSection( Writer& fs )const{
        fs << "\n    /* Begin PBXFileReference section */\n";

        //----------------------------------------------------------------------
        // Source files.
        //----------------------------------------------------------------------

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
        anon_writeFileReference( fs, toPublicRefs(),                 "folder"              );

        //----------------------------------------------------------------------
        // Header files.
        //----------------------------------------------------------------------

        toPublicHeaders().foreach(
          [&]( const File& f ){
            string lastKnownFileType;
            switch( f.tolower().ext().hash() ){
              case".h"_64:
                lastKnownFileType = "sourcecode.c.h";
                break;
              default:
                lastKnownFileType = "folder";
                break;
            }
            fs << "    "
              + f.toFileRefID()
              + " = {isa = PBXFileReference; lastKnownFileType = "
              + lastKnownFileType
              + "; name = "
              + f.filename()
              + "; path = ../"
              + f
              + "; sourceTree = \"<group>\"; };\n"
            ;
          }
        );

        //----------------------------------------------------------------------
        // Library files.
        //----------------------------------------------------------------------

        toLibFiles().foreach(
          [&]( const auto& lib ){
            auto isProject = false;
            Class::foreachs<Xcode>(
              [&]( const auto& xcode ){
                if( this == &xcode ){
                  return true;
                }
                if( lib.basename() == xcode.toLabel() ){
                  isProject = true;
                  return false;
                }
                return true;
              }
            );
            string fileType;
            File f( lib );
            const auto ext = f.ext().tolower().hash();
            switch( ext ){
              case".framework"_64:
                fileType = "wrapper.framework";
                break;
              case".bundle"_64:
                fileType = "wrapper.cfbundle";
                break;
              case".dylib"_64:
                fileType = "\"compiled.mach-o.dylib\"";
                break;
              case".tbd"_64:
                fileType = "\"sourcecode.text-based-dylib-definition\"";
                break;
              case".a"_64:
                fileType = "archive.ar";
                break;
              default:
                return;
            }
            fs << "    " + f.toFileRefID();
            if( !isProject ){
              fs << " = {isa = PBXFileReference; lastKnownFileType = ";
            }else{
              fs << " = {isa = PBXFileReference; explicitFileType = ";
            }
            fs << fileType
              + "; name = "
              + f.filename()
              + "; path = ";
            switch( *f ){
              case'.':
                [[fallthrough]];
              case'~':
                [[fallthrough]];
              case'/':
                fs << f.os();
                break;
              default:
                if(( ext != ".framework"_64 )&&( ext != ".bundle"_64 )){
                  if( f.left( 3 ).tolower().hash() != "lib"_64 ){
                    fs << "lib" << f.filename();
                  }else{
                    fs << f.filename();
                  }
                }else{
                  fs << f.filename();
                }
                break;
            }
            switch( ext ){
              case".framework"_64:
                if( isProject ){
                  fs << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                }else{
                  fs << "; sourceTree = SDKROOT; };\n";
                }
                break;
              case".bundle"_64:
                if( isProject ){
                  fs << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                }else{
                  fs << "; sourceTree = \"<group>\"; };\n";
                }
                break;
              case".tbd"_64:
                fs << "; sourceTree = SDKROOT; };\n";
                break;
              case".dylib"_64:
                [[fallthrough]];
              case".a"_64:
                if( isProject ){
                  fs << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                }else{
                  fs << "; sourceTree = \"<group>\"; };\n";
                }
                break;
              default:
                fs << "; sourceTree = \"<group>\"; };\n";
                break;
            }
          }
        );

        //----------------------------------------------------------------------
        // The project.
        //----------------------------------------------------------------------

        switch( toBuild().hash() ){
          case"framework"_64:
            fs << "    "
              + m_sProductFileRef
              + " /* "
              + toLabel()
              + ".framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = "
              + toLabel()
              + ".framework; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case"bundle"_64:
            fs << "    "
               << m_sProductFileRef
               << " /* "
               << toLabel()
               << ".bundle */ = {isa = PBXFileReference; explicitFileType = wrapper.cfbundle; includeInIndex = 0; path = "
               << toLabel()
               << ".bundle; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case"shared"_64:
            fs << "    "
              + m_sProductFileRef
              + " /* lib"
              + toLabel()
              + ".dylib */ = {isa = PBXFileReference; explicitFileType = \"compiled.mach-o.dylib\"; includeInIndex = 0; path = lib"
              + toLabel()
              + ".dylib; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case"static"_64:
            fs << "    "
              + m_sProductFileRef
              + " /* lib"
              + toLabel()
              + ".a */ = {isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = lib"
              + toLabel()
              + ".a; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case"application"_64:
            fs << "    "
              + m_sProductFileRef
              + " /* "
              + toLabel()
              + " */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = "
              + toLabel()
              + "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
            break;
          case"console"_64:
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
              + "        " + m_sProductFileRef + " /* " + toLabel() + "." + toBuild() + " */,\n"
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
                fs << "        " + file.toFileRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      name = " + toIncPath().basename() + ";\n";
            fs << "      path = \"\";\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
            if( !toLibFiles().empty() ){
              fs << "    " + m_sFrameworkGroup + " /* Frameworks */ = {\n"
                  + "      isa = PBXGroup;\n"
                  + "      children = (\n";
              // m_vLibFiles has the embedded frameworks as well. No need to do
              // them twice as that causes problems in Xcode.
              toLibFiles().foreach(
                [&]( const File& f ){
                  fs << "        " + f.toFileRefID() + " /* " + f.filename() + " */,\n";
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
                fs << "        " + file.toFileRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      name = resources;\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
          }

          //--------------------------------------------------------------------
          // Exporting public headers/references from framework.
          //--------------------------------------------------------------------

          if( !toPublicHeaders().empty() || !toPublicRefs().empty() ){
            fs << "    " + m_sReferencesGroup + " /* references */ = {\n"
                + "      isa = PBXGroup;\n"
                + "      children = (\n";
            files.clear();
            files.pushVector( toPublicHeaders() );
            files.pushVector( toPublicRefs() );
            files.foreach(
              [&]( const File& file ){
                fs << "        " + file.toFileRefID() + " /* ../" + file + " */,\n";
              }
            );
            fs << "      );\n";
            fs << "      name = references;\n";
            fs << "      path = \"\";\n";
            fs << "      sourceTree = \"<group>\";\n";
            fs << "    };\n";
          }

          //--------------------------------------------------------------------
          // Source group.
          //--------------------------------------------------------------------

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
              fs << "        " + file.toFileRefID() + " /* ../" + file + " */,\n";
            }
          );
          fs << "      );\n";
          fs << "      name = src;\n";
          fs << "      path = \"\";\n";
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
            + "        " + m_sFrameworkBuildPhase   + " /* Frameworks */,\n"
            + "        " + m_sResourcesBuildPhase   + " /* Resources */,\n";
        if( !m_sCopyReferences.empty() ){
          fs << "        " + m_sCopyReferences + " /* CopyRefs */,\n";
        }
        if( !m_sHeadersBuildPhase.empty() ){
          fs << "        " + m_sHeadersBuildPhase + " /* Headers */,\n";
        }
        if( !m_sEmbedPlugIns.empty() ){
          fs << "        " + m_sEmbedPlugIns + " /* Embed PlugIns */,\n";
        }
        if( !m_sEmbedFrameworks.empty() ){
          fs << "        " + m_sEmbedFrameworks + " /* Embed Frameworks */,\n";
        }
        fs << "        " + m_sSourcesBuildPhase     + " /* Sources */,\n"
            + "        " + m_sShellScriptBuildPhase + " /* Script */,\n";
        fs << string( "      );\n" )
            + "      buildRules = (\n"
            + "      );\n"
            + "      dependencies = (\n"
            + "      );\n"
            + "      name = " + toLabel() + ";\n"
            + "      productName = " + toLabel() + ";\n";
        switch( toBuild().hash() ){
          case"framework"_64:
            fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".framework */;\n";
            fs << "      productType = \"com.apple.product-type.framework\";\n";
            break;
          case"bundle"_64:
            fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".bundle */;\n";
            fs << "      productType = \"com.apple.product-type.bundle\";\n";
            break;
          case"shared"_64:
            fs << "      productReference = " + m_sProductFileRef + " /* lib" + toLabel() + ".a */;\n";
            fs << "      productType = \"com.apple.product-type.library.dynamic\";\n";
            break;
          case"static"_64:
            fs << "      productReference = " + m_sProductFileRef + " /* lib" + toLabel() + ".a */;\n";
            fs << "      productType = \"com.apple.product-type.library.static\";\n";
            break;
          case"application"_64:
            fs << "      productReference = " + m_sProductFileRef + " /* " + toLabel() + ".app */;\n";
            fs << "      productType = \"com.apple.product-type.application\";\n";
            break;
          case"console"_64:
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
            + "      attributes = {\n";
        if( bmp->bXcode11 ){
          fs << "        LastUpgradeCheck = 1120;\n";
        }else if( bmp->bXcode12 ){
          fs << "        LastUpgradeCheck = 1200;\n";
        }
        fs << "        ORGANIZATIONNAME = \"" + toOrgName() + "\";\n"
            + "        TargetAttributes = {\n"
            + "          " + m_sFrameworkNativeTarget + " = {\n"
            + "            CreatedOnToolsVersion = 11.2.1;\n"
            + "          };\n"
            + "        };\n"
            + "      };\n"
            + "      buildConfigurationList = " + m_sBuildConfigurationList + " /* Build configuration list for PBXProject \"" + toLabel() + "\" */;\n";
        if( bmp->bXcode11 ){
          fs << "      compatibilityVersion = \"Xcode 9.3\";\n";
        }else if( bmp->bXcode12 ){
          fs << "      compatibilityVersion = \"Xcode 12.0\";\n";
        }
        fs << "      developmentRegion = en;\n"
           << "      hasScannedForEncodings = 0;\n"
           << "      knownRegions = (\n"
           << "        en,\n"
           << "        Base,\n"
           << "      );\n"
           << "      mainGroup = " + m_sMainGroup + ";\n"
           << "      productRefGroup = " + m_sProductsGroup + " /* Products */;\n"
           << "      projectDirPath = \"\";\n"
           << "      projectRoot = \"\";\n"
           << "      targets = (\n"
           << "        " + m_sFrameworkNativeTarget + " /* " + toLabel() + " */,\n"
           << "      );\n"
           << "    };\n";
        fs << "    /* End PBXProject section */\n";
      }

      void Workspace::Xcode::writePBXHeadersBuildPhaseSection( Writer& fs )const{
        if( !toPublicHeaders().empty() ){
          fs << "\n    /* Begin PBXHeadersBuildPhase section */\n";
          fs << "    " + m_sHeadersBuildPhase + " /* Headers */ = {\n"
              + "      isa = PBXHeadersBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          Files files;
          files.pushVector( toPublicHeaders() );
          files.foreach(
            [&]( const File& f ){
              fs << "        " + f.toBuildID() + " /* " + f.filename().tolower() + " in Headers */,\n";
            }
          );
          fs << "      );\n";
          fs << "      runOnlyForDeploymentPostprocessing = 0;\n";
          fs << "    };\n";
          fs << "    /* End PBXHeadersBuildPhase section */\n";
        }
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
        files.pushVector( inSources( Type::kM   ));
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

        //----------------------------------------------------------------------
        // Local lambda to write out the LDFLAGS section.
        //----------------------------------------------------------------------

        const auto& addOtherCppFlags = [&]( const string& config ){};
        const auto& addOtherLDFlags  = [&]( const string& config ){
          fs << "          -L/usr/local/lib,\n";
          fs << "          -L../lib/macOS,\n";
          auto libs = toLibraryPaths();
          libs.replace( "$(CONFIGURATION)", config );
          libs.splitAtCommas().foreach(
            [&]( const string& f ){
              auto dir = f;
              if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
                dir = "../" + f;
              }
              fs << "          -L" + dir + ",\n";
            }
          );
        };

        //----------------------------------------------------------------------
        // Begin build configuration section.
        //----------------------------------------------------------------------

        fs << "\n    /* Begin XCBuildConfiguration section */\n";
        fs << "    " + m_sDebugBuildConfiguration + " /* Debug */ = {\n"
            + "      isa = XCBuildConfiguration;\n"
            + "      buildSettings = {\n";
        auto lang( toLanguage() );
        switch( Workspace::bmp->uLanguage ){
          case 20:
            lang = "c++20";
            break;
          case 17:
            lang = "c++17";
            break;
          case 14:
            lang = "c++14";
            break;
          case 11:
            lang = "c++11";
            break;
        }
        fs << string( "        ALWAYS_SEARCH_USER_PATHS = NO;\n" )
            + "        CLANG_ANALYZER_NONNULL = YES;\n"
            + "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
            + "        CLANG_CXX_LANGUAGE_STANDARD = \"" + lang + "\";\n"
            + "        CLANG_CXX_LIBRARY = \"libc++\";\n"
            + "        CLANG_ENABLE_MODULES = YES;\n";
        string enableARC;
        if( isEnableARC() ){
          enableARC = "YES";
        }else{
          enableARC = "NO";
        }
        fs << "        CLANG_ENABLE_OBJC_ARC = " + enableARC + ";\n";
        fs << "        CLANG_ENABLE_OBJC_WEAK = YES;\n";
        if( bmp->bXcode12 ){
          fs << "        CLANG_WARN_QUOTED_INCLUDE_IN_FRAMEWORK_HEADER = YES;\n";
        }
        fs << "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
           << "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
           << "        CLANG_WARN_COMMA = YES;\n"
           << "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
           << "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
           << "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
           << "        CLANG_WARN_DOCUMENTATION_COMMENTS = YES;\n"
           << "        CLANG_WARN_EMPTY_BODY = YES;\n"
           << "        CLANG_WARN_ENUM_CONVERSION = YES;\n"
           << "        CLANG_WARN_INFINITE_RECURSION = YES;\n"
           << "        CLANG_WARN_INT_CONVERSION = YES;\n"
           << "        CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
           << "        CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
           << "        CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
           << "        CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
           << "        CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
           << "        CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
           << "        CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
           << "        CLANG_WARN_UNGUARDED_AVAILABILITY = YES_AGGRESSIVE;\n"
           << "        CLANG_WARN_UNREACHABLE_CODE = YES;\n"
           << "        CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;\n"
           << "        COPY_PHASE_STRIP = NO;\n"
           << "        CURRENT_PROJECT_VERSION = 1;\n"
           << "        DEBUG_INFORMATION_FORMAT = dwarf;\n"
           << "        ENABLE_STRICT_OBJC_MSGSEND = YES;\n"
           << "        ENABLE_TESTABILITY = YES;\n";
        fs << "        GCC_C_LANGUAGE_STANDARD = " + toLanguageC() + ";\n"
           << "        GCC_DYNAMIC_NO_PIC = NO;\n"
           << "        GCC_NO_COMMON_BLOCKS = YES;\n"
           << "        GCC_OPTIMIZATION_LEVEL = 0;\n"
           << "        GCC_PREPROCESSOR_DEFINITIONS = (\n"
           << "          \"$(inherited)\",\n";
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
            + "      buildSettings = {\n";
        if( !isUniversalBinary() ){
          fs << "        ARCHS = x86_64;\n";
        }
        fs << string( "        ALWAYS_SEARCH_USER_PATHS = NO;\n" )
            + "        CLANG_ANALYZER_NONNULL = YES;\n"
            + "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
            + "        CLANG_CXX_LANGUAGE_STANDARD = \"" + toLanguage() + "\";\n"
            + "        CLANG_CXX_LIBRARY = \"libc++\";\n"
            + "        CLANG_ENABLE_MODULES = YES;\n"
            + "        CLANG_ENABLE_OBJC_ARC = " + enableARC + ";\n"
            + "        CLANG_ENABLE_OBJC_WEAK = YES;\n";
        if( bmp->bXcode12 ){
          fs << "        CLANG_WARN_QUOTED_INCLUDE_IN_FRAMEWORK_HEADER = YES;\n";
        }
        fs << "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
           << "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
           << "        CLANG_WARN_COMMA = YES;\n"
           << "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
           << "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
           << "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
           << "        CLANG_WARN_DOCUMENTATION_COMMENTS = YES;\n"
           << "        CLANG_WARN_EMPTY_BODY = YES;\n"
           << "        CLANG_WARN_ENUM_CONVERSION = YES;\n"
           << "        CLANG_WARN_INFINITE_RECURSION = YES;\n"
           << "        CLANG_WARN_INT_CONVERSION = YES;\n"
           << "        CLANG_WARN_NON_LITERAL_NULL_CONVERSION = YES;\n"
           << "        CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF = YES;\n"
           << "        CLANG_WARN_OBJC_LITERAL_CONVERSION = YES;\n"
           << "        CLANG_WARN_OBJC_ROOT_CLASS = YES_ERROR;\n"
           << "        CLANG_WARN_RANGE_LOOP_ANALYSIS = YES;\n"
           << "        CLANG_WARN_STRICT_PROTOTYPES = YES;\n"
           << "        CLANG_WARN_SUSPICIOUS_MOVE = YES;\n"
           << "        CLANG_WARN_UNGUARDED_AVAILABILITY = YES_AGGRESSIVE;\n"
           << "        CLANG_WARN_UNREACHABLE_CODE = YES;\n"
           << "        CLANG_WARN__DUPLICATE_METHOD_MATCH = YES;\n"
           << "        COPY_PHASE_STRIP = NO;\n"
           << "        CURRENT_PROJECT_VERSION = 1;\n"
           << "        DEBUG_INFORMATION_FORMAT = \"dwarf-with-dsym\";\n"
           << "        ENABLE_NS_ASSERTIONS = NO;\n"
           << "        ENABLE_STRICT_OBJC_MSGSEND = YES;\n";
        fs << "        GCC_C_LANGUAGE_STANDARD = " + toLanguageC() + ";\n"
           << "        GCC_NO_COMMON_BLOCKS = YES;\n"
           << "        GCC_OPTIMIZATION_LEVEL = fast;\n"
           << "        GCC_PREPROCESSOR_DEFINITIONS = (\n"
           << "          \"$(inherited)\",\n";
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
            + "      buildSettings = {\n";
        if( !isUniversalBinary() ){
          fs << "        ARCHS = x86_64;\n";
        }
        fs << "        CODE_SIGN_STYLE = Automatic;\n";
        if( !toTeamName().empty() ){
          fs << "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n";
        }
        fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
        fs << "          \"$(inherited)\",\n";
        fs << "          \"@executable_path/../Frameworks\",\n";
        fs << "        );\n";
        fs << "        LIBRARY_SEARCH_PATHS = (\n";
        auto libraryPaths = toFindLibsPaths().splitAtCommas();
        libraryPaths.foreach(
          [&]( const string& f ){
            auto dir = f;
            if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
              dir = "../" + f;
            }
            dir.replace( "$(CONFIGURATION)", "Debug" );
            fs << "          " + dir + ",\n";
          }
        );
        fs << "        );\n";
        fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
        auto frameworkPaths = toFrameworkPaths().splitAtCommas();
        frameworkPaths.foreach(
          [&]( const string& f ){
            auto dir = f;
            if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
              dir = "../" + f;
            }
            dir.replace( "$(CONFIGURATION)", "Debug" );
            fs << "          " + dir + ",\n";
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
          //--------------------------------------|-----------------------------
          //application:{                         |

            case"application"_64:
              fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Debug" );
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Debug" );
              fs << "        );\n";
              break;

          //}:                                    |
          //framework:{                           |

            case"framework"_64:
              fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../Frameworks\",\n";
              fs << "          \"@loader_path/Frameworks\",\n";
              fs << "        );\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Debug" );
              fs << "        );\n";
              fs << "        OTHER_CFLAGS = (\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Debug" );
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;

          //}:                                    |
          //bundle:{                              |

            case"bundle"_64:
              fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/PlugIns\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../PlugIns\",\n";
              fs << "          \"@loader_path/PlugIns\",\n";
              fs << "        );\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Debug" );
              fs << "        );\n";
              fs << "        OTHER_CFLAGS = (\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Debug" );
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;

          //}:                                    |
          //console:{                             |

            case"console"_64:
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Debug" );
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Debug" );
              fs << "        );\n";
              break;

          //}:                                    |
          //shared:{                              |

            case"shared"_64:
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/PlugIns\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../PlugIns\",\n";
              fs << "          \"@loader_path/PlugIns\",\n";
              fs << "        );\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Debug" );
              fs << "        );\n";
              fs << "        OTHER_CFLAGS = (\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Debug" );
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;

          //}:                                    |
          //static:{                              |

            case"static"_64:
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        EXECUTABLE_PREFIX = lib;\n";
              break;

          //}:                                    |
          //--------------------------------------|-----------------------------
        }
        fs << "        SKIP_INSTALL = YES;\n";
        if( toBuild() == "bundle"_64 ){
          fs << "        WRAPPER_EXTENSION = bundle;\n";
        }
        fs << "      };\n";
        fs << "      name = Debug;\n";
        fs << "    };\n";

        //----------------------------------------------------------------------
        // Handle all the build types: Release.
        //----------------------------------------------------------------------

        fs << "    " + m_sReleaseNativeBuildConfig + " /* Release */ = {\n"
            + "      isa = XCBuildConfiguration;\n"
            + "      buildSettings = {\n"
            + "        CODE_SIGN_STYLE = Automatic;\n";
        if( !toTeamName().empty() ){
          fs << "        DEVELOPMENT_TEAM = " + toTeamName() + ";\n";
        }
        fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
        fs << "          \"$(inherited)\",\n";
        fs << "          \"@executable_path/../Frameworks\",\n";
        fs << "        );\n";
        fs << "        LIBRARY_SEARCH_PATHS = (\n";
        libraryPaths.foreach(
          [&]( const string& f ){
            auto dir = f;
            if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
              dir = "../" + f;
            }
            dir.replace( "$(CONFIGURATION)", "Release" );
            fs << "          " + dir + ",\n";
          }
        );
        fs << "        );\n";
        fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
        frameworkPaths.foreach(
          [&]( const string& f ){
            auto dir = f;
            if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
              dir = "../" + f;
            }
            dir.replace( "$(CONFIGURATION)", "Release" );
            fs << "          " + dir + ",\n";
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
          //--------------------------------------|-----------------------------
          //application:{                         |

            case"application"_64:
              fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Release" );
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Release" );
              fs << "        );\n";
              break;

          //}:                                    |
          //framework:{                           |

            case"framework"_64:
              fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../Frameworks\",\n";
              fs << "          \"@loader_path/Frameworks\",\n";
              fs << "        );\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Release" );
              fs << "        );\n";
              fs << "        OTHER_CFLAGS = (\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Release" );
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;

          //}:                                    |
          //bundle:{                              |

            case"bundle"_64:
              fs << "        COMBINE_HIDPI_IMAGES = YES;\n";
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/PlugIns\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../PlugIns\",\n";
              fs << "          \"@loader_path/PlugIns\",\n";
              fs << "        );\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Debug" );
              fs << "        );\n";
              fs << "        OTHER_CFLAGS = (\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Debug" );
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;

          //}:                                    |
          //console:{                             |

            case"console"_64:
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Release" );
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Release" );
              fs << "        );\n";
              break;

          //}:                                    |
          //shared:{                              |

            case"shared"_64:
              fs << "        DEFINES_MODULE = YES;\n";
              fs << "        DYLIB_COMPATIBILITY_VERSION = 1;\n";
              fs << "        DYLIB_CURRENT_VERSION = 1;\n";
              fs << "        DYLIB_INSTALL_NAME_BASE = \"@rpath\";\n";
              if( !toPlistPath().empty() ){
                fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
              }
              fs << "        INSTALL_PATH = \"$(LOCAL_LIBRARY_DIR)/Frameworks\";\n";
              fs << "        LD_RUNPATH_SEARCH_PATHS = (\n";
              fs << "          \"$(inherited)\",\n";
              fs << "          \"@executable_path/../Frameworks\",\n";
              fs << "          \"@loader_path/Frameworks\",\n";
              fs << "        );\n";
              fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
              addOtherCppFlags( "Release" );
              fs << "        );\n";
              fs << "        OTHER_CFLAGS = (\n";
              fs << "        );\n";
              fs << "        OTHER_LDFLAGS = (\n";
              if( isLoadAllSymbols() ){
                fs << "          -all_load,\n";
              }
              addOtherLDFlags( "Release" );
              fs << "        );\n";
              fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId + "\";\n";
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
              break;

          //}:                                    |
          //static:{                              |

            case"static"_64:
              fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
              fs << "        EXECUTABLE_PREFIX = lib;\n";
              break;

          //}:                                    |
          //--------------------------------------|-----------------------------
        }
        fs << "        SKIP_INSTALL = YES;\n";
        if( toBuild() == "bundle"_64 ){
          fs << "        WRAPPER_EXTENSION = bundle;\n";
        }
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
//Ctor:{                                          |

  Workspace::Xcode::Xcode(){}

//}:                                              |
//================================================|=============================
