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

#pragma once

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup strings
  *     @{
  */

//================================================|=============================
//string:{                                        |

  namespace EON{

    namespace gfc{

      /** Generic string class
        *
        * For basic string operations use the string object. It will certainly
        * grow over time. This class breaks our naming convention. All classes
        * must begin with an uppercase letter. gfc::string is obviously bad
        * and/or very naughty. If the case insensitive bit is not set in the
        * flags then the hash of the string will be computed every time cat()
        * is called.
        */

      struct E_PUBLISH string{

        constexpr static const u64 CATV_SIZE = 1048576;

        //----------------------------------------|-----------------------------
        //Aliases:{                               |

          template<u32 N> using pairs = array<std::pair<string,string>,N>;

        //}:                                      |
        //Operate:{                               |

          /** \name Conversion operators.
            *
            * These operators allow you to convert the string to a number of
            * types just by casting it or assigning it to an object of that
            * type.
            *
            * @{
            */

          /** \brief Deference string like any normal C string.
            *
            * This operator will return the first character at the head of the
            * stream. This is not the same as dereferencing the return value
            * from the stream's data method either. The c_str() method returns
            * "" if the stream is empty, which of course stream will not.
            *
            * \return Returns the first character in the string or zero if the
            * string is empty.
            */

          e_forceinline char operator*()const{
            return *c_str();
          }

          /** String to C string operator.
            *
            * Converts a std type string to a C string.
            *
            * \returns A C string.
            */

          e_forceinline operator ccp()const{
            return c_str();
          }

          /** @}
            *
            * \name Assignment operators.
            *
            * These operators provide means for assigning a variety of
            * different objects to the string.
            *
            * @{
            */

          /** \brief Assignment operator.
            *
            * This operator will assign a string object to this one.
            *
            * \param lvalue The string object to assign to this string.
            *
            * \return A reference to *this.
            */

          string& operator=( const string& lvalue );

          /** \brief Move operator.
            *
            * This operator will assign a string object to this one. The
            * operator will do a move operation for extra speed so statements
            * like a+"b" are lighting fast and need no allocations.
            *
            * \param rvalue The string object to assign to this string.
            *
            * \return A reference to *this.
            */

          string& operator=( string&& rvalue );

          /** \brief Assignment operator.
            *
            * This operator will assign a string object to this one.
            *
            * \param pValue The C string to assign to this string.
            *
            * \return A reference to *this.
            */

          string& operator=( ccp pValue );

          /** @}
            *
            * \name Streaming concatenation operators.
            *
            * These operators will append various types to the end of the
            * string.
            *
            * @{
            */

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const string& value ){
            cat( value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const double value ){
            catf( "%lf", value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const u64 value ){
            catf( "%llu", value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const s64 value ){
            catf( "%lld", value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const float value ){
            catf( "%f", value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const u32 value ){
            catf( "%u", value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( const s32 value ){
            catf( "%d", value );
            return *this;
          }

          /** \brief Streaming in operator.
            *
            * This operator will append the given data to the end of this
            * string.
            */

          e_forceinline string& operator<<( ccp value ){
            cat( value );
            return *this;
          }

          /** @}
            *
            * \name Concatenation (appending) operators.
            *
            * These operators will append a variety of values to the end of
            * this string object. They do this by converting the value to a
            * string and then calling cat (or catf) on it.
            *
            * @{
            */

          /** \brief Add operator.
            *
            * This operator will add a string object to this string as if you
            * had called this->cat( sValue ).
            *
            * \param value The string object to append to this string.
            *
            * \return A reference to the string buffer.
            */

          e_forceinline string& operator+=( const string& value ){
            cat( value );
            return *this;
          }

          /** \brief Add operator.
            *
            * This operator will concatenate a single character onto the end of
            * the string data in "m".
            *
            * \param chr The character to append.
            *
            * \return Returns the deferenced this pointer.
            */

          e_forceinline string& operator+=( const char chr ){
            const char as[2]{ chr, 0 };
            cat( as, 1 );
            return *this;
          }

          /** \brief Add operator.
            *
            * This routine will add a string object to this string as if you
            * had called this->cat( sValue ).
            *
            * \param pValue The C string to append to this string.
            *
            * \return A reference to the string buffer.
            */

          e_forceinline string& operator+=( ccp pValue ){
            cat( pValue );
            return *this;
          }

          /** \brief Plus operator.
            *
            * This routine will create a string from this string and s appended
            * to it. This string is not modified.
            *
            * \param s The string object to append.
            *
            * \return A new string which is the concatenation of this and s.
            */

          string operator+( const string& s )const;

          /** \brief Plus operator.
            *
            * This routine will create a string from this string and s appended
            * to it. This string is not modified.
            *
            * \param p The C string to append.
            *
            * \return A new string which is the concatenation of this and s.
            */

          string operator+( ccp p )const;

          /** @}
            *
            * \name Comparison operators.
            *
            * These operators allow us to compare two strings and see if they
            * are the same, less than or greater than one another.
            *
            * @{
            */

          /** \brief Less than operator.
            *
            * This operator compares this string against the string provided
            * using strcmp(). The case insensitivity flag is ignored by this
            * operator.
            *
            * \param s The string object to compare with.
            *
            * \return If strcmp() returns a value that's less than zero then
            * the return value is true otherwise it'll be false.
            */

          bool operator<( const string& s )const;

          /** \brief Less than operator.
            *
            * This operator compares this string against the string provided
            * using strcmp(). The case insensitivity flag is ignored by this
            * operator.
            *
            * \param p The C string to compare with.
            *
            * \return If strcmp() returns a value that's less than zero then
            * the return value is true otherwise it'll be false.
            */

          bool operator<( ccp p )const;

          /** \brief Greater than operator.
            *
            * This operator compares this string against the string provided
            * using strcmp(). The case insensitivity flag is ignored by this
            * operator.
            *
            * \param s The string object to compare with.
            *
            * \return If strcmp() returns a value that's greater than zero then
            * the return value is true otherwise it'll be false.
            */

          bool operator>( const string& s )const;

          /** \brief Greater than operator.
            *
            * This operator compares this string against the string provided
            * using strcmp(). The case insensitivity flag is ignored by this
            * operator.
            *
            * \param p The C string to compare with.
            *
            * \return If strcmp() returns a value that's greater than zero then
            * the return value is true otherwise it'll be false.
            */

          bool operator>( ccp p )const;

          /** \brief Inequality operator.
            *
            * This operator compares this string against the one provided. The
            * case sensitivity flag is used in this code.
            *
            * \param s The string object to compare against this string.
            *
            * \return Returns true if the two strings are identical if the case
            * insensitivity flag is false or if the two strings match in a case
            * insensitive way. Returns false if both of these checks fail.
            */

          bool operator!=( const string& s )const;

          /** \brief Inequality operator.
            *
            * This operator compares this string against the one provided. The
            * case sensitivity flag is used in this code.
            *
            * \param p The C string to compare against this string.
            *
            * \return Returns true if the two strings are identical if the case
            * insensitivity flag is false or if the two strings match in a case
            * insensitive way. Returns false if both of these checks fail.
            */

          bool operator!=( ccp p )const;

          /** Equality operator.
            *
            * Test the equality of this string object with the input c string.
            *
            * \param s Input string to test.
            *
            * \return Returns true if two string are equal and false otherwise.
            */

          bool operator==( const string& s )const;

          /** Equality operator.
            *
            * Test the equality of this string object with the input c string.
            *
            * \param p Input string to test.
            *
            * \return Returns true if two string are equal and false otherwise.
            */

          bool operator==( ccp p )const;

          /** Equality operator.
            *
            * Test the equality of this string object with the input c string.
            *
            * \param h Input hashed string to test.
            *
            * \return Returns true if two string are equal and false otherwise.
            */

          bool operator==( const u64 h )const{
            return( hash() == h );
          }

          /** @} */

        //}:                                      |
        //Methods:{                               |
          //resourceId:{                          |

            /** \brief Generate a unique resource ID.
              *
              * This routine will scan through all resources checking for an id
              * as it builds. The previous string will be obliterated.
              */

            static string resourceId();
            static string guid();

          //}:                                    |
          //scrubf:{                              |

            string  scrubf()const;
            string& scrubf();

          //}:                                    |
          //clone:{                               |

            e_forceinline string clone()const{
              return string( c_str() );
            }

          //}:                                    |
          //skip*:{                               |

            static cp skip_anynonws( ccp s );
            static cp skip_nonws( ccp s );
            static cp skip_anyws( ccp s );
            static cp skip_2eol( ccp s );
            static cp skip_eols( ccp s );
            static cp skip_ws( ccp s );
            static cp skip_number( ccp s );
            static cp skip_cc( ccp s );
            static cp skip_str( ccp s );

          //}:                                    |
          //is*:{                                 |

            static bool is_upper( const char c );
            static bool is_lower( const char c );
            static bool is_eol  ( const char c );

            /** \brief Test if input is an alpha character.
              *
              * This parsing helper routine will test if the input character is
              * an alpha character based on the ASCII table. That's any upper
              * or lower case letter in the range A-Z or a-z.
              *
              * \param c The character to test to see if it's an alphabetic
              * character.
              *
              * \return Returns true if 'c' is an alpha character or false.
              */

            static bool is_alpha( const char c );

            /** \brief Test if C comment.
              *
              * This parsing helper routine will test the given string to see
              * if it is a C11 style line or block comment. The input string
              * may have leading whitespace in it because isCC will
              * automatically skip this.
              *
              * \param s String to be tested as a C11 style comment.
              *
              * \return Returns true if the string started with a C style
              * comment and false otherwise.
              */

            static bool is_cc( ccp s );

            /** \brief Test if input string starts with an alpha character.
              *
              * This parsing helper routine will test if the input string has
              * an alpha character based on the ASCII table. That's any upper
              * or lower case letter in the range A-Z or a-z.
              *
              * \param s The string to test.
              *
              * \return Returns true if 's' has an alpha character or false.
              */

            static bool is_alpha( ccp s );

            /** \brief Test if input string is uppercase or not.
              *
              * This routine will return true if the given string is uppercase
              * or not.
              *
              * \return Returns true if the first non-whitespace character is
              * upper case.
              */

            static bool is_upper( ccp s );

            /** \brief Test if input string is lowercase or not.
              *
              * This routine will return true if the given string is lowercase
              * or not.
              *
              * \return Returns true if the first non-whitespace character is
              * lower case.
              */

            static bool is_lower( ccp s );

            /** \brief Test if character is an identifier character.
              *
              * This parsing helper routine tests to see if the given character
              * is a valid C identifier character. It assumes that it is not
              * the first character so digits are allowed.
              *
              * \param c An ASCII character to test if it's a valid identifier
              * character if not the first one.
              *
              * \param bFirst The character is the first one.
              *
              * \return Returns true if the character falls within the set _
              * a-z 0-9 A-Z. If it doesn't then we return false.
              */

            static bool is_id( const char c, const bool bFirst );

            /** \brief Test if string is valid c identifier.
              *
              * This parsing helper routine will test if the input string is a
              * valid C identifier.
              *
              * \param s A C string to test whether it's a valid C identifier.
              * A good ID is one that starts with either an underscore or an
              * alpha character and continues after with the same or a numeric
              * digit in the range 0-9.
              *
              * \return Returns true if the C string is a good C identifier or
              * false.
              */

            static bool is_id( ccp s );

            /** \brief Test if string is a numeric digit.
              *
              * This parsing helper routine will test to see if the input
              * character is a numeric digit or not.
              *
              * \param c The ASCII character to test.
              *
              * \return Returns true if the input character is a digit or false
              * otherwise.
              */

            static bool is_digit( const char c );

            /** \brief Test if string is a real number or integer.
              *
              * This parsing helper routine will test to see if the input
              * string is a valid real or integer number.
              *
              * \param s The string to test.
              *
              * \return Returns true if the string is a valid number or false.
              */

            static bool is_number( ccp s );

            /** \brief Check if string is a hex value.
              *
              * This routine checks to see if the string is hex and returns the
              * result.
              */

            bool is_hex()const;

            /** \brief Check if string is a sha1 key.
              *
              * This routine checks to see if the string is a good sha1 key
              * candidate and returns true if it is, false if it's not.
              */

            e_forceinline bool is_sha1()const{
              if( len() != 40 ){
                return false;
              }
              if( !is_hex() ){
                return false;
              }
              return true;
            }

          //}:                                    |
          //as:{                                  |

            /** \brief Convert string to given type.
              *
              * This routine will convert the input string to the type T in the
              * template. This is extremely useful for parsers, the XML
              * serializers use it for example. If the template type T is a
              * float or double then the exprtk parser will be used to convert
              * a complex expression into a numeric value which then gets
              * converted to a real value we return from the as method. If T is
              * a u32 or u64 it will first quickly parse the string to see if
              * it begins with $, 0x or 0b. If it does the code will convert
              * the hexadecimal or binary number to an integer and return the
              * value. If the string is a straight up decimal number and T is a
              * [u|s]32 or [u|s]64 then it will be converted just like you
              * called atoi but unsigned integers are handled appropriately. If
              * the same T type is provided a string with a complex expression
              * in it and doesn't start with one of the aforementioned integer
              * prefixes then the string will be sent to the expression toolkit
              * just like it was a real number. You should be able to use any
              * base engine type not including classes but everything within
              * the math<> template and things like u32 and s64 etc.
              *
              * \param pString A textual representation of an engine type.
              *
              * \return Returns a value of typename T parsed from pString.
              */

            template<typename T>static T as( ccp pString );

            /** \brief Convert string to type.
              *
              * This routine will calls the static as method to convert c_str()
              * to a type that's returned from this method.
              *
              * \return Returns a value of typename T parsed from pString.
              */

            template<typename T> e_forceinline T as()const{
              e_sanity_check( !e_isbad( c_str() ));
              return as<T>( c_str() );
            }

          //}:                                        |
          //replace:{                             |

            /** \brief Search and replace method.
              *
              * This routine will search for the string in 'find' and replace
              * it with the string in 'with'.
              *
              * \param find The substring to look for.
              * \param with The replacement string.
              *
              * \return Returns true if one or more replacements were made and
              * false otherwise.
              */

            bool replace( const string& find, const string& with );

          //}:                                    |
          //spaces:{                              |

            static ccp spaces( u32 bytes );

          //}:                                    |
          //repeated:{                            |

            e_forceinline static string repeated( const char c, const u64 count ){
              string r;
              r.repeat( c, count );
              return r;
            }

          //}:                                    |
          //repeat:{                              |

            string& repeat( const char c, const u64 count );

          //}:                                    |
          //setv:{                                |

            /** \brief Set the string to the formatted text provided.
              *
              * This routine will blow away the current contents of the string
              * and replace it with the formatted text provied.
              *
              * \param format The formatted string to replace this with.
              *
              * \param arg Arguments a la vsprintf.
              *
              * \return Returns *this.
              */

            string& setv( ccp format, va_list arg );

          //}:                                    |
          //setf:{                                |

            /** \brief Set the string to the formatted text provided.
              *
              * This routine will blow away the current contents of the string
              * and replace it with the formatted text provied.
              *
              * \param format The formatted string to replace this with.
              *
              * \return Returns *this.
              */

            string& setf( ccp format,... );

          //}:                                    |
          //set:{                                 |

            /** \brief Set the string to the text provided.
              *
              * This routine will blow away the current contents of the string and
              * replace it with the text provied.
              *
              * \param str The string to replace this with.
              *
              * \return Returns *this.
              */

            e_forceinline string& set( ccp str ){
              setf( str );
              return *this;
            }

          //}:                                    |
          //tryv:{                                |

            /** \brief Try building formatted text.
              *
              * This routine will calculate the amount of space requird to
              * store the formatted text provided. The operation takes place in
              * a thread safe 32K buffer which all tryv, setv and catv methods
              * use. The return value can be passed directly into the stream
              * constructor if you choose, which is why the method exists.
              *
              * \param format The formatted text to try.
              *
              * \param arg The arguments.
              *
              * \returns The number of characters that will be allocated.
              */

            static u64 tryv( ccp format, va_list arg );

          //}:                                    |
          //tryf:{                                |

            /** \brief Try building formatted text.
              *
              * This routine will calculate the amount of space requird to
              * store the formatted text provided. The operation takes place in
              * a thread safe 32K buffer which all tryv, setv and catv methods
              * use. The return value can be passed directly into the stream
              * constructor if you choose, which is why the method exists.
              *
              * \param format The formatted text.
              *
              * \param ... The arguments.
              *
              * \returns The number of characters that will be allocated.
              */

            static u64 tryf( ccp format,... );

          //}:                                    |
          //getv:{                                |

            /** \brief Construct a string from formatted text.
              *
              * The constructor serves a similar purpose to the xfs class but
              * allocates in system memory rather than scratch memory.
              *
              * \param format The formatted text as per printf standard.
              * \param va Arguments to pass into formatted text.
              */

            static string getv( ccp format, va_list va );

          //}:                                    |
          //getf:{                                |

            /** \brief Construct a string from formatted text.
              *
              * The constructor serves a similar purpose to the xfs class but
              * allocates in system memory rather than scratch memory.
              *
              * \param format The formatted text as per printf standard.
              *
              * \param ... Arguments to pass into formatted text.
              */

            static string getf( ccp format,... );

          //}:                                    |
          //catv:{                                |

            /** \brief Concatenate formatted text to string.
              *
              * This routine tacks a piece of formatted text to the end of the
              * string object data. It may be empty initially.
              *
              * \param format The formatted string ala printf.
              *
              * \param arg The data arguments.
              *
              * \todo Make this a C++17 variadic template.
              *
              * \returns A '*this' reference.
              */

            string& catv( ccp format, va_list arg );

          //}:                                    |
          //catf:{                                |

            /** \brief Concatenate formatted text to string.
              *
              * This routine tacks a piece of formatted text to the end of the
              * string object data. It may be empty initially.
              \code
              void foo(){
                string s;
                s.catf( "Hello %s", "world" );
              }
              \endcode
              * \returns A '*this' reference.
              */

            string& catf( ccp format, ... );

          //}:                                    |
          //cat:{                                 |

            /** \brief Concatenate text to string.
              *
              * This routine tacks a piece of unformatted text to the end of
              * the string object data. It may be empty initially. The cat
              * functions is the workhorse of concatenation and is called by
              * catf.
              *
              * \param chars The C-string to concatenate.
              \code
                void foo(){
                  string s;
                  s.cat( "Hello world" );
                }
              \endcode
              * \return A string reference to this.
              */

            string& cat( ccp chars );

          //}:                                    |
          //cat:{                                 |

          /** Concatenate text to string.
            *
            * This routine will "cat" a C-string onto the end of this one.
            *
            * \param p The C-string to concatenate.
            *
            * \param n The length of the C string in characters.
            *
            * \return Returns *this.
            */

          string& cat( ccp p, const u64 n );

          /** Concatenate text to string.
            *
            * This routine will "cat" a string onto the end of this one.
            *
            * \param s The string to concatenate.
            *
            * \return Returns *this.
            */

          string& cat( const string& s );

          /** Concatenate text to string.
            *
            * This routine will "cat" a string onto the end of this one.
            *
            * \param a The start of the string to concatenate.
            *
            * \param b The end of the sequence of characters.
            *
            * \return Returns *this.
            */

          string& cat( ccp a, ccp b );

          //}:                                    |
          //hash:{                                |

            /** \brief Get hash of string.
              *
              * This routine will hash the string and return a 64-bit unsigned
              * integer for it. It simply returns the value of m_uHash because
              * that member variable is updated every time you call cat(),
              * which is the primary interface for allocation in the class.
              *
              * \return A 64-bit unsigned hash for the current string.
              */

            u64 hash()const;

            /** \brief Compare hashes.
              *
              * This routine will compute and compare this hash with the
              * incoming 64-bit hash value.
              */

            e_forceinline bool notHash( const u64 hash )const{
              return( m_uHash != hash );
            }

            /** \brief Compare hashes.
              *
              * This routine will compute and compare this hash with the
              * incoming 64-bit hash value.
              */

            e_forceinline bool isHash( const u64 hash )const{
              return( m_uHash == hash );
            }

          //}:                                    |
          //toupper:{                             |

            string toupper()const;

          //}:                                    |
          //upper:{                               |

            void upper();

          //}:                                    |
          //tolower:{                             |

            string tolower()const;

          //}:                                    |
          //lower:{                               |

            void lower();

          //}:                                    |
          //len:{                                 |

            /** String to length method.
              *
              * Converts a stdtype string to a string length.
              *
              * \returns Length of stdtype string.
              */

            u64 len()const;

          //}:                                    |
          //c_str:{                               |

            /** String to C string method.
              *
              * Converts a stdtype string to a C string.
              *
              * \returns A C string.
              */

            ccp c_str()const;

          //}:                                    |
          //empty:{                               |

            /** \brief Is the string empty?
              *
              * This routine checks if this is an empty string, and if it is
              * returns the appropriate boolean result.
              *
              * \return Returns true if the string is unallocated or empty, and
              * false otherwise. A false value means the string is allocated
              * and potentially shared.
              */

            e_forceinline bool empty()const{
              return( m_uHash==0 );
            }

          //}:                                    |
          //clear:{                               |

            /** \brief Clear the string to empty.
              *
              * This routine nukes the string. If the string reference count
              * bottoms out then we'll destroy the string.
              */

            void clear();

          //}:                                    |
          //chr:{                                 |

            ccp chr( const char c )const;

          //}:                                    |
          //find:{                                |

            /** \brief Find sub-string.
              *
              * This routine will search for the given string as a sub-string
              * of this.
              *
              * \param s The sub-string to search for.
              *
              * \return Returns a C string pointer to the given sub-string.
              */

            ccp find( const string& s )const;

            /** \brief Find sub-string.
              *
              * This routine will search for the given string as a sub-string
              * of this.
              *
              * \param pSub The sub-string to search for.
              *
              * \return Returns a C string pointer to the given sub-string.
              */

            ccp find( ccp pSub )const;

          //}:                                    |
          //back:{                                |

            e_forceinline char back()const{
              if( !empty() ){
                return end()[-1];
              }
              return 0;
            }

          //}:                                    |
          //end:{                                 |

            e_forceinline ccp end()const{
              return( c_str() + len() );
            }

          //}:                                    |
          //ltrim:{                               |

            e_forceinline string& e_ltrimstr( string& s, const u64 n ){
              return s.ltrim( n );
            }

            string  ltrimmed( const u64 n )const;
            string& ltrim(    const u64 n );

          //}:                                    |
          //trim:{                                |

            e_forceinline string& e_trimstr( string& s, const u64 n ){
              return s.trim( n );
            }

            string trimmed( const u64 n )const;
            string& trim( const u64 n );

          //}:                                    |
          //mixedcase:{                           |

            e_forceinline friend string e_mixedcase( const string& s ){
              return s.mixedcase();
            }

            /** \brief Convert string to mixed case string.
              *
              * This routine will scan through the string converting it to
              * mixed case. If the first non-whitespace character is a
              * lowercase character it will be converted to A-Z. All other
              * characters will be treated as lowercase unless they are already
              * uppercase or two characters are separated by whitespace or
              * underscores(s). The latter will cause the next non-whitespace
              * or underscore character to be converted from a-z to A-Z.
              *
              * \return Returns a string that's been converted to mixed case.
              * This is especially useful for converting filenames to
              * identifiers in the Swordlight editor.
              */

            string mixedcase()const;

          //}:                                        |
          //camelcase:{                           |

            e_forceinline friend string e_camelcase( const string& s ){
              return s.camelcase();
            }

            string camelcase()const;

          //}:                                    |
          //right:{                               |

            e_forceinline friend string e_rightstr( const string& s, const u64 i ){
              return s.right( i );
            }

            string right( const u64 i )const;

          //}:                                    |
          //left:{                                |

            e_forceinline friend string e_leftstr( const string& s, const u64 n ){
              return s.left( n );
            }

            string left( const u64 n )const;
            bool isleft( ccp pText )const;

          //}:                                    |
          //mid:{                                 |

            e_forceinline friend string e_midstr( const string& s, const u64 left, const u64 right ){
              return s.mid( left, right );
            }

            string mid( const u64 left, const u64 right )const;

          //}:                                    |
          //ext:{                                 |

            /** \brief Get the filename extension.
              *
              * This routine returns a string representing a dot (.) and the
              * filename extension.
              *
              * \return Returns something like ".png" or ".fbx".
              */

            string ext()const;

          //}:                                    |
          //name:{                                |

            /** \brief Get the filename (basename+ext).
              *
              * This routine will scan backwards from the end of the string
              * looking for the first / (*nix) or \\ character in the string.
              * It will return a new string representing the filename.
              */

            string filename()const;

            /** \brief Get the base name.
              *
              * This routine will treat the string as a filename and strip off
              * the extension and path parts.
              */

            string basename()const;

          //}:                                    |
          //os:{                                  |

            /** \brief Convert string to os path.
              *
              * This routine will treat the string as a Unix style path and
              * convert all \ or / as appropriate for the platform.
              *
              * \return Returns a new string representing the converted path.
              */

            string os()const;

          //}:                                    |
          //envs:{                                |

            /** \brief Expand environment variables.
              *
              * This routine is pretty slow. It will run through all
              * environment variables stored in IEngine::envs and expand $()
              * style sub-strings with their matching values.
              */

            string envs()const;

            /** \brief Expand environment variables.
              *
              * This routine is pretty slow. It will run through all
              * environment variables stored in IEngine::envs and expand $()
              * style sub-strings with their matching values.
              */

            string& envs();

          //}:                                    |
          //path:{                                |

            /** \brief Get the path.
              *
              * This routine will treat the string as a filename and strip off
              * the file part plus extension if it has one.
              *
              * \return Returns the path part of the string if it's a filename.
              */

            string path()const;

          //}:                                    |
          //splitAtCommas:{                       |

            /** \brief Separate string into comma delineated lines.
              *
              * This routine will separate the string out into lines and stores
              * them in the returned buffer provided.
              */

            vector<string> splitAtCommas()const;

          //}:                                    |
          //splitLines:{                          |

            /** \brief Separate string into lines.
              *
              * This routine will separate the string out into lines and stores
              * them in the returned buffer provided.
              */

            vector<string> splitLines()const;

          //}:                                    |
          //setHashCode:{                         |

            /** \brief Set the hash code.
              *
              * This is a pretty dangerous API actually. It changes the hash on
              * the string which will instantly point it to a different entry
              * in the string table internally. The accessor therefore only
              * exists to support the string code itself and shouldn't be used
              * by the user unless absolutely necessary and you know what
              * you're doing.
              *
              * \param hash The new hash code. Will instantly change the string
              * table entry without updating the reference count. Danger!
              */

            void setHashCode( const u64 hash ){
              m_uHash = hash;
            }

          //}:                                    |
        //}:                                      |
        //----------------------------------------|-----------------------------

        /** \brief Standard string constructor.
          *
          * This routine will construct a string from a standard string.
          */

        string( const std::string& s );

        /** \brief String's copy constructor.
          *
          * This will construct a string as a duplicate of another string. It
          * will initialize the string to zero then "cat" the string; that way
          * we get a properly allocated copy and don't need to worry about
          * memory leaks.
          */

        string( const string& s );

        /** \brief Move constructor.
          *
          * This constructor will move the old string value into this one
          * without doing any copying whatsoever. It's a great optimization
          * that's part of C++11.
          */

        string( string&& rvalue );

        /** \brief Two string constructor.
          *
          * This will construct a string from two strings: the start and end.
          *
          * \param a The start of the new string.
          * \param b The terminator.
          */

        string( ccp a, ccp b );

        /** \brief Single string constructor.
          *
          * This routine will construct a string out of a C string.
          */

        string( ccp c );

        /** \brief String's default constructor.
          *
          * This will initialize the string to empty: no data and zero size.
          */

        string() = default;

        /** \brief String destructor.
          *
          * This routine does nothing more than call string::clear.
          */

        virtual~string();

      private:

        #if e_compiling( debug )
          mutable ccp m_pData = nullptr;
        #endif
        mutable u64 m_uHash = 0;
      };

      /* Aliases */

      using string_pair  = std::pair<string,string>;
      using string_pairs = vector<string_pair>;
      using strings      = vector<string>;

      /* Macros */

      #define e_xfs(...)::EON::gfc::string::getf(__VA_ARGS__)
    }
  }

//}:                                              |
//================================================|=============================
//                                                :
//                                                :
//                                                :
//================================================|=============================
//e_*:{                                           |
  //e_stricmp:{                                   |

    #if e_compiling( osx )                                                      \
     || e_compiling( ios )                                                      \
     || e_compiling( android )                                                  \
     || e_compiling( linux )                                                    \
     || e_compiling( web )
      #define e_stricmp strcasecmp
    #elif e_compiling( microsoft )
      #define e_stricmp _stricmp
    #else
      #define e_stricmp stricmp
    #endif

  //}:                                            |
//}:                                              |
//================================================|=============================

/**     @}
  *   @}
  * @}
  */
