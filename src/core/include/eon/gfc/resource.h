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
  *   \addtogroup renderer
  *   @{
  *     \addtogroup stream
  *     @{
  */

//================================================+=============================
//Stream:{                                        |

  namespace EON{

    namespace gfc{

      /** \brief Stream object.
        *
        * This object represents all stream types in the system. The chief
        * task of the class is to hold a name and other global metrics that are
        * common to all streams in the system. This includes textures, fonts,
        * meshes, terrains and materials among others.
        */

      struct E_PUBLISH Stream:Object,listener<IStreamEvents>{

        e_reflect( Stream, Object );

        /** \brief Stream version.
          *
          * This equate defines the version of the Stream object. This is
          * used in serialization to make sure any changes to the file format
          * are caught automatically by the reading code. The eon compiler,
          * game and editor cannot get out of sync version wise.
          */

        #define STREAM_VERSION u16(4)

        //----------------------------------------+-----------------------------
        //Structs:{                               |

          enum class Priority:u32{
              kNone
            , kHighest
            , kHigh
            , kNormal
            , kLow
            , kLowest
          };

        //}:                                      |
        //Operate:{                               |

          Stream& operator=( const Stream& lvalue );

        //}:                                      |
        //Methods:{                               |

          /** \name Serialization methods.
            *
            * These methods are used to serialize the stream data in and out
            * of history buffers and IO streams.
            *
            * @{
            */

          virtual void postSerialize( fs::Writer& )const override;
          virtual void preSerialize(  fs::Writer& )const override;
          virtual void postSerialize( fs::Reader& )override;
          virtual void preSerialize(  fs::Reader& )override;

          /** \brief Serialize the stream out to disk.
            *
            * The stream serializer simply writes out a version, the stream
            * name and the path to the original source asset on disk. Other
            * than that it calls down to the Object serializer which does
            * something similar.
            *
            * \param fs The writer file system.
            */

          virtual void serialize( fs::Writer& fs )const override;

          /** \brief Serialize stream from disk.
            *
            * This routine will reconstruct the stream object from a disk
            * based .eon file. All the method does is pull in the stream and
            * check its version.  It is very important that the base class
            * <b>always</b> be called first.
            *
            * \param fs The reader file system.
            *
            * \return Returns the UUID for this object. Must not be zero
            * because that signifies an error. All UUIDs are 1 based when
            * allocated by the system.
            */

          virtual s64 serialize( fs::Reader& fs )override;

          /** @}
            *
            * \name Events.
            *
            * These member functions are callbacks for a variety of events.
            *
            * @{
            */

          virtual void onCompilerSignal()override{}

          /** @}
            *
            * \name Stream identifier.
            *
            * These methods deal with the stream id.
            *
            * @{
            */

          /** \brief Get the texture id.
            *
            * This routine determines whether the id is the SHA1 key or the
            * name.
            */

          e_forceinline u64 getID()const{
            if( m_sSHA1.empty() ){
              return m_sName.hash();
            }
            return m_sSHA1.hash();
          }

          /** \brief Get the stream ID.
            *
            * This routine returns a string representing the stream ID.
            */

          e_forceinline const string& getStreamID()const{
            if( m_sSHA1.empty() ){
              return m_sName;
            }
            return m_sSHA1;
          }

          virtual ccp describe()const override{
            if( m_sComment.empty() ){
              return toName();
            }
            return m_sComment;
          }

          /** @} */

        //}:                                      |
        //----------------------------------------+-----------------------------

        explicit Stream( const string& sName );
        explicit Stream( string&& sName );
        Stream( const Stream& r );
        Stream() = default;

      private:

        e_property_group( Stream,
          e_property_block( Identification,
            e_property_string( Comment, "",             false, IGNORE_CHANGES, "Notes on this stream by the user."              );
            e_property_string( Name,    string::guid(), false, IGNORE_CHANGES, "The name of this asset database stream object; initially, a GUID, but feel free to rename it to any unique identifier." );
          );
          e_property_block( Tracking,
            e_property_string( Path, "<internal>", true,  IGNORE_CHANGES, "The original path for this object."          );
            e_property_string( SHA1, "",           true,  IGNORE_CHANGES, "The unique hash key for this object's data." );
          );
          e_property_block(  Priority,
            e_property_enum( Priority
              , Class
              , kNone
              , false
              , IGNORE_CHANGES
              , "Streaming priority class for this object."
              , "None"
              , "Highest"
              , "High"
              , "Normal"
              , "Low"
              , "Lowest"
            );
          );
        );
      };
    }
  }

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
