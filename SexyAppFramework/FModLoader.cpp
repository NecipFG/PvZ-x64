#include "FModLoader.h"
#include <stdlib.h>

using namespace Sexy;

FMOD_INSTANCE* Sexy::gFMod = NULL;
static long gFModLoadCount = 0;

#ifdef _WIN32
static void CheckFModFunction(unsigned int theFunc, const char *theName)
{
	if (theFunc==0)
	{
		char aBuf[1024];
		sprintf(aBuf,"%s function not found in fmod.dll",theName);
		MessageBoxA(NULL,aBuf,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

FMOD_INSTANCE::FMOD_INSTANCE(const char *dllName)
{
    mModule = LoadLibrary(dllName);
    if (!mModule)
		return;

    #define GETPROC(_x, _y)                                                                       \
    {                                                                                             \
		CheckFModFunction(*((unsigned int *)&_x) = (unsigned int)GetProcAddress(mModule, _y),#_y);    \
    }

    GETPROC(FSOUND_SetBufferSize, "_FSOUND_SetBufferSize@4");
    GETPROC(FSOUND_SetHWND, "_FSOUND_SetHWND@4");
    GETPROC(FSOUND_Init, "_FSOUND_Init@12");
    GETPROC(FSOUND_Close, "_FSOUND_Close@0");

	GETPROC(FSOUND_Sample_Load, "_FSOUND_Sample_Load@16");
    GETPROC(FSOUND_Sample_Alloc, "_FSOUND_Sample_Alloc@28");
    GETPROC(FSOUND_Sample_Free, "_FSOUND_Sample_Free@4");
    GETPROC(FSOUND_Sample_Lock, "_FSOUND_Sample_Lock@28");
    GETPROC(FSOUND_Sample_Unlock, "_FSOUND_Sample_Unlock@20");
    GETPROC(FSOUND_Sample_SetLoopPoints, "_FSOUND_Sample_SetLoopPoints@12");
    GETPROC(FSOUND_Sample_GetLength, "_FSOUND_Sample_GetLength@4");
    GETPROC(FSOUND_Sample_GetDefaults, "_FSOUND_Sample_GetDefaults@20");
    GETPROC(FSOUND_Sample_GetMode, "_FSOUND_Sample_GetMode@4");

	GETPROC(FSOUND_PlaySound, "_FSOUND_PlaySound@8");
    GETPROC(FSOUND_StopSound, "_FSOUND_StopSound@4");
    GETPROC(FSOUND_SetVolume, "_FSOUND_SetVolume@8");
    GETPROC(FSOUND_SetPaused, "_FSOUND_SetPaused@8");
    GETPROC(FSOUND_SetLoopMode, "_FSOUND_SetLoopMode@8");
    GETPROC(FSOUND_IsPlaying, "_FSOUND_IsPlaying@4");

	GETPROC(FSOUND_Stream_OpenFile, "_FSOUND_Stream_OpenFile@12");
    GETPROC(FSOUND_Stream_Close, "_FSOUND_Stream_Close@4");
    GETPROC(FSOUND_Stream_Play, "_FSOUND_Stream_Play@8");

	GETPROC(FMUSIC_LoadSong, "_FMUSIC_LoadSong@4");
    GETPROC(FMUSIC_PlaySong, "_FMUSIC_PlaySong@4");
    GETPROC(FMUSIC_StopSong, "_FMUSIC_StopSong@4");
    GETPROC(FMUSIC_SetLooping, "_FMUSIC_SetLooping@8");
    GETPROC(FMUSIC_SetOrder, "_FMUSIC_SetOrder@8");
    GETPROC(FMUSIC_SetPaused, "_FMUSIC_SetPaused@8");
    GETPROC(FMUSIC_SetMasterVolume, "_FMUSIC_SetMasterVolume@8");
    GETPROC(FMUSIC_IsPlaying, "_FMUSIC_IsPlaying@4");

#undef GETPROC
}

FMOD_INSTANCE::~FMOD_INSTANCE()
{
	if (mModule)
		FreeLibrary(mModule);
}

void Sexy::LoadFModDLL()
{
	InterlockedIncrement(&gFModLoadCount);
	if (gFMod!=NULL)
		return;

	gFMod = new FMOD_INSTANCE("fmod.dll");
	if (gFMod->mModule==NULL)
	{
		MessageBoxA(NULL,"Can't find fmod.dll." ,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

void Sexy::FreeFModDLL()
{
	if (gFMod!=NULL)
	{
		if (InterlockedDecrement(&gFModLoadCount) <= 0)
		{
			delete gFMod;
			gFMod = NULL;
		}
	}
}
#else
FMOD_INSTANCE::FMOD_INSTANCE(const char *dllName)
{
	mModule = NULL;
}
FMOD_INSTANCE::~FMOD_INSTANCE()
{
}
void Sexy::LoadFModDLL()
{
}
void Sexy::FreeFModDLL()
{
}
#endif
