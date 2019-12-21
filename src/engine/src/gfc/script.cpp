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

#include<iterator>
#include<regex>

using namespace EON;
using namespace gfc;

//================================================|=============================
//Script:{                                        |
  //Extends:{                                     |

    e_extends( Script );

  //}:                                            |
  //Structs:{                                     |
    //Grammar:{                                   |
      //Extends:{                                 |

        e_extends( Script::Grammar );

      //}:                                        |
      //Methods:{                                 |
        //set:{                                   |

          void Script::Grammar::set( const string& in_source ){

            //------------------------------------------------------------------
            // Build rule database.
            //------------------------------------------------------------------

            m_mRegex.clear();
            ccp pChr = in_source.c_str();
            ccp pEnd = in_source.end();
            ccp p1st = nullptr;
            ccp p2nd = nullptr;
            while( pChr < pEnd ){
              p1st = string::skip_anyws( pChr );
              p2nd = string::skip_nonws( p1st );
              if( string( p1st, p2nd )=="grammar" ){
                p1st = string::skip_anyws( p2nd );
                p2nd = string::skip_nonws( p1st );
                const  string label( p1st, p2nd );
                p1st = string::skip_anyws( p2nd );
                p2nd = strchr( p1st, '{' );
                if( p2nd ){
                  ++p2nd;
                }else{
                  e_errorf( 20005, "Missing {" );
                }
                while( *p1st != '}' ){
                  p1st = string::skip_anyws(    p2nd );
                  p2nd = string::skip_anynonws( p1st );
                  const  string ruleName( p1st, p2nd );
                  p1st = string::skip_anyws(    p2nd );
                  if( *p1st != '{' ){
                    e_errorf( 20001, "Missing {" );
                    return;
                  }
                  p2nd = p1st + 1;
                  p1st = string::skip_anyws( p2nd );
                  if( *p1st != '}' ){
                    p2nd = string::skip_2eol(   p1st );
                    const string pattern( p1st, p2nd );
                    p1st = string::skip_anyws(  p2nd );
                    if( *p1st != '}' ){
                      e_errorf( 20003, "Missing }" );
                      return;
                    }
                    Rule rule;
                    rule.setName( ruleName );
                    rule.setPattern( pattern );
                    rule.setRegex( pattern );
                    rule.setToken( m_mRegex.size() );
                    m_mRegex.set( e_hashstr64( ruleName.c_str() ), rule );
                    pChr = p2nd = ++p1st;
                  }
                }
                if( *pChr != '}' ){
                  e_errorf( 20003, "Missing }" );
                  return;
                }
                pChr = string::skip_anyws( pChr + 1 );
              }
            }

            //------------------------------------------------------------------
            // Expand rule regexes.
            //------------------------------------------------------------------

            bool bFlipped = true;
            while( bFlipped ){
              bFlipped = false;
              m_mRegex.foreach(
                [&]( Rule& rule ){
                  m_mRegex.foreach(
                    [&]( const Rule& cmpRule ){
                      if( &rule == &cmpRule ){
                        return;
                      }
                      const auto& token = "@"+cmpRule.toName()+"@";
                      if( !strstr( rule.toRegex().c_str(), token.c_str() )){
                        // Call replace as infrequently as possible.
                        return;
                      }
                      const auto& regex = cmpRule.toRegex( );
                      rule.toRegex().replace( token, regex );
                      bFlipped = true;
                    }
                  );
                }
              );
            }
          }

        //}:                                      |
      //}:                                        |
    //}:                                          |
  //}:                                            |
  //Methods:{                                     |
    //parse:{                                     |

      void Script::parse( const string& rulename, const string& text ){
        if( m_hGrammar ){
          // TODO: Update the input text via parsing then send every line.
        }
        strings lines = text.splitLines();
        lines.foreach(
          [&]( const string& line ){
            if( line.empty() ){
              return;
            }
            onMatch( line );
          }
        );
      }

    //}:                                          |
  //}:                                            |
//}:                                              |
//================================================|=============================
