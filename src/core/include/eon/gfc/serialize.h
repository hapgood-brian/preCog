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
  *   \addtogroup gfc
  *   @{
  *     \addtogroup serialization
  *     @{
  */

//================================================|=============================
//Serialization:{                                 |

  //----------------------------------------------------------------------------
  // Background loader functions.
  //----------------------------------------------------------------------------

  /** \brief Implementation of e_stream with no arguments.
    *
    * This routine will create a new fs::Reader class with the given name and
    * test if the .eon file named exists. If it does <b>not</b> then an empty
    * handle is returned of type T otherwise a new handle is generated, it's
    * I/O complete status bit set to zero, and a background thread is spawned
    * off to complete the load. The new handle is returned and can be used with
    * any engine API that expects it. Once the load has completed the thread
    * will automatically delete itself after marking the handle as
    * <i>complete</i>. The stream name will be set automatically for you so
    * you never need to blockUntilIOComplete() just to get the name from the
    * stream object.
    *
    * \param path The name of the .eon file to load without extension.
    *
    * \param onLoaded Lambda to be called when loader thread completes.
    *
    * \param args Variadic arguments.
    *
    * \return Returns a handle of type T to the new object.
    */

  template<typename T,typename...A> e_noinline typename T::handle e_stream(
        const EON::gfc::string& path
      , const std::function<void( T& )>& onLoaded
      , A...args ){

    //--------------------------------------------------------------------------
    // Using namespaces.
    //--------------------------------------------------------------------------

    using namespace EON;
    using namespace gfc;
    using namespace fs;

    //--------------------------------------------------------------------------
    // Bail conditions.
    //--------------------------------------------------------------------------

    // Create a streams reader instance.
    auto* ps = new Reader( path );
    auto& fs = ps->load(
        T::classname() );
    if( fs.isError() ){
      if( kVerbosity ){
        e_msgf(
          "Skipping stream: \"%s\" of class \"%s\"!"
          , ccp( path )
          , T::classname()
        );
      }
      delete ps;
      return 0;
    }

    // Grab the class identifier from the header of the stream.
    const auto clsid = fs.template as<u64>();
    if( !EON::gfc::Class::Factory::describe( clsid )){
      e_msgf(
        "Embedded clsid not known: \"%s\" of class \"%s\"!"
        , ccp( path )
        , T::classname()
      );
      delete ps;
      return 0;
    }

    // Once we have a class identifier we can produce an object.
    typename T::handle hT = e_newt( clsid, std::forward<A>( args )... );
    T& t = hT.cast();

    //--------------------------------------------------------------------------
    // If no errors serialize the object and mark as IO complete otherwise mark
    // object as I/O incomplete (error).
    //--------------------------------------------------------------------------

    const auto UUID = t.UUID;
    const auto& onError = [&]()->int{
      if( ps->isError() ){
        t.toStatus()->bIOComplete = 1;
        t.toStatus()->bIOError = 1;
        if( !t.subref() ){
          EON::gfc::Class::Factory::erase( UUID );
        }
        return-1;
      }
      return 0;
    };

    //--------------------------------------------------------------------------
    // Serializing from reading stream.
    //--------------------------------------------------------------------------

    t.toStatus()->bIOComplete = 0;
    t.preSerialize( *ps );
    if( onError() < 0 ){
      return-1;
    }
    t.serialize( *ps );
    if( onError() < 0 ){
      return-1;
    }
    t.postSerialize( *ps );
    if( onError() < 0 ){
      return-1;
    }

    //--------------------------------------------------------------------------
    // We're considered I/O complete even though the callback lambda hasn't had
    // a chance to monkey with the data.
    //--------------------------------------------------------------------------

    t.toStatus()->bIOComplete = 1;
    if( onLoaded ){
        onLoaded( t );
    }
    return hT;
  }

  /** \brief Implementation of e_stream with no arguments.
    *
    * This routine will create a new Reader class with the given name and test
    * if the .eon file named exists. If it does <b>not</b> then an empty handle
    * is returned of type T otherwise a new handle is generated, it's I/O
    * complete status bit set to zero, and a background thread is spawned off
    * to complete the load. The new handle is returned and can be used with any
    * engine API that expects it. Once the load has completed the thread will
    * automatically delete itself after marking the handle as <i>complete</i>.
    * The stream name will be set automatically for you so you never need to
    * blockUntilIOComplete() just to get the name from the stream object.
    *
    * \param path The name of the .eon file to load without extension.
    *
    * \param onLoaded Lambda to be called when loader thread completes.
    *
    * \param args Variadic arguments.
    *
    * \return Returns a handle of type T to the new object.
    */

  template<typename T,typename...A> e_noinline typename T::handle e_asyncStream(
        const EON::gfc::string& path
      , const std::function<void( T& )>& onLoaded
      , A...args ){

    //--------------------------------------------------------------------------
    // Using namespaces.
    //--------------------------------------------------------------------------

    using namespace EON;
    using namespace gfc;
    using namespace fs;

    //--------------------------------------------------------------------------
    // Bail conditions.
    //--------------------------------------------------------------------------

    // We  always  load the stream in the calling thread, which takes  some
    // time  and  should actually (eventually) be moved into  a  background
    // thread for optimal performance. We have to do this currently because we
    // need the class identifier clsid to create entity components whose type
    // is not known until the first few bytes are loaded.
    const auto start = e_seconds();
    auto* ps = new Reader( path );
    auto& fs = ps->load(
        T::classname() );
    if( fs.isError() ){
      if( kVerbosity ){
        e_msgf(
          "  Skipping stream: \"%s\" of class \"%s\"!"
          , ccp( path )
          , T::classname()
        );
      }
      delete ps;
      return 0;
    }

    // Grab the class identifier from the header of the stream. If the type is
    // not  in the class factory we know the stream is damanged and  we must
    // bail out again.
    const auto clsid = fs.template as<u64>();
    if( !EON::gfc::Class::Factory::describe( clsid )){
      e_msgf(
        "Embedded clsid not known: \"%s\" of class \"%s\"!"
        , ccp( path )
        , T::classname()
      );
      delete ps;
      return 0;
    }

    //--------------------------------------------------------------------------
    // Create a new handle of type clsid (T may not match, it could be  an
    // inherited class type). The startAsynchronicity<> function won't  do any
    // work if it detects the load has already occurred in a different thread.
    // In that case it'll only call the serialization functions on the object
    // we're about to make here.
    //--------------------------------------------------------------------------

    typename T::handle hT = e_newt( clsid, std::forward<A>( args )... );
    fs.template startAsynchronicity<T>( hT.cast(), onLoaded );
    const auto stop = e_seconds();
    if( kVerbosity ){
      e_msgf(
        "  %s took %.2fms to load!"
        , T::classname()
        , ( stop - start )*1000.f
      );
    }
    return hT;
  }

  /** \brief Find stream ready for streaming.
    *
    * This routine will scan all classes of type T and return true if the given
    * name exists in that list and false otherwise.
    *
    * \tparam T The type to search for.
    *
    * \param streamId The name of the type to search for. This can be a SHA1
    * key or a human readable name, corresponding with Stream::m_sSHA1 and
    * Stream::m_sName.
    *
    * \return Returns true if the instance was found.
    */

  template<typename T> e_noinline typename T::handle e_findUniqueAsyncStream(
        const EON::gfc::string& streamId
      , const std::function<void( T& )>& lambda ){

    //--------------------------------------------------------------------------
    // Using namespaces.
    //--------------------------------------------------------------------------

    using namespace EON;
    using namespace gfc;
    using namespace fs;

    //--------------------------------------------------------------------------
    // Search for the stream in the reflection system.
    //--------------------------------------------------------------------------

    typename T::handle hResult;
    EON::gfc::Class::foreachs<T>(
      [&]( const T& t ){
        if( e_isa<Stream>( &t )){
          const auto& stream = dynamic_cast<const Stream&>( t );
          // Check if that stream matches the SHA1 or name. Even though it
          // might be IO incomplete the stream name or SHA1 is guaranteed to
          // be ok.
          if(( stream.toSHA1() == streamId )||( stream.toName() == streamId )){
            // Yes we found it!
            if( lambda ){
              lambda( const_cast<T&>( t ));
            }
            // Save off the stream UUID and stop.
            hResult = t.UUID;
            return false;
          }
        }
        return true;
      }
    );
    return hResult;
  }

  /** \brief Unique stream streaming.
    *
    * This routine will load an unique stream. It does it by checking if the
    * object exists in the reflection database. If it's not present then it'll
    * stream in a new object and return it.
    *
    * \param streamId The name of the stream we want to load.
    *
    * \param lambda The callback function that gets called when the stream is
    * finished streaming off disk.
    *
    * \return Returns a handle to the requested stream.
    */

  template<typename T,typename...A> e_noinline typename T::handle e_uniqueAsyncStream(
        const EON::gfc::string& streamId
      , const std::function<void( T& )>& lambda=nullptr
      , A...args ){
    if( streamId.empty() ){
      return 0;
    }

    //--------------------------------------------------------------------------
    // Using namespaces.
    //--------------------------------------------------------------------------

    using namespace EON;
    using namespace gfc;
    using namespace fs;

    //--------------------------------------------------------------------------
    // Search for the stream in the reflection system.
    //--------------------------------------------------------------------------

    auto hResult = e_findUniqueAsyncStream<T>(
        streamId
      , lambda
    );

    //--------------------------------------------------------------------------
    // If we didn't find anything start streaming it in.
    //--------------------------------------------------------------------------

    if( !hResult ){
      hResult = e_asyncStream<T,A...>( streamId,
        [=]( T& t ){
          if( kVerbosity ){
            e_msgf(
              "  Unique \"%s\" completed!"
              , ccp( streamId )
            );
          }
          if( !lambda ){
            return;
          }
          lambda( t );
        },
        std::forward<A>( args )...
      );
    }else if( hResult->isIOComplete() ){
      if( kVerbosity && kUseTracing ){
        e_msgf(
          "  Stream \"%s\" is already IO complete!"
          , ccp( streamId )
        );
      }
    }else if( kVerbosity && kUseTracing ){
      e_msgf(
        "  Stream \"%s\" already streaming!"
        , ccp( streamId )
      );
    }
    return hResult;
  }

  /** \brief Unique stream streaming.
    *
    * This routine will load an unique stream. It does it by checking if the
    * object exists in the reflection database. If it's not present then it'll
    * stream in a new object and return it.
    *
    * \param streamId The name of the stream we want to load.
    *
    * \param lambda The callback function that gets called when the stream is
    * finished streaming off disk.
    *
    * \return Returns a handle to the requested stream.
    */

  template<typename T> e_noinline typename T::handle e_uniqueAsyncStream(
        const EON::gfc::string& streamId
      , const std::function<void( T& )>& lambda ){

    //--------------------------------------------------------------------------
    // Using namespaces.
    //--------------------------------------------------------------------------

    using namespace EON;
    using namespace gfc;
    using namespace fs;

    //--------------------------------------------------------------------------
    // Bail conditions.
    //--------------------------------------------------------------------------

    if( streamId.empty() ){
      return 0;
    }

    //--------------------------------------------------------------------------
    // Search for the stream in the reflection system.
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Search for the stream in the reflection system.
    //--------------------------------------------------------------------------

    auto hResult = e_findUniqueAsyncStream<T>(
        streamId
      , lambda
    );

    //--------------------------------------------------------------------------
    // If we didn't find anything start streaming it in.
    //--------------------------------------------------------------------------

    if( !hResult ){
      hResult = e_asyncStream<T>( streamId,
        [=]( T& t ){
          if( kVerbosity ){
            e_msgf(
              "    Unique \"%s\" completed!"
              , ccp( streamId )
            );
          }
          if( !lambda ){
            return;
          }
          lambda( t );
        }
      );
    }else if( hResult->isIOComplete() ){
      if( kVerbosity && kUseTracing ){
        e_msgf(
          "  Stream \"%s\" is already IO complete!"
          , ccp( streamId )
        );
      }
    }else if( kVerbosity && kUseTracing ){
      e_msgf(
        "  Stream \"%s\" already streaming!"
        , ccp( streamId )
      );
    }
    return hResult;
  }

  //----------------------------------------------------------------------------
  // Generic blocking serialization methods from EON files.
  //----------------------------------------------------------------------------

  /** \brief Foreground loader from EON format file.
    *
    * This routine will serialize in an object from a .eon file somewhere on
    * disk or in the package under .{platform-name}.
    *
    * \tparam T The type of the object to load.
    *
    * \tparam A The arguments template type to the loader.
    *
    * \param path The path to the input file. If this path is just a name with
    * no extension or path the code will load it from the current package under
    * .osx (for Mac), .android, .ios, .linux or .win. If a path is prepended
    * then the code will load it from there. If the .eon extension is missing
    * then one will be appended before loading so it's perfectly ok to pass in
    * "bob" to load ".osx/bob.eon" for example.
    *
    * \param args Variadic template arguments.
    *
    * \return Returns true if the .eon file was found and loaded and false
    * otherwise.
    */

  template<typename T,typename...A> e_noinline typename T::handle e_load(
        const EON::gfc::string& path
      , A...args ){
    typename T::handle hT = e_asyncStream<T,A...>(
        path
      , nullptr
      , std::forward<A>( args )... );
    if( !hT ){
      return 0;
    }
    hT->blockUntilIOComplete();
    return hT;
  }

  /** \brief Load stream.
    *
    * This routine will load a stream as if calling e_load() but instead of
    * the full path being required this version just takes a stream name or
    * SHA1 identifier.
    *
    * \tparam T The type of the object to load.
    *
    * \tparam A The arguments template type to the loader.
    *
    * \param key The name of the stream. The engine tries very hard to keep
    * these keyed names unique but it can't always manage it. This is why the
    * SHA1 key is provided on a stream asset that's generated from the data
    * and content.
    *
    * \param args Variadic template arguments.
    *
    * \return Returns a nice auto reference counting handle in type T. For T to
    * be used by this routine you must include e_reflect[_no_properties] on it.
    * If there's an error the return value will be an empty handle, or one with
    * the value of zero. You can compare agaainst nullptr if you like.
    */

  template<typename T,typename...A> e_noinline typename T::handle e_loadStream(
        const EON::gfc::string& key
      , A...args ){
    typename T::handle hT = e_load<T>(
        e_packagePath() + "/.cache/" + key + ".eon"
      , std::forward<A>( args )...);
    if( !hT ){
      hT = e_load<T>(
          key
        , std::forward<A>( args )...
      );
    }
    return hT;
  }

  /** \brief Load stream.
    *
    * This routine will load a stream as if calling e_load() but instead of
    * the full path being required this version just takes a stream name or
    * SHA1 identifier.
    */

  template<typename T> e_noinline typename T::handle e_loadStream( const EON::gfc::string& key ){
    typename T::handle hT = e_load<T>( e_packagePath() + "/.cache/" + key + ".eon" );
    if( !hT ){
      hT = e_load<T>( key );
    }
    return hT;
  }

  //------------------------------------------------------------------------
  // Generic unthreaded serialization methods EON files.
  //------------------------------------------------------------------------

  /** \brief Generate engine save path.
    *
    * This routine will generate a save path from the given path.
    *
    * \param path File path to convert.
    *
    * \return Returns a proper engine save path.
    */

  EON::gfc::string e_toSavePath( const EON::gfc::string& path );

  /** \brief Internal save function.
    *
    * Called from e_save() to write out a stream object.
    *
    * \param t The resouce reference.
    *
    * \param pw The shared pointer to a writer object.
    */

  template<typename T> void e_internalSave( T& t, std::shared_ptr<EON::gfc::fs::Writer>& pw ){
    using namespace EON;
    using namespace gfc::fs;
    t.blockUntilIOComplete();
    pw->writeHandle<T>( typename T::handle( t.UUID ));
    pw->save( t.classof() );
    if( pw->toFlags()->bRenameSHA1 ){
      t.setSHA1( pw->toFilename().basename() );
      if( kVerbosity ){
        e_msgf(
          "$(lightgreen)Saving\t$(lightblue)SHA1:%s (%s)"
          , ccp( t.toSHA1() )
          , T::classname()
        );
      }
    }else if( kVerbosity ){
      e_msgf(
        "$(lightgreen)Saving\t$(lightblue) \"%s\" (%s)"
        , ccp( pw->toFilename() )
        , T::classname()
      );
    }
  }

  /** \brief Save object to disk synchronously.
    *
    * This routine will synchronously write a .eon file for the given object in
    * the proper location.
    *
    * \tparam T The type of object we'll save.
    *
    * \param in_onlyPath The path to write the object to.
    *
    * \param r A non-const object reference.
    *
    * \param flags The save flags as passd to the fs::Writer object created.
    *
    * \return Returns the name of the object now on disk.
    */

  template<typename T> e_noinline EON::gfc::string e_save(
        const EON::gfc::string& in_onlyPath
      , typename std::remove_const<T>::type& r
      , const EON::u32 flags ){

    //--------------------------------------------------------------------------
    // Using namespaces.
    //--------------------------------------------------------------------------

    using namespace EON;
    using namespace gfc;
    using namespace fs;

    //--------------------------------------------------------------------------
    // Entry validation.
    //--------------------------------------------------------------------------

    auto hT = typename T::handle( r.UUID );
    if( !hT ){
      e_unreachable( "Bad UUID passed into e_save()" );
    }

    //--------------------------------------------------------------------------
    // Figure out the path we want to save to. If the path starts with only a /
    // or ../ or ~/ or ./ then it's an absolute or relative path, which we will
    // use immediately. Otherwise we'll build the path from the pkg path and
    // .eon/ directory.
    //--------------------------------------------------------------------------

    string path;
    const string& onlyPath = in_onlyPath.path();
    if(( '/' == * onlyPath )
        ||( string( "../" ) == onlyPath.left( 3 ))
        ||( string(  "~/" ) == onlyPath.left( 2 ))
        ||( string(  "./" ) == onlyPath.left( 2 ))){
      path = onlyPath;
    }else{
      if( flags & kIMPORT ){
        path = e_pathof<T>();
      }else{
        path = ".cache/";
      }
      path = e_toSavePath( path );
    }
    path += r.getStreamID();
    path += ".eon";

    //--------------------------------------------------------------------------
    // Don't save SHA1's more than once if they exist on disk already.
    //--------------------------------------------------------------------------

    if(( r.toSHA1().hash() != "0000000000000000000000000000000000000000"_64 )&&( flags & kHISTORIC )&& e_fexists( path )){
      return r.toSHA1();
    }

    //--------------------------------------------------------------------------
    // Create a writer stream, serialize stream to it and save.
    //
    // Consider the following: If we're in a background thread already do we
    // really want to spawn a new thread again? I don't think we do.
    //--------------------------------------------------------------------------

    std::shared_ptr<Writer> pw = std::make_shared<Writer>( path, flags );
    e_internalSave<T>( r, pw );
    if( flags & kSHA1 ){
      return r.toSHA1();
    }
    return path;
  }

//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
