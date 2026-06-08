#ifndef _H_CritSect
#define _H_CritSect

#include "Common.h"
#include <mutex>

class CritSync;

namespace Sexy
{

class CritSect 
{
private:
	mutable std::recursive_mutex mMutex;
	friend class AutoCrit;

public:
	CritSect(void);
	~CritSect(void);
};

}

#endif // _H_CritSect
