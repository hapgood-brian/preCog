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

#import<mach/mach.h>
#import<mach/mach_time.h>
#import<unistd.h>

using namespace EON;
using namespace gfc;

//================================================+=============================
//Functions:{                                     |
  //e_nanoseconds:{                               |

    e_noinline double e_nanoseconds(){
      static mach_timebase_info_data_t s_timebase_info={0,0};
      if( !s_timebase_info.denom ){
        mach_timebase_info( &s_timebase_info );
      }
      const u64 thisTime=((mach_absolute_time()*(s_timebase_info.numer)/s_timebase_info.denom));
      return double( thisTime );
    }

  //}:                                            |
  //e_microseconds:{                              |

    e_noinline double e_microseconds(){
      const double ns = e_nanoseconds();
      return ns/1000.;
    }

  //}:                                            |
  //e_milliseconds:{                              |

    e_noinline double e_milliseconds(){
      const double us = e_microseconds();
      return us/1000.;
    }

  //}:                                            |
  //e_seconds:{                                   |

    e_noinline double e_seconds(){
      const double ms = e_milliseconds();
      return ms/1000.;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                :
//                                                :
//                                                :
//================================================+=============================
//Time:{                                          |
  //nanoseconds:{                                 |

    template<> double Time<Scale::nanosecond>::nanoseconds( const double x ){
      return x*1.;
    }

    template<> double Time<Scale::nanosecond>::microseconds( const double x ){
      return x*1000.;
    }

    template<> double Time<Scale::nanosecond>::milliseconds( const double x ){
      return x*1000000.;
    }

    template<> double Time<Scale::nanosecond>::seconds( const double x ){
      return x*1000000000.;
    }

    template<> double Time<Scale::nanosecond>::minutes( const double x ){
      return x*Time<Scale::nanosecond>::seconds()*60.;
    }

    template<> double Time<Scale::nanosecond>::hours( const double x ){
      return x*Time<Scale::nanosecond>::minutes()*60.;
    }

    template<> double Time<Scale::nanosecond>::days( const double x ){
      return x*Time<Scale::nanosecond>::hours()*24.;
    }

    template<> double Time<Scale::nanosecond>::weeks( const double x ){
      return x*Time<Scale::nanosecond>::days()*7.;
    }

    template<> double Time<Scale::nanosecond>::years( const double x ){
      return x*Time<Scale::nanosecond>::weeks()*52.;
    }

  //}:                                            |
  //microseconds:{                                |

    template<> double Time<Scale::microsecond>::nanoseconds( const double x ){
      return x*1000.;
    }

    template<> double Time<Scale::microsecond>::microseconds( const double x ){
      return x*1.;
    }

    template<> double Time<Scale::microsecond>::milliseconds( const double x ){
      return x*1./1000.;
    }

    template<> double Time<Scale::microsecond>::seconds( const double x ){
      return x*1./1000000.;
    }

    template<> double Time<Scale::microsecond>::minutes( const double x ){
      return x*Time<Scale::microsecond>::seconds()*60.;
    }

    template<> double Time<Scale::microsecond>::hours( const double x ){
      return x*Time<Scale::microsecond>::minutes()*60.;
    }

    template<> double Time<Scale::microsecond>::days( const double x ){
      return x*Time<Scale::microsecond>::hours()*24.;
    }

    template<> double Time<Scale::microsecond>::weeks( const double x ){
      return x*Time<Scale::microsecond>::days()*7.;
    }

    template<> double Time<Scale::microsecond>::years( const double x ){
      return x*Time<Scale::microsecond>::weeks()*52.;
    }

  //}:                                            |
  //milliseconds:{                                |

    template<> double Time<Scale::millisecond>::nanoseconds( const double x ){
      return x*1000000.;
    }

    template<> double Time<Scale::millisecond>::microseconds( const double x ){
      return x*1000.;
    }

    template<> double Time<Scale::millisecond>::milliseconds( const double x ){
      return x*1.;
    }

    template<> double Time<Scale::millisecond>::seconds( const double x ){
      return x*1./1000.;
    }

    template<> double Time<Scale::millisecond>::minutes( const double x ){
      return x*Time<Scale::millisecond>::seconds()*60.;
    }

    template<> double Time<Scale::millisecond>::hours( const double x ){
      return x*Time<Scale::millisecond>::minutes()*60.;
    }

    template<> double Time<Scale::millisecond>::days( const double x ){
      return x*Time<Scale::millisecond>::hours()*24.;
    }

    template<> double Time<Scale::millisecond>::weeks( const double x ){
      return x*Time<Scale::millisecond>::days()*7.;
    }

    template<> double Time<Scale::millisecond>::years( const double x ){
      return x*Time<Scale::millisecond>::weeks()*52.;
    }

  //}:                                            |
  //seconds:{                                     |

    template<> double Time<Scale::second>::nanoseconds( const double x ){
      return x*1000000000.;
    }

    template<> double Time<Scale::second>::microseconds( const double x ){
      return x*1000000.;
    }

    template<> double Time<Scale::second>::milliseconds( const double x ){
      return x*1000.;
    }

    template<> double Time<Scale::second>::seconds( const double x ){
      return x;
    }

    template<> double Time<Scale::second>::minutes( const double x ){
      return x*Time<Scale::second>::seconds()*60.;
    }

    template<> double Time<Scale::second>::hours( const double x ){
      return x*Time<Scale::second>::minutes()*60.;
    }

    template<> double Time<Scale::second>::days( const double x ){
      return x*Time<Scale::second>::hours()*24.;
    }

    template<> double Time<Scale::second>::weeks( const double x ){
      return x*Time<Scale::second>::days()*7.;
    }

    template<> double Time<Scale::second>::years( const double x ){
      return x*Time<Scale::second>::weeks()*52.;
    }

  //}:                                            |
  //minutes:{                                     |

    template<> double Time<Scale::minute>::nanoseconds( const double x ){
      return x*60000000000.;
    }

    template<> double Time<Scale::minute>::microseconds( const double x ){
      return x*60000000.;
    }

    template<> double Time<Scale::minute>::milliseconds( const double x ){
      return x*60000.;
    }

    template<> double Time<Scale::minute>::seconds( const double x ){
      return x*60.;
    }

    template<> double Time<Scale::minute>::minutes( const double x ){
      return x*Time<Scale::minute>::seconds()*60.;
    }

    template<> double Time<Scale::minute>::hours( const double x ){
      return x*Time<Scale::minute>::minutes()*60.;
    }

    template<> double Time<Scale::minute>::days( const double x ){
      return x*Time<Scale::minute>::hours()*24.;
    }

    template<> double Time<Scale::minute>::weeks( const double x ){
      return x*Time<Scale::minute>::days()*7.;
    }

    template<> double Time<Scale::minute>::years( const double x ){
      return x*Time<Scale::minute>::weeks()*52.;
    }

  //}:                                            |
  //hours:{                                       |

    template<> double Time<Scale::hour>::nanoseconds( const double x ){
      return x*3600000000000.;
    }

    template<> double Time<Scale::hour>::microseconds( const double x ){
      return x*3600000000.;
    }

    template<> double Time<Scale::hour>::milliseconds( const double x ){
      return x*3600000.;
    }

    template<> double Time<Scale::hour>::seconds( const double x ){
      return x*3600.;
    }

    template<> double Time<Scale::hour>::minutes( const double x ){
      return x*Time<Scale::hour>::seconds()*60.;
    }

    template<> double Time<Scale::hour>::hours( const double x ){
      return x*Time<Scale::hour>::minutes()*60.;
    }

    template<> double Time<Scale::hour>::days( const double x ){
      return x*Time<Scale::hour>::hours()*24.;
    }

    template<> double Time<Scale::hour>::weeks( const double x ){
      return x*Time<Scale::hour>::days()*7.;
    }

    template<> double Time<Scale::hour>::years( const double x ){
      return x*Time<Scale::hour>::weeks()*52.;
    }

  //}:                                            |
  //Days:{                                        |

    template<> double Time<Scale::day>::nanoseconds( const double x ){
      return x*86400000000000.;
    }

    template<> double Time<Scale::day>::microseconds( const double x ){
      return x*86400000000.;
    }

    template<> double Time<Scale::day>::milliseconds( const double x ){
      return x*86400000.;
    }

    template<> double Time<Scale::day>::seconds( const double x ){
      return x*86400.;
    }

    template<> double Time<Scale::day>::minutes( const double x ){
      return x*Time<Scale::day>::seconds()*60.;
    }

    template<> double Time<Scale::day>::hours( const double x ){
      return x*Time<Scale::day>::minutes()*60.;
    }

    template<> double Time<Scale::day>::days( const double x ){
      return x*Time<Scale::day>::hours()*24.;
    }

    template<> double Time<Scale::day>::weeks( const double x ){
      return x*Time<Scale::day>::days()*7.;
    }

    template<> double Time<Scale::day>::years( const double x ){
      return x*Time<Scale::day>::weeks()*52.;
    }

  //}:                                            |
  //Weeks:{                                       |

    template<> double Time<Scale::week>::nanoseconds( const double x ){
      return x*604800000000000.;
    }

    template<> double Time<Scale::week>::microseconds( const double x ){
      return x*604800000000.;
    }

    template<> double Time<Scale::week>::milliseconds( const double x ){
      return x*604800000.;
    }

    template<> double Time<Scale::week>::seconds( const double x ){
      return x*604800.;
    }

    template<> double Time<Scale::week>::minutes( const double x ){
      return x*Time<Scale::week>::seconds()*60.;
    }

    template<> double Time<Scale::week>::hours( const double x ){
      return x*Time<Scale::week>::minutes()*60.;
    }

    template<> double Time<Scale::week>::days( const double x ){
      return x*Time<Scale::week>::hours()*24.;
    }

    template<> double Time<Scale::week>::weeks( const double x ){
      return x*Time<Scale::week>::days()*7.;
    }

    template<> double Time<Scale::week>::years( const double x ){
      return x*Time<Scale::week>::weeks()*52.;
    }

  //}:                                            |
  //Years:{                                       |

    template<> double Time<Scale::year>::nanoseconds( const double x ){
      return x*29030400000000000.;
    }

    template<> double Time<Scale::year>::microseconds( const double x ){
      return x*29030400000000.;
    }

    template<> double Time<Scale::year>::milliseconds( const double x ){
      return x*29030400000.;
    }

    template<> double Time<Scale::year>::seconds( const double x ){
      return x*29030400.;
    }

    template<> double Time<Scale::year>::minutes( const double x ){
      return x*Time<Scale::year>::seconds()*60.;
    }

    template<> double Time<Scale::year>::hours( const double x ){
      return x*Time<Scale::year>::minutes()*60.;
    }

    template<> double Time<Scale::year>::days( const double x ){
      return x*Time<Scale::year>::hours()*24.;
    }

    template<> double Time<Scale::year>::weeks( const double x ){
      return x*Time<Scale::year>::days()*7.;
    }

    template<> double Time<Scale::year>::years( const double x ){
      return x*Time<Scale::year>::weeks()*52.;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
//                                                                 vim:ft=objcpp
