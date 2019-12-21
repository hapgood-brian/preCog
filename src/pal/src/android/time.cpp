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

//http://stackoverflow.com/questions/17188761/how-to-obtain-computation-time-in-ndk

#include<time.h>

//================================================|=============================
//e_timeNanoseconds:{                             |

  f64 e_nanoseconds(){
    struct timespec now;
    clock_gettime( CLOCK_MONOTONIC, &now );
    return f64( now.tv_sec*1000000000LL + now.tv_nsec );
  }

//}:                                              |
//e_microseconds:{                                |

  f64 e_microseconds(){
    f64 ns = e_nanoseconds();
    return ns/1000.0;
  }

//}:                                              |
//e_milliseconds:{                                |

  f64 e_milliseconds(){
    f64 us = e_microseconds();
    return us/1000.0;
  }

//}:                                              |
//e_seconds:{                                     |

  f64 e_seconds(){
    f64 ms = e_milliseconds();
    return ms/1000.0;
  }

//}:                                              |
//================================================|=============================
