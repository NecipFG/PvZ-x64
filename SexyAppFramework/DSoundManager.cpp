#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "DSoundManager.h"
#include "DSoundInstance.h"
#include "MASoundCommon.h"
#include <cmath>
#include <mutex>
#include "../PakLib/PakInterface.h"

#define USE_OGG_LIB
#ifdef USE_OGG_LIB
#include "ogg/ivorbiscodec.h"
#include "ogg/ivorbisfile.h"

static int p_fseek64_wrap(PFILE *f, ogg_int64_t off, int whence) {
	if (f == NULL) return -1;
	return p_fseek(f, (long)off, whence);
}

static int ov_pak_open(PFILE *f, OggVorbis_File *vf, char *initial, long ibytes) {
	ov_callbacks callbacks = {
		(size_t (*)(void *, size_t, size_t, void *))  p_fread,
		(int (*)(void *, ogg_int64_t, int))             p_fseek64_wrap,
		(int (*)(void *))                             p_fclose,
		(long (*)(void *))                            p_ftell
	};
	return ov_open_callbacks((void *)f, vf, initial, ibytes, callbacks);
}
#endif

using namespace Sexy;

// Miniaudio mixer callback
void DSoundManager::AudioCallback(void* pDeviceVoid, void* pOutput, const void* pInput, unsigned int frameCount)
{
	ma_device* pDevice = (ma_device*)pDeviceVoid;
	DSoundManager* pManager = (DSoundManager*)pDevice->pUserData;
	if (!pManager) return;
	std::lock_guard<std::recursive_mutex> aLock(pManager->mAudioMutex);
	float* pOut = (float*)pOutput;

	// Clear mixed output buffer
	memset(pOut, 0, frameCount * 2 * sizeof(float));

	// Mix all active channels
	for (int i = 0; i < MAX_CHANNELS; ++i)
	{
		DSoundInstance* pInst = pManager->mPlayingSounds[i];
		if (!pInst || pInst->IsReleased()) continue;

		MASoundInstanceData* pData = (MASoundInstanceData*)pInst->mSoundBuffer;
		MASourceSound* pSource = (MASourceSound*)pInst->mSourceSoundBuffer;
		if (!pSource || !pData->mPlaying) continue;

		size_t lenFrames = pSource->mSamples.size() / pSource->mChannels;
		if (lenFrames == 0) continue;

		double step = (double)pSource->mSampleRate / 44100.0 * pData->mPitch;
		double pan = pData->mPan / 10000.0;
		if (pan < -1.0) pan = -1.0;
		if (pan > 1.0) pan = 1.0;

		float panLeft = 1.0f;
		float panRight = 1.0f;
		if (pan > 0.0) panLeft = 1.0f - (float)pan;
		if (pan < 0.0) panRight = 1.0f + (float)pan;

		float vol = (float)(pData->mVolume * pManager->mMasterVolume);

		for (unsigned int f = 0; f < frameCount; ++f)
		{
			double pos = pData->mPlayCursor;
			if (pos >= lenFrames)
			{
				if (pData->mLooping)
				{
					pos = fmod(pos, (double)lenFrames);
					pData->mPlayCursor = pos;
				}
				else
				{
					pData->mPlaying = false;
					pData->mPlayCursor = 0.0;
					if (pInst->mAutoRelease)
					{
						pInst->mReleased = true;
					}
					break;
				}
			}

			float left = 0, right = 0;
			size_t idx1 = (size_t)pos;
			size_t idx2 = idx1 + 1;
			if (idx2 >= lenFrames) idx2 = idx1;
			double frac = pos - idx1;

			if (pSource->mChannels == 1)
			{
				float s1 = pSource->mSamples[idx1] / 32768.0f;
				float s2 = pSource->mSamples[idx2] / 32768.0f;
				float s = s1 + (float)frac * (s2 - s1);
				left = s;
				right = s;
			}
			else
			{
				float l1 = pSource->mSamples[idx1 * 2 + 0] / 32768.0f;
				float l2 = pSource->mSamples[idx2 * 2 + 0] / 32768.0f;
				left = l1 + (float)frac * (l2 - l1);

				float r1 = pSource->mSamples[idx1 * 2 + 1] / 32768.0f;
				float r2 = pSource->mSamples[idx2 * 2 + 1] / 32768.0f;
				right = r1 + (float)frac * (r2 - r1);
			}

			pOut[f * 2 + 0] += left * vol * panLeft;
			pOut[f * 2 + 1] += right * vol * panRight;

			pData->mPlayCursor += step;
		}
	}

	// Clamp output to [-1.0f, 1.0f]
	for (unsigned int f = 0; f < frameCount * 2; ++f)
	{
		if (pOut[f] > 1.0f) pOut[f] = 1.0f;
		else if (pOut[f] < -1.0f) pOut[f] = -1.0f;
	}
}

DSoundManager::DSoundManager(HWND theHWnd, bool haveFMod)
{
	mHaveFMod = false;
	mLastReleaseTick = 0;
	mPrimaryBuffer = NULL;

	int i;
	for (i = 0; i < MAX_SOURCE_SOUNDS; i++)
	{
		mSourceSounds[i] = NULL;
		mBaseVolumes[i] = 1.0;
		mBasePans[i] = 0;
	}

	for (i = 0; i < MAX_CHANNELS; i++)
		mPlayingSounds[i] = NULL;

	mDirectSound = NULL;
	mMasterVolume = 1.0;

	// Initialize Miniaudio Device
	ma_device* pDevice = new ma_device();
	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.playback.format = ma_format_f32;
	config.playback.channels = 2;
	config.sampleRate = 44100;
	config.dataCallback = (ma_device_data_proc)AudioCallback;
	config.pUserData = this;

	if (ma_device_init(NULL, &config, pDevice) == MA_SUCCESS)
	{
		mDirectSound = (LPDIRECTSOUND)pDevice;
		ma_device_start(pDevice);
	}
	else
	{
		delete pDevice;
	}
}

DSoundManager::~DSoundManager()
{
	ReleaseChannels();
	ReleaseSounds();

	ma_device* pDevice = (ma_device*)mDirectSound;
	if (pDevice)
	{
		ma_device_uninit(pDevice);
		delete pDevice;
		mDirectSound = NULL;
	}
}

int DSoundManager::FindFreeChannel()
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	DWORD aTick = GetTickCount();
	if (aTick - mLastReleaseTick > 1000)
	{
		ReleaseFreeChannels();
		mLastReleaseTick = aTick;
	}

	for (int i = 0; i < MAX_CHANNELS; i++)
	{		
		if (mPlayingSounds[i] == NULL)
			return i;
		
		if (mPlayingSounds[i]->IsReleased())
		{
			delete mPlayingSounds[i];
			mPlayingSounds[i] = NULL;
			return i;
		}
	}
	return -1;
}

bool DSoundManager::Initialized()
{
	return (mDirectSound != NULL);
}

int DSoundManager::VolumeToDB(double theVolume)
{
	return 0; // Not used by miniaudio
}

void DSoundManager::SetVolume(double theVolume)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	mMasterVolume = theVolume;
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		if (mPlayingSounds[i] != NULL)
			mPlayingSounds[i]->RehupVolume();
	}
}

bool DSoundManager::LoadWAVSound(unsigned int theSfxID, const std::string& theFilename)
{
	PFILE* fp = p_fopen(theFilename.c_str(), "rb");
	if (fp == NULL)
		return false;

	char riff[4];
	if (p_fread(riff, 1, 4, fp) != 4 || strncmp(riff, "RIFF", 4) != 0)
	{
		p_fclose(fp);
		return false;
	}
	
	ulong riffSize;
	p_fread(&riffSize, 4, 1, fp);
	
	char wave[4];
	if (p_fread(wave, 1, 4, fp) != 4 || strncmp(wave, "WAVE", 4) != 0)
	{
		p_fclose(fp);
		return false;
	}

	uchar anXor = 0;
	std::vector<char> formatChunk;
	std::vector<char> dataChunk;

	while (!p_feof(fp))
	{
		char aChunkType[4];
		if (p_fread(aChunkType, 1, 4, fp) != 4)
			break;
			
		ulong aChunkSize;
		if (p_fread(&aChunkSize, 4, 1, fp) != 1)
			break;

		int aCurPos = p_ftell(fp);

		if (strncmp(aChunkType, "fmt ", 4) == 0)
		{
			formatChunk.resize(aChunkSize + 8);
			memcpy(formatChunk.data(), "fmt ", 4);
			memcpy(formatChunk.data() + 4, &aChunkSize, 4);
			p_fread(formatChunk.data() + 8, 1, aChunkSize, fp);
		}
		else if (strncmp(aChunkType, "xor ", 4) == 0)
		{
			p_fread(&anXor, 1, 1, fp);
		}
		else if (strncmp(aChunkType, "data", 4) == 0)
		{
			dataChunk.resize(aChunkSize);
			p_fread(dataChunk.data(), 1, aChunkSize, fp);
		}

		p_fseek(fp, aCurPos + aChunkSize, SEEK_SET);
	}
	p_fclose(fp);

	if (dataChunk.empty() || formatChunk.empty())
		return false;

	if (anXor != 0)
	{
		for (size_t i = 0; i < dataChunk.size(); i++)
			dataChunk[i] ^= anXor;
	}

	std::vector<char> wavFile;
	ulong wavSize = 4 + formatChunk.size() + 8 + dataChunk.size();
	wavFile.resize(8 + wavSize);
	memcpy(wavFile.data(), "RIFF", 4);
	memcpy(wavFile.data() + 4, &wavSize, 4);
	memcpy(wavFile.data() + 8, "WAVE", 4);
	memcpy(wavFile.data() + 12, formatChunk.data(), formatChunk.size());
	memcpy(wavFile.data() + 12 + formatChunk.size(), "data", 4);
	ulong dataSize = dataChunk.size();
	memcpy(wavFile.data() + 16 + formatChunk.size(), &dataSize, 4);
	memcpy(wavFile.data() + 20 + formatChunk.size(), dataChunk.data(), dataChunk.size());

	ma_decoder decoder;
	ma_decoder_config config = ma_decoder_config_init(ma_format_s16, 0, 0);
	if (ma_decoder_init_memory(wavFile.data(), wavFile.size(), &config, &decoder) != MA_SUCCESS)
	{
		return false;
	}

	ma_uint64 totalFrames = 0;
	ma_decoder_get_length_in_pcm_frames(&decoder, &totalFrames);

	MASourceSound* pSource = new MASourceSound();
	pSource->mChannels = decoder.outputChannels;
	pSource->mSampleRate = decoder.outputSampleRate;
	pSource->mFilename = theFilename;
	pSource->mSamples.resize(totalFrames * pSource->mChannels);

	ma_decoder_read_pcm_frames(&decoder, pSource->mSamples.data(), totalFrames, NULL);
	ma_decoder_uninit(&decoder);

	mSourceSounds[theSfxID] = (LPDIRECTSOUNDBUFFER)pSource;
	mSourceDataSizes[theSfxID] = pSource->mSamples.size() * 2;
	return true;
}

bool DSoundManager::LoadFModSound(unsigned int theSfxID, const std::string& theFilename)
{
	return false;
}

bool DSoundManager::LoadOGGSound(unsigned int theSfxID, const std::string& theFilename)
{
#ifdef USE_OGG_LIB
	OggVorbis_File vf;
	int current_section;

	PFILE *aFile = p_fopen(theFilename.c_str(), "rb");
	if (aFile == NULL)
		return false;

	if (ov_pak_open(aFile, &vf, NULL, 0) < 0) 
	{
		p_fclose(aFile);
		return false;
	}
  
	vorbis_info *anInfo = ov_info(&vf, -1);
	int aLenBytes = (int) (ov_pcm_total(&vf, -1) * anInfo->channels * 2);

	MASourceSound* pSource = new MASourceSound();
	pSource->mChannels = anInfo->channels;
	pSource->mSampleRate = anInfo->rate;
	pSource->mFilename = theFilename;
	pSource->mSamples.resize(aLenBytes / 2);

	char *aPtr = (char*)pSource->mSamples.data();
	int aNumBytes = aLenBytes;
	while (aNumBytes > 0)
	{		
		long ret = ov_read(&vf, aPtr, aNumBytes, &current_section);
		if (ret <= 0) 
			break;
		else 
		{
			aPtr += ret;
			aNumBytes -= ret;
		}
	}

	ov_clear(&vf);
	if (aNumBytes != 0)
	{
		delete pSource;
		return false;
	}

	mSourceSounds[theSfxID] = (LPDIRECTSOUNDBUFFER)pSource;
	mSourceDataSizes[theSfxID] = aLenBytes;
	return true;
#else
	return false;
#endif
}

bool DSoundManager::LoadAUSound(unsigned int theSfxID, const std::string& theFilename)
{
	PFILE* fp = p_fopen(theFilename.c_str(), "rb");
	if (fp == NULL)
		return false;

	char aHeaderId[4];
	p_fread(aHeaderId, 1, 4, fp);
	if (strncmp(aHeaderId, ".snd", 4) != 0)
	{
		p_fclose(fp);
		return false;
	}

	ulong aHeaderSize;
	p_fread(&aHeaderSize, 4, 1, fp);
	aHeaderSize = LONG_BIGE_TO_NATIVE(aHeaderSize);

	ulong aDataSize;
	p_fread(&aDataSize, 4, 1, fp);
	aDataSize = LONG_BIGE_TO_NATIVE(aDataSize);

	ulong anEncoding;
	p_fread(&anEncoding, 4, 1, fp);
	anEncoding = LONG_BIGE_TO_NATIVE(anEncoding);

	ulong aSampleRate;
	p_fread(&aSampleRate, 4, 1, fp);
	aSampleRate = LONG_BIGE_TO_NATIVE(aSampleRate);

	ulong aChannelCount;
	p_fread(&aChannelCount, 4, 1, fp);
	aChannelCount = LONG_BIGE_TO_NATIVE(aChannelCount);

	p_fseek(fp, aHeaderSize, SEEK_SET);

	bool ulaw = false;
	ulong aSrcBitCount = 8;
	ulong aBitCount = 16;
	switch (anEncoding)
	{
	case 1:
		aSrcBitCount = 8;
		aBitCount = 16;
		ulaw = true;
		break;
	case 2:
		aSrcBitCount = 8;
		aBitCount = 8;
		break;
	default:
		p_fclose(fp);
		return false;
	}

	ulong aDestSize = aDataSize * aBitCount / aSrcBitCount;

	MASourceSound* pSource = new MASourceSound();
	pSource->mChannels = aChannelCount;
	pSource->mSampleRate = aSampleRate;
	pSource->mFilename = theFilename;
	int numSamples = aDestSize / 2;
	pSource->mSamples.resize(numSamples);

	uchar* aSrcBuffer = new uchar[aDataSize];
	int aReadSize = p_fread(aSrcBuffer, 1, aDataSize, fp);
	p_fclose(fp);

	if (ulaw)
	{
		short* aDestBuffer = pSource->mSamples.data();
		for (ulong i = 0; i < aDataSize; i++)
		{
			int ch = aSrcBuffer[i];
			int sign = (ch < 128) ? -1 : 1;
			ch = ch | 0x80;
			if (ch > 239)
				ch = ((0xF0 | 15) - ch) * 2;
			else if (ch > 223)
				ch = (((0xE0 | 15) - ch) * 4) + 32;
			else if (ch > 207)
				ch = (((0xD0 | 15) - ch) * 8) + 96;
			else if (ch > 191)
				ch = (((0xC0 | 15) - ch) * 16) + 224;
			else if (ch > 175)
				ch = (((0xB0 | 15) - ch) * 32) + 480;
			else if (ch > 159)
				ch = (((0xA0 | 15) - ch) * 64) + 992;
			else if (ch > 143)
				ch = (((0x90 | 15) - ch) * 128) + 2016;
			else if (ch > 128)
				ch = (((0x80 | 15) - ch) * 256) + 4064;
			else
				ch = 0xff;

			aDestBuffer[i] = sign * ch * 4;
		}
	}
	else
	{
		memcpy(pSource->mSamples.data(), aSrcBuffer, aDataSize);
	}

	delete[] aSrcBuffer;
	mSourceSounds[theSfxID] = (LPDIRECTSOUNDBUFFER)pSource;
	mSourceDataSizes[theSfxID] = aDestSize;
	return true;
}

bool DSoundManager::LoadSound(unsigned int theSfxID, const std::string& theFilename)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	if (theSfxID >= MAX_SOURCE_SOUNDS)
		return false;

	ReleaseSound(theSfxID);

	mSourceFileNames[theSfxID] = theFilename;
	std::string aFilename = theFilename;

	if (LoadWAVSound(theSfxID, aFilename + ".wav"))	
		return true;

	if (LoadOGGSound(theSfxID, aFilename + ".ogg"))
		return true;

	if (LoadAUSound(theSfxID, aFilename + ".au"))
		return true;

	return false;
}

int DSoundManager::LoadSound(const std::string& theFilename)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	int i;
	for (i = 0; i < MAX_SOURCE_SOUNDS; i++)
	{
		if (mSourceFileNames[i] == theFilename)
			return i;
	}

	for (i = MAX_SOURCE_SOUNDS - 1; i >= 0; i--)
	{		
		if (mSourceSounds[i] == NULL)
		{
			if (!LoadSound(i, theFilename))
				return -1;
			else
				return i;
		}
	}	

	return -1;
}

void DSoundManager::ReleaseSound(unsigned int theSfxID)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	if (theSfxID < MAX_SOURCE_SOUNDS && mSourceSounds[theSfxID] != NULL)
	{
		delete (MASourceSound*)mSourceSounds[theSfxID];
		mSourceSounds[theSfxID] = NULL;
		mSourceFileNames[theSfxID] = "";
	}
}

int DSoundManager::GetFreeSoundId()
{
	for (int i = 0; i < MAX_SOURCE_SOUNDS; i++)
	{
		if (mSourceSounds[i] == NULL)
			return i;
	}
	return -1;
}

int DSoundManager::GetNumSounds()
{
	int aCount = 0;
	for (int i = 0; i < MAX_SOURCE_SOUNDS; i++)
	{
		if (mSourceSounds[i] != NULL)
			aCount++;
	}
	return aCount;
}

bool DSoundManager::SetBaseVolume(unsigned int theSfxID, double theBaseVolume)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	if (theSfxID >= MAX_SOURCE_SOUNDS)
		return false;

	mBaseVolumes[theSfxID] = theBaseVolume;
	return true;
}

bool DSoundManager::SetBasePan(unsigned int theSfxID, int theBasePan)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	if (theSfxID >= MAX_SOURCE_SOUNDS)
		return false;

	mBasePans[theSfxID] = theBasePan;
	return true;
}

bool DSoundManager::GetTheFileTime(const std::string& theDepFile, FILETIME* theFileTime)
{	
	return false;
}

bool DSoundManager::WriteWAV(unsigned int theSfxID, const std::string& theFilename, const std::string& theDepFile)
{
	return true;
}

SoundInstance* DSoundManager::GetSoundInstance(unsigned int theSfxID)
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	if (theSfxID >= MAX_SOURCE_SOUNDS)
		return NULL;

	int aFreeChannel = FindFreeChannel();
	if (aFreeChannel < 0)
		return NULL;

	if (mDirectSound == NULL)
	{
		mPlayingSounds[aFreeChannel] = new DSoundInstance(this, NULL);
	}
	else
	{
		if (mSourceSounds[theSfxID] == NULL)
			return NULL;

		mPlayingSounds[aFreeChannel] = new DSoundInstance(this, mSourceSounds[theSfxID]);
	}

	mPlayingSounds[aFreeChannel]->SetBasePan(mBasePans[theSfxID]);
	mPlayingSounds[aFreeChannel]->SetBaseVolume(mBaseVolumes[theSfxID]);

	return mPlayingSounds[aFreeChannel];
}

void DSoundManager::ReleaseSounds()
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	for (int i = 0; i < MAX_SOURCE_SOUNDS; i++)
	{
		if (mSourceSounds[i] != NULL)
		{
			delete (MASourceSound*)mSourceSounds[i];
			mSourceSounds[i] = NULL;
		}
	}
}

void DSoundManager::ReleaseChannels()
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		if (mPlayingSounds[i] != NULL)
		{
			delete mPlayingSounds[i];
			mPlayingSounds[i] = NULL;
		}
	}
}

void DSoundManager::ReleaseFreeChannels()
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		if (mPlayingSounds[i] != NULL && mPlayingSounds[i]->IsReleased())
		{
			delete mPlayingSounds[i];
			mPlayingSounds[i] = NULL;
		}
	}
}

void DSoundManager::StopAllSounds()
{
	std::lock_guard<std::recursive_mutex> aLock(mAudioMutex);
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		if (mPlayingSounds[i] != NULL)
		{
			bool isAutoRelease = mPlayingSounds[i]->mAutoRelease;
			mPlayingSounds[i]->Stop();
			mPlayingSounds[i]->mAutoRelease = isAutoRelease;
		}
	}
}

double DSoundManager::GetMasterVolume()
{
	return mMasterVolume;
}

void DSoundManager::SetMasterVolume(double theVolume)
{
	mMasterVolume = theVolume;
	SetVolume(theVolume);
}

void DSoundManager::Flush()
{
}

void DSoundManager::SetCooperativeWindow(HWND theHWnd, bool isWindowed)
{
}
