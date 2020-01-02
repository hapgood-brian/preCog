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

      struct Workspace final:Object{

        e_reflect_no_properties( Workspace, Object );

        //----------------------------------------|-----------------------------
        //Structs:{                               |

          struct Project:Object{

            e_reflect_no_properties( Project, Object );

            //------------------------------------|-----------------------------
            //Structs:{                           |

              struct File final:string{
                File( const string& s )
                  : string( s )
                {}
                File( string&& s )
                  : string( std::move( s ))
                {}
                File() = default;
              ~ File() = default;

              private:

                e_var_string( BuildID ) = string::resourceId();
                e_var_string( RefID   ) = string::resourceId();
                e_var_bool(   Public  ) = false;
              };

            //}:                                  |
            //Aliases:{                           |

              using Files = vector<File>;

            //}:                                  |
            //Classes:{                           |

              enum class Source:u32{
                kNone,
                kStoryboard,
                kSharedlib,
                kStaticlib,
                kFramework,
                kXcasset,
                kLproj,
                kPlist,
                kRtf,
                kPng,
                kHpp,
                kCpp,
                kMm,
                kInl,
                kH,
                kC,
                kM,
                kPrefab,
                kMax
              };

            //}:                                  |
            //------------------------------------|-----------------------------

          private:

            e_var_array(  Files,    Sources, Source::kMax );
            e_var(        Files, v, PublicHeaders         );
            e_var(        Files, v, EmbedFiles            );
            e_var(        Files, v, LibFiles              );
            e_var_handle( Object,   Generator             );
            e_var_string(           DisableOptions        );
            e_var_string(           InstallScript         );
            e_var_string(           IncludePaths          );
            e_var_string(           PrefixHeader          );
            e_var_string(           IgnoreParts           );
            e_var_string(           Deployment            ) = "10.15";
            e_var_string(           DefinesRel            ) = "NDEBUG, RELEASE";
            e_var_string(           DefinesDbg            ) = "_DEBUG, DEBUG";
            e_var_string(           SkipUnity             );
            e_var_string(           LinkWith              );
            e_var_string(           TeamName              );
            e_var_string(           Language              ) = "c++17";
            e_var_string(           FrameworkPaths        );
            e_var_string(           LibraryPaths          );
            e_var_string(           IncPath               );
            e_var_string(           SrcPath               );
            e_var_string(           PlistPath             );
            e_var_string(           ResPath               );
            e_var_string(           OrgName               );
            e_var_string(           TypeID                );
            e_var_string(           Build                 );
            e_var_string(           Label                 );
            e_var_bool(             HardenedRuntime       ) = false;
          };

          struct Xcode final:Project{

            e_reflect_no_properties( Xcode, Project );

            //------------------------------------|-----------------------------
            //Aliases:{                           |

              using Unity = array<array<Files,5>,4>;

            //}:                                  |
            //Methods:{                           |

              virtual void serialize( fs::Writer& )const override;

            //}:                                  |
            //------------------------------------|-----------------------------

          private:

            e_var_string( ReleaseBuildConfiguration ) = string::resourceId();
            e_var_string( ReleaseNativeBuildConfig  ) = string::resourceId();
            e_var_string( DebugBuildConfiguration   ) = string::resourceId();
            e_var_string( DebugNativeBuildConfig    ) = string::resourceId();
            e_var_string( BuildConfigurationList    ) = string::resourceId();
            e_var_string( ShellScriptBuildPhase     ) = string::resourceId();
            e_var_string( BuildNativeTarget         ) = string::resourceId();
            e_var_string( EmbedFrameworks           ) = string::resourceId();
            e_var_string( FrameworkNativeTarget     ) = string::resourceId();
            e_var_string( PublicHeadersBuildPhase   ) = string::resourceId();
            e_var_string( ResourcesBuildPhase       ) = string::resourceId();
            e_var_string( FrameworkBuildPhase       ) = string::resourceId();
            e_var_string( HeadersBuildPhase         ) = string::resourceId();
            e_var_string( SourcesBuildPhase         ) = string::resourceId();
            e_var_string( VariantBuildPhase         ) = string::resourceId();
            e_var_string( ProductFileRef            ) = string::resourceId();
            e_var_string( ProjectObject             ) = string::resourceId();
            e_var_string( ReferencesGroup           ) = string::resourceId();
            e_var_string( ResourcesGroup            ) = string::resourceId();
            e_var_string( CodeGroup                 ) = string::resourceId();
            e_var_string( FrameworkGroup            ) = string::resourceId();
            e_var_string( ProductsGroup             ) = string::resourceId();
            e_var_string( IncludeGroup              ) = string::resourceId();
            e_var_string( ResGroup                  ) = string::resourceId();
            e_var_string( SrcGroup                  ) = string::resourceId();
            e_var_string( MainGroup                 ) = string::resourceId();
            e_var_string( ProductBundleId           );
            e_var1(    a, Unity                     );

            void writePBXBuildFileSection(             fs::Writer& )const;
            void writePBXCopyFilesBuildPhaseSection(   fs::Writer& )const;
            void writePBXFileReferenceSection(         fs::Writer& )const;
            void writePBXFrameworksBuildPhaseSection(  fs::Writer& )const;
            void writePBXGroupSection(                 fs::Writer& )const;
            void writePBXNativeTargetSection(          fs::Writer& )const;
            void writePBXProjectSection(               fs::Writer& )const;
            void writePBXResourcesBuildPhaseSection(   fs::Writer& )const;
            void writePBXShellScriptBuildPhaseSection( fs::Writer& )const;
            void writePBXHeadersBuildPhaseSection(     fs::Writer& )const;
            void writePBXSourcesBuildPhaseSection(     fs::Writer& )const;
            void writePBXVariantGroupSection(          fs::Writer& )const;
            void writeXCBuildConfigurationSection(     fs::Writer& )const;
            void writeXCConfigurationListSection(      fs::Writer& )const;
          };

          struct MSVC final:Project{

            e_reflect_no_properties( MSVC, Project );

            //------------------------------------|-----------------------------
            //Methods:{                           |

              virtual void serialize( fs::Writer& )const override;

            //}:                                  |
            //------------------------------------|-----------------------------
          };

        //}:                                      |
        //Methods:{                               |

          virtual void serialize( fs::Writer& )const override;

        //}:                                      |
        //----------------------------------------|-----------------------------

      private:

        e_var_handle_vector1( Project );
        e_var_string(         TypeID  );
        e_var_string(         Name    );
      };
    }
  }

//}:                                              |
//================================================|=============================
