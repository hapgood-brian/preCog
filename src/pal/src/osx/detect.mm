//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#import<foundation/foundation.h>

using namespace EON;

//================================================+=============================
//IEngine:{                                       |
  //is:{                                          |

    //https://en.wikipedia.org/wiki/OS_X

    template<> bool IEngine::is<Platform::Sierra>(){
      NSOperatingSystemVersion osVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
      return( osVersion.majorVersion == 10 )&&( osVersion.minorVersion == 12);
    }

    template<> bool IEngine::is<Platform::HighSierra>(){
      NSOperatingSystemVersion osVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
      return( osVersion.majorVersion == 10 )&&( osVersion.minorVersion == 13);
    }

    template<> bool IEngine::is<Platform::Mojave>(){
      NSOperatingSystemVersion osVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
      return( osVersion.majorVersion == 10 )&&( osVersion.minorVersion == 14);
    }

    template<> bool IEngine::is<Platform::Catalina>(){
      NSOperatingSystemVersion osVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
      return( osVersion.majorVersion == 10 )&&( osVersion.minorVersion == 15);
    }

    template<> bool IEngine::is<Platform::BigSur>(){
      NSOperatingSystemVersion osVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
      return( osVersion.majorVersion == 11 )&&( osVersion.minorVersion == 0);
    }

    template<> bool IEngine::is<Platform::Linux>(){
      return false;
    }

    template<> bool IEngine::is<Platform::Win10>(){
      return false;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                                 vim:ft=objcpp
