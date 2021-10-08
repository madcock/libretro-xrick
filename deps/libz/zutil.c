/* zutil.c -- target dependent utility functions for the compression library
 * Copyright (C) 1995-2005, 2010, 2011, 2012 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* @(#) $Id$ */

#include <compat/zutil.h>

char z_errmsg[10][21] = {
   "need dictionary",     /* Z_NEED_DICT       2  */
   "stream end",          /* Z_STREAM_END      1  */
   "",                    /* Z_OK              0  */
   "file error",          /* Z_ERRNO         (-1) */
   "stream error",        /* Z_STREAM_ERROR  (-2) */
   "data error",          /* Z_DATA_ERROR    (-3) */
   "insufficient memory", /* Z_MEM_ERROR     (-4) */
   "buffer error",        /* Z_BUF_ERROR     (-5) */
   "incompatible version",/* Z_VERSION_ERROR (-6) */
   ""};

#if defined(_WIN32_WCE)
/* The Microsoft C Run-Time Library for Windows CE doesn't have
 * errno.  We define it as a global variable to simplify porting.
 * Its value is always 0 and should not be used.
 */
int errno = 0;
#endif

voidpf ZLIB_INTERNAL zcalloc (voidpf opaque, unsigned items, unsigned size)
{
   if (opaque) items += size - size; /* make compiler happy */
   return sizeof(uInt) > 2 ? (voidpf)malloc(items * size) :
      (voidpf)calloc(items, size);
}

void ZLIB_INTERNAL zcfree (voidpf opaque, voidpf ptr)
{
   free(ptr);
   if (opaque) return; /* make compiler happy */
}
