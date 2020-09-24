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

#include<sys/stat.h>
#include<lz4hc.h>
#include<lz4.h>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//AsyncLoader:{                                   |
  //Structs:{                                     |

    struct Profiling final{

      Profiling( const string& name )
        : m_sName( name )
      {}

    ~ Profiling(){
        const f64& term = e_seconds();
        const f64& time = term-m_fBeg;
        string s = m_sName;
        s.replace( "EON::gfc::fs::", "" );
        e_msgf( "%s%s%.9f", s.c_str(), string::spaces( 100-e_min( 100, int( s.len() ))), f64::cast( time ));
        if( time > 10. ){
          e_unreachable( "Read took too long!" );
        }
      }

    private:

      e_var_string( Name );
      e_var( f64, f, Beg ) = e_seconds();
    };

  //}:                                            |
  //Equates:{                                     |

    #if e_compiling( android )
      #define MULTI_THREADED 0
    #else
      #define MULTI_THREADED 1
    #endif

    #if e_compiling( debug )
      #define __compiling_light_logging__ 0
      #define __compiling_logging__       0
    #endif

    #define MASK_MODIFIED 1
    #define MASK_KEYFRAME 2
    #define MASK_HIDDEN   4
    #define TAB_SIZE      2

    #define USE_PROFILING 0

  //}:                                            |
  //Methods:{                                     |
    //run:{                                       |

#ifdef __APPLE__
  #pragma mark - AsyncLoader -
#endif

      int AsyncLoader::run(){

        // Load the object from its eon file.
        if( e_getCvar( bool, "USE_LOGGING" )){
          e_msgf( "Loading %s of type '%s'", ccp( m_pFS->toName() ), ccp( m_sTag ));
        }
        m_pFS->load( m_sTag );

        // If no errors serialize the object and mark as I/O complete otherwise
        // mark object as I/O incomplete (error).
        const s64 UUID = m_pObject->UUID;
        if( m_pFS->isError() ){
          e_assert( !"The IO error bit is set!" );
          m_pObject->toStatus()->bIOError = 1;
          delete m_pFS;
          return -1;
        }

        // Serializing from reading stream.
        m_pObject->preSerialize ( *m_pFS );
        m_pObject->serialize    ( *m_pFS );
        m_pObject->postSerialize( *m_pFS );

        // Every object MUST have at least a name.
        if( e_isa<Resource>( m_pObject )){
          auto* pResource = static_cast<Resource*>( m_pObject );
          if( pResource->toName().empty() ){
            pResource->setName( m_pFS->toName() );
          }
        }

        // Serialize could reallocate (e_renew).
        m_pObject = Class::Factory::pcast( UUID );

        // We're considered I/O complete even though the callback lambda
        // hasn't had a chance to monkey with the data.
        m_pObject->toStatus()->bIOComplete = 1;
        if( m_onLoaded ){
          m_onLoaded( m_pObject );
        }

        // If the object went out of scope then just erase it.
        if( !m_pObject->subref() ){
          Class::Factory::erase( UUID );
        }

        // Delete the file system object.
        delete m_pFS;

        // Return success.
        return 0;
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    AsyncLoader::AsyncLoader( const string& tag, const std::function<void(Object*)>& onLoaded, Reader* pfs, Object* pObject )
        : m_onLoaded( onLoaded )
        , m_pObject( pObject )
        , m_pFS( pfs )
        , m_sTag( tag ){
      m_pObject->addref();
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Writer:{                                        |
  //Private:{                                     |
    //writePropertyMap:{                          |

#ifdef __APPLE__
  #pragma mark - Private -
#endif

      namespace{
        using ObjectPair = std::pair<u64,Object::prop_ptr>;
        u64 writePropertyMap( Writer& fs, const Object::prop_map& out_mProperties ){
          u64 bytes = 0;
          if( !out_mProperties.empty() ){
            vector<ObjectPair> out_vProperties;
            out_mProperties.foreach(
              [&]( const Object::prop_ptr& pProperty ){
                if( pProperty ){
                  out_vProperties.push( std::make_pair( pProperty->toName().hash(), pProperty ));
                }
              }
            );
            out_vProperties.sort(
              []( const ObjectPair& a, const ObjectPair& b ){
                return( a.second->toName() < b.second->toName() );
              }
            );
            out_vProperties.foreach(
              [&]( ObjectPair& objPair ){
                #if e_compiling( light_logging )
                  printf( "%8x: WRITING %s\n"
                      , u32( fs.toStream().tell() )
                      , objPair.second->toName().c_str() );
                #endif
                bytes += fs.write( objPair.second );
              }
            );
          }
          return bytes;
        }
      }

    //}:                                          |
    //saveResource:{                              |

      namespace{

        void saveResource( Writer& fs, std::atomic<u32>& atPending, const Resource::handle& hResource, const u64 offset, const u32 flags ){

          // Optimization: typecast resource now up front. If the resource has
          // it's archived status bit set then e_save() will not write it out
          // to disk. It will block until the resource is not longer pending.
          Resource& resource = hResource.noconst().cast();
          resource.toStatus()->bUnavailable = 1;
          resource.blockUntilIOComplete();

          // Save the resource out to the right location.
          string sha1;
          if( fs.toFlags()->bImporting ){
            const string& path = IEngine::toPackagePath() + "src/data/" + resource.pathof();
            const string& retn = e_save<Resource>( path, resource, flags | kIMPORT );
            sha1 = retn.basename();
            #if e_compiling( debug )
              if( !sha1.is_sha1() ){
                e_unreachable( "Not an SHA1 key returned!" );
              }
            #endif
          }else{
            const string& path = IEngine::toPackagePath() + ".eon/";
            const string& retn = e_save<Resource>( path, resource, flags );
            sha1 = retn.basename();
            #if e_compiling( debug )
              if( !sha1.is_sha1() ){
                e_unreachable( "Not an SHA1 key returned!" );
              }
            #endif
          }

          // Ok now we can touch the resource letting others know.
          resource.toStatus()->bUnavailable = 0;
          resource.toStatus()->bIOTouched   = 1;
          resource.toStatus()->bArchived    = 1;

          // Overwrite 40 byte SHA1 key now that we know it.
          if( flags & kSHA1 ){
            fs.atomic( [&](){
              const u64 previous = fs.reset();
              fs.skip( offset );
              fs.pack( sha1 );
              fs.reset();
              fs.skip( previous );
            });
            --atPending;
          }
        }
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //packInternal:{                              |

#ifdef __APPLE__
  #pragma mark - Writer -
#endif

      u64 Writer::packInternal( ccp pSrc, const u64 bytes ){

        //----------------------------------------------------------------------
        // Handle empty strings.
        //----------------------------------------------------------------------

        // Make the entire packing step fully atomic.
        e_guardw( m_tLock );

        // If no bytes just write out zero for 'all'.
        if( !bytes ){
          return write( u8( 0 ));
        }

        //----------------------------------------------------------------------
        // Build the dictionary from the input string.
        //----------------------------------------------------------------------

        // Duplicate dictionary and bitmap so we don't unecessarily pollute.
        vector<u8> dictionary = m_vDictionary;
        array<u64,4> bitmap = m_aBitmap;

        // Now build the dictionary, counting hex chars.
        const u64 begin = m_tStream.size();
        u8 ixMaxDict = 0;
        u8 isHex = 1;{
          u8* r = (u8*)pSrc;
          u32 n = dictionary.size();
          dictionary.resize( n + bytes );
          dictionary.alter( 0, [&]( u8& _1st ){
            u8* dict = &_1st;
            while( *r ){
              if( isHex && !((( *r >= 'A' )&&( *r <= 'F' ))||(( *r >= '0' )&&( *r <= '9' )))){
                isHex = 0;
              }
              u64& field = bitmap[ *r / 64 ];
              const u64 mask = 1ULL << u64( *r % 64 );
              if( mask !=( field & mask )){
                ixMaxDict = e_max( ixMaxDict, u8( n & 0xFF ));
                dict[ n++ ]=*r;
                field |= mask;
              }else{
                for( u8 i=0; i<n; ++i ){
                  if( dict[ i ]==*r ){
                    ixMaxDict = e_max( ixMaxDict, i );
                    break;
                  }
                }
              }
              ++r;
            }
          });
          dictionary.resize( n );
        }
        const u8 dictSize = dictionary.empty() ? 0 : ixMaxDict+1;
        // byte 00 : FF means it's a hex string to follow.
        // byte 00 : FE means it's a C   string to follow.
        // byte 00 : 00 means no data to follow.
        u64 out = 0;
        if( isHex ){
          out += write<u8>( 0xFF );
        }else{
          if( dictSize >= 0xFE ){
            out += write<u8>( 0xFE );
            out += write( pSrc, bytes );
            out += write<u8>( 0 );
            return out;
          }
          if( !dictSize ){
            out += write( u8( 0 ));
            return out;
          }
          m_vDictionary = dictionary;
          m_aBitmap = bitmap;
        }

        //----------------------------------------------------------------------
        // Now write string with as few bits as possible.
        //----------------------------------------------------------------------

        // Calculate number of bits.
        u8 bits = 4;
        if( !isHex ){
          e_assert( dictSize < 254 );
          e_assert( dictSize > 0x0 );
          bits = Power::to2bsf<u32>( dictSize );
          out += write( bits );
        }

        // Write out a 24-bit size so header is 32-bit with bit count.
        e_assert( isHex || ( 1 << bits ) > ixMaxDict );
        e_assert( bytes < ( 1ULL << 24ULL ));
        u8* n = (u8*)m_tStream.realloc( 3 );
        u8* b = (u8*)&bytes;
        *n++ = *b++;
        *n++ = *b++;
        *n++ = *b++;
        out += 3;

        // Write out indexes into dictionary for every character.
        { u64 out_bytes=(( bytes * bits + 7 ) & ~7 ) >> 3;
          if( !out_bytes ){
            e_unreachable( "Empty string!" );
          }
          u8* w = (u8*)m_tStream.realloc( out_bytes );
          if( out_bytes > bytes+2 ){
            m_tStream.reset( begin );
            out = 0;
            out += write( u8( 0xFE ));
            out += write( pSrc, bytes );
            out += write( u8( 0 ));
            return out;
          }
          u8* r = (u8*)pSrc;
          if( isHex ){
            const u8* e = w + out_bytes;
            while( w < e ){
              u8& o = *w++;
              for( u8 j=0; j<2; ++j ){
                const u32 c = *r++;
                if( !r ){
                  break;
                }
                if(( c >= '0' )&&( c <= '9' )){
                  o |= ( c - '0' ) << (( 1-j ) * 4 );
                }else if(( c >= 'A' )&&( c <= 'F' )){
                  o |= ( 10 + c-'A' ) << (( 1-j ) * 4 );
                }else if(( c >= 'a' )&&( c <= 'f' )){
                  o |= ( 10 + c-'a' ) << (( 1-j ) * 4 );
                }
              }
            }
          }else{
            u64 ix = 0;
            dictionary.alter( 0,
              [&]( u8& _1st ){
                u8* dict = &_1st;
                memset( w, 0, out_bytes );
                while( *r ){
                  for( u64 i=0; i<u64( dictSize ); ++i ){
                    if( *r != dict[ i ]){
                      continue;
                    }
                    const u64 iy = ix / 8;
                    const u64 iz = ix % 8;
                    const u64 lomask=(( 1ULL<<( 8ULL-iz ))-1ULL );
                    w[ iy ]|=u8(( i & lomask )<<iz );
                    const u64 iw = iz + bits;
                    if( iw > 8ULL ){
                      const u64 himask=(( 1ULL<<( iw-8 ))-1 )<<( 8ULL-iz );
                      if( iy+1 >= out_bytes ){
                        // Handle the very rare case where iy+1 == out_bytes.
                        w = (u8*)m_tStream.realloc( ++out_bytes );
                      }
                      w[ iy+1 ]|=u8(( i & himask )>>( 8ULL-iz ));
                    }
                    break;
                  }
                  ix += bits;
                  ++r;
                }
              }
            );
          }
        }

        // Return byte count.
        return( m_tStream.size() - begin );
      }

    //}:                                          |
    //serialize:{                                 |

      u64 Writer::serializeResource( Resource& r ){
        if( m_tFlags->bRecording ){
          e_errorf( 625519, "Cannot record history and serialize resource." );
          return 0;
        }
        const u64 objectStarts = m_tStream.size();
        r.blockUntilIOComplete();
        r.preSerialize ( *this );
        r.serialize    ( *this );
        r.postSerialize( *this );
        const u64 objectStops = m_tStream.size();
        const u64 objectCount = ( objectStops - objectStarts ) * m_tStream.stride();
        const double then = e_seconds();
        while( m_atPending ){
          e_backoff( then );
        }
        atomic(
          [&](){
            m_tStream.query( objectStarts,
              [&]( ccp pBuffer ){
                if( m_tFlags->bRenameSHA1 ){
                  r.setSHA1( IEngine::sha1of( pBuffer, objectCount ));
                }
                m_tStream.reset( objectStarts );
                r.preSerialize( *this );
                m_tStream.reset( objectStops );
              }
            );
          }
        );
        return objectCount;
      }

    //}:                                          |
    //compress:{                                  |

      u64 Writer::compress( cp& pOut ){
        e_guardw( m_tLock );
        u64 zDst = LZ4_compressBound( int( used() ));
        u64 zSrc = used();
        cp  pDst = pad( zDst );
        u64 size = 0;
        m_tStream.query(
          [&]( ccp pBuffer ){
            cp pSrc = cp( pBuffer );
            if(( zDst <= ~0ULL )&&( zSrc <= ~0ULL )){
              size = LZ4_compress_HC(
                  pSrc
                , pDst
                , s32( zSrc )
                , s32( zDst )
                , LZ4HC_CLEVEL_MAX
              );
            }
          }
        );
        e_assert( size, "Compression failure!" );
        pOut = pDst;
        return size;
      }

    //}:                                          |
    //exports:{                                   |

      u64 Writer::exports( const AutoRef<Resource>& in_hResource, const u32 in_flags ){

        //----------------------------------------------------------------------
        // If resource is empty just write zero and bail out.
        //----------------------------------------------------------------------

        Resource::handle hResource = in_hResource;
        if( !hResource ){
          return write( u8( 0 ));
        }

        //----------------------------------------------------------------------
        // Otherwise write one and start profiling export.
        //----------------------------------------------------------------------

        const u64 before = used();
        write( u8( 1 ));

        //----------------------------------------------------------------------
        // Propigate flags.
        //----------------------------------------------------------------------

        u32 flags = in_flags;
        if( m_tFlags->bRenameSHA1 ){
          flags |= kSHA1;
        }
        if( m_tFlags->bCompress ){
          flags |= kCOMPRESS;
        }

        //----------------------------------------------------------------------
        // Serialize the resource directly into stream if building history.
        //----------------------------------------------------------------------

        if( m_tFlags->bRecording ){
          serializeHandle( hResource );
          return( used() - before );
        }

        //----------------------------------------------------------------------
        // If resource was archived then we can easy out.
        //----------------------------------------------------------------------

        const Resource& resource = hResource.cast();
        if( resource.toStatus()->bArchived ){
          if( flags & kSHA1 ){
            const double then = e_seconds();
            while( !resource.toStatus()->bIOTouched ){
              e_backoff( then );
            }
            const string& sha1 = resource.toSHA1();
            e_assert( sha1.is_sha1() );
            pack( sha1 );
          }else{
            pack( resource.toName() );
          }
          return( used() - before );
        }

        //----------------------------------------------------------------------
        // Otherwise asynchronously export resource out to disk.
        //----------------------------------------------------------------------

        // Write out dummy SHA1 key if flags allow.
        u64 offset = 0;
        if( flags & kSHA1 ){
          const string& sha1 = string::repeated( '0', 40 );
          offset = m_tStream.tell();
          ++m_atPending;
          pack( sha1 );
        }else{
          pack( resource.toName() );
        }

        // Increment pending atomic to cause save() and ~Writer() to stall.
        saveResource( *this, m_atPending, hResource, offset, flags );

        // Return bytes written.
        return( used() - before );
      }

    //}:                                          |
    //version:{                                   |

      void Writer::version( const u16 ver ){
        if( !m_tFlags->bText ){
          write( ver );
        }
      }

    //}:                                          |
    //write*:{                                    |
      //writePropertyHandle:{                     |

        bool Writer::writePropertyHandle( const Object::prop_ptr& pProperty, u64& bytes ){
          if( !pProperty->isHandle() || pProperty->isContainer() ){
            return false;
          }
          pProperty->queryAs<Object::handle>(
            [&]( const Object::handle& hObject ){
              if( !hObject ){
                write<u8>( 0 );
                return;
              }
              write<u8>( 1 );
              const Object& object = hObject.cast();
              object.blockUntilIOComplete();
              bytes += write( object.probeid( ) );
              bytes += writeProperties( hObject );
            }
          );
          return true;
        }

      //}:                                        |
      //writePropertyVector:{                     |

        bool Writer::writePropertyVector( const Object::prop_ptr& pProperty, u64& bytes ){
          if( !pProperty->isVector() ){
            return false;
          }

          //--------------------------------------------------------------------
          // Write string vectors.
          //--------------------------------------------------------------------

          if( pProperty->isType( e_classid<string>() )){
            pProperty->queryAs<vector<string>>(
              [&]( const vector<string>& lines ){
                bytes += write<u32>( lines.size() );
                lines.foreach(
                  [&]( const string& s ){
                    bytes += pack( s );
                  }
                );
              }
            );
            return true;
          }

          //--------------------------------------------------------------------
          // Write handle vectors.
          //--------------------------------------------------------------------

          if( pProperty->isHandle() ){
            pProperty->alterAs<vector<Object::handle>>(
              [&]( vector<Object::handle>& objects ){
                const auto& n = objects.size();
                write<u32>( n );
                objects.foreach(
                  [&]( Object::handle& hObject ){
                    writeProperties( hObject );
                  }
                );
              }
            );
            return true;
          }
          return false;
        }

      //}:                                        |
      //writePropertyString:{                     |

        bool Writer::writePropertyString( const Object::prop_ptr& pProperty, u64& bytes ){
          if( pProperty->isType( e_classid<string>() )){
            pProperty->queryAs<string>(
              [&]( const string& s ){
                bytes += pack( s );
              }
            );
            return true;
          }
          return false;
        }

      //}:                                        |
      //writePropertyArray:{                      |

        bool Writer::writePropertyArray( const Object::prop_ptr& pProperty, u64& bytes ){

          //--------------------------------------------------------------------
          // Bail conditions.
          //--------------------------------------------------------------------

          if( !pProperty->isArray() ){
            return false;
          }

          //--------------------------------------------------------------------
          // Write properties of all object handles in array.
          //--------------------------------------------------------------------

          if( pProperty->isHandle() ){
            pProperty->alterAs<array<Object::handle,1>>(
              [&]( array<Object::handle,1>& objects ){
                const u32 n = objects.size();
                write( n );
                objects.foreach(
                  [&]( Object::handle& hObject ){
                    writeProperties( hObject );
                  }
                );
              }
            );
            return true;
          }

          //--------------------------------------------------------------------
          // Write data for non-handle arrays.
          //--------------------------------------------------------------------

          pProperty->queryAs<array<char,1>>(
            [&]( const array<char,1>& objects ){
              const u32 capacity = objects.capacity();
              const u32 stride = objects.stride();
              bytes += write( capacity );
              bytes += write( stride );
              objects.query( 0,
                [&]( const char& buffer ){
                  write( &buffer, stride * capacity );
                }
              );
            }
          );
          return true;
        }

      //}:                                        |
      //writeProperties:{                         |

        u64 Writer::writeProperties( const Object::handle& in_hObject ){
          e_guardw( m_tLock );

          //--------------------------------------------------------------------
          // Add serialization guard.
          //
          // TODO: We should write zero if the object isn't shallow IO complete.
          //--------------------------------------------------------------------

          u64 bytes = 0;
          const Object::handle hObject( in_hObject );
          if( !hObject ){
            bytes += write<u8>( 0 );
          }else{
            bytes += write<u8>( 1 );

            //------------------------------------------------------------------
            // Go ahead and block until object IO complete.
            //
            // TODO: Do we want to block until IO complete shallow?
            //------------------------------------------------------------------

            const Object& object = hObject.cast();
            object.blockUntilIOComplete();

            //------------------------------------------------------------------
            // Write and defer objects.
            //------------------------------------------------------------------

            object.preSerialize(  *this );
            Object::prop_map             out_mProperties;
            object.getProperties(        out_mProperties );
            writePropertyMap(     *this, out_mProperties );
            object.postSerialize( *this );
          }
          return bytes;
        }

      //}:                                        |
      //write:{                                   |

        u64 Writer::write( const Object::prop_ptr& pProperty ){
          e_guardw( m_tLock );

          //--------------------------------------------------------------------
          // Bail condition.
          //--------------------------------------------------------------------

          if( !pProperty ){
            return 0;
          }

          //--------------------------------------------------------------------
          // Bail condition: store stream position and bail if ignored. Flag is
          // useful for turning off properties you want the user to see but not
          // be serialized in the data. The SHOW states of the Scene class for
          // example should never be serialized but are always shown. This is
          // the perfect case.
          //--------------------------------------------------------------------

          if( pProperty->isIgnored() ){
            #if e_compiling( light_logging )
              printf( "\t\t(ignored)\n" );
            #endif
            return 0;
          }

          //--------------------------------------------------------------------
          // Pack property name.
          //--------------------------------------------------------------------

          pack( pProperty->toName() );

          //--------------------------------------------------------------------
          // Save off the property type and the property name then write 64-bit
          // zero so we can skip past all this property's data while reading.
          //--------------------------------------------------------------------

          if( pProperty->isCaption() ){
            return writePropertyMap( *this, pProperty->toChildren() );
          }

          //--------------------------------------------------------------------
          // Save off most important flags.
          //--------------------------------------------------------------------

          u64 bytes = 0;
          u8  mask  = 0;
          if( pProperty->toFlags()->bKeyframe ){
            mask |= MASK_KEYFRAME;
            mask |= MASK_MODIFIED;
          }
          if( pProperty->toFlags()->bHidden ){
            mask |= MASK_HIDDEN;
          }
          bytes += write( mask );

          //--------------------------------------------------------------------
          // Write out properties.
          //--------------------------------------------------------------------

          if( writePropertyVector( pProperty, bytes )){ goto sk; }
          if( writePropertyHandle( pProperty, bytes )){ goto sk; }
          if( writePropertyString( pProperty, bytes )){ goto sk; }
          if( writePropertyArray(  pProperty, bytes )){ goto sk; }

          //--------------------------------------------------------------------
          // Write other types.
          //--------------------------------------------------------------------

          pProperty->query(
            [&]( cvp data, const u32 size ){
              bytes += write( data, size );
            }
          );
          bytes += writePropertyMap( *this, pProperty->toChildren() );

          //--------------------------------------------------------------------
          // Write out children first.
          //--------------------------------------------------------------------

sk:       return bytes;
        }

        u64 Writer::write( cvp ptr, const u64 size ){
          if( m_tFlags->bText ){
            return 0;
          }
          if( !ptr ){
            return 0;
          }
          e_guardw( m_tLock );
          const u64 bytes = m_tStream.bytes();
          cp pOut = m_tStream.realloc( size );
          memcpy( pOut, ptr, size * m_tStream.stride() );
          const u64 wrote =( m_tStream.bytes() - bytes );
          //listen<IWriter>::trigger( &IWriter::onWrite, wrote );
          return wrote;
        }

        u64 Writer::write( const stream& st ){
          e_guardw( m_tLock );
          u64 bytes = 0;
          if( !m_tFlags->bText ){
            bytes += write( st.stride() );
            bytes += write( st.size() );
            if( !st.empty() ){
              bytes += write( u8( 1 ));
              st.query(
                [&]( ccp pBuffer ){
                  bytes += write( pBuffer, st.bytes() );
                }
              );
            }else{
              bytes += write( u8( 0 ));
            }
          }
          return bytes;
        }

        u64 Writer::write( const buffer& buffer ){
          e_guardw( m_tLock );
          u64 bytes = 0;
          if( !m_tFlags->bText ){
            bytes += write( buffer.stride() );
            bytes += write( buffer.size() );
            if( buffer.data() ){
              bytes += write( u8( 1 ));
              bytes += write(
                buffer.data(),
                buffer.bytes()
              );
            }else{
              bytes += write( u8( 0 ));
            }
          }
          return bytes;
        }

        u64 Writer::write( const string& s ){
          e_guardw( m_tLock );
          u64 l = s.len();
          cp pOut = m_tStream.realloc( l );
          memcpy( pOut, s.c_str(), l );
          if( !m_tFlags->bText ){
            l += write( u8( 0 ));
          }
          return l;
        }

        u64 Writer::write( ccp pString ){
          e_guardw( m_tLock );
          u64 l = strlen( pString );
          cp pOut = m_tStream.realloc( l );
          memcpy( pOut, pString, l );
          if( !m_tFlags->bText ){
            l += write( u8( 0 ));
          }
          return l;
        }

      //}:                                        |
    //}:                                          |
    //cache:{                                     |

      u64 Writer::cache( ccp cString ){
        if( cString ){
          if( !m_pCache ){
            m_pCache = std::make_shared<Writer>();
          }
          const u64 start = m_tStream.tell();
          const u64 key64 = e_hashstr64( cString );
          const u64 packs = m_pCache->m_tStream.tell();
          const u64 bytes = m_pCache->packInternal( cString, strlen( cString ));
          if( !bytes ){
            write<u64>( 0 );
          }else{
            stream st;
            m_pCache->m_tStream.query( packs,
              [&]( ccp pBuffer ){
                memcpy( st.alloc( bytes ), pBuffer, bytes );
              }
            );
            m_mStringTable.set( key64, std::move( st ));
            write( key64 );
          }
          return( m_tStream.tell() - start );
        }
        write<u64>( 0 );
        return 0;
      }

    //}:                                          |
    //skip:{                                      |

      u64 Writer::skip( const u64 size ){
        e_guardw( m_tLock );
        if( size ){
          m_tStream.realloc( size );
        }
        return( size * m_tStream.stride() );
      }

    //}:                                          |
    //save:{                                      |

      u64 Writer::save( ccp tag ){

        //----------------------------------------------------------------------
        // Block until all exports complete.
        //----------------------------------------------------------------------

        const double then = e_seconds();
        while( m_atPending ){
          e_backoff( then );
        }

        //----------------------------------------------------------------------
        // Rename red file to SHA1 of uncompressed data.
        //----------------------------------------------------------------------

        bool bHashing = false;{
          e_guardr( m_tLock );
          if( m_tFlags->bRenameSHA1 ){
            const string& basename = m_sFilename.basename();
            const bool bHashed=(( basename.len() == 40 ) && basename.is_hex() );
            if( !bHashed ){
              m_sFilename = m_sFilename.path() + IEngine::sha1of( m_tStream ) + ".eon";
              bHashing = true;
            }
          }
        }

        //----------------------------------------------------------------------
        // Append the dictionary.
        //----------------------------------------------------------------------

        // Dangerous territory so get a more substantial lock.
        e_guardw( m_tLock );

        // If there's a tag append dictionary to eof.
        if( tag && !m_tFlags->bText ){
          m_vDictionary.query( 0,
            [&]( const u8& a ){
              write( &a, m_vDictionary.size() );
              write<u8>( m_vDictionary.size() );
            }
          );
        }

        //----------------------------------------------------------------------
        // Compress file data.
        //----------------------------------------------------------------------

        // Now we can compress the daylights out of it.
        const u64 slen = used();
        u64 dlen;
        cp dbuf;
        if( tag && m_tFlags->bCompress ){
          dlen = compress( dbuf );
          if( dlen >= slen ){//handle negative compression.
            m_tStream.query(
              [&]( ccp pBuffer ){
                dbuf = cp( pBuffer );
              }
            );
            dlen = slen;
          }
        }else{
          m_tStream.query(
            [&]( ccp pBuffer ){
              dbuf = cp( pBuffer );
            }
          );
          dlen = slen;
        }

        //----------------------------------------------------------------------
        // Commit to hardfile
        //----------------------------------------------------------------------

        string filename;
        if( bHashing ){
          filename = m_sFilename;
        }else{
          filename = m_sFilename;
          const string& ext = filename.ext();
          if( tag && ext.empty() ){
            filename += ".eon";
          }
        }
        auto* f = e_fopen( filename, "wb" );
        if( ! f ){
          e_errorf( 181273, "Couldn't save %s", ccp( filename ));
          return 0;
        }
        auto bytes = 0ull;
        if( tag ){
          bytes += fwrite( tag,   1, strlen( tag  ), f );
          bytes += fwrite( &slen, 1, sizeof( slen ), f );//uncompressed
          bytes += fwrite( &dlen, 1, sizeof( dlen ), f );//compressed
        }
        fwrite( dbuf, 1, dlen, f );
        m_sFilename = std::move(
           filename );
        fclose( f );
        return dlen;
      }

    //}:                                          |
    //init:{                                      |

      void Writer::init( const u32 uFlags ){
        if( uFlags & kCOMPRESS ){
          m_tFlags->bCompress = 1;
        }
        if( uFlags & kIMPORT ){
          m_tFlags->bImporting = 1;
        }
        if( uFlags & kTEXT ){
          m_tFlags->bText = 1;
        }
        if( uFlags & kSHA1 ){
          m_tFlags->bRenameSHA1 = 1;
        }
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    Writer::Writer( const string& filename, const u32 uFlags )
        : m_sFilename( filename ){
      init( uFlags );
      //listen<IWriter>::trigger( &IWriter::onOpen, filename );
    }

    Writer::Writer( const stream& st, const u32 uFlags )
        : m_tStream( st ){
      init( uFlags );
      //listen<IWriter>::trigger( &IWriter::onOpen, "" );
    }

  //}:                                            |
  //Dtor:{                                        |

    Writer::~Writer(){
      const double then = e_seconds();
      while( m_atPending ){
        e_backoff( then );
      }
      //listen<IWriter>::trigger( &IWriter::onShut );
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Reader:{                                        |
  //Private:{                                     |
    //getPrefabFilePointer:{                      |

#ifdef __APPLE__
  #pragma mark - Private -
#endif

      namespace{
        e_noinline FILE* getPrefabFilePointer( const string& name ){
          FILE* pFile = nullptr;
          bool bContinue = true;
          IEngine::prefabs.foreachs(
            [&]( Prefab::handle& hPrefab ){
              const Prefab& prefab = hPrefab.cast();
              prefab.toFiles().foreachs(
                [&]( const Prefab::File& F ){
                  if( F.toName() == name ){
                    pFile = e_fopen( prefab.toPath(), "rb" );
                    if( pFile ){
                      #if e_compiling( logging )
                        printf( "Found %s in prefab, seeking to %llu\n", F.toName().c_str(), F.toBase() );
                      #endif
                      fseek( pFile, F.toBase(), SEEK_SET );
                    }
                    bContinue = false;
                  }
                  return bContinue;
                }
              );
              return bContinue;
            }
          );
          return pFile;
        }
      }

    //}:                                          |
    //readPropertyMap:{                           |

      namespace{
        using ObjectPair = std::pair<u64,Object::prop_ptr>;
        u64 readPropertyMap( Reader& fs, Object::prop_map& out_mProperties ){
          u64 bytes = 0;
          if( !out_mProperties.empty() ){
            vector<ObjectPair> out_vProperties;
            out_mProperties.foreach(
              [&]( Object::prop_ptr& pProperty ){
                if( pProperty ){
                  out_vProperties.push( std::make_pair( pProperty->toName().hash(), pProperty ));
                }
              }
            );
            out_vProperties.sort(
              []( const ObjectPair& a, const ObjectPair& b ){
                return( a.second->toName() < b.second->toName() );
              }
            );
            out_vProperties.foreach(
              [&]( ObjectPair& objPair ){
                #if e_compiling( light_logging )
                  printf( "%8x: READING %s\n"
                      , u32( fs.toStream().tell() )
                      , objPair.second->toName().c_str() );
                #endif
                bytes += fs.read( objPair.second );
              }
            );
          }
          return bytes;
        }
      }

    //}:                                          |
    //getFilePointer:{                            |

      namespace{
        e_noinline FILE* getFilePointer( const string& name, const string& tag ){
          FILE* pFile = e_fopen( name, "rb" );
          if( !pFile ){
            string eonPath;
            if( tag.empty() ){
              eonPath = e_packagePath();
            }else{
              eonPath = e_eonPath();
            }
            eonPath += name;
            if( !tag.empty() &&( name.ext().tolower() != ".eon" )){
              eonPath += ".eon";
            }
            pFile = e_fopen( eonPath, "rb" );
            if( !pFile ){
              string resPath = IEngine::toResourcePath() + name;
              if( !tag.empty() &&( name.ext() != ".eon" )){
                resPath += ".eon";
              }
              pFile = e_fopen( resPath, "rb" );
              if( !pFile ){
                #if VERBOSE
                  e_logf( "Cannot load %s or %s", respath.c_str(), name.c_str() );
                #endif
                return nullptr;
              }
              #if VERBOSE
                e_logf( "Serializing %s", respath.c_str() );
              #endif
            }else{
              #if VERBOSE
                e_logf( "Serializing %s", name.c_str() );
              #endif
            }
          }else{
            #if VERBOSE
              e_logf( "Serializing %s", name.c_str() );
            #endif
          }
          return pFile;
        }
      }

    //}:                                          |
    //deepCompare:{                               |

      namespace{
        bool deepCompare( const Object::handle&, const Object::handle& );
        bool deepCompare( const Object::prop_ptr& ch_pProperty, const Object::prop_ptr& af_pProperty ){

          //--------------------------------------------------------------------
          // Handle the caption property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isCaption() ){
            bool bResult = true;
            auto cit = ch_pProperty->toChildren().getIterator();
            auto ait = af_pProperty->toChildren().getIterator();
            while( cit && ait ){
              if( !deepCompare( *cit, *ait )){
                bResult = false;
                break;
              }
              ++cit;
              ++ait;
            }
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle the Vector property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isVector() ){

            //------------------------------------------------------------------
            // Vector of handles case.
            //------------------------------------------------------------------

            if( ch_pProperty->isHandle() ){
              bool bResult = true;
              ch_pProperty->  alterAs<vector<Object::handle>>(
                [&]( vector<Object::handle>& ch_vObjects ){
                  af_pProperty->alterAs<vector<Object::handle>>(
                    [&]( vector<Object::handle>& af_vObjects ){
                      auto cit = ch_vObjects.getIterator();
                      auto ait = af_vObjects.getIterator();
                      while( cit && ait ){
                        if( !deepCompare( *cit, *ait )){
                          bResult = false;
                          break;
                        }
                        ++cit;
                        ++ait;
                      }
                    }
                  );
                }
              );
              return bResult;
            }

            //------------------------------------------------------------------
            // Vector of types case.
            //------------------------------------------------------------------

            //TODO: Need to have a callback method to help resolve this case.
            return false;
          }

          //--------------------------------------------------------------------
          // Handle the Array property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isArray() ){

            //------------------------------------------------------------------
            // Array of handles case.
            //------------------------------------------------------------------

            if( ch_pProperty->isHandle() ){
              bool bResult = true;
              ch_pProperty->  alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& ch_aObjects ){
                af_pProperty->alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& af_aObjects ){
                  for( u32 n=ch_aObjects.capacity(), i=0; i<n; ++i ){
                    if( !deepCompare( ch_aObjects[ i ], af_aObjects[ i ])){
                      bResult = false;
                      return;
                    }
                  }
                });
              });
              return bResult;
            }

            //------------------------------------------------------------------
            // Array of types case.
            //------------------------------------------------------------------

            bool bResult = true;
            ch_pProperty->  alterAs<array<char,1>>( [&]( array<char,1>& ch_aObjects ){
              af_pProperty->alterAs<array<char,1>>( [&]( array<char,1>& af_aObjects ){
                for( u32 n=ch_aObjects.capacity(), i=0; i<n; ++i ){
                  if( memcmp( &ch_aObjects[ i ], &af_aObjects[ i ], af_aObjects.stride() )){
                    bResult = false;
                    break;
                  }
                }
              });
            });
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle the Handle property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isHandle() ){
            bool bResult = true;
            ch_pProperty->  alterAs<Object::handle>( [&]( Object::handle& ch_hObject ){
              af_pProperty->alterAs<Object::handle>( [&]( Object::handle& af_hObject ){
                if( !deepCompare( ch_hObject, af_hObject )){
                  bResult = false;
                }
              });
            });
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle the String property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isType( e_classid<string>() )){
            bool bResult = true;
            ch_pProperty->  alterAs<string>( [&]( string& ch_string ){
              af_pProperty->alterAs<string>( [&]( string& af_string ){
                if( ch_string != af_string ){
                  bResult = false;
                }
              });
            });
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle all other property types.
          //--------------------------------------------------------------------

          bool bResult = true;
          ch_pProperty->  query( [&]( cvp ch_pData, const u32 ch_uSize ){
            af_pProperty->query( [&]( cvp af_pData, const u32 af_uSize ){
              if( memcmp( ch_pData, af_pData, af_uSize )){
                bResult = false;
              }
            });
          });
          return bResult;
        }

        bool deepCompare( const Object::handle& hObjectA, const Object::handle& hObjectB ){
          if( hObjectA == hObjectB ){
            return true;
          }
          if( hObjectA->probeid() != hObjectB->probeid() ){
            return false;
          }

          //--------------------------------------------------------------------
          // Compare all properties recursively.
          //--------------------------------------------------------------------

          Object::prop_map         vOutA;
          hObjectA->getProperties( vOutA );
          auto itA = vOutA.getIterator();
          Object::prop_map         vOutB;
          hObjectB->getProperties( vOutB );
          auto itB = vOutB.getIterator();
          while( itA && itB ){
            if( !deepCompare( *itA, *itB )){
              return false;
            }
            ++itA;
            ++itB;
          }
          return true;
        }
      }

    //}:                                          |
    //merge*:{                                    |

      namespace{
        void twoWayMerge(
            Object::prop_map::iterator& ch_it
          , Object::prop_map::iterator& af_it
        );
      }

      namespace{

        void threeWayMerge(
              Object::prop_map::iterator& bf_it
            , Object::prop_map::iterator& ch_it
            , Object::prop_map::iterator& af_it ){

          //--------------------------------------------------------------------
          // Handle all three good iterators.
          //--------------------------------------------------------------------

          while( bf_it && ch_it && af_it ){

            Object::prop_ptr bf_pProperty = *bf_it++;
            Object::prop_ptr ch_pProperty = *ch_it++;
            Object::prop_ptr af_pProperty = *af_it++;

            //------------------------------------------------------------------
            // Handle the caption property type.
            //------------------------------------------------------------------

            if( bf_pProperty->isCaption() ){
              auto bit = bf_pProperty->toChildren().getIterator();
              auto cit = ch_pProperty->toChildren().getIterator();
              auto ait = af_pProperty->toChildren().getIterator();
              threeWayMerge( bit, cit, ait );
              continue;
            }

            //------------------------------------------------------------------
            // Handle the Vector property type.
            //------------------------------------------------------------------

            if( bf_pProperty->isVector() ){

              //----------------------------------------------------------------
              // Vector of handles case.
              //----------------------------------------------------------------

              if( bf_pProperty->isHandle() ){
                bf_pProperty->    alterAs<vector<Object::handle>>( [&]( vector<Object::handle>& bf_vObjects ){
                  ch_pProperty->  alterAs<vector<Object::handle>>( [&]( vector<Object::handle>& ch_vObjects ){
                    af_pProperty->alterAs<vector<Object::handle>>( [&]( vector<Object::handle>& af_vObjects ){

                      auto bf_it = bf_vObjects.getIterator();
                      auto ch_it = ch_vObjects.getIterator();
                      auto af_it = af_vObjects.getIterator();

                      //--------------------------------------------------------
                      // Inserting a new vector between two empty vectors.
                      //--------------------------------------------------------

                      if( !af_it ){
                        if( ch_it ){
                          af_vObjects = ch_vObjects;
                        }else if( bf_it ){
                          af_vObjects = bf_vObjects;
                        }
                        return;
                      }

                      //--------------------------------------------------------
                      // Add object from CHanges vector.
                      //--------------------------------------------------------

                      if( ch_it ){
                        vector<Object::handle> temp;
                        while( ch_it ){
                          bool bFound = false;
                          while( af_it ){
                            if( deepCompare( *ch_it, *af_it )){
                              bFound = true;
                              break;
                            }
                            ++af_it;
                          }
                          if( !bFound ){
                            temp += *ch_it;
                          }
                          af_it = af_vObjects.getIterator();
                          ++ch_it;
                        }
                        af_vObjects.pushVector( temp );
                        return;
                      }

                      //--------------------------------------------------------
                      // Add object from BeFore vector.
                      //--------------------------------------------------------

                      if( bf_it ){
                        vector<Object::handle> temp;
                        while( bf_it ){
                          bool bFound = false;
                          while( af_it ){
                            if( deepCompare( *bf_it, *af_it )){
                              bFound = true;
                              break;
                            }
                            ++af_it;
                          }
                          if( !bFound ){
                            temp += *bf_it;
                          }
                          af_it = af_vObjects.getIterator();
                          ++bf_it;
                        }
                        af_vObjects.pushVector( temp );
                        return;
                      }

                      //--------------------------------------------------------
                      // Just clear the objects vector.
                      //--------------------------------------------------------

                      af_vObjects.clear();
                    });
                  });
                });
                continue;
              }

              //----------------------------------------------------------------
              // Vector of types case.
              //----------------------------------------------------------------

              continue;
            }

            //------------------------------------------------------------------
            // Handle the Array property type.
            //------------------------------------------------------------------

            if( bf_pProperty->isArray() ){

              //----------------------------------------------------------------
              // Array of handles case.
              //----------------------------------------------------------------

              if( bf_pProperty->isHandle() ){
                bf_pProperty->    alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& bf_aObjects ){
                  ch_pProperty->  alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& ch_aObjects ){
                    af_pProperty->alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& af_aObjects ){
                      Object::handle* bf_pObject = &bf_aObjects[0];
                      Object::handle* bf_eObject = bf_pObject + bf_aObjects.capacity();
                      Object::handle* ch_pObject = &ch_aObjects[0];
                      Object::handle* ch_eObject = ch_pObject + ch_aObjects.capacity();
                      Object::handle* af_pObject = &af_aObjects[0];
                      Object::handle* af_eObject = af_pObject + af_aObjects.capacity();
                      while(( bf_pObject < bf_eObject ) && ( ch_pObject < ch_eObject ) && ( af_pObject < af_eObject )){
                        Object::prop_map bf_props;
                        Object::prop_map ch_props;
                        Object::prop_map af_props;
                        if( *bf_pObject ){
                          (*bf_pObject++)->getProperties( bf_props );
                        }else{
                          ++bf_pObject;
                        }
                        if( *ch_pObject ){
                          (*ch_pObject++)->getProperties( ch_props );
                        }else{
                          ++ch_pObject;
                        }
                        if( *af_pObject ){
                          (*af_pObject++)->getProperties( af_props );
                        }else{
                          ++af_pObject;
                        }
                        auto bit = bf_props.getIterator();
                        auto cit = ch_props.getIterator();
                        auto ait = af_props.getIterator();
                        threeWayMerge( bit, cit, ait );
                      }
                      while(( bf_pObject < bf_eObject ) && ( af_pObject < af_eObject )){
                        Object::prop_map bf_props;
                        Object::prop_map af_props;
                        if( *bf_pObject ){
                          (*bf_pObject++)->getProperties( bf_props );
                        }else{
                          ++bf_pObject;
                        }
                        if( *af_pObject ){
                          (*af_pObject++)->getProperties( af_props );
                        }else{
                          ++af_pObject;
                        }
                        auto bit = bf_props.getIterator();
                        auto ait = af_props.getIterator();
                        twoWayMerge( bit, ait );
                      }
                      while(( ch_pObject < ch_eObject ) && ( af_pObject < af_eObject )){
                        Object::prop_map ch_props;
                        Object::prop_map af_props;
                        if( *ch_pObject ){
                          (*ch_pObject++)->getProperties( ch_props );
                        }else{
                          ++ch_pObject;
                        }
                        if( *af_pObject ){
                          (*af_pObject++)->getProperties( af_props );
                        }else{
                          ++af_pObject;
                        }
                        auto cit = ch_props.getIterator();
                        auto ait = af_props.getIterator();
                        twoWayMerge( cit, ait );
                      }
                    });
                  });
                });
                continue;
              }

              //----------------------------------------------------------------
              // Array of types case.
              //----------------------------------------------------------------

              bf_pProperty->    alterAs<array<char,1>>( [&]( array<char,1>& bf_aObjects ){
                ch_pProperty->  alterAs<array<char,1>>( [&]( array<char,1>& ch_aObjects ){
                  af_pProperty->alterAs<array<char,1>>( [&]( array<char,1>& af_aObjects ){
                    cp bf_pBuffer = &bf_aObjects[ 0 ];
                    cp ch_pBuffer = &ch_aObjects[ 0 ];
                    cp af_pBuffer = &af_aObjects[ 0 ];
                    for( u32 n=ch_aObjects.capacity(), i=0; i<n; ++i ){
                      if( !memcmp( bf_pBuffer, af_pBuffer, af_aObjects.stride() ) && memcmp( ch_pBuffer, af_pBuffer, af_aObjects.stride() )){
                        memcpy( af_pBuffer, ch_pBuffer, ch_aObjects.stride() );
                      }
                      bf_pBuffer += bf_aObjects.stride();
                      ch_pBuffer += ch_aObjects.stride();
                      af_pBuffer += af_aObjects.stride();
                    }
                  });
                });
              });
              continue;
            }

            //------------------------------------------------------------------
            // Handle the Handle property type.
            //------------------------------------------------------------------

            if( bf_pProperty->isHandle() ){
              bf_pProperty->    alterAs<Object::handle>( [&]( Object::handle& bf_hObject ){
                ch_pProperty->  alterAs<Object::handle>( [&]( Object::handle& ch_hObject ){
                  af_pProperty->alterAs<Object::handle>( [&]( Object::handle& af_hObject ){
                    if( !af_hObject ){
                      if( ch_hObject ){
                        af_hObject = ch_hObject;
                      }
                      return;
                    }
                    if( !bf_hObject || !ch_hObject ){
                      return;
                    }
                    Object::prop_map bf_props;
                    Object::prop_map ch_props;
                    Object::prop_map af_props;
                    if( bf_hObject ){
                      bf_hObject->getProperties( bf_props );
                    }
                    if( ch_hObject ){
                      ch_hObject->getProperties( ch_props );
                    }
                    af_hObject->getProperties( af_props );
                    auto bit = bf_props.getIterator();
                    auto cit = ch_props.getIterator();
                    auto ait = af_props.getIterator();
                    threeWayMerge( bit, cit, ait );
                  });
                });
              });
              continue;
            }

            //------------------------------------------------------------------
            // Handle the String property type.
            //------------------------------------------------------------------

            if( bf_pProperty->isType( e_classid<string>() )){
              bf_pProperty->    alterAs<string>( [&]( string& bf_string ){
                ch_pProperty->  alterAs<string>( [&]( string& ch_string ){
                  af_pProperty->alterAs<string>( [&]( string& af_string ){
                    if( bf_string == af_string && ch_string != af_string ){
                      af_string = ch_string;
                    }
                  });
                });
              });
              continue;
            }

            //------------------------------------------------------------------
            // Handle all other property types.
            //------------------------------------------------------------------

            bf_pProperty->    query( [&]( cvp bf_pData, const u32 bf_uSize ){
              ch_pProperty->  query( [&]( cvp ch_pData, const u32 ch_uSize ){
                af_pProperty->query( [&]( cvp af_pData, const u32 af_uSize ){
                  if( !memcmp( bf_pData, af_pData, af_uSize ) && memcmp( ch_pData, af_pData, af_uSize )){
                    memcpy( vp( af_pData ), ch_pData, ch_uSize );
                  }
                });
              });
            });
          }

          //--------------------------------------------------------------------
          // Handle two good iterators.
          //--------------------------------------------------------------------

          if( bf_it ){
            if( af_it ){
              twoWayMerge( bf_it, af_it );
            }
            return;
          }
          if( ch_it ){
            if( af_it ){
              twoWayMerge( ch_it, af_it );
            }
            return;
          }
        }
      }

      namespace{

        void twoWayMerge( Object::prop_map::iterator& ch_it, Object::prop_map::iterator& af_it ){

          //--------------------------------------------------------------------
          // Handle all three good iterators.
          //--------------------------------------------------------------------

          while( ch_it && af_it ){

            Object::prop_ptr ch_pProperty = *ch_it++;
            Object::prop_ptr af_pProperty = *af_it++;

            //------------------------------------------------------------------
            // Handle the caption property type.
            //------------------------------------------------------------------

            if( ch_pProperty->isCaption() ){
              auto cit = ch_pProperty->toChildren().getIterator();
              auto ait = af_pProperty->toChildren().getIterator();
              twoWayMerge( cit, ait );
              continue;
            }

            //------------------------------------------------------------------
            // Handle the Vector property type.
            //------------------------------------------------------------------

            if( ch_pProperty->isVector() ){

              //----------------------------------------------------------------
              // Vector of handles case.
              //----------------------------------------------------------------

              if( ch_pProperty->isHandle() ){
                vector<Object::handle> temp;
                ch_pProperty->  alterAs<vector<Object::handle>>( [&]( vector<Object::handle>& ch_vObjects ){
                  af_pProperty->alterAs<vector<Object::handle>>( [&]( vector<Object::handle>& af_vObjects ){

                      auto ch_it = ch_vObjects.getIterator();
                      auto af_it = af_vObjects.getIterator();

                      //--------------------------------------------------------
                      // Inserting a new vector between two empty vectors.
                      //--------------------------------------------------------

                      if( !af_it ){
                        if( ch_it ){
                          af_vObjects = ch_vObjects;
                        }
                        return;
                      }

                      //--------------------------------------------------------
                      // Add object from CHanges vector.
                      //--------------------------------------------------------

                      if( ch_it ){
                        vector<Object::handle> temp;
                        while( ch_it ){
                          bool bFound = false;
                          while( af_it ){
                            if( deepCompare( *ch_it, *af_it )){
                              bFound = true;
                              break;
                            }
                            ++af_it;
                          }
                          if( !bFound ){
                            temp += *ch_it;
                          }
                          af_it = af_vObjects.getIterator();
                          ch_it++;
                        }
                        af_vObjects.pushVector( temp );
                        return;
                      }

                      //--------------------------------------------------------
                      // The unhandled cases must be caught and implemented!
                      //--------------------------------------------------------

                      e_unreachable( "Unhandled case!" );
                  });
                });
                continue;
              }

              //----------------------------------------------------------------
              // Vector of types case.
              //----------------------------------------------------------------

              continue;
            }

            //------------------------------------------------------------------
            // Handle the Array property type.
            //------------------------------------------------------------------

            if( ch_pProperty->isArray() ){

              //----------------------------------------------------------------
              // Array of handles case.
              //----------------------------------------------------------------

              if( ch_pProperty->isHandle() ){
                ch_pProperty->  alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& ch_aObjects ){
                  af_pProperty->alterAs<array<Object::handle,1>>( [&]( array<Object::handle,1>& af_aObjects ){
                    Object::handle* ch_pObject = &ch_aObjects[0];
                    Object::handle* ch_eObject = ch_pObject + ch_aObjects.capacity();
                    Object::handle* af_pObject = &af_aObjects[0];
                    Object::handle* af_eObject = af_pObject + af_aObjects.capacity();
                    while(( ch_pObject < ch_eObject ) && ( af_pObject < af_eObject )){
                      Object::prop_map ch_props;
                      Object::prop_map af_props;
                      if( *ch_pObject ){
                        (*ch_pObject++)->getProperties( ch_props );
                      }else{
                        ++ch_pObject;
                      }
                      if( *af_pObject ){
                        (*af_pObject++)->getProperties( af_props );
                      }else{
                        ++af_pObject;
                      }
                      auto cit = ch_props.getIterator();
                      auto ait = af_props.getIterator();
                      twoWayMerge( cit, ait );
                    }
                  });
                });
                continue;
              }

              //----------------------------------------------------------------
              // Array of types case.
              //----------------------------------------------------------------

              ch_pProperty->  alterAs<array<char,1>>( [&]( array<char,1>& ch_aObjects ){
                af_pProperty->alterAs<array<char,1>>( [&]( array<char,1>& af_aObjects ){
                  cp ch_pBuffer = &ch_aObjects[ 0 ];
                  cp af_pBuffer = &af_aObjects[ 0 ];
                  for( u32 n=ch_aObjects.capacity(), i=0; i<n; ++i ){
                    if( ch_pProperty->toFlags()->bKeyframe && !af_pProperty->toFlags()->bKeyframe ){
                      memcpy( af_pBuffer, ch_pBuffer, ch_aObjects.stride() );
                    }
                    ch_pBuffer += ch_aObjects.stride();
                    af_pBuffer += af_aObjects.stride();
                  }
                });
              });
              continue;
            }

            //------------------------------------------------------------------
            // Handle the Handle property type.
            //------------------------------------------------------------------

            if( ch_pProperty->isHandle() ){
              ch_pProperty->  alterAs<Object::handle>( [&]( Object::handle& ch_hObject ){
                af_pProperty->alterAs<Object::handle>( [&]( Object::handle& af_hObject ){
                  if( !af_hObject ){
                    if( ch_hObject ){
                      af_hObject = ch_hObject;
                    }
                    return;
                  }
                  if( !ch_hObject ){
                    return;
                  }
                  Object::prop_map ch_props;
                  Object::prop_map af_props;
                  ch_hObject->getProperties( ch_props );
                  af_hObject->getProperties( af_props );
                  auto cit = ch_props.getIterator();
                  auto ait = af_props.getIterator();
                  twoWayMerge( cit, ait );
                });
              });
              continue;
            }

            //------------------------------------------------------------------
            // Handle the String property type.
            //------------------------------------------------------------------

            if( ch_pProperty->isType( e_classid<string>() )){
              ch_pProperty->  alterAs<string>( [&]( string& ch_string ){
                af_pProperty->alterAs<string>( [&]( string& af_string ){
                  if( ch_string != af_string ){
                    af_string = ch_string;
                  }
                });
              });
              continue;
            }

            //------------------------------------------------------------------
            // Handle all other property types.
            //------------------------------------------------------------------

            ch_pProperty->  query( [&]( cvp ch_pData, const u32 ch_uSize ){
              af_pProperty->query( [&]( cvp af_pData, const u32 af_uSize ){
                if( memcmp( ch_pData, af_pData, af_uSize )){
                  memcpy( vp( af_pData ), ch_pData, ch_uSize );
                }
              });
            });
          }
        }
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //mergeProperties:{                           |

#ifdef __APPLE__
  #pragma mark - Reader -
#endif

      void Reader::mergeProperties( const Object::handle& hObject, Reader& bfr, Reader& chr ){
        Reader& afr = *this;
        const u8 bfHasObject = bfr.read<u8>();
        const u8 chHasObject = chr.read<u8>();
        const u8 afHasObject = afr.read<u8>();
        if( bfHasObject ){
          if( chHasObject ){
            if( afHasObject ){
              // Construct and serialize the BeFore object.
              Object::handle bf_hObject = e_newt( hObject->probeid() );
              Object& bf_object = bf_hObject.cast();
              bf_object.preSerialize( bfr );
              Object::prop_map bf_out;
              bf_object.getProperties( bf_out );
              bf_out.foreach( [&]( Object::prop_ptr& pProperty ){ bfr.read( pProperty ); });
              bf_object.postSerialize( bfr );
              // Construct and serialize the CHanges object.
              Object::handle ch_hObject = e_newt( hObject->probeid() );
              Object& ch_object = ch_hObject.cast();
              ch_object.preSerialize( chr );
              Object::prop_map ch_out;
              ch_object.getProperties( ch_out );
              ch_out.foreach( [&]( Object::prop_ptr& pProperty ){ chr.read( pProperty ); });
              ch_object.postSerialize( chr );
              // Construct and serialize the AFter object.
              Object::handle af_hObject = hObject;
              Object& af_object = af_hObject.cast();
              af_object.preSerialize( afr );
              Object::prop_map af_out;
              af_object.getProperties( af_out );
              af_out.foreach( [&]( Object::prop_ptr& pProperty ){ afr.read( pProperty ); });
              af_object.postSerialize( afr );
              // Merge the three property lists.
              auto bit = bf_out.getIterator();
              auto cit = ch_out.getIterator();
              auto ait = af_out.getIterator();
              threeWayMerge( bit, cit, ait );
            }else{
              // Construct and serialize the BeFore object.
              Object::handle bf_hObject = e_newt( hObject->probeid() );
              Object& bf_object = bf_hObject.cast();
              bf_object.preSerialize( bfr );
              Object::prop_map bf_out;
              bf_object.getProperties( bf_out );
              bf_out.foreach( [&]( Object::prop_ptr& pProperty ){ bfr.read( pProperty ); });
              bf_object.postSerialize( bfr );
              // Construct and serialize the AFter object.
              Object::handle ch_hObject = hObject;
              Object& ch_object = ch_hObject.cast();
              ch_object.preSerialize( chr );
              Object::prop_map ch_out;
              ch_object.getProperties( ch_out );
              ch_out.foreach( [&]( Object::prop_ptr& pProperty ){ chr.read( pProperty ); });
              ch_object.postSerialize( chr );
              // Merge the two property lists.
              auto bit = bf_out.getIterator();
              auto cit = ch_out.getIterator();
              twoWayMerge( bit, cit );
            }
          }else if( afHasObject ){
            // Construct and serialize the BeFore object.
            Object::handle bf_hObject = e_newt( hObject->probeid() );
            Object& bf_object = bf_hObject.cast();
            bf_object.preSerialize( bfr );
            Object::prop_map bf_out;
            bf_object.getProperties( bf_out );
            bf_out.foreach( [&]( Object::prop_ptr& pProperty ){ bfr.read( pProperty ); });
            bf_object.postSerialize( bfr );
            // Construct and serialize the AFter object.
            Object::handle af_hObject = hObject;
            Object& af_object = af_hObject.cast();
            af_object.preSerialize( afr );
            Object::prop_map af_out;
            af_object.getProperties( af_out );
            af_out.foreach( [&]( Object::prop_ptr& pProperty ){ afr.read( pProperty ); });
            af_object.postSerialize( afr );
            // Merge the two property lists.
            auto bit = bf_out.getIterator();
            auto ait = af_out.getIterator();
            twoWayMerge( bit, ait );
          }else{
            // Construct and serialize the BeFore object.
            Object::handle bf_hObject = hObject;
            Object& bf_object = bf_hObject.cast();
            bf_object.preSerialize( bfr );
            Object::prop_map bf_out;
            bf_object.getProperties( bf_out );
            bf_out.foreach( [&]( Object::prop_ptr& pProperty ){ bfr.read( pProperty ); });
            bf_object.postSerialize( bfr );
          }
        }else if( chHasObject ){
          if( afHasObject ){
            // Construct and serialize the CHanges object.
            Object::handle ch_hObject = e_newt( hObject->probeid() );
            Object& ch_object = ch_hObject.cast();
            ch_object.preSerialize( chr );
            Object::prop_map ch_out;
            ch_object.getProperties( ch_out );
            ch_out.foreach( [&]( Object::prop_ptr& pProperty ){ chr.read( pProperty ); });
            ch_object.postSerialize( chr );
            // Construct and serialize the CHanges object.
            Object::handle af_hObject = hObject;
            Object& af_object = af_hObject.cast();
            af_object.preSerialize( afr );
            Object::prop_map af_out;
            af_object.getProperties( af_out );
            af_out.foreach( [&]( Object::prop_ptr& pProperty ){ afr.read( pProperty ); });
            af_object.postSerialize( afr );
            // Merge the two property lists.
            auto cit = ch_out.getIterator();
            auto ait = af_out.getIterator();
            twoWayMerge( cit, ait );
          }else{
            // Construct and serialize the CHanges object.
            Object::handle ch_hObject = hObject;
            Object& ch_object = ch_hObject.cast();
            ch_object.preSerialize( chr );
            Object::prop_map ch_out;
            ch_object.getProperties( ch_out );
            ch_out.foreach( [&]( Object::prop_ptr& pProperty ){ chr.read( pProperty ); });
            ch_object.postSerialize( chr );
          }
        }else if( afHasObject ){
          // Construct and serialize the AFter object.
          Object::handle af_hObject = hObject;
          Object& af_object = af_hObject.cast();
          af_object.preSerialize( afr );
          Object::prop_map af_out;
          af_object.getProperties( af_out );
          af_out.foreach( [&]( Object::prop_ptr& pProperty ){ afr.read( pProperty ); });
          af_object.postSerialize( afr );
        }
      }

    //}:                                          |
    //startAsyncLoad:{                            |

      void Reader::startAsyncLoad( const string& tag, Object* pObject, const OnLoaded& onLoaded ){
        Thread* pThread = new AsyncLoader( tag, onLoaded, this, pObject );
        #if MULTI_THREADED
          pThread->autodelete()->start();
        #else
          pThread->run();
          pThread->release();
          delete pThread;
        #endif
      }

    //}:                                          |
    //decompress:{                                |

      u64 Reader::decompress( cp dst, const u64 ndst, cvp src, const u64 nsrc ){
        if(( ndst <= ~0ULL )&&( nsrc <= ~0ULL )){
          return u64( LZ4_decompress_safe( ccp( src ), dst, int( nsrc ), int( ndst )));
        }
        return 0;
      }

    //}:                                          |
    //serialize:{                                 |

      Object::handle Reader::serializeHandleUnguarded( const std::function<void( Object& )>& lambda ){
        Object::handle hResult = e_newt( as<u64>() );
        if( hResult ){
          Object& object = hResult.cast();
          object.toStatus()->bIOComplete = 0;
          object.preSerialize(  *this );
          object.serialize(     *this );
          object.postSerialize( *this );
          object.toStatus()->bIOComplete = 1;
          if( lambda ){
            lambda( object );
          }
        }
        return hResult;
      }

      Object::handle Reader::serializeHandle( const std::function<void( Object& )>& lambda ){
        Object::handle hResult;
        const u32 hasHandle = read<u8>();
        e_assert( hasHandle < 2 );
        if( hasHandle ){
          hResult = serializeHandleUnguarded( lambda );
        }
        return hResult;
      }

      void Reader::serializeHandle( const Object::handle& hObject ){
        const u32 hasHandle = read<u8>();
        e_assert( hasHandle < 2 );
        if( hasHandle ){
          Object& object = hObject.noconst().cast();
          object.preSerialize(  *this );
          object.serialize(     *this );
          object.postSerialize( *this );
        }
      }

    //}:                                          |
    //version:{                                   |

      u16 Reader::version( const u16 ver ){
        const u16 cmp = read<u16>();
        if( cmp != ver ){
          e_logf( "Version mismatch ok!" );
        }
        return cmp;
      }

    //}:                                          |
    //uncache:{                                   |

      string Reader::uncache(){
        string out;
        const u64 key64 = read<u64>();
        if( !key64 ){
          return nullptr;
        }
        if( m_mStringTable.find( key64 )){
          Reader r( m_mStringTable[ key64 ]);
          r.setDictionary( m_vDictionary );
          out = r.unpackInternal();
        }
        return out;
      }

    //}:                                          |
    //unpack:{                                    |

      string Reader::unpackInternal(){

        //----------------------------------------------------------------------
        // The first byte tells us size of dictionary or if it's a hex string.
        //----------------------------------------------------------------------

        const u8   all = read<u8>();
        const bool hex = (255==all);
        const bool cst = (254==all);
        const bool nol = (0x0==all);

        //----------------------------------------------------------------------
        // Bail on nil strings.
        //----------------------------------------------------------------------

        if( nol ){
          return nullptr;
        }

        //----------------------------------------------------------------------
        // Construct a C string.
        //----------------------------------------------------------------------

        string out;
        if( cst ){
          read( out );
          return out;
        }

        //----------------------------------------------------------------------
        // Read the length of the string.
        //----------------------------------------------------------------------

        union{
          u32 x=0;
          u8 a[4];
        } N;
        read( N.a, 3 );

        //----------------------------------------------------------------------
        // Construct a hex string.
        //----------------------------------------------------------------------

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

        //----------------------------------------------------------------------
        // Construct a dictionary.
        //----------------------------------------------------------------------

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
          ixdict |= ( r[iy] & lomask ) >> iz;
          const u64 iw = iz + bits;
          if( iw > 8 ){
            const u64 himask = ( 1 << ( iw-8 ))-1;
            ixdict |= ( r[iy+1] & himask ) << ( 8-iz );
          }
          out += m_vDictionary[ ixdict ];
          ix += bits;
        }
        return out;
      }

    //}:                                          |
    //exists:{                                    |

      bool Reader::exists( const string& tag )const{
        FILE* file = getPrefabFilePointer( m_sName );
        if( !file ){
          file =  getFilePointer( m_sName, tag );
        }
        bool ok = false;
        if( file ){
          cp pTag = new char[ tag.len()+1 ];
          fread( pTag, 1, tag.len(), file );
          pTag[ tag.len() ]=0;
          fclose( file );
          ok = !strcmp( tag, pTag );
          delete[] pTag;
        }
        return ok;
      }

    //}:                                          |
    //read*:{                                     |
      //readPropertyVector:{                      |

        bool Reader::readPropertyVector( Object::prop_ptr& pProperty, u64& bytes ){

          //--------------------------------------------------------------------
          // Bail conditions.
          //--------------------------------------------------------------------

          if( !pProperty->isVector() ){
            return false;
          }

          //--------------------------------------------------------------------
          // Read string vector.
          //--------------------------------------------------------------------

          if( pProperty->isType( e_classid<string>() )){
            pProperty->alterAs<vector<string>>(
              [&]( vector<string>& lines ){
                const u32 n = read<u32>();
                lines.clear();
                bytes += 4;
                lines.resize( n );
                lines.foreach(
                  [&]( string& line ){
                    const u64 beg = m_tStream.tell();
                    unpack( line );
                    const u64 end = m_tStream.tell();
                    bytes +=( end - beg );
                  }
                );
              }
            );
            return true;
          }

          //--------------------------------------------------------------------
          // Stream in a vector of object handles.
          //--------------------------------------------------------------------

          if( pProperty->isHandle() ){
            pProperty->alterAs<vector<Object::handle>>(
              [&]( vector<Object::handle>& objects ){
                const u32 n = read<u32>();
                bytes += 4;
                pProperty->alterAs<vector<Object::handle>>(
                  [&]( vector<Object::handle>& objects ){
                    objects.clear();
                    objects.resize( n );
                    objects.foreach(
                      [&]( Object::handle& hObject ){
                        bytes += readProperties( hObject );
                      }
                    );
                  }
                );
              }
            );
            return true;
          }
          return false;
        }

      //}:                                        |
      //readPropertyHandle:{                      |

        bool Reader::readPropertyHandle( Object::prop_ptr& pProperty, u64& out_uBytes ){
          if( !pProperty->isHandle() || pProperty->isContainer() ){
            return false;
          }
          const u8 bExists = read<u8>();
          out_uBytes += 1;
          if( !bExists ){
            return true;
          }
          const u64 uProbeId = read<u64>();
          out_uBytes += 8;
          pProperty->alterAs<Object::handle>(
            [&]( Object::handle& hObject ){
              hObject = e_newt( uProbeId );
              out_uBytes += readProperties( hObject );
            }
          );
          return true;
        }

      //}:                                        |
      //readPropertyString:{                      |

        bool Reader::readPropertyString( Object::prop_ptr& pProperty, u64& bytes ){
          if( pProperty->isType( e_classid<string>() )){
            pProperty->alterAs<string>(
              [&]( string& out ){
                const u64 beg = size();
                unpack( out );
                const u64 end = size();
                bytes +=( end - beg );
              }
            );
            return true;
          }
          return false;
        }

      //}:                                        |
      //readPropertyArray:{                       |

        bool Reader::readPropertyArray( Object::prop_ptr& pProperty, u64& bytes ){

          //--------------------------------------------------------------------
          // Bail conditions.
          //--------------------------------------------------------------------

          if( !pProperty->isArray() ){
            return false;
          }

          //--------------------------------------------------------------------
          // Stream in array of object handles.
          //--------------------------------------------------------------------

          if( pProperty->isHandle() ){
            pProperty->alterAs<array<Object::handle,1>>(
              [&]( array<Object::handle,1>& objects ){
                const u32 n = read<u32>();
                bytes += 4;
                for( u32 i=0; i<n; ++i ){
                  bytes += readProperties( objects[ i ]);
                }
              }
            );
            return true;
          }

          //--------------------------------------------------------------------
          // Stream in array of pod objects.
          //--------------------------------------------------------------------

          pProperty->alterAs<array<char,1>>(
            [&]( array<char,1>& objects ){
              const u32 capacity = read<u32>();
              const u32 stride   = read<u32>();
              bytes += 4;
              bytes += 4;
              objects.alter( 0,
                [&]( char& _1st ){
                  char* pBuffer = &_1st;
                  for( u32 i=0; i<capacity; ++i ){
                    bytes += read( pBuffer, stride );
                    pBuffer += stride;
                  }
                }
              );
            }
          );
          return true;
        }

      //}:                                        |
      //readProperties:{                          |

        u64 Reader::readProperties( Object::handle& hObject ){
          const u32 hasObject = read<u8>();
          e_assert( hasObject < 2 );
          u64 bytes = 1;
          if( hasObject ){

            //------------------------------------------------------------------
            // Create hObject if it hasn't been alraedy. Think handle vectors.
            //------------------------------------------------------------------

            if( !hObject ){
              // We're guaranteed the first eight bytes is the class identifier.
              const u64 classId = as<u64>();
              // Create an object and replace incoming.
              hObject = e_newt( classId );
            }

            //------------------------------------------------------------------
            // Serialize in the object.
            //------------------------------------------------------------------

            // Cast new object to reference.
            Object& object = hObject.cast();
            // Pre-serialize the object.
            object.preSerialize( *this );
            // Grab all the properties from the object. This replaces the
            // normal serialize() call because we're only loading props.
            Object::prop_map out_mProperties;
            object.getProperties( out_mProperties );
            readPropertyMap( *this, out_mProperties );
            // Post serialize.
            object.postSerialize( *this );
          }
          return bytes;
        }

      //}:                                        |
      //read:{                                    |

        u64 Reader::read( Object::prop_ptr& pProperty, const std::function<void( Object* )>& lambda ){

          //--------------------------------------------------------------------
          // Bail conditions.
          //
          // If the property is nullptr or it's to be ignored then we just bail
          // out of the method and continue onto the next.
          //--------------------------------------------------------------------

          if( !pProperty ){
            return 0;
          }
          if( pProperty->isIgnored() ){
            return 0;
          }

          //--------------------------------------------------------------------
          // Read in property name.
          //--------------------------------------------------------------------

          string  name;
          unpack( name );
          e_assert( pProperty->isName( name ));

          //--------------------------------------------------------------------
          // Read in the caption and it's children.
          //--------------------------------------------------------------------

          if( pProperty->isCaption() ){
            return readPropertyMap( *this, pProperty->toChildren() );
          }

          //--------------------------------------------------------------------
          // Read in most important flags.
          //--------------------------------------------------------------------

          u64 bytes = 1;
          u32 umask = read<u8>();
              pProperty->toFlags()->bHidden  =( MASK_HIDDEN  ==( umask & MASK_HIDDEN   ));
              pProperty->toFlags()->bKeyframe=( MASK_KEYFRAME==( umask & MASK_KEYFRAME ));
          if( pProperty->toFlags()->bKeyframe ){
              pProperty->toFlags()->bModified = 1;
          }

          //--------------------------------------------------------------------
          // Read in vector property.
          //--------------------------------------------------------------------

          if( readPropertyVector( pProperty, bytes )){ goto sk; }
          if( readPropertyHandle( pProperty, bytes )){ goto sk; }
          if( readPropertyString( pProperty, bytes )){ goto sk; }
          if( readPropertyArray ( pProperty, bytes )){ goto sk; }

          //--------------------------------------------------------------------
          // Write out all other pod data.
          //--------------------------------------------------------------------

          // Compare property data with buffer.
          pProperty->alter(
            [&]( vp data, const u32 size ){
              bytes += read( data, size );
            }
          );
          readPropertyMap( *this, pProperty->toChildren() );

          //--------------------------------------------------------------------
          // Write out all children.
          //--------------------------------------------------------------------

sk:       if( lambda ){
            lambda( (Object*)pProperty->toOuter() );
          }
          return bytes;
        }

        u64 Reader::read( vp data, const u64 n ){
          memcpy( data, c_str(), n );
          skip( n );
          return n;
        }

        u64 Reader::read( stream& st ){
          u64 bytes = 17;
          const u64 stride = read<u64>();
          const u64 size   = read<u64>();
          const u8  data   = read<u8>();
          st = stream( nullptr, stride, size );
          if( data ){
            bytes += read( st.alloc( size ), size*stride );
          }
          return bytes;
        }

        u64 Reader::read( buffer& b ){
          u64 bytes  = 9;
          u32 stride = read<u32>();
          u32 size   = read<u32>();
          u8  data   = read<u8>();
          b.setStride( stride );
          if( data ){
            b.alloc( size );
            bytes += read( b.data(), size*stride );
          }
          return bytes;
        }

      //}:                                        |
    //}:                                          |
    //skip:{                                      |

      bool Reader::skip( const u64 bytes ){
        return m_tStream.skip( bytes );
      }

    //}:                                          |
    //load:{                                      |

      Reader& Reader::load( const string& tag ){

        //----------------------------------------------------------------------
        // Open file and decompress if possible.
        //----------------------------------------------------------------------

        FILE* pFile = getPrefabFilePointer( m_sName );
        if( !pFile ){
          pFile = getFilePointer( m_sName, tag );
          // Failing all that look in the cache.
          if( !pFile ){
            pFile = e_fopen( "~/.eon/" + m_sName.basename() + ".eon", "rb" );
          }
        }
        m_tFlags->bError = !pFile;
        if( isError() ){
          return *this;
        }
        if( !tag.empty() ){

          //--------------------------------------------------------------------
          // First of all check that the tags match.
          //--------------------------------------------------------------------

          #if !e_compiling( clang )
            char* aLabel = new char[ tag.len()+1 ];
          #else
            char aLabel[ tag.len()+1 ];
          #endif
          aLabel[ tag.len() ] = 0;
          fread( aLabel, tag.len(), 1, pFile );
          const bool isMatch=( tag == aLabel );
          #if !e_compiling( clang )
            delete[] aLabel;
          #endif
          if( !isMatch ){
            m_tFlags->bError = 1;
            fclose( pFile );
            return *this;
          }

          //--------------------------------------------------------------------
          // Then get the packed and unpacked sizes.
          //--------------------------------------------------------------------

          u64 unpackedSize;
          #if e_compiling( debug )
            u64 unpacked_size_result =
          #endif
          fread( &unpackedSize, 1, sizeof( unpackedSize ), pFile );
          e_assert( unpacked_size_result == sizeof( unpackedSize ));
          #if e_compiling( io_logging )
            printf( "%llu\n", unpackedSize );
          #endif
          u64 packedSize;
          #if e_compiling( debug )
            u64 packed_size_result =
          #endif
          fread( &packedSize, 1, sizeof( packedSize ), pFile );
          e_assert( packed_size_result == sizeof( packedSize ));
          e_assert( packedSize <= unpackedSize );
          #if e_compiling( io_logging )
            printf( "%llu\n", packedSize );
          #endif

          //--------------------------------------------------------------------
          // Load data then if the sizes differ decompress.
          //--------------------------------------------------------------------

          if( unpackedSize == packedSize ){
            cp pBuffer = m_tStream.alloc( unpackedSize );
            pBuffer[ unpackedSize ]=0;
            #if e_compiling( debug )
              u64 unpacked_read_result =
            #endif
            fread( pBuffer, 1, unpackedSize, pFile );
            e_assert( unpacked_read_result == unpackedSize );
            fclose( pFile );
            m_tStream.reset();
          }else{
            cp pPackedBuffer = m_tStream.alloc( packedSize+unpackedSize );
            #if e_compiling( debug )
              u64 packed_read_result =
            #endif
            fread( pPackedBuffer, 1, packedSize, pFile );
            e_assert( packed_read_result == packedSize );
            fclose( pFile );
            cp pUnpackedBuffer = pPackedBuffer + packedSize;
            pUnpackedBuffer[ unpackedSize ]=0;
            decompress(
                pUnpackedBuffer,
                unpackedSize,
                pPackedBuffer,
                packedSize );
            m_tStream.reset();
            m_tStream.skip( packedSize );
          }

          //--------------------------------------------------------------------
          // Read dictionary from end of buffer.
          //--------------------------------------------------------------------

          m_tStream.end( [&]( ccp pEnd ){
            const u8  ndict = (( u8* )pEnd)[ unpackedSize - 1 ];
            const u8* pdict = (( u8* )pEnd )+unpackedSize - 1 - ndict;
            m_vDictionary.resize( ndict );
            m_vDictionary.alter( 0, [&]( u8& dict ){
              memcpy( &dict, pdict, ndict );
            });
          });
          return *this;
        }

        //----------------------------------------------------------------------
        // If no tag specified just load the pFile as a string.
        //----------------------------------------------------------------------

        struct stat st;
        const int result = stat( m_sName, &st );
        if( !result ){
          const u64 size = st.st_size;
          if( size ){
            cp data = cp( m_tStream.alloc( size ));
            fread( data, 1, size, pFile );
            data[ size ]=0;
          }
        }
        fclose( pFile );
        m_tStream.reset();
        return *this;
      }

    //}:                                          |
    //eof:{                                       |

      bool Reader::eof()const{
        const u64 scap = m_tStream.capacity();
        const u64 size = m_tStream.size();
        const u64 algn = ( size+( stream::PARAGRAPH-1 )) & ~( stream::PARAGRAPH-1 );
        return( algn == scap );
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    Reader::Reader( const string& name )
      : m_sName( name )
    {}

    /** \brief Reader copy constructor.
      *
      * This constructor copies the name and the stream.
      */

    Reader::Reader( const Reader& lvalue )
      : m_tStream( lvalue.m_tStream )
      , m_sName( lvalue.m_sName )
    {}


    /** \brief Reader constructor with stream.
      *
      * This constructor will build a reader from an existing stream
      * object.
      */

    Reader::Reader( const stream& st )
        : m_tStream( st ){
      m_tStream.reset( 0 );
    }

  //}:                                            |
  //Dtor:{                                        |

    Reader::~Reader(){
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
