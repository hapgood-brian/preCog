//------------------------------------------------------------------------------
//          Copyright 2014 RED Engine Games, LLC. All rights reserved.
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

#import<Foundation/NSString.h>
#import<sys/utsname.h>

//================================================|=============================
//is*:{                                           |
  //IEngine::is:{                                 |

    namespace{
      NSString* machineName(){
        utsname systemInfo;
        uname( &systemInfo );
        return [NSString stringWithCString:systemInfo.machine
          encoding:NSUTF8StringEncoding]
        ;
      }
    }

    template<> bool IEngine::is<Platform::iPod>(){
      return [machineName() isEqual:@"iPod1,1"];
    }

    template<> bool IEngine::is<Platform::iPod2>(){
      return [machineName() isEqual:@"iPod2,1"];
    }

    template<> bool IEngine::is<Platform::iPod3>(){
      return [machineName() isEqual:@"iPod3,1"];
    }

    template<> bool IEngine::is<Platform::iPod4>(){
      return [machineName() isEqual:@"iPod4,1"];
    }

    template<> bool IEngine::is<Platform::iPhone>(){
      return [machineName() isEqual:@"iPhone1,1"];
    }

    template<> bool IEngine::is<Platform::iPhone3G>(){
      return [machineName() isEqual:@"iPhone1,2"];
    }

    template<> bool IEngine::is<Platform::iPhone3GS>(){
      return [machineName() isEqual:@"iPhone2,1"];
    }

    template<> bool IEngine::is<Platform::iPhone4>(){
      return [machineName() isEqual:@"iPhone3,1"];
    }

    template<> bool IEngine::is<Platform::iPhone4S>(){
      return [machineName() isEqual:@"iPhone4,1"];
    }

    template<> bool IEngine::is<Platform::iPhone5>(){
      return
        [machineName() isEqual:@"iPhone5,1"] ||
        [machineName() isEqual:@"iPhone5,2"]
      ;
    }

    template<> bool IEngine::is<Platform::iPhone5C>(){
      return
        [machineName() isEqual:@"iPhone5,3"] ||
        [machineName() isEqual:@"iPhone5,4"]
      ;
    }

    template<> bool IEngine::is<Platform::iPhone5S>(){
      return
        [machineName() isEqual:@"iPhone6,1"] ||
        [machineName() isEqual:@"iPhone6,2"]
      ;
    }

    template<> bool IEngine::is<Platform::iPad>(){
      return [machineName() isEqual:@"iPad1,1"];
    }

    template<> bool IEngine::is<Platform::iPad2>(){
      return [machineName() isEqual:@"iPad2,1"];
    }

    template<> bool IEngine::is<Platform::iPad3>(){
      return [machineName() isEqual:@"iPad3,1"];
    }

    template<> bool IEngine::is<Platform::iPadMini>(){
      return [machineName() isEqual:@"iPad2,5"];
    }

    template<> bool IEngine::is<Platform::iPadMini2>(){
      return
          [machineName() isEqual:@"iPad4,4"] ||
          [machineName() isEqual:@"iPad4,5"];
    }

    template<> bool IEngine::is<Platform::iPadAir>(){
      return
          [machineName() isEqual:@"iPad4,1"] ||
          [machineName() isEqual:@"iPad4,2"];
    }

  //}:                                            |
//}:                                              |
//================================================|=============================
//                                                                vim: ft=objcpp
