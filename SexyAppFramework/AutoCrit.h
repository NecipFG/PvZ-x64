#ifndef __AUTOCRIT_INCLUDED__
#define __AUTOCRIT_INCLUDED__

#include "Common.h"
#include "CritSect.h"
#include <mutex>

namespace Sexy
{

class AutoCrit
{
	std::recursive_mutex*	mMutex;
public:
	AutoCrit(std::recursive_mutex* theMutex) : 
		mMutex(theMutex)
	{ 
		if (mMutex) mMutex->lock();
	}

	AutoCrit(std::recursive_mutex& theMutex) : 
		mMutex(&theMutex)
	{ 
		mMutex->lock();
	}

	AutoCrit(CritSect* theCritSect) : 
		mMutex(theCritSect ? &theCritSect->mMutex : nullptr)
	{ 
		if (mMutex) mMutex->lock();
	}

	AutoCrit(const CritSect& theCritSect) : 
		mMutex(&theCritSect.mMutex)
	{ 
		mMutex->lock();
	}

	~AutoCrit()
	{ 
		if (mMutex) mMutex->unlock();
	}
};

}

#endif //__AUTOCRIT_INCLUDED__