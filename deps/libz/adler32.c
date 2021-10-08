/* adler32.c -- compute the Adler-32 checksum of a data stream
 * Copyright (C) 1995-2003 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* @(#) $Id$ */

#define ZLIB_INTERNAL
#include <stdint.h>
#include <stddef.h>

#define BASE 65521UL    /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {s1 += buf[i]; s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

#define MOD(a) a %= BASE

/* ========================================================================= */
uint32_t adler32(uint32_t adler, const uint8_t *buf, size_t len)
{
   uint32_t s1 = adler & 0xffff;
   uint32_t s2 = (adler >> 16) & 0xffff;
   int k;

   if (buf == NULL)
      return 1L;

   while (len > 0) {
      k = len < NMAX ? (int)len : NMAX;
      len -= k;
      while (k >= 16) {
         DO16(buf);
         buf += 16;
         k -= 16;
      }
      if (k != 0) do {
         s1 += *buf++;
         s2 += s1;
      } while (--k);
      MOD(s1);
      MOD(s2);
   }
   return (s2 << 16) | s1;
}
