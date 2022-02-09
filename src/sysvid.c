/*
 * xrick/src/sysvid.c
 *
 * Copyright (C) 1998-2002 BigOrno (bigorno@bigorno.net). All rights reserved.
 *
 * The use and distribution terms for this software are contained in the file
 * named README, which can be found in the root of this distribution. By
 * using this software in any fashion, you are agreeing to be bound by the
 * terms of this license.
 *
 * You must not remove this notice, or any other, from this software.
 */

#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "system.h"
#include "game.h"
#include "img.h"
#include "debug.h"

#ifdef __MSVC__
#include <memory.h> /* memset */
#endif

U8 *sysvid_fb = NULL; /* frame buffer */

static SDL_Color palette[256];
static SDL_Surface *screen = NULL;

#include "img_icon.e"

/*
 * color tables
 */

#ifdef GFXPC
static U8 RED[] = { 0x00, 0x50, 0xf0, 0xf0, 0x00, 0x50, 0xf0, 0xf0 };
static U8 GREEN[] = { 0x00, 0xf8, 0x50, 0xf8, 0x00, 0xf8, 0x50, 0xf8 };
static U8 BLUE[] = { 0x00, 0x50, 0x50, 0x50, 0x00, 0xf8, 0xf8, 0xf8 };
#endif
#ifdef GFXST
static U8 RED[] = { 0x00, 0xd8, 0xb0, 0xf8,
                    0x20, 0x00, 0x00, 0x20,
                    0x48, 0x48, 0x90, 0xd8,
                    0x48, 0x68, 0x90, 0xb0,
                    /* cheat colors */
                    0x50, 0xe0, 0xc8, 0xf8,
                    0x68, 0x50, 0x50, 0x68,
                    0x80, 0x80, 0xb0, 0xe0,
                    0x80, 0x98, 0xb0, 0xc8
};
static U8 GREEN[] = { 0x00, 0x00, 0x6c, 0x90,
                      0x24, 0x48, 0x6c, 0x48,
                      0x6c, 0x24, 0x48, 0x6c,
                      0x48, 0x6c, 0x90, 0xb4,
		      /* cheat colors */
                      0x54, 0x54, 0x9c, 0xb4,
                      0x6c, 0x84, 0x9c, 0x84,
                      0x9c, 0x6c, 0x84, 0x9c,
                      0x84, 0x9c, 0xb4, 0xcc
};
static U8 BLUE[] = { 0x00, 0x00, 0x68, 0x68,
                     0x20, 0xb0, 0xd8, 0x00,
                     0x20, 0x00, 0x00, 0x00,
                     0x48, 0x68, 0x90, 0xb0,
		     /* cheat colors */
                     0x50, 0x50, 0x98, 0x98,
                     0x68, 0xc8, 0xe0, 0x50,
                     0x68, 0x50, 0x50, 0x50,
                     0x80, 0x98, 0xb0, 0xc8};
#endif

void sysvid_setPalette(img_color_t *pal, U16 n)
{
   U16 i;

   for (i = 0; i < n; i++)
   {
      palette[i].r = pal[i].r;
      palette[i].g = pal[i].g;
      palette[i].b = pal[i].b;
   }
   SDL_SetPalette(screen,0, (SDL_Color *)&palette, 0, n);
}

void sysvid_setGamePalette(void)
{
   U8 i;
   img_color_t pal[256];

   for (i = 0; i < 32; ++i)
   {
      pal[i].r = RED[i];
      pal[i].g = GREEN[i];
      pal[i].b = BLUE[i];
   }
   sysvid_setPalette(pal, 32);
}

/*
 * Initialise video
 */
void sysvid_init(void)
{
   /* video modes and screen */
   screen     = SDL_CreateRGBSurface( SDL_HWSURFACE | SDL_HWPALETTE, SYSVID_WIDTH, SYSVID_HEIGHT, 8 , 0x00ff0000,0x0000ff00,0xff,0xff000000);

   /*
    * create v_ frame buffer
    */
   sysvid_fb = malloc(SYSVID_WIDTH * SYSVID_HEIGHT);
}

/*
 * Shutdown video
 */
void sysvid_shutdown(void)
{
   if (sysvid_fb)
      free(sysvid_fb);
   sysvid_fb = NULL;

   if (screen)
      SDL_FreeSurface(screen);
   screen = NULL;
}

extern SDL_Surface *sdlscrn; 

void blit(void)
{
   SDL_BlitSurface(screen, NULL, sdlscrn, NULL);
}

/*
 * Update screen
 * NOTE errors processing ?
 */
void sysvid_update(rect_t *rects)
{
   U16 x, y;
   U8 *p, *q, *p0, *q0;

   if (!rects)
      return;

   while (rects)
   {
      p0  = sysvid_fb;
      p0 += rects->x + rects->y * SYSVID_WIDTH;
      q0  = (U8 *)screen->pixels;
      q0 += (rects->x + rects->y * SYSVID_WIDTH);

      for (y = rects->y; y < rects->y + rects->height; y++)
      {
         p = p0;
         q = q0;
         for (x = rects->x; x < rects->x + rects->width; x++)
         {
            *q = *p;
            q++;
            p++;
         }
         q0 += SYSVID_WIDTH;
         p0 += SYSVID_WIDTH;
      }

      rects  = rects->next;
   }
}

/*
 * Clear screen
 * (077C)
 */
void sysvid_clear(void)
{
   memset(sysvid_fb, 0, SYSVID_WIDTH * SYSVID_HEIGHT);
}
