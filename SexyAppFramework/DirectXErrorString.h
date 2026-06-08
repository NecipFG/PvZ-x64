#ifndef __SEXY_DIRECTXERRORSTRING_H__
#define __SEXY_DIRECTXERRORSTRING_H__
#include <string>
#ifdef _WIN32
#include <ddraw.h>
#else
#include "Common.h"
#endif

namespace Sexy
{
	std::string GetDirectXErrorString(HRESULT theResult);
} // namespace 

#endif