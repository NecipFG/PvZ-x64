#include "DSoundInstance.h"
#include "DSoundManager.h"
#include "MASoundCommon.h"
#include <cmath>
#include <mutex>

using namespace Sexy;

DSoundInstance::DSoundInstance(DSoundManager* theSoundManager, LPDIRECTSOUNDBUFFER theSourceSound)
{
	mSoundManagerP = theSoundManager;
	mReleased = false;
	mAutoRelease = false;
	mHasPlayed = false;
	mSourceSoundBuffer = theSourceSound;
	mSoundBuffer = NULL;

	MASoundInstanceData* pData = new MASoundInstanceData();
	pData->mPlayCursor = 0.0;
	pData->mLooping = false;
	pData->mPlaying = false;
	pData->mReleased = false;
	pData->mVolume = 1.0;
	pData->mPan = 0.0;
	pData->mPitch = 1.0;
	mSoundBuffer = (LPDIRECTSOUNDBUFFER)pData;

	mBaseVolume = 1.0;
	mBasePan = 0;

	mVolume = 1.0;
	mPan = 0;

	mDefaultFrequency = 44100;

	if (mSourceSoundBuffer != NULL)
	{
		MASourceSound* pSource = (MASourceSound*)mSourceSoundBuffer;
		mDefaultFrequency = pSource->mSampleRate;
	}

	RehupVolume();
}

DSoundInstance::~DSoundInstance()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData != NULL)
	{
		delete pData;
		mSoundBuffer = NULL;
	}
}

void DSoundInstance::RehupVolume()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData != NULL)
	{
		pData->mVolume = mBaseVolume * mVolume;
	}
}

void DSoundInstance::RehupPan()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData != NULL)
	{
		pData->mPan = mBasePan + mPan;
	}
}

void DSoundInstance::Release()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	Stop();
	mReleased = true;
	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData != NULL)
	{
		pData->mReleased = true;
	}
}

void DSoundInstance::SetVolume(double theVolume) 
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	mVolume = theVolume;
	RehupVolume();	
}

void DSoundInstance::SetPan(int thePosition) 
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	mPan = thePosition;
	RehupPan();	
}

void DSoundInstance::SetBaseVolume(double theBaseVolume)
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	mBaseVolume = theBaseVolume;
	RehupVolume();
}

void DSoundInstance::SetBasePan(int theBasePan)
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	mBasePan = theBasePan;
	RehupPan();
}

bool DSoundInstance::Play(bool looping, bool autoRelease)
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	mHasPlayed = true;	
	mAutoRelease = autoRelease;	

	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData == NULL)
	{
		return false;
	}

	pData->mPlayCursor = 0.0;
	pData->mLooping = looping;
	pData->mPlaying = true;

	return true;
}

void DSoundInstance::Stop()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData != NULL)
	{
		pData->mPlaying = false;
		pData->mPlayCursor = 0.0;
		mAutoRelease = false;
	}
}

void DSoundInstance::AdjustPitch(double theNumSteps)
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData != NULL)
	{
		pData->mPitch = pow(1.0594630943592952645618252949463, theNumSteps);
	}
}

bool DSoundInstance::IsPlaying()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	if (!mHasPlayed)
		return false;

	MASoundInstanceData* pData = (MASoundInstanceData*)mSoundBuffer;
	if (pData == NULL)
		return false;

	return pData->mPlaying;
}

bool DSoundInstance::IsReleased()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	if ((!mReleased) && (mAutoRelease) && (mHasPlayed) && (!IsPlaying()))	
		Release();	

	return mReleased;
}

double DSoundInstance::GetVolume()
{
	std::lock_guard<std::recursive_mutex> aLock(mSoundManagerP->mAudioMutex);
	return mVolume; 
}
