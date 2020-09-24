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

/** \addtogroup engine
  * @{
  *   \addtogroup startup
  *   @{
  */

//================================================|=============================
//Platform:{                                      |

  namespace EON{

  /** \brief Platform identifiers.
    *
    * Every platform the engine supports must be enumerated here. The
    * templatized as<> function is specialized in each platform shim.
    \code
      "i386"      on the simulator
      "iPod1,1"   on iPod Touch
      "iPod2,1"   on iPod Touch Second Generation
      "iPod3,1"   on iPod Touch Third Generation
      "iPod4,1"   on iPod Touch Fourth Generation
      "iPhone1,1" on iPhone
      "iPhone1,2" on iPhone 3G
      "iPhone2,1" on iPhone 3GS
      "iPad1,1"   on iPad
      "iPad2,1"   on iPad 2
      "iPad3,1"   on 3rd Generation iPad
      "iPhone3,1" on iPhone 4
      "iPhone4,1" on iPhone 4S
      "iPhone5,1" on iPhone 5 (model A1428, AT&T/Canada)
      "iPhone5,2" on iPhone 5 (model A1429, everything else)
      "iPad3,4"   on 4th Generation iPad
      "iPad2,5"   on iPad Mini
      "iPhone5,3" on iPhone 5c (model A1456, A1532 | GSM)
      "iPhone5,4" on iPhone 5c (model A1507, A1516, A1526 (China), A1529 | Global)
      "iPhone6,1" on iPhone 5s (model A1433, A1533 | GSM)
      "iPhone6,2" on iPhone 5s (model A1457, A1518, A1528 (China), A1530 | Global)
      "iPad4,1"   on 5th Generation iPad (iPad Air) - Wifi
      "iPad4,2"   on 5th Generation iPad (iPad Air) - Cellular
      "iPad4,4"   on 2nd Generation iPad Mini - Wifi
      "iPad4,5"   on 2nd Generation iPad Mini - Cellular
    \endcode
    */

    enum class Platform:u32{

      Android,      //!< Gingerbread and above.

      iPod,         //!< iPod touch
      iPod2,        //!< 2nd generation iPod
      iPod3,        //!< 3rd generation iPod
      iPod4,        //!< 4th generation iPod

      iPhone,       //!< iPhone
      iPhone3G,     //!< iPhone 3G
      iPhone3GS,    //!< iPhone 3GS
      iPhone4,      //!< iPhone 4
      iPhone4S,     //!< iPhone 4S
      iPhone5,      //!< iPhone 5
      iPhone5C,     //!< iPhone 5
      iPhone5S,     //!< iPhone 5

      iPad,         //!< Original iPad
      iPad2,        //!< iPad2 (2nd generation)
      iPad3,        //!< New iPad
      iPadMini,     //!< iPad Mini
      iPadAir,      //!< iPad Air (5th generation)
      iPadMini2,    //!< iPad Mini (2nd generation)

      OSXElCapitan,
      OSXYosemite,
      OSXMavericks,
      OSXMountainLion,

      Win10
    };
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Cursors:{                                       |

  namespace EON{

    /** \brief Cursors types.
      *
      * This enum class defines all the stock cursors supported by the engine.
      * They are based on the OSX standard cursors but more will almost
      * certainly be added over time. The same enums will work for Windows,
      * Linux and OSX; where they're not native cursor streams will be
      * provided.
      *
      * \todo Add custom cursor enum. Then add an API to IEngine that let's you
      * query the maximum size of the hardware cursor and set the cursor pixels
      * from a Texture::handle. Ideally we'd have a Bitmap class (not Image)
      * that is shared with the Texture for transporting texels.
      */

    enum class Cursors:u32{
      kArrow,               //!< Standard pointer.
      kIBeam,               //!< IBeam cursor (should be used on all edit boxes).
      kCrossHair,           //!< Cross hair cursor.
      kClosedHand,          //!< Closed hand cursor (should be used when dragging in 3D views).
      kOpenHand,            //!< Open hand cursor (should be used in 3D views).
      kPointingHand,        //!< Pointing hand cursor.
      kResizeLeft,          //!< Resize left cursor.
      kResizeRight,         //!< Resize right cursor.
      kResizeLeftAndRight,  //!< Reize horizontally cursor.
      kResizeUp,            //!< The resize up cursor.
      kResizeDown,          //!< The resize down cursor.
      kResizeUpAndDown,     //!< The resize vertically cursor.
      kDisappearingItem,    //!< The disappearing item.
      kIBeamVertical,       //!< The vertical IBeam cursor (should be used on all vertical edit boxes).
      kNotAllowed,          //!< Not allowed cursor.
      kDragLink,            //!< Drag link cursor.
      kDragCopy,            //!< Drag copy cursor.
      kContextMenu,         //!< Context menu (should be used on menus widgets).
    };
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//IEngine:{                                       |

  namespace EON{

    /** \brief Core engine class.
      *
      * This interface is the first line of defense against the user :) In other
      * words this is the first class any programmer will deal with as it shims
      * away all the details of graphics SDKs like OpenGL and DirectX, physics
      * and sound while providing those services on as many platforms as it
      * possibly can.
      */

    struct E_PUBLISH IEngine final{

      #define ENGINE_VERSION u16(1)

      //------------------------------------------|-----------------------------
      //Structs:{                                 |
        //CVars:{                                 |

          struct E_PUBLISH Cvar final{

            /** \brief Add cvar.
              *
              * This routine will add a variable to the cvar system.
              */

            template<typename T> static bool set( const gfc::string& key, const T& value );
            template<typename T> static T    get( const gfc::string& key );
          };

          #define e_setCvar(key,value)                                          \
            IEngine::Cvar::set( key, value )                                    \

          #define e_getCvar(type,key)                                           \
            IEngine::Cvar::get<type>( key )                                     \

        //}:                                      |
      //}:                                        |
      //Globals:{                                 |

        /** \brief Engine CPU delta time.
          *
          * This real number contains the delta time passed in to onTick().
          */

        static f64 cpuDT;

        /** \brief Engine GPU delta time.
          *
          * This real number contains the delta time for the GPU.
          */

        static f64 gpuDT;

        /** \brief Engine prefabs.
          *
          * This vector contains all the prefabs that the application pass-thru
          * mechanism works from.
          */

        static gfc::vector<gfc::Prefab::handle> prefabs;

        /** \brief Environment vars.
          *
          * This vector contains all the environment variables as passed into
          * main. Not filled on all platforms.
          */

        static gfc::string_pairs envs;

        /** \brief Engine args.
          *
          * This vector contains the arguments as passed to IEngine::main on
          * startup. Query this member at any time to know the name of the
          * application (args[0]) and all the command line arguments.
          */

        static gfc::strings args;

      //}:                                        |
      //Methods:{                                 |
        //[platform]:{                            |

          /** @}
            *
            * \name Engine and platform queries.
            *
            * This section contains all the methods to do with querying the
            * hardware such as asking if we're on the main thread, if we're
            * running on a particular platform, etc.
            *
            * @{
            */

          /** \brief Hardware querying function.
            *
            * This routine is a fun way of querying whether the engine IS
            * sitting on top of the given platform. The function is specialized
            * in the platform layer to implement the various enumerated types
            * supported by it.
            \code
              #include<eon.h>

              using namespace EON;

              int IEngine::main(){
                if( is<Platform::OSXElCapitan>() ){
                  return run( "Game", 1024, 768 );
                }
                return 0;
              }
            \endcode
            *
            * \return Returns true if the platform is current and false
            * otherwise.
            */

          template<Platform> static bool is(){
            return false;
          }

          /** \brief Execute lambda on main thread.
            *
            * This routine will execute a lambda function on the main thread no
            * matter what thread you're calling from.
            */

          static void runOnMainThread( const std::function< void() >& lambda );
          #define e_runOnMainThread( lambda ) ::EON::IEngine::runOnMainThread( lambda )

          /** \brief Is main thread.
            *
            * This routine will return true if the current thread is the main
            * one.  This is the same thread as the GL context was created on. A
            * special note on Android: the engine considers the thread that
            * called android_main() to be the main thread not the main thread
            * of the activity, which is different. What isMainThread truly
            * means is "is the current thread the one I created the GL context
            * in?"
            *
            * \return Returns true if the current thread is the main one.
            */

          static bool isMainThread();

        //}:                                      |
        //[bundles]:{                             |

          /** @}
            *
            * \name Engine path helpers.
            *
            * The following APIs exist solely for the purpose of finding data
            * when you're running from a tool's app or a game. The bundle path
            * mimics the OSX application bundle on all platforms and will
            * return a path to the directory where the executable can be found
            * or in the case of OSX inside the .app folder. On Windows it'll be
            * somewhere in Program Files. The executable will be packaged in a
            * similar manner to OSX on all platforms, even Linux, so this API
            * will work without change across the board. So you're guaranteed
            * to be able to say the following everywhere:
            * @{
            */

          /** \brief Get path to application bundle.
            *
            * This routine will return a ccp type (const char*) to a file path.
            * This path works on all platforms but most notably on ios and osx.
            * It is expected that all .eon files will be found at the bundle
            * path. The bundle path is the directory for the bundle on iOS and
            * OSX. On Windows it is the directory containing the EXE. On
            * Android it is the asset directory or in other words the string is
            * empty because all file operations on that platform are relative
            * to the asset directory.
            *
            * Even on Windows there's the concept of a bundle, but not just in
            * the same way as the Mac just simply inspired by it.
            *
            * \return Return the bundle path.
            */

          static gfc::string toBundlePath();

          /** \brief Get path to stream bundle.
            *
            * This routine returns a string object containing a path to the
            * stream directory in the application bundle on iOS and OSX. On
            * Windows the directory structure of the game will match OSX and
            * iOS and toStreamPath() will return the stream directory. A
            * special note must be made that file operations that fail in the
            * stream path will pass through to the RESOURCE SECTION of the
            * executable on Win32 and Win64. For Android toStreamPath() calls
            * are identical to toBundlePath() ones because both paths are in
            * the asset directory. On consoles toStreamPath() will be a path
            * to the stream directory on that platform.
            *
            * \return Returns a gfc::string containing the path.
            */

          static gfc::string toStreamPath();

          /** \brief Set the fall back directory.
            *
            * The fall back directory exists for non-mobile/console
            * applications that need to find data outside of a bundle or
            * relative to an executable. This applies equally to Windows which
            * simulates the bundle concept. All paths regardless of platform
            * are in Unix form.
            */

          static void setPackagePath( const gfc::string& dir );

          /** \brief Get the fall back directory.
            *
            * This routine will return the current fall back directory. This is
            * used by the editor, for example, to load textures and entities to
            * be rendered in the object preview screen.
            */

          static gfc::string toPackagePath();

          /** \brief Get the plugins directory (OSX).
            *
            * This routine will return a string containing the path to the
            * plugins directory in the application bundle. This is only
            * applicable to OSX applications. If running on Windows, Linux,
            * mobile or consoles the value of toHelpersPath() is unique to the
            * platform.
            */

          static gfc::string toPluginsPath();

          /** \brief Get the helpers directory (OSX).
            *
            * This routine will return a string containing the path to the
            * helpers directory in the application bundle. This is only
            * applicable to OSX applications. If running on Windows, Linux,
            * mobile or consoles the value of toHelpersPath() is unique to the
            * platform.
            */

          static gfc::string toHelpersPath();

          /** \brief Get the eon files directory.
            *
            * This routine will return the directory where the eon file are
            * located in. It is a combination of the package path and the
            * stream path if running on mobile.
            *
            * If there is no package path set then toEonPath() will be the same
            * as if you called toStreamPath(). If a pacakge path has been set
            * and the first characters are "./" then the eon path is the
            * stream path plus the package path appended with ".eon" or
            * whatever your platform is the code is running on.
            *
            * \return Returns a GFC string containing the path to the .eon
            * directory in the project package.
            */

          static gfc::string toEonPath();

          /** \brief Get temporary directory.
            *
            * This routine will return a string containing a temporary
            * directory.
            *
            * \return A string containing the new directory.
            */

          static gfc::string tempPath();

          /** \brief Get home directory.
            *
            * This routine will return a string describing the location of the
            * home directory on this system. This folder is safe to write
            * program specific data to on all platforms. On Windows it is
            * "C:/Users/Name/" and on OSX "~/".
            *
            * \return Returns a string containing the home directory.
            */

          static gfc::string homePath();

        //}:                                      |
        //[debug]:{                               |

          /** @{
            *
            * \name Debugging helpers.
            *
            * These methods allow you to check for the presence of the debugger
            * attached to your running process and do other things.
            *
            * @{
            */

          /** \brief Is debugging?
            *
            * This routine will check if a debugger is attached to this
            * process.  It currently only works on Windows and Mac.
            *
            * \return Returns true if the debugger is attached or false.
            */

          static bool isDebugging();

        //}:                                      |
        //[hash]:{                                |

          static u128 hash128( cvp pData, const u32 size );

        //}:                                      |
        //[sha1]:{                                |

          static gfc::string sha1of( cvp pBuf, const u64 size );
          static gfc::string sha1of( const gfc::string& s );
          static gfc::string sha1of( const gfc::stream& s );
          #define e_sha1of( x ) EON::IEngine::sha1of( x )

        //}:                                      |
        //[ipc]:{                                 |

          /** @}
            *
            * \name Engine IPC support.
            *
            * This group of member functions allow you to allocate shared
            * memory among other things.
            *
            * @{
            */

          /** \brief Allocate a block of shared memory.
            *
            * This routine will allocate a block of shared memory using the key
            * provided.
            *
            * \param key A gfc string that gets hashed down to something the OS
            * can use. On OSX this boils down to a key_t value that's send over
            * to shmget(). On Windows we use memory mapped files.
            *
            * \param bytes The number of bytes to allocate.
            *
            * \param bServer Set to true if calling from the server application
            * of false otherwise. In the case of Swordlight, it would be the
            * server and the eon compiler (NDA--No Dave's Allowed) would be the
            * client.
            *
            * \return Returns a character pointer to the allocated space or
            * nullptr if an error ocurred.
            */

          static cp share( const gfc::string& key, const u64 bytes, bool bServer );

          /** \brief Free a block of shared memory.
            *
            * This routine will free up a block of shared memory.
            *
            * \param key The return value from shared_alloc.
            *
            * \return Returns true if the shared memory was released or false.
            */

          static bool unshare( const gfc::string& key );

          /** brief Synchronize shared memory.
            *
            * This routine will synchronize the shared memory for the given key.
            */

          static bool ssync( const gfc::string& key );

        //}:                                      |
        //[i/o]:{                                 |

          //--------------------------------------------------------------------
          // Cross platform file and directory services.
          //--------------------------------------------------------------------

          /** @}
            *
            * \name Engine file API.
            *
            * This group of member functions is recommended for related File
            * I/O, especially in the case of fopen. To be compatible with
            * Android do not call the std C fopen() but always
            * IEngine::fopen().
            *
            * @{
            */

          /** \brief Open a file for reading.
            *
            * This routine will open a file for reading. The data should be
            * considered read-only. On Android this routine reads out of the
            * compressed APK.
            */

          #define e_fopen( path, mode )                                         \
              IEngine::fopen( path, mode )                                      \

          /** \brief Open a file for reading.
            *
            * This routine will open a file for reading. The data should be
            * considered read-only. On Android this routine reads out of the
            * compressed APK.
            */

          static FILE* fopen( const gfc::string& path, ccp mode );

          /** \brief Take a snapshot of a directory.
            *
            * This routine will look at the given directory and call a lambda
            * for every file and directory underneath it.
            *
            * @param path The path of the directory to scan.
            * @param lambda The lambda function to call with each entry in the
            * directory excluding ".".
            *
            * @return Returns false if "path" was an invalid directory and true
            * otherwise.
            */

          static bool dir( const gfc::string& path
              , const std::function<void( const gfc::string& dir, const gfc::string& name, const bool bIsDirectory )>& lambda );

          /** \brief Test directory.
            *
            * This routine will test to see if a directory exists. If it does
            * the code returns true otherwise false.
            *
            * \param path The full path to the directory we want to check for.
            */

          static bool dexists( const gfc::string& path );

          /** \brief Test file.
            *
            * This routine will test to see if a file exists. If it does
            * the code returns true otherwise false.
            *
            * \param path The full path to the file we want to check for.
            */

          static bool fexists( const gfc::string& path );

          /** \brief Make a directory recursively.
            *
            * This routine will create directories as if mkdir -p was passed to
            * a base shell. It doesn't spawn the command line shell, however,
            * so it's quite a bit faster than system( "mkdir -p dir/subdir" )
            * could be.
            *
            * \param directories A string containing a full path. This path may
            * reference a file too. This is fine because my mkdir static method
            * will strip off the filename, convert the resulting path to an OS
            * specific version (everything in the engine uses Unix stlye paths
            * regardless of platform) -- think Windows paths, which suck!
            *
            * \return Returns true if the directory structure was created or
            * false otherwise.
            */

          static bool mkdir( const gfc::string& directories );

          /** \brief Load file.
            *
            * This routine will load a file into a stream object.
            *
            * \param path The path (in unix notation) to the file in question.
            *
            * \return Returns a stream object full of data. The stream will be
            * empty if there was an error.
            */

          #define e_fload( path )                                               \
              IEngine::fload( path )                                            \

          /** \brief Load file.
            *
            * This routine will load a file into a stream object.
            *
            * \param path The path (in unix notation) to the file in question.
            *
            * \return Returns a stream object full of data. The stream will be
            * empty if there was an error.
            */

          static gfc::stream fload( const gfc::string& path );

          /** \brief Shortcut version of mkdir.
            *
            * This routine is just a pass through for mkdir() and works in the
            * exact same way. For those die hard ODS junkies out there.
            *
            * \param directories A string containing a full path. This path may
            * reference a file too. This is fine because my mkdir static method
            * will strip off the filename, convert the resulting path to an OS
            * specific version (everything in the engine uses Unix stlye paths
            * regardless of platform) -- think Windows paths, which suck!
            *
            * \return Returns true if the directory structure was created or
            * false otherwise.
            */

          e_forceinline static bool md( const gfc::string& directories ){
            return mkdir( directories );
          }

          /** \brief Make directories recursively.
            *
            * This routine will create directories from a vector of paths.
            * It'll call the singular mkdir() repeatedly and so what applies to
            * that function applies to each individual path string in the
            * vector.
            *
            * \param directories A vector of path strings to be passed
            * consecutively to mkdir( path ).
            */

          e_noinline static u32 mkdir( const gfc::strings& directories ){
            u32 n = 0;
            directories.foreach(
              [&]( const gfc::string& path ){
                n += u32( mkdir( path ));
              }
            );
            return n;
          }

          /** \brief Make directories recursively (shortcut).
            *
            * This routine will create directories from a vector of paths.
            * It'll call the singular mkdir() repeatedly and so what applies to
            * that function applies to each individual path string in the
            * vector.
            *
            * \param directories A vector of path strings to be passed
            * consecutively to mkdir( path ).
            */

          e_forceinline static bool md( const gfc::strings& directories ){
            return( 0 != mkdir( directories ));
          }

          /** \brief Remove directory or file.
            *
            * This routine will blow away a file or an entire directory
            * structure recursively.
            *
            * \param path A string containing a full path. This path may
            * reference a file too. This is fine because my rm static method
            * will strip off the filename, convert the resulting path to an OS
            * specific version (everything in the engine uses Unix stlye paths
            * regardless of platform) -- think Windows paths, which suck!
            *
            * \return Returns true if the file or directory structure was nuked
            * or false otherwise.
            */

          static bool rm( const gfc::string& path );

          /** \brief Remove directories and/or files.
            *
            * This routine will repeatedly call rm() on individual files and/or
            * directories passed in.
            *
            * \param paths A vector of paths to pass into rm() in the singular.
            */

          e_noinline static u32 rm( const gfc::strings& paths ){
            u32 n = 0;
            paths.foreach(
              [&]( const gfc::string& path ){
                n += u32( rm( path ));
              }
            );
            return n;
          }

          /** \brief Copy file or directory.
            *
            * This routine will copy a file or directory to another location on
            * the file system.
            *
            * \param source This can be either a directory or a file, both will
            * work just fine.
            * \param target This can be either a directory or a file, both will
            * work just fine.
            */

          static void copy(
              const gfc::string& source
            , const gfc::string& target );

          //--------------------------------------------------------------------
          // Desktop process functions.
          //--------------------------------------------------------------------

          /** @}
            *
            * \name Desktop only methods.
            *
            * This section only compiles for desktop operating systems and will
            * be stripped from iOS, Android and consoles.
            *
            * @{
            */

          /** \brief Run system command.
            *
            * This routine will execute a system command.
            */

          #define e_system(...)                                                 \
           IEngine::system(__VA_ARGS__)                                         \

          /** \brief Run a shell command (asynchronous).
            *
            * This routine will execute a shell command as if you had called
            * the C function system(). The existence of this function is a
            * wrapper for popen() maybe system() on *nix systems and a special
            * indirector for Windows. Always use IEngine::shell() never
            * standard system() or popen/_popen.
            *
            * \param cmd The command line arguments to pass to operating
            * system.  On Windows this routine is a call to CreateProcess()
            * with the cmd always being "cmd.exe" and the args being the value
            * of "cmd".
            * \param args A vector of string objects that should be passed on
            * the program's command line.
            * \param lambda If a valid lambda function is passed on or
            * std::bind then the code pointed to by it will be called when the
            * shell command completes. This is an asynchronous operation or in
            * other words on Windows after the CreateProcess no call to
            * WaitForSingleObject is made. On *nix's like Linux and OSX a
            * pthread is kicked off to service the shell command.
            *
            * \return Returns true if the command executed correctly and false
            * otherwise.
            */

          static bool system( const gfc::string& cmd
              , const gfc::strings& args
              , const std::function<void()>& lambda=nullptr );

          /** \brief Run a shell command (asynchronous).
            *
            * This routine will execute a shell command as if you had called
            * the C function system(). The existence of this function is a
            * wrapper for popen() maybe system() on *nix systems and a special
            * indirector for Windows. Always use IEngine::shell() never
            * standard system() or popen/_popen.
            *
            * \param cmd The command line arguments to pass to operating
            * system.  On Windows this routine is a call to CreateProcess()
            * with the cmd always being "cmd.exe" and the args being the value
            * of "cmd".
            * \param lambda If a valid lambda function is passed on or
            * std::bind then the code pointed to by it will be called when the
            * shell command completes. This is an asynchronous operation or in
            * other words on Windows after the CreateProcess no call to
            * WaitForSingleObject is made. On *nix's like Linux and OSX a
            * pthread is kicked off to service the shell command.
            *
            * \return Returns true if the command executed correctly and false
            * otherwise.
            */

          e_forceinline static bool system( const gfc::string& cmd, const std::function<void()>& lambda=nullptr){
            return system( cmd, {}, lambda );
          }

          /** \brief Spawn new procsss function macro.
            *
            * This function macro will spawn a process.
            */

          #define e_exec( a, b, c, d )                                          \
           IEngine::exec( a, b, c, d )

          /** \brief Spawn a process (asynchronous).
            *
            * This routine will spawn a new process of the named application.
            * On OSX this may be a .app or CLI program. On Linux this may only
            * be a CLI program at the time of writing; when support for Weyland
            * GUIs are supported then this spawner code will allow the
            * launching of desktop apps as well.
            *
            * \param program The name of the program to launch. Must include an
            * absolute path to the program or it must be in the PATH
            * environment variable.
            *
            * \param args A vector of string objects that should be passed on
            * the program's command line.
            *
            * \param bBlocking If true the call to spawn() is a blocking one.
            * On Unix (including macOS) and all Linux (including Android) this
            * call does a fork + exec and so is intrinsically asynchronous. To
            * block until the program named in "program" completes then set
            * this parameter to true, if you don't care or if the new process
            * is a daemon or server then set it to false.
            *
            * \param lambda If a valid lambda function is passed on or
            * std::bind then the code pointed to by it will be called when the
            * spawned program completes. This is an asynchronous operation or
            * in other words on Windows after the CreateProcess no call to
            * WaitForSingleObject is made. On *nix's like Linux and OSX a
            * pthread is kicked off to service the shell command.
            *
            * \return Returns true if the command executed correct or false.
            */

          static bool exec(
              const gfc::string& program
            , const gfc::strings& args
            , const bool bBlocking
            , const std::function<void( const s32 return_code )>& lambda=nullptr );

          /** \brief Spawn in process plugin.
            *
            * This routine will spawn a plugin instance and run it
            * asynchronously.
            *
            * \param program The name of the program to launch. Must include an
            * absolute path to the program or it must be in the PATH
            * environment variable.
            *
            * \param args A vector of string objects that should be passed on
            * the program's command line.
            *
            * \param bBlocking If true the call to spawn() is a blocking one.
            * On Unix (including macOS) and all Linux (including Android) this
            * call does a fork + exec and so is intrinsically asynchronous. To
            * block until the program named in "program" completes then set
            * this parameter to true, if you don't care or if the new process
            * is a daemon or server then set it to false.
            *
            * \param lambda If a valid lambda function is passed on or
            * std::bind then the code pointed to by it will be called when the
            * spawned program completes. This is an asynchronous operation or
            * in other words on Windows after the CreateProcess no call to
            * WaitForSingleObject is made. On *nix's like Linux and OSX a
            * pthread is kicked off to service the shell command.
            *
            * \return Returns true if the command executed correct or false.
            */

          static bool spawn(
              const gfc::string& program
            , const gfc::strings& args
            , const bool bBlocking
            , const std::function<void( const s32 return_code )>& lambda=nullptr );

          /** \brief Read from stderr of spawned apps.
            *
            * This routine will read from stderr for every spawned app, which
            * of course means the compiler if the app is Swordlight.
            *
            * \return Returns a string containing the current contents of
            * stderr from the child process' perspective.
            */

          static gfc::string& getStderr();

          /** \brief Read from stdout of spawned apps.
            *
            * This routine will read from stdout for every spawned app, which
            * of course means the compiler if the app is Swordlight.
            *
            * \return Returns a string containing the current contents of
            * stdout from the child process' perspective.
            */

          static gfc::string& getStdout();

        //}:                                      |
        //[app]:{                                 |

          /** @}
            *
            * \name Application APIs.
            *
            * This group of member functions are entirely devoted to running
            * and booting the application. For example you always implement
            * main() yourself and call one of the run() functions to boot the
            * game.
            *
            * @{
            */

          /** \brief Application entry point.
            *
            * This routine will be called by the startup code and must be
            * written by the application programmer to start the engine. After
            * a little initialization the user must call IEngine::run(...) to
            * continue.
            *
            * \param args The arguments passed on the command line.
            *
            * \return Returns 0 on success otherwise an error code.
            */

          #ifdef __APPLE__
            #pragma GCC visibility push(hidden)
          #endif
            static int main( const gfc::strings& args );
          #ifdef __APPLE__
            #pragma GCC visibility pop
          #endif

          /** \brief Engine exit routine.
            *
            * Call this static member function to shut down the engine and
            * release all memory streams including textures, meshes,
            * entities, etc. On consoles, Andorid and iOS this function has no
            * effect otherwise it will shut down the app completely and return
            * to the desktop. This is especially useful in the case of Mac OSX.
            * Just intercept the close event and call exit().
            */

          static void exit();

          /** @} */

        //}:                                      |
      //}:                                        |
      //------------------------------------------|-----------------------------
    };
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//e_isMainThread:{                                |

  inline bool e_isMainThread(){
    return::EON::IEngine::isMainThread();
  }

//}:                                              |
//================================================|=============================

/**   @}
  * @}
  */
