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
  *     \addtogroup reflection
  *     @{
  */

//================================================+=============================
//Macros:{                                        |
  //e_reflect_no_properties:{                     |

    /** \brief Property-less reflection macro.
      *
      * This function macro code generates the engine reflection code without
      * any property code. Use this for classes that have no e_property*
      * statements in it.
      */

    #define e_reflect_no_properties(T,S)                                        \
      private:                                                                  \
        static u8 mojo;                                                         \
        static u8 jojo;                                                         \
      public:                                                                   \
        using weak_handle = WeakRef<T>;                                         \
        using handle      = AutoRef<T>;                                         \
        using super       = S;                                                  \
        using self        = T;                                                  \
        using cptrc       = const self* const;                                  \
        using cptr        = const self*;                                        \
        using ptrc        = self* const;                                        \
        using ptr         = self*;                                              \
        constexpr static ccp classname(){                                       \
          return #T;                                                            \
        }                                                                       \
        virtual bool isa( const u64 clsid )const override{                      \
          if( e_classid<self>() != clsid ){                                     \
            return super::isa( clsid );                                         \
          }                                                                     \
          return true;                                                          \
        }                                                                       \
        /* Not used: exists for utility and completeness */                     \
        virtual u64 hashof()const override{                                     \
          return e_hashstr64_const( #T );                                       \
        }                                                                       \
        virtual ccp classof()const override{                                    \
          return #T;                                                            \
        }                                                                       \
        virtual u64 probeid()const override{                                    \
          static const u64 clsid = e_classid<T>();                              \
          return clsid;                                                         \
        }                                                                       \
        virtual const::EON::gfc::string& pathof()const override{                \
          return pathid();                                                      \
        }                                                                       \
        virtual u32 asSizeof()const override{                                   \
          return sizeof( *this );                                               \
        }                                                                       \
        static const::EON::gfc::string& pathid(){                               \
          static::EON::gfc::string path = prettyup(( super::pathid()            \
              + boost::typeindex::type_id<T>().pretty_name().c_str()            \
              + "/" ).tolower() );                                              \
          return path;                                                          \
        }                                                                       \
        static::EON::s64 reflect( const u64 ){                                  \
          return/* s64 */::EON::gfc::Class::alloc<T>();                         \
        }                                                                       \

  //}:                                            |
  //e_reflect:{                                   |

    /** \brief The reflection statement.
      *
      * This function macro declares all the glue required by the engine to get
      * proper RTTI and object reflection working. Every class that is derived
      * from Object (every engine class) needs to add this line. It will define
      * a number of very useful constructs for you:
      *
      * \param T The type of the class whose inheritance chain we're trying to
      * define. This argument will be used to figure out what the classof
      * method should return (#T) among other things.
      *
      * \param S The immediate base class of T. Will be used to typedef a super
      * type name in T.
      */

    #define e_reflect( T, S )                                                   \
      protected:                                                                \
        e_noinline_or_debug virtual prop_ptr ___property(                       \
              const u64 h )const override{                                      \
          prop_map out_mProperties;                                             \
          flattenProperties(___root_property->toChildren(), out_mProperties );  \
          prop_ptr pProperty;                                                   \
          if( !out_mProperties.find( h, pProperty )){                           \
            pProperty = super::___property( h );                                \
          }                                                                     \
          return pProperty;                                                     \
        }                                                                       \
      private:                                                                  \
        prop_ptr ___root_property=std::make_shared<::EON::gfc::Property>( #T ); \
        prop_ptr ___group_property;                                             \
        prop_ptr ___head_property;                                              \
      public:                                                                   \
        e_noinline virtual void getProperties( prop_map& out_mProperties )      \
              const override{                                                   \
          super::getProperties( out_mProperties );                              \
          ___root_property->toChildren().foreach(                               \
            [&out_mProperties]( const prop_ptr& pProperty ){                    \
              if( pProperty ){                                                  \
                out_mProperties.set( pProperty->toName().hash(), pProperty )    \
            ; }                                                                 \
            }                                                                   \
          );                                                                    \
        }                                                                       \
        e_reflect_no_properties( T, S )                                         \

    /** \brief Extend reflection macro.
      *
      * This macro extends the class factory by implementing the virtual
      * methods exposed by the e_reflect macro and setting up special hooks to
      * the class factory so that we can construct a default constructed object
      * with just a 64-bit class identifier.
      *
      * \param T The name of the class we're adding to the reflection system.
      */

    #define e_specialized_extends(T)                                            \
      template<> u8 T::mojo=Class::Factory::extends(                            \
          e_classid<T>(),                                                       \
          T::reflect );                                                         \
      template<> u8 T::jojo=Class::Factory::catalog(                            \
          e_classid<T>(),                                                       \
          T::classname(),                                                       \
          e_classid<T::super>(),                                                \
          T::super::classname() )                                               \

    /** \brief Extend reflection macro.
      *
      * This macro extends the class factory by implementing the virtual
      * methods exposed by the e_reflect macro and setting up special hooks to
      * the class factory so that we can construct a default constructed object
      * with just a 64-bit class identifier.
      *
      * \param T The name of the class we're adding to the reflection system.
      */

    #define e_extends(T)                                                        \
      u8 T::mojo=Class::Factory::extends(                                       \
          e_classid<T>(),                                                       \
          T::reflect );                                                         \
      u8 T::jojo=Class::Factory::catalog(                                       \
          e_classid<T>(),                                                       \
          T::classname(),                                                       \
          e_classid<T::super>(),                                                \
          T::super::classname() )                                               \

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Class:{                                         |

  namespace EON{

    namespace gfc{

      /** \brief Class template.
        *
        * This template class is the basis of the engine reflection system. In
        * the EON Engine there are classes and there are objects. Objects are
        * always the base class of everything guaranteed. Making new objects is
        * as easy as saying
        \code
          struct myObject:Object{
            e_reflect( myObject, Object );
          };
        \endcode
        * Once an object has been defined you can take advantage of the
        * engine's fast and elegant RTTI system.
        \code
          void main(){
            e_assert( e_classid<myObject>()==myObject::classid() );
          }
        \endcode
        * You create new objects by using the e_new<> APU. (The e means engine
        * in EON parlance).  The e_reflect() macro used above adds a lot of
        * nice types to your object, once of them is called "handle".
        * Everything in the system is essentially handle based. Such are
        * automatic reference counting values (64-bit in size) that uniquely
        * describe a class instance: it's UUID.
        \code
          void main(){
            //automatically destroyed when it falls out of scope.
            auto hObject = e_new<myObject>();
            auto UUID = hObject->UUID;
            auto UUI2 = hObject;
            //both will match.
          }
        \endcode
        */

      struct E_PUBLISH Class final{

        //----------------------------------------+-----------------------------
        //Structs:{                               |

          /** \brief A class to generate new AutoRef classes.
            *
            * This class factory produces new AutoRef classes via the e_new<>()
            * APU. It is used predominantly by the gfc::Class.
            */

          struct E_PUBLISH Factory final{

            //------------------------------------+-----------------------------
            //Structs:{                           |

              struct E_PUBLISH CatalogNode final{

                using ptr = std::weak_ptr<CatalogNode>;
                using vec = vector<ptr>;

                /** \brief Add child node.
                  *
                  * This routine will add a child node.
                  */

                e_forceinline void add( const ptr& pNode ){
                  m_vChildren.push( pNode );
                }

                /** \brief Determine if node is empty.
                  *
                  * This routine will return true if there are no child nodes and
                  * false otherwise.
                  */

                e_forceinline bool empty()const{
                  return m_vChildren.empty();
                }

                /** \brief Return children as a vector.
                  *
                  * This routine will return the children as a vector by value.
                  */

                e_forceinline const vec& asVector()const{
                  return m_vChildren;
                }

                /** \brief Get the number of children.
                  *
                  * This routine returns the current child count.
                  */

                e_forceinline u64 size()const{
                  return m_vChildren.size();
                }

              private:

                e_var(     ccp, p, ClassOf  ) = nullptr;
                e_var(     u64, u, ClassID  ) = 0;
                e_var(     u64, u, SuperID  ) = 0;
                e_var(     ptr, p, Parent   );
                e_var_vector( ptr, Children );
              };

              using Catalog = hashmap<u64,std::shared_ptr<CatalogNode>>;
              using Objects = pool<Object*,1048576>;
              using Extends = s64(*)( const u64 );
              using ClassID = u64;

            //}:                                  |
            //Methods:{                           |

              /** \brief Get catalog node for class identifier.
                *
                * This routine will return a catalog node for the given class
                * id.
                *
                * \return Returns a shared pointer to the node.
                */

              static std::shared_ptr<CatalogNode> describe( const ClassID clsid );

              /** \brief Add a new entry to the type catalog.
                *
                * This routine will expand the type catalog by adding a new
                * type to that tree. From the type catalog, given a class
                * identifier, you can query all the types that derive from said
                * id. This is critical for the property system which uses the
                * type catalog with vectors of handles thereby providing a nice
                * "add" button to create components on an entity of all
                * possible types. Much better than having a massive list of if
                * statements that do an isa() on the handle.
                *
                * If all parameters are nullptr and zero, as called from the
                * engine startup code prior to IEngine::main(), then the type
                * catalog will be traversed and the tree constructed from
                * there.
                *
                * \param classid The class identifier of the object to be added
                * to the class factory's type catalog.
                *
                * \param classof The class name of the type being added.
                *
                * \param superid The class identifier of the immediate super
                * class of the type to be added to the class factory's type
                * catalog.
                *
                * \param superof The class name of the super class.
                *
                * \return Returns 1 on success and 0 otherwise.
                */

              static u8 catalog(
                  const ClassID classid,
                  ccp           classof,
                  const ClassID superid,
                  ccp           superof );

              /** \brief Add a new RTTI extension.
                *
                * This routine will add a new ClassID to Object* aka extension
                * to the class factory. This must be called for every
                * e_reflect's corresponding e_extends globally before main.
                *
                * \param clsid The class identifier to associate the callback
                * extension function with.
                *
                * \param extension A function pointer that gets called when we
                * want to produce a new Object* from the class identifier. Must
                * call e_new<>.  If you use the e_extends() macro all this is
                * handled for you properly.
                *
                * \return Returns 1 if the extension was added and there was
                * room to receive it, or 0 to indicate an error. Used so the
                * e_extends() macro can hook into the program's C++ global
                * construction.
                */

              static u8 extends( const ClassID clsid, const Extends extension );

              /** \brief Try to query the class factory.
                *
                * This routine will perform a query on the associated object
                * and run the lambda provided if there weren't any errors.
                *
                * \param UUID The universally unique identifier of an object.
                *
                * \param lambda The callback routine to call if the query goes
                * through.
                *
                * \return Returns -1 if the class factory's hash map was busy
                * or in other words write locked by another thread, 1 if the
                * query was successful (the read lock was acquired and the
                * lambda called) and zero if the UUID didn't exist in the map.
                * If the latter is true then you may still be ok, another
                * thread's e_new<> may still be in the object's constructor.
                * Wait a little bit of time (1s should be good) and if it's
                * still zero then your program is probably dead.
                */

              static s32 tryQuery( const s64 UUID, const std::function<void( const Object* )>& lambda );

              /** \brief Get factory objects.
                *
                * This routine will return the factory objects map. It's really
                * important to use this method for iterating over the object
                * map because gfc::map does not acquire a spinlock like a
                * gfc::vector does.  Class::Factory::foreach will wrap the
                * object map foreach call with lock and unlock for maximum
                * safety. If it didn't you could be iterating over the object
                * map and have it reallocated and changed by another thread.
                */

              static void foreachsClass( const std::function<bool( const u64 clsid )>& lambda );

              /** \brief Get factory objects.
                *
                * This routine will return the factory objects map. It's really
                * important to use this method for iterating over the object
                * map because gfc::map does not acquire a spinlock like a
                * gfc::vector does.  Class::Factory::foreach will wrap the
                * object map foreach call with lock and unlock for maximum
                * safety. If it didn't you could be iterating over the object
                * map and have it reallocated and changed by another thread.
                */

              static void foreachClass( const std::function<void( const u64 clsid )>& lambda );

              /** \brief Get factory objects.
                *
                * This routine will return the factory objects map. It's really
                * important to use this method for iterating over the object
                * map because gfc::map does not acquire a spinlock like a
                * gfc::vector does.  Class::Factory::foreach will wrap the
                * object map foreach call with lock and unlock for maximum
                * safety. If it didn't you could be iterating over the object
                * map and have it reallocated and changed by another thread.
                */

              static void foreachs( const std::function<bool( const Object* )>& lambda );

              /** \brief Get factory objects.
                *
                * This routine will return the factory objects map. It's really
                * important to use this method for iterating over the object
                * map because gfc::map does not acquire a spinlock like a
                * gfc::vector does.  Class::Factory::foreach will wrap the
                * object map foreach call with lock and unlock for maximum
                * safety. If it didn't you could be iterating over the object
                * map and have it reallocated and changed by another thread.
                */

              static void foreach( const std::function<void( const Object* )>& lambda );

              /** \brief Allocate an object using classid.
                *
                * This routine will allocate an object via the reflection table
                * of the class factory. This will ensure the fully derived
                * class is created with all virtual functions intact.
                *
                * \param clsid The 64-bit class identifier which is a hash of
                * the type name.
                *
                * \return Returns a handle to the new type. Calling base
                * virtual methods on the object are guaranteed to call in the
                * fully derived class T. The type T is unknown to you and must
                * be gathered by other means outside the scope of this
                * function.
                */

              static s64 allocate( const ClassID clsid );

              /** \brief Return an object for the given object id.
                *
                * This routine converts the object-id provided into an Object
                * object.  This is a great database mechanism the game can use
                * to lookup objects per class. For example, if a shader wanted
                * to get all instances of its class one would simply write:
                *
                * \param UUID The identifier for the object in question.
                *
                * \return Returns a reference to an Object pointer.
                */

              e_noinline_or_debug static Object* pcast( const s64 UUID ){
                #if !e_compiling( master )
                  #if e_compiling( sanity )
                    if( UUID > m_oObjects.capacity() ){
                      e_unreachablef( "UUID (%lld) is out of bounds!", UUID );
                    }
                  #elif e_compiling( debug )
                    if( !UUID ){
                      e_unreachable( "Null UUID!" );
                    }
                  #endif
                #endif
                Object* pObject = nullptr;
                if( !m_oObjects.query( UUID, [&]( Object* pQuery ){ pObject = pQuery; })){
                  #if !e_compiling( master )
                    e_break( e_xfs( "UUID (%lld) not found in object database!", UUID ));
                  #endif
                }
                return pObject;
              }

              /** \brief Set an object for an UUID.
                *
                * This routine will set the object for the given slot.
                *
                * \param pObject The object to get the UUID from. Safe than
                * just pObject->UUID because it checks the object database.
                *
                * \return Returns the UUID of the passed in object. Slower than
                * using the UUID directly.
                */

              static s64 getUUID( Object* pObject );

              /** \brief Erase class from the factory.
                *
                * This routine will nuke an AutoRef pointer from orbit. It does
                * it in a thread safe manner.
                *
                * \param UUID The universally unique identifier. By universally
                * unique I actually mean "locally unique" because the value is
                * only unique within the current process NOT across the
                * network. Those values are called GUID or globally/network
                * unique identifier.  It's a small disclaimer.
                */

              static void erase( const s64 UUID );

              /** \brief Test if UUID is valid.
                *
                * This routine will test whether the given UUID is valid or
                * not.
                */

              static bool valid( const s64 UUID );

              /** \brief Clear touch flag.
                *
                * This routine will clear the touched status bit on all
                * reflected objects available to the class factory.
                */

              static void clearTouched();

              /** \brief Clear out the class factory.
                *
                * This routine will clear out the class factory without
                * deleting any objects. The only routine that has the authority
                * to calls this is the IEngine::exit function.
                */

              static void clear();

            //}:                                  |
            //------------------------------------+-----------------------------

          private:

            static Factory::Catalog*                           m_pTypeCatalog;
            static hashmap<Factory::ClassID,Factory::Extends>* m_pExtensions;
            static Factory::Objects                            m_oObjects;
          };

        //}:                                      |
        //Aliases:{                               |

          using ClassID = typename Factory::ClassID;

        //}:                                      |
        //Grouped:{                               |
          //Reference counting:{                  |

            /** \brief Decrement reference counter.
              *
              * This routine will atomically decrement the reference count of
              * an AutoRef pointer.
              *
              * \param UUID The 64-bit integer handle to an AutoRef object. The
              * value is used to index into the global class factory, the basis
              * of any reflection in the engine.
              */

            e_noinline static bool subweak( const s64 UUID ){
              if( Factory::valid( UUID )){
                Object* pObject = Factory::pcast( UUID );
                if( pObject ){
                  double start = e_seconds();
                  while( pObject->toStatus()->bStallDelete ){
                    e_backoff( start );
                  }
                  const auto ref = pObject->subweak();
                  if( !ref && !pObject->refs() ){
                    erase( UUID );
                  }
                  return true;
                }
              }
              return false;
            }

            /** \brief Increment reference counter.
              *
              * This routine will atomically increment the reference count of
              * an AutoRef pointer.
              */

            e_noinline static bool addweak( const s64 UUID ){
              if( Factory::valid( UUID )){
                Object* pObject = Factory::pcast( UUID );
                if( pObject ){
                  pObject->addweak();
                  return true;
                }
              }
              return false;
            }

            /** \brief Decrement reference counter.
              *
              * This routine will atomically decrement the reference count of
              * an AutoRef pointer.
              *
              * \param UUID The 64-bit integer handle to an AutoRef object. The
              * value is used to index into the global class factory, the basis
              * of any reflection in the engine.
              */

            e_noinline static bool subref( const s64 UUID ){
              if( Factory::valid( UUID )){
                Object* pObject = Factory::pcast( UUID );
                if( pObject ){
                  double start = e_seconds();
                  while( pObject->toStatus()->bStallDelete ){
                    e_backoff( start );
                  }
                  const auto ref = pObject->subref();
                  if( !ref && !pObject->weak() ){
                    erase( UUID );
                  }
                  return true;
                }
              }
              return false;
            }

            /** \brief Increment reference counter.
              *
              * This routine will atomically increment the reference count of
              * an AutoRef pointer.
              */

            e_noinline static bool addref( const s64 UUID ){
              if( Factory::valid( UUID )){
                Object* pObject = Factory::pcast( UUID );
                if( pObject ){
                  pObject->addref();
                  return true;
                }
              }
              return false;
            }

          //}:                                    |
          //Class identifier:{                    |

            /** \brief Generate new classid for the type.
              *
              * This routine produces a brand new classid for this new class
              * object.  It is guaranteed to be unique for each class and will
              * not repeat or be reused. This gives us a maximum number of
              * classes at around four billion. Do not expect the classid() to
              * be the same between runs however. Class identifiers are handed
              * out on a first-come-first-serve basis so it's highly likely
              * your myObject could have a CLSID of 1 the first time through
              * and 7 the second.
              *
              * \return Returns a new class identifier (CLSID). The CLSID is
              * actually one based because zero means not-an-object or error.
              */

            template<typename T> e_forceinline static ClassID classid(){
              return e_classid<T>();
            }

          //}:                                    |
          //Identification:{                      |

            /** \brief Check whether integer handle is a member of the class.
              *
              * This routine uses engine reflection to check whether the given
              * id is of the same class as given by the template argument.
              */

            template<typename T> e_forceinline static bool owns( const s64 UUID ){
              Object* pObject = Factory::pcast( UUID );
              if( !pObject ){
                return false;
              }
              return( pObject->toOwner() == e_classid<T>() );
            }

            /** \brief Same as owns but for classes.
              *
              * Compares the two classids and returns true if they're equal or
              * false.
              *
              * \param clsid The 64-bit class identifier of the type you want to
              * check if it's of this class.
              */

            template<typename T> e_forceinline static bool is( const ClassID clsid ){
              return e_classid<T>()==clsid;
            }

          //}:                                    |
          //Allocation:{                          |

            /** \brief Reflection mechanism.
              *
              * This variadic template will allocate a new Object class with
              * args to it's constructor.
              */

            template<typename T, typename...A> e_noinline static s64 alloc( A...args ){
              void* m = e_malloc( sizeof( T ));
              T*    p = new( m )T( std::forward<A>( args )... );
              return Class::Factory::getUUID( p );
            }

            /** \brief remove object mechanism.
              *
              * This routine will delete an object from the class.
              *
              * \param UUID The UUID of the AutoRef object.
              */

            e_noinline static void erase( const s64 UUID ){
              if( Factory::valid( UUID )){
                  Factory::erase( UUID );
              }
            }

          //}:                                    |
          //Iteration:{                           |
            //foreach:{                           |

              /** \brief Iterate over class instances.
                *
                * This routine will iteratre over all objects of this class
                * that have been created with e_new<>.
                *
                * \param lambda A callback for all objects that match the
                * classid.
                */

              template<typename T> e_noinline static void foreach( const std::function<void( const T& )>& lambda ){
                Factory::foreach(
                  [&]( const Object* pObject ){
                    if( e_isa<T>( pObject )){
                      lambda( *(const T*)pObject );
                    }
                  }
                );
              }

              /** \brief Iterate over class instances.
                *
                * This routine will iterate over all objects of this class
                * that have been created with e_new<>.
                *
                * \param lambda A callback for all objects that match the
                * classid.
                */

              template<typename T> e_noinline static void foreachT( const std::function<void( const T& )>& lambda ){
                Factory::foreach(
                  [&]( const Object* pObject ){
                    if( e_classid<T>() == pObject->probeid() ){
                      lambda( *(const T*)pObject );
                    }
                  }
                );
              }

              /** \brief Iterate over class instances.
                *
                * This routine will iterate over all objects of this class
                * that have been created with e_new<>.
                *
                * \param lambda A callback for all objects that match the
                * classid.
                */

              template<typename T> e_noinline static void foreachs( const std::function<bool( const T& )>& lambda ){
                Factory::foreachs(
                  [&]( const Object* pObject ){
                    if( e_isa<T>( pObject )){
                      return lambda( static_cast<const T&>( *pObject ));
                    }
                    return true;
                  }
                );
              }

            //}:                                  |
          //}:                                    |
          //Casting:{                             |

            /** \brief Casting mechanism
              *
              * This routine will convert an integer handle to a stream type
              * if it exists. A handle value of zero means no stream and an
              * error message is thrown back in the caller's face.
              *
              * \param UUID A value returned by IEngine::insert.
              *
              * \return Returns a valid object or throws an error message of
              * type ccp so you have to surround Casts with a try/catch block.
              */

            template<typename T> e_forceinline_always static T& cast( const s64 UUID ){
              return *pcast<T>( UUID );
            }

            /** \brief Casting mechanism: to pointer.
              *
              * This routine will convert an integer handle (UUID) to a pointer
              * one can use to manipulate the referenced object.
              *
              * \param UUID The UUID of the object handle/AutoRef we want to
              * cast.
              */

            template<typename T> e_forceinline_always static T* pcast( const s64 UUID ){
              auto* pObject = Factory::pcast( UUID );
              #if e_compiling( sanity )
                if( !e_isa<T>( pObject )){
                  e_unreachablef( "Not a %s", e_classof<T>().c_str() );
                }
              #endif
              return(T*)pObject;
            }

          //}:                                    |
        //}:                                      |
        //----------------------------------------+-----------------------------
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//API:{                                           |
  //e_new[t]:{                                    |

    template<typename T> e_forceinline::EON::gfc::AutoRef<T> e_newt( const::EON::s64 clsid ){
      return::EON::gfc::Class::Factory::allocate( clsid );
    }

    template<typename T,typename...A> e_forceinline::EON::gfc::AutoRef<T> e_new( A...args ){
      return::EON::gfc::Class::alloc<T>( std::forward<A>( args )... );
    }

    template<typename T> e_forceinline::EON::gfc::AutoRef<T> e_newt(){
      return::EON::gfc::Class::Factory::allocate( e_classid<T>() );
    }

    e_forceinline::EON::s64 e_newt( const EON::s64 clsid ){
      return::EON::gfc::Class::Factory::allocate( clsid );
    }

  //}:                                            |
//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
