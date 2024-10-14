//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#include<windows.h>
#include<VersionHelpers.h>
#include<eon//eon.h>

using namespace EON;
using namespace gfc;

//================================================|=============================
//IEngine:{                                       |
  //osVersion:{                                   |

    string IEngine::osVersion(){
      return"Windows";
    }

  //}:                                            |
  //is:{                                          |

    template<> bool IEngine::is<Platform::Sierra>(){
      return false;
    }

    template<> bool IEngine::is<Platform::HighSierra>(){
      return false;
    }

    template<> bool IEngine::is<Platform::Mojave>(){
      return false;
    }

    template<> bool IEngine::is<Platform::Catalina>(){
      return false;
    }

    template<> bool IEngine::is<Platform::BigSur>(){
      return false;
    }

    template<> bool IEngine::is<Platform::Linux>(){
      return false;
    }

    template<> bool IEngine::is<Platform::Win10>(){
      return true;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
