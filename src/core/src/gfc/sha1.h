/* sha1.h

Copyright (c) 2005 Michael D. Leonhard

http://tamale.net/

This file is licensed under the terms described in the
accompanying LICENSE file.
*/

#pragma once

namespace EON
{
  class SHA1
  {
      void process();

      u32 H0 = 0x67452301;
      u32 H1 = 0xefcdab89;
      u32 H2 = 0x98badcfe;
      u32 H3 = 0x10325476;
      u32 H4 = 0xc3d2e1f0;
      u64 unprocessedBytes = 0;
      u64 size = 0;
      u8 bytes[64];

    public:

      void addBytes( ccp data, const u64 num );
      u8* getDigest();

      static u32 lrot( const u32 x, const u32 bits );
      static void storeBigEndian( u8* byte, const u64 num );
      static void hexPrinter( u8* c, const u64 l );

      SHA1() = default;
  };
}
