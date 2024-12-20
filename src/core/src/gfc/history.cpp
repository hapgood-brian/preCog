//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

using namespace EON;
using namespace gfc;
using namespace fs;

//================================================+=============================
//History:{                                       |
  //Options:{                                     |

#ifdef __APPLE__
  #pragma mark - Options -
#endif

    namespace{
      const bool e_distinct(cvars)[]{
        e_setCvar( "USE_HISTORY_PROPIGATION", true ),
      };
    }

  //}:                                            |
  //Aliases:{                                     |

    using StatePair = std::pair<History::State,History::State>;

  //}:                                            |
  //Private:{                                     |
    //getReaderFromState:{                        |

#ifdef __APPLE__
  #pragma mark - Private methods -
#endif

      namespace{
        Reader getReaderFromState( const History::State& state ){

          //--------------------------------------------------------------------
          // Build the string tabel and dictionary first.
          //--------------------------------------------------------------------

          Reader::StringTable stringTable;
          Reader::Dictionary dictionary;
          const stream& st = state;
          st.query(
            [&]( ccp pBuffer ){
              const auto dn = *reinterpret_cast<const u8*>( pBuffer+st.size()-1 );
              if( dn ){
                // Read the dictionary characters.
                dictionary.resize( dn );
                dictionary.alter( 0,
                  [&]( u8& data ){
                    memcpy( &data, pBuffer+st.size()-1-dn, dn );
                  }
                );
                // Read the string table.
                auto pTable  = pBuffer+st.size()-1-dn-4;
                auto nTable  = *reinterpret_cast<const u32*>( pTable );
                auto pEnd    = pTable;
                     pTable -= nTable;
                while( pTable < pEnd ){
                  const auto k = *(u64*)pTable; pTable += 8;
                  const auto s = *(u64*)pTable; pTable += 8;
                  const auto n = *(u64*)pTable; pTable += 8;
                  if( *pTable++ ){
                    stringTable.setBy( k,
                      [&]( stream& out ){
                        memcpy( out.realloc( s*n ), pTable, size_t( s*n ));
                        pTable += s*n;
                      }
                    );
                  }
                }
              }
            }
          );

          //--------------------------------------------------------------------
          // Serialize object by reader.
          //--------------------------------------------------------------------

          Reader fs( st );
          fs.toStringTable() = std::move( stringTable );
          fs.toDictionary()  = std::move( dictionary  );
          fs.toFlags()->bPlayback = 1;
          return fs;
        }
      }

    //}:                                          |
    //applyChanges2Object:{                       |

      namespace{
        void applyChanges2Object( const History::State& now, Object::handle& hObject ){
          getReaderFromState( now ).readProperties( hObject );
        }
      }

    //}:                                          |
    //recordChanges:{                             |

      namespace{
        History::State recordChanges(
              const u64             uAnimTickHead
            , const Object::handle& hObject
            , const bool            bScanning ){
          History::State state;
          // Create write file system.
          Writer fs( string::null, 0 );
          fs.toFlags()->bRecording = 1;
          fs.toFlags()->bScanning  = bScanning;
          // Deep traversal including all meshes, materials, shaders etc.
          fs.writeProperties( hObject );
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
    //binarySearch:{                              |

      namespace{
        History::Timeline::iterator binarySearch( const u64 animTick, const History::Timeline& timeline ){
          auto it = timeline.binarySearch(
            [&]( const History::State& state )->s32{
              if( animTick < state.toAnimTick() ){
                return -1;
              }
              if( animTick > state.toAnimTick() ){
                return 1;
              }
              return 0;
            }
          );
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
          * The job of this function is to take a change state and apply it
          * to to all the following frames to the end of time. This is
          * expensive of course but necessary because the key frames are in a
          * delta compressed state.
          *
          * There are two cases we need to be concerned about: we're
          * inserting a new change between two existing states; and we're
          * changing a state between two states.
          *
          * The first big task is to figure get the uncompressed state for
          * the state after the change's position in the timeline and the
          * unpacked state for the change itself. Now this is tricky because
          * the change may have already blown away the deltas that the after
          * state needs.
          *
          * To get around this we need to pass in those after-before and the
          * unpacked changes state.
          *
          * \param before The uncompressed state that after references in
          * from.
          * \param change The uncompressed state with latest changes.
          * \param after  The   compressed state that comes after.
          */

        StatePair propigate( const u64 clsid, const History::State& before, const History::State& change, History::State& after, History::Timeline& timeline ){

          StatePair result;

          //------------------------------------------------------------------
          // Build the AFter state.
          //------------------------------------------------------------------

          // Simple abbreviated states.
          const auto&    bf = before;
          const auto&    ch = change;
          History::State af = after;
          result.first = af;

          //------------------------------------------------------------------
          // Perform three-way merge using reader objects.
          //------------------------------------------------------------------

          // Create readrs for all three states.
          Reader bfr( getReaderFromState( bf ));
          Reader chr( getReaderFromState( ch ));
          Reader afr( getReaderFromState( af ));

          // Create a temporary object to stream into.
          Object::handle hObject = e_newt( clsid );

          // Now tell AFter reader to merge other.
          afr.mergeProperties( hObject, bfr, chr );

          // Convert AFter back into stream.
          af = recordChanges( after.toAnimTick(), hObject, false );

          //------------------------------------------------------------------
          // Compress merged changes.
          //------------------------------------------------------------------

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
        m_dTimeline.foreach(
          [&]( const State& state ){
            state.serialize( fs );
          }
        );
      }

      s64 History::serialize( Reader& fs ){
        const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );
        super::serialize( fs );
        fs.version( OBJECT_HISTORY_VERSION );
        fs >> m_uHead;
        fs >> m_uTail;
        const auto n = fs.read<u32>();
        if( usePropertyLogs ){
          e_msgf( "  Will stream %u states into history object", n );
        }
        for( u32 i=0; i<n; ++i ){
          m_dTimeline.pushBy(
            [&]( State& state ){
              if( usePropertyLogs ){
                e_msgf( "  Serializing state!" );
              }
              state.serialize( fs );
            }
          );
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
        if( !m_sUUID ){
          return false;
        }
        State changes = recordChanges(
            m_uHead
          , m_sUUID
          , m_tFlags->bScanning );
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

        State changes;
        //std::lock_guard _lock( m_tMutex );
        if( e_getCvar( bool, "USE_PROPERTY_LOGS" )){
          e_msgf( "\t\tRecording changes:" );
        }
        if( !record( changes )||changes->empty() ){
          if( e_getCvar( bool, "USE_PROPERTY_LOGS" )){
            e_msgf( "\t\t\tNo changes!" );
          }
          return false;
        }

        //----------------------------------------------------------------------
        // Replace existing state with recorded state and optionally propigate.
        //----------------------------------------------------------------------

        const auto useHistoryPropigation = e_getCvar( bool, "USE_HISTORY_PROPIGATION" );
        const auto useTracing = e_getCvar( bool, "USE_PROPERTY_LOGS" );
        const auto clsid = hObject->toOwner();
        History::Timeline::iterator r;
        if( useHistoryPropigation ){
          r = binarySearch( m_uHead, m_dTimeline );
          if( r ){
            *r = changes;
            if( m_uHead < m_uTail ){
              if( m_uHead ){
                if( useTracing ){
                  e_msgf( "\t\t\tPropigating changes." );
                }
                propigate( clsid, *(r-1), *r, r );
              }
            }
            return true;
          }
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
        if( useHistoryPropigation ){
          r = binarySearch( m_uHead, m_dTimeline );
          if( r && ( r+1 ) && ( r-1 )){
            // Are we inserting between two states?
            if(/* yes */!r->isAnimTick( m_uTail )){
              if( useTracing ){
                e_msgf( "\t\t\tPropigating changes." );
              }
              // Apply temporal replication to future states.
              propigate( clsid, *(r-1), *r, r );
            }
          }
        }

        // Set the tail to the max game turn.
        m_uTail = e_max( m_uHead, m_uTail );
        return true;
      }

    //}:                                          |
    //track:{                                     |

      void History::track( const Object::handle& hObject ){
        m_sUUID = hObject;
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
        std::lock_guard _lock( m_tMutex );

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

        // If not IO complete then bail out.
        const auto usePropertyLogs = e_getCvar( bool, "USE_PROPERTY_LOGS" );
        if( !isIOComplete() ){
          if( usePropertyLogs ){
            e_msgf( "$(red)History stream is IO incomplete!" );
          }
          return;
        }

        // Bail out if UUID is null.
        if( !m_sUUID ){
          if( usePropertyLogs ){
            e_msgf( "$(red)NO UUID!" );
          }
          return;
        }

        // Bail out if the object isn't IO complete. Otherwise the competing
        // stream will stomp all over this one.
        Object::handle hObject( m_sUUID );
        if( !hObject->isIOComplete() ){
          if( usePropertyLogs ){
            e_msgf( "$(red)DANGER: $(green)Object incomplete!" );
          }
        }

        //----------------------------------------------------------------------
        // Output property logs.
        //----------------------------------------------------------------------

        if( usePropertyLogs ){
          e_msgf(
            "History::seek( %llu )"
            , zeroBasedFrameIndex
          );
        }

        //----------------------------------------------------------------------
        // Get the new AnimTick and check if it's changed.
        //----------------------------------------------------------------------

        const auto seekTick = m_tFlags->bActive
            ? zeroBasedFrameIndex : ( zeroBasedFrameIndex < m_uTail )
            ? zeroBasedFrameIndex : m_uTail;
        if( seekTick > m_uTail ){
          if( usePropertyLogs ){
            e_msgf( "$(orange)Bailout: $(green)seekTick > m_uTail!" );
          }
          m_uHead = seekTick;
          return;
        }

        //----------------------------------------------------------------------
        // Set the head to new game turn; if seek and tail are the same then we
        // will always apply changes to object.  This is for the first build of
        // an empty history that's just been loaded.
        //----------------------------------------------------------------------

        const auto bChanged=( seekTick != m_uHead )||( seekTick == m_uTail );
        m_uHead = seekTick;
        if( bChanged ){
          if( usePropertyLogs ){
            e_msgf( "$(green)Changed: $(yellow)attempting to get the range!" );
          }
          auto it = m_dTimeline.getIterator();
          if( !it ){
            if( usePropertyLogs ){
              e_msgf( "$(red)Can't get iterator: $(green)bailing out!" );
            }
            return;
          }
          if( usePropertyLogs ){
            e_msgf( "$(blue)rangeOf( %llu )", m_uHead );
          }
          rangeOf( m_uHead, it );
          if( !it ){
            if( usePropertyLogs ){
              e_msgf( "$(red)NO ITERATOR: Couldn't grab the range of application!" );
            }
          }else{
            if( usePropertyLogs ){
              e_msgf( "\t\tApplying changes from history object." );
            }
            Object::handle hObject = m_sUUID;
            applyChanges2Object( *it
              , hObject
            );
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
//================================================+=============================
