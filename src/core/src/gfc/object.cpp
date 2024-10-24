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
//Property:{                                      |
  //Methods:{                                     |
    //sendChanges:{                               |

      void Property::sendChange()const{}

    //}:                                          |
    //clone:{                                     |

      bool Property::clone(){
        e_guardw( m_tLock );
        if( !Class::Factory::valid( m_pOuter->UUID )){
          return false;
        }
        if( !m_pData ){
          return false;
        }
        u32 size = 0;
        if( isVector() ){
          size = sizeof( vector<char> );
        }else if( isArray() ){
          const array<char,1>& a = *reinterpret_cast<const array<char,1>*>( m_pData );
          size = sizeof( a )+toSize()*u32( a.capacity()-1 );
        }else{
          size = toSize();
        }
        vp pData = e_malloc( size );
        memcpy( pData, m_pData, size );
        m_pData = pData;
        m_tFlags->bCloned = 1;
        return true;
      }

    //}:                                          |
    //tick:{                                      |

      void Property::tick(){
        e_guardr( m_tLock );
        m_tFlags->bTicked = 1;
        if( m_onTick ){
          e_sanity_check( m_pOuter, "You cannot get string without an outer reference!" );
          if( m_pOuter && Class::Factory::valid( m_pOuter->UUID )){
            ( const_cast<Object*>( m_pOuter )->*( m_onTick ))( this );
          }
        }
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Object:{                                        |
  //Statics:{                                     |

    u64 Object::gameTurn = 0;

  //}:                                            |
  //Operate:{                                     |

    Object& Object::operator=( const Object& lvalue ){
      m_tStatus = lvalue.m_tStatus;
      return *this;
    }

  //}:                                            |
  //Methods:{                                     |
    //flattenProperties:{                         |

      void Object::flattenProperties( const prop_map& in, prop_map& out ){
        in.foreach(
          [&]( const prop_ptr& pProperty ){
            out.set( pProperty->toName().hash(), pProperty );
            flattenProperties( pProperty->toChildren(), out );
          }
        );
      }

    //}:                                          |
    //toProperty:{                                |

      Object::prop_ptr Object::toProperty( ccp pPropertyName ){
        return ___property( e_hashstr64( pPropertyName ));
      }

    //}:                                          |
    //clone:{                                     |

      Object::handle Object::clone()const{

        //----------------------------------------------------------------------
        // We can't record this object if it's not IO complete; block till then.
        // Otherwise the  recording will fail and we're stuck with junk to send
        // to the GPU, especially in the case of textures and materials.
        //----------------------------------------------------------------------

        blockUntilIOComplete();
        handle hObject = e_newt( m_uOwner );
        hObject
          -> toStatus()
          -> bIOComplete = 0;
        #if !e_compiling( web )
          e_runAsync([ =,this ]()mutable{
        #endif

          //------------------------------------------------------------------
          // Record history from this object...
          //------------------------------------------------------------------

          auto hHistory = e_new<History>( UUID );
          auto& history = hHistory.cast();
          history.record();

          //------------------------------------------------------------------
          // ...into new body.
          //------------------------------------------------------------------

          history.setUUID( hObject );
          history.seek( 0 );
          hObject->onClone(
            *this
          );
          hObject
            -> toStatus()
            -> bIOComplete = 1;
        #if !e_compiling( web )
            }
          );
        #endif
        return hObject;
      }

    //}:                                          |
    //blockUntilIOCompleteShallow:{               |

      void Object::blockUntilIOCompleteShallow()const{
        const double then = e_seconds();
        bool bStop = false;
        while( !bStop ){
          if( m_tStatus->bIOComplete ){
            bStop = true;
          }else if( m_tStatus->bIOError ){
            bStop = true;
          }
          if( !bStop ){
            e_backoff( then );
          }
        }
      }

    //}:                                          |
    //blockUntilIOComplete:{                      |

      void Object::blockUntilIOComplete()const{
        const auto then = e_seconds();
        auto bStop = false;
        while( !bStop ){
          if( m_tStatus->bIOError ){
            bStop = true;
          }else if( isIOComplete() ){
            bStop = true;
          }
          if( !bStop ){
            e_backoff( then );
          }
        }
      }

    //}:                                          |
    //isIOComplete:{                              |

      bool Object::isIOComplete()const{
        if( m_tStatus->bIOError ){
          // If there was an error loading the object then we must assume that
          // the object is I/O complete. In other words no more I/O can be
          // applied to it and we tell the caller that we're done with loading.
          // It is up to that routine to check the bIOError status bit to know
          // whether it is safe to use the handle or not. In actuality the
          // caller must throw the handle away if bIOError is set.
          return true;
        }
        if( !m_tStatus->bIOComplete ){
          // This object is currently loading in a background thread and cannot
          // be relied on to be complete. Only part of it's member variables
          // and their members potentially (if streaming) may be loaded.
          return false;
        }
        // This object is currently IO complete. All members have finished
        // loading via the serializer methods and e_load or stream has set the
        // completion status bit for us.
        return true;
      }

    //}:                                          |
    //serialize:{                                 |

      void Object::preSerialize( Writer& fs )const{
        fs.write( probeid() );
      }

      void Object::serialize( Writer& fs )const{
        fs.version( OBJECT_VERSION );
      }

      void Object::preSerialize( Reader& fs ){
        const auto clsid = fs.read<u64>();
        const auto probe = probeid();
        if( clsid != probe ){
          e_warnsf(
              "Class failure: %llx (read) %llx (probed); \"%s\" instantiated!"
            , clsid
            , probe
            , classof()
          );
        }
      }

      s64 Object::serialize( Reader& fs ){
        fs.version( OBJECT_VERSION );
        return UUID;
      }

    //}:                                          |
    //prettyup:{                                  |

      string Object::prettyup( const string& cPath ){
        string path = cPath;
        path.replace( "eon::gfc::", "" );
        path.replace( "eon::sg::",  "" );
        path.replace( "eon::ri::",  "" );
        path.replace( "eon::ai::",  "" );
        path.replace( "eon::ui::",  "" );
        path.replace( "::", "/" );
        path.replace( "/entity/physics/shape/entity/physics", "/physics/shape" );
        path.replace( "/behavior/schematic",                  "/behavior"      );
        return path;
      }

    //}:                                          |
    //probeid:{                                   |

      u64 Object::probeid()const{
        static const auto clsid = e_classid<Object>();
        return clsid;
      }

    //}:                                          |
    //pathof:{                                    |

      const string& Object::pathof()const{
        return pathid();
      }

    //}:                                          |
    //pathid:{                                    |

      const string& Object::pathid(){
        static string path = "object/";
        return path;
      }

    //}:                                          |
    //isa:{                                       |

      bool Object::isa( const u64 clsid )const{
        return( e_classid<Object>() == clsid );
      }

    //}:                                          |
  //}:                                            |
  //Dtor:{                                        |

    Object::~Object(){
      prop_map out_mProperties;
      getProperties( out_mProperties );
      out_mProperties.foreach( []( prop_ptr& pProperty ){
        pProperty->clear();
      });
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
