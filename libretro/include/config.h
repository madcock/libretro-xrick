#ifdef __LIBRETRO__
#if defined(ANDROID) || defined(__PS3__)
#undef HAVE_POSIX_MEMALIGN
#else
#define HAVE_POSIX_MEMALIGN 1
#endif
#else
/* Define to 1 if you have the 'posix_memalign' function. */
#define HAVE_POSIX_MEMALIGN 1
#endif
