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
#include<stdarg.h>
#include<stdio.h>
#include<mutex>

using namespace EON;
using namespace gfc;

//================================================|=============================
//Private:{                                       |

  e_noinline atomic::ShareLockRecursive& lock(){
      static atomic::ShareLockRecursive _lock;
    return _lock;
  }
  #define e_syncr() e_guardr( lock() )
  #define e_syncw() e_guardw( lock() )
  namespace{
    using unordered_map = std::unordered_map<u64,std::unique_ptr<stream>>;
    unordered_map& db(){
      static unordered_map m;
      return m;
    }
  __thread char catvBuffer[ string::CATV_SIZE ];
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//Public:{                                        |
  //e_strsplit:{                                  |

#ifdef __APPLE__
  #pragma mark - Functions -
#endif

    bool e_strsplit( cp& inout_L, const char at, cp& out_R ){
      e_sanity_check( !e_isbad( inout_L ));
      e_sanity_check( !e_isbad( out_R ));
      if( inout_L && at ){
        cp M = strchr( inout_L, at );
        while( M[-1] == ' ' ){
          --M;
        }
        u32 N = u32( M - inout_L );
        ccp L = inout_L;
        inout_L = new char[ N+1 ];
        memcpy( inout_L, L, N );
        inout_L[ N ] = 0;
        out_R = M + 1;
        return true;
      }
      return false;
    }

  //}:                                            |
  //e_pathof:{                                    |

    string e_pathof( const u64 clsid, ccp subattr ){
      Object::handle hObject = e_newt( clsid );
      Object& object = hObject.cast();
      string path = "src/data/";
      if( !subattr ){
        return path + object.pathof();
      }
      path += object.pathof() + subattr;
      if( *path.right( 1 ) != '/' ){
        path += '/';
      }
      return path;
    }

  //}:                                            |
  //e_attrof:{                                    |

    string e_attrof( const u64 clsid, ccp subattr ){
      string attr = e_pathof( clsid, subattr );
      attr.replace( "/", " " );
      attr.ltrim( 9 );
      attr.trim( 1 );
      attr = ( "id " + attr ).camelcase();
      return attr;
    }

  //}:                                            |
  //e_strto:{                                     |

    void e_strto( const string& value, const u64 clsid, vp pData ){

      e_sanity_check( !e_isbad( pData ));

      //------------------------------------------------------------------------
      // Real metric numbers.
      //------------------------------------------------------------------------

      if( clsid == e_classid<ssi_mass<f64::Value>>() ){
        *reinterpret_cast<f64*>( pData ) = value.as<f64>();
      }else if( clsid == e_classid<ssi_mass<f64>>() ){
        *reinterpret_cast<f64*>( pData ) = value.as<f64>();
      }else if( clsid == e_classid<ssi_mass<f32::Value>>() ){
        *reinterpret_cast<f32*>( pData ) = value.as<f32>();
      }else if( clsid == e_classid<ssi_mass<f32>>() ){
        *reinterpret_cast<f32*>( pData ) = value.as<f32>();
      }else if( clsid == e_classid<ssi<f64>>() ){
        *reinterpret_cast<f64*>( pData ) = value.as<f64>();
      }else if( clsid == e_classid<ssi<f32>>() ){
        *reinterpret_cast<f32*>( pData ) = value.as<f32>();

      //------------------------------------------------------------------------
      // Real numbers.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<double>() ){
        *reinterpret_cast<double*>( pData ) = value.as<double>();
      }else if( clsid == e_classid<f64::Value>() ){
        reinterpret_cast<f64::Value*>( pData )->setValue( value.as<f64>() );
      }else if( clsid == e_classid<f64>() ){
        *reinterpret_cast<f64*>( pData ) = value.as<f64>();
      }else if( clsid == e_classid<float>() ){
        *reinterpret_cast<float*>( pData ) = value.as<float>();
      }else if( clsid == e_classid<f32::Value>() ){
        reinterpret_cast<f32::Value*>( pData )->setValue( value.as<f32>() );
      }else if( clsid == e_classid<f32>() ){
        *reinterpret_cast<f32*>( pData ) = value.as<f32>();
      }else if( clsid == e_classid<f16>() ){
        *reinterpret_cast<f16*>( pData ) = value.as<f32>();

      //------------------------------------------------------------------------
      // Boolean.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<bool>() ){
        *reinterpret_cast<bool*>( pData ) = value.as<bool>();

      //------------------------------------------------------------------------
      // Integers.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<u64>() ){
        *reinterpret_cast<u64*>( pData ) = value.as<u64>();
      }else if( clsid == e_classid<s64>() ){
        *reinterpret_cast<s64*>( pData ) = value.as<s64>();
      }else if( clsid == e_classid<u32>() ){
        *reinterpret_cast<u32*>( pData ) = value.as<u32>();
      }else if( clsid == e_classid<s32>() ){
        *reinterpret_cast<s32*>( pData ) = value.as<s32>();
      }else if( clsid == e_classid<u16>() ){
        *reinterpret_cast<u16*>( pData ) = value.as<u16>();
      }else if( clsid == e_classid<s16>() ){
        *reinterpret_cast<s16*>( pData ) = value.as<s16>();
      }else if( clsid == e_classid<u8>() ){
        *reinterpret_cast<u8*>( pData ) = value.as<u8>();
      }else if( clsid == e_classid<s8>() ){
        *reinterpret_cast<s8*>( pData ) = value.as<s8>();

      //------------------------------------------------------------------------
      // Strings.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<string>() ){
        *reinterpret_cast<gfc::string*>( pData ) = value;

      //------------------------------------------------------------------------
      // Transforms.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<vec4x4>() ){
      }else if( clsid == e_classid<qst3>() ){

      //------------------------------------------------------------------------
      // Vectors and points.
      //------------------------------------------------------------------------

      }else if(( clsid == e_classid<vec4>() )||( clsid == e_classid<ssi<vec4>>() )||( clsid == e_classid<ssi_mass<vec4>>() )){
        *reinterpret_cast<vec4*>( pData ) = value.as<vec4>();
      }else if(( clsid == e_classid<vec3>() )||( clsid == e_classid<ssi<vec3>>() )||( clsid == e_classid<ssi_mass<vec3>>() )){
        *reinterpret_cast<vec3*>( pData ) = value.as<vec3>();
      }else if(( clsid == e_classid<vec2>() )||( clsid == e_classid<ssi<vec2>>() )||( clsid == e_classid<ssi_mass<vec2>>() )){
        *reinterpret_cast<vec2*>( pData ) = value.as<vec2>();
      }else if(( clsid == e_classid<pt3>() )||( clsid == e_classid<ssi<pt3>>() )||( clsid == e_classid<ssi_mass<pt3>>() )){
        *reinterpret_cast<pt3*>( pData ) = value.as<pt3>();
      }else if(( clsid == e_classid<pt2>() )||( clsid == e_classid<ssi<pt2>>() )||( clsid == e_classid<ssi_mass<pt2>>() )){
        *reinterpret_cast<pt2*>( pData ) = value.as<pt2>();
      }else if( clsid == e_classid<vec4i>() ){
        *reinterpret_cast<vec4i*>( pData ) = value.as<vec4i>();
      }else if( clsid == e_classid<vec3i>() ){
        *reinterpret_cast<vec3i*>( pData ) = value.as<vec3i>();
      }else if( clsid == e_classid<vec2i>() ){
        *reinterpret_cast<vec2i*>( pData ) = value.as<vec2i>();

      //------------------------------------------------------------------------
      // Bounding volumes.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<aabb3>() ){
        *reinterpret_cast<aabb3*>( pData ) = value.as<aabb3>();
      }else if( clsid == e_classid<aabb2>() ){
        *reinterpret_cast<aabb2*>( pData ) = value.as<aabb2>();

      //------------------------------------------------------------------------
      // Rotations.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<quat>() ){
        *reinterpret_cast<quat*>( pData ) = value.as<quat>();

      //------------------------------------------------------------------------
      // Colors.
      //------------------------------------------------------------------------

      }else if( clsid == e_classid<rgba>() ){
        *reinterpret_cast<rgba*>( pData ) = value.as<rgba>();
      }else{
        DEBUG_BREAK
      }
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//string:{                                        |
  //Private:{                                     |
    //kmp:{                                       |

      namespace{
        s32* prekmp( ccp pattern, const s32 n_pattern ){
          s32 k = -1;
          s32 i = 1;
          s32* pi = (s32*)catvBuffer;
          *pi = k;
          for( i=1; i<n_pattern; ++i ){
            while(( k > -1 )&&( pattern[k+1] != pattern[i] )){
              k = pi[k];
            }
            if( pattern[i] == pattern[k+1] ){
              k++;
            }
            pi[i] = k;
          }
          return pi;
        }
        std::vector<s32> kmp( ccp target, const s32 n_target, ccp pattern, const s32 n_pattern ){
          std::vector<s32> matches;
          s32 i;
          s32 *pi = prekmp( pattern, n_pattern );
          s32 k = -1;
          for( i=0; i<n_target; ++i ){
            while(( k > -1 )&&( pattern[k+1] != target[i] )){
              k = pi[k];
            }
            if( target[i] == pattern[k+1] ){
              k++;
            }
            if( k == n_pattern-1 ){
              matches.push_back( i-k );
              k = -1;
            }
          }
          return matches;
        }
      }

    //}:                                          |
  //}:                                            |
  //Operate:{                                     |
    //operator!=:{                                |

#ifdef __APPLE__
  #pragma mark - Operators -
#endif

      bool string::operator!=( const string& s )const{
        e_syncr();
        const bool iSempty = s.empty();
        const bool isEmpty = empty();
        if( isEmpty && !iSempty ){
          return true;
        }
        if( !isEmpty && iSempty ){
          return true;
        }
        if( isEmpty && iSempty ){
          return false;
        }
        return( m_uHash != s.m_uHash );
      }

      bool string::operator!=( ccp pSrc )const{
        e_syncr();
        const bool isEmpty = empty();
        if( !isEmpty && !pSrc ){
          return true;
        }
        if( isEmpty && pSrc ){
          return true;
        }
        if( isEmpty && !pSrc ){
          return false;
        }
        return( m_uHash != e_hashstr64( pSrc ));
      }

    //}:                                          |
    //operator==:{                                |

      bool string::operator==( const string& s )const{
        e_syncr();
        const bool iSempty = s.empty();
        const bool isEmpty = empty();
        if( isEmpty && !iSempty ){
          return false;
        }
        if( !isEmpty && iSempty ){
          return false;
        }
        if( isEmpty && iSempty ){
          return true;
        }
        return( m_uHash == s.m_uHash );
      }

      bool string::operator==( ccp pSrc )const{
        e_syncr();
        const bool isEmpty = empty();
        if( !isEmpty && !pSrc ){
          return false;
        }
        if( isEmpty && pSrc ){
          return false;
        }
        if( isEmpty && !pSrc ){
          return true;
        }
        return( m_uHash == e_hashstr64( pSrc ));
      }

    //}:                                          |
    //operator=:{                                 |

      string& string::operator=( const string& lvalue ){
        e_syncr();
        if(( this != &lvalue )&&( m_uHash != lvalue.m_uHash )){
          clear();
          cat( lvalue );
        }
        return *this;
      }

      string& string::operator=( string&& rvalue ){
        e_syncw();
        if(( this != &rvalue )&&( m_uHash != rvalue.m_uHash )){
          clear();
          if( rvalue.m_uHash ){
            m_uHash = rvalue.m_uHash;
            rvalue.m_uHash = 0;
            #if e_compiling( debug )
              m_pData = std::move( rvalue.m_pData );
            #endif
          }
        }
        return *this;
      }

      string& string::operator=( ccp pValue ){
        clear();
        if( pValue ){
          cat( pValue );
        }
        return *this;
      }

    //}:                                          |
    //operator+:{                                 |

      string string::operator+( const string& s )const{
        string r;
        r.cat( *this );
        r.cat( s );
        return r;
      }

      string string::operator+( ccp p )const{
        string r;
        r.cat( *this );
        if( p ){
          r.cat( p );
        }
        return r;
      }

    //}:                                          |
    //operator<:{                                 |

      bool string::operator<( const string& s )const{
        e_syncr();
        if( this != &s ){
          return( strcmp( c_str(), s.c_str() ) < 0 );
        }
        return false;
      }

      bool string::operator<( ccp pCmp )const{
        return( strcmp( c_str(), pCmp ) < 0 );
      }

    //}:                                          |
    //operator>:{                                 |

      bool string::operator>( const string& s )const{
        e_syncr();
        if( this != &s ){
          return( strcmp( c_str(), s.c_str() ) > 0 );
        }
        return false;
      }

      bool string::operator>( ccp pCmp )const{
        return( strcmp( c_str(), pCmp ) > 0 );
      }

    //}:                                          |
  //}:                                            |
  //Structs:{                                     |

#ifdef __APPLE__
  #pragma mark - Classes -
#endif

    enum class Status:s8{
      kNaN=-1,
      kFalse,
      kTrue,
    };

  //}:                                            |
  //Private:{                                     |
    //asDecimal:{                                 |

      namespace{
        template<typename T>Status asDecimal( ccp pString, T& value ){
          e_sanity_check( !e_isbad( pString ));
          cp s = string::skip_anyws( cp( pString ));
          if( !s ){
            return Status::kNaN;
          }
          cp E = s+strlen( s );

          //--------------------------------------------------------------------
          // Hexidecimal numbers
          //--------------------------------------------------------------------

          bool bIsHexidecimal=false;
          if( !strncmp( s, "0x", 2 )){
            bIsHexidecimal = true;
            s += 2;
          }else if( *s == '$' ){
            bIsHexidecimal = true;
            s++;
          }
          if( bIsHexidecimal ){
            cp e = s;
            while(( isdigit( *e )||(( *e >= 'A' )&&( *e <= 'F' ))||(( *e >= 'a' )&&( *e <= 'f' )))&&( e < E )){
              ++e;
            }
            if( e == s ){
              return Status::kNaN;
            }
            if( string::is_alpha( *e )){
              //failed hexadecimal test but still alpha so NaN.
              return Status::kNaN;
            }
            T shift = 0;
            value = 0;
            while( --e >= s ){
              if(( *e >= '0' )&&( *e <= '9' )){
                value |= (*e-'0')<<shift;
              }else if(( *e >= 'A' )&&( *e <= 'F' )){
                value |= (*e-'A'+10)<<shift;
              }else if(( *e >= 'a' )&&( *e <= 'f' )){
                value |= (*e-'a'+10)<<shift;
              }else{
                return Status::kNaN;
              }
              shift+=4;
            }
            return Status::kTrue;
          }

          //--------------------------------------------------------------------
          // Binary numbers: 0101_0001_1111_1011011
          //--------------------------------------------------------------------

          bool bIsBinary=false;
          if( !strncmp( s, "0b", 2 )){
            bIsBinary = true;
            s += 2;
          }
          if( bIsBinary ){
            cp e = s;
            while((( *e == '0' )||( *e == '1' )||( *e == '_' ))&&( e < E )){
              ++e;
            }
            if( e == s ){
              return Status::kNaN;
            }
            T shift = 0;
            value = 0;
            while( --e >= s ){
              if(( *e == '0' )||( *e == '1' )){
                value |= (*e-'0')<<shift;
              }else if( *e=='_' ){
                continue;
              }else{
                return Status::kNaN;
              }
              shift++;
            }
            return Status::kTrue;
          }

          //--------------------------------------------------------------------
          // Decimal numbers
          //--------------------------------------------------------------------

          cp e = s;
          while( string::is_digit( *e )&&( e < E )){
            ++e;
          }
          if( e == s ){
            value = T( atof( s ));
          }else{
            if( string::is_alpha( *e )){
              e_assert( !"invalid number in string" );
              return Status::kNaN;
            }
            T shift = 1;
            while( --e >= s ){
              value += (*e-'0')*shift;
              shift *= 10;
            }
          }
          return Status::kTrue;
        }
      }

    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //[parsing]:{                                 |
      //skip_anyws:{                              |

#ifdef __APPLE__
  #pragma mark - Skip methods -
#endif

        cp string::skip_anyws( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = cp( in );
          while(( *s > 0 )&&( *s <= ' ' )){
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skip_anynonws:{                           |

        cp string::skip_anynonws( ccp in ){
          e_sanity_check( !e_isbad( in ));
          const u8* s = (u8*)in;
          if( *s <= 32 ){
            return cp( s );
          }
          if( is_alpha( *s )||( *s == '_' )){
            while( *s &&(( *s == '_' )||is_alpha( *s )||is_digit( *s ))){
              ++s;
            }
            return cp( s );
          }
          while( *s ){
            if( *s <= 32 ){
              break;
            }
            ++s;
          }
          return cp( s );
        }

      //}:                                        |
      //skip_nonws:{                              |

        cp string::skip_nonws( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = cp( in );
          while(( *s > 0x0 )&&(
              (( *s >= 'a' )&&( *s <= 'z' ))
            ||(( *s >= 'A' )&&( *s <= 'Z' ))
            ||(( *s >= '0' )&&( *s <= '9' )))){
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skip_2eol:{                               |

        cp string::skip_2eol( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = cp( in );
          while( !is_eol( *s )){
            if( !*s ){
              return nullptr;
            }
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skipeols:{                                |

        cp string::skip_eols( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = cp( in );
          while(( *s > 0 )&& is_eol( *s )){
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skipws:{                                  |

        cp string::skip_ws( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = cp( in );
          while(( *s <= ' ' )&&( *s != 13 )&&( *s != 10 )){
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skipnumber:{                              |

        cp string::skip_number( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = skip_anyws( in );
          if(( *s == '-' )||( *s == '+' )){
            ++s;
          }
          bool dot=false;
          bool exp=false;
          while((( *s>='0' )&&( *s<='9' ))||( *s=='.' )){
            if( *s == '.' ){
              if( !dot ){
                dot = true;
              }else{
                break;
              }
            }
            if( *s == 'e' ){
              if( !exp ){
                if(( s[1] == '-' )||( s[1] == '+' )){
                  s++;
                }
                exp = true;
              }else{
                break;
              }
            }
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skipcc:{                                  |

        //skip c comment.
        cp string::skip_cc( ccp in ){
          e_sanity_check( !e_isbad( in ));
          u32 d = 0;
          cp s = skip_anyws( in );
          while( *s > 0 ){
            if(( s[0] == '/' )&&( s[1] == '/' )&& !d ){
              s = skip_2eol( s );
              s = skip_eols( s );
              break;
            }
            if(( s[0] == '/' )&&( s[1] == '*' )){
              ++s;
              ++d;
            }
            if(( s[0] == '*' )&&( s[1] == '/' )){
              ++s;
              --d;
              if( !d ){
                break;
              }
            }
            ++s;
          }
          return s;
        }

      //}:                                        |
      //skipstr:{                                 |

        cp string::skip_str( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = skip_anyws( cp( in ));
          if( *s == '"' ){
            s=strchr( s+1, '"' );
          }else if( *s == '\'' ){
            s=strchr( s+1, '\'' );
          }
          return s;
        }

      //}:                                        |
      //is_eol:{                                  |

#ifdef __APPLE__
  #pragma mark - Is methods -
#endif

        bool string::is_eol( const char c ){
          return( c == 10 )||( c == 13 );
        }

      //}:                                        |
      //is_hex:{                                  |

        bool string::is_hex()const{
          ccp p = c_str();
          ccp e = end();
          while( p < e ){
            if(!( is_digit( *p )||((( *p >= 'a' )&&( *p <= 'f' ))||(( *p >= 'A' )&&( *p <= 'F' ))))){
              return false;
            }
            ++p;
          }
          return true;
        }

      //}:                                        |
      //iscc:{                                    |

        bool string::is_cc( ccp s ){
          e_sanity_check( !e_isbad( s ));
          if( skip_cc( s ) > s ){
            return true;
          }
          return false;
        }

      //}:                                        |
      //isalpha:{                                 |

        bool string::is_alpha( const char ascii_char ){
          if(( ascii_char >= 'a' )&&( ascii_char <= 'z' )){
            return true;
          }
          if(( ascii_char >= 'A' )&&( ascii_char <= 'Z' )){
            return true;
          }
          return false;
        }

        bool string::is_alpha( ccp in ){
          e_sanity_check( !e_isbad( in ));
          ccp s = skip_anyws( in );
          return is_alpha( *s );
        }

      //}:                                        |
      //isupper:{                                 |

        bool string::is_upper( const char c ){
          return( c >= 'A' )&&( c <= 'Z' );
        }

        bool string::is_upper( ccp s ){
          e_sanity_check( !e_isbad( s ));
          while( *s ){
            if( !is_upper( *s )){
              return false;
            }
            ++s;
          }
          return true;
        }

      //}:                                        |
      //islower:{                                 |

        bool string::is_lower( const char c ){
          return( c >= 'a' )&&( c <= 'z' );
        }

        bool string::is_lower( ccp s ){
          e_sanity_check( !e_isbad( s ));
          while( *s ){
            if( !is_lower( *s )){
              return false;
            }
            ++s;
          }
          return true;
        }

      //}:                                        |
      //isid:{                                    |

        bool string::is_id( const char ascii_char, const bool bFirst ){
          if( ascii_char == '_' ){
            return true;
          }
          if( is_alpha( ascii_char )){
            return true;
          }
          if( !bFirst && is_digit( ascii_char )){
            return true;
          }
          return false;
        }

        bool string::is_id( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = skip_anyws( in );
          if( !*s ){
            return false;
          }
          if( !is_id( *s, true )){
            return false;
          }
          while( *++s > 0 ){
            if( !is_id( *s, false )){
              return false;
            }
          }
          return true;
        }

      //}:                                        |
      //isnm:{                                    |

        bool string::is_digit( const char c ){
          if( c >= '0' && c <= '9' ){
            return true;
          }
          return false;
        }

        bool string::is_number( ccp in ){
          e_sanity_check( !e_isbad( in ));
          cp s = skip_anyws( in );
          if( skip_number( s ) > s ){
            return true;
          }
          return false;
        }

      //}:                                        |
    //}:                                          |
    //[as]:{                                      |
      //as<strings>:{                             |

#ifdef __APPLE__
  #pragma mark - As methods -
#endif

        namespace EON{
          namespace gfc{
            template<> strings string::as<strings>( ccp pString ){
              return string( pString ).splitLines();
            }
          }
        }

      //}:                                        |
      //as<double>:{                              |

        namespace EON{
          namespace gfc{
            template<> double string::as<double>( ccp pString ){
              return atof( pString );
            }
          }
        }

      //}:                                        |
      //as<float>:{                               |

        namespace EON{
          namespace gfc{
            template<> float string::as<float>( ccp pString ){
              return atof( pString );
            }
          }
        }

      //}:                                        |
      //as<vec4x4>:{                              |

        namespace EON{
          namespace gfc{
            template<> vec4x4 string::as<vec4x4>( ccp pString ){
              float f[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return vec4x4(
                  f[ 0], f[ 1], f[ 2], f[ 3],
                  f[ 4], f[ 5], f[ 6], f[ 7],
                  f[ 8], f[ 9], f[10], f[11],
                  f[12], f[13], f[14], f[15] );
            }
          }
        }

      //}:                                        |
      //as<aabb3>:{                               |

        namespace EON{
          namespace gfc{
            template<> aabb3 string::as<aabb3>( ccp pString ){
              float f[6]={0,0,0,0,0,0};
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return aabb3( f[0], f[1], f[2], f[3], f[4], f[5] );
            }
          }
        }

      //}:                                        |
      //as<aabb2>:{                               |

        namespace EON{
          namespace gfc{
            template<> aabb2 string::as<aabb2>( ccp pString ){
              float f[4]={0,0,0,0};
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return aabb2( f[0], f[1], f[2], f[3] );
            }
          }
        }

      //}:                                        |
      //as<quat>:{                                |

        namespace EON{
          namespace gfc{
            template<> quat string::as<quat>( ccp pString ){
              float f[3]={ 0,0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              quat q;
              q.setEuler( vec3( f[0], f[1], f[2] ));
              return q;
            }
          }
        }

      //}:                                        |
      //as<rgba>:{                                |

        namespace EON{
          namespace gfc{
            template<> rgba string::as<rgba>( ccp pString ){
              rgba c = rgba::kBlack;
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<4; ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  c.tuple[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return c;
            }
          }
        }

      //}:                                        |
      //as<vec4i>:{                               |

        namespace EON{
          namespace gfc{
            template<> vec4i string::as<vec4i>( ccp pString ){
              s32 f[4]={ 0,0,0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<s32>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return *(vec4i*)f;
            }
          }
        }

      //}:                                        |
      //as<vec4>:{                                |

        namespace EON{
          namespace gfc{
            template<> vec4 string::as<vec4>( ccp pString ){
              float f[4]={ 0,0,0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return vec4( f[0], f[1], f[2], f[3] );
            }
          }
        }

      //}:                                        |
      //as<pt3>:{                                 |

        namespace EON{
          namespace gfc{
            template<> pt3 string::as<pt3>( ccp pString ){
              float f[3]={ 0,0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return pt3( f[0], f[1], f[2] );
            }
          }
        }

      //}:                                        |
      //as<vec3i>:{                               |

        namespace EON{
          namespace gfc{
            template<> vec3i string::as<vec3i>( ccp pString ){
              s32 f[3]={ 0,0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<s32>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return *(vec3i*)f;
            }
          }
        }

      //}:                                        |
      //as<vec3>:{                                |

        namespace EON{
          namespace gfc{
            template<> vec3 string::as<vec3>( ccp pString ){
              pt3 p = as<pt3>( pString );
              return vec3( p.x, p.y, p.z );
            }
          }
        }

      //}:                                        |
      //as<pt2>:{                                 |

        namespace EON{
          namespace gfc{
            template<> pt2 string::as<pt2>( ccp pString ){
              float f[2]={ 0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return pt2( f[0], f[1] );
            }
          }
        }

      //}:                                        |
      //as<vec2i>:{                               |

        namespace EON{
          namespace gfc{
            template<> vec2i string::as<vec2i>( ccp pString ){
              s32 f[2]={ 0,0 };
              if( pString ){
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<s32>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
              }
              return *(vec2i*)f;
            }
          }
        }

      //}:                                        |
      //as<vec2>:{                                |

        namespace EON{
          namespace gfc{
            template<> vec2 string::as<vec2>( ccp pString ){
              pt2 p = as<pt2>( pString );
              return vec2( p.x, p.y );
            }
          }
        }

      //}:                                        |
      //as<qst3>:{                                |

        namespace EON{
          namespace gfc{
            template<> qst3 string::as<qst3>( ccp pString ){
              qst3 L2W;
              if( pString ){
                float f[7]={ 0,0,0,0,0,0,0 };
                cp s = cp( pString );
                cp E = s+strlen( s );
                for( u32 i=0; i<e_dimof( f ); ++i ){
                  s = string::skip_anyws( s );
                  cp e = strchr( s, ',' );
                  if( !e ){
                    e = strchr( s, ' ' );
                  }
                  string expr( s, e ? e : E );
                  f[i] = expr.as<float>();
                  if( !e ){
                    break;
                  }
                  s = e+1;
                }
                L2W.toPosition() = pt3( f[0], f[1], f[2] );
                L2W.toRotation().setEuler( vec3( f[3], f[4], f[5] ));
                L2W.setScale( f[6] );
              }
              return L2W;
            }
          }
        }

      //}:                                        |
      //as<?64>:{                                 |

        namespace EON{
          namespace gfc{
            template<> u64 string::as<u64>( ccp pString ){
              u64 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
            template<> s64 string::as<s64>( ccp pString ){
              s64 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
          }
        }

      //}:                                        |
      //as<?32>:{                                 |

        namespace EON{
          namespace gfc{
            template<> u32 string::as<u32>( ccp pString ){
              u32 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
            template<> s32 string::as<s32>( ccp pString ){
              s32 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
          }
        }

      //}:                                        |
      //as<u16>:{                                 |

        namespace EON{
          namespace gfc{
            template<> u16 string::as<u16>( ccp pString ){
              u16 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
            template<> s16 string::as<s16>( ccp pString ){
              s16 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
          }
        }

      //}:                                        |
      //as<u8>:{                                  |

        namespace EON{
          namespace gfc{
            template<> u8 string::as<u8>( ccp pString ){
              u8 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
            template<> s8 string::as<s8>( ccp pString ){
              s8 value = 0;
              if( pString ){
                asDecimal( pString, value );
              }
              return value;
            }
          }
        }

      //}:                                        |
      //as<f64>:{                                 |

        namespace EON{
          namespace gfc{
            template<> f64 string::as<f64>( ccp pString ){
              return as<double>( pString );
            }
          }
        }

      //}:                                        |
      //as<f32>:{                                 |

        namespace EON{
          namespace gfc{
            template<> f32 string::as<f32>( ccp pString ){
              return as<float>( pString );
            }
          }
        }

      //}:                                        |
      //as<bool>:{                                |

        namespace EON{
          namespace gfc{
            template<> bool string::as<bool>( ccp pString ){
              switch( e_hashstr64( pString )){
                case"FALSE"_64:
                case"False"_64:
                case"false"_64:
                case   "NO"_64:
                case   "No"_64:
                case   "no"_64:
                case    "0"_64:
                  return false;
                case "TRUE"_64:
                case "True"_64:
                case "true"_64:
                case  "YES"_64:
                case  "Yes"_64:
                case  "yes"_64:
                case    "1"_64:
                  return true;
                default:
                  return false;
              }
            }
          }
        }

      //}:                                        |
    //}:                                          |
    //find:{                                      |

#ifdef __APPLE__
  #pragma mark - Search methods -
#endif

      ccp string::find( const string& s )const{
        e_syncr();
        e_sanity_check( !e_isbad( s.get().query() ));
        const auto itA = db().find( s.m_uHash );
        if( itA == db().end() ){
          return nullptr;
        }
        const auto itB = db().find( m_uHash );
        if( itB == db().end() ){
          return nullptr;
        }
        ccp pDst = nullptr;
        itA->second->query(
          [&]( ccp pData ){
            pDst = pData;
          }
        );
        if( !pDst ){
          return nullptr;
        }
        ccp pSrc = nullptr;
        itB->second->query(
          [&]( ccp pData ){
            pSrc = pData;
          }
        );
        if( !pSrc ){
          return nullptr;
        }
        return::strstr( pDst, pSrc );
      }

      ccp string::find( ccp pSrc )const{
        e_syncr();
        e_sanity_check( !e_isbad( s.get().query() ));
        const auto it = db().find( m_uHash );
        if( it == db().end() ){
          return nullptr;
        }
        ccp pDst = nullptr;
        it->second->query(
          [&]( ccp pData ){
            pDst = pData;
          }
        );
        if( !pDst ){
          return nullptr;
        }
        return::strstr( pDst, pSrc );
      }

    //}:                                          |
    //chr:{                                       |

      ccp string::chr( const char c )const{
        e_syncr();
        e_sanity_check( !e_isbad( s.get().query() ));
        const auto it = db().find( m_uHash );
        if( it == db().end() ){
          return nullptr;
        }
        ccp pDst = nullptr;
        it->second->query(
          [&]( ccp pData ){
            pDst = pData;
          }
        );
        if( !pDst ){
          return nullptr;
        }
        return::strchr( pDst, c );
      }

    //}:                                          |
    //camelcase:{                                 |

#ifdef __APPLE__
  #pragma mark - Case methods -
#endif

      string string::camelcase()const{
        e_syncr();
        if( empty() ){
          return nullptr;
        }
        stream st;
        ccp r = c_str();
        ccp e = end();
        if( *r >= 'A' && *r <= 'Z' ){
          *st.realloc( 1 )=( *r+++32 );
        }
        while( r < e ){
          cp s = skip_anyws( r );
          while( *s == '_' ){
            ++s;
          }
          const bool b=( s > r );
          if( b &&( *s >= 'a' )&&( *s <= 'z' )){
            *st.realloc( 1 )=( *s-32 );
          }else if( s < e ){
            *st.realloc( 1 )=*s;
          }
          r = s+1;
        }
        string s;
        st.query(
          [&]( ccp pBuffer ){
            s.cat( pBuffer );
          }
        );
        return s;
      }

    //}:                                          |
    //mixedcase:{                                 |

      string string::mixedcase()const{
        e_syncr();
        if( empty() ){
          return nullptr;
        }
        stream st;
        ccp r = c_str();
        ccp e = end();
        if( *r >= 'a' && *r <= 'z' ){
          *st.realloc( 1 )=( *r++-32 );
        }
        while( r < e ){
          cp s = skip_anyws( r );
          while( *s == '_' ){
            ++s;
          }
          const bool b=( s > r );
          if( b &&( *s >= 'a' )&&( *s <= 'z' )){
            *st.realloc( 1 )=( *s-32 );
          }else if( s < e ){
            *st.realloc( 1 )=*s;
          }
          r = s+1;
        }
        string s;
        st.query(
          [&]( ccp pBuffer ){
            s.cat( pBuffer );
          }
        );
        return s;
      }

    //}:                                          |
    //lower:{                                     |

      void string::lower(){
        if( empty() ){
          return;
        }
        e_syncw();
        stream st;
        ccp r = c_str();
        ccp e = end();
        while( r < e ){
          cp s = cp( r );
          if(( *s >= 'A' )&&( *s <= 'Z' )){
            *st.realloc( 1 )=( *s+32 );
          }else{
            *st.realloc( 1 )=*s;
          }
          ++r;
        }
        u64 hash = 0;
        st.query(
          [&]( ccp pBuffer ){
            hash = e_hashstr64( pBuffer );
          }
        );
        clear();
        const auto it = db().find( hash );
        if( it == db().end() ){
          db().insert_or_assign( hash, std::make_unique<stream>( std::move( st )));
        }else{
          it->second->addRef();
        }
        m_uHash = hash;
        #if e_compiling( debug )
          m_pData = c_str();
        #endif
      }

      string string::tolower()const{
        e_syncr();
        if( empty() ){
          return *this;
        }
        string lwr( *this );
        lwr.lower();
        return lwr;
      }

    //}:                                          |
    //upper:{                                     |

      void string::upper(){
        if( empty() ){
          return;
        }
        e_syncw();
        stream st;
        ccp r = c_str();
        ccp e = end();
        while( r < e ){
          cp s = cp( r );
          if(( *s >= 'a' )&&( *s <= 'z' )){
            *st.realloc( 1 )=( *s-32 );
          }else{
            *st.realloc( 1 )=*s;
          }
          ++r;
        }
        u64 hash = 0;
        st.query(
          [&]( ccp pBuffer ){
            hash = e_hashstr64( pBuffer );
          }
        );
        clear();
        const auto it = db().find( hash );
        if( it == db().end() ){
          db().insert_or_assign( hash, std::make_unique<stream>( std::move( st )));
        }else{
          it->second->addRef();
        }
        m_uHash = hash;
        #if e_compiling( debug )
          m_pData = c_str();
        #endif
      }

      string string::toupper()const{
        e_syncr();
        if( empty() ){
          return *this;
        }
        string upr( *this );
        upr.upper();
        return upr;
      }

    //}:                                          |
    //resourceId:{                                |

#ifdef __APPLE__
  #pragma mark - Allocation methods -
#endif

      string string::resourceId(){
        char text[21]{};
      tryAgain:
        for( u32 i=0; i<20; ++i ){
          const u8 select = e_rand<u8>() % 2;
          switch( select ){
            case 0:
              text[ i ] = s8( e_rand<u32>( 'A', 'F' ) & 0xFF );
              break;
            case 1:
              text[ i ] = s8( e_rand<u32>( '0', '9' ) & 0xFF );
              break;
          }
        }
        auto it = db().find( e_hashstr64( text ));
        if( it != db().end() ){
          goto tryAgain;
        }
        string r;
        r.setf( text );
        return r;
      }

    //}:                                          |
    //setv:{                                      |

      string& string::setv( ccp format, va_list va ){
        e_syncw();
        clear();
        catv( format, va );
        return *this;
      }

    //}:                                          |
    //setf:{                                      |

      string& string::setf( ccp format,... ){
        e_syncw();
        e_sanity_check( !e_isbad( format ));
        if( format ){
          va_list va;
          va_start( va, format );
            setv( format, va );
          va_end( va );
        }
        return *this;
      }

    //}:                                          |
    //tryv:{                                      |

      u64 string::tryv( ccp format, va_list va ){
        e_syncw();
        e_sanity_check( !e_isbad( format ));
        u64 bytes = 0;
        if( format ){
          #if e_compiling( microsoft )
            bytes = u64( vsprintf_s( catvBuffer, CATV_SIZE, format, va ));
          #else
            bytes = u64( vsnprintf( catvBuffer, CATV_SIZE, format, va ));
            if( bytes >= CATV_SIZE ){
              e_brk( "Holy crap!" );
            }
          #endif
        }
        return bytes;
      }

    //}:                                          |
    //tryf:{                                      |

      u64 string::tryf( ccp format,... ){
        e_syncw();
        u64 bytes = 0;
        va_list va;
        va_start( va, format );
          bytes = tryv( format, va );
        va_end( va );
        return bytes;
      }

    //}:                                          |
    //getv:{                                      |

      string string::getv( ccp format, va_list va ){
        e_sanity_check( !e_isbad( format ));
        string r;
        r.catv( format, va );
        return r;
      }

    //}:                                          |
    //getf:{                                      |

      string string::getf( ccp format,... ){
        va_list va;
        va_start( va, format );
          e_sanity_check( !e_isbad( format ));
          const string& r = getv( format, va );
        va_end( va );
        return r;
      }

    //}:                                          |
    //catv:{                                      |

      string& string::catv( ccp format, va_list va ){
        e_sanity_check( !e_isbad( format ));
        if( format ){
          #if e_compiling( microsoft )
            const u64 bytes = u64( vsprintf_s( catvBuffer, CATV_SIZE, format, va ));
          #else
            const u64 bytes = u64( vsnprintf( catvBuffer, CATV_SIZE, format, va ));
            if( bytes > CATV_SIZE ){
              e_brk( "Holy crap!" );
            }
          #endif
          cat( catvBuffer, bytes );
        }
        return *this;
      }

    //}:                                          |
    //catf:{                                      |

      string& string::catf( ccp format,... ){
        va_list va;
        va_start( va, format );
          catv( format, va );
        va_end( va );
        return *this;
      }

    //}:                                          |
    //cat:{                                       |

      string& string::cat( ccp a, const u64 n ){
        e_syncw();

        //----------------------------------------------------------------------
        // Validation and sanity checks for the good of ya soul.
        //----------------------------------------------------------------------

        e_sanity_check( !e_isbad( a ));
        if( !a || !*a ){
          return *this;
        }
        if( !n ){
          return *this;
        }

        //----------------------------------------------------------------------
        // Create shared stream pointer.
        //----------------------------------------------------------------------

        stream ou;
        const auto it = db().find( m_uHash );
        if( it != db().end() ){
          ou = *it->second.get();
          const s32 refs = it->second->subRef();
          if( refs < 1 ){
            db().erase( it );
          }else{
            ou = *it->second.get();
          }
        }

        //----------------------------------------------------------------------
        // Allocate string at end.
        //----------------------------------------------------------------------

        cp ptr = ou.realloc( n );//always allocates +1 for exactly this case.
        memcpy( ptr, a, n );
        ptr[ n ]=0;

        //----------------------------------------------------------------------
        // Figure out the hash.
        //----------------------------------------------------------------------

        u64 hash = 0;
        ou.query(
          [&]( ccp pBuffer ){
            hash = e_hashstr64( pBuffer );
          }
        );
        const auto i2nd = db().find( hash );
        if( i2nd == db().end() ){
          ou.setRefs( 1 );
          db()[ hash ] = std::make_unique<stream>();
          db()[ hash ].get()[ 0 ] = std::move( ou );
        }else{
          i2nd->second->addRef();
        }
        m_uHash = hash;

        //----------------------------------------------------------------------
        // Store off the data and stream.
        //----------------------------------------------------------------------

        #if e_compiling( debug )
          m_pData = c_str();
        #endif
        return *this;
      }

      string& string::cat( ccp s, ccp e ){

        //----------------------------------------------------------------------
        // Validation and sanity checks for the good of ya soul.
        //----------------------------------------------------------------------

        if( !s || !*s ){
          return *this;
        }
        e_sanity_check( !e_isbad( s ));
        e_sanity_check( !e_isbad( e ));
        e_assert( s <= e, "Bad ptr!" );

        //----------------------------------------------------------------------
        // Apend input string between s and e inclusive.
        //----------------------------------------------------------------------

        return cat( s, e-s );
      }

      string& string::cat( const string& s ){
        e_syncr();
        if( !s.empty() ){
          cat( s.c_str(), s.len() );
        }
        return *this;
      }

      string& string::cat( ccp pChars ){
        if( pChars && *pChars ){
          cat( pChars, strlen( pChars ));
        }
        return *this;
      }

    //}:                                          |
    //len:{                                       |

      u64 string::len()const{
        e_syncr();
        u64 ln = 0ULL;
        if( m_uHash ){
          const auto it = db().find( m_uHash );
          if( it != db().end() ){
            ln = it->second->size();
          }
        }
        return ln;
      }

    //}:                                          |
    //right:{                                     |

#ifdef __APPLE__
  #pragma mark - Substring methods -
#endif

      string string::right( const u64 i )const{
        e_syncr();
        if( empty() ){
          return nullptr;
        }
        const u64 n = len();
        if( n > i ){
          return string( c_str()+n-i, end() );
        }
        return nullptr;
      }

    //}:                                          |
    //left:{                                      |

      bool string::isleft( ccp pText )const{
        e_syncr();
        if( !empty() ){
          return !strncmp( c_str(), pText, e_min<size_t>( len(), strlen( pText )));
        }
        return false;
      }

      string string::left( const u64 n )const{
        e_syncr();
        if( empty() ){
          return nullptr;
        }
        const u64 l = len();
        if( n > l ){
          return nullptr;
        }
        if( n < l ){
          ccp pBuffer = c_str();
          return string( pBuffer, pBuffer+n );
        }
        return *this;
      }

    //}:                                          |
    //mid:{                                       |

      string string::mid( const u64 left, const u64 right )const{
        e_syncr();
        e_assert( right-left <= len() );
        e_assert( right > left );
        ccp pData = c_str();
        if( pData ){
          return string( pData+left, pData+right );
        }
        return nullptr;
      }

    //}:                                          |
    //ltrim:{                                     |

      string string::ltrimmed( const u64 i )const{
        string r( *this );
        r.ltrim( i );
        return r;
      }

      string& string::ltrim( const u64 i ){

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

        if( empty() || !i ){
          return *this;
        }

        //----------------------------------------------------------------------
        // If trim greater than length just clear and return.
        //----------------------------------------------------------------------

        auto n = len();
        if( i >= n ){
          clear();
          return *this;
        }
        n -= i;

        //----------------------------------------------------------------------
        // Set the left trim amount and debug clear to '.'s.
        //----------------------------------------------------------------------

        stream st;
        cp ptr = st.realloc( n );
        memcpy( ptr, c_str()+i, n );
        clear();
        cat( ptr );
        return *this;
      }

    //}:                                          |
    //trim:{                                      |

      string string::trimmed( const u64 i )const{
        string r( *this );
        r.trim( i );
        return r;
      }

      string& string::trim( const u64 i ){
        e_syncw();
        if( empty() || !i ){
          return *this;
        }
        const u64 l = len();
        if( i >= l ){
          clear();
        }else{
          const u64 n=( l - i );
          stream st;
          cp ptr = st.alloc( n );
          memcpy( ptr, c_str(), n );
          ptr[n] = 0;
          clear();
          cat( ptr );
        }
        return *this;
      }

    //}:                                          |
    //clear:{                                     |

#ifdef __APPLE__
  #pragma mark - Utility methods -
#endif

      void string::clear(){
        e_syncw();
        // Decrement the reference count and destroy.
        const auto it = db().find( m_uHash );
        if( it != db().end() ){
          const s32 refs = it->second->subRef();
          if( refs < 1 ){
            db().erase( it );
          }
        }
        // Nuke the hash key.
        m_uHash = 0;
        // Clear the debug only std::string copy.
        #if e_compiling( debug )
          m_pData = nullptr;
        #endif
      }

    //}:                                          |
    //repeat:{                                    |

      string& string::repeat( const char c, const u64 count ){
        // Bail conditions.
        if( !count || !c ){
          return *this;
        }
        // Create a stream object to replicate 'c' into.
        stream st;
        const u64 n = count * st.stride();
        cp p = st.realloc( count );
        memset( p, c, n );
        p[ n ] = 0;
        // Append stream data.
        cat( p );
        // Return *this.
        return *this;
      }

    //}:                                          |
    //hash:{                                      |

      u64 string::hash()const{
        e_syncr();
        return m_uHash;
      }

    //}:                                          |
    //c_str:{                                     |

      ccp string::c_str()const{
        e_syncr();
        const auto it = db().find( m_uHash );
        if( it == db().end() ){
          return"";
        }
        ccp pResult = "";
        it->second->query(
          [&]( ccp p ){
            pResult = p;
          }
        );
        return pResult;
      }

    //}:                                          |
    //replace:{                                   |

      bool string::replace( const string& find, const string& with ){

        //----------------------------------------------------------------------
        // Bail conditions.
        //----------------------------------------------------------------------

        // Not using e_guardw due to std::move later on that would obliterate
        // the stream reference.
        if( empty() ){
          return false;
        }

        //----------------------------------------------------------------------
        // Find matches.
        //----------------------------------------------------------------------

        const auto L = find.len();
        std::vector<s32> matches;
        matches = kmp( c_str(), s32( len() ), find.c_str(), s32( L ));
        if( matches.empty() ){
          return false;
        }

        //----------------------------------------------------------------------
        // Slow string reconstruction.
        //----------------------------------------------------------------------

        stream st;
        const s32* e = matches.data() + matches.size();
        const s32* p = matches.data();
        ccp r = c_str();
        u64 l = 0;
        s64 n;
        cp  w;
        while( p < e ){
          n = *p - l;
          e_assert( n >= 0 );
          if( n ){
            w = st.realloc( n );
            if( w != r ){
              memcpy( w, r, n );
              w[n]=0;
            }
            r += n;
          }
          l = *p + L;
          r += L;
          n = with.len();
          if( n ){
            w = st.realloc( n );
            if( n > 1 ){
              memcpy( w, with, n );
              w[n]=0;
            }else{
              *w = *with;
            }
          }
          ++p;
        }
        n = strlen( r );
        if( n ){
          w = st.realloc( n );
          memcpy( w, r, n );
          w[n]=0;
        }

        //----------------------------------------------------------------------
        // Assign new stream.
        //----------------------------------------------------------------------

        st.query(
          [&]( ccp pBuffer ){
            set( pBuffer );
          }
        );
        return true;
      }

    //}:                                          |
    //spaces:{                                    |

      ccp string::spaces( const u32 bytes ){
        if( bytes ){
          cp spaces = e_salloc( bytes+1 );
          memset( spaces, 0x20202020, bytes );
          spaces[ bytes ] = 0;
          return spaces;
        }
        return"";
      }

    //}:                                          |
    //ext:{                                       |

      string string::ext()const{
        e_syncr();
        string r;
        ccp s = c_str();
        ccp z = end();
        ccp e = z;
        while( e > s ){
          if( *e == '.' ){
            r = string( e );
            break;
          }
          if( *e == '/' ){
            break;
          }
          --e;
        }
        return r;
      }

    //}:                                          |
    //envs:{                                      |

      string& string::envs(){
        IEngine::envs.foreach(
          [&]( const string_pair& sp ){
            replace( e_xfs( "$(%s)", sp.first.c_str() ), sp.second );
          }
        );
        return *this;
      }

      string string::envs()const{
        return string( *this ).envs();
      }

    //}:                                          |
    //os:{                                        |

      string string::os()const{
        e_syncr();
        e_assert( !find( "\\" ), "Not a Unix path!" );
        #if e_compiling( microsoft )
          if( !empty() ){
            string os;
            switch( *(u16*)c_str() ){
              case 0x2F7E:// ~/
                os = e_getenv( "HOME" ) + "/";
                os.ltrim( 2 );
                break;
              case 0x2F2E:// ./
                os = e_getenv( "PWD"  ) + "/";
                os.ltrim( 2 );
                break;
              default:
                os = *this;
                break;
            }
            os.replace( "/", "\\" );
            if( os[ 1 ] != ':' ){
              os = "C:\\" + os;
            }
            while( os.replace( "\\\\", "\\" ));
            return os;
          }
        #else
          if( !empty() ){
            string os;
            switch( *(u16*)c_str() ){
              case 0x2F7E:// ~/
                os = e_getenv( "HOME" ) + "/" + ltrimmed( 2 );
                break;
              case 0x2F2E:// ./
                os = e_getenv( "PWD"  ) + "/" + ltrimmed( 2 );
                break;
              default:
                os = *this;
                break;
            }
            while( os.replace( "//", "/" ));
            return os;
          }
        #endif
        return *this;
      }

    //}:                                          |
    //path:{                                      |

      string string::path()const{
        e_syncr();
        if( !empty() ){
          ccp s = c_str();
          ccp e = end();
          while( e > s ){
            if( *e == '/' ){
              return string( s, e+1 );
            }
            --e;
          }
        }
        return nullptr;
      }

    //}:                                          |
    //filename:{                                  |

      string string::filename()const{
        e_syncr();
        if( empty() ){
          return nullptr;
        }
        string r;
        ccp s = c_str();
        ccp z = end();
        ccp e = z;
        while( e > s ){
          if( *e == '/' ){
            r = e+1;
            break;
          }
          --e;
        }
        if( r.empty() ){
          r = s;
        }
        return r;
      }

    //}:                                          |
    //basename:{                                  |

      string string::basename()const{
        e_syncr();
        if( empty() ){
          return nullptr;
        }
        ccp e = end();
        ccp start = c_str();
        while( e-- > start ){
          if( *e == '.' ){
            break;
          }
        }
        ccp s = e;
        if( s <= start ){
          s = e = end();
          while( s > start ){
            if( *s == '/' ){
              ++s;
              break;
            }
            --s;
          }
        }else{
          while( s > start ){
            if( *s == '/' ){
              ++s;
              break;
            }
            --s;
          }
        }
        return string( s, e );
      }

    //}:                                          |
    //splitLines:{                                |

      strings string::splitLines()const{
        e_syncr();
        strings vLines;
        if( !empty() ){
          ccp p = c_str();
          ccp e = p+len();
          ccp s = p;
          while( s < e ){
            ccp eol = skip_2eol( s );
            if( !eol ){
              vLines.push( s );
              break;
            }
            if( !is_eol( *s )){
              const string res = string( s, eol );
              vLines.push( res );
            }
            s = skip_eols( eol );
            u64 i = u64( eol > p );
            const u64 n = s-eol;
            while( i++ < n ){
              vLines.push( nullptr );
            }
          }
        }
        return vLines;
      }

    //}:                                          |
    //splitAtCommas:{                             |

      strings string::splitAtCommas()const{
        e_syncr();
        strings v;
        if( !empty() ){
          string tmp( c_str() );
          if( tmp.replace( ",", "\n" )){
            v = tmp.splitLines();
            auto it = v.getIterator();
            while( it ){
              string& s = *it;
              if( s.empty() ){
                it.erase();
                continue;
              }
              auto* b = s.c_str();
              auto* e = b;
              while( *e && ( *e <= ' ' )){
                ++e;
              }
              s.ltrim( e-b );
              ++it;
            }
          }else{
            v += tmp;
          }
        }
        return v;
      }

    //}:                                          |
    //scrubf:{                                    |

      string& string::scrubf(){
        e_syncw();
        if( empty() ){
          return *this;
        }
        const auto it = db().find( m_uHash );
        if( it == db().end() ){
          return *this;
        }
        stream ou;
        ccp    s = c_str();//because of left trimming.
        ccp    e = end();
        ccp    p = s;
        while( p < e ){
          if(( *p == '$' )&&( p[1] == '(' )){
            while( ++p < e ){
              if( *p == ')' ){
                ++p;
                break;
              }
            }
          }else if( !strncmp( p, "\033[", 2 )){
            while( ++p < e ){
              if( *p == 'm' ){
                ++p;
                break;
              }
            }
          }else{
            *ou.realloc( 1 ) = *p++;
          }
        }
        const s32 refs = it->second->subRef();
        e_assert( refs > -1 );
        if( !refs ){
          db().erase( it );
        }
        ou.addRef();
        ou.query(
          [&]( ccp pBuffer ){
            m_uHash = e_hashstr64( pBuffer );
          }
        );
        db().insert_or_assign( m_uHash, std::make_unique<stream>( std::move( ou )));
        #if e_compiling( debug )
          m_pData = c_str();
        #endif
        return *this;
      }

      string string::scrubf()const{
        string out( *this );
        return out.scrubf();
      }

    //}:                                          |
  //}:                                            |
  //Ctor:{                                        |

#ifdef __APPLE__
  #pragma mark - Ctors -
#endif

    string::string( const std::string& lvalue ){
      cat( lvalue.c_str() );
    }

    string::string( string&& rvalue ){
      if(( this != &rvalue ) && rvalue.m_uHash ){
        m_uHash = rvalue.m_uHash;
        rvalue.m_uHash = 0ULL;
        #if e_compiling( debug )
          m_pData = rvalue.m_pData;
          rvalue.m_pData = nullptr;
        #endif
      }
    }

    string::string( const string& lvalue ){
      if(( this != &lvalue ) && lvalue.m_uHash ){
        cat( lvalue );
      }
    }

    string::string( ccp a, ccp b ){
      e_sanity_check( !e_isbad( a ));
      e_sanity_check( !e_isbad( b ));
      e_assert( a <= b );
      if( !a || !*a || !b ){
        return;
      }
      cat( a, b );
    }

    string::string( ccp a ){
      e_sanity_check( !e_isbad( a ));
      if( !a || !*a ){
        return;
      }
      cat( a );
    }

  //}:                                            |
  //Dtor:{                                        |

    string::~string(){
      clear();
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
