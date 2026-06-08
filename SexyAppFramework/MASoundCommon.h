#ifndef __MASOUNDCOMMON_H__
#define __MASOUNDCOMMON_H__

#include <vector>
#include <string>

namespace Sexy
{

struct MASourceSound
{
	std::vector<short> mSamples;
	int mChannels;
	int mSampleRate;
	std::string mFilename;
};

struct MASoundInstanceData
{
	double mPlayCursor; // double for sub-sample interpolation & pitch shifting
	bool mLooping;
	bool mPlaying;
	bool mReleased;
	double mVolume;
	double mPan;
	double mPitch;
};

}

#endif // __MASOUNDCOMMON_H__
