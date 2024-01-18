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

/** \addtogroup engine
  * @{
  *   \addtogroup gfc
  *   @{
  *     \addtogroup histories
  *     @{
  */

//================================================+=============================
//Joystick:{                                      |

  namespace EON{

    namespace gfc{

      /** \brief The joystick structure.
        *
        * This structure contains information about a joystick. There may be many
        * joystick devices attached to the system and therefore multiple joystick
        * structures to be dispatched via the IController::onJoystick() action.
        *
        * The m_aAxis array has two elements, one for each of two sticks used when
        * the joystick is a gamepad controller.
        *
        * The m_tFlags bitfield contains bits for the dpad and up to 12 buttons. If
        * the bit is set (1) the button is pressed otherwise clear (0).
        *
        * The default value of all members is zero when constructed.
        */

      struct E_PUBLISH Joystick final{
        using Stick = array<vec2,33>;
        e_var_array1( Stick, 4 );
        e_var_bits( Flags
          , DPadU:1
          , DPadD:1
          , DPadL:1
          , DPadR:1
          , Btn1:1
          , Btn2:1
          , Btn3:1
          , Btn4:1
          , Btn5:1
          , Btn6:1
          , Btn7:1
          , Btn8:1
          , Btn9:1
          , BtnA:1
          , BtnB:1
          , BtnC:1
          , Trigger1:1
          , Trigger2:1
          , ID:32
        );
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//MouseData:{                                     |

  namespace EON{

    namespace gfc{

      /** \brief The mouse structure.
        *
        * This structure contains the delta position and button information for
        * the mouse. It is assumed by the code that only one mouse will be
        * attached at any given time.
        */

      struct E_PUBLISH Mouse final{

        /** \brief Delta position.
          *
          * This member contains the delta position of the mouse. The xy origin
          * is at the top-left corner of the screen otherwise known as device
          * coordinates. The x coordinate is +right and the y coordinate is
          * +down just like Windows. The z component is the value of the mouse
          * wheel.
          */

        vec3 deltaPos;

        /** \brief Button values.
          *
          * This member variable contains the button states of the mouse. The x
          * value is 1 if the left mouse button is down and 0 otherwise, the y
          * value is 1 if the right mouse button is pressed, and the z value is
          * 1 if the middle button is pressed.
          */

        vec4 btn;
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Key:{                                    |

  namespace EON{

    namespace gfc{

      /** \brief Keyboard identifier.
        *
        * This enumerated type lists all the keyboard identifiers supported by the
        * EON engine. These are the codes sent to the onKeyDown and onKeyUp actions
        * dispatched to the IController class.
        */

      enum class Key:u32{        kMISSING         = 0x00,
        kESCAPE          = 0x01, k1               = 0x02,
        k2               = 0x03, k3               = 0x04,
        k4               = 0x05, k5               = 0x06,
        k6               = 0x07, k7               = 0x08,
        k8               = 0x09, k9               = 0x0A,
        k0               = 0x0B, kMINUS           = 0x0C,
        kEQUALS          = 0x0D, kBACK            = 0x0E,
        kTAB             = 0x0F, kQ               = 0x10,
        kW               = 0x11, kE               = 0x12,
        kR               = 0x13, kT               = 0x14,
        kY               = 0x15, kU               = 0x16,
        kI               = 0x17, kO               = 0x18,
        kP               = 0x19, kLBRACKET        = 0x1A,
        kRBRACKET        = 0x1B, kRETURN          = 0x1C,
        kLCONTROL        = 0x1D, kA               = 0x1E,
        kS               = 0x1F, kD               = 0x20,
        kF               = 0x21, kG               = 0x22,
        kH               = 0x23, kJ               = 0x24,
        kK               = 0x25, kL               = 0x26,
        kSEMICOLON       = 0x27, kAPOSTROPHE      = 0x28,
        kGRAVE           = 0x29, kLSHIFT          = 0x2A,
        kBACKSLASH       = 0x2B, kZ               = 0x2C,
        kX               = 0x2D, kC               = 0x2E,
        kV               = 0x2F, kB               = 0x30,
        kN               = 0x31, kM               = 0x32,
        kCOMMA           = 0x33, kPERIOD          = 0x34,
        kSLASH           = 0x35, kRSHIFT          = 0x36,
        kMULTIPLY        = 0x37, kLMENU           = 0x38,
        kSPACE           = 0x39, kCAPITAL         = 0x3A,
        kF1              = 0x3B, kF2              = 0x3C,
        kF3              = 0x3D, kF4              = 0x3E,
        kF5              = 0x3F, kF6              = 0x40,
        kF7              = 0x41, kF8              = 0x42,
        kF9              = 0x43, kF10             = 0x44,
        kNUMLOCK         = 0x45, kSCROLL          = 0x46,
        kNUMPAD7         = 0x47, kNUMPAD8         = 0x48,
        kNUMPAD9         = 0x49, kSUBTRACT        = 0x4A,
        kNUMPAD4         = 0x4B, kNUMPAD5         = 0x4C,
        kNUMPAD6         = 0x4D, kADD             = 0x4E,
        kNUMPAD1         = 0x4F, kNUMPAD2         = 0x50,
        kNUMPAD3         = 0x51, kNUMPAD0         = 0x52,
        kDECIMAL         = 0x53, kOEM_102         = 0x56,
        kF11             = 0x57, kF12             = 0x58,
        kF13             = 0x64, kF14             = 0x65,
        kF15             = 0x66, kKANA            = 0x70,
        kABNT_C1         = 0x73, kCONVERT         = 0x79,
        kNOCONVERT       = 0x7B, kYEN             = 0x7D,
        kABNT_C2         = 0x7E, kNUMPADEQUALS    = 0x8D,
        kPREVTRACK       = 0x90, kAT              = 0x91,
        kCOLON           = 0x92, kUNDERLINE       = 0x93,
        kKANJI           = 0x94, kSTOP            = 0x95,
        kAX              = 0x96, kUNLABELED       = 0x97,
        kNEXTTRACK       = 0x99, kNUMPADENTER     = 0x9C,
        kRCONTROL        = 0x9D, kMUTE            = 0xA0,
        kCALCULATOR      = 0xA1, kPLAYPAUSE       = 0xA2,
        kMEDIASTOP       = 0xA4, kVOLUMEDOWN      = 0xAE,
        kVOLUMEUP        = 0xB0, kWEBHOME         = 0xB2,
        kNUMPADCOMMA     = 0xB3, kDIVIDE          = 0xB5,
        kSYSRQ           = 0xB7, kRMENU           = 0xB8,
        kPAUSE           = 0xC5, kHOME            = 0xC7,
        kUP              = 0xC8, kPRIOR           = 0xC9,
        kLEFT            = 0xCB, kRIGHT           = 0xCD,
        kEND             = 0xCF, kDOWN            = 0xD0,
        kNEXT            = 0xD1, kINSERT          = 0xD2,
        kDELETE          = 0xD3, kLWIN            = 0xDB,
        kRWIN            = 0xDC, kAPPS            = 0xDD,
        kPOWER           = 0xDE, kSLEEP           = 0xDF,
        kWAKE            = 0xE3, kWEBSEARCH       = 0xE5,
        kWEBFAVORITES    = 0xE6, kWEBREFRESH      = 0xE7,
        kWEBSTOP         = 0xE8, kWEBFORWARD      = 0xE9,
        kWEBBACK         = 0xEA, kMYCOMPUTER      = 0xEB,
        kMAIL            = 0xEC, kMEDIASELECT     = 0xED,
        kMETA            = 0xFE, kUNKNOWN         = 0xFF,
      };

      static ccp cKey[]={ ""           ,
        "ESCAPE"      ,   "1"          ,
        "2"           ,   "3"          ,
        "4"           ,   "5"          ,
        "6"           ,   "7"          ,
        "8"           ,   "9"          ,
        "0"           ,   "MINUS"      ,
        "EQUALS"      ,   "BACK"       ,
        "TAB"         ,   "Q"          ,
        "W"           ,   "E"          ,
        "R"           ,   "T"          ,
        "Y"           ,   "U"          ,
        "I"           ,   "O"          ,
        "P"           ,   "LBRACKET"   ,
        "RBRACKET"    ,   "RETURN"     ,
        "LCONTROL"    ,   "A"          ,
        "S"           ,   "D"          ,
        "F"           ,   "G"          ,
        "H"           ,   "J"          ,
        "K"           ,   "L"          ,
        "SEMICOLON"   ,   "APOSTROPHE" ,
        "GRAVE"       ,   "LSHIFT"     ,
        "BACKSLASH"   ,   "Z"          ,
        "X"           ,   "C"          ,
        "V"           ,   "B"          ,
        "N"           ,   "M"          ,
        "COMMA"       ,   "PERIOD"     ,
        "SLASH"       ,   "RSHIFT"     ,
        "MULTIPLY"    ,   "LMENU"      ,
        "SPACE"       ,   "CAPITAL"    ,
        "F1"          ,   "F2"         ,
        "F3"          ,   "F4"         ,
        "F5"          ,   "F6"         ,
        "F7"          ,   "F8"         ,
        "F9"          ,   "F10"        ,
        "NUMLOCK"     ,   "SCROLL"     ,
        "NUMPAD7"     ,   "NUMPAD8"    ,
        "NUMPAD9"     ,   "SUBTRACT"   ,
        "NUMPAD4"     ,   "NUMPAD5"    ,
        "NUMPAD6"     ,   "ADD"        ,
        "NUMPAD1"     ,   "NUMPAD2"    ,
        "NUMPAD3"     ,   "NUMPAD0"    ,
        "DECIMAL"     ,   "OEM_102"    ,
        "F11"         ,   "F12"        ,
        "F13"         ,   "F14"        ,
        "F15"         ,   "KANA"       ,
        "ABNT_C1"     ,   "CONVERT"    ,
        "NOCONVERT"   ,   "YEN"        ,
        "ABNT_C2"     ,   "NUMPADEQUAL",
        "PREVTRACK"   ,   "AT"         ,
        "COLON"       ,   "UNDERLINE"  ,
        "KANJI"       ,   "STOP"       ,
        "AX"          ,   "UNLABELED"  ,
        "NEXTTRACK"   ,   "NUMPADENTER",
        "RCONTROL"    ,   "MUTE"       ,
        "CALCULATOR"  ,   "PLAYPAUSE"  ,
        "MEDIASTOP"   ,   "VOLUMEDOWN" ,
        "VOLUMEUP"    ,   "WEBHOME"    ,
        "NUMPADCOMMA" ,   "DIVIDE"     ,
        "SYSRQ"       ,   "RMENU"      ,
        "PAUSE"       ,   "HOME"       ,
        "UP"          ,   "PRIOR"      ,
        "LEFT"        ,   "RIGHT"      ,
        "END"         ,   "DOWN"       ,
        "NEXT"        ,   "INSERT"     ,
        "DELETE"      ,   "LWIN"       ,
        "RWIN"        ,   "APPS"       ,
        "POWER"       ,   "SLEEP"      ,
        "WAKE"        ,   "WEBSEARCH"  ,
        "WEBFAVORITES",   "WEBREFRESH" ,
        "WEBSTOP"     ,   "WEBFORWARD" ,
        "WEBBACK"     ,   "MYCOMPUTER" ,
        "MAIL"        ,   "MEDIASELECT",
        "META"        ,   "UNKNOWN"    ,
      };
    }
  }

//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//IController:{                                   |

  namespace EON{

    namespace gfc{

      struct E_PUBLISH IController{

        //----------------------------------------+-----------------------------
        //Actions:{                               |

          /** \brief Joystick interaction action.
            *
            * This action is triggered when a connected joystick or gamepad
            * device is interacted with. The payload is a Joystick object with
            * information on the current state of the device.
            *
            * \param pJoystick A pointer to a Joystick object containing
            * detailed information on the state of the device.
            */

          virtual void onJoystick( const Joystick* pJoystick){
            (void)pJoystick;
          }

          /** \brief Mouse interaction action.
            *
            * This action is triggered when the use moves the mouse or presses
            * a mouse button.
            *
            * \param pMouse A pointer to a Mouse object containing information
            * about the delta position of the mouse and which buttons are
            * pressed.
            */

          virtual void onMouse( const Mouse* pMouse ){
            (void)pMouse;
          }

          /** \brief Mouse wheel action.
            *
            * This action is triggered when the mouse wheel is scrolled. It is
            * a bit of a helper function that duplicates the onMouse() action.
            *
            * \param pMouse A pointer to a Mouse object containing information
            * about the delta position of the mouse and which buttons are
            * pressed.  The delta wheel value is contained in
            * pMouse->toAxis().z.
            */

          virtual void onWheel( const Mouse* pMouse ){
            (void)pMouse;
          }

          /** \brief Tilt callback.
            *
            * This callback is called from the action system when a mobile
            * device is tilted. It is not received from desktop versions of the
            * engine.
            *
            * \param x The x axis value.
            * \param y The y axis value.
            * \param z The z axis value.
            */

          virtual void onTilt( const f32& x, const f32& y, const f32& z ){
            (void)x;
            (void)y;
            (void)z;
          }

          /** \brief Key down action.
            *
            * This action is triggered when the user presses a key on the
            * keyboard.
            *
            * \param code The keyboard code for the action.
            */

          virtual bool onKeyDown( const Key code ){
            (void)code;
            return false;
          }

          /** \brief Key up action.
            *
            * This action is triggered when the user releases a pressed key on
            * the keyboard.
            *
            * \param code The keyboard code for the action.
            */

          virtual bool onKeyUp( const Key code ){
            return false;
          }

        //}:                                      |
        //----------------------------------------+-----------------------------

        virtual~IController() = default;
      };
    }
  }

//}:                                              |
//================================================+=============================

/**     @}
  *   @}
  * @}
  */
