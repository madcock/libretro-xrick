/*
 * xrick/src/sysarg.c
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

/*
 * 20021010 added test to prevent buffer overrun in -keys parsing.
 */

#include <stdlib.h>  /* atoi */
#include <string.h>  /* strcasecmp */

#include "system.h"
#include "config.h"
#include "game.h"

#include "maps.h"
#include "syssnd.h"

/* handle Microsoft Visual C (must come after system.h!) */
#ifdef __MSVC__
#define strcasecmp stricmp
#endif

int sysarg_args_period = 0;
int sysarg_args_map = 0;
int sysarg_args_submap = 0;
int sysarg_args_nosound = 0;
int sysarg_args_vol = 0;
char *sysarg_args_data = NULL;

/*
 * Fail
 */
void
sysarg_fail(char *msg)
{
	exit(1);
}

/*
 * Read and process arguments
 */
void
sysarg_init(int argc, char **argv)
{
  int i;

  for (i = 1; i < argc; i++) {

    if (!strcmp(argv[i], "-speed")) {
      if (++i == argc) sysarg_fail("missing speed value");
      sysarg_args_period = atoi(argv[i]) - 1;
      if (sysarg_args_period < 0 || sysarg_args_period > 99)
	sysarg_fail("invalid speed value");
    }

    else if (!strcmp(argv[i], "-map")) {
      if (++i == argc) sysarg_fail("missing map number");
      sysarg_args_map = atoi(argv[i]) - 1;
      if (sysarg_args_map < 0 || sysarg_args_map >= MAP_NBR_MAPS-1)
	sysarg_fail("invalid map number");
    }

    else if (!strcmp(argv[i], "-submap")) {
      if (++i == argc) sysarg_fail("missing submap number");
      sysarg_args_submap = atoi(argv[i]) - 1;
      if (sysarg_args_submap < 0 || sysarg_args_submap >= MAP_NBR_SUBMAPS)
	sysarg_fail("invalid submap number");
    }
#ifdef ENABLE_SOUND
    else if (!strcmp(argv[i], "-vol")) {
      if (++i == argc) sysarg_fail("missing volume");
      sysarg_args_vol = atoi(argv[i]) - 1;
      if (sysarg_args_submap < 0 || sysarg_args_submap >= SYSSND_MAXVOL)
	sysarg_fail("invalid volume");
    }

    else if (!strcmp(argv[i], "-nosound")) {
      sysarg_args_nosound = 1;
    }
#endif
	else if (!strcmp(argv[i], "-data")) {
		if (++i == argc) sysarg_fail("missing data");
		sysarg_args_data = argv[i];
	}

    else {
      sysarg_fail("invalid argument(s)");
    }

  }

  /* this is dirty (sort of) */
  if (sysarg_args_submap > 0 && sysarg_args_submap < 9)
    sysarg_args_map = 0;
  if (sysarg_args_submap >= 9 && sysarg_args_submap < 20)
    sysarg_args_map = 1;
  if (sysarg_args_submap >= 20 && sysarg_args_submap < 38)
    sysarg_args_map = 2;
  if (sysarg_args_submap >= 38)
    sysarg_args_map = 3;
  if (sysarg_args_submap == 9 ||
      sysarg_args_submap == 20 ||
      sysarg_args_submap == 38)
    sysarg_args_submap = 0;

}

/* eof */





