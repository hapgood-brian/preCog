//------------------------------------------------------------------------------
//       Copyright 2014-2018 Creepy Doll Games LLC. All rights reserved.
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

#import<foundation/foundation.h>

using namespace EON;

//================================================|=============================
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
//================================================|=============================
//                                                                 vim:ft=objcpp
