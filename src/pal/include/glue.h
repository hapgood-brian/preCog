//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

#pragma once

#ifdef __OBJC__
  #import<Cocoa/Cocoa.h>
#endif

//================================================+=============================
//AppDelegate:{                                   |

  #ifdef __OBJC__
    @interface AppDelegate:NSObject<NSApplicationDelegate,NSUserNotificationCenterDelegate>
      @property( assign )IBOutlet NSWindow* window;
    @end
  #endif

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//AppWindow:{                                     |

  #ifdef __OBJC__
    @interface AppWindow:NSWindow
      +(AppWindow*)create:(CGSize)size;
    @end
  #endif

//}:                                              |
//================================================+=============================
//                                                                 vim:ft=objcpp
