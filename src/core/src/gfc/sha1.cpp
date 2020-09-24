/* sha1.cpp

Copyright (c) 2005 Michael D. Leonhard

http://tamale.net/

This file is licensed under the terms described in the
accompanying LICENSE file.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "sha1.h"

using namespace EON;

// print out memory in hexadecimal
void SHA1::hexPrinter( u8* c, u64 l )
{
	assert( c );
	assert( l > 0 );
	while( l > 0 )
	{
		printf( " %02x", *c );
		l--;
		c++;
	}
}

// circular left bit rotation.  MSB wraps around to LSB
u32 SHA1::lrot( const u32 x, const u32 bits )
{
	return (x<<bits) | (x>>(32 - bits));
};

// Save a 32-bit unsigned integer to memory, in big-endian order
void SHA1::storeBigEndian( u8* byte, const u64 num )
{
	assert( byte );
	byte[0] = u8(( num>>24 ) & 0xFF );
	byte[1] = u8(( num>>16 ) & 0xFF );
	byte[2] = u8(( num>> 8 ) & 0xFF );
	byte[3] = u8(( num>> 0 ) & 0xFF );
}

// process ***********************************************************
void SHA1::process(){
	assert( unprocessedBytes == 64 );
	//printf( "process: " ); hexPrinter( bytes, 64 ); printf( "\n" );
	int t;
	u32 a, b, c, d, e, K, f, W[80];
	// starting values
	a = H0;
	b = H1;
	c = H2;
	d = H3;
	e = H4;
	// copy and expand the message block
	for( t = 0; t < 16; t++ ) W[t] =
    (bytes[t*4 + 0] << 24)
  + (bytes[t*4 + 1] << 16)
  + (bytes[t*4 + 2] << 8)
  +  bytes[t*4 + 3];
	for(; t< 80; t++ ){
    W[t] = lrot( W[t-3]^W[t-8]^W[t-14]^W[t-16], 1 );
  }

	/* main loop */
	u32 temp;
	for( t=0; t<80; ++t ){
		if( t < 20 ){
			K = 0x5a827999;
			f = (b & c) | ((b ^ 0xFFFFFFFF) & d);//TODO: try using ~
		}else if( t < 40 ){
			K = 0x6ed9eba1;
			f = b ^ c ^ d;
		}else if( t < 60 ){
			K = 0x8f1bbcdc;
			f = (b & c) | (b & d) | (c & d);
		}else{
			K = 0xca62c1d6;
			f = b ^ c ^ d;
		}
		temp = lrot( a, 5 ) + f + e + W[t] + K;
		e = d;
		d = c;
		c = lrot( b, 30 );
		b = a;
		a = temp;
		//printf( "t=%d %08x %08x %08x %08x %08x\n",t,a,b,c,d,e );
	}
	/* add variables */
	H0 += a;
	H1 += b;
	H2 += c;
	H3 += d;
	H4 += e;
	//printf( "Current: %08x %08x %08x %08x %08x\n",H0,H1,H2,H3,H4 );
	/* all bytes have been processed */
	unprocessedBytes = 0;
}

// addBytes **********************************************************
void SHA1::addBytes( ccp data, u64 num ){
	assert( data );
	assert( num > 0 );
	// add these bytes to the running total
	size += num;
	// repeat until all data is processed
	while( num > 0 ){
		// number of bytes required to complete block
		u64 needed = 64 - unprocessedBytes;
		assert( needed > 0 );
		// number of bytes to copy (use smaller of two)
		u64 toCopy = (num < needed) ? num : needed;
		// Copy the bytes
		memcpy( bytes + unprocessedBytes, data, size_t( toCopy ));
		// Bytes have been copied
		num -= toCopy;
		data += toCopy;
		unprocessedBytes += toCopy;

		// there is a full block
		if( unprocessedBytes == 64 ){
      process();
    }
	}
}

// digest ************************************************************
u8* SHA1::getDigest(){
	// save the message size
	u64 totalBitsL = size << 3;
	u64 totalBitsH = size >> 29;
	// add 0x80 to the message
	addBytes( "\x80", 1 );
	u8 footer[64]={
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	// block has no room for 8-byte filesize, so finish it
	if( unprocessedBytes > 56 ){
		addBytes(( char* )footer, 64-unprocessedBytes );
  }
	assert( unprocessedBytes <= 56 );
	// how many zeros do we need
	u64 neededZeros = 56 - unprocessedBytes;
	// store file size (in bits) in big-endian format
	storeBigEndian( footer + neededZeros    , totalBitsH );
	storeBigEndian( footer + neededZeros + 4, totalBitsL );
	// finish the final block
	addBytes( (char*)footer, neededZeros + 8 );
	// allocate memory for the digest bytes
	u8* digest = (u8*)e_malloc( 20 );
	// copy the digest bytes
	storeBigEndian( digest +  0, H0 );
	storeBigEndian( digest +  4, H1 );
	storeBigEndian( digest +  8, H2 );
	storeBigEndian( digest + 12, H3 );
	storeBigEndian( digest + 16, H4 );
	// return the digest
	return digest;
}
