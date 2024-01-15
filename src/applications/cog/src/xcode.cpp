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

//http://www.monobjc.net/xcode-project-file-format.html
//libtool -static -o new.a old1.a old2.a

#include"generators.h"
#include"luacore.h"
#include"std.h"
#include"ws.h"
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_extends( Workspace::Xcode );

//}:                                              |
//Statics:{                                       |

  namespace{
    hashmap<u64,s8>keyCache;
    hashmap<u64,s8>libCache;
    hashmap<u64,s8>grpCache;
  }

//}:                                              |
//Private:{                                       |
  //normalizeInstallScript:{                      |

#ifdef __APPLE__
  #pragma mark (private)
#endif

    namespace{
      string anon_normalizeInstallScript( const string& inScript ){
        string r( inScript );
        r.replace( "\"", "\\\"" );
        r.replace( "\n", "\\n" );
        return r;
      }
    }

  //}:                                            |
//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //addToPBX*PhaseSection:{                     |

      void Workspace::Xcode::addToPBXSourcesBuildPhaseSection( Writer& fs
            , const std::function<void( const string& source )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          if( target == "macos"_64 ){
            lambda( m_aSourcesBuildPhase[ Target::macOS ]);
          }else{
            lambda( m_aSourcesBuildPhase[ Target::iOS ]);
          }
          ++it;
        }
      }

      void Workspace::Xcode::addToPBXNativeTargetSection( Writer& fs
        , const std::function<void( const string& target
                                  , const string& label
                                  , const string& build
                                  , const string& frame
                                  , const string& phaseFramework
                                  , const string& phaseResources
                                  , const string& phaseHeaders
                                  , const string& phaseSources
                                  , const string& phaseScript
                                  , const string& embedFrameworks
                                  , const string& embedPlugins
                                  , const string& productFileRef
                                  , const string& copyRefs )>& lambda )const{

        //----------------------------------------------------------------------
        // Select platform from macOS or iOS.
        //----------------------------------------------------------------------

        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          string buildNativeTarget;
          string frameworkNativeTarget;
          string phaseNativeFramework;
          string phaseResources;
          string phaseNativeHeaders;
          string embedNativeFrameworks;
          string embedNativePlugins;
          string phaseNativeSources;
          string phaseNativeScript;
          string productFileRef;
          string copyRefs;
          auto label( toLabel() );
          if( target == "macos"_64 ){
            frameworkNativeTarget = m_aFrameNativeTarget  [ Target::macOS ];
            buildNativeTarget     = m_aBuildNativeTarget  [ Target::macOS ];
            phaseNativeFramework  = m_aFrameworkBuildPhase[ Target::macOS ];
            phaseResources        = m_aResourcesBuildPhase[ Target::macOS ];
            phaseNativeHeaders    = m_aHeadersBuildPhase  [ Target::macOS ];
            phaseNativeSources    = m_aSourcesBuildPhase  [ Target::macOS ];
            embedNativeFrameworks = m_aFrameworksEmbed    [ Target::macOS ];
            embedNativePlugins    = m_aPluginsEmbed       [ Target::macOS ];
            productFileRef        = m_aProductFileRef     [ Target::macOS ];
            copyRefs              = m_aCopyRefs           [ Target::macOS ];
          }else if( target == "ios"_64 ){
            frameworkNativeTarget = m_aFrameNativeTarget  [ Target::iOS ];
            buildNativeTarget     = m_aBuildNativeTarget  [ Target::iOS ];
            phaseNativeFramework  = m_aFrameworkBuildPhase[ Target::iOS ];
            phaseResources        = m_aResourcesBuildPhase[ Target::iOS ];
            phaseNativeHeaders    = m_aHeadersBuildPhase  [ Target::iOS ];
            phaseNativeSources    = m_aSourcesBuildPhase  [ Target::iOS ];
            embedNativeFrameworks = m_aFrameworksEmbed    [ Target::iOS ];
            embedNativePlugins    = m_aPluginsEmbed       [ Target::iOS ];
            productFileRef        = m_aProductFileRef     [ Target::iOS ];
            copyRefs              = m_aCopyRefs           [ Target::iOS ];
            label << "ios";
          } ++it;
          lambda(
              target
            , label
            , buildNativeTarget
            , frameworkNativeTarget
            , phaseNativeFramework
            , phaseResources
            , phaseNativeHeaders
            , phaseNativeSources
            , phaseNativeScript
            , embedNativeFrameworks
            , embedNativePlugins
            , productFileRef
            , copyRefs
          );
        }
      }

      void Workspace::Xcode::addToPBXGroupSection( Writer& fs
          , const std::function<void(
            const string& product
          , const string& target
          , const string& label )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          string product;
          string label( toLabel() );
          if( target == "macos"_64 ){
            product = m_aProductFileRef[ Target::macOS ];
          }else{
            product = m_aProductFileRef[ Target::iOS ];
            label = "ios";
          }
          lambda( product
            , target
            , label );
          ++it;
        }
      }

      void Workspace::Xcode::addToPBXShellScriptBuildPhaseSection( Writer& fs
          , const std::function<void(
            const string& target
          , const string& shellScript )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          //TODO: Do something!
          ++it;
        }
      }

      void Workspace::Xcode::addToPBXFileReferenceSection( Writer& fs,
          const std::function<void(
              const string& target
            , const string& label
            , const string& prod )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          string label;
          string prod;
          if( target == "macos"_64 ){
            prod = m_aProductFileRef[ Target::macOS ];
          }else{
            prod = m_aProductFileRef[ Target::iOS ];
            label = "ios";
          }
          lambda( target
            , label
            , prod );
          ++it;
        }
      }

    //}:                                          |
    //writeFileReference:{                        |

#ifdef __APPLE__
  #pragma mark - Xcode -
#endif

      void Workspace::Xcode::writeFileReference( Writer& fs
          , const string& refId
          , const string& _path
          , const string& _name
          , const string& _sect )const{
        // Note _path ends with /
        File _file( _path + _name );
        fs << "    "
           << refId
           << " = {isa = PBXFileReference; lastKnownFileType = "
           << _sect
           << "; name = "
           << _name
           << "; path = "
           << ( lookfor( _file ) ? _file.toWhere() : ( "../" + _file ));
        fs << "; sourceTree = "
           << "\"<group>\"";
        fs << "; };\n";
      }

      void Workspace::Xcode::writeFileReference( Writer& fs
          , const Files& files
          , const string& projectType )const{
        auto paths( files );
        ignore( paths, toIgnoreParts() );
        if( !paths.sort(
          []( const auto& a, const auto& b ){
            return(
                a.filename().tolower()
              < b.filename().tolower()
            );
          }
        )){// If true bail.
          return;
        }
        const auto& targets = getTargets();
        paths.foreach(
          [&]( const auto& f ){
            auto it = targets.getIterator();
            while( it ){
              const auto& target = *it; ++it;
              switch( target.hash() ){
                case "macos"_64:
                  writeFileReference( fs
                    , f.toFileRefID()
                    , f.path()
                    , f.filename()
                    , projectType );
                  break;
                case "ios"_64:/**/{
                  const auto& ext = f
                    . ext()
                    . tolower();
                  string name;
                  switch( ext.hash() ){
                    case".framework"_64:
                      [[fallthrough]];
                    case".a"_64:/**/{
                      const auto& iosPath = f.tolower();
                      const auto& iosName = iosPath
                        + f.basename()
                        + ext;
                      writeFileReference( fs
                        , f.toFileRefID()
                        , f.path()
                        , iosName
                        , projectType );
                      break;
                    }
                    case".bundle"_64:
                      [[fallthrough]];
                    case".dylib"_64:
                      // No support on iOS for bundles or dylibs.
                      break;
                    default:/**/{
                      writeFileReference( fs
                        , f.toFileRefID()
                        , f.path()
                        , f.filename()
                        , projectType );
                      break;
                    }
                  }
                  break;
                }
              }
            }
          }
        );
      }

    //}:                                          |
    //write*PhaseSection:{                        |

      void Workspace::Xcode::writePBXShellScriptBuildPhaseSection( Writer& fs )const{
        if( toInstallScript().empty() )
          return;
        fs << "\n    /* Begin PBXShellScriptBuildPhase section */\n";
        addToPBXShellScriptBuildPhaseSection( fs,
          [&]( const auto& target
             , const auto& shellScript ){
            fs << "    " + shellScript
              + " /* ShellScript */ = {\n"
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
              + "      shellScript = \""
              + anon_normalizeInstallScript( toInstallScript() )
              + "\";\n"
              + "    };\n"
            ;
          }
        );
        fs << "    /* End PBXShellScriptBuildPhase section */\n";
      }

      void Workspace::Xcode::writeLibraries( Writer& out )const{

        //----------------------------------------------------------------------
        // SDK locations.
        //----------------------------------------------------------------------

        static constexpr const ccp kMacSdkUsrLib13_3 =
          "/Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/usr/lib/";
        static constexpr const ccp kMacSdkUsrLib12_3 =
          "/Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/usr/lib/";
        static constexpr const ccp kMacSdkUsrLib =
          "/Applications/Xcode.app/Contents/Developer/Platforms/"
          "MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib/";
        static constexpr const ccp kIosSdkUsrLib =
          "/Applications/Xcode.app/Contents/Developer/Platforms/"
          "iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/lib/";

        //----------------------------------------------------------------
        // Lambdas for testing different TBD paths on macOS and iOS.
        //----------------------------------------------------------------

        static const auto& getPath=[]( const auto hash
            , const auto& lib )->strings{
          strings out;
          if( hash == "macos"_64 ){
            out.push( kMacSdkUsrLib13_3 );
            out.push( kMacSdkUsrLib12_3 );
            out.push( kMacSdkUsrLib );
          }else if( hash == "ios"_64 ){
            out.push( kIosSdkUsrLib );
          }
          return out;
        };

        //----------------------------------------------------------------------
        // Walk all the linker candidates including frameworks and .tbd's.
        //----------------------------------------------------------------------

        Files files;
        if( !toLinkWith().empty() ){
          const auto& libs = toLinkWith().splitAtCommas();
          libs.foreach(
            [&]( const auto& lib ){

              //----------------------------------------------------------------
              // Bail conditions.
              //----------------------------------------------------------------

              if( lib.empty() )
                return;

              //****************************************************************

              //----------------------------------------------------------------
              // Test whether the intent was to link with a TBD file.
              //----------------------------------------------------------------

              static const auto xcodeExists =
                  e_dexists( "/Applications/Xcode.app" );
              const auto& ext = lib.ext().tolower( );
              if( xcodeExists ){
                const auto& targets = getTargets();
                auto it = targets.getIterator();
                while( it ){
                  const auto paths( getPath( lib.hash(), lib ));
                  auto stop = false;
                  paths.foreachs(
                    [&]( const auto& input )->bool{
                      string path;
                             path << input << lib << ".tbd";
                      if( !m_mLibCache.find( path.hash() )){
                           m_mLibCache.set( path.hash(),1 );
                        if( e_fexists( path )){
                          e_msgf( "  Found library %s.tbd"
                               , ccp( path.basename() ));
                          files.push( File( path.os() ));
                          stop = true;
                        }
                      }
                      return!stop;
                    }
                  );
                  ++it;
                }
              }

              //****************************************************************

              //----------------------------------------------------------------
              // Link against system frameworks in the SDK.
              //----------------------------------------------------------------

              const auto wantsFramework=(// Path and file extension are nil.
                lib.path().empty() && ext.empty() );// Hints to the machine.
              if( wantsFramework ){

                //--------------------------------------------------------------
                // Test whether intent was to link with system framework.
                //--------------------------------------------------------------

                const auto& rootLibraryPath
                  = "/Library/Frameworks/"
                  + lib
                  + ".framework";
                if( e_dexists( rootLibraryPath )){
                  files.push( File(
                    rootLibraryPath.os() ));
                  return;
                }

                //--------------------------------------------------------------
                // Test whether the intent was to link with a user framework.
                //--------------------------------------------------------------

                const auto& homeLibraryPath
                  = "~/Library/Frameworks/"
                  + lib
                  + ".framework";
                if( e_dexists( homeLibraryPath )){
                  files.push( File( homeLibraryPath.os() ));
                  return;
                }

                //--------------------------------------------------------------
                // Test whether the intent was to link with a user framework.
                //--------------------------------------------------------------

                const auto& devLibraryPath =
                  "/Applications/Xcode.app/Contents/Developer/"
                  "Library/Frameworks/"
                  + lib
                  + ".framework";
                if( e_dexists( devLibraryPath )){
                  files.push( File( devLibraryPath.os() ));
                  return;
                }

                //--------------------------------------------------------------
                // Test whether the intent was to link with managed framework.
                //--------------------------------------------------------------

                if( e_getCvar( bool, "ALLOW_MANAGED" )){
                  string location;
                  IEngine::dir( "/Library/ManagedFrameworks/",
                    [&]( const auto& folder
                       , const auto& name
                       , const auto ){
                      const auto& ext = name
                        . ext()//faster
                        . tolower();
                      location
                        = folder
                        + name;

                      //----------------------------------------------------------
                      // Is it a framework?
                      //----------------------------------------------------------

                      if( ext == ".framework"_64 ){
                        const auto key
                          = location
                          . hash();
                        if( !keyCache.find( key )){
                          e_msgf( // Let the user know we found it.
                            "Found managed framework %s @ %s"
                            , ccp( location.basename() )
                            , ccp( location ));
                          auto& me = *const_cast<self*>( this );
                          me.toFrameworkPaths()
                            << location.path().trimmed( 1 )
                            << ",";
                          files.push( File( location ));
                          keyCache
                            . set( key
                            , 1
                          );
                        }
                        return false;
                      }

                      //----------------------------------------------------------
                      // Is it a dylib?
                      //----------------------------------------------------------

                      if( ext == ".dylib"_64 ){//rare case.
                        location
                          = folder
                          + name;
                        const auto key
                          = location
                          . hash();
                        if( !keyCache.find( key )){
                          e_msgf( // Let the user know we found it.
                            "Found managed DYLIB %s @ %s"
                            , ccp( location.basename() )
                            , ccp( location ));
                          keyCache.set( key, 1 );
                          files.push(
                            File(
                              location.os()
                            )
                          );
                        }
                        return false;
                      }
                      return true;
                    }
                  );
                }

                //------------------------------------------------------------
                // Test whether it's a framework inside Xcode app bundle.
                //------------------------------------------------------------

                if( xcodeExists ){
                  static constexpr ccp macOSsdkSystem =
                    "/Applications/Xcode.app/Contents/Developer/Platforms/"
                    "MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/"
                    "Library/Frameworks";
                  static constexpr ccp iOSsdkSystem =
                    "/Applications/Xcode.app/Contents/Developer/Platforms/"
                    "iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/"
                    "Library/Frameworks";
                  string path;
                  const auto& targets = getTargets();
                  auto it = targets.getIterator();
                  while( it ){
                    if( it->hash() == "macos"_64 ){
                      path = macOSsdkSystem;
                    }else{
                      path = iOSsdkSystem;
                    }
                    const auto& tbd = path
                      + "/lib"
                      + lib
                      + ".tbd";
                    if( e_fexists( tbd )){
                      e_msgf( "Found library %s (tbd)"
                           , ccp( tbd.basename() ));
                      files.push( File( tbd.os() ));
                      return;
                    }
                    ++it;
                  }
                  const auto& framework = path
                    + "/"
                    + lib
                    + ".framework";
                  if( e_dexists( framework )){
                    files.push( File( framework.os() ));
                    return;
                  }
                }

                //--------------------------------------------------------------
                // Write for macOS local lambda function.
                //--------------------------------------------------------------

                const auto& stayOnTarget=[&]( const string& target
                    , const Xcode& xcode )->bool{

                  //------------------------------------------------------------
                  // Everything allowed; the whole kit and kaboodle.
                  //------------------------------------------------------------

                  switch( target.hash() ){
                    case "macos"_64:/**/{
                      if( xcode.toLabel().ext().tolower() == ".dylib"_64 ){
                        const auto& label =
                            xcode.toLabel()
                          + "."
                          + xcode.toBuild();
                        e_msgf(
                          "Found dylib %s"
                          , ccp( lib.basename() ));
                        File f( label.os() );
                        if( !isNoEmbedAndSign() ){
                          f.setEmbed( true );
                          f.setSign( true );
                          const_cast<Xcode*>(
                            this
                          )->toEmbedFiles().push( f );
                        }
                        files.push( f );
                        return true;
                      }
                      switch( xcode.toBuild().hash() ){
                        case"shared"_64:
                          [[fallthrough]];
                        case"bundle"_64:/**/{
                          const auto& label =
                              xcode.toLabel()
                            + "."
                            + xcode.toBuild();
                          File f( label.os() );
                          if( !isNoEmbedAndSign() ){
                            f.setEmbed( true );
                            f.setSign( true );
                            const_cast<Xcode*>( this )
                              -> toEmbedFiles().push( f )
                            ;
                          }
                          files.push( f );
                          return true;
                        }
                        case"framework"_64:/**/{
                          const auto& label =
                              xcode.toLabel()
                            + "."
                            + xcode.toBuild();
                          File f( label.os() );
                          if( !isNoEmbedAndSign() ){
                            f.setEmbed( true );
                            f.setSign( true );
                            const_cast<Xcode*>( this )->toEmbedFiles().push( f );
                          }
                          files.push( f );
                          break;
                        }
                      }
                      return true;
                    }
                    case "ios"_64:/**/{
                      e_brk( "Unhandled case" );
                    }
                  }

                  //------------------------------------------------------------
                  // Only frameworks and archives allowed on iOS.
                  //------------------------------------------------------------

                  if( xcode.toBuild() == "framework"_64 ){
                    const auto& label =
                        xcode.toLabel()
                      + "."
                      + xcode.toBuild( );
                    File f( label.os() );
                    if( !isNoEmbedAndSign() ){
                      f.setEmbed( true );
                      f.setSign( true );
                      const_cast<Xcode*>( this )
                        -> toEmbedFiles().push(
                        f
                      );
                    }
                    files.push( f );
                    return true;
                  }
                  return false;
                };

                //--------------------------------------------------------------
                // Test whether it's a project framework or bundle.
                //--------------------------------------------------------------

                auto found = false;
                Class::foreachs<Xcode>(
                  [&]( const auto& xcode ){
                    if( this == &xcode )
                      return true;
                    if( lib == xcode.toLabel() ){
                      const auto& targets = getTargets();
                      auto it = targets.getIterator();
                      while( it ){
                        const auto& target = *it;
                        if( stayOnTarget/* Lighten up! */( target
                            , xcode )){
                          found = true;
                        }
                        ++it;
                      }
                    }
                    return true;
                  }
                );
                if( found ){
                  return;
                }
              }

              //****************************************************************

              auto& T = *const_cast<self*>( this );

              //----------------------------------------------------------------
              // Handle pathing directly to desired library. If we don't find
              // it here then the find_frameworks and find_library calls will
              // pick it up later.
              //----------------------------------------------------------------

              const auto& L = lib.os();
              const auto& X = L.ext().tolower();
              auto embedAndSign = true;
              switch( X.hash() ){
                case".a"_64:/**/{
                  embedAndSign = false;
                  File f ( L );
                  lookfor( f );
                  files.push(
                      f.toWhere() );
                  break;
                }
                default:/**/{
                  if( !embedAndSign )
                    break;
                  File f ( L );
                  lookfor( f );
                  f.setEmbed( true );
                  f.setSign(  true );
                  files.push( f );
                  T.toEmbedFiles()
                    . push( f
                  );
                }
              }

              //****************************************************************

              //----------------------------------------------------------------
              // Handle .a and .dylib extensions.
              //----------------------------------------------------------------

              const auto& libExt = lib.ext().tolower().hash();
              if(( libExt != ".framework"_64 ) &&
                 ( libExt != ".bundle"_64 )){
                File f( lib.os() );

                //--------------------------------------------------------------
                // Searching for future (not compiled yet) libraries.
                //--------------------------------------------------------------

                if( !e_fexists( lib )){
                  Class::foreachs<Xcode>(
                    [&]( const Xcode& xcode ){
                      if( lib.left( 3 ) == "lib"_64 ){
                        const auto& base = lib
                          . ltrimmed( 3 )
                          . basename();
                        if( xcode.toLabel() == base ){
                          switch( xcode.toBuild().hash() ){

                            //--------------------------------------------------
                            // iOS doesn't allow shared libraries.
                            //--------------------------------------------------

                            case"shared"_64:/**/{
                              const auto& targets = getTargets();
                              auto it = targets.getIterator();
                              while( it ){
                                if( it->hash() == "macos"_64 )
                                  files.push( File( lib ));
                                ++it;
                              }
                              break;
                            }

                            //--------------------------------------------------
                            // Static libraries supported by both platforms.
                            //--------------------------------------------------

                            case"static"_64:/**/{
                              const auto& targets = getTargets();
                              auto it = targets.getIterator();
                              while( it ){
                                if( it->hash() == "macos"_64 ){
                                  files.push( File( lib ));
                                }else{
                                  string ioslib;
                                  ioslib << "lib";
                                  ioslib << base;
                                  ioslib << "ios.a";
                                  files.push(
                                    File(
                                      ioslib
                                    )
                                  );
                                }
                                ++it;
                              }
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

                //--------------------------------------------------------------
                // Sort the files.
                //--------------------------------------------------------------

                files.sort(
                  []( const auto& a, const auto& b ){
                    return a.tolower() < b.tolower();
                  }
                );
              }
            }
          );

          //--------------------------------------------------------------------
          // Write out the file and embedding line.
          //--------------------------------------------------------------------

          const_cast<self*>( this )->setLibFiles( files );
          const auto embedAndSign = toEmbedAndSign();
          const auto& vectorsSign = embedAndSign
              . splitAtCommas();{
            files.foreach(
              [&]( File& f ){
                if( f.empty() )
                  return;
                const auto& wt = f.toWhere( );
                if( libCache.find( f.hash() ))
                  return;
                libCache.set( f.hash(), 1 );
                const auto/* no & */ext = f
                  . ext()
                  . tolower()
                  . hash();
                vectorsSign.foreach(
                  [&]( const auto& _f ){
                    if( strstr( f, _f )){
                      const auto key = wt.hash()
                        ? wt.hash()
                        : _f.hash();
                      if( !keyCache.find( key )){
                        e_msgf( "  $(lightblue)Embedding $(off)lib%s%s"
                          , ccp( f
                          . basename()
                          . ltrimmed( 3 ))
                          , ccp( f.ext().tolower() ));
                        f.setEmbed( true );
                        f.setSign( true );
                        keyCache.set( key
                          , 1
                        );
                      }
                    }
                  }
                );
                if( f.isEmbed() ){

                  //------------------------------------------------------------
                  // Reference in frameworks.
                  //------------------------------------------------------------

                  out << "    "
                    + f.toBuildID()
                    + " /* "
                    + f.filename();
                  if(( ext == ".framework"_64 )||( ext == ".dylib"_64 )){
                    out << " in Frameworks */ = {isa = PBXBuildFile; fileRef = ";
                  }else if( ext == ".bundle"_64 ){
                    out << " in PlugIns */ = {isa = PBXBuildFile; fileRef = ";
                  }else{
                    out << " */ = {isa = PBXBuildFile; fileRef = ";
                  }
                  out << f.toFileRefID()
                    + " /* "
                    + f.filename()
                    + " */; };\n";

                  //------------------------------------------------------------
                  // Local lambda function to add embedding syntax.
                  //------------------------------------------------------------

                  const auto& stayOnTarget=[&]( const string& target ){
                    const auto verbose = e_getCvar( bool, "VERBOSE_LOGGING" );
                    if( verbose )
                      e_msgf( "  Attempting to embed \"%s\"", ccp( f ));
                    if( target.hash() == "ios"_64 ){
                      if( ext == ".bundle"_64 ){
                        if( verbose )
                          e_msgf( "  Failed embedding \"%s\"", ccp( f ));
                        return;
                      }
                      if( ext == ".dylib"_64 ){
                        if( verbose )
                          e_msgf( "  Failed embedding \"%s\"", ccp( f ));
                        return;
                      }
                    }
                    out << "    "
                      + f.toEmbedID()
                      + " /* "
                      + f.filename();
                    if( ext == ".framework"_64 ){
                      out << " in Embed Frameworks */ = {isa = PBXBuildFile; fileRef = ";
                    }else if(( target != "ios" )&&( ext == ".bundle"_64 )){
                      out << " in Embed Bundles */ = {isa = PBXBuildFile; fileRef = ";
                    }else if(( target != "ios" )&&( ext == ".dylib"_64 )){
                      out << " in Embed Dylibs */ = {isa = PBXBuildFile; fileRef = ";
                    }else{
                      out << " */ = {isa = PBXBuildFile; fileRef = ";
                    }
                    out << f.toFileRefID()
                      + " /* "
                      + f.filename()
                      + " */; settings = {ATTRIBUTES = (";
                    if( f.isSign() ){
                      out << "CodeSignOnCopy, ";
                    }
                    if(( ext == ".framework"_64 ) && f.isStrip() ){
                      out << "RemoveHeadersOnCopy, ";
                    }
                    out << "); }; };\n";
                  };

                  //------------------------------------------------------------
                  // Reference in embedded frameworks.
                  //------------------------------------------------------------

                  const auto& targets = getTargets();
                  auto it = targets.getIterator();
                  while( it ){
                    stayOnTarget( *it );
                    ++it;
                  }
                  return;
                }

                //--------------------------------------------------------------
                // Handle non-embeddable targets.
                //--------------------------------------------------------------

                const auto& targets = getTargets();
                auto it = targets.getIterator();
                while( it ){
                  if( it->hash() == "macos"_64 ){
                    switch( ext ){
                      case".framework"_64:
                        [[fallthrough]];
                      case".bundle"_64:
                        [[fallthrough]];
                      case".dylib"_64:
                        [[fallthrough]];
                      case".tbd"_64:
                        [[fallthrough]];
                      case".a"_64:/**/{
                        out << "    "
                          + f.toBuildID()
                          + " /* "
                          + f.filename();
                        break;
                      }
                      default:/**/{
                        return;
                      }
                    }
                  }else{
                    switch( ext ){
                      case".framework"_64:
                        [[fallthrough]];
                      case".tbd"_64:
                        [[fallthrough]];
                      case".a"_64:/**/{
                        out << "    "
                          + f.toBuildID()
                          + " /* "
                          + f.filename();
                        break;
                      }
                      default:/**/{
                        return;
                      }
                    }
                  }
                  if( it->hash() == "macos"_64 ){
                    if( e_getCvar( bool, "VERBOSE_LOGGING" )){
                      e_msgf( "  Filename is \"%s\" (%s)",
                          ccp( f )
                        , ccp( f.toWhere() )
                      );
                    }
                    switch( ext ){
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
                  }else switch( ext ){
                    case".framework"_64:
                      out << " in Frameworks */ = {isa = PBXBuildFile; fileRef = ";
                      break;
                    case".tbd"_64:
                      out << " in TBDs */ = {isa = PBXBuildFile; fileRef = ";
                      break;
                    case".a"_64:
                      out << " in Statics */ = {isa = PBXBuildFile; fileRef = ";
                      break;
                  }
                  out << f.toFileRefID()
                    + " /* "
                    + f.filename()
                    + " */; };\n";
                  ++it;
                }
              }
            );
          }
        }
      }

      void Workspace::Xcode::writePBXBuildFileSection( Writer& out )const{

        //----------------------------------------------------------------------
        // Staring comment.
        //----------------------------------------------------------------------

        out << "\n    /* Begin PBXBuildFile section */\n";

        //----------------------------------------------------------------------
        // Add and filter all resource files by known type.
        //----------------------------------------------------------------------

        Files files;
        addToFiles( files, inSources( Type::kStoryboard ));
        addToFiles( files, inSources( Type::kXcasset ));
        addToFiles( files, inSources( Type::kPrefab ));
        addToFiles( files, inSources( Type::kLproj ));
        addToFiles( files, inSources( Type::kPlist ));
        if( !files.empty() ){
          ignore( files, toIgnoreParts() );
          files.foreach(
            [&]( auto& f ){
              if( f.empty() )
                return;
              out << "    "
                  << f.toBuildID()
                  << " /* "
                  << f.filename()
                  << " in Resources */ = {isa = PBXBuildFile; fileRef = "
                  << f.toFileRefID()
                  << " /* "
                  << f.filename();
              out << " */; };\n";
            }
          );
        }

        //----------------------------------------------------------------------
        // Now add all the CopyFile references (for framework projects).
        //----------------------------------------------------------------------

        files.clear();
        if( addToFiles( files, toPublicRefs() )){
          ignore( files, toIgnoreParts() );
          files.foreach(
            [&]( auto& f ){
              if( f.empty() )
                return;
              out << "    "
                  << f.toBuildID()
                  << " /* "
                  << f.filename()
                  << " in CopyFiles */ = {isa = PBXBuildFile; fileRef = "
                  << f.toFileRefID()
                  << " /* "
                  << f.filename();
              out << " */; };\n";
            }
          );
        }

        //----------------------------------------------------------------------
        // Now add all the private header files.
        //----------------------------------------------------------------------

        files.clear();
        if( addToFiles( files, toPrivateHeaders() )){
          ignore( files, toIgnoreParts() );
          files.foreach(
            [&]( auto& f ){
              if( f.empty() )
                return;
              out << "    "
                  << f.toBuildID()
                  << " /* "
                  << f.filename()
                  << " in Headers */ = {isa = PBXBuildFile; fileRef = "
                  << f.toFileRefID()
                  << " /* "
                  << f.filename();
              out << " */; settings = {ATTRIBUTES = (Private, ); }; };\n";
            }
          );
        }

        //----------------------------------------------------------------------
        // Now add all the public header files.
        //----------------------------------------------------------------------

        files.clear();
        if( addToFiles( files, toPublicHeaders() )){
          ignore( files, toIgnoreParts() );
          files.foreach(
            [&]( auto& f ){
              if( f.empty() )
                return;
              out << "    "
                  << f.toBuildID()
                  << " /* "
                  << f.filename()
                  << " in Headers */ = {isa = PBXBuildFile; fileRef = "
                  << f.toFileRefID()
                  << " /* "
                  << f.filename();
              out << " */; settings = {ATTRIBUTES = (Public, ); }; };\n";
            }
          );
        }

        //----------------------------------------------------------------------
        // Source files.
        //----------------------------------------------------------------------

        files.clear();
        addToFiles( files, inSources( Type::kCpp ));
        addToFiles( files, inSources( Type::kMm  ));
        addToFiles( files, inSources( Type::kM   ));
        addToFiles( files, inSources( Type::kC   ));
        if( !files.empty() ){
          ignore( files, toIgnoreParts() );
          files.foreach(
            [&]( auto& f ){
              if( f.empty() )
                return;
              out << "    "
                  << f.toBuildID()
                  << " /* "
                  << f.filename()
                  << " in Sources */ = {isa = PBXBuildFile; fileRef = "
                  << f.toFileRefID()
                  << " /* "
                  << f.filename();
              out << " */; };\n";
            }
          );
        }

        //----------------------------------------------------------------------
        // Ending comment.
        //----------------------------------------------------------------------

        out << "    /* End PBXBuildFile section */\n";
      }

      void Workspace::Xcode::writePBXCopyFilesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXCopyFilesBuildPhase section */\n";

          //--------------------------------------------------------------------
          // Local lambda function to embed files.
          //--------------------------------------------------------------------

          const auto& writePBXCopyFilesBuildPhase=[&](
                const auto& subfolderSpec
              , const auto& files
              , const auto& id
              , const auto& comment
              , const std::function<void( const File& )>& lbuild
              , const std::function<void()>& lambda ){
            if( files.empty() )
              return;
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
            if( lambda )
                lambda();
            fs << "      runOnlyForDeploymentPostProcessing = 0;\n";
            fs << "    };\n";
          };

          //--------------------------------------------------------------------
          // Declare a lambda to do the heavy lifting of the copy build phase.
          //--------------------------------------------------------------------

          const auto& onCopy=[&](
                const string& target
              , const auto& frameworks
              , const auto& copyRefs
              , const auto& plugins ){

            //------------------------------------------------------------------
            // Copy dylib references into frameworks folder.
            //------------------------------------------------------------------

            writePBXCopyFilesBuildPhase(
                string( "6"/* CopyFiles */)
              , toEmbedFiles()
              , copyRefs
              , string( "CopyFiles" )
              , [&]( const File& f ){
                  if( f.filename().ext().tolower().hash() != ".dylib"_64 )
                    return;
                  fs << "        ";
                  fs << f.toBuildID();
                  fs << " /* "
                    + f.filename()
                    + " in CopyFiles */,\n"
                  ;
                }
              , nullptr );

            //------------------------------------------------------------------
            // Copy references into resources folder.
            //------------------------------------------------------------------

            writePBXCopyFilesBuildPhase(
                string( "7"/* CopyFiles */)
              , toPublicRefs()
              , copyRefs
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
            // Copy embedded frameworks and dylibs etc, into Frameworks folder.
            //--------------------------------------------------------------------

            writePBXCopyFilesBuildPhase(
                string( "13"/* PlugIns CopyFiles */)
              , toEmbedFiles()
              , plugins
              , string( "Embed PlugIns" )
              , [&]( const File& f ){
                  if( f.ext().tolower() == ".bundle"_64 ){
                    fs << "        ";
                    fs << f.toEmbedID();
                    fs << " /* " + f.filename();
                    fs << " in CopyFiles */,\n";
                  }
                }
              , nullptr
            );

            //------------------------------------------------------------------
            // Copy embedded frameworks and dylibs etc into Frameworks folder.
            // DB0FA58B2758022D00CA287A /* Embed Frameworks */ = {
            //   isa = PBXCopyFilesBuildPhase;
            //   buildActionMask = 2147483647;
            //   dstPath = "";
            //   dstSubfolderSpec = 10;
            //   files = (
            //      DB0FA58C2758023000CA287A /* libfbxsdk.dylib in Embed Frameworks */,
            //      DB0FA58A2758022D00CA287A /* eon.framework in Embed Frameworks */,
            //   );
            //   name = "Embed Frameworks";
            //   runOnlyForDeploymentPostprocessing = 0;
            // };
            //--------------------------------------------------------------------

            writePBXCopyFilesBuildPhase(
                string( "10"/* Frameworks */)
              , toEmbedFiles()
              , frameworks
              , string( "Embed Frameworks" )
              , [&]( const File& f ){
                  switch( f.ext().tolower().hash() ){
                    case".dylib"_64:
                      if( target.hash() != "macos"_64 )
                        break;
                      [[fallthrough]];
                    case".framework"_64:/**/{
                      fs << "        ";
                      fs << f.toEmbedID();
                      fs << " /* "
                        + f.filename()
                        + " in Embed Frameworks */,\n";
                      break;
                    }
                  }
                }
            , [&](){
                fs << "      name = \"Embed Frameworks\";\n";
              }
            );
          };

          //--------------------------------------------------------------------
          // Copy references into resources folder.
          //--------------------------------------------------------------------

          const auto& targets = getTargets();
          auto it = targets.getIterator();
          while( it ){
            auto target( *it );
            string embedFrameworks;
            string embedPlugins;
            string copyRefs;

            //------------------------------------------------------------------
            // Gather all strings.
            //------------------------------------------------------------------

            if( target == "macos"_64 ){
              embedFrameworks = m_aFrameworksEmbed[ Target::macOS ];
              embedPlugins    = m_aPluginsEmbed[    Target::macOS ];
              copyRefs        = m_aCopyRefs[        Target::macOS ];
            }else{
              embedFrameworks = m_aFrameworksEmbed[ Target::iOS ];
              embedPlugins    = m_aPluginsEmbed[    Target::iOS ];
              copyRefs        = m_aCopyRefs[        Target::iOS ];
            }

            //------------------------------------------------------------------
            // Run action to write to PBX section.
            //------------------------------------------------------------------

            onCopy( target
              , embedFrameworks
              , embedPlugins
              , copyRefs );
            ++it;
          }
        fs << "    /* End PBXCopyFilesBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXFileReferenceSection( Writer& out )const{
        out << "\n    /* Begin PBXFileReference section */\n";

        //----------------------------------------------------------------------
        // Library files.
        //----------------------------------------------------------------------

        writeLibraries( out );

        //----------------------------------------------------------------------
        // Source files.
        //----------------------------------------------------------------------

        writeFileReference( out, inSources( Type::kStoryboard ), "file.storyboard"     );
        writeFileReference( out, inSources( Type::kXcasset    ), "folder.assetcatalog" );
        writeFileReference( out, inSources( Type::kPrefab     ), "file"                );
        writeFileReference( out, inSources( Type::kLproj      ), "folder"              );
        writeFileReference( out, inSources( Type::kPlist      ), "text.plist.xml"      );
        writeFileReference( out, inSources( Type::kCpp        ), "sourcecode.cpp.cpp"  );
        writeFileReference( out, inSources( Type::kMm         ), "sourcecode.cpp.objc" );
        writeFileReference( out, inSources( Type::kHpp        ), "sourcecode.cpp.h"    );
        writeFileReference( out, inSources( Type::kInl        ), "sourcecode.cpp.h"    );
        writeFileReference( out, inSources( Type::kM          ), "sourcecode.c.objc"   );
        writeFileReference( out, inSources( Type::kH          ), "sourcecode.c.h"      );
        writeFileReference( out, inSources( Type::kC          ), "sourcecode.c.c"      );
        writeFileReference( out, toPublicRefs(),                 "folder"              );

        //----------------------------------------------------------------------
        // Entitlements.
        //----------------------------------------------------------------------

        if( hasEntitlements() ){
          File f( toLabel() + ".entitlements" );
          f.setFileRefID( m_sEntFileRefID );
          f.setBuildID( m_sEntBuildID );
          Files v{ f };
          writeFileReference( out
            , v
            , "text.plist.entitlements"
          );
        }

        //----------------------------------------------------------------------
        // Header files.
        //----------------------------------------------------------------------

        toPublicHeaders().foreach(
          [&]( const auto& _f ){
            if( _f.empty() )
              return;
            File f( _f );
            string lastKnownFileType;
            switch( f.tolower().ext().hash() ){
              case".h"_64:
                lastKnownFileType = "sourcecode.c.h";
                break;
              default:
                lastKnownFileType = "folder";
                break;
            }
            const auto found = lookfor( f );
            out << "    "
                << f.toFileRefID()
                << " = {isa = PBXFileReference; lastKnownFileType = "
                << lastKnownFileType
                << "; name = " << f.filename();
            if( found/* not ../ already in pwd for e_fexists */){
              out << "; path = " << f.toWhere();
            }else{
              out << "; path = " << f;
            }
            out << "; sourceTree = \"<group>\"; };\n";
          }
        );

        //----------------------------------------------------------------------
        // Library files.
        //----------------------------------------------------------------------

        addToPBXFileReferenceSection( out,
          [&]( const auto& target
             , const auto& label
             , const auto& prod ){

            //------------------------------------------------------------------
            // Everything we're linking against.
            //------------------------------------------------------------------

            toLibFiles().foreach(
              [&]( const auto& lib ){
                if( lib.empty() )
                  return;
                auto isProduct = false;
                Class::foreachs<Xcode>(
                  [&]( const auto& xcode ){
                    if( this == &xcode )
                      return true;
                    auto l( "lib"
                      + xcode.toLabel()
                      + ".a" );
                    if( l == lib ){
                      isProduct = true;
                    }else{
                      auto l( xcode.toLabel()
                        + ".bundle" );
                      if( l == lib ){
                        isProduct = true;
                      }else{
                        auto l( xcode.toLabel()
                          + ".framework" );
                        if( l == lib ){
                          isProduct = true;
                        }else{
                          auto l( xcode.toLabel()
                            + ".dylib" );
                          if( l == lib ){
                            isProduct = true;
                          }
                        }
                      }
                    }
                    return!isProduct;
                  }
                );
                File f( lib );
                const auto found = lookfor( f );
                const auto _ext = f.ext().tolower();
                const auto hash = _ext.hash();
                string fileType;
                if( target == "macos"_64 ){
                  switch( hash ){
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
                    default:/* assume tbd */{
                      fileType = "\"sourcecode.text-based-dylib-definition\"";
                      break;
                    }
                  }
                }else if( target == "ios" ){
                  switch( hash ){
                    case".framework"_64:
                      fileType = "wrapper.framework";
                      break;
                    case".tbd"_64:
                      fileType = "\"sourcecode.text-based-dylib-definition\"";
                      break;
                    case".a"_64:
                      fileType = "archive.ar";
                      break;
                    default:/**/{
                      e_msg( "ERROR in unhandled type (ios)" );
                      break;
                    }
                  }
                }
                out << "    " + f.toFileRefID();
                if( !isProduct ){
                  out << " = {isa = PBXFileReference; lastKnownFileType = ";
                }else{
                  out << " = {isa = PBXFileReference; explicitFileType = ";
                }
                out << fileType
                    << "; name = "
                    << f.filename()
                    << "; path = ";
                if(( hash != ".framework"_64 )&&
                   ( hash != ".bundle"_64 )){
                  if( found ){
                    out << "../" + f.toWhere();
                  }else{
                    out << "../" + f.os();
                  }
                }else{
                  out << f.basename();
                  out << f.ext();
                }
                switch( hash ){
                  case".framework"_64:
                    if( isProduct ){
                      out << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                    }else{
                      out << "; sourceTree = SDKROOT; };\n";
                    }
                    break;
                  case".bundle"_64:
                    if( isProduct ){
                      out << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                    }else{
                      out << "; sourceTree = \"<group>\"; };\n";
                    }
                    break;
                  case".tbd"_64:
                    out << "; sourceTree = SDKROOT; };\n";
                    break;
                  case".dylib"_64:
                    [[fallthrough]];
                  case".a"_64:
                    if( isProduct ){
                      out << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                      break;
                    }
                    [[fallthrough]];
                  default:
                    out << "; sourceTree = \"<group>\"; };\n";
                    break;
                }
              }
            );
          }
        );

        //----------------------------------------------------------------------
        // The project.
        //----------------------------------------------------------------------

        addToPBXFileReferenceSection( out,
          [&]( const auto& target
             , const auto& label
             , const auto& prod ){
            switch( toBuild().hash() ){
              case"framework"_64:
                out << "    "
                    << prod
                    << " /* "
                    << toLabel()
                    << label
                    << ".framework */ = {isa = PBXFileReference; explicitFileType = wrapper.framework; includeInIndex = 0; path = "
                    << toLabel()
                    << label
                    << ".framework; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                break;
              case"bundle"_64:
                if( target.hash() != "ios"_64 )
                  out << "    "
                      << prod
                      << " /* "
                      << toLabel()
                      << label
                      << ".bundle */ = {isa = PBXFileReference; explicitFileType = wrapper.cfbundle; includeInIndex = 0; path = "
                      << toLabel()
                      << label
                      << ".bundle; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                break;
              case"shared"_64:
                if( target.hash() != "ios"_64 )
                  out << "    "
                      << prod
                      << " /* lib"
                      << toLabel()
                      << label
                      << ".dylib */ = {isa = PBXFileReference; explicitFileType = \"compiled.mach-o.dylib\"; includeInIndex = 0; path = lib"
                      << toLabel()
                      << label
                      << ".dylib; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                break;
              case"static"_64:
                out << "    "
                    << prod
                    << " /* lib"
                    << toLabel()
                    << label
                    << ".a */ = {isa = PBXFileReference; explicitFileType ="
                    << " archive.ar; includeInIndex = 0; path = lib"
                    << toLabel()
                    << label
                    << ".a; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                break;
              case"application"_64:
                out << "    "
                    << prod
                    << " /* "
                    << toLabel()
                    << label
                    << " */ = {isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = "
                    << toLabel()
                    << label
                    << ".app; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                break;
              case"console"_64:
                if( target.hash() != "ios"_64 )
                  out << "    "
                      << prod
                      << " /* "
                      << toLabel()
                      << label
                      << " */ = {isa = PBXFileReference; explicitFileType = compiled.mach-o.executable; includeInIndex = 0; path = "
                      << toLabel()
                      << label
                      << "; sourceTree = BUILT_PRODUCTS_DIR; };\n";
                break;
            }
          }
        );
        out << "    /* End PBXFileReference section */\n";
      }

      void Workspace::Xcode::addToPBXFrameworksBuildPhaseSection( Writer& fs
          , const std::function<void(
              const string& target
            , const string& buildPhase )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          string buildPhase;
          if( target == "macos"_64 ){
            buildPhase = m_aFrameworkBuildPhase[
              Target::macOS
            ];
          }else{
            buildPhase = m_aFrameworkBuildPhase[
              Target::iOS
            ];
          }
          lambda( target
            , buildPhase );
          ++it;
        }
      }

      void Workspace::Xcode::writePBXFrameworksBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXFrameworksBuildPhase section */\n";
        addToPBXFrameworksBuildPhaseSection( fs,
          [&]( const string& target
             , const string& frameworkBuildPhase ){
            fs << "    " + frameworkBuildPhase + " /* frameworks */ = {\n";
            fs << "      isa = PBXFrameworksBuildPhase;\n"
               << "      buildActionMask = 2147483647;\n"
               << "      files = (\n";
            toLibFiles().foreach(
              [&]( const auto& f ){
                if( f.empty() )
                  return;
                fs << "        "
                   << + f.toBuildID()
                   << " /* "
                   << f.filename();
                fs << " */,\n";
              }
            );
            fs << string( "      );\n" )
              + "      runOnlyForDeploymentPostprocessing = 0;\n"
              + "    };\n"
            ;
          }
        );
        fs << "    /* End PBXFrameworksBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXGroupSection( Writer& fs )const{
        fs << "\n    /* Begin PBXGroup section */\n";
        e_msgf( "Generating %s", ccp( toLabel() ));

          //--------------------------------------------------------------------
          // Top level group.
          //--------------------------------------------------------------------

          fs << "    " + m_sMainGroup + " = {\n"
             << "      isa = PBXGroup;\n"
             << "      children = (\n";
          if( hasEntitlements() ){
            fs << "        "
              + m_sEntFileRefID
              + " /* "
              + toLabel()
              + ".entitlements */,\n"
            ;
          }
          fs << "        " + m_sFrameworkGroup + " /* Frameworks */,\n"
             << "        " + m_sProductsGroup  + " /* Products */,\n"
             << "        " + m_sCodeGroup + " /* Code */,\n"
             << "      );\n"
             << "      sourceTree = \"<group>\";\n"
             << "    };\n";

          //--------------------------------------------------------------------
          // Add PBX groups.
          //--------------------------------------------------------------------

          Files files;
          addToPBXGroupSection( fs,
            [&]( const string& product
               , const string& target
               , const string& label ){

              //----------------------------------------------------------------
              // Products group.
              //----------------------------------------------------------------

              auto build( toBuild() );
              switch( build.hash( ) ){
                case"shared"_64:
                  build = ".dylib";
                  break;
                case"static"_64:
                  build = ".a";
                  break;
                default:
                  build = "." + build;
                  break;
              }
              fs << "    "
                 << m_sProductsGroup
                 << " /* Products */ = {\n"
                 << "      isa = PBXGroup;\n"
                 << "      children = (\n"
                 << "        "
                 << product
                 << " /* "
                 << label
                 << build
                 << " */,\n"
                 << "      );\n"
                 << "      name = Products;\n"
                 << "      sourceTree = \"<group>\";\n"
                 << "    };\n"
                 << "    "
                 << m_sIncludeGroup
                 << " /* include */ = {\n"
                 << "      isa = PBXGroup;\n"
                 << "      children = (\n";
              files.pushVector( inSources( Type::kHpp ));
              files.pushVector( inSources( Type::kInl ));
              files.pushVector( inSources( Type::kH   ));
              files.sort(
                []( const auto& a, const auto& b ){
                  return(
                      a.filename().tolower()
                    < b.filename().tolower()
                  );
                }
              );
              files.foreach(
                [&]( const File& file ){
                  // File reference added per child.
                  fs << "        "
                     << file.toFileRefID()
                     << " /* " + file
                     << " */,\n";
                }
              );
              fs << "      );\n";
              fs << "      name = include;\n";
              fs << "      sourceTree = \"<group>\";\n";
              fs << "    };\n";

              //----------------------------------------------------------------
              // Frameworks group.
              //----------------------------------------------------------------

              fs << "    "
                 << m_sFrameworkGroup
                 << " /* Frameworks */ = {\n"
                 << "      isa = PBXGroup;\n"
                 << "      children = (\n";

              // The idea here is if you embed something it automatically shows
              // up in the library files vector, an assumption, but a good one.
              { auto& embedded = const_cast<self*>( this )->toEmbedFiles();
                auto et = embedded.getIterator();
                while( et ){
                  auto& f = *et;
                  if( !f.empty() ){
                    if( lookfor( f )){
                      e_msgf( "  Embed with %s @ \"%s\""
                        , ccp( f )
                        , ccp( f.toWhere() ));
                      files.push( f );
                    }
                  }
                  ++et;
                }
              }

              // m_vLibFiles has the embedded frameworks as well now. So, no
              // need to do them twice as that causes problems in Xcode.
              toLibFiles().foreach(
                [&]( const auto& f ){
                  if( f.empty() )
                    return;
                  // TODO: Why is this necessary? Surely fix the bug than this?
                  // The group cache contains all the files we've already added
                  // so we never accidentally add the bugger twice or more.
                  if( grpCache.find( f.hash() ))
                     return;
                  grpCache.set( f
                    . hash()
                    , 1 );
                  fs << "        " // Library reference per child.
                     << f.toFileRefID()
                     << " /* "
                     << f.filename()
                     << " */,\n";
                  e_msgf( "  %s links with %s"
                    , ccp( toLabel().mixedcase() )
                    , ccp( f.filename() )
                  );
                }
              );
              fs << string( "      );\n" )
                 << "      name = Frameworks;\n"
                 << "      sourceTree = \"<group>\";\n"
                 << "    };\n";
            }
          );

          //--------------------------------------------------------------------
          // Resources group.
          //--------------------------------------------------------------------

          fs << "    " + m_sResourcesGroup + " /* resources */ = {\n"
             << "      isa = PBXGroup;\n"
             << "      children = (\n";
          files.clear();
          files.pushVector( inSources( Type::kStoryboard ));
          files.pushVector( inSources( Type::kXcasset    ));
          files.pushVector( inSources( Type::kPrefab     ));
          files.pushVector( inSources( Type::kLproj      ));
          files.sort(
            []( const File& a, const File& b ){
              return( a.filename().tolower() < b.filename().tolower() );
            }
          );
          files.foreach(
            [&]( const File& file ){
              fs << "        "
                 << file.toFileRefID()
                 << " /* "
                 << file;
              fs << " */,\n";
            }
          );
          fs << "      );\n";
          fs << "      name = resources;\n";
          fs << "      sourceTree = \"<group>\";\n";
          fs << "    };\n";

          //--------------------------------------------------------------------
          // Code group.
          //--------------------------------------------------------------------

          fs << "    " + m_sCodeGroup + " /* Code */ = {\n"
             << "      isa = PBXGroup;\n"
             << "      children = (\n";
          const auto hasReferences=( !toPublicHeaders().empty()||!toPublicRefs().empty() );
          if( hasReferences )fs
             << "        " + m_sReferencesGroup + " /* references */,\n";
          fs << "        " + m_sResourcesGroup + " /* resources */,\n"
             << "        " + m_sIncludeGroup + " /* include */,\n"
             << "        " + m_sSrcGroup + " /* src */,\n"
             << "      );\n"
             << "      name = Code;\n"
             << "      sourceTree = \"<group>\";\n"
             << "    };\n";

          //--------------------------------------------------------------------
          // Exporting public headers/references from framework.
          //--------------------------------------------------------------------

          if( hasReferences ){
            fs << "    " + m_sReferencesGroup + " /* references */ = {\n"
               << "      isa = PBXGroup;\n"
               << "      children = (\n";
            files.clear();
            files.pushVector( toPublicHeaders() );
            files.pushVector( toPublicRefs() );
            files.sort(
              []( const File& a, const File& b ){
                return( a
                  . filename()
                  . tolower()
                  < b
                  . filename()
                  . tolower()
                );
              }
            );
            files.foreach(
              [&]( const auto& file ){
                fs
                  << "        "
                  << file.toFileRefID()
                  << " /* "
                  << ccp( file )
                  << " */,\n"
                ;
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
          files.clear();
          files.pushVector( inSources( Type::kCpp ));
          files.pushVector( inSources( Type::kMm  ));
          files.pushVector( inSources( Type::kC   ));
          files.pushVector( inSources( Type::kM   ));
          files.sort(
            []( const auto& a, const auto& b ){
              return( a
                . filename()
                . tolower()
                < b
                . filename()
                . tolower()
              );
            }
          );
          files.foreach(
            [&]( const File& file ){
              fs << "        " + file.toFileRefID() + " /* " + file + " */,\n";
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
        addToPBXNativeTargetSection( fs,
          [&]( const auto& target // e.g. macos, ios, ipados
             , const auto& label // e.g. LeluXD, LeluXD-iOS, LeluXD-iPadOS
             , const auto& targetBuild
             , const auto& targetFramework
             , const auto& phaseFramework
             , const auto& phaseResources
             , const auto& phaseHeaders
             , const auto& phaseSources
             , const auto& phaseScript
             , const auto& embedFrameworks
             , const auto& embedPlugins
             , const auto& productFileRef
             , const auto& copyRefs ){
            fs << "    "
                + targetFramework
                + " /* framework */ = {\n"
                + "      isa = PBXNativeTarget;\n"
                + "      buildConfigurationList = "
                + targetBuild
                + " /* Build configuration list for PBXNativeTarget \""
                + label
                + "\" */;\n"
                + "      buildPhases = (\n"
                + "        "
                + phaseFramework
                + " /* Frameworks */,\n"
                + "        "
                + phaseResources
                + " /* Resources */,\n";
            if(( toBuild() == "application"_64 )||(
                 toBuild() == "framework"_64 )||((
                 toBuild() == "bundle"_64 ))){
              fs << "        " + copyRefs + " /* CopyRefs */,\n";
              fs << "        " + embedPlugins + " /* Embed PlugIns */,\n";
              fs << "        " + embedFrameworks + " /* Embed Frameworks */,\n";
            }
            if( !phaseHeaders.empty() )
              fs << "        " + phaseHeaders + " /* Headers */,\n";
            fs << "        " + phaseSources + " /* Sources */,\n";
            if( !toInstallScript().empty() )
              fs << "        " + phaseScript + " /* Script */,\n";
            fs << string( "      );\n" )
                + "      buildRules = (\n"
                + "      );\n"
                + "      dependencies = (\n"
                + "      );\n"
                + "      name = \"" + label + "\";\n"
                + "      productName = \"" + label + "\";\n";
            switch( toBuild().hash() ){
              case"framework"_64:
                fs << "      productReference = " + productFileRef + " /* " + label + ".framework */;\n";
                fs << "      productType = \"com.apple.product-type.framework\";\n";
                break;
              case"bundle"_64:
                if( target.hash() != "macos"_64 )
                  e_errorf( 1091, "Cannot create a bundle for iOS targets." );
                fs << "      productReference = " + productFileRef + " /* " + label + ".bundle */;\n";
                fs << "      productType = \"com.apple.product-type.bundle\";\n";
                break;
              case"shared"_64:
                if( target.hash() != "macos"_64 )
                  e_errorf( 1091, "Cannot create a shared library for iOS targets." );
                fs << "      productReference = " + productFileRef + " /* lib" + label + ".a */;\n";
                fs << "      productType = \"com.apple.product-type.library.dynamic\";\n";
                break;
              case"static"_64:
                fs << "      productReference = " + productFileRef + " /* lib" + label + ".a */;\n";
                fs << "      productType = \"com.apple.product-type.library.static\";\n";
                break;
              case"application"_64:
                fs << "      productReference = " + productFileRef + " /* " + label + ".app */;\n";
                fs << "      productType = \"com.apple.product-type.application\";\n";
                break;
              case"console"_64:
                if( target.hash() != "macos"_64 )
                  e_errorf( 1091, "Cannot create a shared library for iOS targets!" );
                fs << "      productReference = " + productFileRef + " /* " + label + " */;\n";
                fs << "      productType = \"com.apple.product-type.tool\";\n";
                break;
            }
            fs << "    };\n";
          }
        );
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
        }else if( bmp->bXcode14 ){
          fs << "        LastUpgradeCheck = 1420;\n";
        }
        if( !toOrgName().empty() ){ fs
          << "        ORGANIZATIONNAME = \"" + toOrgName() + "\";\n";
        }
        fs << "        TargetAttributes = {\n"
           << "          ";
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          const auto& target = *it;
          string label;
          if( target == "macos"_64 ){
            fs << m_aFrameNativeTarget[ Target::macOS ];
          }else if( target == "ios"_64 ){
            fs << m_aFrameNativeTarget[ Target::iOS ];
            label = toLabel() + "ios";
          }else{
            fs << m_aFrameNativeTarget[ Target::macOS ];
          }
          fs << " = {\n";
          if( bmp->bXcode11 ){
            fs << "            CreatedOnToolsVersion = 11.2.1;\n";
          }else if( bmp->bXcode12 ){
            fs << "            CreatedOnToolsVersion = 12;\n";
          }else if( bmp->bXcode14 ){
            fs << "            CreatedOnToolsVersion = 14.2;\n";
          }
          fs << "          };\n        };\n      };\n";
          fs << "      buildConfigurationList = ";
          if( target == "macos"_64 ){
            fs << m_aBuildConfigurationList[ Target::macOS ];
          }else{
            fs << m_aBuildConfigurationList[ Target::iOS ];
          }
          fs << " /* Build configuration list for PBXProject \""
             << "\" */;\n";
          if( bmp->bXcode11 ){
            fs << "      compatibilityVersion = \"Xcode 9.3\";\n";
          }else if( bmp->bXcode12 ){
            fs << "      compatibilityVersion = \"Xcode 12.0\";\n";
          }else if( bmp->bXcode14 ){
            fs << "      compatibilityVersion = \"Xcode 14.0\";\n";
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
             << "      targets = (\n";
          if( target == "macos"_64 ){
            fs << "        "
              + m_aFrameNativeTarget[ Target::macOS ]
              + " /* "
              + toLabel()
              + " */,\n"
            ;
          }else{
            const auto& framework = m_aFrameNativeTarget[ Target::iOS ];
            fs << "        "
              + framework
              + " /* "
              + toLabel()
              + target
              + " */,\n"
            ;
          }
          fs << "      );\n"
             << "    };\n";
          ++it;
        }
        fs << "    /* End PBXProject section */\n";
      }

      void Workspace::Xcode::writePBXHeadersBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXHeadersBuildPhase section */\n";
          const auto& onHeader=[&]( const string& target ){
            if( target == "ios"_64 ){ fs
              << "    "
              << m_aHeadersBuildPhase[ Target::iOS ]
              << " /* Headers */ = {\n";
            }else{ fs
              << "    "
              << m_aHeadersBuildPhase[ Target::macOS ]
              << " /* Headers */ = {\n";
            }
            fs << "      isa = PBXHeadersBuildPhase;\n"
               << "      buildActionMask = 2147483647;\n"
               << "      files = (\n";
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
          };
          const auto& targets = getTargets();
          auto it = targets.getIterator();
          while( it ){
            onHeader( *it );
            ++it;
          }
        fs << "    /* End PBXHeadersBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXResourcesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXResourcesBuildPhase section */\n";
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          Target t;
          if( it->hash() == "ios"_64 ){
            t = Target::iOS;
          }else{
            t = Target::macOS;
          }
          fs << "    " + m_aResourcesBuildPhase[ t ] + " /* Resources */ = {\n"
              + "      isa = PBXResourcesBuildPhase;\n"
              + "      buildActionMask = 2147483647;\n"
              + "      files = (\n";
          Files files;
          // TODO: This assumes that the iOS and macOS builds have the same resources.
          // TODO: This might not be correct; so, keep an eye on it.
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
          ++it;
        }
        fs << "    /* End PBXResourcesBuildPhase section */\n";
      }

      void Workspace::Xcode::writePBXVariantGroupSection( Writer& fs )const{
        fs << "\n    /* Begin PBXVariantGroup section */\n";
        fs << "    /* End PBXVariantGroup section */\n";
      }

      void Workspace::Xcode::writePBXSourcesBuildPhaseSection( Writer& fs )const{
        fs << "\n    /* Begin PBXSourcesBuildPhase section */\n";
        addToPBXSourcesBuildPhaseSection( fs,
          [&]( const string& source ){
            fs << "    " + source + " /* Sources */ = {\n"
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
          }
        );
        fs << "    /* End PBXSourcesBuildPhase section */\n";
      }

      void Workspace::Xcode::addToXCBuildConfigurationSection( fs::Writer&
          , const std::function<void(
            const string& target
          , const string& relNative
          , const string& dbgNative
          , const string& relBuild
          , const string& dbgBuild )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          const auto& target = *it;
          if( target == "macos"_64 ){
            lambda( "macos"
              , m_aReleaseBuildConfiguration[ Target::macOS ]
              , m_aDebugBuildConfiguration  [ Target::macOS ]
              , m_aReleaseNativeBuildConfig [ Target::macOS ]
              , m_aDebugNativeBuildConfig   [ Target::macOS ]
            );
          }else{
            lambda( "ios"
              , m_aReleaseBuildConfiguration[ Target::iOS ]
              , m_aDebugBuildConfiguration  [ Target::iOS ]
              , m_aReleaseNativeBuildConfig [ Target::iOS ]
              , m_aDebugNativeBuildConfig   [ Target::iOS ]
            );
          }
          ++it;
        }
      }

      void Workspace::Xcode::writeXCBuildConfigurationSection( Writer& fs )const{

        //----------------------------------------------------------------------
        // Local lambda to write out the LDFLAGS section.
        //----------------------------------------------------------------------

        const auto& addOtherCppFlags = [&]( const string& config ){};
        const auto& addOtherLDFlags  = [&](
              const string& config
            , const string& target ){
          auto libs = toLibraryPaths();
          libs.replace( "$(CONFIGURATION)", config );
          libs.splitAtCommas().foreach(
            [&]( const string& f ){
              auto dir = f;
              if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
                dir = "../" + f;
              }
              // Some verbose logging to debug the directory 'dir'.
              if( e_getCvar( bool, "VERBOSE_LOGGING" )){
                e_msgf( "      dir: \"%s\""
                  , ccp( dir )
                );
              }
              fs << "          -L" + dir + ",\n";
            }
          );
        };

        //----------------------------------------------------------------------
        // Begin build configuration section.
        //----------------------------------------------------------------------

        fs << "\n    /* Begin XCBuildConfiguration section */\n";
        addToXCBuildConfigurationSection( fs,
          [&]( const string& target
             , const string& relNative
             , const string& dbgNative
             , const string& relConfig
             , const string& dbgConfig ){

            //------------------------------------------------------------------
            // DEBUG build configuration.
            //------------------------------------------------------------------

            fs << "    " + dbgNative + " /* Debug */ = {\n"
                + "      isa = XCBuildConfiguration;\n"
                + "      buildSettings = {\n";
            string lang;
            switch( wsp->toLanguage() ){
              case"c++23"_64:
                lang = "c++2b";
                break;
              case"c++20"_64:// TODO: Which is right? In msvc.cpp we use 20!
                lang = "c++20";
                break;
              case"c++17"_64:
                lang = "c++17";
                break;
              case"c++14"_64:
                lang = "c++14";
                break;
              case"c++11"_64:
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
            if( !bmp->bXcode11 ){
              fs << "        CLANG_WARN_QUOTED_INCLUDE_IN_FRAMEWORK_HEADER = YES;\n";
            }
            fs << "        CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING = YES;\n"
               << "        CLANG_WARN_BOOL_CONVERSION = YES;\n"
               << "        CLANG_WARN_COMMA = NO;\n"
               << "        CLANG_WARN_CONSTANT_CONVERSION = YES;\n"
               << "        CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS = YES;\n"
               << "        CLANG_WARN_DIRECT_OBJC_ISA_USAGE = YES_ERROR;\n"
               << "        CLANG_WARN_DOCUMENTATION_COMMENTS = NO;\n"
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
            const auto& dbgVector = dbgDefines.splitAtCommas();
            dbgVector.foreach(
              [&]( const auto& define ){
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
                + "        ONLY_ACTIVE_ARCH = YES;\n";
            if( target == "macos"_64 ){
              fs << "        SDKROOT = macosx;\n";
            }else{
              fs << "        SDKROOT = iphoneos;\n";
            }
            fs << "        VERSIONING_SYSTEM = \"apple-generic\";\n"
               << "        VERSION_INFO_PREFIX = \"\";\n"
               << "      };\n"
               << "      name = Debug;\n"
               << "    };\n";
            fs << "    " + relConfig + " /* Release */ = {\n"
                + "      isa = XCBuildConfiguration;\n"
                + "      buildSettings = {\n";
            if( isUniversalBinary() ){
              //Note: no ARCHS = ? gives us a universal binary.
            }else if(( target == "ios" )||isAppleSilicon() ){
              fs << "        VALID_ARCHS = arm64;\n";
              fs << "        ARCHS = arm64;\n";
            }else{
              fs << "        VALID_ARCHS = x86_64;\n";
              fs << "        ARCHS = x86_64;\n";
            }
            fs << "        ALWAYS_SEARCH_USER_PATHS = NO;\n"
               << "        CLANG_ANALYZER_NONNULL = YES;\n"
               << "        CLANG_ANALYZER_NUMBER_OBJECT_CONVERSION = YES_AGGRESSIVE;\n"
               << "        CLANG_CXX_LANGUAGE_STANDARD = \"" + toLanguage() + "\";\n"
               << "        CLANG_CXX_LIBRARY = \"libc++\";\n"
               << "        CLANG_ENABLE_MODULES = YES;\n"
               << "        CLANG_ENABLE_OBJC_ARC = " + enableARC + ";\n"
               << "        CLANG_ENABLE_OBJC_WEAK = YES;\n";
            if( !bmp->bXcode11 ){
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
            if( target == "ios"_64 ){
              fs << string( "        IPHONEOS_DEPLOYMENT_TARGET = " + toDeployment() + ";\n" );
            }else{
              fs << string( "        MACOSX_DEPLOYMENT_TARGET = " + toDeployment() + ";\n" );
            }
            fs << "        MTL_ENABLE_DEBUG_INFO = NO;\n"
               << "        MTL_FAST_MATH = YES;\n";
            if( target == "ios"_64 ){
              fs << "        SDKROOT = iphoneos;\n";
            }else{
              fs << "        SDKROOT = macosx;\n";
            }
            fs << "        VERSIONING_SYSTEM = \"apple-generic\";\n"
               << "        VERSION_INFO_PREFIX = \"\";\n"
               << "      };\n"
               << "      name = Release;\n"
               << "    };\n";
            fs << "    " + dbgNative + " /* Debug */ = {\n"
                + "      isa = XCBuildConfiguration;\n"
                + "      buildSettings = {\n";
            if( isUniversalBinary() ){
              //Note: no ARCHS = ? gives us a universal binary.
            }else if(( target == "ios" )||isAppleSilicon() ){
              fs << "        VALID_ARCHS = arm64;\n";
              fs << "        ARCHS = arm64;\n";
            }else{
              fs << "        VALID_ARCHS = x86_64;\n";
              fs << "        ARCHS = x86_64;\n";
            }
            if( hasEntitlements() ){
              fs << "        CODE_SIGN_ENTITLEMENTS = "
                 << toLabel()
                 << ".entitlements;\n"
              ;
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
                // Fix corner case where 'dir' is destroyed.
                ccp end = strstr( dir, ".framework" );
                if( end ){
                  dir = string( dir, end );
                }
                fs << "          " + dir + ",\n";
              }
            );
            fs << "        );\n";
            fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
            auto frameworkSearchPaths
              = toFrameworkPaths()
              . splitAtCommas();
            frameworkSearchPaths.foreach(
              [&]( const auto& _dir ){
                File dir( _dir );
                if( lookfor( dir )){
                  dir.replace( "$(CONFIGURATION)", "Debug" );
                  fs << "          " + dir + ",\n";
                }
              }
            );
            fs << "        );\n";
            fs << "        SYSTEM_HEADER_SEARCH_PATHS = (\n";
            strings paths;
            if( !toIncludePaths().empty() ){
              const auto& syspaths = toIncludePaths().splitAtCommas();
              hashmap<u64,s8>_;
              syspaths.foreach(
                [&]( const auto& _syspath ){
                  if( _syspath.empty() )
                    return;
                  if( !_.find( _syspath.hash() ))
                    _.set( _syspath.hash(), 1 );
                  else return;
                  File f( _syspath );
                  if( lookfor( f )){
                    paths.push( f );
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
              //----------------------------------|-----------------------------
              //application:{                     |

                case"application"_64:
                  fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
                  if( !toPlistPath().empty() ){
                    fs << "        INFOPLIST_FILE = \"$(SRCROOT)/../" + toPlistPath() + "\";\n";
                  }
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
                  fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
                  fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
                  addOtherCppFlags( "Debug" );
                  fs << "        );\n";
                  fs << "        OTHER_LDFLAGS = (\n";
                  if( isLoadAllSymbols() ){
                    fs << "          -all_load,\n";
                  }
                  addOtherLDFlags( "Debug", target );
                  fs << "        );\n";
                  break;

              //}:                                |
              //framework:{                       |

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
                  addOtherLDFlags( "Debug", target );
                  fs << "        );\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
                  break;

              //}:                                |
              //console:{                         |

                case"console"_64:
                  if( target == "ios"_64 )
                    e_errorf( 987, "Cannot set the target to \"console\" for iOS." );
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
                  fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
                  fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
                  addOtherCppFlags( "Debug" );
                  fs << "        );\n";
                  fs << "        OTHER_LDFLAGS = (\n";
                  if( isLoadAllSymbols() ){
                    fs << "          -all_load,\n";
                  }
                  addOtherLDFlags( "Debug", target );
                  fs << "        );\n";
                  break;

              //}:                                |
              //bundle:{                          |

                case"bundle"_64:
                  if( target == "ios"_64 )
                    e_errorf( 987, "Cannot set the target to \"console\" for iOS." );
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
                  addOtherLDFlags( "Debug", target );
                  fs << "        );\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
                  break;

              //}:                                |
              //shared:{                          |

                case"shared"_64:
                  if( target == "ios"_64 )
                    e_errorf( 987, "Cannot set the target to \"console\" for iOS." );
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
                  addOtherLDFlags( "Debug", target );
                  fs << "        );\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
                  break;

              //}:                                |
              //static:{                          |

                case"static"_64:
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
                  fs << "        EXECUTABLE_PREFIX = lib;\n";
                  break;

              //}:                                |
              //----------------------------------|-----------------------------
            }
            fs << "        SKIP_INSTALL = YES;\n";
            if( toBuild() == "bundle"_64 ){
              fs << "        WRAPPER_EXTENSION = bundle;\n";
            }
            fs << "      };\n";
            fs << "      name = Debug;\n";
            fs << "    };\n";

            //------------------------------------------------------------------
            // Handle all the build types: Release.
            //------------------------------------------------------------------

            fs << "    " + relNative + " /* Release */ = {\n"
               << "      isa = XCBuildConfiguration;\n"
               << "      buildSettings = {\n";
            if( hasEntitlements() ){
              fs << "        CODE_SIGN_ENTITLEMENTS = "
                 << toLabel()
                 << ".entitlements;\n"
              ;
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
            libraryPaths.foreach(
              [&]( const string& f ){
                auto dir = f;
                if(( *dir != '/' )&&( *dir != '~' )&&( *dir != '.' )){
                  dir = "../" + f;
                }
                // Fix corner case where 'dir' is destroyed.
                ccp end = strstr( dir, ".framework" );
                if( end ){
                  dir = string( dir, end );
                }
                dir.replace( "$(CONFIGURATION)", "Release" );
                fs << "          " + dir + ",\n";
              }
            );
            fs << "        );\n";
            fs << "        FRAMEWORK_SEARCH_PATHS = (\n";
            frameworkSearchPaths.foreach(
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
                if( path.empty() )
                  return;
                File f( path );
                if( lookfor( f )){
                  fs << "          " + f.toWhere() + ",\n";
                  return;
                }
                  fs << "          " + f + ",\n";
              }
            );
            fs << "        );\n";
            switch( toBuild().hash() ){
              //----------------------------------|-----------------------------
              //application:{                     |

                case"application"_64:
                  fs << "        ASSETCATALOG_COMPILER_APPICON_NAME = AppIcon;\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
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
                  addOtherLDFlags( "Release", target );
                  fs << "        );\n";
                  break;

              //}:                                |
              //framework:{                       |

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
                  addOtherLDFlags( "Release", target );
                  fs << "        );\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
                  break;

              //}:                                |
              //console:{                         |

                case"console"_64:
                  if( target == "ios"_64 )
                    e_errorf( 987, "Cannot set the target to \"console\" for iOS." );
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
                  fs << "        ENABLE_HARDENED_RUNTIME = " + string( isHardenedRuntime() ? "YES" : "NO" ) + ";\n";
                  fs << "        OTHER_CPLUSPLUSFLAGS = (\n";
                  addOtherCppFlags( "Release" );
                  fs << "        );\n";
                  fs << "        OTHER_LDFLAGS = (\n";
                  if( isLoadAllSymbols() ){
                    fs << "          -all_load,\n";
                  }
                  addOtherLDFlags( "Release", target );
                  fs << "        );\n";
                  break;

              //}:                                |
              //bundle:{                          |

                case"bundle"_64:
                  if( target == "ios"_64 )
                    e_errorf( 987, "Cannot set the target to \"console\" for iOS." );
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
                  addOtherLDFlags( "Debug", target );
                  fs << "        );\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
                  break;

              //}:                                |
              //shared:{                          |

                case"shared"_64:
                  if( target == "ios"_64 )
                    e_errorf( 987, "Cannot set the target to \"console\" for iOS." );
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
                  addOtherLDFlags( "Release", target );
                  fs << "        );\n";
                  fs << "        PRODUCT_BUNDLE_IDENTIFIER = \"" + m_sProductBundleId.tolower() + "\";\n";
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME:c99extidentifier)\";\n";
                  break;

              //}:                                |
              //static:{                          |

                case"static"_64:
                  fs << "        PRODUCT_NAME = \"$(TARGET_NAME)\";\n";
                  fs << "        EXECUTABLE_PREFIX = lib;\n";
                  break;

              //}:                                |
              //----------------------------------|-----------------------------
            }
            fs << "        SKIP_INSTALL = YES;\n";
            if( toBuild() == "bundle"_64 ){
              fs << "        WRAPPER_EXTENSION = bundle;\n";
            }
            fs << "      };\n";
            fs << "      name = Release;\n";
            fs << "    };\n";
          }
        );
        fs << "    /* End XCBuildConfiguration section */\n";
      }

      void Workspace::Xcode::addToXCConfigurationListSection( Writer& fs
          , const std::function<void(
              const string& target
            , const string& config
            , const string& build
            , const string& relNative
            , const string& dbgNative
            , const string& relBuild
            , const string& dbgBuild
            , const string& label )>& lambda )const{
        const auto& targets = getTargets();
        auto it = targets.getIterator();
        while( it ){
          auto target( *it );
          string config;
          string build;
          string relN;
          string relB;
          string dbgN;
          string dbgB;
          string lbl;
          if( target == "macos"_64 ){
            config = m_aBuildConfigurationList   [ Target::macOS ];
            build  = m_aBuildNativeTarget        [ Target::macOS ];
            relN   = m_aReleaseNativeBuildConfig [ Target::macOS ];
            dbgN   = m_aDebugNativeBuildConfig   [ Target::macOS ];
            relB   = m_aReleaseBuildConfiguration[ Target::macOS ];
            dbgB   = m_aDebugBuildConfiguration  [ Target::macOS ];
          }else{//iPhone and iPad.
            config = m_aBuildConfigurationList   [ Target::iOS ];
            build  = m_aBuildNativeTarget        [ Target::iOS ];
            relN   = m_aReleaseNativeBuildConfig [ Target::iOS ];
            dbgN   = m_aDebugNativeBuildConfig   [ Target::iOS ];
            relB   = m_aReleaseBuildConfiguration[ Target::iOS ];
            dbgB   = m_aDebugBuildConfiguration  [ Target::iOS ];
            lbl    = "ios";
          }
          lambda( target
            , config
            , build
            , relN
            , dbgN
            , relB
            , dbgB
            , toLabel() + lbl );
          ++it;
        }
      }

      void Workspace::Xcode::writeXCConfigurationListSection( Writer& fs )const{
        fs << "\n    /* Begin XCConfigurationList section */\n";
        addToXCConfigurationListSection( fs,
          [&]( const string& target
             , const string& config
             , const string& build
             , const string& relN
             , const string& dbgN
             , const string& relB
             , const string& dbgB
             , const string& lbl ){
            fs << "    "
                + build
                + " /* Build configuration list for PBXProject \""
                + lbl
                + "\" */ = {\n"
                + "      isa = XCConfigurationList;\n"
                + "      buildConfigurations = (\n"
                + "        " + relB + " /* Release */,\n"
                + "        " + dbgB + " /* Debug */,\n"
                + "      );\n"
                + "      defaultConfigurationIsVisible = 0;\n"
                + "      defaultConfigurationName = Release;\n"
                + "    };\n";
            fs << "    "
                + config
                + " /* Build configuration list for PBXNativeTarget \""
                + lbl
                + "\" */ = {\n"
                + "      isa = XCConfigurationList;\n"
                + "      buildConfigurations = (\n"
                + "        " + relN + " /* Release */,\n"
                + "        " + dbgN + " /* Debug */,\n"
                + "      );\n"
                + "      defaultConfigurationIsVisible = 0;\n"
                + "      defaultConfigurationName = Release;\n"
                + "    };\n"
             ;
          }
        );
        fs << "    /* End XCConfigurationList section */\n";
      }

    //}:                                          |
    //saveEntitlements:{                          |

      void Workspace::Xcode::saveEntitlements( const string& path )const{
        if( !hasEntitlements() )
          return;
        Writer wr( path
          + "/../"// tmp directory
          + toLabel()
          + ".entitlements"
          , kTEXT );
        wr << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        wr << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
        wr << "<plist version=\"1.0\">\n";
        wr << "<dict>\n";
        if( isEnableJIT() ){
          wr << "  	<key>com.apple.security.cs.allow-jit</key>\n";
          wr << "  <true/>\n";
        }
        if( isDisableLibValidation() ){
          wr << "  <key>com.apple.security.cs.disable-library-validation</key>\n";
          wr << "  <true/>\n";
        }
        wr << "</dict>\n";
        wr << "</plist>\n";
        wr.save( nullptr );
      }

    //}:                                          |
    //hasEntitlements:{                           |

      bool Workspace::Xcode::hasEntitlements()const{
        return isDisableLibValidation();
      }

    //}:                                          |
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
        const auto& ext = path
          . ext()
          . tolower();
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
            [[fallthrough]];
          case".hxx"_64:
            [[fallthrough]];
          case".hh"_64:
            inSources( Type::kHpp ).push( path );
            break;
          case".cpp"_64:
            [[fallthrough]];
          case".cxx"_64:
            [[fallthrough]];
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
        if( !isUnity && !Workspace::bmp->bUnity ){
          writeProject<Xcode>( fs, Type::kCpp );
          writeProject<Xcode>( fs, Type::kMm );
          writeProject<Xcode>( fs, Type::kC );
          writeProject<Xcode>( fs, Type::kM );
        }else{
          u32 i = 0;
          const u32 cores = std::thread::hardware_concurrency();
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
        }else if( Workspace::bmp->bXcode14 ){
          fs << "  objectVersion = 54;\n";//TODO: Change this to proper value.
        }
        fs << "  objects = {\n";
        writePBXBuildFileSection(             fs );
        writePBXFileReferenceSection(         fs );
        writePBXShellScriptBuildPhaseSection( fs );
        writePBXFrameworksBuildPhaseSection(  fs );
        writePBXResourcesBuildPhaseSection(   fs );
        writePBXCopyFilesBuildPhaseSection(   fs );
        writePBXHeadersBuildPhaseSection(     fs );
        writePBXSourcesBuildPhaseSection(     fs );
        writePBXGroupSection(                 fs );
        writePBXVariantGroupSection(          fs );
        writePBXNativeTargetSection(          fs );
        writePBXProjectSection(               fs );
        writeXCBuildConfigurationSection(     fs );
        writeXCConfigurationListSection(      fs );
        fs << "  };\n";
        fs << "  rootObject = " + m_sProjectObject + " /* Project object */;\n";
        fs << "}\n\n// vim:ft=cpp";
      }

    //}:                                          |
    //walkfor:{                                   |

      bool Workspace::Xcode::scanfor( File& ff, const string& files )const{

        //----------------------------------------------------------------------
        // Direct ../ path first.
        //----------------------------------------------------------------------

        if( files.empty() ){
          if( e_fexists( ff ) || e_dexists( ff )){
            switch( *ff ){
              case'~': [[fallthrough]];
              case'/': [[fallthrough]];
              case'.':
                if( ff[ 1 ]=='.' )
                  e_brk( ".. in file specs are illegal" );
                ff.setWhere( ff );
                break;
              default:
                ff.setWhere( "../" + ff );
                break;
            }
            return true;
          }
        }else{
          const auto ln = files.splitAtCommas();
          auto it = ln.getIterator();
          while( it ){
            const auto& spec = *it + "/" + ff;
            if( e_fexists( spec )||
                e_dexists( spec )){
              ff.setWhere( spec );
              return true;
            }
            ++it;
          }
        }

        //----------------------------------------------------------------------
        // Hunt for the bugger.
        //----------------------------------------------------------------------

        switch( *ff ){
          case'~': [[fallthrough]];
          case'/': [[fallthrough]];
          case'.':
            if( !e_fexists( ff ))
              break;
            ff.setWhere( ff.path() );
            return true;
          default:/* relative or search */{
            const auto ln = files.splitAtCommas();
            auto it = ln.getIterator();
            while( it ){
              const auto _ext((( *it )+ff ).ext().tolower() );
              switch( _ext.hash() ){
                case".entitlements"_64:
                  ff.setWhere( ff );
                  return true;
                case".framework"_64:    [[fallthrough]];
                case".dylib"_64:        [[fallthrough]];
                case".a"_64:/**/{
                  const auto/**/ path(( *it )+"/"+ff );
                  if( e_fexists( path ))/* tmp/ rel */{
                    ff.setWhere( "../" + path );
                    return true;
                  }
                  [[fallthrough]];
                }
                default:
                  ++ it;
                  break;
              }
            }
          }
        }
        return false;
      }

    //}:                                          |
    //lookfor:{                                   |

      e_noinline bool Workspace::Xcode::lookfor( Workspace::File& ff )const{
        auto& _this = const_cast<self&>( *this );
        strings paths;
                paths.push( _this.toFrameworkPaths() );
                paths.push( _this.toLibraryPaths() );
        auto it = paths.getIterator();
        while( it ){
          if( scanfor( ff, *it ))
            return true;
          ++it;
        }
        return false;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//Ctor:{                                          |

#ifdef __APPLE__
  #pragma mark (ctor)
#endif

  Workspace::Xcode::Xcode(){
    keyCache.clear();
    libCache.clear();
  }

//}:                                              |
//================================================|=============================
