//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
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
//Forward:{                                       |

  namespace EON{
    namespace gfc{
      template<typename T> struct AutoRef;
      namespace fs{
        struct Writer;
        struct Reader;
      }
      struct Property;
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Hashes:{                                        |

  namespace EON{

    namespace gfc{

      // https://github.com/LordJZ/consthash/tree/master/include/consthash

      static u64 constexpr kCRC64[] = {
        0x0000000000000000ULL, 0x24854997ba2f81e7ULL, 0x490a932f745f03ceULL, 0x6d8fdab8ce708229ULL,
        0x9215265ee8be079cULL, 0xb6906fc95291867bULL, 0xdb1fb5719ce10452ULL, 0xff9afce626ce85b5ULL,
        0x66daad56789639abULL, 0x425fe4c1c2b9b84cULL, 0x2fd03e790cc93a65ULL, 0x0b5577eeb6e6bb82ULL,
        0xf4cf8b0890283e37ULL, 0xd04ac29f2a07bfd0ULL, 0xbdc51827e4773df9ULL, 0x994051b05e58bc1eULL,
        0xcdb55aacf12c7356ULL, 0xe930133b4b03f2b1ULL, 0x84bfc98385737098ULL, 0xa03a80143f5cf17fULL,
        0x5fa07cf2199274caULL, 0x7b253565a3bdf52dULL, 0x16aaefdd6dcd7704ULL, 0x322fa64ad7e2f6e3ULL,
        0xab6ff7fa89ba4afdULL, 0x8feabe6d3395cb1aULL, 0xe26564d5fde54933ULL, 0xc6e02d4247cac8d4ULL,
        0x397ad1a461044d61ULL, 0x1dff9833db2bcc86ULL, 0x7070428b155b4eafULL, 0x54f50b1caf74cf48ULL,
        0xd99a54b24bb2d03fULL, 0xfd1f1d25f19d51d8ULL, 0x9090c79d3fedd3f1ULL, 0xb4158e0a85c25216ULL,
        0x4b8f72eca30cd7a3ULL, 0x6f0a3b7b19235644ULL, 0x0285e1c3d753d46dULL, 0x2600a8546d7c558aULL,
        0xbf40f9e43324e994ULL, 0x9bc5b073890b6873ULL, 0xf64a6acb477bea5aULL, 0xd2cf235cfd546bbdULL,
        0x2d55dfbadb9aee08ULL, 0x09d0962d61b56fefULL, 0x645f4c95afc5edc6ULL, 0x40da050215ea6c21ULL,
        0x142f0e1eba9ea369ULL, 0x30aa478900b1228eULL, 0x5d259d31cec1a0a7ULL, 0x79a0d4a674ee2140ULL,
        0x863a28405220a4f5ULL, 0xa2bf61d7e80f2512ULL, 0xcf30bb6f267fa73bULL, 0xebb5f2f89c5026dcULL,
        0x72f5a348c2089ac2ULL, 0x5670eadf78271b25ULL, 0x3bff3067b657990cULL, 0x1f7a79f00c7818ebULL,
        0xe0e085162ab69d5eULL, 0xc465cc8190991cb9ULL, 0xa9ea16395ee99e90ULL, 0x8d6f5faee4c61f77ULL,
        0xf1c4488f3e8f96edULL, 0xd541011884a0170aULL, 0xb8cedba04ad09523ULL, 0x9c4b9237f0ff14c4ULL,
        0x63d16ed1d6319171ULL, 0x475427466c1e1096ULL, 0x2adbfdfea26e92bfULL, 0x0e5eb46918411358ULL,
        0x971ee5d94619af46ULL, 0xb39bac4efc362ea1ULL, 0xde1476f63246ac88ULL, 0xfa913f6188692d6fULL,
        0x050bc387aea7a8daULL, 0x218e8a101488293dULL, 0x4c0150a8daf8ab14ULL, 0x6884193f60d72af3ULL,
        0x3c711223cfa3e5bbULL, 0x18f45bb4758c645cULL, 0x757b810cbbfce675ULL, 0x51fec89b01d36792ULL,
        0xae64347d271de227ULL, 0x8ae17dea9d3263c0ULL, 0xe76ea7525342e1e9ULL, 0xc3ebeec5e96d600eULL,
        0x5aabbf75b735dc10ULL, 0x7e2ef6e20d1a5df7ULL, 0x13a12c5ac36adfdeULL, 0x372465cd79455e39ULL,
        0xc8be992b5f8bdb8cULL, 0xec3bd0bce5a45a6bULL, 0x81b40a042bd4d842ULL, 0xa531439391fb59a5ULL,
        0x285e1c3d753d46d2ULL, 0x0cdb55aacf12c735ULL, 0x61548f120162451cULL, 0x45d1c685bb4dc4fbULL,
        0xba4b3a639d83414eULL, 0x9ece73f427acc0a9ULL, 0xf341a94ce9dc4280ULL, 0xd7c4e0db53f3c367ULL,
        0x4e84b16b0dab7f79ULL, 0x6a01f8fcb784fe9eULL, 0x078e224479f47cb7ULL, 0x230b6bd3c3dbfd50ULL,
        0xdc919735e51578e5ULL, 0xf814dea25f3af902ULL, 0x959b041a914a7b2bULL, 0xb11e4d8d2b65faccULL,
        0xe5eb469184113584ULL, 0xc16e0f063e3eb463ULL, 0xace1d5bef04e364aULL, 0x88649c294a61b7adULL,
        0x77fe60cf6caf3218ULL, 0x537b2958d680b3ffULL, 0x3ef4f3e018f031d6ULL, 0x1a71ba77a2dfb031ULL,
        0x8331ebc7fc870c2fULL, 0xa7b4a25046a88dc8ULL, 0xca3b78e888d80fe1ULL, 0xeebe317f32f78e06ULL,
        0x1124cd9914390bb3ULL, 0x35a1840eae168a54ULL, 0x582e5eb66066087dULL, 0x7cab1721da49899aULL,
        0xa17870f5d4f51b49ULL, 0x85fd39626eda9aaeULL, 0xe872e3daa0aa1887ULL, 0xccf7aa4d1a859960ULL,
        0x336d56ab3c4b1cd5ULL, 0x17e81f3c86649d32ULL, 0x7a67c58448141f1bULL, 0x5ee28c13f23b9efcULL,
        0xc7a2dda3ac6322e2ULL, 0xe3279434164ca305ULL, 0x8ea84e8cd83c212cULL, 0xaa2d071b6213a0cbULL,
        0x55b7fbfd44dd257eULL, 0x7132b26afef2a499ULL, 0x1cbd68d2308226b0ULL, 0x383821458aada757ULL,
        0x6ccd2a5925d9681fULL, 0x484863ce9ff6e9f8ULL, 0x25c7b97651866bd1ULL, 0x0142f0e1eba9ea36ULL,
        0xfed80c07cd676f83ULL, 0xda5d45907748ee64ULL, 0xb7d29f28b9386c4dULL, 0x9357d6bf0317edaaULL,
        0x0a17870f5d4f51b4ULL, 0x2e92ce98e760d053ULL, 0x431d14202910527aULL, 0x67985db7933fd39dULL,
        0x9802a151b5f15628ULL, 0xbc87e8c60fded7cfULL, 0xd108327ec1ae55e6ULL, 0xf58d7be97b81d401ULL,
        0x78e224479f47cb76ULL, 0x5c676dd025684a91ULL, 0x31e8b768eb18c8b8ULL, 0x156dfeff5137495fULL,
        0xeaf7021977f9cceaULL, 0xce724b8ecdd64d0dULL, 0xa3fd913603a6cf24ULL, 0x8778d8a1b9894ec3ULL,
        0x1e388911e7d1f2ddULL, 0x3abdc0865dfe733aULL, 0x57321a3e938ef113ULL, 0x73b753a929a170f4ULL,
        0x8c2daf4f0f6ff541ULL, 0xa8a8e6d8b54074a6ULL, 0xc5273c607b30f68fULL, 0xe1a275f7c11f7768ULL,
        0xb5577eeb6e6bb820ULL, 0x91d2377cd44439c7ULL, 0xfc5dedc41a34bbeeULL, 0xd8d8a453a01b3a09ULL,
        0x274258b586d5bfbcULL, 0x03c711223cfa3e5bULL, 0x6e48cb9af28abc72ULL, 0x4acd820d48a53d95ULL,
        0xd38dd3bd16fd818bULL, 0xf7089a2aacd2006cULL, 0x9a87409262a28245ULL, 0xbe020905d88d03a2ULL,
        0x4198f5e3fe438617ULL, 0x651dbc74446c07f0ULL, 0x089266cc8a1c85d9ULL, 0x2c172f5b3033043eULL,
        0x50bc387aea7a8da4ULL, 0x743971ed50550c43ULL, 0x19b6ab559e258e6aULL, 0x3d33e2c2240a0f8dULL,
        0xc2a91e2402c48a38ULL, 0xe62c57b3b8eb0bdfULL, 0x8ba38d0b769b89f6ULL, 0xaf26c49cccb40811ULL,
        0x3666952c92ecb40fULL, 0x12e3dcbb28c335e8ULL, 0x7f6c0603e6b3b7c1ULL, 0x5be94f945c9c3626ULL,
        0xa473b3727a52b393ULL, 0x80f6fae5c07d3274ULL, 0xed79205d0e0db05dULL, 0xc9fc69cab42231baULL,
        0x9d0962d61b56fef2ULL, 0xb98c2b41a1797f15ULL, 0xd403f1f96f09fd3cULL, 0xf086b86ed5267cdbULL,
        0x0f1c4488f3e8f96eULL, 0x2b990d1f49c77889ULL, 0x4616d7a787b7faa0ULL, 0x62939e303d987b47ULL,
        0xfbd3cf8063c0c759ULL, 0xdf568617d9ef46beULL, 0xb2d95caf179fc497ULL, 0x965c1538adb04570ULL,
        0x69c6e9de8b7ec0c5ULL, 0x4d43a04931514122ULL, 0x20cc7af1ff21c30bULL, 0x04493366450e42ecULL,
        0x89266cc8a1c85d9bULL, 0xada3255f1be7dc7cULL, 0xc02cffe7d5975e55ULL, 0xe4a9b6706fb8dfb2ULL,
        0x1b334a9649765a07ULL, 0x3fb60301f359dbe0ULL, 0x5239d9b93d2959c9ULL, 0x76bc902e8706d82eULL,
        0xeffcc19ed95e6430ULL, 0xcb7988096371e5d7ULL, 0xa6f652b1ad0167feULL, 0x82731b26172ee619ULL,
        0x7de9e7c031e063acULL, 0x596cae578bcfe24bULL, 0x34e374ef45bf6062ULL, 0x10663d78ff90e185ULL,
        0x4493366450e42ecdULL, 0x60167ff3eacbaf2aULL, 0x0d99a54b24bb2d03ULL, 0x291cecdc9e94ace4ULL,
        0xd686103ab85a2951ULL, 0xf20359ad0275a8b6ULL, 0x9f8c8315cc052a9fULL, 0xbb09ca82762aab78ULL,
        0x22499b3228721766ULL, 0x06ccd2a5925d9681ULL, 0x6b43081d5c2d14a8ULL, 0x4fc6418ae602954fULL,
        0xb05cbd6cc0cc10faULL, 0x94d9f4fb7ae3911dULL, 0xf9562e43b4931334ULL, 0xddd367d40ebc92d3ULL,
      };

      static constexpr u32 kCRC32[ 256 ]={
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Object:{                                        |

  namespace EON{

    namespace gfc{

      struct IPropertyEvents;
      struct IStreamEvents;
      struct IObjectEvents;

      /** \brief Auto-referencing base template.
        *
        * The Object class is a base class for all objects that wish to be
        * garbage collected by the system when they go out of scope. It's a poor
        * substitute for a full blown gc but works well enough for our uses in
        * the EON Engine.
        *
        * The e_property() macro is used here to present a globally consistent
        * interface to the reference count and classid. toRefs() will return a
        * reference to the ref-count and setRefs() will set it. See the
        * definition of the e_property macro in base/common.h.
        *
        * The e_var* macros do the same thing but without generating Property
        * objects.
        */

      struct E_PUBLISH Object{

        #define OBJECT_VERSION u16(2)

        //----------------------------------------+-----------------------------
        //Statics:{                               |

          /** \brief Current game turn.
            *
            * This variable contains the current game turn. This 64-bit value
            * is incremented once per frame before tick and draw are called. It
            * is the basis of the object time line functionality but can be
            * used for many other things too.
            */

          static u64 gameTurn;

        //}:                                      |
        //Aliases:{                               |

          /** \brief Create alias to property pointer.
            *
            * This type facilitates the property system by defining the
            * standard shared pointer to properties.
            */

          using prop_ptr = std::shared_ptr<Property>;
          using weak_ptr = std::weak_ptr<Property>;
          using prop_map = hashmap<u64,prop_ptr>;
          using prop_vec = vector<prop_ptr>;

          /** \brief Define object handle.
            *
            * This typedef fulfills the contract of all Object derived classes
            * must expose a handle type so you can say Entity::super::handle
            * all the way to the base Object type.  Object is the only class
            * that does not have a super member.
            */

          using handle = AutoRef<Object>;

          /** \brief Define self type.
            *
            * This type fulfills the contract of all Object classes that it
            * must have a self type that is the same as the class itself.
            */

          using self = Object;

          /** \brief Define const pointer type.
            *
            * This type fulfills the contract of all Object classes that it
            * must have a cptr type that is the same as the class itself.
            */

          using cptr = const self*;

          /** \brief Define const pointer type.
            *
            * This type fulfills the contract of all Object classes that it
            * must have a ptr type that is the same as the class itself.
            */

          using ptr = self*;

        //}:                                      |
        //Operate:{                               |

          /** \brief Object copy assignment operator.
            *
            * This operator will assign one object to another, duplicating it
            * as it goes.
            *
            * \param lvalue The object to be cloned.
            *
            * \return Returns *this.
            */

          Object& operator=( const Object& lvalue );

        //}:                                      |
        //Changes:{                               |

          /** \brief Default function for ignoring changes.
            *
            * This is passed into the e_property() macro to tell the system you
            * don't care about being informed when a property value changes,
            * either through script, the property system, or setNameProp().
            */

          void IGNORE_CHANGES(){}

        //}:                                      |
        //Methods:{                               |
          //Classification:{                      |

            /** \name Property system support.
              *
              * These methods are implemented in every object with an
              * e_reflect() declaration. It returns the root properties for the
              * complete inheritance chain.
              *
              * @{
              */

            /** \brief Flatten properties.
              *
              * This routine will flatten a tree of property maps.
              */

            static void flattenProperties( const prop_map& in, prop_map& out );

            /** \brief Get properties from object.
              *
              * This routine will get all the properties, unflattened, for this
              * object including all descendant classes.
              */

            virtual void getProperties( prop_map& )const{}

            /** \brief Get property object for this object.
              *
              * This routine will return the property object for the named
              * e_property in the derived object.
              */

            prop_ptr toProperty( ccp pPropertyName );

            /** @}
              *
              * \name Classification methods.
              *
              * These methods are designed to fulfill the role of object
              * classifier in the engine. They form the backbone of the EON
              * reflection system which extends C++'s RTTI with proper isa and
              * inverse new from class identifier.
              *
              * @{
              */

            /** \brief Return the classname of the object.
              *
              * This is part of the e_relect() generated API that every Object
              * class gets. It simply returns "Object".
              */

            constexpr static ccp classname(){
              return "Object";
            }

            /** \brief Type query.
              *
              * This routine will return true if the object is a member of the
              * given or named class. This is done through a tricky little
              * static function that is defined in the e_reflect() macro.
              */

            virtual bool isa( const u64 clsid )const;

            /** \brief Get the derivative class id.
              *
              * This routine returns the classid for the deepest derived child
              * class in the inheritance chain.
              *
              * \return Returns the classid, a hash of the type name "Object".
              */

            virtual u64 probeid()const;

            /** \brief Return the sizeof the class.
              *
              * This routine will return the size of the most derived class.
              *
              * \return Returns the sizeof the class even if all you have is
              * the base class Object.
              */

            virtual u32 asSizeof()const{
              return sizeof( *this );
            }

            /** \brief Get the object class name as a hash.
              *
              * This routine returns the hash of classof.
              */

            virtual u64 hashof()const;

            /** \brief Return the name of the object.
              *
              * This virtual interface returns the class name just like
              * classname() but in a way that can be overridden by the
              * descendant classes. In other words classname() always returns
              * the name of the class at a particular super level but classof()
              * returns the most descendant child class name.
              *
              * \return Returns the name of the class via classname().
              */

            virtual ccp classof()const{
              return classname();
            }

            /** \brief Return an inheritance chain.
              *
              * This routine will return the inheritance chain as a linux style
              * path. This is used in the asset database to layout the
              * hierarchy of directories.
              *
              * \return Returns the object type as a path
              */

            virtual const string& pathof()const;

            /** \brief The same as pathof() but accessible from everywhere.
              *
              * This routine will return the inheritance chain as a Linux/Unix
              * style path. It is the static version of pathof().
              *
              * \return Returns the object type as a path.
              */

            static const string& pathid();

          //}:                                    |
          //Serialization:{                       |

            /** @}
              *
              * \name Object serialization methods.
              *
              * These methods are used for serializing an object. It also
              * features methods that test whether background loading tasks
              * have completed.
              *
              * @{
              */

            /** \brief Block until IO complete shallow.
              *
              * This routine blocks until the IO complete flag is set. It only
              * checks this object's status bit not the entire data chain.
              */

            void blockUntilIOCompleteShallow()const;

            /** \brief Block until I/O complete.
              *
              * This routine will block the current thread until the object is
              * said to be I/O complete. This method must be implemented by all
              * object classes who have members that might not be I/O complete
              * at the time of the call. In other words any member AutoRef<>
              * [handle] that has been initialized with a call to e_stream.
              */

            void blockUntilIOComplete()const;

            /** \brief Shallow check if IO complete.
              *
              * This routine only checks if this object is IO complete not if
              * whole object chain is.
              */

            e_forceinline bool isIOCompleteShallow()const{
              if( m_tStatus->bIOComplete ){
                return true;
              }
              return false;
            }

            /** \brief Check if I/O complete and all descendants are I/O
              * complete.
              *
              * This routine must be implemented by all classes who have
              * members that may not be I/O complete. This includes all handles
              * that have been assigned a value via the e_stream macro or the
              * stream<> template function.
              *
              * \return Returns true if the object and all serializing members
              * are I/O complete, that is that their mStatus->bIOComplete flag
              * is set when the object is loaded.  If there was an error
              * loading the object then we must assume that the object is I/O
              * complete. In other words no more I/O can be applied to it and
              * we tell the caller that we're done with loading. It is up to
              * that routine to check the bIOError status bit to know whether
              * it is safe to use the handle or not. In actuality the caller
              * must throw the handle away if bIOError is set.
              */

            virtual bool isIOComplete()const;

            /** \brief Post serialize [out] method.
              *
              * This routine is called after the main serialize() method and
              * gives the class a chance to do something after saving happens.
              */

            virtual void postSerialize( fs::Writer& )const{}

            /** \brief Post serialize [in] method.
              *
              * This routine is called afteer the main serialize() method and
              * gives the class a chance to do something after saving happens.
              */

            virtual void postSerialize( fs::Reader& ){}

            /** \brief Pre serialize [out] method.
              *
              * This routine is called before the main serialize() method and
              * gives the class a chance to do something before saving begins.
              */

            virtual void preSerialize( fs::Writer& )const;

            /** \brief Pre serialize [in] method.
              *
              * This routine is called before the main serialize() method and
              * gives the class a chance to do something before saving begins.
              */

            virtual void preSerialize( fs::Reader& );

            /** \brief Serialize out interface.
              *
              * This interface is designed to allow serialization of an object
              * to memory and from there to disk. This interface is the primary
              * method of writing data to .eon files that the compiler and
              * engine uses for textures, meshes, entities, etc. When
              * serializing your own objects always be sure to call
              * super::serialize( fs ) as the first line in your code. Here's
              * an example from the Prop class of both serialize methods you
              * need to employ.
              \code
                void Prop::serialize( fs::Writer& fs )const{
                  Entity::serialize( fs );
                  fs.version( PROP_VERSION );
                  fs.write( m_fFadeDistance );
                  u8 hasMesh = bool( m_hMesh );
                  fs.write( hasMesh );
                  if( hasMesh ){
                    fs.serializeHandle( m_hMesh );
                  }
                }

                s64 Prop::serialize( fs::Reader& fs ){
                  Entity::serialize( fs );
                  fs.version( PROP_VERSION );
                  fs.read( m_fFadeDistance );
                  u8 hasMesh;
                  fs.read( hasMesh );
                  if( hasMesh ){
                    m_hMesh = fs.serializeHandle().as<Mesh>();
                  }
                  return UUID;
                }

              \endcode
              */

            virtual void serialize( fs::Writer& fs )const;

            /** \brief Serialize in interface.
              *
              * This interface is designed to load what serialize( fs::Writer&
              * ) wrote out. It is the backbone of all data transmission within
              * the engine.
              */

            virtual s64 serialize( fs::Reader& );

          //}:                                    |
          //Ownership:{                           |

            /** \brief Set the owning object.
              *
              * A sense of ownership is maintained at the object level, so we
              * can know at runtime what objects belong to what class. Not
              * widely used in the engine at this time but may come into
              * greater use with C#.
              *
              * \param uuid The owning class universal user identifier.
              */

            e_forceinline void setOwner( const u64 uuid ){
              m_uOwner = uuid;
            }

            /** \brief Get the owning object.
              *
              * This routine returns the owner object as UUID.
              *
              * \return Returns the UUID for this object's owner.
              */

            e_forceinline u64 toOwner()const{
              return m_uOwner;
            }

          //}:                                    |
          //Resources:{                           |

            /** @}
              *
              * \name Resource management methods.
              *
              * These two routines are there for setting up data for an object
              * lifetime and tearing them down again (purging). They are both
              * constant virtuals so you can only work on mutable types. They
              * are entirely in the user domina and will not be called by the
              * engine ever. That's what constructors et all are for.
              *
              * @{
              */

            virtual void setup()const{}
            virtual void purge()const{}

          //}:                                    |
          //Hashing:{                             |

            /** @}
              *
              * \name CRC methods for RTTI and string hashing.
              *
              * These CRC routines are used in the reflection system for RTTI
              * and in the string class for faster case sensitive comparisons.
              *
              * @{
              */

          protected:

            static string prettyup( const string& );

          private:

            constexpr static u64 const_crc64impl( const u64 prevCrc, ccp str, const u64 size ){
              return !size ? prevCrc : const_crc64impl(( prevCrc >> 8) ^ kCRC64[( prevCrc^*str ) & 0xff ], str+1, size-1 );
            }

            constexpr static u32 const_crc32impl( const u32 prevCrc, ccp str, const u32 size ){
              return !size ? prevCrc : const_crc32impl(( prevCrc >> 8) ^ kCRC32[( prevCrc^*str ) & 0xff ], str+1, size-1 );
            }

            static u64 crc64impl( u64 crc, ccp str, const u64 size ){
              ccp end = str + size;
              while( str < end ){
                crc = ( crc >> 8 ) ^ kCRC64[( crc^*str++ ) & 0xff ];
              }
              return crc;
            }

            static u32 crc32impl( u32 crc, ccp str, const u32 size ){
              ccp end = str + size;
              while( str < end ){
                crc = ( crc >> 8 ) ^ kCRC32[( crc^*str++ ) & 0xff ];
              }
              return crc;
            }

          public:

            /** \brief CRC a string.
              *
              * This routine will CRC a string.
              */

            static constexpr u64 const_crc64( ccp str, const u64 size ){
              return const_crc64impl( ~0ULL, str, size ) ^ ~0ULL;
            }

            /** \brief CRC a string.
              *
              * This routine will CRC a string.
              */

            static u64 crc64( ccp str, const u64 size ){
              return crc64impl( ~0ULL, str, size ) ^ ~0ULL;
            }

            /** \brief CRC a string.
              *
              * This routine will CRC a string.
              */

            static constexpr u32 const_crc32( ccp str, const u32 size ){
              return const_crc32impl( ~0U, str, size ) ^ ~0U;
            }

            /** \brief CRC a string.
              *
              * This routine will CRC a string.
              */

            static u32 crc32( ccp str, const u32 size ){
              return crc32impl( ~0U, str, size ) ^ ~0U;
            }

          //}:                                    |
          //Cloning:{                             |

            /** @}
              *
              * \name Cloning functions.
              *
              * Any object can be cloned if it's derived classes support it.
              *
              * @{
              */

          protected:

            virtual void onClone( const Object& )const{}

          public:

            /** \brief Clone object.
              *
              * This routine will clone the object.
              */

            handle clone()const;

          //}:                                    |
          //Naming:{                              |

            virtual ccp describe()const{
              return"";
            }

          //}:                                    |
          //Time:{                                |

            /** @}
              *
              * \name Timing functions.
              *
              * These routines establish the time surrounding object spawning.
              *
              * @{
              */

            /** \brief Set the "when" game turn.
              *
              * This routine is called by the reflection system to record the
              * game turn that this object was created on. This should never be
              * called by non-engine code. That's illegal!
              *
              * \param when The time when the object was created.
              */

            e_forceinline void setWhen( const u64 when ){
              m_uWhen = when;
            }

            /** \brief Get creation time.
              *
              * This routine will return the game turn that the object was
              * created on.
              */

            e_forceinline u64 when()const{
              return m_uWhen;
            }

          //}:                                    |
          //Refs:{                                |

            /** @}
              *
              * \name Referenceing counting methods.
              *
              * These methods are used to track the reference count of an
              * object.  When the reference count reaches zero the object is
              * retired but only the AutoRef object has the rights to manage
              * this.
              *
              * @{
              */

            /** \brief Increment reference count.
              *
              * This routine will increment the object's reference count. Do
              * not call this function, it exists only to satisfy the
              * relationship between the class template and the object.
              */

            e_forceinline void addweak()const{
              m_sWeak++;
            }

            /** \brief Decrement the reference count.
              *
              * This routine will decrement the object's reference count. Do
              * not call this function, it exists only to satisfy the
              * relationship between the class template and the object.
              */

            e_forceinline s32 subweak()const{
              return --m_sWeak;
            }

            /** \brief Get the reference count for the object.
              *
              * This routine will return the object's current reference count.
              * If it is zero then it has been deleted.
              */

            e_forceinline s32 weak()const{
              return m_sWeak;
            }

            /** \brief Increment reference count.
              *
              * This routine will increment the object's reference count. Do
              * not call this function, it exists only to satisfy the
              * relationship between the class template and the object.
              */

            e_forceinline void addref()const{
              ++m_sRefs;
            }

            /** \brief Decrement the reference count.
              *
              * This routine will decrement the object's reference count. Do
              * not call this function, it exists only to satisfy the
              * relationship between the class template and the object.
              */

            e_forceinline s32 subref()const{
              return --m_sRefs;
            }

            /** \brief Get the reference count for the object.
              *
              * This routine will return the object's current reference count.
              * If it is zero then it has been deleted.
              */

            e_forceinline s32 refs()const{
              return m_sRefs;
            }

            /** @} */

          //}:                                    |
        //}:                                      |
        //----------------------------------------+-----------------------------

        Object( const Object& lvalue )
            : m_tStatus( lvalue.m_tStatus )
            , m_uOwner(  lvalue.m_uOwner )
            , m_uWhen(   lvalue.m_uWhen ){
          m_sWeak.store( lvalue.m_sWeak );
          m_sRefs.store( lvalue.m_sRefs );
        }

        Object( Object&& rvalue )
            : m_tStatus( std::move( rvalue.m_tStatus ))
            , m_uOwner(  std::move( rvalue.m_uOwner ))
            , m_uWhen(   std::move( rvalue.m_uWhen )){
          m_sWeak.store( std::move( rvalue.m_sWeak ));
          m_sRefs.store( std::move( rvalue.m_sRefs ));
        }

        Object() = default;

        virtual~Object();

      protected:

        e_noinline_or_debug virtual prop_ptr ___property( const u64 )const{
          return nullptr;
        }

      private:

        /* Engine facing vars */

        e_var_volatile_bits( Status
          , bIOComplete  : 1 //!< The object has finished background loading, not necessarily all members.
          , bInitialized : 1 //!< The object has not been initialized if zero.
          , bUnavailable : 1 //!< Currently unavailable, wait awhile and it'll come back online.
          , bStallDelete : 1 //!< Do not delete this object in subref until this flag is zero.
          , bIOTouched   : 1 //!< IO touched bit.
          , bScripted    : 1 //!< Defined in C#.
          , bArchived    : 1 //!< IO archive bit.
          , bIOError     : 1 //!< And error ocurred during IO.
          , bDirty       : 1 //!< The object is dirty; meaning is per derived class.
        );

        /* Purely private */

        u64                      m_uOwner = 0; //!< The class id in the reflection database.
        u64                      m_uWhen  = 0; //!< The game turn when object was created.
        mutable std::atomic<s32> m_sWeak{};    //!< The current reference count.
        mutable std::atomic<s32> m_sRefs{};    //!< The current reference count.

      public:

        s64 UUID = 0;

        #if e_compiling( debug )
          cp  file = nullptr;
          int line = 0;
        #endif
      };

      /** \brief Base object interface.
        *
        * This interface defines property APIs common to all objects.
        *
        * \todo Deprecate and remove this class.
        */

      struct E_PUBLISH IObjectEvents{
      };

      /** \brief Base object interface.
        *
        * This interface defines property APIs common to all objects.
        *
        * \todo Deprecate and remove this class.
        */

      struct E_PUBLISH IPropertyEvents{
        virtual void onPropertySignal( const Object::prop_map* )=0;
        virtual void onStartEditing( cvp payload )=0;
        virtual void onEndEditing()=0;
      };

      /** \brief Base object for events.
        *
        * This interface defines the base listener for events.
        */

      struct E_PUBLISH IStreamEvents{
        virtual void onCompilerSignal()=0;
        virtual~IStreamEvents()=default;
      };
    }

    /** \brief Macro for hashing literal strings.
      *
      * This macro will produce a 32-bit hash of the literal string you give
      * it.
      *
      * \param x The string to hash.
      */

    #define e_hashstr64_const( x )                                              \
      EON::gfc::Object::const_crc64( x, sizeof( x )-1 )                         \

    /** \brief Macro for hashing literal strings.
      *
      * This macro will produce a 32-bit hash of the literal string you give
      * it.
      *
      * \param i Either 32 or 64.
      * \param x The string to hash.
      */

    #define e_hashstr64( x )                                                    \
      EON::gfc::Object::crc64( x, EON::u32( strlen( x )))                       \

    /** \brief Macro for hashing literal strings.
      *
      * This macro will produce a 32-bit hash of the literal string you give
      * it.
      *
      * \param x The string to hash.
      */

    #define e_hashstr_const( x )                                                \
      EON::gfc::Object::const_crc32( x, sizeof( x )-1 )                         \

    /** \brief Macro for hashing literal strings.
      *
      * This macro will produce a 32-bit hash of the literal string you give it.
      *
      * \param i Either 32 or 64.
      * \param x The string to hash.
      */

    #define e_hashstr( x )                                                        \
      EON::gfc::Object::crc32( x, EON::u32( strlen( x )))                         \

    /** \brief Implementation of the hashof member function.
      *
      * This routine returns the hashof the classof.
      */

    e_forceinline u64 gfc::Object::hashof()const{
      return e_hashstr64_const( "Object" );
    }

    inline constexpr u64 operator""_64( char const*const s, const size_t n ){
      return gfc::Object::const_crc64( s, n );
    }

    inline constexpr u32 operator""_32( char const*const s, const size_t n ){
      return gfc::Object::const_crc32( s, u32( n & 0xFFFFFFFF ));
    }
  }

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
