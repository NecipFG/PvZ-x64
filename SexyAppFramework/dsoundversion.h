#ifndef __DSOUNDVERSION_H__
#define __DSOUNDVERSION_H__

#ifdef _WIN32
#define DIRECTSOUND_VERSION 0x0600
#include <dsound.h>
#else
typedef void* LPDIRECTSOUND;
typedef void* LPDIRECTSOUNDBUFFER;
#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME {
    unsigned int dwLowDateTime;
    unsigned int dwHighDateTime;
} FILETIME;
#endif
#endif

#endif