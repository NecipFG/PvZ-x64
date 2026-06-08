import sys
import re

def rewrite_init(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    new_init = "void SexyAppBase::Init()\n"
    new_init += "{\n"
    new_init += "\tFILE* fLog = fopen(\"framework_init_log.txt\", \"w\");\n"
    new_init += "\tif (fLog) { fprintf(fLog, \"SexyAppBase::Init started\\n\"); fflush(fLog); }\n"
    new_init += "\tmPrimaryThreadId = GetCurrentThreadId();\n"
    new_init += "\tif (mShutdown) return;\n"
    new_init += "\tif (fLog) { fprintf(fLog, \"Checking DLLs\\n\"); fflush(fLog); }\n"
    new_init += "\tif (!mCmdLineParsed) DoParseCmdLine();\n"
    new_init += "\tif (gHInstance==NULL) gHInstance = (HINSTANCE)GetModuleHandle(NULL);\n"
    new_init += "\tif (!ChangeDirHook(mChangeDirTo.c_str())) chdir(mChangeDirTo.c_str());\n"
    new_init += "\tgPakInterface->AddPakFile(\"main.pak\");\n"
    new_init += "\tmMutex = NULL;\n"
    new_init += "\tmRandSeed = GetTickCount();\n"
    new_init += "\tSRand(mRandSeed);\n"
    new_init += "\tsrand(GetTickCount());\n"
    new_init += "\tif (CheckFor98Mill())\n"
    new_init += "\t{\n"
    new_init += "\t\tmIsWideWindow = false;\n"
    new_init += "\t\tWNDCLASSA wc; memset(&wc, 0, sizeof(wc)); wc.lpfnWndProc = WindowProc; wc.hInstance = gHInstance; wc.lpszClassName = \"MainWindow\"; RegisterClassA(&wc);\n"
    new_init += "\t\twc.lpszClassName = \"InvisWindow\"; RegisterClassA(&wc);\n"
    new_init += "\t\tmInvisHWnd = CreateWindowExA(0, \"InvisWindow\", \"Invis\", 0, 0, 0, 0, 0, NULL, NULL, gHInstance, NULL);\n"
    new_init += "\t}\n"
    new_init += "\telse\n"
    new_init += "\t{\n"
    new_init += "\t\tmIsWideWindow = sizeof(SexyChar) == sizeof(wchar_t);\n"
    new_init += "\t\tWNDCLASSW wc; memset(&wc, 0, sizeof(wc)); wc.lpfnWndProc = WindowProc; wc.hInstance = gHInstance; wc.lpszClassName = L\"MainWindow\"; RegisterClassW(&wc);\n"
    new_init += "\t\twc.lpszClassName = L\"InvisWindow\"; RegisterClassW(&wc);\n"
    new_init += "\t\tmInvisHWnd = CreateWindowExW(0, L\"InvisWindow\", L\"Invis\", 0, 0, 0, 0, 0, NULL, NULL, gHInstance, NULL);\n"
    new_init += "\t}\n"
    new_init += "\tif (mInvisHWnd) SetWindowLongPtr(mInvisHWnd, GWLP_USERDATA, (LONG_PTR) this);\n"
    new_init += "\tif (mSoundManager == NULL) mSoundManager = new DSoundManager(mNoSoundNeeded?NULL:mHWnd, mWantFMod);\n"
    new_init += "\tmMusicInterface = CreateMusicInterface(mInvisHWnd);\n"
    new_init += "\tInitHook();\n"
    new_init += "\tmInitialized = true;\n"
    new_init += "\tif (fLog) { fprintf(fLog, \"SexyAppBase::Init SUCCESS\\n\"); fclose(fLog); }\n"
    new_init += "}"

    pattern = re.compile(r'void SexyAppBase::Init\(\).*?\n}', re.DOTALL | re.MULTILINE)
    new_content = pattern.sub(new_init, content)
    
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(new_content)

rewrite_init(sys.argv[1])
