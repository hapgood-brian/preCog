//------------------------------------------------------------------------------
//       Copyright 2014-2020 Creepy Doll Games LLC. All rights reserved.
//
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A  PARTICULAR PURPOSE ARE DISCLAIMED.  IN
// NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,   OR  CONSEQUENTIAL  DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON  ANY  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING  NEGLIGENCE  OR  OTHERWISE)  ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include<sys/stat.h>
#include<lz4hc.h>
#include<lz4.h>

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================+=============================
//APIs:{                                          |
  //FileIO:{                                      |
    //e_?exists:{                                 |

      bool e_fexists( const string& path ){
        return IEngine::fexists( path );
      }

      bool e_dexists( const string& path ){
        return IEngine::dexists( path );
      }

      bool e_lexists( const string& path ){
        return IEngine::lexists( path );
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Writer:{                                        |
  //Options:{                                     |

#ifdef __APPLE__
  #pragma mark - Options -
#endif

    namespace{
      using ObjectPair = std::pair<u64,Object::prop_ptr>;
      #define __compiling_no_property_logs_ever__ 0
      #define __compiling_no_failsafes__ 0
      constexpr auto kChunkID = 'PROP';
      constexpr auto kCPUs = 4;
    }

  //}:                                            |
  //Private:{                                     |
    //writePropertyMap:{                          |

#ifdef __APPLE__
  #pragma mark - Private -
#endif

      namespace{
        //
        //  NUMS : u32 - number of chunks
        //  PROP : u32 - 'PROP'
        //  SIZE : u64 (in bytes)
        //  NAME : packed
        //  MASK : u8
        //  DATA : vector|handle|string|array|raw
        //  CMAP : children (1)
        //  -----:-------------------------------
        //  NUMS : Number of properties
        //  PROP : u32
        //  SIZE : u64 (count)
        //  NAME : packed
        //  MASK : u8
        //  DATA : vector|handle|string|array|raw
        //
        u64 writePropertyMap( Writer& fs, const Object::prop_map& out_mProperties ){
          const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );
          const auto count = out_mProperties.size();
          const auto start = fs.toStream().tell();
          #if !e_compiling( no_property_logs_ever )
            if( usePropertyLogs ){
              e_msgf( "\t\tNUMS: %u", count );
            }
          #endif
          fs.write<u32>( count );
          out_mProperties.foreachKV(
            [&]( const u64 key, const Object::prop_ptr& pProperty ){
              if( !pProperty ){
                return;
              }

              //----------------------------------------------------------------
              // Write property header.
              //----------------------------------------------------------------

              // PROP - write out the chunk ID.
              #if !e_compiling( no_property_logs_ever )
                if( usePropertyLogs ){
                  e_msgf( "\t\tPROP: 'PROP' with key of %llu", key );
                }
              #endif
              fs.write( kChunkID );

              // SIZE - write out 64-bit # of bytes in chunk.
              const auto sizeOffset = fs.toStream().tell();
              #if !e_compiling( no_property_logs_ever )
                if( usePropertyLogs ){
                  e_msgf( "\t\tSIZE: 0 (priming)" );
                }
              #endif
              fs.write<u64>( 0 );

              // Lack of a property leaves size at zero.
              if( !pProperty ){
                // NB: This should never happen ever! You'd have to go out of
                // your way to be stupid; but I'm more cunning than that!
                e_errorf( 29832472, "The laws of physics broke down!" );
                return;
              }

              // Properties set to be ignored leave size at zero too.
              if( pProperty->isIgnored() ){
                return;
              }

              //----------------------------------------------------------------
              // Record the current position to test the read size with 'sz'.
              //----------------------------------------------------------------

              const auto begDataOffset = fs
                . toStream()
                . tell();
              fs.write( key );
              fs.pack( pProperty->toName() );

              //----------------------------------------------------------------
              // Write property data.
              //----------------------------------------------------------------

              // Write out the property name.
              if( usePropertyLogs ){
                e_msgf(
                  "  Writing property: \"%s\""
                  , pProperty->toName().c_str()
                );
              }

              // DATA - write out the property and it's data.
              fs.write( pProperty );
              const auto endDataOffset = fs
                 . toStream()
                 . tell();
              const auto lenChunk = endDataOffset - begDataOffset;
              #if !e_compiling( no_property_logs_ever )
                if( usePropertyLogs ){
                  e_msgf( "\t\tSIZE: %llu", lenChunk );
                }
              #endif

              //----------------------------------------------------------------
              // Write back chunk size in bytes and step back to this location.
              //----------------------------------------------------------------

              fs.toStream().seek( sizeOffset );
              fs.write<u64>( lenChunk );
              fs.toStream().seek(
                endDataOffset
              );
            }
          );
          const auto stop = fs.toStream().tell();
          return( stop - start );
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
        auto dictionary = m_vDictionary;
        auto bitmap = m_aBitmap;

        // Now build the dictionary, counting hex chars.
        const auto begin = m_tStream.size();
        u8 ixMaxDict = 0;
        u8 isHex = 1;{
          u8* r = (u8*)pSrc;
          u32 n = dictionary.size();
          dictionary.resize( n + bytes );
          dictionary.alter( 0,
            [&]( u8& _1st ){
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
            }
          );
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
            m_tStream.seek( begin );
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
                , static_cast<int>( zSrc )
                , static_cast<int>( zDst )
                , 9
              );
            }
          }
        );
        e_assert( size, "Compression failure!" );
        pOut = pDst;
        return size;
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
      //writePropertyHandle[private]:{            |

        bool Writer::writePropertyHandle( const Object::prop_ptr& pProperty ){
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
              write( object .probeid() );
              writeProperties( hObject );
            }
          );
          return true;
        }

      //}:                                        |
      //writePropertyVector:{                     |

        bool Writer::writePropertyVector( const Object::prop_ptr& pProperty ){
          if( !pProperty->isVector() ){
            return false;
          }

          //--------------------------------------------------------------------
          // Write string vectors.
          //--------------------------------------------------------------------

          if( pProperty->isType( e_classid<string>() )){
            pProperty->queryAs<vector<string>>(
              [&]( const vector<string>& lines ){
                write<u32>( lines.size() );
                lines.foreach(
                  [&]( const string& s ){
                    pack( s );
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

        bool Writer::writePropertyString( const Object::prop_ptr& pProperty ){
          if( pProperty->isType( e_classid<string>() )){
            pProperty->queryAs<string>(
              [&]( const string& s ){
                pack( s );
              }
            );
            return true;
          }
          return false;
        }

      //}:                                        |
      //writePropertyArray:{                      |

        bool Writer::writePropertyArray( const Object::prop_ptr& pProperty ){

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
              write( capacity );
              write( stride );
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
          const auto hObject = in_hObject;
          if( !hObject ){
            bytes += write<u8>( 0 );
          }else{
            bytes += write<u8>( 1 );

            //------------------------------------------------------------------
            // Go ahead and block until object IO complete.
            //
            // TODO: Do we want to block until IO complete shallow?
            //------------------------------------------------------------------

            Object::prop_map out_mProperties;
            const Object& object = hObject.cast();
            object.getProperties( out_mProperties );
            object.blockUntilIOComplete();

            //------------------------------------------------------------------
            // Write and defer objects.
            //------------------------------------------------------------------

            object.preSerialize( *this );
            writePropertyMap( *this, out_mProperties );
            object.postSerialize( *this );
          }
          return bytes;
        }

      //}:                                        |
      //write:{                                   |

        u64 Writer::write( const Object::prop_ptr& pProperty ){
          const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );
          e_guardw( m_tLock );

          //--------------------------------------------------------------------
          // Bail conditions.
          //--------------------------------------------------------------------

          // If incoming property is read-only we never automatically serialize
          // it out to this stream.  Read-only includes buffers and bounds that
          // are never changed by the user.
          if( pProperty->isIgnored() ){
            if( usePropertyLogs ){
              e_msgf(
                "    Ignoring property '%s'"
                , ccp( pProperty->toName() )
              );
            }
            return 0;
          }

          //--------------------------------------------------------------------
          // First thing's first: write out the property name.
          //--------------------------------------------------------------------

          const auto start = m_tStream.tell();
          if( pProperty->toName().empty() ){
            e_brk( "Property has no name" );
          }
          pack( pProperty->toName() );

          //--------------------------------------------------------------------
          // Save off the property type and the property name then write 64-bit
          // zero so we can skip past all this property's data while reading.
          //--------------------------------------------------------------------

          u8 mask = 0;
          if( pProperty->isCaption() ){
            goto sk;
          }

          //--------------------------------------------------------------------
          // Save off most important flags.
          //--------------------------------------------------------------------

          if( pProperty->toFlags()->bKeyframe ){
            mask |= MASK_KEYFRAME;
            mask |= MASK_MODIFIED;
          }
          if( pProperty->toFlags()->bHidden ){
            mask |= MASK_HIDDEN;
          }
          write( mask );

          //--------------------------------------------------------------------
          // Write out properties.
          //--------------------------------------------------------------------

          if( writePropertyVector( pProperty )){ goto sk; }
          if( writePropertyHandle( pProperty )){ goto sk; }
          if( writePropertyString( pProperty )){ goto sk; }
          if( writePropertyArray(  pProperty )){ goto sk; }

          //--------------------------------------------------------------------
          // Write other types.
          //--------------------------------------------------------------------

          pProperty->query(
            [&]( cvp data, const u32 size ){
              write( data, size );
            }
          );

          //--------------------------------------------------------------------
          // Write out all the children.
          //--------------------------------------------------------------------

          sk: writePropertyMap( *this, pProperty->toChildren() );

          //--------------------------------------------------------------------
          // Write out children first.
          //--------------------------------------------------------------------

          const auto stop = m_tStream.tell();
          return( stop - start );
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
          return( m_tStream.bytes() - bytes );
        }

        u64 Writer::write( const stream& st ){
          e_guardw( m_tLock );
          u64 bytes = 0;
          if( m_tFlags->bText ){
            bytes += write( ccp( st.data() ));
          }else{
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

        if( !m_vPending.empty() ){
          #if !e_compiling( web )
            e_forAsync<Pending,8>( m_vPending,
          #else
            m_vPending.foreach(
          #endif
            [&]( const Pending& pending ){
              const auto streamEndsAt = m_tStream.tell();
              pending();
              m_tStream.seek( streamEndsAt );
            }
          );
        }
        m_vPending.clear( );
        e_guardw( m_tLock );

        //----------------------------------------------------------------------
        // Rename eon file to SHA1 of uncompressed data.  It's really important
        // that we do this before the pending async writes that update the SHA1
        // keys because otherwise we'll alter the SHA1 of the data that we have
        // recorded in the stream. After filename is computed then we can alter
        // the stream to point to the newly saved exported files.
        //----------------------------------------------------------------------

        bool bHashing = false;{
          e_guardr( m_tLock );
          if( m_tFlags->bRenameSHA1 ){
            const auto& basename = m_sFilename.basename();
            const bool bHashed = basename.is_sha1();
            if( !bHashed ){
              m_sFilename = m_sFilename.path() + IEngine::sha1of( m_tStream );
              if( !m_tFlags->bNoExt ){
                m_sFilename += ".eon";
              }
              bHashing = true;
            }
          }
        }

        //----------------------------------------------------------------------
        // Create target directory structure.
        //----------------------------------------------------------------------

        e_md( m_sFilename.path() );

        //----------------------------------------------------------------------
        // Append the dictionary.
        //----------------------------------------------------------------------

        // If there's a tag append dictionary to eof.
        if( tag && !m_tFlags->bText ){
          m_vDictionary.query( 0,
            [&]( const u8& dict ){
              write( &dict, m_vDictionary.size() );
              write( u8( m_vDictionary.size() ));
            }
          );
        }

        //----------------------------------------------------------------------
        // Compress file data.
        //----------------------------------------------------------------------

        // Now we can compress the daylights out of it.
        const u64 slen = used();
        u64 dlen;
        cp  dbuf;
        if( tag && m_tFlags->bCompress ){
          dlen = compress( dbuf );
          if( dlen >= slen ){//handle negative compression.
            dbuf = cp( m_tStream.data() );
            dlen = slen;
          }
        }else{
          dbuf = cp( m_tStream.data() );
          dlen = slen;
        }

        //----------------------------------------------------------------------
        // Commit to hardfile
        //----------------------------------------------------------------------

        string filename;
        if( bHashing ){
          filename = std::move( m_sFilename );
        }else{
          const string& ext = m_sFilename.ext();
          filename = std::move( m_sFilename );
          if( tag && ext.empty() && !m_tFlags->bNoExt ){
            filename += ".eon";
          }
        }
        u64 bytes = 0;
        FILE* f = e_fopen( filename.c_str(), "wb" );
        if( f ){
          if( tag ){
            bytes += fwrite( tag,   1, strlen( tag  ), f );
            bytes += fwrite( &slen, 1, sizeof( slen ), f );//uncompressed
            bytes += fwrite( &dlen, 1, sizeof( dlen ), f );//compressed
          }
          bytes += fwrite( dbuf, 1, dlen, f );
          fclose( f );
        }else{
          e_logf( "Couldn't save %s", filename.c_str() );
        }
        m_sFilename = std::move( filename );
        return bytes;
      }

    //}:                                          |
    //init:{                                      |

      void Writer::init( const u32 uFlags ){
        if( uFlags & kHISTORIC ){
          m_tFlags->bHistoric = 1;
        }
        if( uFlags & kCOMPRESS ){
          m_tFlags->bCompress = 1;
        }
        if( uFlags & kIMPORT ){
          m_tFlags->bImporting = 1;
        }
        if( uFlags & kNOEXT ){
          m_tFlags->bNoExt = 1;
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

    Writer::Writer( const Writer& w )
      : m_mStringTable( w.m_mStringTable )
      , m_vDictionary(  w.m_vDictionary )
      , m_sFilename(    w.m_sFilename )
      , m_aBitmap(      w.m_aBitmap )
      , m_tStream(      w.m_tStream )
      , m_tFlags(       w.m_tFlags )
      , m_sTag(         w.m_sTag )
    {}

    Writer::Writer( Writer&& w )
      : m_mStringTable( std::move( w.m_mStringTable ))
      , m_vDictionary(  std::move( w.m_vDictionary ))
      , m_sFilename(    std::move( w.m_sFilename ))
      , m_aBitmap(      std::move( w.m_aBitmap ))
      , m_tStream(      std::move( w.m_tStream ))
      , m_tFlags(       std::move(  w.m_tFlags ))
      , m_sTag(         std::move( w.m_sTag ))
    {}

    Writer::Writer( const string& filename, const u32 uFlags )
        : m_sFilename( filename ){
      init( uFlags );
    }

    Writer::Writer( const stream& st, const u32 uFlags )
        : m_tStream( st ){
      init( uFlags );
    }

  //}:                                            |
  //Dtor:{                                        |

    Writer::~Writer(){
      // Need to repeat this just in case it hasn't been done; not all writers
      // have their save() functions called.
      if( !m_vPending.empty() ){
        #if !e_compiling( web )
          e_forAsync<Pending,8>( m_vPending,
        #else
          m_vPending.foreach(
        #endif
          [&]( const Pending& pending ){
            pending();
          }
        );
      }
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
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
                [&]( const Prefab::File::handle& F ){
                  if( F->toName() == name ){
                    pFile = e_fopen( prefab.toPath(), "rb" );
                    if( pFile ){
                      fseek( pFile
                        , F->toBase()
                        , SEEK_SET
                      );
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
        //
        //  NUMS : Number of chunks
        //       : for loop begin
        //  PROP : u32
        //  SIZE : u64 (in bytes)
        //  ---- : ----
        //  NAME : packed
        //  MASK : u8
        //  DATA : vector|handle|string|array|raw
        //  CMAP : children (1)
        //  -----:-------------------------------
        //  NUMS : Number of properties
        //  PROP : u32
        //  SIZE : u64 (count)
        //  NAME : packed
        //  MASK : u8
        //  DATA : vector|handle|string|array|raw
        //
        u64 readPropertyMap( Reader& fs, Object::prop_map& out_mProperties ){

          //********************************************************************

          //--------------------------------------------------------------------
          // Dump out all the property names for reference.
          //--------------------------------------------------------------------

          #if !e_compiling( no_property_logs_ever )
            const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );
            if( usePropertyLogs ){
              e_msgf( "    + ------------- +" );
              e_msgf( "    | Property List |" );
              e_msgf( "    + ------------- +" );
              out_mProperties.foreachKV(
                []( const u64 key, Object::prop_ptr& pProperty ){
                  if( !pProperty ){
                    return;
                  }
                  const auto& name = pProperty->toName();
                  e_msgf(
                    "      Key: %llu Value: \"%s\" (%llu)"
                    , key
                    , name.c_str()
                    , name.hash()
                  );
                }
              );
            }
          #endif

          //--------------------------------------------------------------------
          // Prime all the properties first.
          //--------------------------------------------------------------------

          out_mProperties.foreach(
            []( Object::prop_ptr& pProperty ){
              if( !pProperty ){
                return;
              }
              pProperty->tick();
            }
          );

          //--------------------------------------------------------------------
          // NUMS: number of properties (count). Now remember it's ok for the
          // count to be different from the out_mProperties.size() because we
          // will be loading the property map by keyed value.
          //--------------------------------------------------------------------

          const auto count = fs.read<u32>();
          if( !count ){
            return 0;
          }
          #if !e_compiling( no_property_logs_ever )
            if( usePropertyLogs ){
              e_msgf(
                "\t\tNUMS: %u"
                , count
              );
            }
          #endif

          //********************************************************************

          //--------------------------------------------------------------------
          // Walk over count number of properties.
          //--------------------------------------------------------------------

          vector<Reader> chunks;
          for( u32 i=0; i<count; ++i ){

            //------------------------------------------------------------------
            // Read the property header and verify it's contents.
            //------------------------------------------------------------------

            // PROP: Magic ID for sanity checking the stream; chunk marker.
            const auto magicID = fs.read<u32>();
            if( magicID != kChunkID ){
              e_errorf( 6015512, "magic number mismatch" );
              return 0;
            }

            // Log the reading of the magic ID so we can see if it's right.
            #if !e_compiling( no_property_logs_ever )
              if( usePropertyLogs ){
                ccp p = ccp( &magicID );
                e_msgf(
                  "\t\tPROP: '%c%c%c%c'"
                  , p[ 3 ]
                  , p[ 2 ]
                  , p[ 1 ]
                  , p[ 0 ]
                );
              }
            #endif

            //------------------------------------------------------------------
            // Load chunk.
            //------------------------------------------------------------------

            // SIZE: The size of the chunk in bytes. Must match at the end or
            // we'll bail out with an error condition.
            const auto sz = fs.read<u64>();
            if( !sz ){
              continue;
            }
            #if !e_compiling( no_property_logs_ever )
              if( usePropertyLogs ){
                e_msgf( "\t\tSIZE: %llu", sz );
              }
            #endif

            // Load the chunk for later.
            stream st;
            cp pRead = st.realloc( sz );
            fs.read( pRead, sz );
            st.reset();

            // Push stream into new file stream Reader and add to future farm.
            chunks.pushBy(
              [&]( Reader& chunk ){
                chunk.setName( "internal/" + fs.toName() );
                chunk.toStringTable() = fs.toStringTable();
                chunk.setDictionary( fs.toDictionary() );
                chunk.toStream() = std::move( st );
                chunk.setFlags( fs.toFlags() );
              }
            );
          }

          //********************************************************************

          //--------------------------------------------------------------------
          // Create thread farm to process the chunks we just loaded while the
          // rest of the function goes on to the next chunks.
          //--------------------------------------------------------------------

          std::atomic<u64> atomicBytes = 0;
          #if !e_compiling( web )
            e_forAsync<Reader,kCPUs>( chunks,
          #else
            chunks.foreach(
          #endif
            [&]( const Reader& in ){
              auto& fs = const_cast<Reader&>( in );

              //----------------------------------------------------------------
              // Get the key and optionally log it out.
              //----------------------------------------------------------------

              const auto key = fs.read<u64>();
              const auto txt = fs.unpack();
              #if !e_compiling( no_property_logs_ever )
                if( usePropertyLogs ){
                  e_msgf( "key of %llu is %s", key, ccp( txt ));
                }
              #endif

              //----------------------------------------------------------------
              // Index into the map and update the property there.
              //----------------------------------------------------------------

              // So we're guaranteed now to have everything we need to safely
              // serialize the object back into memory. This memory location
              // should be at the start of the Object::preSerialize function.
              Object::prop_ptr pProperty = out_mProperties[ key ];
              if( !pProperty ){
                pProperty = out_mProperties[ txt.hash() ];
                if( !pProperty ){
                  #if !e_compiling( no_property_logs_ever )
                    if( usePropertyLogs ){
                      e_warnsf( "cannot find property; tried twice" );
                    }
                  #endif
                }
              }

              //------------------------------------------------------------
              // Bail conditions.
              //------------------------------------------------------------

              #if e_compiling( sanity )
                if( pProperty->isIgnored() ){
                  e_unreachable( "Big bad fatal error!" );
                }
              #endif
              if( !pProperty ){
                #if!e_compiling( no_property_logs_ever )
                  if( usePropertyLogs ){
                    e_msgf( "      Ignoring %s", ccp( txt ));
                  }
                #endif
                return;
              }

              //------------------------------------------------------------
              // Read the property in from the stream.
              //------------------------------------------------------------

              // DATA: Now  we should have a valid object to read into. We
              // have to be really careful all the way in because the
              // streaming service is completely asynchronous.
              const auto bytesRead = fs.read( pProperty );
              if( usePropertyLogs ){
                e_msgf(
                  "      Read %llu of %llu bytes (%s)"
                  , bytesRead
                  , fs.toStream().capacity()
                  , ccp( pProperty->toName() )
                );
              }
              atomicBytes += bytesRead;
            }
          );

          //********************************************************************

          //--------------------------------------------------------------------
          // Load the number of bytes read from atomic value and return it.
          //--------------------------------------------------------------------

          #if !e_compiling( web )
            return atomicBytes.load();
          #else
            return atomicBytes;
          #endif
        }
      }

    //}:                                          |
    //getFilePointer:{                            |

      namespace{
        e_noinline FILE* getFilePointer( const string& name, const string& tag, const bool bNoExt ){
          FILE* pFile = e_fopen( name, "rb" );
          if( !pFile ){
            string eonPath;
            if( tag.empty() ){
              eonPath = e_packagePath();
            }else{
              eonPath = e_eonPath();
            }
            eonPath += name;
            if( !tag.empty() &&( name.ext().tolower().hash() != ".eon"_64 )){
              if( !bNoExt ){
                eonPath += ".eon";
              }
            }
            pFile = e_fopen( eonPath, "rb" );
            if( !pFile ){
              string resPath = IEngine::toStreamPath() + name;
              if( !tag.empty() &&( name.ext().tolower().hash() != ".cache"_64 )){
                if( !bNoExt ){
                  resPath += ".cache";
                }
              }
              pFile = e_fopen( resPath, "rb" );
              if( !pFile ){
                return nullptr;
              }
            }
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
              ch_pProperty->alterAs<array<Object::handle,1>>(
                [&]( array<Object::handle,1>& ch_aObjects ){
                  af_pProperty->alterAs<array<Object::handle,1>>(
                    [&]( array<Object::handle,1>& af_aObjects ){
                      for( u32 n=ch_aObjects.capacity(), i=0; i<n; ++i ){
                        if( !deepCompare( ch_aObjects[ i ], af_aObjects[ i ])){
                          bResult = false;
                          return;
                        }
                      }
                    }
                  );
                }
              );
              return bResult;
            }

            //------------------------------------------------------------------
            // Array of types case.
            //------------------------------------------------------------------

            bool bResult = true;
            ch_pProperty->alterAs<array<char,1>>(
              [&]( array<char,1>& ch_aObjects ){
                af_pProperty->alterAs<array<char,1>>(
                  [&]( array<char,1>& af_aObjects ){
                    for( u32 n=ch_aObjects.capacity(), i=0; i<n; ++i ){
                      if( memcmp( &ch_aObjects[ i ], &af_aObjects[ i ], af_aObjects.stride() )){
                        bResult = false;
                        break;
                      }
                    }
                  }
                );
              }
            );
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle the Handle property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isHandle() ){
            bool bResult = true;
            ch_pProperty->alterAs<Object::handle>(
              [&]( Object::handle& ch_hObject ){
                af_pProperty->alterAs<Object::handle>(
                  [&]( Object::handle& af_hObject ){
                    if( !deepCompare( ch_hObject, af_hObject )){
                      bResult = false;
                    }
                  }
                );
              }
            );
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle the String property type.
          //--------------------------------------------------------------------

          if( ch_pProperty->isType( e_classid<string>() )){
            bool bResult = true;
            ch_pProperty->alterAs<string>(
              [&]( string& ch_string ){
                af_pProperty->alterAs<string>(
                  [&]( string& af_string ){
                    if( ch_string != af_string ){
                      bResult = false;
                    }
                  }
                );
              }
            );
            return bResult;
          }

          //--------------------------------------------------------------------
          // Handle all other property types.
          //--------------------------------------------------------------------

          bool bResult = true;
          ch_pProperty->query(
            [&]( cvp ch_pData, const u32 ch_uSize ){
              af_pProperty->query(
                [&]( cvp af_pData, const u32 af_uSize ){
                  if( memcmp( ch_pData, af_pData, af_uSize )){
                    bResult = false;
                  }
                }
              );
            }
          );
          return bResult;
        }

        bool deepCompare( const Object::handle& hObjectA, const Object::handle& hObjectB ){
          if( hObjectA == hObjectB ){
            return true;
          }
          if( hObjectA->probeid() != hObjectB->probeid() ){
            return false;
          }
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
                ch_pProperty->alterAs<vector<Object::handle>>(
                  [&]( vector<Object::handle>& ch_vObjects ){
                    af_pProperty->alterAs<vector<Object::handle>>(
                      [&]( vector<Object::handle>& af_vObjects ){
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
                      }
                    );
                  }
                );
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
                ch_pProperty->alterAs<array<Object::handle,1>>(
                  [&]( array<Object::handle,1>& ch_aObjects ){
                    af_pProperty->alterAs<array<Object::handle,1>>(
                      [&]( array<Object::handle,1>& af_aObjects ){
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
                      }
                    );
                  }
                );
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
              ch_pProperty->alterAs<Object::handle>(
                [&]( Object::handle& ch_hObject ){
                  af_pProperty->alterAs<Object::handle>(
                    [&]( Object::handle& af_hObject ){
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
                    }
                  );
                }
              );
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
    //decompress:{                                |

      u64 Reader::decompress( cp dst, const u64 ndst, cvp src, const u64 nsrc ){
        if(( ndst <= ~0ULL )&&( nsrc <= ~0ULL )){
          return u64( LZ4_decompress_safe( ccp( src ), dst, int( nsrc ), int( ndst )));
        }
        return 0;
      }

    //}:                                          |
    //version:{                                   |

      u16 Reader::version( const u16 ver ){
        const u16 cmp = read<u16>();
        if( cmp != ver ){
          e_errorf( 987234982
            , "Version mismatch: %uv%u (%s)"
            , ver
            , cmp
            , ccp( m_sName )
          );
        }
        return cmp;
      }

    //}:                                          |
    //uncache:{                                   |

      string Reader::uncache(){
        string out;
        const auto key = read<u64>();
        if( !key ){
          return nullptr;
        }
        if( m_mStringTable.find( key )){
          Reader r( m_mStringTable[ key ]);
          r.setDictionary( m_vDictionary );
          out = r.unpackInternal();
        }
        return out;
      }

    //}:                                          |
    //exists:{                                    |

      bool Reader::exists( const string& tag )const{
        FILE* file = getPrefabFilePointer( m_sName );
        if( !file ){
          file =  getFilePointer( m_sName, tag, 1==m_tFlags->bNoExt );
        }
        if( !file ){
          e_msgf(
            "  Asset not found: \"%s\" with tag \"%s\"!"
            , ccp( m_sName )
            , ccp( tag ));
          return false;
        }
        const auto l = tag.len();
        cp pTag = new char[ l+1 ];
        fread( pTag, 1, l, file );
        fclose( file );
        pTag[ l ] = 0;
        const auto ok=( 0 == strcmp( pTag, tag.c_str() ));
        delete[] pTag;
        return ok;
      }

    //}:                                          |
    //read*:{                                     |
      //readPropertyVector:{                      |

        bool Reader::readPropertyVector( Object::prop_ptr& pProperty ){

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
                lines.resize( n );
                lines.foreach(
                  [&]( string& line ){
                    unpack( line );
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
                const auto n = read<u32>();
                pProperty->alterAs<vector<Object::handle>>(
                  [&]( vector<Object::handle>& objects ){
                    const auto k = objects.size();
                    if( k >= n ){
                      objects.foreach(
                        [&]( Object::handle& hObject ){
                          readProperties( hObject );
                        }
                      );
                    }else{
                      auto i=0u;
                      for( i=0; i<k; ++i ){
                        objects.alter( i,
                          [&]( Object::handle& hObject ){
                            readProperties( hObject );
                          }
                        );
                      }
                      for( i=k; i<n; ++i ){
                        objects.push();
                      }
                      for( i=k; i<n; ++i ){
                        objects.alter( i,
                          [&]( Object::handle& hObject ){
                            readProperties( hObject );
                          }
                        );
                      }
                    }
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

        bool Reader::readPropertyHandle( Object::prop_ptr& pProperty ){
          if( !pProperty ){
            return false;
          }
          if( pProperty->isContainer() ){
            return false;
          }
          if( !pProperty->isHandle() ){
            return false;
          }
          const auto bExists = read<u8>();
          if( !bExists ){
            return true;
          }
          const auto probeId = as<u64>();
          if( !Class::Factory::describe( probeId )){
            e_errorf( 198730234, "Couldn't describe id: %llx", probeId );
            return false;
          }
          skip( sizeof( u64 ));
          pProperty->alterAs<Object::handle>(
            [&]( Object::handle& hObject ){
              if( !hObject ){
                hObject = e_newt( probeId );
              }
              readProperties( hObject );
            }
          );
          return true;
        }

      //}:                                        |
      //readPropertyString:{                      |

        bool Reader::readPropertyString( Object::prop_ptr& pProperty ){
          if( pProperty->isType( e_classid<string>() )){
            pProperty->alterAs<string>(
              [&]( string& out ){
                unpack( out );
              }
            );
            return true;
          }
          return false;
        }

      //}:                                        |
      //readPropertyArray:{                       |

        bool Reader::readPropertyArray( Object::prop_ptr& pProperty ){

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
                const auto n = read<u32>();
                for( u32 i=0; i<n; ++i ){
                  readProperties( objects[ i ]);
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
              const auto capacity = read<u32>();
              const auto stride   = read<u32>();
              objects.alter( 0,
                [&]( char& _1st ){
                  char* pBuffer = &_1st;
                  for( u32 i=0; i<capacity; ++i ){
                    read( pBuffer, stride );
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
          const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );
          Object::prop_map out_mProperties;

          //--------------------------------------------------------------------
          // Validate sentinel value. Bail if greater than 1.
          //--------------------------------------------------------------------

          const auto hasObject = as<u8>();
          if( hasObject > 1 ){
            e_brk( "Bad sentinel value!" );
            return 0;
          }
          if( !hasObject ){
            return 0;
          }
          const auto start = m_tStream.tell();
          skip( 1 );

          //--------------------------------------------------------------------
          // Create hObject if it hasn't been alraedy. Think handle vectors.
          //--------------------------------------------------------------------

          if( !hObject ){
            // We're guaranteed the first eight bytes is the class identifier.
            const auto classId = as<u64>();
            if( !Class::Factory::describe( classId )){
              e_errorf( 871263, "Undescribed class id: %llx!", classId );
              return 1;
            }
            // Create an object and replace incoming.
            hObject = e_newt( classId );
            // Log this operation.
            if( usePropertyLogs ){
              e_msgf(
                "      Object created: class=\"%s\" (%llx)"
                , hObject->classof()
                , classId
              );
            }
          }else if( usePropertyLogs ){
            e_msgf( "Reader::readProperties( \"%s\" )", hObject->classof() );
            if( hObject.isa<Stream>() ){
              const auto& r = hObject.as<Stream>().cast();
              e_msgf( "\tPath: \"%s\"", ccp( r.toPath() ));
              e_msgf( "\tName: \"%s\"", ccp( r.toName() ));
              e_msgf( "\tSHA1: \"%s\"", ccp( r.toSHA1() ));
            }
          }

          //--------------------------------------------------------------------
          // Serialize in the object.
          //--------------------------------------------------------------------

          // Cast new object to reference.
          auto& object = hObject.cast();

          // Pre-serialize the object.
          object.preSerialize( *this );

          // Grab all the properties from the object. This replaces the
          // normal serialize() call because we're only loading props.
          object.getProperties(   out_mProperties );
          readPropertyMap( *this, out_mProperties );

          // Post serialize.
          object.postSerialize( *this );
          const auto stop = m_tStream.tell();
          return stop - start;
        }

      //}:                                        |
      //read:{                                    |

        u64 Reader::read( Object::prop_ptr& pProperty, const std::function<void( Object* )>& lambda ){
          const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );

          //--------------------------------------------------------------------
          // Bail conditions.
          //--------------------------------------------------------------------

          // If incoming property is read-only we never automatically serialize
          // it out to this stream.  Read-only includes buffers and bounds that
          // are never changed by the user.
          if( pProperty->isReadOnly() ){
            if( usePropertyLogs ){
              e_msgf(
                "    Skipping read-only property: \"%s\""
                , ccp( pProperty->toName() )
              );
            }
            return 0;
          }

          //--------------------------------------------------------------------
          // First thing's first: read in the property name aka the 'key'.
          //--------------------------------------------------------------------

          const auto start = m_tStream.tell();
          const auto propertyKey = unpack();
          if( usePropertyLogs ){
            e_msgf(
              "    Reading property: \"%s\""
              , ccp( propertyKey )
            );
          }

          //--------------------------------------------------------------------
          // Read in the caption and it's children if we're at that property.
          //--------------------------------------------------------------------

          u8 umask;
          if( pProperty->isCaption() ){
            goto sk;
          }
          umask = read<u8>();

          //--------------------------------------------------------------------
          // Read in most important flags.
          //--------------------------------------------------------------------

              pProperty->toFlags()->bHidden  =( MASK_HIDDEN  ==( umask & MASK_HIDDEN   ));
              pProperty->toFlags()->bKeyframe=( MASK_KEYFRAME==( umask & MASK_KEYFRAME ));
          if( pProperty->toFlags()->bKeyframe ){
              pProperty->toFlags()->bModified = 1;
          }

          //--------------------------------------------------------------------
          // Read in vector, handle, string or array property.
          //--------------------------------------------------------------------

          if( readPropertyVector( pProperty )){ goto sk; }
          if( readPropertyHandle( pProperty )){ goto sk; }
          if( readPropertyString( pProperty )){ goto sk; }
          if( readPropertyArray ( pProperty )){ goto sk; }

          //--------------------------------------------------------------------
          // Write out all other pod data.
          //--------------------------------------------------------------------

          pProperty->alter(
            [&]( vp data, const u32 size ){
              read( data, size );
            }
          );

          //--------------------------------------------------------------------
          // Write out all children.
          //--------------------------------------------------------------------

sk:       readPropertyMap(
              *this
            , pProperty->toChildren() );
          if( lambda ){
            lambda( (Object*)pProperty->toOuter() );
          }
          const auto stop = m_tStream.tell();
          return( stop - start );
        }

        u64 Reader::read( const u64 o, vp d, const u64 n ){
          m_tStream.query( o,
            [&]( ccp s ){
              memcpy( d, s, n );
            }
          );
          return n;
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
          const u8  data   = read<u8> ();
          st = stream( nullptr
            , stride
            , size );
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

      bool Reader::skip( const u64 n ){
        return m_tStream.skip( n );
      }

    //}:                                          |
    //load:{                                      |

      Reader& Reader::load( const string& tag ){

        //----------------------------------------------------------------------
        // Preloading message if tracing.
        //----------------------------------------------------------------------

        const auto useTracing = e_getCvar( bool, "USE_TRACING" );
        if( useTracing ){
          e_msgf(
            "\n\nPRELOADING '%s'"
            , ccp( tag )
          );
        }

        //----------------------------------------------------------------------
        // Open file and log the event.
        //----------------------------------------------------------------------

        FILE* pFile = nullptr;
        if( IEngine::prefabs.empty() ){
          if( useTracing ){
            e_msgf( "Pre-prefab attempt: %s (from disk)", ccp( m_sName ));
          }
          pFile = getFilePointer( m_sName, tag, 1==m_tFlags->bNoExt );
          if( !pFile && useTracing ){
            e_msgf( "Failures to load: %s", ccp( m_sName ));
          }
        }else{
          if( useTracing ){
            e_msgf( "Attempting to load: %s (from prefab)", ccp( m_sName ));
          }
          pFile = getPrefabFilePointer( m_sName );
          if( !pFile ){
            if( useTracing ){
              e_msgf( "Attempting to load: %s (from disk)", ccp( m_sName ));
            }
            pFile = getFilePointer( m_sName, tag, 1==m_tFlags->bNoExt );
            if( !pFile ){
              if( useTracing ){
                e_msgf( "Attempting to load: %s (from cache)", ccp( m_sName ));
              }
              pFile = e_fopen( "~/.eon/" + m_sName.basename() + ".eon", "rb" );
              if( !pFile && useTracing ){
                e_msgf( "Failures to load: %s", ccp( m_sName ));
              }
            }
          }
        }

        //----------------------------------------------------------------------
        // File now open so decompress if possible.
        //----------------------------------------------------------------------

        m_tFlags->bError = !pFile;
        if( isError() ){
          if( useTracing ){
            e_msgf(
              "$(red)Error in preload: '%s' not found in prefabs, on disk or the cache."
              , ccp( tag )
            );
          }
          return *this;
        }
        if( !tag.empty() ){

          //--------------------------------------------------------------------
          // First of all check that the tags match.
          //--------------------------------------------------------------------

          if( useTracing ){
            e_msgf( "Loading: %s (%s)", ccp( m_sName ), ccp( tag ));
          }
          #if !e_compiling( clang )
            char* aLabel = new char[ tag.len()+1 ];
          #else
            char aLabel[ tag.len()+1 ];
          #endif
          aLabel[ tag.len() ] = 0;
          fread( aLabel, tag.len(), 1, pFile );
          const bool isMatch=( tag == aLabel );
          const string stag = aLabel;
          #if !e_compiling( clang )
            delete[] aLabel;
          #endif
          if( !isMatch ){
            if( useTracing ){
              e_msgf(
                "$(red)Error matching: '%s' versus '%s'"
                , ccp( stag )
                , ccp( tag )
              );
            }
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
          u64 packedSize;
          #if e_compiling( debug )
            u64 packed_size_result =
          #endif
          fread( &packedSize, 1, sizeof( packedSize ), pFile );
          e_assert( packed_size_result == sizeof( packedSize ));
          e_assert( packedSize <= unpackedSize );

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

          m_tStream.end(
            [&]( ccp pEnd ){
              const u8  ndict = (( u8* )pEnd)[ unpackedSize - 1 ];
              const u8* pdict = (( u8* )pEnd )+unpackedSize - 1 - ndict;
              m_vDictionary.resize( ndict );
              m_vDictionary.alter( 0,
                  [&]( u8& dict ){
                  memcpy( &dict, pdict, ndict );
                }
              );
            }
          );
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
        m_tStream.reset();
        fclose( pFile );
        if( useTracing ){
          e_msgf(
            "$(green)Success$(off) loading '$(lightgreen)%s$(off)'!"
            , ccp( tag )
          );
        }
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

    Reader::Reader( const Reader& lvalue )
        : m_mStringTable( lvalue.m_mStringTable )
        , m_vDictionary( lvalue.m_vDictionary )
        , m_tStream( lvalue.m_tStream )
        , m_sName( lvalue.m_sName )
        , m_tFlags( lvalue.m_tFlags ){
    }

    Reader::Reader( Reader&& rvalue )
        : m_mStringTable( std::move( rvalue.m_mStringTable ))
        , m_vDictionary(  std::move( rvalue.m_vDictionary ))
        , m_tStream(      std::move( rvalue.m_tStream ))
        , m_sName(        std::move( rvalue.m_sName )){
      m_tFlags = rvalue.m_tFlags;
      rvalue.m_tFlags.all = 0;
    }

    Reader::Reader( const stream& st )
        : m_tStream( st )
        , m_sName( string::streamId() ){
      m_tStream.reset();
    }

  //}:                                            |
  //Dtor:{                                        |

    Reader::~Reader(){}

  //}:                                            |
//}:                                              |
//================================================+=============================
