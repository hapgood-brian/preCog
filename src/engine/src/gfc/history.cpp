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

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================|=============================
//History:{                                       |
  //Aliases:{                                     |

    using StatePair = std::pair<History::State,History::State>;

  //}:                                            |
  //Private:{                                     |
    //getTailLengthFromState:{                    |

#ifdef __APPLE__
  #pragma mark - Private methods -
#endif

      #if 0
        namespace{
          u32 getTailLengthFromState( const History::State& state ){
            u32 result = 0;
            state->query( [&]( ccp pBuffer ){
              const u32 n = *reinterpret_cast<const  u8*>( pBuffer+state->size()-1     );
              const u32 z = *reinterpret_cast<const u32*>( pBuffer+state->size()-1-n-4 );
              result = 1+n+4+z;
            });
            return result;
          }
        }
      #endif

    //}:                                          |
    //getReaderFromState:{                        |

      namespace{

        Reader getReaderFromState( const History::State& state ){

          //--------------------------------------------------------------------
          // Build the string tabel and dictionary first.
          //--------------------------------------------------------------------

          Reader::StringTable stringTable;
          Reader::Dictionary dictionary;
          const stream& st = state;
          st.query( [&]( ccp pBuffer ){
            const u8 dn = *reinterpret_cast<const u8*>( pBuffer+st.size()-1 );
            if( dn ){
              // Read the dictionary characters.
              dictionary.resize( dn );
              dictionary.alter( 0, [&]( u8& data ){
                memcpy( &data, pBuffer+st.size()-1-dn, dn );
              });
              // Read the string table.
              ccp pTable  = pBuffer+st.size()-1-dn-4;
              u32 nTable  = *reinterpret_cast<const u32*>( pTable );
              ccp pEnd    = pTable;
                  pTable -= nTable;
              while( pTable < pEnd ){
                const u64 k = *(u64*)pTable; pTable += 8;
                const u64 s = *(u64*)pTable; pTable += 8;
                const u64 n = *(u64*)pTable; pTable += 8;
                if( *pTable++ ){
                  stringTable.setBy( k, [&]( stream& out ){
                    memcpy( out.realloc( s*n ), pTable, size_t( s*n ));
                    pTable += s*n;
                  });
                }
              }
            }
          });

          //--------------------------------------------------------------------
          // Serialize object by reader.
          //--------------------------------------------------------------------

          Reader fs( st );
          fs.toFlags()->bPlayback = 1;
          fs.toStringTable() = std::move( stringTable );
          fs.setDictionary( std::move( dictionary ));
          return fs;
        }
      }

    //}:                                          |
    //appendStateStrings:{                        |

      #if 0
        namespace{
          void appendStateStrings( stream& out, const History::State& bf, const History::State& ch, const History::State& af ){

            //------------------------------------------------------------------
            // Merge all three string tables.
            //------------------------------------------------------------------

            const Reader readers[3]{
              getReaderFromState( bf ),
              getReaderFromState( ch ),
              getReaderFromState( af ),
            };
            string s;
            Writer w;
            for( u32 i=0; i<3; ++i ){
              readers[i].toStringTable().foreachKV( [&]( const u64 k0, const stream& v0 ){
                bool bFound = false;
                w.toStringTable().foreachKV( [&]( const u64 k1, const stream& v1 ){
                  if( k0 == k1 ){
                    bFound = true;
                  }
                });
                if( !bFound ){
                  Reader r( v0 );
                  r.setDictionary( readers[i].toDictionary() );
                  r.unpack( s );
                  w.cache( s );
                }
              });
            }

            //------------------------------------------------------------------
            // Append the string table.
            //------------------------------------------------------------------

            Writer fs( out, 0 );
            u64 bytes = 0;
            w.toStringTable().foreachKV( [&]( const u64 k, const stream& v ){
              bytes += fs.write( k );
              bytes += fs.write( v );
            });
            fs.write<u32>( bytes & 0xffffffff );

            //------------------------------------------------------------------
            // Append the dictionary.
            //------------------------------------------------------------------

            const Writer::Dictionary& D = w.toCache()->toDictionary();
            if( D.empty() ){
              fs.write<u8>( 0 );
            }else{
              D.query( 0, [&]( const u8& d ){
                fs.write( &d, D.size() );
                fs.write( u8( D.size() ));
              });
            }
            out = fs.toStream();
          }
        }
      #endif

    //}:                                          |
    //applyChanges2Object:{                       |

      namespace{
        void applyChanges2Object( const History::State& now, Object::handle& hObject, const History::StreamIn& lambda ){
          Reader fs( getReaderFromState( now ));
          fs.readProperties( hObject );
          if( lambda ){
            lambda( fs );
          }
        }
      }

    //}:                                          |
    //recordChanges:{                             |

      namespace{
        History::State recordChanges(
              const u64                 uAnimTickHead
            , const Object::handle&     hObject
            , const bool                bScanning
            , const History::StreamOut& lambda ){
          History::State state;
          // Create write file system.
          Writer fs;
          fs.toFlags()->bRecording = 1;
          fs.toFlags()->bScanning  = bScanning;
          // Deep traversal including all meshes, materials, shaders etc.
          fs.writeProperties( hObject );
          if( lambda ){
            lambda( fs );
          }
          // Append the string table.
          u64 bytes = 0;
          fs.toStringTable().foreachKV(
            [&]( const u64 k, const stream& v ){
              bytes += fs.write( k );
              bytes += fs.write( v );
            }
          );
          fs.write<u32>( bytes & 0xffffffff );
          // Append the dictionary.
          if( !fs.toCache() ){
            fs.write<u8>( 0 );
          }else{
            const Writer::Dictionary& D = fs.toCache()->toDictionary();
            if( D.empty() ){
              fs.write<u8>( 0 );
            }else{
              D.query( 0, [&]( const u8& d ){
                fs.write( &d, D.size() );
                fs.write( u8( D.size() ));
              });
            }
          }
          state = std::move( fs.toStream() );
          state.setAnimTick( uAnimTickHead );
          return state;
        }
      }

    //}:                                          |
    //replayChanges:{                             |

      #if 0
        namespace{
          stream replayChanges( const stream& unpacked, const stream& state ){
            if( !state.isPartial() ){
              return state;
            }
            stream results;
            state.query(
              [&]( ccp p_src ){
                u8* src = (u8*)p_src;
                u8* end = src + state.size();
                #if e_compiling( sanity )
                  ccp san = pSanity ? pSanity->data() : nullptr;
                #endif
                results = unpacked;
                results.query(
                  [&]( ccp p_dst ){
                    u8* dst = (u8*)p_dst;
                    while( src < end ){
                      const u8 tok = *((u8*&)src)++;
                      if( tok < 0x80 ){
                        #if e_compiling( sanity )
                          if( san ){
                            e_sanity_check( !memcmp( dst, san, tok ));
                            san += tok;
                          }
                        #endif
                        dst += tok;
                      }else{
                        e_sanity_check( tok & 0x80, "Broken high bit!" );
                        const u8 nn = ( tok & 0x7F );
                        for( u8 i=0; i<nn; ++i ){
                          *dst = *src+*dst;
                          #if e_compiling( sanity )
                            if( san ){
                              e_sanity_check( *dst == *san );
                              ++san;
                            }
                          #endif
                          ++dst;
                          ++src;
                        }
                      }
                    }
                  }
                );
              }
            );
            return results;
          }
          stream replayChanges( const History::State& state, const History::Timeline::iterator& it ){
            return replayChanges( state, *it );
          }
        }
      #endif

    //}:                                          |
    //binarySearch:{                              |

      namespace{
        History::Timeline::iterator binarySearch( const u64 animTick, const History::Timeline& timeline ){
          auto it = timeline.binarySearch( [&]( const History::State& state )->s32{
            if( animTick < state.toAnimTick() ){
              return -1;
            }
            if( animTick > state.toAnimTick() ){
              return 1;
            }
            return 0;
          });
          return it;
        }
      }

    //}:                                          |
    //slowRangeOf:{                               |

      namespace{
        s32 slowRangeOf( const u64 uAnimTick, History::Timeline::iterator& i ){
          auto it = i;
          s32  ii = 0;
          while( it ){
            if( it->isAnimTick( uAnimTick )){
              s32 j = 0;
              while(( j > -ii )&&( it+j )->toStream()->isPartial() ){
                --j;
              }
              i = it;
              return j;
            }
            if( it->toAnimTick() > uAnimTick ){
              break;
            }
            ++ii;
            ++it;
          }
          const s32 jj = ii-1;
          i = i + jj;
          return -jj;
        }
      }

    //}:                                          |
    //calcDeltas:{                                |

      #if 0
        namespace{
          stream calcDeltas( const stream& prev, const stream& curr ){
            stream result;
            curr.query(
              [&]( ccp a ){
                u8* pDst = (u8*)a;
                prev.query(
                  [&]( ccp b ){
                    const u8* pSrc = (u8*)b;
                    const u8* pEnd = pSrc+prev.size();
                    while( pSrc < pEnd ){
                      *result.realloc( 1 )=( *pDst-*pSrc );
                      ++pDst;
                      ++pSrc;
                    }
                  }
                );
              }
            );
            return result;
          }
        }
      #endif

    //}:                                          |
    //propigate:{                                 |

      namespace{

        /** \brief Change replication.
          *
          * The job of this function is to take a change state and apply it to
          * to all the following frames to the end of time. This is expensive
          * of course but necessary because the key frames are in a delta
          * compressed state.
          *
          * There are two cases we need to be concerned about: we're inserting
          * a new change between two existing states; and we're changing a
          * state between two states.
          *
          * The first big task is to figure get the uncompressed state for the
          * state after the change's position in the timeline and the unpacked
          * state for the change itself. Now this is tricky because the change
          * may have already blown away the deltas that the after state needs.
          *
          * To get around this we need to pass in those after-before and the
          * unpacked changes state.
          *
          * \param before The uncompressed state that after references in from.
          * \param change The uncompressed state with latest changes.
          * \param after  The   compressed state that comes after.
          */

        StatePair propigate( const u64 clsid, const History::State& before, const History::State& change, History::State& after, History::Timeline& timeline ){

          StatePair result;

          //--------------------------------------------------------------------
          // Build the AFter state.
          //--------------------------------------------------------------------

          // Simple abbreviated states.
          const auto&    bf = before;
          const auto&    ch = change;
          History::State af = after;
          result.first = af;

          //--------------------------------------------------------------------
          // Perform three-way merge using reader objects.
          //--------------------------------------------------------------------

          // Create readrs for all three states.
          Reader bfr( getReaderFromState( bf ));
          Reader chr( getReaderFromState( ch ));
          Reader afr( getReaderFromState( af ));

          // Create a temporary object to stream into.
          Object::handle hObject = e_newt( clsid );

          // Now tell AFter reader to merge other.
          afr.mergeProperties( hObject, bfr, chr );

          // Convert AFter back into stream.
          af = recordChanges( after.toAnimTick(), hObject, false, nullptr );

          //--------------------------------------------------------------------
          // Compress merged changes.
          //--------------------------------------------------------------------

          result.second = after = af;
          result.second.setAnimTick( af.toAnimTick() );
          return result;
        }

        void propigate( const u64 clsid, const History::State& before, const History::State& change, const History::Timeline::iterator& it ){
          auto               af = History::Timeline::iterator( it );
          History::Timeline& dq = *af.container();
          History::State     bf = before;
          History::State     ch = change;
          while( ++af ){
            const StatePair& sp = propigate( clsid, bf, ch, *af, dq );
            ch = sp.second;
            bf = sp.first;
          }
        }
      }

    //}:                                          |
    //rangeOf:{                                   |

      namespace{

        s32 rangeOf( const u64 uAnimTick, History::Timeline::iterator& i ){

          //--------------------------------------------------------------------
          // Bail conditions.
          //--------------------------------------------------------------------

          if( !i ){
            return 1;
          }

          //--------------------------------------------------------------------
          // Did we click on a keyframe?
          //--------------------------------------------------------------------

          auto r = binarySearch( uAnimTick, *i.container() );
          if( !r ){
            return slowRangeOf( uAnimTick, i );
          }

          //--------------------------------------------------------------------
          // Great, scan backwards from r looking for last non-partial state.
          //--------------------------------------------------------------------

          const s32 ii = r.index();
          s32 j = 0;
          while(( j > -ii )&&( r+j )->toStream()->isPartial() ){
            --j;
          }
          i = r;
          return j;
        }
      }

    //}:                                          |
    //rleZ:{                                      |

      #if 0
        namespace{
          // First byte of a run tells us to type of run. If high bit is set 0x80
          // then we'll copy the next lower 7 bits (127) worth of data. If said
          // bit is NOT set then we'll compress the next 7 bits worth of data
          // down to nothing.
          stream rleZ( const stream& in ){
            stream st;
            in.query(
              [&]( ccp s ){
                ccp e = s + in.size();
                while( s < e ){
                  if( !*s ){
                    u32 n = 0;
                    while( !*s && s < e ){
                      ++n;
                      ++s;
                    }
                    if( s < e ){
                      for( u32 k=n/0x7F, i=0; i<k; ++i ){
                        *reinterpret_cast<u8*>( st.realloc( 1 )) = 0x7F;
                      }
                      if( n % 0x7F ){
                        *reinterpret_cast<u8*>( st.realloc( 1 )) = n % 0x7F;
                      }
                    }
                  }else{
                    ccp p = s;
                    u32 n = 0;
                    while( *s && ( n < 0x7F ) && ( s < e )){
                      ++n;
                      ++s;
                    }
                    *reinterpret_cast<u8*>( st.realloc( 1 ))=( n & 0xFF ) | 0x80;
                    u8* blk = reinterpret_cast<u8*>( st.realloc( n ));
                    memcpy( blk, p, n );
                  }
                }
                st.setIsPartial( true );
              }
            );
            return st;
          }
        }
      #endif

    //}:                                          |
  //}:                                            |
  //Structs:{                                     |
    //State:{                                     |
      //Methods:{                                 |
        //serialize:{                             |

#ifdef __APPLE__
  #pragma mark - History state -
#endif

          void History::State::serialize( Writer& fs )const{
            fs << m_timeAnimTick;
            if( m_dataStream ){
              fs << u8( 1 );
              fs << *m_dataStream.get();
            }else{
              fs << u8( 0 );
            }
          }

          void History::State::serialize( Reader& fs ){
            fs >> m_timeAnimTick;
            if( fs.read<u8>() ){
              m_dataStream = std::make_shared<stream>();
              fs >> *m_dataStream.get();
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Extends:{                                     |

#ifdef __APPLE__
  #pragma mark - History -
#endif

    e_extends( History );

  //}:                                            |
  //Methods:{                                     |
    //eraseChanges:{                              |

      void History::eraseChanges(){
        const u64 bPlayback = m_tFlags->bPlayback;
        m_tFlags->bPlayback = 1;
        seek( m_uHead );
        m_tFlags->bPlayback = bPlayback & 1;
      }

    //}:                                          |
    //serialize:{                                 |

      void History::serialize( Writer& fs )const{
        super::serialize( fs );
        fs.version( OBJECT_HISTORY_VERSION );
        fs << m_uHead;
        fs << m_uTail;
        fs << m_dTimeline.size();
        m_dTimeline.foreach( [&]( const State& state ){
          state.serialize( fs );
        });
      }

      s64 History::serialize( Reader& fs ){
        super::serialize( fs );
        fs.version( OBJECT_HISTORY_VERSION );
        fs >> m_uHead;
        fs >> m_uTail;
        const u32 n = fs.read<u32>();
        for( u32 i=0; i<n; ++i ){
          m_dTimeline.pushBy( [&]( State& state ){
            state.serialize( fs );
          });
        }
        m_tFlags->bPlayback = 1;
        return UUID;
      }

    //}:                                          |
    //tracking:{                                  |

      Object::handle History::tracking()const{
        return m_sUUID;
      }

    //}:                                          |
    //record:{                                    |

      bool History::record( State& out ){

        //----------------------------------------------------------------------
        // Bail out if the UUID is zero.
        //----------------------------------------------------------------------

        if( !m_sUUID ){
          return false;
        }

        //----------------------------------------------------------------------
        // Construct a new stream from the current object property state.
        //----------------------------------------------------------------------

        // Record changes into new state and save to output state.
        State changes = recordChanges( m_uHead, m_sUUID, m_tFlags->bScanning, m_onWriter );

        // Compare changes against previous recording.
        auto it = m_dTimeline.getIterator();
        if( it ){
          rangeOf( m_uHead, it );
          if( *it == changes ){
            return false;
          }
        }

        // Record "from" and "anim" ticks into returning state.
        changes.setAnimTick( m_uHead );
        out = std::move( changes );
        return true;
      }

      bool History::record(){

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

        // If history not IO complete then bail out.
        if( !isIOComplete() ){
          return false;
        }

        // Bail out if the object isn't valid and IO complete.
        Object::handle hObject( m_sUUID );
        if( !hObject.valid() ){
          return false;
        }
        if( !hObject->isIOComplete() ){
          return false;
        }

        //----------------------------------------------------------------------
        // Record potential changes in now and out.
        //----------------------------------------------------------------------

        // Grab the class identifier for tracking object.
        const u64 clsid = hObject->probeid();

        // Record frame and bail if nothing changed.
        State changes;
        if( !record( changes ) || changes->empty() ){
          return false;
        }

        //----------------------------------------------------------------------
        // Replace existing state with recorded state and optionally propigate.
        //----------------------------------------------------------------------

        auto r = binarySearch( m_uHead, m_dTimeline );
        if( r ){
          *r = changes;
          if( m_uHead < m_uTail ){
            if( m_uHead ){
              propigate( clsid, *(r-1), *r, r );
            }
          }
          return true;
        }

        //----------------------------------------------------------------------
        // Add new recorded state and optionally propigate.
        //----------------------------------------------------------------------

        // Add new recorded state to the timeline then sort the timeline so
        // states are sequential based on animation tick.
        m_dTimeline.push( changes );
        m_dTimeline.sort(
          []( const State& a, const State& b ){
            return( a.toAnimTick() < b.toAnimTick() );
          }
        );

        // Get iterator for head after insertion.
        r = binarySearch( m_uHead, m_dTimeline );
        if( r && ( r+1 ) && ( r-1 )){
          // Are we inserting between two states?
          if(/* yes */!r->isAnimTick( m_uTail )){
            // Apply temporal replication to future states.
            propigate( clsid, *(r-1), *r, r );
          }
        }

        // Set the tail to the max game turn.
        m_uTail = e_max( m_uHead, m_uTail );
        return true;
      }

    //}:                                          |
    //track:{                                     |

      void History::track( const Object::handle& hObject ){
        // You cannot track yourself or track what we're already tracking.
        if(( hObject != UUID )&&( m_sUUID != hObject )){
          m_dTimeline.clear();
          m_sUUID = hObject;
        }
      }

    //}:                                          |
    //scan:{                                      |

      bool History::scan(){
        m_tFlags-> bScanning = 1;
        seek( Object::gameTurn );
        const bool b = record( );
        m_tFlags-> bScanning = 0;
        return b;
      }

    //}:                                          |
    //seek:{                                      |

      void History::seek( const u64 zeroBasedFrameIndex ){

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

        // If not IO complete then bail out.
        if( !isIOComplete() ){
          return;
        }

        // Bail out if UUID is null.
        if( !m_sUUID ){
          return;
        }

        // Bail out if the object isn't IO complete.
        Object::handle hObject( m_sUUID );
        if( !hObject->isIOComplete() ){
          return;
        }

        // Get the new AnimTick and check if it's changed.
        const u64 seekTick = m_tFlags->bActive
            ? zeroBasedFrameIndex : ( zeroBasedFrameIndex < m_uTail )
            ? zeroBasedFrameIndex : m_uTail;
        if( seekTick > m_uTail ){
          m_uHead = seekTick;
          return;
        }

        // Set the head to new game turn; if seek and tail are the same then we
        // will always apply changes to object.  This is for the first build of
        // an empty history that's just been loaded.
        const bool bChanged=( seekTick != m_uHead )||( seekTick == m_uTail );
        m_uHead = seekTick;
        if( bChanged ){
          auto it = m_dTimeline.getIterator();
          rangeOf( m_uHead, it );
          if( it ){
            Object::handle hObject( m_sUUID );
            applyChanges2Object( *it, hObject, m_onReader );
          }
        }
      }

    //}:                                          |
    //ff:{                                        |

      void History::ff( const u64 delta ){
        // TODO: step back one frame by adding recorded deltas.
      }

    //}:                                          |
    //rw:{                                        |

      void History::rw( const u64 delta ){
        // TODO: step back one frame by adding recorded deltas in reverse.
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

    History::History( const s64 uuid )
      : m_sUUID( uuid )
    {}

  //}:                                            |
  //Dtor:{                                        |

    History::~History()
    {}

  //}:                                            |
//}:                                              |
//================================================|=============================
