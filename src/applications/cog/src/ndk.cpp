//------------------------------------------------------------------------------
//                    Copyright 2022 Creepy Doll Software LLC.
//                            All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------

#include"generators.h"
#include"luacore.h"
#include"std.h"
#include"ws.h"
#if e_compiling( microsoft )
  //TODO: Figure out how we do symlinks on Windows.
#else
  #include<unistd.h>
#endif
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_extends( Workspace::NDK );

//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //extFromEnum:{                               |

      ccp Workspace::NDK::extFromEnum( const Type e )const{
        switch( e ){
          case decltype( e )::kCpp:
            return".cpp";
          case decltype( e )::kC:
            return".c";
          default:
            return"";
        }
      }

    //}:                                          |
    //sortingHat:{                                |

      bool Workspace::NDK::sortingHat( const string& in_path ){

        //----------------------------------------------------------------------
        // Gather together all the sources and store them off. Ignore them at
        // the end of the function.
        //----------------------------------------------------------------------

        const auto& path = File( in_path );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case ".prefab"_64:
            inSources( Type::kPrefab ).push( path );
            break;
          case ".eon"_64:
            inSources( Type::kEon ).push( path );
            break;
          case ".a"_64:
            inSources( Type::kStaticlib ).push( path );
            break;

          //--------------------------------------------------------------------
          // Source and header file types.
          //--------------------------------------------------------------------

          case ".inl"_64:/**/{
            auto& inl = inSources( Type::kInl );
            if( ! inl.find( path )){
              inl.push( path );
            }
            break;
          }
          case ".hpp"_64:
          case ".hxx"_64:
          case ".hh"_64:/**/{
            auto& hpp = inSources( Type::kHpp );
            if( ! hpp.find( path )){
              hpp.push( path );
            }
            break;
          }
          case ".cpp"_64:
          case ".cxx"_64:
          case ".cc"_64:/**/{
            auto& cpp = inSources( Type::kCpp );
            if( ! cpp.find( path )){
              cpp.push( path );
            }
            break;
          }
          case ".h"_64:/**/{
            auto& h = inSources( Type::kH );
            if( ! h.find( path )){
              h.push( path );
            }
            break;
          }
          case ".c"_64:/**/{
            auto& c = inSources( Type::kC );
            if( ! c.find( path )){
              c.push( path );
            }
            break;
          }
          default:
            return false;
        }
        return true;
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::NDK::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Ignore files.
        //----------------------------------------------------------------------

        const auto& onIgnore=[this]( auto it ){
          while( it ){
            auto ok = false;
            { auto parts = toIgnoreParts();
              parts.erase( "\n" );
              parts.erase( "\t" );
              parts.erase( " " );
              const auto& splits = parts.splitAtCommas();
              splits.foreachs(
                [&]( const auto& split ){
                  if( isIgnoreFile( split
                      , *it )){
                    e_msgf( "  Ignoring %s"
                      , ccp( it->filename() ));
                    ok = true;
                  }
                  return!ok;
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
        onIgnore( const_cast<NDK*>( this )
          -> inSources( Type::kCpp ).getIterator() );
        onIgnore( const_cast<NDK*>( this )
          -> inSources( Type::kC ).getIterator() );

        //----------------------------------------------------------------------
        // Add my funny little calling card; should probably make jokes random.
        //----------------------------------------------------------------------

        const string commentLine = "//--------------------------------------"
          "----------------------------------------\n";
        const string jokeLine = "//                  The best method for acc"
          "elerating a computer\n//                     is the one that boos"
          "ts it by 9.8 m/s2.\n";
        fs << commentLine
           << jokeLine
           << commentLine
           << "// GENERATED FILE DO NOT EDIT IN ANY WAY SHAPE OR FORM SOMETHIN"
             "G BAD WILL HAPPEN\n"
           << commentLine
           << "\n";

        //----------------------------------------------------------------------
        // Write to the build.gradle file.
        //----------------------------------------------------------------------

        switch( toBuild().hash() ){
          case"application"_64:
            fs << "plugins{ id 'cpp-application' }\n";
            break;
          case"shared"_64:
            [[fallthrough]];
          case"static"_64:
            fs << "plugins{ id 'cpp-library' }\n";
            break;
        }
        if( bmp->bNDK ){
          fs <<
            "library{ targetMachines"
            ".add( machines"
            ".android"
            ".architecture( \"aarch64\" ))}\n";
        }else{
          fs <<
            "library{ targetMachines"
            ".add( machines"
            ".macOS"
            ".architecture( \"aarch64\" ))}\n"
          ;
        }

        //----------------------------------------------------------------------
        // Make symlinks to files.
        //----------------------------------------------------------------------

        const auto& onSymlink=[this](
              const auto& path
            , auto it ){
          while( it ){
            const auto& src = string( "./" ).os() + *it;
            const auto& dst = string( "./" ).os() + path + "/" + it->filename();
            if( e_getCvar( bool, "VERBOSE_LOGGING" )){
              e_msgf( "symlink: \"%s\" -> \"%s\""
                , ccp( src )
                , ccp( dst )
              );
            }
            #if!e_compiling( microsoft )
              const auto err = symlink( src, dst );
              if( err )
                e_errorf( 20394, "Didn't make symlink!" );
            #else
              //TODO: Make a 64-bit Windows 10/11 junction.
            #endif
            ++it;
          }
        };

        //----------------------------------------------------------------------
        // Create all the symlinks in public and cpp directories.
        //----------------------------------------------------------------------

        if( !inSources( Type::kCpp ).empty() ){
          auto pubFolder = fs
            . toFilename()
            . path()
            + "public";
          pubFolder.replace( "//", "/" );
          onSymlink( pubFolder
            , inSources( Type::kInl )
            . getIterator() );
          onSymlink( pubFolder
            , inSources( Type::kHpp )
            . getIterator() );
          onSymlink( pubFolder
            , inSources( Type::kH )
            . getIterator()
          );
        }
        if( !inSources( Type::kCpp ).empty() ){
          auto cppFolder = fs
            . toFilename()
            . path()
            + "cpp";
          cppFolder.replace( "//", "/" );
          onSymlink( cppFolder
            , inSources( Type::kCpp )
            . getIterator()
          );
        }
        if( !inSources( Type::kC ).empty() ){
          auto cFolder = fs
            . toFilename()
            . path()
            + "c";
          cFolder.replace( "//", "/" );
          onSymlink( cFolder
            , inSources( Type::kC )
            . getIterator()
          );
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
