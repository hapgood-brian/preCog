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

#include<generators.h>
#include<luacore.h>
#include<std.h>
#include<ws.h>
#include<regex>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//Extends:{                                       |

#ifdef __APPLE__
  #pragma mark - Extensions -
#endif

  e_extends( Workspace::Qmake );

//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //extFromEnum:{                               |

      ccp Workspace::Qmake::extFromEnum( const Type e )const{
        switch( e ){
          case decltype( e )::kStaticLib:
            #if e_compiling( osx )||e_compiling( linux )
              return".a";
            #elif e_compiling( microsoft )
              return".lib";
            #endif
          case decltype( e )::kSharedLib:
            #if e_compiling( osx )
              return".dylib";
            #elif e_compiling( linux )
              return".so";
            #elif e_compiling( microsoft )
              return".dll";
            #endif
          case decltype( e )::kCpp:
            return".cpp";
          case decltype( e )::kCxx:
            return".cxx";
          case decltype( e )::kCC:
            return".cc";
          case decltype( e )::kC:
            return".c";
          case decltype( e )::kInl:
            return".inl";
          case decltype( e )::kHpp:
            return".hpp";
          case decltype( e )::kHxx:
            return".hxx";
          case decltype( e )::kH:
            return".h";
          default:
            return"";
        }
      }

    //}:                                          |
    //sortingHat:{                                |

      bool Workspace::Qmake::sortingHat( const string& in_path ){
        const auto& path = File( in_path );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          #if e_compiling( linux )
            case".so"_64:
          #elif e_compiling( osx )
            case".dylib"_64:
          #elif e_compiling( microsoft )
            case".dll"_64:
          #endif
            inSources( Type::kSharedLib ).push( path );
            break;
          #if e_compiling( linux )||e_compiling( osx )
            case".a"_64:
          #elif e_compiling( microsoft )
            case".lib"_64:
          #endif
            inSources( Type::kStaticLib ).push( path );
            break;

          //--------------------------------------------------------------------
          // Source and header file types.
          //--------------------------------------------------------------------

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

      void Workspace::Qmake::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        const auto isUnity = isUnityBuild();
        if( !isUnity||!Workspace::bmp->bUnity ){
          writeProject<Qmake>( fs, Type::kCpp );
          writeProject<Qmake>( fs, Type::kCxx );
          writeProject<Qmake>( fs, Type::kCC );
          writeProject<Qmake>( fs, Type::kC );
        }else{
          const auto cores = u32( std::thread::hardware_concurrency() );
          auto i=0u;
          const_cast<Qmake*>( this )->toUnity().resize( cores );
          const_cast<Qmake*>( this )->unifyProject<Qmake>( Type::kCpp, i );
          const_cast<Qmake*>( this )->unifyProject<Qmake>( Type::kCxx, i );
          const_cast<Qmake*>( this )->unifyProject<Qmake>( Type::kCC,  i );
          const_cast<Qmake*>( this )->unifyProject<Qmake>( Type::kC,   i );
          writeProject<Qmake>( fs, Type::kCpp );
          writeProject<Qmake>( fs, Type::kCxx );
          writeProject<Qmake>( fs, Type::kCC );
          writeProject<Qmake>( fs, Type::kC );
        }

        //----------------------------------------------------------------------
        // Add the comment joke at the top of the file.
        //----------------------------------------------------------------------

        const string commentLine = "#---------------------------------------"
          "----------------------------------------\n";
        const string jokeLine = "#                   The best method for acc"
          "elerating a computer\n#                      is the one that boos"
          "ts it by 9.8 m/s2.\n";
        fs << commentLine
           << jokeLine
           << commentLine
           << "# GENERATED FILE DON'T EDIT IN ANY WAY SHAPE OR FORM SOMETHING "
             "BAD WILL HAPPEN!\n"
           << commentLine;

        //----------------------------------------------------------------------
        // Add template and first bit of config.
        //----------------------------------------------------------------------

        fs << "\n";
        const auto& build = toBuild().tolower();
        switch( build.hash() ){
          case"application"_64:
            fs << "TEMPLATE = app\n";
            break;
          case"console"_64:
            fs << "TEMPLATE = app\n"
               << "CONFIG += console\n";
            break;
          case"shared"_64:
            fs << "TEMPLATE = lib\n";
            break;
          case"static"_64:
            fs << "TEMPLATE = lib\n"
               << "CONFIG += staticlib\n";
            break;
        }
        fs << "CONFIG += "
           << toLanguage()
           << "\n";

        //----------------------------------------------------------------------
        // Handle prefix header.
        //----------------------------------------------------------------------

        if( !toPrefixHeader().empty() ){
          const auto& prefix = toPrefixHeader();
          #if e_compiling( osx ) || e_compiling( linux )
            fs << "QMAKE_CXXFLAGS += -include ../../"
               << prefix
               << "\n";
            fs << "QMAKE_CFLAGS += -include ../../"
               << prefix
               << "\n";
          #elif e_compiling( microsoft )
            fs << "QMAKE_CXXFLAGS += -FI\"../../"
               << prefix
               << "\"\n";
            fs << "QMAKE_CFLAGS += -FI\"../../"
               << prefix
               << "\"\n";
          #endif
        }

        //----------------------------------------------------------------------
        // Add include paths.
        //----------------------------------------------------------------------

        auto& includePaths = toIncludePaths();
        if( ! includePaths.empty() ){
          auto paths = includePaths.splitAtCommas();
          paths.foreach(
            [&]( string& path ){
              fs << "INCLUDEPATH +=";
              if(( *path == '~' )||( *path == '.' )||( *path == '/' )){
                fs << " " << path << "\n";
                return;
              }
              fs << " ../../" << path << "\n";
            }
          );
        }

        //----------------------------------------------------------------------
        // Add libraries on *nix and Windows targets; also handle dependencies.
        //----------------------------------------------------------------------

        auto& linkage = toLinkWith();
        if( ! linkage.empty() ){
          const auto& linkages = linkage.splitAtCommas();
          string lib;
          linkages.foreach(
            [&]( const string& linkage ){
              if( isProject<QMAKE_PROJECT_SLOTS>( linkage )){
                #if e_compiling( osx ) || e_compiling( linux )
                  lib = "-L../" + linkage + " -l" + linkage;
                #elif e_compiling( microsoft )
                  lib = "..\\" + linkage + "\\" + linkage + ".lib";
                #endif
              }else{
                lib = linkage;
              }
              #if e_compiling( osx ) || e_compiling( linux )
                fs << "LIBS += "
                   << lib
                   << "\n";
              #elif e_compiling( microsoft )
                const auto& lwrpath = lib.tolower();
                if((( lwrpath[1] == ':' )&&( *lwrpath >= 'a' && *lwrpath <= 'z' ))
                    ||( *lib == '~' )||( *lib == '.' )||( *lib == '/' )){
                  fs << "LIBS += "
                     << lib.os()
                     << "\n";
                  return;
                }
                fs << "LIBS += "
                   << lib.os()
                   << "\n";
              #endif
            }
          );
        }

        //----------------------------------------------------------------------
        // Extract SOURCES.
        //----------------------------------------------------------------------

        strings sources;
        inSources( Qmake::Type::kCpp ).foreach( [&]( const File& f ){ sources.push( static_cast<const string&>( f )); });
        inSources( Qmake::Type::kCxx ).foreach( [&]( const File& f ){ sources.push( static_cast<const string&>( f )); });
        inSources( Qmake::Type::kCC  ).foreach( [&]( const File& f ){ sources.push( static_cast<const string&>( f )); });
        inSources( Qmake::Type::kC   ).foreach( [&]( const File& f ){ sources.push( static_cast<const string&>( f )); });
        if( !sources.empty() ){
          fs << "\n"
             << commentLine
             << "# Source files.\n"
             << commentLine
             << "\n";
          sources.foreach(
            [&]( const string& s ){
              fs << "SOURCES += ../../"
                 << s
                 << "\n"
              ;
            }
          );
        }

        //----------------------------------------------------------------------
        // Extract HEADERS.
        //----------------------------------------------------------------------

        strings headers;
        inSources( Qmake::Type::kHpp ).foreach( [&]( const File& f ){ headers.push( static_cast<const string&>( f )); });
        inSources( Qmake::Type::kHxx ).foreach( [&]( const File& f ){ headers.push( static_cast<const string&>( f )); });
        inSources( Qmake::Type::kHH  ).foreach( [&]( const File& f ){ headers.push( static_cast<const string&>( f )); });
        inSources( Qmake::Type::kH   ).foreach( [&]( const File& f ){ headers.push( static_cast<const string&>( f )); });
        if( !headers.empty() ){
          fs << "\n"
             << commentLine
             << "# Header files.\n"
             << commentLine
             << "\n";
          headers.foreach(
            [&]( const string& h ){
              fs << "HEADERS += ../../"
                 << h
                 << "\n"
              ;
            }
          );
        }

        //----------------------------------------------------------------------
        // Finish up the file with some vimscript.
        //----------------------------------------------------------------------

        fs << "\n"
           << commentLine
           << "# vim:ft=qmake\n"
           << commentLine
        ;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
