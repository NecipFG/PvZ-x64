// _DEBUG is controlled by CMake: -DCMAKE_BUILD_TYPE=Debug sets it, Release sets NDEBUG
#include "LawnApp.h"
#include "Resources.h"
#include "Sexy.TodLib/TodStringFile.h"

using namespace Sexy;

static_assert(sizeof(void*) == 8, "Target build environment is not configured for 64-bit execution!");
static_assert(sizeof(DWORD) == 4, "Portability Failure: DWORD must be exactly 4 bytes!");
static_assert(sizeof(ULONG) == 4, "Portability Failure: ULONG must be exactly 4 bytes!");
static_assert(sizeof(LONG) == 4, "Portability Failure: LONG must be exactly 4 bytes!");


bool (*gAppCloseRequest)();				//[0x69E6A0]
SexyString (*gGetCurrentLevelName)();

#ifdef _WIN32
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
#ifndef _WIN32
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	extern std::string gCommandLineStr;
	gCommandLineStr = "";
	for (int i = 0; i < argc; ++i) {
		if (i > 0) gCommandLineStr += " ";
		std::string arg = argv[i];
		if (arg.find(' ') != std::string::npos) {
			gCommandLineStr += "\"" + arg + "\"";
		} else {
			gCommandLineStr += arg;
		}
	}
#endif
	//gHInstance = hInstance;
#ifdef _DEBUG
	// FILE* fDummy;
	// freopen_s(&fDummy, "CONIN$", "r", stdin);
	// freopen_s(&fDummy, "CONOUT$", "w", stderr);
	// freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	TodStringListSetColors(gLawnStringFormats, gLawnStringFormatCount);
	gGetCurrentLevelName = LawnGetCurrentLevelName;
	gAppCloseRequest = LawnGetCloseRequest;

	gLawnApp = new LawnApp();
	if (Sexy::FileExists("properties\\resources.xml"))
		gLawnApp->mChangeDirTo = ".";
	else if (Sexy::FileExists("..\\properties\\resources.xml"))
		gLawnApp->mChangeDirTo = "..";
	else if (Sexy::FileExists("Debug\\properties\\resources.xml"))
		gLawnApp->mChangeDirTo = "Debug";
	else if (Sexy::FileExists("build\\properties\\resources.xml"))
		gLawnApp->mChangeDirTo = "build";
	else
		gLawnApp->mChangeDirTo = ".";
	
	gLawnApp->Init();
	gLawnApp->Start();

	gLawnApp->Shutdown();
	if (gLawnApp)
		delete gLawnApp;

	return 0;
};