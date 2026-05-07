#define _DEBUG
#include "LawnApp.h"
#include "Resources.h"
#include "Sexy.TodLib/TodStringFile.h"

using namespace Sexy;

bool (*gAppCloseRequest)();				//[0x69E6A0]
bool (*gAppHasUsedCheatKeys)();			//[0x69E6A4]
SexyString (*gGetCurrentLevelName)();

//0x44E8F0
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//gHInstance = hInstance;
#ifdef _DEBUG
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

	TodStringListSetColors(gLawnStringFormats, gLawnStringFormatCount);
	gGetCurrentLevelName = LawnGetCurrentLevelName;
	gAppCloseRequest = LawnGetCloseRequest;
	gAppHasUsedCheatKeys = LawnHasUsedCheatKeys;

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