//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

using namespace EON;

//================================================|=============================
//IEngine:{                                       |
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
      return true;
    }

    template<> bool IEngine::is<Platform::Win10>(){
      return false;
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
