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
//Private:{                                       |
  //isUnityBuild:{                                |

    namespace{
      bool anon_isUnityBuild(){
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

  //}:                                            |
//}:                                              |
//Private:{                                       |
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

      ccp Workspace::MSVC::extFromEnum( const Type e )const{
        switch( e ){
          case decltype( e )::kCpp:
            return ".cpp";
          case decltype( e )::kC:
            return ".c";
          default:
            return "";
        }
      }

    //}:                                          |
    //sortingHat:{                                |

      void Workspace::MSVC::sortingHat( const string& in_path ){

        //----------------------------------------------------------------------
        // Solutions need Windows GUIDs. If we're running on Windows then you
        // are guaranteed to have a GUID that's universally unique. If running
        // on the Mac or Linux, you're only guaranteed to have file uniqueness.
        //----------------------------------------------------------------------

        File path = in_path;
        path.setBuildID( string::guid() );
        path.setRefID(   string::guid() );
        const auto& ext = path.ext().tolower();
        switch( ext.hash() ){

          //--------------------------------------------------------------------
          // Platform specific file types.
          //--------------------------------------------------------------------

          case e_hashstr64_const( ".lib" ):
            inSources( Type::kLib ).push( path );
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
    //writePropertyGroup:{                        |

      void Workspace::MSVC::writePropGroup( Writer& fs, const string& group, const string& config )const{
        switch( group.hash() ){
          case e_hashstr64_const( "Condition" ):
            fs << "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\" Label=\"Configuration\">\n";
            switch( toBuild().hash() ){
              case e_hashstr64_const( "application" ):
                fs << "  <ConfigurationType>Application</ConfigurationType>\n";
                break;
              case e_hashstr64_const( "console" ):
                fs << "  <ConfigurationType>Console</ConfigurationType>\n";
                break;
            }
            fs << "  <CharacterSet>"+toUnicodeType()+"</CharacterSet>\n";
            fs << "  <PlatformToolset>"+toPlatformTools()+"</PlatformToolset>\n";
            fs << "</PropertyGroup>\n";
            break;
        }
      }

      void Workspace::MSVC::writePropGroup( Writer& fs, const string& group )const{
        if( e_hashstr64_const( "<arch>" ) != group.hash() ){
          fs << "<PropertyGroup Label=\""+group+"\"/>\n";
        }else{
          fs << "<PropertyGroup>\n";
        }
        switch( group.hash() ){
          case e_hashstr64_const( "UserMacros" ):
            // TODO: Put in user macros here.
            break;
          case e_hashstr64_const( "Globals" ):
            fs << "  <ProjectGuid>"+toProjectGUID()+"</ProjectGuid>\n";
            fs << "  <WindowsTargetPlatformVersion>"+toWindowsSDK()+"</WindowsTargetPlatformVersion>\n";
            fs << "  <Keyword>Win32Proj</Keyword>\n";
            fs << "  <Platform>"+m_sArchitecture+"</Platform>\n";
            fs << "  <ProjectName>"+toLabel()+"</ProjectName>\n";
            fs << "  <VCProjectUpgraderObjectName>NoUpgrade</VCProjectUpgraderObjectName>\n";
            break;
          // Anything in <> is a special case and not sent through to the vcxproj.
          case e_hashstr64_const( "<arch>" ):
            fs << "  <PreferredToolArchitecture>"+toPreferredArch()+"</PreferredToolArchitecture>\n";
            break;
        }
        if( e_hashstr64_const( "<arch>" ) != group.hash() ){
          fs << "</PropertyGroup>\n";
        }
      }

    //}:                                          |
    //writeItemGroup:{                            |

      void Workspace::MSVC::writeItemGroup( Writer& fs, const string& group )const{
        switch( group.hash() ){
          case e_hashstr64_const( "ProjectConfigurations" ):
            fs << "<ItemGroup Label=\"ProjectConfigurations\">\n";
            fs << "  <ProjectConfiguration Include=\"Debug|"+m_sArchitecture+"\">\n";
            fs << "    <Configuration>Debug</Configuration>\n";
            fs << "    <Platform>"+m_sArchitecture+"</Platform>\n";
            fs << "  </ProjectConfiguration>\n";
            fs << "  <ProjectConfiguration Include=\"Release|"+m_sArchitecture+"\">\n";
            fs << "    <Configuration>Release</Configuration>\n";
            fs << "    <Platform>"+m_sArchitecture+"</Platform>\n";
            fs << "  </ProjectConfiguration>\n";
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
        fs << "<"+tag+" Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">$(SolutionDir)"+path+"\\"+config+"\\</"+tag+">\n";
      }

    //}:                                          |
    //extFromBuildString:{                        |

      ccp Workspace::MSVC::extFromBuildString()const{
        switch( toBuild().hash() ){
          case e_hashstr64_const( "application" ):
            [[fallthrough]];
          case e_hashstr64_const( "console" ):
            return "exe";
          case e_hashstr64_const( "static" ):
            return "lib";
          case e_hashstr64_const( "shared" ):
            return "dll";
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
          case e_hashstr64_const( "Name" ):
            fs << "<TargetName Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">"+toLabel()+"</TargetName>\n";
            break;
          case e_hashstr64_const( "Ext" ):
            fs << "<TargetExt Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">."+extFromBuildString()+"</TargetExt>\n";
            break;
        }
      }

    //}:                                          |
    //writeLinkerVar:{                            |

      void Workspace::MSVC::writeLinkerVar( Writer& fs, const string& label, const string& config )const{
        switch( label.hash() ){
          case e_hashstr64_const( "Incremental" ):
            fs << "<LinkIncremental Condition=\"'$(Configuration)|$(Platform)'=='"+config+"|"+m_sArchitecture+"'\">"+m_sLinkIncremental+"</LinkIncremental>\n";
            break;
        }
      }

    //}:                                          |
    //writeImport*:{                              |

      void Workspace::MSVC::writeImportGroup( Writer& fs, const string& label, const string& path )const{
        fs << "<ImportGroup Label=\""+label+"\">\n";
        switch( label.hash() ){
          case e_hashstr64_const( "PropertySheeets" ):
            fs << "<Import Project=\"$(UserRootDir)\\"+path+"\" Condition=\"exists('$(UserRootDir)\\"+path+"')\" Label=\"LocalAppDataPlatform\"/>\n";
            break;
        }
        fs << "</ImportGroup>\n";
      }

      void Workspace::MSVC::writeImportGroup( Writer& fs, const string& label )const{
        fs << "<ImportGroup Label=\""+label+"\">\n";
        switch( label.hash() ){
          case e_hashstr64_const( "ExtensionSettings" ):
            // TODO: Add extension settings for this vcxproj.
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
        fs << "  <ClCompile>\n";
        fs << "    <AdditionalIncludeDirectories>";
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
              }else{
                paths.push( "../" + syspath );
              }
            }
          );
          auto it = paths.getIterator();
          while( it ){
            const auto& s = *it++;
            fs << s + ";";
          }
        }
        fs << "%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n";
        fs << "    <AssemblerListingLocation>$(IntDir)</AssemblerListingLocation>\n";
        fs << "    <BasicRuntimeChecks>EnableFastChecks</BasicRuntimeChecks>\n";
        fs << "    <CompileAs>CompileAsCpp</CompileAs>\n";
        fs << "    <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\n";
        fs << "    <ExceptionHandling>Sync</ExceptionHandling>\n";
        fs << "    <ForcedIncludeFiles>"+toPrefixHeader()+"</ForcedIncludeFiles>\n";
        fs << "    <InlineFunctionExpansion>";
        switch( config.hash() ){
          case e_hashstr64_const( "Debug" ):
            fs << "    <InlineFunctionExpansion>Disabled</InlineFunctionExpansion>\n";
            break;
          case e_hashstr64_const( "Release" ):
            // TODO: Any suitable instead of Disabled.
            fs << "    <InlineFunctionExpansion>Disabled</InlineFunctionExpansion>\n";
            break;
        }
        fs << "</InlineFunctionExpansion>\n";
        fs << "    <LanguageStandard>";
        switch( toLanguage().hash() ){
          case e_hashstr64_const( "c++17" ):
            fs << "stdcpp17";
            break;
        }
        fs << "</LanguageStandard>\n";
        fs << "    <Optimization>";
        switch( config.hash() ){
          case e_hashstr64_const( "Debug" ):
            fs << "Disabled";
            break;
          case e_hashstr64_const( "Release" ):
            // TODO: Heavy optimization here.
            fs << "Disabled";
            break;
        }
        fs << "</Optimization>\n";
        fs << "    <PrecompiledHeader>NotUsing</PrecompiledHeader>\n";
        switch( config.hash() ){
          case e_hashstr64_const( "Debug" ):
            fs << "    <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>\n";
            break;
          case e_hashstr64_const( "Release" ):
            fs << "    <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>\n";
            break;
        }
        fs << "    <RuntimeTypeInfo>true</RuntimeTypeInfo>\n";
        fs << "    <UseFullPaths>false</UseFullPaths>\n";
        fs << "    <WarningLevel>Level3</WarningLevel>\n";
        fs << "    <PreprocessorDefinitions>";
        string defs = toDefinesDbg() + "," + toLabel().toupper();
        defs.replace( ",", ";" );
        defs.replace( " ", "" );
        switch( config.hash() ){
          case e_hashstr64_const( "Debug" ):
            fs << toDefinesDbg();
            break;
          case e_hashstr64_const( "Release" ):
            fs << toDefinesRel();
            break;
        }
        fs << "    %(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
        fs << "    <ObjectFileName>$(IntDir)</ObjectFileName>\n";
        fs << "  </ClCompile>\n";
//      fs << "  <ResourceCompile>\n";
//      fs << "    <PreprocessorDefinitions>WIN32;_DEBUG;_WINDOWS;DEBUG;cog;LUA_FLOAT_TYPE=1;CMAKE_INTDIR=\"Debug\";%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
//      fs << "    <AdditionalIncludeDirectories>C:\H7\src\cog\usr\share\boost\1.71.0;C:\H7\src\cog\usr\share;C:\H7\src\cog\src\engine\include;C:\H7\src\cog\src\lua\5.3.5;C:\H7\src\cog\src\app\include;C:\H7\src\cog\etc;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n";
//      fs << "  </ResourceCompile>\n";
//      fs << "  <Midl>\n";
//      fs << "    <AdditionalIncludeDirectories>C:\H7\src\cog\usr\share\boost\1.71.0;C:\H7\src\cog\usr\share;C:\H7\src\cog\src\engine\include;C:\H7\src\cog\src\lua\5.3.5;C:\H7\src\cog\src\app\include;C:\H7\src\cog\etc;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n";
//      fs << "    <OutputDirectory>$(ProjectDir)/$(IntDir)</OutputDirectory>\n";
//      fs << "    <HeaderFileName>%(Filename).h</HeaderFileName>\n";
//      fs << "    <TypeLibraryName>%(Filename).tlb</TypeLibraryName>\n";
//      fs << "    <InterfaceIdentifierFileName>%(Filename)_i.c</InterfaceIdentifierFileName>\n";
//      fs << "    <ProxyFileName>%(Filename)_p.c</ProxyFileName>\n";
//      fs << "  </Midl>\n";
        fs << "  <Link>\n";
        fs << "    <AdditionalDependencies>kernel32.lib;user32.lib;gdi32.lib;winspool.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;comdlg32.lib;advapi32.lib</AdditionalDependencies>\n";
        fs << "    <AdditionalLibraryDirectories>%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\n";
        fs << "    <AdditionalOptions>%(AdditionalOptions) /machine:"+m_sArchitecture+"</AdditionalOptions>\n";
        fs << "    <GenerateDebugInformation>true</GenerateDebugInformation>\n";
        fs << "    <IgnoreSpecificDefaultLibraries>%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>\n";
//      fs << "    <ImportLibrary>C:/H7/src/cog/etc/src/app/Debug/cog.lib</ImportLibrary>\n";
        fs << "    <ProgramDataBaseFile>$(IntDir)"+toLabel()+".pdb</ProgramDataBaseFile>\n";
        switch( toBuild().hash() ){
          case e_hashstr64_const( "application" ):
            fs << "    <SubSystem>Windows</SubSystem>\n";
            break;
          case e_hashstr64_const( "console" ):
            fs << "    <SubSystem>Console</SubSystem>\n";
            break;
        }
        fs << "  </Link>\n";
        fs << "  <ProjectReference>\n";
        fs << "    <LinkLibraryDependencies>false</LinkLibraryDependencies>\n";
        fs << "  </ProjectReference>\n";
        fs << "</ItemDefinitionGroup>\n";
      }

    //}:                                          |
    //serialize:{                                 |

      void Workspace::MSVC::serialize( Writer& fs )const{
        static constexpr ccp aConfigs[2]{ "Debug", "Release" };

        //----------------------------------------------------------------------
        // Populate build files across unity space.
        //----------------------------------------------------------------------

        if( !anon_isUnityBuild() ){
          const_cast<MSVC*>( this )->toUnity().resize( 1 );
        }else{
          u32 cores = 4;//std::thread::hardware_concurrency();
          u32 i=0;
          const_cast<MSVC*>( this )->toUnity().resize( cores );
          const_cast<MSVC*>( this )->unifyProject<MSVC>( fs, Type::kCpp, i );
          const_cast<MSVC*>( this )->unifyProject<MSVC>( fs, Type::kC,   i );
          writeProject<MSVC>( fs, Type::kCpp );
          writeProject<MSVC>( fs, Type::kC );
        }

        //----------------------------------------------------------------------
        // Save MSVC project to XML format file.
        //----------------------------------------------------------------------

        fs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
        fs << "<Project DefaultTargets=\"Build\" ToolsVersion=\"16.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
        writePropGroup(    fs, "<arch>" );
        writeItemGroup(    fs, "ProjectConfigurations" );
        writePropGroup(    fs, "Globals" );
        writeImport(       fs, "Project",   "Microsoft.Cpp.Default.props" );
        for( u32 n=e_dimof( aConfigs ), i=0; i<n; ++i ){
          writePropGroup(  fs, "Condition", aConfigs[ i ]);
        }
        writeImport(       fs, "Project", "Microsoft.Cpp.props" );
        writeImportGroup(  fs, "ExtensionSettings" );
        writeImportGroup(  fs, "PropertySheets", "Microsoft.Cpp.$(Platform).user.props" );
        writePropGroup(    fs, "UserMacros" );
        fs << "<PropertyGroup>\n";
          writeProjVersion(  fs );
          for( u32 n=e_dimof( aConfigs ), i=0; i<n; ++i ){
            writeSetDirectory( fs, "OutDir",      aConfigs[ i ], m_sOutDir );
            writeSetDirectory( fs, "IntDir",      aConfigs[ i ], m_sIntDir );
            writeTargetVar(    fs, "Name",        aConfigs[ i ]);
            writeTargetVar(    fs, "Ext",         aConfigs[ i ]);
            writeLinkerVar(    fs, "Incremental", aConfigs[ i ]);
            writeManifestData( fs, "Debug" );
          }
        fs << "</PropertyGroup>\n";
        for( u32 n=e_dimof( aConfigs ), i=0; i<n; ++i ){
          writeItemDefGroup( fs, aConfigs[ i ]);
        }
         // TODO: Scan srcdir and incdir.
        fs << "<Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.targets\"/>\n";
        fs << "  <ImportGroup Label=\"ExtensionTargets\">\n";
        fs << "</ImportGroup>\n";
        fs << "</Project>\n";
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
