#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <windows.h>
#include "SexyAppFramework/SexyAppBase.h"

using namespace Sexy;

int main() {
    SexyAppBase* app = (SexyAppBase*)0x1000000;
    std::cout << "mRandSeed: " << (uintptr_t)&app->mRandSeed - 0x1000000 << std::endl;
    std::cout << "mAppName: " << (uintptr_t)&app->mAppName - 0x1000000 << std::endl;
    std::cout << "mDDInterface: " << (uintptr_t)&app->mDDInterface - 0x1000000 << std::endl;
    std::cout << "mCritSect: " << (uintptr_t)&app->mCritSect - 0x1000000 << std::endl;
    return 0;
}
