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

using namespace EON;
using namespace gfc;

//================================================+=============================
//Factory:{                                       |
  //Classes:{                                     |

    namespace EON{
      namespace gfc{
        Class::Factory::Catalog*                                  Class::Factory::m_pTypeCatalog = nullptr;
        hashmap<Class::Factory::ClassID,Class::Factory::Extends>* Class::Factory::m_pExtensions  = nullptr;
        Class::Factory::Objects                                   Class::Factory::m_oObjects;
      }
    }

  //}:                                            |
  //Private:{                                     |
    //sanityCheck:{                               |

      #if e_compiling( sanity ) && 0
        namespace{
          void sanityCheck(){
            m_oObjects.foreach(
              []( Object* pObject ){
                e_sanity_check( !e_isbad( pObject ),       "Bad object!"      );
                e_sanity_check( !e_isbad( pObject->file ), "Bad object file!" );
                e_assert( pObject->line <= 10000000, "Bad object line!" );
              }
            );
          }
        }
      #else
        #define sanityCheck() ((void)0)
      #endif

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //clearTouched:{                              |

      void Class::Factory::clearTouched(){
        sanityCheck();
        m_oObjects.foreach(
          []( Object* pObject ){
            pObject->toStatus()->bIOTouched = 0;
          }
        );
        sanityCheck();
      }

    //}:                                          |
    //tryQuery:{                                  |

      s32 Class::Factory::tryQuery( const s64 UUID, const std::function<void( const Object* )>& lambda ){
        #if e_compiling( sanity )
          if( UUID > m_oObjects.capacity() ){
            e_unreachablef( "UUID (%lld) is out of bounds!", UUID );
          }
          if( !UUID ){
            e_unreachable( "Null UUID!" );
          }
        #endif
        return const_cast<const Objects&>( m_oObjects ).tryQuery( UUID, lambda );
      }

    //}:                                          |
    //allocate:{                                  |

      s64 Class::Factory::allocate( const u64 clsid ){
        if( !m_pExtensions->find( clsid )){
          e_break( e_xfs( "Couldn't find the given class identifier: %llu!", clsid ));
          return 0;
        }
        return( *m_pExtensions )[ clsid ]( clsid/* needed by Mono layer */);
      }

    //}:                                          |
    //describe:{                                  |

      std::shared_ptr<Class::Factory::CatalogNode> Class::Factory::describe( const ClassID clsid ){
        e_assert( m_pTypeCatalog && m_pTypeCatalog->find( clsid ));
        return (* m_pTypeCatalog )[ clsid ];
      }

    //}:                                          |
    //foreach:{                                   |

      void Class::Factory::foreachsClass( const std::function<bool( const u64 clsid )>& lambda ){
        sanityCheck();
          const double then = e_seconds();
          for(;;){
            const s32 result = m_pTypeCatalog->tryForeachsKV(
              [&]( const u64 clsid, const std::shared_ptr<CatalogNode>& ){
                return lambda( clsid );
              }
            );
            if( 1 != result ){
              e_backoff( then );
              continue;
            }
            break;
          }
        sanityCheck();
      }

      void Class::Factory::foreachClass( const std::function<void( const u64 clsid )>& lambda ){
        sanityCheck();
          const double then = e_seconds();
          for(;;){
            const s32 result = m_pTypeCatalog->tryForeachKV(
              [&]( const u64 clsid, const std::shared_ptr<CatalogNode>& ){
                lambda( clsid );
              }
            );
            if( 1 != result ){
              e_backoff( then );
              continue;
            }
            break;
          }
        sanityCheck();
      }

      void Class::Factory::foreachs( const std::function<bool( const Object* )>& lambda ){
        sanityCheck();
          const_cast<const Objects&>( m_oObjects ).foreachs( lambda );
        sanityCheck();
      }

      void Class::Factory::foreach( const std::function<void( const Object* )>& lambda ){
        sanityCheck();
          const_cast<const Objects&>( m_oObjects ).foreach( lambda );
        sanityCheck();
      }

    //}:                                          |
    //catalog:{                                   |

      u8 Class::Factory::catalog( const u64 clsid, ccp clsof, const u64 sprid, ccp sprof ){

        #if e_compiling( debug ) && 0
          e_logf( "%llu: %s", clsid, clsof );
        #endif

        //----------------------------------------------------------------------
        // Traverse the type catalog building our type relationships as we go.
        //----------------------------------------------------------------------

        if( !clsid || !clsof || !sprid || !sprof ){
          m_pTypeCatalog->foreach(
            [&]( std::shared_ptr<CatalogNode>& pInner ){
              std::shared_ptr<CatalogNode> pOuter;
              if( !pInner->toSuperID() ){
                return;
              }
              if( !m_pTypeCatalog->find( pInner->toSuperID(), pOuter )){
                return;
              }
              pInner->setParent( pOuter );
              pOuter->add( pInner );
            }
          );
          return 1;
        }

        //----------------------------------------------------------------------
        // Populate the type catalog one entry at a time (no super classes).
        //----------------------------------------------------------------------

        // Create new type catalog if one doesn't exist.
        if( !m_pTypeCatalog ){
          m_pTypeCatalog = new Catalog;
          // Now create root object in the catalog or just "Object" by name.
          std::shared_ptr<CatalogNode> pRoot = std::make_shared<CatalogNode>();
          m_pTypeCatalog->set( e_classid<Object>(), pRoot );
          pRoot->setClassID( e_classid<Object>() );
          pRoot->setClassOf( "Object" );
        }

        // Bail out if this class has already been cataloged.
        if( m_pTypeCatalog->find( clsid )){
          return 0;
        }

        // Validate the object inputs.
        e_assert( clsid );
        e_assert( clsof );
        e_assert( sprid );
        e_assert( sprof );

        // Now create a catalog node and fill it with data. All parent/child
        // relationships will be resolved in the engine startup code.
        std::shared_ptr<CatalogNode> pNode = std::make_shared<CatalogNode>();
        m_pTypeCatalog->set( clsid, pNode );
        pNode->setClassID( clsid );
        pNode->setClassOf( clsof );
        pNode->setSuperID( sprid );
        return 1;
      }

    //}:                                          |
    //extend:{                                    |

      u8 Class::Factory::extends( const u64 clsid, const Extends extension ){
        if( !m_pExtensions ){
          m_pExtensions = new hashmap<Class::Factory::ClassID,Class::Factory::Extends>;
        }
        m_pExtensions->set( clsid, extension );
        #if e_compiling( debug )
          e_msgf( "Extending %llx", clsid );
        #endif
        return 1;
      }

    //}:                                          |
    //valid:{                                     |

      bool Class::Factory::valid( const s64 UUID ){
        if( !m_oObjects.find( UUID )){
          return false;
        }
        #if e_compiling( debug )
          if( UUID == 0xddddddddddddddddUL ){
            e_warnsf( "Object deleted!" );
            return false;
          }
          if( UUID > s64( m_oObjects.capacity() )){
            e_warnsf( "Not an UUID! Not within bitbucket range." );
            return false;
          }
          if( UUID < 0 ){
            e_warnsf( "Negative UUID!" );
            return false;
          }
        #endif
        return true;
      }

    //}:                                          |
    //erase:{                                     |

      void Class::Factory::erase( const s64 UUID ){
        #if e_compiling( debug )
          if( UUID > s64( m_oObjects.capacity() )){
            e_unreachablef( "UUID (%lld) is out of bounds!", UUID );
          }
          if( !UUID ){
            e_unreachable( "Null UUID!" );
          }
        #endif
        sanityCheck();
        s32 result=-1;
        for(;;){
          Object* out_pObject = nullptr;
          result = m_oObjects.tryErase( UUID,
            [&]( Object*& pObject ){
              // Immediate object must be I/O complete but dangling handle don't.
              // The background streaming system will automatically delete them.
              #if e_compiling( debug )
                if( !pObject->isIOCompleteShallow() ){
                  e_unreachablef( "Internal error object %lld was not IO complete.", UUID );
                }
              #endif
              out_pObject = pObject;
              pObject = nullptr;
            }
          );
          if( -1 == result ){
            continue;
          }
          if( 1 == result ){
            // We can't delete the object inside the m_oObjects lock or we'll
            // deadlock against threads trying to release and acquire in the
            // destructor.
            if( out_pObject ){
              out_pObject->~Object();
              e_free( out_pObject );
              out_pObject = nullptr;
            }
            // Finally sanity check the system and return.
            sanityCheck();
            return;
          }
          // The UUID has already been erased.
          return;
        }
      }

    //}:                                          |
    //clear:{                                     |

      void Class::Factory::clear(){
        sanityCheck();
          m_oObjects.clear();
        sanityCheck();
      }

    //}:                                          |
    //set:{                                       |

      s64 Class::Factory::getUUID( Object* pObject ){
        const s64 UUID = m_oObjects.set( pObject );
        pObject->setOwner( pObject->probeid() );
        pObject->toStatus()->bIOComplete = 1;
        pObject->setWhen( e_when() );
        pObject->UUID = UUID;
        return UUID;
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================+=============================
