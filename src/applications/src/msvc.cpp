//------------------------------------------------------------------------------
//       Copyright 2014-2020 Creepy Doll Games LLC. All rights reserved.
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
//Configs:{                                       |

  namespace{
    constexpr ccp anon_aConfigs[2]{ "Debug", "Release" };
  }

//}:                                              |
//Methods:{                                       |
  //[project]:{                                   |
    //extFromSource<>:{                           |

      ccp Workspace::MSVC::extFromEnum( const Type e )const{
        switch( e ){
          case decltype( e )::kDef:
            return".def";
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

      bool Workspace::MSVC::sortingHat( const string& in_path ){

        //----------------------------------------------------------------------
        // Solutions need Windows GUIDs. If we're running on Windows then you
        // are guaranteed to have a GUID that's universally unique. If running
        // on the Mac or Linux, you're only guaranteed to have file uniqueness.
        //----------------------------------------------------------------------

        File path = in_path;
        path.setBuildID(   string::guid() );
        path.setFileRefID( string::guid() );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case".lib"_64:
            inSources( Type::kLib ).push( path );
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
          case".def"_64:
            inSources( Type::kDef ).push( path );
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
    //writePropertyGroup:{                        |

      void Workspace::MSVC::writePropGroup( Writer& fs, const string& group, const string& config )const{
        switch( group.hash() ){
          case"Condition"_64:
            fs << "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\" Label=\"Configuration\">\n";
            switch( toBuild().hash() ){
              case"application"_64:
                [[fallthrough]];
              case"console"_64:
                fs << "\t<ConfigurationType>Application</ConfigurationType>\n";
                break;
              case"shared"_64:
                fs << "\t<ConfigurationType>DynamicLibrary</ConfigurationType>\n";
                break;
              case"static"_64:
                fs << "\t<ConfigurationType>StaticLibrary</ConfigurationType>\n";
                break;
            }
            fs << "\t<CharacterSet>"+toUnicodeType()+"</CharacterSet>\n";
            fs << "\t<PlatformToolset>"+toPlatformTools()+"</PlatformToolset>\n";
            fs << "</PropertyGroup>\n";
            break;
        }
      }

      void Workspace::MSVC::writePropGroup( Writer& fs, const string& group )const{

        //----------------------------------------------------------------------
        // Handle target extensions for both Debug and Release.
        //----------------------------------------------------------------------

        if( "TargetExt"_64 == group.hash() ){
          for( u32 n=e_dimof( anon_aConfigs ), i=0; i<n; ++i ){
            fs << "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='"
              + string( anon_aConfigs[ i ])
              + "|"
              + m_sArchitecture
              + "'\">";
            fs << "\t<TargetExt>"+/* Workspace */ext+"</TargetExt>\n";
            fs << "</PropertyGroup>\n";
          }
          return;
        }

        //----------------------------------------------------------------------
        // Handle all other property group types.
        //----------------------------------------------------------------------

        if( "<arch>"_64 != group.hash() ){
          fs << "<PropertyGroup Label=\""+group+"\">\n";
        }else{
          fs << "<PropertyGroup>\n";
        }
        switch( group.hash() ){
          case"UserMacros"_64:
            // TODO: Put in user macros here.
            break;
          case"Globals"_64:
            fs << "\t<ProjectGuid>"+toProjectGUID()+"</ProjectGuid>\n";
            fs << "\t<WindowsTargetPlatformVersion>"+toWindowsSDK()+"</WindowsTargetPlatformVersion>\n";
            fs << "\t<Keyword>Win32Proj</Keyword>\n";
            fs << "\t<Platform>"+m_sArchitecture+"</Platform>\n";
            fs << "\t<ProjectName>"+toLabel()+"</ProjectName>\n";
            fs << "\t<VCProjectUpgraderObjectName>NoUpgrade</VCProjectUpgraderObjectName>\n";
            break;
          // Anything in <> is a special case and not sent through to the vcxproj.
          case"<arch>"_64:
            fs << "\t<PreferredToolArchitecture>"+toPreferredArch()+"</PreferredToolArchitecture>\n";
            break;
        }
        fs << "</PropertyGroup>\n";
      }

    //}:                                          |
    //writeItemGroup:{                            |

      void Workspace::MSVC::writeItemGroup( Writer& fs, const string& group )const{
        switch( group.hash() ){
          case"ProjectConfigurations"_64:
            fs << "<ItemGroup Label=\"ProjectConfigurations\">\n";
            fs << "\t<ProjectConfiguration Include=\"Debug|"+m_sArchitecture+"\">\n";
            fs << "\t\t<Configuration>Debug</Configuration>\n";
            fs << "\t\t<Platform>"+m_sArchitecture+"</Platform>\n";
            fs << "\t</ProjectConfiguration>\n";
            fs << "\t<ProjectConfiguration Include=\"Release|"+m_sArchitecture+"\">\n";
            fs << "\t\t<Configuration>Release</Configuration>\n";
            fs << "\t\t<Platform>"+m_sArchitecture+"</Platform>\n";
            fs << "\t</ProjectConfiguration>\n";
            fs << "</ItemGroup>\n";
            break;
          case"<source>"_64:
            fs << "<ItemGroup>\n";
            toSources().foreach(
              [&]( const Files& files ){
                if( files.empty() ){
                  return;
                }
                auto it = files.getIterator();
                while( it ){
                  if( *it ){
                    if( e_fexists( *it )){
                      string path( *it );
                      // Need to handle all the pathing cases: including "c:/", "../", "/hello", etc.
                      path.replace( "&", "&amp;" );
                      string osPath;
                      if(( *path == '/' )||( path[ 1 ]==':' )||( *path == '.' )){
                        osPath = path.os();
                      }else{
                        osPath = "../"+path.os();
                      }
                      // If the filename has a ampersand in it the final project will break
                      // because vcxproj's are XML files really.
                      const auto& ext = it->ext().tolower();
                      switch( ext.hash() ){
                        case".cpp"_64:
                        case".cxx"_64:
                        case".cc"_64:
                        case".c"_64:
                          fs << "\t<ClCompile Include=\""+osPath+"\"/>\n";
                          break;
                        case".def"_64:
                          fs << "\t<None Include=\""+osPath+"\"/>\n";
                          break;
                        case".inl"_64:
                        case".hxx"_64:
                        case".hh"_64:
                        case".hpp"_64:
                        case".h"_64:
                          fs << "\t<ClInclude Include=\""+osPath+"\"/>\n";
                          break;
                        case".png"_64:
                        case".bmp"_64:
                        case".jpg"_64:
                        case".tga"_64:
                          // This should capture all the image data; we'll need something special for .rc files.
                          fs << "\t<Image Include=\""+osPath+"\">\n";
                          fs << "\t\t<ExcludedFromBuild Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">true</ExcludedFromBuild>\n";
                          fs << "\t\t<ExcludedFromBuild Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</ExcludedFromBuild>\n";
                          fs << "\t</Image>\n";
                          break;
                        default:
                          break;
                      }
                    }
                  }
                  ++it;
                }
              }
            );
            fs << "</ItemGroup>\n";
            break;
        }
      }

    //}:                                          |
    //writeProjVersion:{                          |

      void Workspace::MSVC::writeProjVersion( Writer& fs )const{
        fs << "<_ProjectFileVersion>"+m_sProjectVersion+"</_ProjectFileVersion>\n";
      }

    //}:                                          |
    //writeSetDirectory:{                         |

      void Workspace::MSVC::writeSetDirectory( Writer& fs, const string& tag, const string& config, const string& path )const{
        fs << "<"+tag+" Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">$(SolutionDir)"+path+"\\$(Configuration)\\$(TargetName)\\$(PlatformTarget)\\</"+tag+">\n";
      }

    //}:                                          |
    //extFromBuildString:{                        |

      //https://help.autodesk.com/view/MAXDEV/2021/ENU/?guid=__developer_writing_plug_ins_creating_a_plug_in_project_manually_creating_a_new_plug_in__plug_in_file_extensions_html
      ccp Workspace::MSVC::extFromBuildString()const{
        switch( toBuild().hash() ){
          case"application"_64:
            [[fallthrough]];
          case"console"_64:
            return"exe";
          case"static"_64:
            return"lib";
          case"shared"_64:
            if( bmp->bMaxPlugin ){
              return"dlu";
            }
            return"dll";
        }
        return nullptr;
      }

    //}:                                          |
    //writeManfestData:{                          |

      void Workspace::MSVC::writeManifestData( Writer& fs, const string& config )const{
        fs << "<GenerateManifest Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">"+m_sGenManifest+"</GenerateManifest>\n";
      }

    //}:                                          |
    //writeTargetVar:{                            |

      void Workspace::MSVC::writeTargetVar( Writer& fs, const string& config, const string& tag )const{
        switch( tag.hash() ){
          case"Name"_64:
            fs << "<TargetName Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">"+toLabel()+"</TargetName>\n";
            break;
          case"Ext"_64:
            fs << "<TargetExt Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">."+extFromBuildString()+"</TargetExt>\n";
            break;
        }
      }

    //}:                                          |
    //writeLinkerVar:{                            |

      void Workspace::MSVC::writeLinkerVar( Writer& fs, const string& label, const string& config )const{
        switch( label.hash() ){
          case"Incremental"_64:
            fs << "<LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">"+m_sLinkIncremental+"</LinkIncremental>\n";
            break;
        }
      }

    //}:                                          |
    //writeImport*:{                              |

      void Workspace::MSVC::writeImportGroup( Writer& fs, const string& label, const string& path )const{
        fs << "<ImportGroup Label=\""+label+"\">\n";
        switch( label.hash() ){
          case"PropertySheets"_64:
            fs << "<Import Project=\"$(UserRootDir)\\"+path+"\" Condition=\"exists('$(UserRootDir)\\"+path+"')\" Label=\"LocalAppDataPlatform\"/>\n";
            break;
        }
        fs << "</ImportGroup>\n";
      }

      void Workspace::MSVC::writeImportGroup( Writer& fs, const string& label )const{
        fs << "<ImportGroup Label=\""+label+"\">\n";
        switch( label.hash() ){
          case"ExtensionSettings"_64:
            break;
        }
        fs << "</ImportGroup>\n";
      }

      void Workspace::MSVC::writeImport( Writer& fs, const string& type, const string& path )const{
        fs << "<Import "+type+"=\"$(VCTargetsPath)\\"+path+"\"/>\n";
      }

    //}:                                          |
    //wrieItemDefGroup:{                          |

      void Workspace::MSVC::writeItemDefGroup( Writer& fs, const string& config )const{
        fs << "<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">\n";
        fs << "\t<ClCompile>\n";
        fs << "\t\t<AdditionalIncludeDirectories>";
        if( !toIncludePaths().empty() ){
          const auto& syspaths = toIncludePaths().splitAtCommas();
          strings paths;
          syspaths.foreach(
            [&]( const string& syspath ){
              if( syspath.empty() ){
                return;
              }
              if( *syspath == '/' ){
                paths.push( syspath );
              }else if( *syspath == '.' ){
                paths.push( syspath );
              }else if( syspath[ 1 ] == ':' ){
                paths.push( syspath );
              }else{
                paths.push( "../" + syspath );
              }
            }
          );
          auto it = paths.getIterator();
          while( it ){
            const auto& s = *it++;
            fs << s.os() + ";";
          }
        }
        fs << "%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n";
        fs << "\t\t<AssemblerListingLocation>$(IntDir)</AssemblerListingLocation>\n";
        fs << "\t\t<BasicRuntimeChecks>"+m_sBasicRuntimeChk+"</BasicRuntimeChecks>\n";
        fs << "\t\t<CompileAs>Default</CompileAs>\n";
        fs << "\t\t<DebugInformationFormat>"+m_sDebugInfoFormat+"</DebugInformationFormat>\n";
        fs << "\t\t<ExceptionHandling>"+m_sExceptionHndlng+"</ExceptionHandling>\n";
        if( !toPrefixHeader().empty() ){
          const auto& prefix = toPrefixHeader();
          fs << "\t\t<ForcedIncludeFiles>"+prefix.os()+"</ForcedIncludeFiles>\n";
        }
        fs << "\t\t<InlineFunctionExpansion>";
        switch( config.hash() ){
          case"Debug"_64:
            fs << "Disabled";
            break;
          case"Release"_64:
            fs << "AnySuitable";
            break;
        }
        fs << "</InlineFunctionExpansion>\n";
        fs << "\t\t<LanguageStandard>";
        switch( toLanguage().hash() ){
          case"c++17"_64:
            fs << "stdcpp17";
            break;
          case"c++14"_64:
            fs << "stdcpp14";
            break;
          case"c++11"_64:
            fs << "stdcpp11";
            break;
        }
        fs << "</LanguageStandard>\n";
        fs << "\t\t<Optimization>";
        switch( config.hash() ){
          case"Debug"_64:
            fs << "Disabled";
            break;
          case"Release"_64:
            fs << "MaxSpeed";
            break;
        }
        fs << "</Optimization>\n";
        fs << "\t\t<PrecompiledHeader>"+m_sPCH+"</PrecompiledHeader>\n";
        switch( config.hash() ){
          case"Debug"_64:
            fs << "\t\t<RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\n";
            break;
          case"Release"_64:
            fs << "\t\t<RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\n";
            break;
        }
        fs << "\t\t<RuntimeTypeInfo>"+m_sRTTI+"</RuntimeTypeInfo>\n";
        fs << "\t\t<UseFullPaths>false</UseFullPaths>\n";
        fs << "\t\t<WarningLevel>"+m_sWarningLevel+"</WarningLevel>\n";
        fs << "\t\t<PreprocessorDefinitions>";
        switch( config.hash() ){
          case"Debug"_64:/**/{
            string defs = toDefinesDbg() + ";__compiling_" + toLabel().tolower()+"__=1";
            defs.replace( "\t", "" );
            defs.replace( "\n", "" );
            defs.replace( ",", ";" );
            defs.replace( " ", "" );
            fs << defs + ";";
            break;
          }
          case"Release"_64:/**/{
            string defs = toDefinesRel() + ";__compiling_" + toLabel().tolower()+"__=1";
            defs.replace( "\t", "" );
            defs.replace( "\n", "" );
            defs.replace( ",", ";" );
            defs.replace( " ", "" );
            fs << defs + ";";
            break;
          }
        }
        fs << "%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
        fs << "\t\t<ObjectFileName>$(IntDir)</ObjectFileName>\n";
        if( config.hash() == "Release"_64 ){
          fs << "\t\t<StringPooling>true</StringPooling>\n";
          fs << "\t\t<FunctionLevelLinking>true</FunctionLevelLinking>\n";
          fs << "\t\t<EnableEnhancedInstructionSet>AdvancedVectorExtensions</EnableEnhancedInstructionSet>\n";
          fs << "\t\t<IntrinsicFunctions>true</IntrinsicFunctions>\n";
          fs << "\t\t<FavorSizeOrSpeed>Speed</FavorSizeOrSpeed>\n";
        }
        fs << "\t\t<AdditionalOptions>/bigobj %(AdditionalOptions)</AdditionalOptions>\n";
        fs << "\t</ClCompile>\n";
        fs << "\t<Link>\n";
        fs << "\t\t<AdditionalDependencies>";
        string libs = toLinkWith();
        libs.replace( "\t", "" );
        libs.replace( "\n", "" );
        const strings& libList = libs.splitAtCommas();
        libs.replace( ",", ";" );
        fs << libs + ";";
        fs << "kernel32.lib;user32.lib;gdi32.lib;winspool.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;comdlg32.lib;advapi32.lib";
        fs << "</AdditionalDependencies>\n";
        fs << "\t\t<AdditionalLibraryDirectories>";
        string dirs = toLibraryPaths();
        dirs.replace( "\t", "" );
        dirs.replace( "\n", "" );
        const strings& dirList = dirs.splitAtCommas();
        dirs.replace( ",", ";" );
        auto it = dirList.getIterator();
        string path;
        while( it ){
          if(( *it )[ 1 ]==':' ){
            path = *it;
          }else{
            switch( **it ){
              case'/':
                path = it->os();
                break;
              case'.':
                path = it->os();
                break;
              default:
                path = "..\\" + it->os();
                break;
            }
          }
          fs << path + ";";
          ++it;
        }
        auto i2 = libList.getIterator();
        while( i2 ){
          fs << "$(SolutionDir).output/$(Configuration)/"+i2->basename()+"/$(PlatformTarget);";
          ++i2;
        }
        fs << "%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\n";
        fs << "\t\t<AdditionalOptions>%(AdditionalOptions) /machine:"+m_sArchitecture+"</AdditionalOptions>\n";
        fs << "\t\t<GenerateDebugInformation>"+m_sGenReleaseDBInf+"</GenerateDebugInformation>\n";
        fs << "\t\t<IgnoreSpecificDefaultLibraries>%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\n";
        fs << "\t\t<ProgramDataBaseFile>$(IntDir)"+toLabel()+".pdb</ProgramDataBaseFile>\n";
        switch( toBuild().hash() ){
          case"application"_64:
            fs << "\t\t<SubSystem>Windows</SubSystem>\n";
            break;
          case"console"_64:
            fs << "\t\t<SubSystem>Console</SubSystem>\n";
            break;
        }
        switch( toBuild().hash() ){
          case"application"_64:
            [[fallthrough]];
          case"console"_64:/**/{
            const string& path = "$(SolutionDir).output/$(Configuration)/$(TargetName)/$(PlatformTarget)";
            fs << "\t\t<OutputFile>" + path + "/" + toLabel() + ".exe</OutputFile>\n";
            break;
          }
        }
        fs << "\t</Link>\n";
        fs << "\t<ProjectReference>\n";
        fs << "\t\t<LinkLibraryDependencies>"+m_sLinkLibDepends+"</LinkLibraryDependencies>\n";
        fs << "\t</ProjectReference>\n";
        fs << "</ItemDefinitionGroup>\n";
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::MSVC::serialize( Writer& fs )const{

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        if( !isUnityBuild() ){
          writeProject<MSVC>( fs, Type::kCpp );
          writeProject<MSVC>( fs, Type::kC );
        }else{
          const u32 cores = std::thread::hardware_concurrency();
          u32 i=0;
          const_cast<MSVC*>( this )->toUnity().resize( cores );
          const_cast<MSVC*>( this )->unifyProject<MSVC>( Type::kCpp, i );
          const_cast<MSVC*>( this )->unifyProject<MSVC>( Type::kC,   i );
          writeProject<MSVC>( fs, Type::kCpp );
          writeProject<MSVC>( fs, Type::kC );
        }

        //----------------------------------------------------------------------
        // Save MSVC project to XML format file.
        //----------------------------------------------------------------------

        fs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
        fs << "<Project DefaultTargets=\"Build\" ToolsVersion=\"16.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
        writePropGroup( fs, "<arch>" );
        writeItemGroup( fs, "ProjectConfigurations" );
        writeItemGroup( fs, "<source>" );
        writePropGroup( fs, "Globals" );
        writeImport(    fs, "Project", "Microsoft.Cpp.Default.props" );
        for( u32 n=e_dimof( anon_aConfigs ), i=0; i<n; ++i ){
          writePropGroup( fs, "Condition", anon_aConfigs[ i ]);
        }
        writeImport(      fs, "Project", "Microsoft.Cpp.props" );
        writeImportGroup( fs, "ExtensionSettings" );
        writeImportGroup( fs, "PropertySheets", "Microsoft.Cpp.$(Platform).user.props" );
        writePropGroup(   fs, "UserMacros" );
        fs << "<PropertyGroup>\n";
          writeProjVersion(  fs );
          for( u32 n=e_dimof( anon_aConfigs ), i=0; i<n; ++i ){
            writeSetDirectory( fs, "OutDir",      anon_aConfigs[ i ], m_sOutDir );
            writeSetDirectory( fs, "IntDir",      anon_aConfigs[ i ], m_sIntDir );
            writeTargetVar(    fs, "Name",        anon_aConfigs[ i ]);
            writeTargetVar(    fs, "Ext",         anon_aConfigs[ i ]);
            writeLinkerVar(    fs, "Incremental", anon_aConfigs[ i ]);
            writeManifestData( fs, "Debug" );
          }
        fs << "</PropertyGroup>\n";
        if( !ext.empty() ){
          writePropGroup( fs, "TargetExt" );
        }
        for( u32 n=e_dimof( anon_aConfigs ), i=0; i<n; ++i ){
          writeItemDefGroup( fs, anon_aConfigs[ i ]);
        }
        fs << "<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\"/>\n";
        fs << "\t<ImportGroup Label=\"ExtensionTargets\">\n";
        fs << "</ImportGroup>\n";
        fs << "</Project>\n";

        //----------------------------------------------------------------------
        // Create filters file parallel to this project.
        //----------------------------------------------------------------------

        Writer filters( fs.toFilename() + ".filters", kTEXT );
        writeFilter( filters );
        filters.save();
      }

    //}:                                          |
    //writeFilter:{                               |

      void Workspace::MSVC::writeFilter( Writer& fs )const{

        fs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
        fs << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";

        //----------------------------------------------------------------------
        // Base directory for filtering from. This should come from somewhere in
        // the project rather than just guessing;  will fail if user doesn't lay
        // out the project src/ and include/.
        //----------------------------------------------------------------------

        static constexpr ccp kIncludeFilterName = "include";

        //----------------------------------------------------------------------
        // Include filter.
        //----------------------------------------------------------------------

        Files includes;
        includes.pushVector( inSources( Type::kHpp ));
        includes.pushVector( inSources( Type::kInl ));
        includes.pushVector( inSources( Type::kH ));
        if( !includes.empty() ){
          fs << "\t<ItemGroup>\n";
          auto it = includes.getIterator();
          string path;
          while( it ){
            const auto& f = *it;
            if( f.left( 4 ).tolower().hash() == "tmp/"_64 ){
              path = "./" + f.os();
            }else if( f[ 1 ]==':' ){
              path = f.os();
            }else{
              switch( *f ){
                case'/':
                  [[fallthrough]];
                case'.':
                  path = f.os();
                  break;
                default:
                  path = "../" + f.os();
                  break;
              }
            }
            path.replace( "&", "&amp;" );
            fs << "    <ClInclude Include=\"" + path + "\">\n";
            fs << "      <Filter>"+string( strstr( f.path(), kIncludeFilterName )).trimmed( 1 ).os()+"</Filter>\n";
            fs << "    </ClInclude>\n";
            ++it;
          }
          fs << "\t</ItemGroup>\n";
        }

        //----------------------------------------------------------------------
        // Filter GUIDs.
        //----------------------------------------------------------------------

        fs << "  <ItemGroup>\n";
        if( !includes.empty() ){
          fs << "    <Filter Include=\""+string( kIncludeFilterName )+"\">\n";
          fs << "      <UniqueIdentifier>"+string::guid()+"</UniqueIdentifier>\n";
          fs << "    </Filter>\n";
          auto it = includes.getIterator();
          while( it ){
            const auto& f = string( strstr( *it, kIncludeFilterName ));
            ccp p = f;
            while( *p ){
              if( *p == '/' ){
                const string fn( f.c_str(), p );
                fs << "    <Filter Include=\""+fn.os()+"\">\n";
                fs << "      <UniqueIdentifier>"+string::guid()+"</UniqueIdentifier>\n";
                fs << "    </Filter>\n";
              }
              ++p;
            }
            ++it;
          }
        }
        fs << "    <Filter Include=\"images\">\n";
        fs << "      <UniqueIdentifier>"+string::guid()+"</UniqueIdentifier>\n";
        fs << "    </Filter>\n";
        fs << "    <Filter Include=\"defs\">\n";
        fs << "      <UniqueIdentifier>"+string::guid()+"</UniqueIdentifier>\n";
        fs << "    </Filter>\n";
        fs << "    <Filter Include=\"src\">\n";
        fs << "      <UniqueIdentifier>"+string::guid()+"</UniqueIdentifier>\n";
        fs << "    </Filter>\n";
        fs << "  </ItemGroup>\n";

        //----------------------------------------------------------------------
        // Images filter.
        //----------------------------------------------------------------------

        Files images;
        images.pushVector( inSources( Type::kPng ));
        if( !images.empty() ){
          fs << "  <ItemGroup>\n";
          auto i2 = images.getIterator();
          while( i2 ){
            auto f = *i2;
            f.replace( "&", "&amp;" );
            const auto& path = "../" + f.os();
            fs << "    <Image Include=\""+path+"\">\n";
            fs << "      <Filter>images</Filter>\n";
            fs << "    </Image>\n";
            ++i2;
          }
          fs << "  </ItemGroup>\n";
        }

        //----------------------------------------------------------------------
        // DEF file filter.
        //----------------------------------------------------------------------

        Files defs;
        defs.pushVector( inSources( Type::kDef ));
        if( !defs.empty() ){
          fs << "  <ItemGroup>\n";
          auto i2 = defs.getIterator();
          while( i2 ){
            auto f = *i2;
            f.replace( "&", "&amp;" );
            const auto& path = "../" + f.os();
            fs << "    <None Include=\""+path+"\">\n";
            fs << "      <Filter>defs</Filter>\n";
            fs << "    </None>\n";
            ++i2;
          }
          fs << "  </ItemGroup>\n";
        }

        //----------------------------------------------------------------------
        // Source filter.
        //----------------------------------------------------------------------

        Files srcs;
        srcs.pushVector( inSources( Type::kCpp ));
        srcs.pushVector( inSources( Type::kC ));
        if( !srcs.empty() ){
          fs << "  <ItemGroup>\n";
          auto i2 = srcs.getIterator();
          while( i2 ){
            auto f = *i2;
            f.replace( "&", "&amp;" );
            const auto& path = "../" + f.os();
            fs << "    <ClCompile Include=\""+path+"\">\n";
            fs << "      <Filter>src</Filter>\n";
            fs << "    </ClCompile>\n";
            ++i2;
          }
          fs << "  </ItemGroup>\n";
        }
        fs << "</Project>\n";
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
