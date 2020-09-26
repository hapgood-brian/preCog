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

  e_extends( Workspace::Ninja );

//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //extFromEnum:{                               |

      ccp Workspace::Ninja::extFromEnum( const Type e )const{
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

      bool Workspace::Ninja::sortingHat( const string& in_path ){
        const auto& path = File( in_path );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case ".prefab"_64:
            inSources( Type::kPrefab ).push( path );
            break;
          case ".a"_64:
            inSources( Type::kStaticlib ).push( path );
            break;

          //--------------------------------------------------------------------
          // Source and header file types.
          //--------------------------------------------------------------------

          case ".inl"_64:
            inSources( Type::kInl ).push( path );
            break;
          case ".hpp"_64:
          case ".hxx"_64:
          case ".hh"_64:
            inSources( Type::kHpp ).push( path );
            break;
          case ".cpp"_64:
          case ".cxx"_64:
          case ".cc"_64:
            inSources( Type::kCpp ).push( path );
            break;
          case ".h"_64:
            inSources( Type::kH ).push( path );
            break;
          case ".c"_64:
            inSources( Type::kC ).push( path );
            break;
          default:
            return false;
        }
        return true;
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::Ninja::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        if( !isUnityBuild() ){
          writeProject<Ninja>( fs, Type::kCpp );
          writeProject<Ninja>( fs, Type::kC );
        }else{
          const auto cores = u32( std::thread::hardware_concurrency() );
          auto i=0u;
          const_cast<Ninja*>( this )->toUnity().resize( cores );
          const_cast<Ninja*>( this )->unifyProject<Ninja>( Type::kCpp, i );
          const_cast<Ninja*>( this )->unifyProject<Ninja>( Type::kC,   i );
          writeProject<Ninja>( fs, Type::kCpp );
          writeProject<Ninja>( fs, Type::kC );
        }

        //----------------------------------------------------------------------
        // Create CFLAGS variable in build ninja.
        //----------------------------------------------------------------------

        const auto clabel = toLabel().toupper() + "_CFLAGS";
        const auto cstart = clabel + " = ";
        string cflags = cstart;
        if( !toIncludePaths().empty() ){
          const auto& includePaths = toIncludePaths().splitAtCommas();
          includePaths.foreach(
            [&]( const string& path ){
              if( path.empty() ){
                return;
              }
              cflags << "\\\n  -I../" << path;
            }
          );
        }
        if( !toDefinesRel().empty() ){
          const auto& defines = toDefinesRel().splitAtCommas();
          defines.foreach(
            [&]( const string& define ){
              cflags << "\\\n  -D" << define;
            }
          );
        }
        if( !toPrefixHeader().empty() ){
          const auto& prefix = toPrefixHeader();
          if( !prefix.empty() ){
            cflags << "\\\n  -include ../" << prefix << "\n";
          }
        }
        if( cstart != cflags ){
          fs << cflags << "\n";
        }

        //----------------------------------------------------------------------
        // Create LFLAGS variable in build ninja.
        //----------------------------------------------------------------------

        const auto llabel = toLabel().toupper() + "_LFLAGS";
        const auto lstart = llabel + " = ";
        string lflags = lstart;
        if( lstart != lflags ){
          fs << lflags << "\n";
        }

        //----------------------------------------------------------------------
        // Construct C++ command string based on environment.
        //----------------------------------------------------------------------

        const string cxx_start = "command = ";
        string cxx = cxx_start;
        if( cstart != cflags ){
          if( e_fexists( "/usr/bin/clang++" )){
            cxx << "/usr/bin/clang++";
          }else if( e_fexists( "/usr/bin/g++" )){
            cxx << "/usr/bin/g++";
          }else{
            e_errorf( 870612, "Compiler not found." );
            return;
          }
          cxx << " $" << clabel << " -MD -MT $out -MF $DEP_FILE -o $out -c $in\n";
        }

        //----------------------------------------------------------------------
        // Write CXX compilation rule string.
        //----------------------------------------------------------------------

        if( cxx != cxx_start ){
          fs << "rule CXX_" << toLabel().toupper() + "\n";
          fs << "  depfile $DEP_FILE\n";
          fs << "  deps = gcc\n";
          fs << "  " + cxx;
          fs << "  description = Building C++ object $out\n";
          fs << "\n";
        }

        //----------------------------------------------------------------------
        // Construct C command string based on environment.
        //----------------------------------------------------------------------

        const string c_start = "command = ";
        string c = c_start;
        if( cstart != cflags ){
          if( e_fexists( "/usr/bin/clang" )){
            c << "/usr/bin/clang";
          }else if( e_fexists( "/usr/bin/gcc" )){
            c << "/usr/bin/gcc";
          }else{
            e_errorf( 870612, "Compiler not found." );
            return;
          }
          c << " $" << clabel << " -MD -MT $out -MF $DEP_FILE -o $out -c $in\n";
        }

        //----------------------------------------------------------------------
        // Write C compilation rule string.
        //----------------------------------------------------------------------

        if( c != c_start ){
          fs << "rule C_" << toLabel().toupper() + "\n";
          fs << "  depfile $DEP_FILE\n";
          fs << "  deps = gcc\n";
          fs << "  " + c;
          fs << "  description = Building C object $out\n";
          fs << "\n";
        }

        //----------------------------------------------------------------------
        // Write CXX linking rule string. Favors clang over gcc.
        //----------------------------------------------------------------------

        switch( toBuild().hash() ){

          //--------------------------------------------------------------------
          // Static libraries of type a (Microsoft not supported by Ninja path)
          //--------------------------------------------------------------------

          case"static"_64:
            #if e_compiling( linux ) || e_compiling( osx )
              fs << "rule STATIC_LIB_" << toLabel().toupper() + "\n";
            #else
              fs << "rule COFF_LIB_" << toLabel().toupper() + "\n";
            #endif
            fs << "  depfile $DEP_FILE\n";
            fs << "  deps = gcc\n";
            fs << "  command = $PRE_LINK && ";
            if( e_fexists( "/usr/bin/ar" )){
              fs << "/usr/bin/ar qc $TARGET_FILE $LINK_PATH $LINK_LIBRARIES && /usr/bin/ranlib $TARGET_FILE && $POST_BUILD\n";
            }else{
              e_errorf( 870612, "Compiler not found." );
              return;
            }
            #if e_compiling( linux ) || e_compiling( osx )
              fs << "  description = Linking library $out\n";
            #else
              fs << "  description = Linking COFF library $out\n";
            #endif
            fs << "  restat = $RESTAT\n";
            break;

          //--------------------------------------------------------------------
          // Shared libraries of type so and dylib. Microsoft not supported.
          //--------------------------------------------------------------------

          case"shared"_64:
            break;

          //--------------------------------------------------------------------
          // Executables.
          //--------------------------------------------------------------------

          case"application"_64:
            #if e_compiling( linux )
              [[fallthrough]];
            #else
              break;
            #endif

          case"console"_64:
            #if e_compiling( linux ) || e_compiling( osx )
              fs << "rule ELF_LINKER_" << toLabel().toupper() + "\n";
            #else
              fs << "rule PE_LINKER_" << toLabel().toupper() + "\n";
            #endif
            fs << "  depfile $DEP_FILE\n";
            fs << "  deps = gcc\n";
            fs << "  command = $PRE_LINK && ";
            if( e_fexists( "/usr/bin/clang++" )){
              fs << "/usr/bin/clang++";
            }else if( e_fexists( "/usr/bin/g++" )){
              fs << "/usr/bin/g++";
            }else{
              e_errorf( 870612, "Compiler not found." );
              return;
            }
            if( lstart != lflags ){
              fs << " $" << llabel;
            }
            fs << " $in -o $TARGET_FILE $LINK_PATH $LINK_LIBRARIES && $POST_BUILD\n";
            #if e_compiling( linux ) || e_compiling( osx )
              fs << "  description = Linking ELF binary $out\n";
            #else
              fs << "  description = Linking PE binary $out\n";
            #endif
            break;
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
