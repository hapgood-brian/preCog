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

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup serialization
  *     @{
  */

//================================================|=============================
//Serialization:{                                 |

  namespace EON{

    namespace gfc{

      //------------------------------------------------------------------------
      // Foreground loader functions.
      //------------------------------------------------------------------------

      /** \brief Pull object in from reader stream.
        *
        * This routine will create a handle and load it from the current reader
        * stream. If the stream has an error 0 is returned.
        *
        * \param st The stream to read from; will be passed to object's
        * serialize method after it has been created.
        * \param args Variadic template arguments passed to e_new().
        *
        * \return Returns a handle of type T or 0 if the stream had an error in
        * it.
        */

      template<typename T,typename...A > e_forceinline typename T::handle e_stream( fs::Reader& st, A...args ){
        if( !st ){
          return 0;
        }
        typename T::handle hT = e_new( T, std::forward<A>( args )... );
             hT->preSerialize ( st );
        hT = hT->serialize    ( st );
             hT->postSerialize( st );
        return hT;
      }

      //------------------------------------------------------------------------
      // Background loader functions.
      //------------------------------------------------------------------------

      /** \brief Implementation of e_stream with no arguments.
        *
        * This routine will create a new fs::Reader class with the given name
        * and test if the .eon file named exists. If it does <b>not</b> then an
        * empty handle is returned of type T otherwise a new handle is
        * generated, it's I/O complete status bit set to zero, and a background
        * thread is spawned off to complete the load. The new handle is
        * returned and can be used with any engine API that expects it. Once
        * the load has completed the thread will automatically delete itself
        * after marking the handle as <i>complete</i>. The resource name will
        * be set automatically for you so you never need to
        * blockUntilIOComplete() just to get the name from the resource object.
        *
        * \param name The name of the .eon file to load without extension.
        * \param onLoaded Lambda to be called when loader thread completes.
        * \param args Variadic arguments.
        *
        * \return Returns a handle of type T to the new object.
        */

      template<typename T,typename...A> e_noinline typename T::handle e_asyncStream( const string& name, const std::function<void( Object* )>& onLoaded, A...args ){
        // Create a file system reader to stream from.
        auto* pfs = new fs::Reader( name );
        if( !pfs->exists( T::classname() )){
          delete pfs;
          return 0;
        }
        // Create a new handle of type T.
        typename T::handle hT = e_new( T, std::forward<A>( args )... );
        T* pT = hT.pcast();
        // Set the IO status to incomplete.
        pT->toStatus()->bIOComplete = 0;
        // If we're streaming a resource then set it's name or SHA1 key ahead of
        // time so e_uniqueAsyncStream() can be assured that this name or SHA1 key is
        // good.
        if( e_isa<Resource>( pT )){
          auto* pResource = dynamic_cast<Resource*>( pT );
          if( name.is_sha1() ){
            if( pResource->toSHA1().empty() ){
              pResource->setSHA1( name );
            }
          }else if( pResource->toName().empty() ){
            pResource->setName( name );
          }
        }
        // Start loading the resource asynchronously for best performance.
        pfs->startAsyncLoad( T::classname(), pT, onLoaded );
        return hT;
      }

      /** \brief Unique resource streaming.
        *
        * This routine will load an unique resource. It does it by checking if
        * the object exists in the reflection database. If it's not present
        * then it'll stream in a new object and return it.
        *
        * \param resourceId The name of the resource we want to load.
        * \param lambda The callback function that gets called when the
        * resource is finished streaming off disk.
        *
        * \return Returns a handle to the requested resource.
        */

      template<typename T> e_noinline typename T::handle e_uniqueAsyncStream( const string& resourceId, const std::function<void( Object* )>& lambda=nullptr ){

        //----------------------------------------------------------------------
        // Search for the resource in the reflection system.
        //----------------------------------------------------------------------

        typename T::handle hResult;
        Class::foreachs<T>( [&]( const T& object ){
          e_assert( e_isa<Resource>( &object ), "This is not a resource!" );
          const Resource& resource = static_cast<const Resource&>( object );
          // Check if that resource matches the SHA1 or name. Even though it
          // might be IO incomplete the resource name or SHA1 is guaranteed to
          // be ok.
          if(( resource.toSHA1() == resourceId )||( resource.toName() == resourceId )){
            // Yes we found it!
            if( lambda ){
              lambda(( Object* )&object );
            }
            // Save off the resource UUID and stop.
            hResult = object.UUID;
            return false;
          }
          return true;
        });

        //----------------------------------------------------------------------
        // If we didn't find anything start streaming it in.
        //----------------------------------------------------------------------

        if( !hResult ){
          hResult = e_asyncStream<T>( resourceId, lambda );
        }
        return hResult;
      }

      //------------------------------------------------------------------------
      // Generic un-threaded serialization methods from XML and EON files.
      //------------------------------------------------------------------------

      /** \brief Foreground loader from EON format file.
        *
        * This routine will serialize in an object from a .eon file somewhere
        * on disk or in the package under .{platform-name}.
        *
        * \param path The path to the input file. If this path is just a name
        * with no extension or path the code will load it from the current
        * package under .osx (for Mac), .android, .ios, .linux or .win. If a
        * path is prepended then the code will load it from there. If the .eon
        * extension is missing then one will be appended before loading so it's
        * perfectly ok to pass in "bob" to load ".osx/bob.eon" for example.
        *
        * \return Returns true if the .eon file was found and loaded and false
        * otherwise.
        */

      template<typename T> e_noinline typename T::handle e_load( const string& path ){
        typename T::handle hT;
        using namespace fs;
        Reader fs( path );
        fs.load( T::classname() );
        if( !fs.isError() ){
          #if e_compiling( debug )
            auto pStart = fs.c_str();
          #endif
          e_assert( e_classid<T>() == *reinterpret_cast<const u64*>( pStart ));
          hT = fs.serializeHandleUnguarded().as<T>();
        }
        return hT;
      }

      /** \brief Load resource.
        *
        * This routine will load a resource as if calling e_load() but instead
        * of the full path being required this version just takes a resource
        * name or SHA1 identifier.
        */

      template<typename T> e_noinline typename T::handle e_loadResource( const string& key ){
        typename T::handle hT = e_load<T>( e_packagePath() + "/.eon/" + key + ".eon" );
        if( !hT ){
          hT = e_load<T>( key );
        }
        return hT;
      }

      /** \brief Load resource.
        *
        * This routine will load a resource as if calling e_load() but instead
        * of the full path being required this version just takes a resource
        * name or SHA1 identifier.
        */

      template<typename T> e_noinline typename T::handle e_loadSource( const string& key ){
        return e_load<T>( e_packagePath() + "/" + e_pathof<T>() + "/" + key + ".eon" );
      }

      //------------------------------------------------------------------------
      // Generic unthreaded serialization methods to XML and EON files.
      //------------------------------------------------------------------------

      /** \brief Generate engine save path.
        *
        * This routine will generate a save path from the given path.
        *
        * \param path File path to convert.
        *
        * \return Returns a proper engine save path.
        */

      string e_toSavePath( const string& path );

      /** \brief Save object to disk synchronously.
        *
        * This routine will synchronously write a .eon file for the given
        * object in the proper location.
        *
        * \tparam T The type of object we'll save.
        * \param in_onlyPath The path to write the object to.
        * \param resource A non-const resource reference.
        * \param flags The save flags as passd to the fs::Writer object
        * created.
        *
        * \return Returns the name of the object now on disk.
        */

      template<typename T> e_noinline string e_save( const string& in_onlyPath, typename std::remove_const<T>::type& resource, const u32 flags ){

        resource.blockUntilIOComplete();

        //----------------------------------------------------------------------
        // Figure out the path we want to save to.
        //----------------------------------------------------------------------

        string path;
        const string& onlyPath = in_onlyPath.path();
        if(( '/' == * onlyPath )||( string( "./" ) == onlyPath.left( 2 ))){
          path = onlyPath;
        }else{
          if( flags & fs::kIMPORT ){
            path = e_pathof<T>();
          }else{
            path = ".eon/";
          }
          path = e_toSavePath( path );
        }

        //----------------------------------------------------------------------
        // Create a writer stream, serialize resource to it and save.
        //----------------------------------------------------------------------

        fs::Writer fs( path + resource.toName() + ".eon", flags );
        fs.serializeResource<T>( resource );
        if( flags & fs::kSHA1 ){
          fs.setFilename( path + resource.toSHA1() + ".eon" );
        }
        fs.save( resource.classof() );
        return fs.toFilename();
      }
    }
  }

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
