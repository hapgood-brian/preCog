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
  *    \addtogroup io
  *    @{
  */

//================================================+=============================
//Asynchronicity:{                                |

  namespace EON{
    namespace gfc{
      namespace fs{
        template<typename T> using  OnLoaded = std::function<void( T& )>;
        template<typename T> struct Asynchronicity;
      }
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//FileSystemAPI:{                                 |

#ifdef __APPLE__
  #pragma mark - File API -
#endif

  bool e_fexists( const EON::gfc::string& ); // <-- file
  bool e_dexists( const EON::gfc::string& ); // <-- directory
  bool e_lexists( const EON::gfc::string& ); // <-- link

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//IWriter:{                                       |

#ifdef __APPLE__
  #pragma mark - IWriter interface -
#endif

  namespace EON{
    namespace gfc{
      namespace fs{
        struct E_PUBLISH IWriter{
          virtual void onOpen( const string& ){}
          virtual void onWrite( const u32 ){}
          virtual void onShut(){}
        };
      }
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Writer:{                                        |

#ifdef __APPLE__
  #pragma mark - Writer class and flags -
#endif

  namespace EON{
    namespace gfc{
      namespace fs{

        //------------------------------------------------------------------------
        // Constant expressions.
        //------------------------------------------------------------------------

        #if !e_compiling( master )
          static constexpr bool kUseTracing = false;
          static constexpr bool kVerbosity  = false;
          static constexpr bool kErrors     = true;
        #else
          static constexpr bool kUseTracing = false;
          static constexpr bool kVerbosity  = false;
          static constexpr bool kErrors     = false;
        #endif
        #if e_compiling( android )
          static constexpr bool kMultiThreaded = false;
        #else
          static constexpr bool kMultiThreaded = true;
        #endif

        //------------------------------------------------------------------------
        // Enumerated types.
        //------------------------------------------------------------------------

        /** \brief Special serialization masks.
          *
          * These masks are used for recording keyframes in the history.
          */

        enum Masks{
          MASK_MODIFIED = 1, //!< A keyframe was modified.
          MASK_KEYFRAME = 2, //!< Is a keyframe.
          MASK_HIDDEN   = 4, //!< Hidden frame.
        };

        /** \brief Saving flags.
          *
          * These flags should be OR'd together when calling writer save.
          */

        enum Flags{
          kHISTORIC =  1, //!< Marks the writer stream as historic.
          kCOMPRESS =  2, //!< Compress the file stream.
          kIMPORT   =  4, //!< Import a new file stream.
          kTEXT     =  8, //!< Text only stream.
          kSHA1     = 16, //!< Stamp SHA1 key on the stream.
          kNOEXT    = 32, //!< Don't add .eon extension.
        };

        /** \brief File system abstraction object.
          *
          * The FS class takes the work out of handling files on every platform the
          * engine supports.
          */

        struct E_PUBLISH Writer final{

          //--------------------------------------+-----------------------------
          //Aliases:{                             |

            using Pending = std::function<void()>;

          //}:                                    |
          //Operate:{                             |

            /** \name Streaming operators.
              *
              * These operators let you write various objects into the stream.
              *
              * @{
              */

            template<typename T> e_forceinline Writer& operator<<( const vector<T>& v ){
              write( v );
              return *this;
            }

            template<typename T> e_forceinline Writer& operator<<( const T& t ){
              write( t );
              return *this;
            }

            e_forceinline Writer& operator<<( const Object::prop_ptr& pProperty ){
              write( pProperty );
              return *this;
            }

            e_forceinline Writer& operator<<( const stream& st ){
              write( st );
              return *this;
            }

            e_forceinline Writer& operator<<( const buffer& b ){
              write( b );
              return *this;
            }

            e_forceinline Writer& operator<<( const string& s ){
              write( s );
              return *this;
            }

            e_forceinline Writer& operator<<( ccp s ){
              write( s );
              return *this;
            }

            /** @} */

          //}:                                    |
          //Methods:{                             |
            //write:{                             |

              /** \name Serializing (write) handles.
                *
                * These routines will serialize a handle with guards out to this
                * output [writer] stream.
                */

              /** \brief Serialize an object handle.
                *
                * This routine will serialize out an object handle to this stream.
                * If it's a stream then we'll go through the special serialize
                * above which does a double pass to calculate and save the SHA1
                * key of the object.
                *
                * \param hT The handle to type T object.
                */

              template<typename T> e_noinline u64 writeHandle( const AutoRef<T>& hT ){
                const auto bytes = m_tStream.bytes();
                if( !hT ){
                  e_break( "Empty handle!" );
                }
                const T&   t = hT.cast();
                t.blockUntilIOComplete();
                t.preSerialize ( *this );
                t.serialize    ( *this );
                t.postSerialize( *this );
                const u64 ending = m_tStream.bytes();
                return(   ending - bytes );
              }

              /** \brief Serialize handle asynchronously.
                *
                * This routine will serialize a handle asynchronously. It does
                * it by writing out a packed string for the default key value.
                * That will make sure the dictionary is populated fully so we
                * can come back later and patch the stream.
                *
                * \tparam T The object type we want to serialize. Must be a
                * derivitive of Stream.
                *
                * \param in_hT The input object to serialize.
                */

              template<typename T> e_noinline void detachAsyncStream( const AutoRef<T>& in_hT ){
                if( !in_hT )
                  e_break( "Empty handle!" );
                auto& t = in_hT.noconst().cast();

                //--------------------------------------------------------------
                // Save out already SHA1'd assets.
                //--------------------------------------------------------------

                if( t.toSHA1().is_sha1() ){
                  pack( t.toSHA1() );
                  if( kVerbosity ){
                    e_msgf(
                      "$(green)short $(lightgreen)circuiting$(off): \"$(lightblue)%s$(off)\""
                      , ccp( t.toSHA1() )
                    );
                  }
                  t.toStatus()->bUnavailable = 1;
                  e_save<T>(
                      string::null
                    , t
                    , 0
                      | ( isRecording() ? kHISTORIC : 0 )
                      | kCOMPRESS
                      | kIMPORT
                      | kSHA1 );
                  t.toStatus()->bUnavailable = 0;
                  return;
                }

                //--------------------------------------------------------------
                // Save and later stamp on top of packed SHA1 id.
                //--------------------------------------------------------------

                const auto start = m_tStream.tell();
                pack( string::repeating( '0', 40 ));
                m_vPending.push(
                  // Will save and update the SHA1 key from async foreach.
                  [this,start,in_hT](){
                    auto& t = in_hT.noconst().cast();
                    t.toStatus()->bUnavailable = 1;
                    const auto& sha1 = e_save<T>(
                        string::null
                      , t
                      , 0
                        | ( isRecording() ? kHISTORIC : 0 )
                        | kCOMPRESS
                        | kIMPORT
                        | kSHA1 )
                      . basename();
                    t.toStatus()->bUnavailable = 0;
                    t.setSHA1( sha1 );
                    e_guardw( m_tLock );
                    m_tStream.seek( start );
                    pack( sha1 );
                  }
                );
              }

              /** @} */

            //}:                                  |
            //compress:{                          |

            /** \name Streaming operators.
              *
              * These methods let you compress the stream.
              *
              * @{
              */

              /** \brief Compress write stream.
                *
                * This routine will compress the writers stream.
                *
                * \return Returns the number of bytes written.n
                */

              u32 compress();

              /** @} */

            //}:                                  |
            //version:{                           |

              /** \name Versioning and testing.
                *
                * These methods let you version your stream.
                *
                * @{
                */

              /** \brief Write a version code.
                *
                * This routine will write a version code to the stream.
                *
                * \param ver The version code to write to the stream.
                */

              void version( const u16 ver );

              /** @} */

            //}:                                  |
            //write:{                             |

              /** \name Writing and seeking.
                *
                * These methods let you write data to the stream.
                *
                * @{
                */

              u64 writeProperties( const Object::handle& );
              u64 write( cvp p, const u64 n );
              u64 write( const stream& );
              u64 write( const buffer& );
              u64 write( const string& );
              u64 write( ccp string );

              /** \brief Write a hashmap of objects.
                *
                * This routine will write out a hashmap of objects including size
                * etc. so we can read it back in later.
                *
                * \tparam K The type of key in the container.
                * \tparam V The type of object to write.
                * \param m The hashmap of T objects to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename K,typename V> e_noinline u64 write( const hashmap<K,V>& m ){
                e_guardw( m_tLock );
                u64 bytes = 0;
                bytes += write( m.size() );
                m.foreachKV(
                  [&]( const K k, const V& v ){
                    bytes += write( k );
                    bytes += write( v );
                  }
                );
                return bytes;
              }

              /** \brief Write a map of objects.
                *
                * This routine will write out a hashmap of objects including size
                * etc. so we can read it back in later.
                *
                * \tparam K The type of key in the container.
                * \tparam V The type of object to write.
                * \param m The hashmap of T objects to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename K,typename V> e_noinline u64 write( const map<K,V>& m ){
                e_guardw( m_tLock );
                u64 bytes = 0;
                bytes += write( m.size() );
                m.foreachKV(
                  [&]( const K k, const V& v ){
                    bytes += write( k );
                    bytes += write( v );
                  }
                );
                return bytes;
              }

              /** \brief Write a pool of objects.
                *
                * This routine will write out a pool of objects including size
                * etc. so we can read it back in later.
                *
                * \tparam T The type of object to write.
                * \param p The pool of T objects to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename T, u64 N> e_noinline u64 write( const pool<T,N>& p ){
                e_guardw( m_tLock );
                u64 bytes = 0;
                bytes += write( p.size() );
                p.foreach(
                  [&]( const T& t ){
                    bytes += write( t );
                  }
                );
                return bytes;
              }

              /** \brief Write an array of objects.
                *
                * This routine will write out an array of objects including size
                * etc. so we can read it back in later.
                *
                * \tparam T The type of object to write.
                * \param  a The array of T objects to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename T, u32 N> e_noinline u64 write( const array<T,N>& a ){
                e_guardw( m_tLock );
                u64 bytes = 0;
                bytes += write( a.size() );
                bytes += write( &a[0], sizeof( T )*a.size() );
                return bytes;
              }

              /** \brief Write a vector of objects.
                *
                * This routine will write out a vector of objects including size
                * etc. so we can read it back in later.
                *
                * \tparam T The type of object to write.
                * \param v The vector of T objects to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename T> e_noinline u64 write( const vector<T>& v ){
                e_guardw( m_tLock );
                u64 bytes = 0;
                bytes += write( v.size() );
                bytes += write( &v[0], sizeof( T )*v.size() );
                return bytes;
              }

              /** \brief Write a deque of objects.
                *
                * This routine will write out a deque of objects including size
                * etc. so we can read it back in later.
                *
                * \tparam T The type of object to write.
                * \param  d The deque of T objects to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename T> e_noinline u64 write( const deque<T>& d ){
                e_guardw( m_tLock );
                u64 bytes = 0;
                bytes += write( d.size() );
                d.foreach(
                  [&]( const T& t ){
                    bytes += write( t );
                  }
                );
                return bytes;
              }

              /** \brief Write an object of type T.
                *
                * This routine writes a single object of any type.
                *
                * \tparam T The type of the object to write.
                * \param t The object to write.
                *
                * \return Returns the number of bytes written.
                */

              template<typename T> e_noinline u64 write( const T& t ){
                e_guardw( m_tLock );
                return write( &t, sizeof( T ));
              }

              /** \brief Write a object property.
                *
                * This routine will write out an object property pointer.
                */

              u64 write( const Object::prop_ptr& );

              /** \brief Seek in stream.
                *
                * This routine seeks in the stream.
                */

              e_forceinline void seek( const u64 o ){
                e_guardw( m_tLock );
                m_tStream.seek( o );
              }

              /** \brief Special thread safe write via lambda.
                *
                * This routine will acquire a write guard before calling the lamda.
                * This will make a group of writes atomic.
                *
                * \param lambda A callback function to be called atomically.
                */

              e_forceinline void atomic( const std::function<void()>& lambda ){
                if( lambda ){
                  e_guardw( m_tLock );
                  lambda();
                }
              }

              /** \brief Skip N bytes.
                *
                * This code will skip forward N elements. Often used in conjunction
                * with the reset() method.
                *
                * \param size The number of elements to skip forward.
                *
                * \return Returns the number of bytes written.
                */

              u64 skip( const u64 size );

              /** @} */

            //}:                                  |
            //reset:{                             |

              /** \name Reset the stream tail back to the head.
                *
                * This method lets you reset the stream back to the beginning.
                *
                * @{
                */

              e_forceinline u64 reset(){
                e_guardw( m_tLock );
                const u64 size = m_tStream.size();
                m_tStream.reset();
                return size;
              }

              /** @} */

            //}:                                  |
            //used:{                              |

              /** \name Return the number of bytes used by the stream.
                *
                * This method lets you track how many bytes have been written
                * to the writer stream.
                *
                * @{
                */

              e_forceinline u64 used()const{
                e_guardr( m_tLock );
                return m_tStream.size();
              }

              /** @} */

            //}:                                  |
            //cache:{                             |

              /** \name Grab the string cache.
                *
                * This method lets you get hold of the low level cache of
                * strings.
                *
                * @{
                */

              e_forceinline const std::shared_ptr<Writer>& toCache()const{
                return m_pCache;
              }

              /** @} */

            private:

              /** \brief Cache string till last.
                *
                * This routine will call pack() on the packing output stream so we
                * can have an invariant record for histories, undo/redo and scene
                * automation timeline. The data will be written at the very end of
                * the file, right before the dictionary.
                *
                * \param s A gfc string object containing the C string to cache.
                *
                * \return Returns the number of bytes writeen to the cache.
                */

              e_forceinline u64 cache( const string& s ){
                return cache( s.c_str() );
              }

              /** \brief Cache string till last.
                *
                * This routine will call pack() on the packing output stream so we
                * can have an invariant record for histories, undo/redo and scene
                * automation timeline. The data will be written at the very end of
                * the file, right before the dictionary.
                *
                * \param cString A C string.
                *
                * \return Returns the number of bytes writeen to the cache.
                */

              u64 cache( ccp cString );

            public:

            //}:                                  |
            //pack:{                              |

              /** \name Pack a string into the stream.
                *
                * This method compresses a string and copies the bytes into a
                * bit packed state and dictionary.
                *
                * @{
                */

              e_forceinline u64 pack( ccp cString, const u64 nString ){
                u64 bytes;
                if( !isRecording() || isShallow() ){
                  bytes = cache( string( cString, cString+nString ));
                }else{
                  bytes = packInternal( cString,  nString );
                }
                return bytes;
              }

              e_forceinline u64 pack( const string& string ){
                u64 bytes;
                if( !isRecording() || isShallow() ){
                  bytes = packInternal( string.c_str(), string.len() );
                }else{
                  bytes = cache( string );
                }
                return bytes;
              }

              /** @} */

            //}:                                  |
            //save:{                              |

              u64 save( ccp tag = nullptr );

            //}:                                  |
            //pad:{                               |

              e_forceinline cp pad( const u64 size ){
                e_guardw( m_tLock );
                return m_tStream.realloc( size );
              }

            //}:                                  |
            //is:{                                |

              e_forceinline bool isRecording()const{
                return( 1==m_tFlags->bRecording );
              }

              e_forceinline bool isHistoric()const{
                return( 1==m_tFlags->bHistoric );
              }

              e_forceinline bool isShallow()const{
                return( 1==m_tFlags->bShallow );
              }

            //}:                                  |
          //}:                                    |
          //--------------------------------------+-----------------------------

          Writer( const string& filename, const u32 uFlags );
          Writer( const stream& st, const u32 uFlags );
          Writer( const Writer& );
          Writer( Writer&& );
          Writer()
            : Writer( string::null, 0 )
          {}
          virtual~Writer();

        private:

          using PropertyPayload = std::pair<u64,vector<Object::handle>>;
          using PropertyObjects = std::shared_ptr<PropertyPayload>;
          using CacheWriter     = std::shared_ptr<Writer>;

          /* Private member functions */

          bool writePropertyHandle( const Object::prop_ptr& );
          bool writePropertyVector( const Object::prop_ptr& );
          bool writePropertyString( const Object::prop_ptr& );
          bool writePropertyArray ( const Object::prop_ptr& );
          void init( const u32 );
          u64 packInternal( ccp, const u64 );
          u64 compress( cp& );

          /* Engine facing vars */

          e_var_string_map( stream, StringTable );
          e_var_vector( u8,         Dictionary  );
          e_var_string(             Filename    );
          e_var_array(  u64,        Bitmap,  4  );
          e_var_string(             Tag         );
          e_var( stream, t,         Stream      );
          e_var_volatile_bits( Flags
              , bRenameSHA1:1
              , bImporting:1
              , bRecording:1  //!< Part of a historic stream (undo/redo)
              , bHistoric:1   //!< Part of a historic stream (detaching)
              , bScanning:1
              , bCompress:1
              , bShallow:1
              , bNoExt:1
              , bText:1 );

          /* Private vars */

          vector<Pending>            m_vPending;
          CacheWriter                m_pCache;
          atomic::ShareLockRecursive m_tLock;
        };
      }
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//IReader:{                                       |

#ifdef __APPLE__
  #pragma mark - IReader interface -
#endif

  namespace EON{
    namespace gfc{
      namespace fs{
        struct E_PUBLISH IReader{
          virtual void onRead( const string&/* filename */, const u64/* bytesWritten */, const u64/* streamLength */){}
          virtual void onOpen( const string&/* filename */){}
          virtual void onShut( const string&/* filename */){}
        };
      }
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Reader:{                                        |

#ifdef __APPLE__
  #pragma mark - Reader class -
#endif

  namespace EON{
    namespace gfc{
      namespace fs{

        /** \brief File system abstraction object.
          *
          * The FS class takes the work out of handling files on every
          * platform the engine supports.
          */

        struct E_PUBLISH Reader final{

          //--------------------------------------+-----------------------------
          //Structs:{                             |

            #if !e_compiling( web )
              template<typename T> struct E_PUBLISH Asynchronicity final:Thread{
            #else
              template<typename T> struct E_PUBLISH Asynchronicity final{
            #endif

              //----------------------------------+-----------------------------
              //Methods:{                         |
                //rejected:{                      |

                  bool rejected()const{
                    return( nullptr == m_pReader );
                  }

                //}:                              |
                //run:{                           |

                  int run(){

                    //----------------------------------------------------------
                    // Bail conditions.
                    //----------------------------------------------------------

                    if( rejected() ){
                      return 0;
                    }

                    //----------------------------------------------------------
                    // Load the object from its eon file.
                    //----------------------------------------------------------

                    const auto begms = e_seconds();
                    if( kVerbosity ){
                      e_msgf( "Streaming %s of class '%s'"
                        , ccp( m_pReader->toName() )
                        , m_tObject.classof()
                      );
                    }

                    //----------------------------------------------------------
                    // If no errors serialize the object and mark as IO complete
                    // otherwise mark object as I/O incomplete (error).
                    //----------------------------------------------------------

                    const auto UUID = m_tObject.UUID;
                    const auto& onError = [&]()->int{
                      if( m_pReader->isError() ){
                        m_tObject.toStatus()->bIOComplete = 1;
                        m_tObject.toStatus()->bIOError = 1;
                        if( !m_tObject.subref() ){
                          Class::Factory::erase( UUID );
                        }
                        return-1;
                      }
                      return 0;
                    };

                    //----------------------------------------------------------
                    // Serializing from reading stream.
                    //----------------------------------------------------------

                    m_tObject.preSerialize( *m_pReader );
                    if( onError() < 0 ){
                      return-1;
                    }
                    m_tObject.serialize( *m_pReader );
                    if( onError() < 0 ){
                      return-1;
                    }
                    m_tObject.postSerialize( *m_pReader );
                    if( onError() < 0 ){
                      return-1;
                    }

                    //----------------------------------------------------------
                    // We're considered I/O complete even though the callback
                    // lambda hasn't had a chance to monkey with the data.
                    //----------------------------------------------------------

                    string id;
                    if( m_tObject.isa( e_classid<Stream>() )){
                      auto& re = dynamic_cast<Stream&>( m_tObject );
                      id = m_pReader->toName( );
                      if( re.toName().empty() ){
                        re.setName( id );
                      }
                    }
                    m_tObject.toStatus()->bIOComplete = 1;
                    if( m_onLoaded ){
                      m_onLoaded( m_tObject );
                    }

                    //----------------------------------------------------------
                    // If the object went out of scope then just erase it.
                    //----------------------------------------------------------

                    const auto endms = e_seconds();
                    if( kVerbosity ){
                      if( m_tObject.isa( e_classid<Stream>() )){
                        const auto& id = dynamic_cast<Stream&>( m_tObject ).getStreamID();
                        e_msgf(
                          "  Stream \"%s\" took %.2fms"
                          , ccp( id )
                          , ( endms - begms ) * 1000.
                        );
                      }else{
                        e_msgf(
                          "  Stream $(red)\"unknown id\"($off) took $(green)%.2fms"
                          , ( endms - begms ) * 1000.
                        );
                      }
                    }
                    if( !m_tObject.subref() ){
                      Class::Factory::erase( UUID );
                    }
                    return 0;
                  }

                //}:                              |
              //}:                                |
              //----------------------------------+-----------------------------

              Asynchronicity(
                  const string&      tag
                , const OnLoaded<T>& lambda
                , Reader*            me
                , T&                 t )
                  : m_sTag(     tag    )
                  , m_onLoaded( lambda )
                  , m_tObject(  t      )
                  , m_pReader(  me     ){
                if( me->exists( tag )){
                  t.toStatus()->bIOComplete = 0;
                  t.addref();
                }else{
                  m_pReader = nullptr;
                }
              }

            ~ Asynchronicity() = default;
              Asynchronicity() = delete;

            private:

              OnLoaded<T> m_onLoaded;
              T&          m_tObject;
              Reader*     m_pReader = nullptr;
              string      m_sTag;
            };

          //}:                                    |
          //Operate:{                             |

            template<typename T> e_forceinline Reader& operator>>( vector<T>& v ){
              read( v );
              return *this;
            }

            template<typename T> e_forceinline Reader& operator>>( T& t ){
              read( t );
              return *this;
            }

            e_forceinline Reader& operator>>( stream& b ){
              read( b );
              return *this;
            }

            e_forceinline Reader& operator>>( buffer& b ){
              read( b );
              return *this;
            }

            e_forceinline Reader& operator>>( string& s ){
              read( s );
              return *this;
            }

            e_forceinline Reader& operator>>( ccp s ){
              read( s );
              return *this;
            }

            /** \brief Error operator.
              *
              * \return This operator will return true if there was no error in the
              * read and false otherwise.
              */

            e_forceinline operator bool()const{
              return !m_tFlags->bError;
            }

          //}:                                    |
          //Methods:{                             |
            //startAsynchronicity:{               |

              /** \name Super top secret code.
                *
                * This code should never be called by the user code.
                *
                * @{
                */

              /** \brief I/O completion method.
                *
                * This method should never be called by the user ever. It is
                * necessary for background loading.
                *
                * \return Returns true if a true asynchronous call was made and
                * false if we were already running in a background thread.
                */

              template<typename T> bool startAsynchronicity(
                    T&                 clsidObjective
                  , const OnLoaded<T>& onLoadedObject
                  , const bool         bDeletion=true ){
                auto* pThread = new Asynchronicity<T>(
                    clsidObjective.classof()
                  , onLoadedObject
                  , this
                  , clsidObjective );
                if( pThread->rejected() ){
                  if( bDeletion ){
                    delete this;
                  }
                  return false;
                }

                //--------------------------------------------------------------
                // The main thread clause here is for collapsing as many thread
                // objects as we can so we are not dealing with hundreds of the
                // buggers when we load a level into the game or editor.
                //--------------------------------------------------------------

                if( kMultiThreaded ){
                  #if !e_compiling( web )
                    pThread->autodelete()->start();
                    return true;
                  #endif
                }

                //--------------------------------------------------------------
                // Just run the thread without starting it.
                //--------------------------------------------------------------

                pThread->run();

                //--------------------------------------------------------------
                // We are releasing the thread even though we don't start it so
                // the thread flags and alive() function can be called properly
                // and in the proper places and times.
                //--------------------------------------------------------------

                #if !e_compiling( web )
                  pThread->release();
                #endif
                delete pThread;

                //--------------------------------------------------------------
                // Delete 'this' pointer if so instructed.
                //--------------------------------------------------------------

                if( bDeletion ){
                  delete this;
                }
                return false;
              }

            //}:                                  |
            //read*:{                             |

              /** \name Handle serializers.
                *
                * These methods are used for serializing handles of various
                * descriptions from disk.
                *
                * @{
                */

              /** \brief Serialize in an object.
                *
                * This routine will create a new object and serialize it in,
                * returning a handle to it.
                *
                * \tparam T The type of object to cast to upon returning.
                *
                * \param lambda An initialzation callback that lets you set up
                * state on the newly created object before serialize is called.
                *
                * \return Returns an object handle or zero if the head of the
                * stream was not a valid class identifier.
                */

              template<typename T> typename T::handle read( const std::function<void( T& )>& lambda ){
                typename T::handle hT = readHandle<T>();
                if( lambda ){
                  lambda( hT.cast() );
                }
                return hT;
              }

              /** \brief Serializee a handle from disk.
                *
                * This routine will essentially load a handle from disk and
                * populate it.
                *
                * \tparam T The type of the handle to load.
                *
                * \return Returns the handle of type T.
                */

              template<typename T> e_noinline AutoRef<T> readHandle( const std::function<void( T& )>& lambda=nullptr ){
                const auto clsid = as<u64>();
                const auto descr = Class::Factory::describe( clsid );
                if( !descr ){
                  if( kErrors ){
                    e_break( e_xfs(
                      "%s failed to load: clsid:$%llx!"
                      , T::classname()
                      , clsid
                    ));
                  }
                  e_break( "Undescribed class identifier!" );
                  return 0;
                }
                typename T::handle hResult = e_newt( clsid );
                auto& t = hResult.cast();
                t.toStatus()->bIOComplete = 0;
                t.preSerialize(  *this );
                t.serialize(     *this );
                t.postSerialize( *this );
                t.toStatus()->bIOComplete = 1;
                if( lambda ){
                  lambda( t );
                }
                return hResult;
              }

              /** \brief Follow asynchronously.
                *
                * This follows a breadcrumb link to another file. The second
                * file will be loaded asynchronously.
                *
                * \tparam T The type of the object to be loaded from the other
                * file. The extension for all files is "eon".
                *
                * \return Returns a new handle of type T immediately. If there
                * was a problem the autoref handle will contain zero. In other
                * words the handle will be invalid. Use the unary not operator
                * to test for the failure case.
                \code
                  Mesh::handle foo( fs::Writer& i ){
                    auto hMesh = i.attachAsyncStream<Mesh>();
                    if( !hMesh ){
                      e_error( 187165234, "Couldn't find mesh handle!" );
                    }
                    return hMesh;
                  }
                \endcode
                */

              template<typename T> e_noinline AutoRef<T> attachAsyncStream(
                  const std::function<void( T& )>& lambda=nullptr ){
                const auto& path = unpack();
                const auto hStream = e_uniqueAsyncStream<T>(
                    path
                  , lambda );
                if( !hStream ){
                  m_tFlags->bError = 1;
                  if( kErrors ){
                    e_msgf(
                      "attachAsyncStream<%s>: error loading \"%s\""
                      , T::classname()
                      , ccp( path )
                    );
                  }
                }
                return hStream;
              }

              /** @} */

            //}:                                  |
            //isError:{                           |

              /** \brief Is error check.
                *
                * This routine checks to see if there's an error in the stream.
                */

              e_forceinline bool isError()const{
                return( 1 == m_tFlags->bError );
              }

            //}:                                  |
            //version:{                           |

              /** \brief Check version.
                *
                * This routine will check the version and assert out if there's a
                * mismatch in the stream.
                */

              u16 version( const u16 ver );

            //}:                                  |
            //uncache:{                           |

            private:

              string uncache();

            public:

            //}:                                  |
            //unpack:{                            |

              /** \brief Unpack string from stream.
                *
                * This routine will unpack a packed string into the string
                * provided.
                *
                * \return Returns an unpacked string.
                */

              e_noinline string unpack(){
                if( !isPlayback() || isShallow() ){
                  return unpackInternal();
                }
                return uncache();
              }

              /** \brief Unpack string from stream.
                *
                * This routine will unpack a packed string into the string
                * provided.
                *
                * \param ou Returns an unpacked string here.
                */

              e_forceinline void unpack( string& ou ){
                ou = unpack();
              }

            //}:                                  |
            //exists:{                            |

              /** \brief Check if file exists.
                *
                * This routine will return true if the file exists or false.
                *
                * \param tag Check if tag exists.
                *
                * \return Returns true if the tag exists or false otherwise.
                */

              bool exists( const string& tag )const;

            //}:                                  |
            //merge:{                             |

              /** \brief Three-way merge of properties.
                *
                * This routine will perform a three-way merge of the properties in
                * hObject.
                *
                * \param hObject The object to receive the results of the merge and
                * from whom we get the property list.
                *
                * \param bfr The BeFore reader. It must have been recorded with the
                * same property list as is contained in hObject.
                *
                * \param chr The CHange reader. It must have been recorded with the
                * same property list as is contained in hObject.
                */

              void mergeProperties( const Object::handle& hObject, Reader& bfr, Reader& chr );

            //}:                                  |
            //error:{                             |

              /** \brief Set the error flag.
                *
                * This routine will set the error flag.
                */

              e_forceinline void error(){
                m_tFlags->bError = 1;
              }

            //}:                                  |
            //c_str:{                             |

              /** \brief Get C string.
                *
                * This code will return the current read position as a C string.
                */

              e_forceinline ccp c_str()const{
                ccp pResult = nullptr;
                m_tStream.query( m_tStream.size(),
                  [&]( ccp p ){
                    pResult = p;
                  }
                );
                return pResult;
              }

            //}:                                  |
            //load:{                              |

              /** \brief Load buffer.
                *
                * This routine will load a eon file and optionally decompress
                * it.
                *
                * \param tag If tag is nullptr then the file will be loaded as
                * is and no additional processing is done on it otherwise the
                * file must be a .eon format file and will be optionally
                * decompressed and checked against the tag.
                *
                * \return Returns a reference to the *this pointer.
                */

              Reader& load( const string& tag );

            //}:                                  |
            //eof:{                               |

              /** \brief Check if we reached the end of the stream.
                *
                * This routine will return true if there are no more bytes
                * available in the stream and false otherwise.
                */

              bool eof()const;

            //}:                                  |
            //read:{                              |

              /** \brief Read a hashmap of objects.
                *
                * This routine will read a hashmap of type T and return it.
                *
                * \tparam K The type of key to use in container.
                * \tparam V The type of object to read.
                * \param  m The hashmap to receive the objects.
                *
                * \return Returns the number of bytes read.
                */

              template<typename K,typename V> e_noinline u64 read( hashmap<K,V>& m ){
                u64 bytes = 0;
                u32 size;
                bytes += read( size );
                m.clear();
                K k;
                V v;
                for( u32 i=0; i<size; ++i ){
                  bytes += read( k );
                  bytes += read( v );
                  m.set( k, v );
                }
                return bytes;
              }

              /** \brief Read a map of objects.
                *
                * This routine will read a map of type T and return it.
                *
                * \tparam K The type of key to use in container.
                * \tparam V The type of object to read.
                * \param  m The map to receive the objects.
                *
                * \return Returns the number of bytes read.
                */

              template<typename K,typename V> e_noinline u64 read( map<K,V>& m ){
                u64 bytes = 0;
                u32 size;
                bytes += read( size );
                m.clear();
                K k;
                V v;
                for( u32 i=0; i<size; ++i ){
                  bytes += read( k );
                  bytes += read( v );
                  m.set( k, v );
                }
                return bytes;
              }

              /** \brief Read a pool of objects.
                *
                * This routine will read a pool of type T and return it.
                *
                * \tparam T The type of key to use in the pool..
                * \tparam N The number of elements in the pool.
                * \param  p The pool to receive the objects.
                *
                * \return Returns the number of bytes read.
                */

              template<typename T,u64 N> e_noinline u64 read( pool<T,N>& p ){
                u64 bytes = 0;
                u32 size;
                bytes += read( size );
                p.clear();
                T t;
                for( u32 i=0; i<size; ++i ){
                  bytes += read( t );
                  p += t;
                }
                return bytes;
              }

              /** \brief Read a array of objects.
                *
                * This routine will read a array of type T and return it.
                *
                * \tparam T The type of objects to read.
                * \param  a The array to receive the objects.
                *
                * \return Returns the number of bytes read.
                */

              template<typename T,u32 N> e_noinline u64 read( array<T,N>& a ){
                u64 bytes = 0;
                u32 size;
                bytes += read( size );
                bytes += read( &a[0], sizeof( T )*size );
                return bytes;
              }

              /** \brief Read a vector of objects.
                *
                * This routine will read a vector of type T and return it.
                *
                * \tparam T The type of objects to read.
                * \param  v The vector to receive the objects.
                *
                * \return Returns the number of bytes read.
                */

              template<typename T> e_noinline u64 read( vector<T>& v ){
                u64 bytes = 0;
                u32 size;
                bytes += read( size );
                v.clear();
                v.reserve( size );
                for( u32 n=size, i=0; i<n; ++i ){
                  T t;
                  read( t );
                  v.push( t );
                }
                return bytes;
              }

              /** \brief Read a deque of objects.
                *
                * This routine will read a deque of type T and return it.
                *
                * \tparam T The type of objects to read.
                * \param  d The deque to receive the objects.
                *
                * \return Returns the number of bytes read.
                */

              template<typename T> e_noinline u64 read( deque<T>& d ){
                u64 bytes = 0;
                u32 size;
                bytes += read( size );
                d.clear();
                T t;
                for( u32 i=0; i<size; ++i ){
                  bytes += read( t );
                  d.push( t );
                }
                return bytes;
              }

              /** \brief Read a templated type.
                *
                * This routine will read a type T and return it in 't'.
                *
                * \tparam T The type of object to read.
                * \param  t Will receive read value.
                *
                * \return Returns the number of bytes read.
                */

              template<typename T> e_forceinline u64 read( T& t ){
                const u64 bytes = sizeof( T );
                memcpy( vp( &t ), cvp( c_str() ), sizeof( T ));
                skip(  bytes );
                return bytes;
              }

              /** \brief Read a value and return it.
                *
                * This routine will read a value and return it.
                */

              template<typename T> e_forceinline T read(){
                T t;
                read( t );
                return t;
              }

              /** \brief Read size bytes from fs stream.
                *
                * This routine will read a given number of bytes into the buffer
                * provided.
                *
                * \param size The number of bytes to read into buffer.
                *
                * \return Returns size.
                */

              u64 read( const u64 offset, vp ptr, const u64 size );

              /** \brief Read size bytes from fs stream.
                *
                * This routine will read a given number of bytes into the buffer
                * provided.
                *
                * \param size The number of bytes to read into buffer.
                *
                * \return Returns size.
                */

              u64 read( vp ptr, const u64 size );

              /** \brief Read a string.
                *
                * This routine will read text into the string provided.
                *
                * \param s The string to be filled.
                *
                * \return Returns the number of bytes read.
                */

              e_forceinline u64 read( string& s ){
                s = c_str();
                const u64 bytes = s.len()+1;
                skip( bytes );
                return bytes;
              }

              /** \brief Read property from stream.
                *
                * This routine will read a property object from the stream.
                *
                * \param pProprty The property shared pointer to load with a brand
                * new instance of type.
                *
                * \return Returns the number of bytes read in.
                */

              u64 read( Object::prop_ptr& pProprty, const std::function<void( Object* )>& lambda=nullptr );

              /** \brief Read properties into object.
                *
                * This routine will read a list of properties into the given object
                * handle.
                *
                * \param hObject An autoref handle to the requested object. It
                * is non-const because we will replace it with a new object
                * during serialization.
                *
                * \return Returns the number of bytes streamed in.
                */

              u64 readProperties( Object::handle& hObject );

              /** \brief Read a stream.
                *
                * This routine will read a stream.
                *
                * \param st The stream to be replaced with data.
                *
                * \return Returns the number of bytes read.
                */

              u64 read( stream& st );

              /** \brief Read a buffer.
                *
                * This routine will read a buffer.
                *
                * \param b The buffer to be replaced with new data.
                *
                * \return Returns the number of bytes read.
                */

              u64 read( buffer& b );

            //}:                                  |
            //size:{                              |

              e_forceinline u64 size()const{
                return m_tStream.size();
              }

            //}:                                  |
            //skip:{                              |

              /** \brief Skip N bytes.
                *
                * This routine will skip N bytes of data.
                *
                * \param bytes The number of bytes to skip in the memory mapped
                * file.
                *
                * \return Returns true if the skip succeeded and false if the read
                * head went off the end of the file.
                */

              bool skip( const u64 bytes );

            //}:                                  |
            //as:{                                |

              /** \brief Type cast the read position.
                *
                * This routine will type cast the read pointer and return a
                * reference to it. This is especially useful when dealing with
                * objects because you can typecast to a u64 to get the class
                * identifier.
                *
                * \return Returns the type-cast T value.
                */

              template<typename T> const T& as()const{
                return *reinterpret_cast<const T*>( c_str() );
              }

            //}:                                  |
            //is:{                                |

              e_forceinline bool isPlayback()const{
                return( 1==m_tFlags->bPlayback );
              }

              e_forceinline bool isShallow()const{
                return( 1==m_tFlags->bShallow );
              }

            //}:                                  |
          //}:                                    |
          //--------------------------------------+-----------------------------

          /** \brief Reader constructor.
            *
            * This routine will initialize itself with the name provided. The
            * scratch buffer will be empty until load() is called.
            */

          explicit Reader( const string& name );

          /** \brief Reader copy constructor.
            *
            * This constructor copies the name and the stream.
            */

          Reader( const Reader& lvalue );

          /** \brief Reader move constructor.
            *
            * This constructor moves the name and the stream.
            */

          Reader( Reader&& rvalue );

          /** \brief Reader constructor with stream.
            *
            * This constructor will build a reader from an existing stream
            * object.
            */

          explicit Reader( const stream& st );
          Reader() = default;
        ~ Reader();

        private:

          //--------------------------------------+-----------------------------
          //Methods:{                             |
            //unpackInternal:{                    |

              e_noinline string unpackInternal(){

                //--------------------------------------------------------------
                // The first byte tells us size of dictionary or if it's a hex
                // string.
                //--------------------------------------------------------------

                const auto all = read<u8>();
                const auto hex = (255==all);
                const auto cst = (254==all);
                const auto nol = (0x0==all);

                //--------------------------------------------------------------
                // Bail on nil strings.
                //--------------------------------------------------------------

                if( nol ){
                  return nullptr;
                }

                //--------------------------------------------------------------
                // Construct a C string.
                //--------------------------------------------------------------

                string out;
                if( cst ){
                  read( out );
                  return out;
                }

                //--------------------------------------------------------------
                // Read the length of the string.
                //--------------------------------------------------------------

                union{
                  u32 x=0;
                  u8 a[4];
                } N;
                read( N.a, 3 );

                //--------------------------------------------------------------
                // Construct a hex string.
                //--------------------------------------------------------------

                if( hex ){
                  // Handle the many character case.
                  for( u32 n=N.x>>1, i=0; i<n; ++i ){
                    const u8 ch = read<u8>();
                    const u8 lo = ( ch & 0x0F );
                    const u8 hi = ( ch & 0xF0 ) >> 4;
                    if( hi > 9 ){
                      out += 'A' + ( hi - 10 );
                    }else{
                      out += '0' + hi;
                    }
                    if( lo > 9 ){
                      out += 'A' + ( lo - 10 );
                    }else{
                      out += '0' + lo;
                    }
                  }
                  // Handle single character case.
                  if( N.x & 1 ){
                    const u8 ch = read<u8>();
                    const u8 hi = ( ch & 0xF0 ) >> 4;
                    #if e_compiling( debug )
                      const u8 lo = ( ch & 0x0F );
                      e_assert( !lo, "Not a string!" );
                    #endif
                    if( hi > 9 ){
                      out += 'A' + ( hi - 10 );
                    }else{
                      out += '0' + hi;
                    }
                  }
                  return out;
                }

                //--------------------------------------------------------------
                // Construct a dictionary.
                //--------------------------------------------------------------

                if( m_vDictionary.empty() ){
                  e_unreachable( "You cannot unpack without a dictionary!" );
                }
                const u8 bits = all;
                const u64 in_bytes=(( N.x * bits + 7 ) & ~7 ) >> 3;
                ccp r = m_tStream.realloc( in_bytes );
                u64 ix = 0;
                for( u32 i=0; i<N.x; ++i ){
                  u8 ixdict = 0;
                  const u64 iy = ix / 8;
                  const u64 iz = ix % 8;
                  const u64 lomask = ((( 1 << bits )-1 ) << iz ) & 0xFF;
                  ixdict |= ( r[ iy ] & lomask ) >> iz;
                  const u64 iw = iz + bits;
                  if( iw > 8 ){
                    const u64 himask = ( 1 << ( iw-8 ))-1;
                    ixdict |= ( r[ iy+1 ] & himask ) << ( 8-iz );
                  }
                  out += m_vDictionary[ ixdict ];
                  ix += bits;
                }
                return out;
              }

            //}:                                  |
            //readProperty*:{                     |

              bool readPropertyVector( Object::prop_ptr& );
              bool readPropertyHandle( Object::prop_ptr& );
              bool readPropertyString( Object::prop_ptr& );
              bool readPropertyArray ( Object::prop_ptr& );

            //}:                                  |
            //decompress:{                        |

              u64 decompress( cp, const u64, cvp, const u64 );

            //}:                                  |
          //}:                                    |
          //--------------------------------------+-----------------------------

          e_var_string_map( stream, StringTable );
          e_var_vector( u8,         Dictionary  );
          e_var( stream, t,         Stream      );
          e_var_string(             Name        );
          e_var_bits(               Flags
            , bPlayback:1
            , bScanning:1
            , bPending:10
            , bShallow:1
            , bNoExt:1
            , bError:1
          );
        };
      }
    }
  }

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
