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

//================================================|=============================
//Capsule:{                                       |

  /** \brief Capsule class.
    *
    * This class defines the geometric capsulte.
    */

  struct Capsule{

    //--------------------------------------------|-----------------------------
    //Methods:{                                   |
      //toRadius:{                                |

        e_forceinline self radius()const{
          return m_fRadius;
        }

      //}:                                        |
      //toHeight:{                                |

        e_forceinline self height()const{
          return m_fHeight;
        }

      //}:                                        |
    //}:                                          |
    //--------------------------------------------|-----------------------------

    e_forceinline Capsule( const self& r, const self& h )
      : m_fRadius( r )
      , m_fHeight( h )
    {}

    Capsule() = default;

  private:

    self m_fRadius = 1;
    self m_fHeight = 1;
  };

//}:                                              |
//================================================|=============================
