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

#pragma once

//================================================|=============================
//Workspace:{                                     |

  namespace EON{

    namespace gfc{

      #define XCODE_PROJECT_SLOTS 18
      #define NINJA_PROJECT_SLOTS  8
      #define QMAKE_PROJECT_SLOTS 11
      #define  MSVC_PROJECT_SLOTS 10
      #define   NDK_PROJECT_SLOTS  8

      extern strings g_vIncludeStatements;

      //--------------------------------------------------------------------
      // All platforms and tools workspace/solution generator.
      //--------------------------------------------------------------------

      struct Workspace final:Object{

        e_reflect_no_properties( Workspace, Object );

        //----------------------------------------|-----------------------------
        //Equates:{                               |

          static constexpr const ccp kSourceSet = "src/main";

        //}:                                      |
        //Structs:{                               |

          //--------------------------------------------------------------------
          // File object.
          //--------------------------------------------------------------------

          struct File final:string{
            string abs()const{
              if( **this == '.' ){
                return os();
              }
              if( **this == '~' ){
                return os();
              }
              if( **this == '/' ){
                return os();
              }
              if( **this == '$' ){
                return os();
              }
              return "../" + *this;
            }
            File( const string& s )
              : string( s )
            {}
            File( const File& f )
              : string( f )
              , m_sFileRefID(f.m_sFileRefID )
              , m_sBuildID(  f.m_sBuildID   )
              , m_sEmbedID(  f.m_sEmbedID   )
              , m_bPublic(   f.m_bPublic    )
              , m_bStrip(    f.m_bStrip     )
              , m_bEmbed(    f.m_bEmbed     )
              , m_bSign(     f.m_bSign      )
            {}
            File() = default;
          ~ File() = default;

          private:

            e_var_string( FileRefID ) = string::streamId();
            e_var_string( BuildID   ) = string::streamId();
            e_var_string( EmbedID   ) = string::streamId();
            e_var_string( Where     );
            e_var_bool(   Public    ) = false;
            e_var_bool(   Strip     ) = true;
            e_var_bool(   Embed     ) = false;
            e_var_bool(   Sign      ) = false;
          };

          //--------------------------------------------------------------------
          // Project object.
          //--------------------------------------------------------------------

          template<const u32 N> struct Project:Object{

            e_reflect_no_properties( Project, Object );

            //------------------------------------|-----------------------------
            //Aliases:{                           |

              using Files = vector<File>;
              using Unity = vector<array<Files,N>>;

            //}:                                  |
            //------------------------------------|-----------------------------

          protected:

            /** \brief Combine files into unity files.
              *
              * This templatized member function will combine all the CPP, MM,
              * C and M files into a handful of unity files.
              *
              * \param eSourceIndex The file channel.
              */

            template<typename T, typename E> void unifyProject( const E eSourceIndex, u32& i )const{
              T& me = *(T*)this;
              auto it = me.inSources( eSourceIndex ).getIterator();
              while( it ){
                auto& f = *it;
                auto parts = toIgnoreParts();
                parts.erase( "\n" );
                parts.erase( "\t" );
                parts.erase( " " );
                const auto& splits = parts.splitAtCommas();
                auto ok = false;
                splits.foreach(
                  [&]( const string& split ){
                    if( isIgnoreFile( split, f )){
                      e_msgf( "  Ignoring %s", ccp( f ));
                    }else{
                      const auto ix=( i++ % m_vUnity.size() );
                      me.m_vUnity.alter( ix,
                        [&]( array<Files,N>& t ){
                          t[ eSourceIndex ].push( f );
                        }
                      );
                      ok = true;
                    }
                  }
                );
                if( ok ){
                  it.erase();
                  continue;
                }
                ++it;
              }
            }

            /** \brief Write project files out to unity objects.
              *
              * This routine will construct the #include"" statements for the
              * unity files.
              *
              * \param fs The writer object representing the unity file.
              *
              * \param eSourceIndex The file channel.
              *
              * \return Returns true if something was written otherwise false.
              */

            template<typename T,typename E> bool writeProject( fs::Writer& fs, const E eSourceIndex )const{
              T& me = *(T*)this;
              const auto disableUnity =
                  ( nullptr != toDisableOptions().tolower().find( "unity" ));
              if( disableUnity ){
                return true;
              }
              auto it = m_vUnity.getIterator();
              for( u32 i=0; it; ++i ){
                if( !(*it)[ eSourceIndex ].empty() ){
                  me.m_sSaveID = "tmp/"
                    + IEngine::sha1of( e_xfs( "%s%u%u", ccp( m_sLabel ), i, e_underlying( eSourceIndex )))
                    + me.extFromEnum( eSourceIndex );
                  me.inSources( eSourceIndex ).push( m_sSaveID );
                  fs::Writer tr_unit( m_sSaveID, fs::kTEXT );
                  tr_unit << "//------------------------------------------------------------------------------\n";
                  tr_unit << "//                  The best method for accelerating a computer\n";
                  tr_unit << "//                     is the one that boosts it by 9.8 m/s2.\n";
                  tr_unit << "//------------------------------------------------------------------------------\n";
                  tr_unit << "// GENERATED FILE DO NOT EDIT IN ANY WAY SHAPE OR FORM SOMETHING BAD WILL HAPPEN\n";
                  tr_unit << "//------------------------------------------------------------------------------\n";
                  u32 writeCount = 0;
                  (*it)[ eSourceIndex ].foreach(
                    [&]( const File& f ){
                      const auto& findUnity = me.toSkipUnity().tolower();
                      const auto& skipUnity = findUnity.splitAtCommas();
                      bool bSkip = false;
                      skipUnity.foreachs(
                        [&]( const string& skip ){
                          if( strstr( f, skip )){
                            bSkip = true;
                          }
                          return!bSkip;
                        }
                      );
                      if( bSkip ){
                        me.inSources( eSourceIndex ).push( f );
                      }else{
                        const auto& includeStatement = "#include\"../" + f + "\"\n";
                        g_vIncludeStatements.push( includeStatement );
                        me.inSources( eSourceIndex ).erase( f );
                        tr_unit.write( includeStatement );
                        ++writeCount;
                      }
                    }
                  );
                  if( writeCount ){
                    tr_unit.save();
                  }
                }
                ++it;
              }
              return true;
            }

          public:

            void reset(){
              m_vEmbedFiles.clear();
              m_vLibFiles.clear();
              m_sLinkWith.clear();
            }

          private:

            virtual bool sortingHat( const string& ){ return false; }

            e_var(        Files, v, PublicHeaders   );
            e_var(        Files, v, PublicRefs      );
            e_var(        Files, v, EmbedFiles      );
            e_var(        Files, v, LibFiles        );
            e_var_array(  Files,    Sources,N       );
            e_var_handle( Object,   Generator       );
            e_var_string(           SaveID          );
            e_var_string(           DisableOptions  );
            e_var_string(           InstallScript   );
            e_var_string(           IncludePaths    );
            e_var_string(           PrefixHeader    );
            e_var_string(           IgnoreParts     );
            e_var_string(           DefinesRel      ) = "NDEBUG, RELEASE";
            e_var_string(           DefinesDbg      ) = "_DEBUG, DEBUG";
            e_var_vector( string,   LinkWithTable   );
            e_var_string(           LinkWith        );
            e_var_string(           Language        ) = "c++20";
            e_var_string(           LanguageC       ) = "c11";
            e_var_string(           FindLibsPaths   );
            e_var_string(           LibraryPaths    );
            e_var_bool(             UnityBuild      ) = false;
            e_var_string(           SkipUnity       );
            e_var_string(           SrcPath         );
            e_var_string(           ResPath         );
            e_var_string(           Build           );
            e_var_string(           Label           );
            e_var1( v,              Unity           );
          };

          //--------------------------------------------------------------------
          // Apple Xcode project generator.
          //--------------------------------------------------------------------

          struct Xcode final:Project<XCODE_PROJECT_SLOTS>{

            e_reflect_no_properties( Xcode, Project<XCODE_PROJECT_SLOTS> );

            //------------------------------------|-----------------------------
            //Classes:{                           |

              static constexpr u32 kMax = 2;
              enum class Target:u32{
                  macOS
                , iOS
              };
              enum class Type:u32{
                  kStoryboard
                , kSharedlib
                , kStaticlib
                , kFramework
                , kXcasset
                , kBundle
                , kLproj
                , kPlist
                , kRtf
                , kPng
                , kHpp
                , kCpp
                , kMm
                , kInl
                , kH
                , kC
                , kM
                , kPrefab
                , kMax
              };

            //}:                                  |
            //Asserts:{                           |

              static_assert( e_underlying( Type::kMax )==XCODE_PROJECT_SLOTS );

            //}:                                  |
            //Methods:{                           |

              virtual bool sortingHat( const string& path )override;
              virtual void serialize( fs::Writer& )const override;
              void saveEntitlements( const string& path )const;
              ccp extFromEnum( const Type e )const;
              bool hasEntitlements()const;

            //}:                                  |
            //------------------------------------|-----------------------------

            virtual~Xcode() = default;
            Xcode();

          private:

            void writeFileReference( fs::Writer&
              , const Files& files
              , const string& _pt/* project type */)const;
            void writeFileReference( fs::Writer&
              , const string& refId
              , const string& path
              , const string& _id
              , const string& _pt )const;
            bool lookfor( File& )const;

            e_var_string( ProjectObject   ) = string::streamId();
            e_var_string( ReferencesGroup ) = string::streamId();
            e_var_string( ResourcesGroup  ) = string::streamId();
            e_var_string( CodeGroup       ) = string::streamId();
            e_var_string( FrameworkGroup  ) = string::streamId();
            e_var_string( ProductsGroup   ) = string::streamId();
            e_var_string( IncludeGroup    ) = string::streamId();
            e_var_string( EntFileRefID    ) = string::streamId();
            e_var_string( EntBuildID      ) = string::streamId();
            e_var_string( ResGroup        ) = string::streamId();
            e_var_string( SrcGroup        ) = string::streamId();
            e_var_string( MainGroup       ) = string::streamId();
            e_var_string( EmbedAndSign    );
            e_var_string( ProductBundleId );
            e_var_string( Deployment      ) = "12.0";
            e_var_string( TeamName        );
            e_var_string( FrameworkPaths  );
            e_var_string( PlistPath       );
            e_var_string( OrgName         );
            e_var_string( TargetOS        );

            void addToPBXShellScriptBuildPhaseSection( fs::Writer&
              , const std::function<void(
                const string& target
              , const string& shellScript )>& )const;
            void addToPBXNativeTargetSection( fs::Writer&
              , const std::function<void(
              // Target stuff
                const string& target
              , const string& label
              , const string& build
              , const string& frame
              // Phases
              , const string& phaseFramework
              , const string& phaseResource
              , const string& phaseHeaders
              , const string& phaseSources
              , const string& phaseScript
              // Embedding
              , const string& embedFrameworks
              , const string& embedPlugins
              // File refs
              , const string& productFileRef
              // Copy refs
              , const string& copyRefs )>& )const;
            void addToPBXFrameworksBuildPhaseSection( fs::Writer&
              , const std::function<void(
                const string& target
              , const string& frameworkBuildPhase )>& )const;
            void addToXCConfigurationListSection( fs::Writer&
              , const std::function<void(
                const string& target
              , const string& config
              , const string& build
              , const string& relNative
              , const string& dbgNative
              , const string& relBuild
              , const string& dbgBuild
              , const string& label )>& )const;
            void addToPBXGroupSection( fs::Writer&
                , const std::function<void(
                  const string& product
                , const string& target
                , const string& label )>& lambda )const;
            void addToPBXFileReferenceSection( fs::Writer&
              , const std::function<void(
                const string&
              , const string&
              , const string& )>& )const;
            void addToPBXSourcesBuildPhaseSection( fs::Writer&
              , const std::function<void(
                const string& )>& )const;
            void addToXCBuildConfigurationSection( fs::Writer&
              , const std::function<void(
                const string& target
              , const string& relNative
              , const string& dbgNative
              , const string& relBuild
              , const string& dbgBuild )>& lambda )const;

            e_var_bool( UniversalBinary      ) = false; // If true, then both x64 and apple silicons.
            e_var_bool( AppleSilicon         ) = false; // If false, x64 target, otherwise M1 target.
            e_var_bool( DisableLibValidation ) = false; // If false, x64 target, otherwise M1 target.
            e_var_bool( HardenedRuntime      ) = true;
            e_var_bool( NoEmbedAndSign       ) = false;
            e_var_bool( LoadAllSymbols       ) = false;
            e_var_bool( EnableARC            ) = true;
            e_var_bool( EnableJIT            ) = false;

            void writePBXShellScriptBuildPhaseSection( fs::Writer& )const;
            void writePBXFrameworksBuildPhaseSection(  fs::Writer& )const;
            void writePBXCopyFilesBuildPhaseSection(   fs::Writer& )const;
            void writePBXResourcesBuildPhaseSection(   fs::Writer& )const;
            void writePBXHeadersBuildPhaseSection(     fs::Writer& )const;
            void writePBXSourcesBuildPhaseSection(     fs::Writer& )const;
            void writeXCBuildConfigurationSection(     fs::Writer& )const;
            void writeXCConfigurationListSection(      fs::Writer& )const;
            void writePBXFileReferenceSection(         fs::Writer& )const;
            void writePBXNativeTargetSection(          fs::Writer& )const;
            void writePBXVariantGroupSection(          fs::Writer& )const;
            void writePBXBuildFileSection(             fs::Writer& )const;
            void writePBXProjectSection(               fs::Writer& )const;
            void writePBXGroupSection(                 fs::Writer& )const;
            void writeLibraries(                       fs::Writer& )const;

            // Note to self : if kMax changes update each of these vector initializers.
            e_var_array( string, ReleaseBuildConfiguration, kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, DebugBuildConfiguration,   kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, ReleaseNativeBuildConfig,  kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, DebugNativeBuildConfig,    kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, BuildConfigurationList,    kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, FrameNativeTarget,         kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, ResourceNativeTarget,      kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, BuildNativeTarget,         kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, ShellScriptBuildPhase,     kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, ResourcesBuildPhase,       kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, FrameworkBuildPhase,       kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, CopyRefsBuildPhase,        kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, HeadersBuildPhase,         kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, SourcesBuildPhase,         kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, VariantBuildPhase,         kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, ScriptBuildPhase,          kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, FrameworksEmbed,           kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, PluginsEmbed,              kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, ProductFileRef,            kMax ){ string::streamId(), string::streamId() };
            e_var_array( string, CopyRefs,                  kMax ){ string::streamId(), string::streamId() };
          };

          //--------------------------------------------------------------------
          // Qt project Qmake generator.
          //--------------------------------------------------------------------

          struct Qmake final:Project<QMAKE_PROJECT_SLOTS>{

            e_reflect_no_properties( Qmake, Project<QMAKE_PROJECT_SLOTS> );

            //------------------------------------|-----------------------------
            //Classes:{                           |

              enum class Type:u32{
                kStaticLib,
                kSharedLib,
                kHxx,
                kHpp,
                kCxx,
                kCpp,
                kInl,
                kCC,
                kHH,
                kH,
                kC,
                kMax
              };

              static_assert( e_underlying( Type::kMax )==QMAKE_PROJECT_SLOTS );

            //}:                                  |
            //Methods:{                           |

              virtual bool sortingHat( const string& path )override;
              virtual void serialize( fs::Writer& )const override;
              ccp extFromEnum( const Type e )const;

            //}:                                  |
            //------------------------------------|-----------------------------

            virtual~Qmake() = default;
            Qmake() = default;
          };

          //--------------------------------------------------------------------
          // Android NDK project generator.
          //--------------------------------------------------------------------

          struct NDK final:Project<NDK_PROJECT_SLOTS>{

            e_reflect_no_properties( NDK, Project<NDK_PROJECT_SLOTS> );

            //------------------------------------|-----------------------------
            //Classes:{                           |

              enum class Type:u32{
                kHpp,
                kCpp,
                kInl,
                kH,
                kC,
                kStaticlib,
                kPrefab,
                kEon,
                kMax
              };

              static_assert( e_underlying( Type::kMax )==NDK_PROJECT_SLOTS );

            //}:                                  |
            //Methods:{                           |

              virtual bool sortingHat( const string& path )override;
              virtual void serialize( fs::Writer& )const override;
              ccp extFromEnum( const Type e )const;

            //}:                                  |
            //------------------------------------|-----------------------------

            virtual~NDK() = default;
            NDK() = default;
          };

          //--------------------------------------------------------------------
          // Unix Ninja project generator.
          //--------------------------------------------------------------------

          struct Ninja final:Project<NINJA_PROJECT_SLOTS>{

            e_reflect_no_properties( Ninja, Project<NINJA_PROJECT_SLOTS> );

            //------------------------------------|-----------------------------
            //Classes:{                           |

              enum class Type:u32{
                kHpp,
                kCpp,
                kInl,
                kH,
                kC,
                kStaticlib,
                kPrefab,
                kEon,
                kMax
              };

              static_assert( e_underlying( Type::kMax )==NINJA_PROJECT_SLOTS );

            //}:                                  |
            //Methods:{                           |

              virtual bool sortingHat( const string& path )override;
              virtual void serialize( fs::Writer& )const override;
              ccp extFromEnum( const Type e )const;

            //}:                                  |
            //------------------------------------|-----------------------------

            virtual~Ninja() = default;
            Ninja() = default;
          };

          //--------------------------------------------------------------------
          // Microsoft Visual Studio project generator.
          //--------------------------------------------------------------------

          struct MSVC final:Project<MSVC_PROJECT_SLOTS>{

            e_reflect_no_properties( MSVC, Project<MSVC_PROJECT_SLOTS> );

            //------------------------------------|-----------------------------
            //Classes:{                           |

              enum class Type:u32{
                kLib,
                kPng,
                kHpp,
                kCpp,
                kInl,
                kH,
                kC,
                kPrefab,
                kEon,
                kDef,
                kMax
              };

              static_assert( e_underlying( Type::kMax )==MSVC_PROJECT_SLOTS );

            //}:                                  |
            //Methods:{                           |

              virtual void serialize( fs::Writer& )const override;
              virtual bool sortingHat( const string& )override;
              ccp extFromEnum( const Type e )const;
              ccp extFromBuildString()const;

            //}:                                  |
            //------------------------------------|-----------------------------

            virtual~MSVC() = default;
            MSVC() = default;

          private:

            void writeSetDirectory( fs::Writer&, const string&, const string&, const string& )const;
            void writePropGroup(    fs::Writer&, const string&, const string& )const;
            void writeImport(       fs::Writer&, const string&, const string& )const;
            void writeImportGroup(  fs::Writer&, const string&, const string& )const;
            void writeTargetVar(    fs::Writer&, const string&, const string& )const;
            void writeLinkerVar(    fs::Writer&, const string&, const string& )const;
            void writeManifestData( fs::Writer&, const string& )const;
            void writeImportGroup(  fs::Writer&, const string& )const;
            void writePropGroup(    fs::Writer&, const string& )const;
            void writeItemGroup(    fs::Writer&, const string& )const;
            void writeItemDefGroup( fs::Writer&, const string& )const;
            void writeProjVersion(  fs::Writer& )const;
            void writeFilter(       fs::Writer& )const;

            e_var_string( ProjectVersion  ) = "10.0.20506.1";
            e_var_string( PlatformTools   ) = "v142";
            e_var_string( PreferredArch   ) = "x64";
            e_var_string( Architecture    ) = "x64";
            e_var_string( ProjectGUID     ) = string::guid();
            e_var_string( WindowsSDK      ) = "10.0.22000.0";
            e_var_string( GenReleaseDBInf ) = "true";
            e_var_string( ExceptionHndlng ) = "Sync";
            e_var_string( DebugInfoFormat ) = "ProgramDatabase";
            e_var_string( BasicRuntimeChk ) = "Default";
            e_var_string( LinkLibDepends  ) = "true";
            e_var_string( WarningLevel    ) = "Level3";
            e_var_string( RTTI            ) = "true";
            e_var_string( PCH             ) = "NotUsing";
            e_var_string( IntDir          ) = ".intermediate";
            e_var_string( OutDir          ) = ".output";
            e_var_string( LinkIncremental ) = "true";
            e_var_string( GenManifest     ) = "true";
            e_var_string( Dependencies    );
            e_var_string( Definition      );
          };

        //}:                                      |
        //Aliases:{                               |

          using Target = Object;

        //}:                                      |
        //Methods:{                               |

          virtual void serialize( fs::Writer& )const override;
          static bool isUnityBuild();
          static bool isIgnoreFile(
              const string& regex
            , const string& );
          void cleanup()const;

          template<const u32 N> static bool isProject( const string& lbl ){
            bool r = false;
            Class::foreachs<Project<N>>(
              [&]( const Project<N>& project ){
                if( project.isLabel( lbl )){
                  r = true;
                }
                return !r;
              }
            );
            return r;
          }

          void again();
          
        //}:                                      |
        //----------------------------------------|-----------------------------

        Workspace()
          : m_tStates( bmp )
        {}

      private:

        /* Private methods */

        void serializeSln2022( fs::Writer& )const;
        void serializeSln2019( fs::Writer& )const;
        void serializeXcode(   fs::Writer& )const;
        void serializeNinja(   fs::Writer& )const;
        void serializeQmake(   fs::Writer& )const;
        void serializeGradle(  fs::Writer& )const;

        /* Program facing */

        e_var_string(         Name   );
        e_var_handle_vector1( Target );
        e_var_bits(           States
          , bEmscripten:1
          , bMaxPlugin:1
          , bGenerate:1
          , bXcode11:1
          , bXcode12:1
          , bXcode14:1
          , bVSTools143:1
          , uLanguage:8
          , bVS2019:1
          , bVS2022:1
          , bVSMTNoDLL:1
          , bGradle:1
          , bNinja:1
          , bCmake:1
          , bQmake:1
          , bUnity:1
          , bNDK:1
          // Special platform hints.
          , osIphone:1
          , osMac:1
          // Unicode.
          , bUTF16:1
        );

      public:

        static strings getTargets();
        static string gen; //!< Generation identifier.
        static string ext; //!< Plugin extension.
        static States bmp; //!< Global flags.
      };
    }
  }

//}:                                              |
//================================================|=============================
