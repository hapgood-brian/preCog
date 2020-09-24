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

//================================================|=============================
//Capsule:{                                       |

  /** \brief Capsule class.
    *
    * This class defines the geometric capsulte.
    */

  struct E_PUBLISH Capsule final{

    //--------------------------------------------|-----------------------------
    //Methods:{                                   |
      //position:{                                |

        e_forceinline Point3 position()const{
          return m_tPos.xyz();
        }

      //}:                                        |
      //top:{                                     |

        e_forceinline Point3 top()const{
          return position() + axis();
        }

      //}:                                        |
      //length:{                                  |

        e_forceinline self length()const{
          return m_vDir.xyz().length();
        }

      //}:                                        |
      //radius:{                                  |

        e_forceinline self radius()const{
          return m_vDir.w;
        }

      //}:                                        |
      //height:{                                  |

        e_forceinline self height()const{
          return m_tPos.w;
        }

      //}:                                        |
      //axis:{                                    |

        e_forceinline Vector3 axis()const{
          return m_vDir.xyz();
        }

      //}:                                        |
    //}:                                          |
    //--------------------------------------------|-----------------------------

    Capsule( const Point3& start, const Point3& end, const self& radius )
      : m_vDir( e_normalize( Vector3( end - start )), radius )
      , m_tPos( start, Vector3( end-start ).length() )
    {}

    Capsule( const Ray3& ray, const self& radius )
      : m_vDir( e_normalize( ray.d ), radius )
      , m_tPos( ray.p, ray.d.length() )
    {}

    Capsule() = default;

  private:

    e_var( Vector4, v, Dir ) = Vector4( Vector3::kUp, 1/* radius */);
    e_var( Vector4, t, Pos ) = Vector4( 0, 0, 0, 1.f/* height */);
  };

//}:                                              |
//================================================|=============================
