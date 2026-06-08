#ifndef __SEXYAPPFRAMEWORK_COMMON_H__
#define __SEXYAPPFRAMEWORK_COMMON_H__

#pragma warning(disable:4786)
#pragma warning(disable:4503)

#undef _WIN32_WINNT
#undef WIN32_LEAN_AND_MEAN

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#undef _UNICODE
#undef UNICODE

#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h> 
#include <mmsystem.h>
#else
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <strings.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <climits>
#include <cstdarg>
#include <cctype>
#include <alloca.h>
#include <cmath>
#define _finite std::isfinite
#define _alloca alloca

inline char* strupr(char* s) {
    for (char* p = s; *p; ++p) *p = std::toupper((unsigned char)*p);
    return s;
}

typedef long long           __int64;
typedef void*               HCURSOR;
typedef void*               HICON;
typedef void*               LPEXCEPTION_POINTERS;
#define HKEY_CURRENT_USER ((HKEY)0)

#ifdef _WIN32
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        ULONG;
typedef long                LONG;
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;
typedef unsigned long long  QWORD;
typedef short               SHORT;
typedef unsigned short      USHORT;
typedef char                CHAR;
typedef wchar_t             WCHAR;
#else
typedef uint32_t            DWORD;
typedef int                 BOOL;
typedef uint8_t             BYTE;
typedef uint16_t            WORD;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef int                 INT;
typedef uint32_t            UINT;
typedef uint32_t            ULONG;
typedef int32_t             LONG;
typedef int64_t             LONGLONG;
typedef uint64_t            ULONGLONG;
typedef uint64_t            QWORD;
typedef short               SHORT;
typedef uint16_t            USHORT;
typedef char                CHAR;
typedef wchar_t             WCHAR;
#endif
typedef DWORD* LPDWORD;
typedef UINT* PUINT;
typedef void*               LPVOID;
typedef const void*         LPCVOID;
typedef void*               HANDLE;
typedef void*               HWND;
typedef void*               HDC;
typedef void*               HINSTANCE;
typedef void*               HMODULE;
typedef void*               HFONT;
typedef void*               HBRUSH;
typedef void*               HKEY;
typedef void*               HGLOBAL;
typedef DWORD               HFX;
typedef DWORD               HSYNC;
typedef DWORD               HPLUGIN;
typedef DWORD               HSAMPLE;
typedef DWORD               HCHANNEL;
typedef DWORD               HMUSIC;
typedef DWORD               HSTREAM;
typedef long long           __time64_t;
typedef int                 __time32_t;
typedef int                 HRESULT;
typedef const char*         LPCSTR;
typedef char*               LPSTR;
typedef const wchar_t*      LPCWSTR;
typedef wchar_t*            LPWSTR;
typedef uintptr_t           WPARAM;
typedef intptr_t            LPARAM;
typedef intptr_t            LRESULT;
typedef intptr_t            LONG_PTR;
typedef size_t              SIZE_T;
typedef const char*         LPCTSTR;
typedef char*               LPTSTR;
typedef void*               HBITMAP;
typedef void*               HMONITOR;
typedef void*               LPDIRECT3D7;
typedef void*               LPDIRECT3DDEVICE7;
typedef void*               LPDIRECTDRAWPALETTE;

#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

#define vsnwprintf vswprintf

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

inline bool operator==(const GUID& a, const GUID& b) {
    return memcmp(&a, &b, sizeof(GUID)) == 0;
}
inline bool operator!=(const GUID& a, const GUID& b) {
    return !(a == b);
}

#define ZeroMemory(p, s) memset((p), 0, (s))

typedef struct _DDSCAPS2 {
    DWORD dwCaps;
    DWORD dwCaps2;
    DWORD dwCaps3;
    DWORD dwCaps4;
} DDSCAPS2;

struct IDirectDraw {
    inline ULONG Release() { return 0; }
    inline HRESULT GetAvailableVidMem(void*, DWORD*, DWORD*) { return 0; }
    inline HRESULT FlipToGDISurface() { return 0; }
    inline HRESULT RestoreDisplayMode() { return 0; }
};
typedef IDirectDraw* LPDIRECTDRAW;
typedef IDirectDraw* LPDIRECTDRAW7;

struct IDirectDrawSurface {
    inline ULONG AddRef() { return 1; }
    inline ULONG Release() { return 0; }
    inline HRESULT Lock(void*, void*, DWORD, void*) { return 0; }
    inline HRESULT Unlock(void*) { return 0; }
    inline HRESULT GetSurfaceDesc(void*) { return 0; }
    inline HRESULT SetColorKey(DWORD, void*) { return 0; }
    inline HRESULT Blt(void*, void*, void*, DWORD, void*) { return 0; }
};
typedef IDirectDrawSurface* LPDIRECTDRAWSURFACE;
typedef IDirectDrawSurface* LPDIRECTDRAWSURFACE7;

typedef struct {
    DWORD dwColorSpaceLowValue;
    DWORD dwColorSpaceHighValue;
} DDCOLORKEY;

typedef struct {
    DWORD dwSize;
    DWORD dwFillColor;
} DDBLTFX;

#define DDSD_HEIGHT 0
#define DDSD_WIDTH 0
#define DDSD_CAPS 0
#define DDSD_PIXELFORMAT 0
#define DDLOCK_SURFACEMEMORYPTR 0
#define DDLOCK_WAIT 0
#define DDCKEY_SRCBLT 0
#define DDBLT_WAIT 0
#define DDBLT_KEYSRC 0
#define DDSCAPS_VIDEOMEMORY 0
#define DDSCAPS_SYSTEMMEMORY 0
#define DD_OK 0

#define CALLBACK
#define WINAPI
#define __stdcall
#define _cdecl
#define __cdecl
#define _In_
#define _In_opt_
#define TRUE 1
#define FALSE 0

#define WM_CLOSE 0x0010
#define WM_ACTIVATEAPP 0x001C
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_RBUTTONDBLCLK 0x0206
#define WM_MBUTTONDOWN 0x0207
#define WM_MBUTTONUP 0x0208
#define WM_MOUSEWHEEL 0x020A
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_CHAR 0x0102
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP 0x0105
#define WM_MOVE 0x0003
#define WM_SIZE 0x0005
#define WM_TIMER 0x0113
#define WM_SYSCOLORCHANGE 0x0015
#define WM_DISPLAYCHANGE 0x007E
#define WM_ENABLE 0x000A
#define WM_QUERYOPEN 0x0013
#define WM_SYSCHAR 0x0106
#define WM_NCLBUTTONDOWN 0x00A1
#define WM_SYSCOMMAND 0x0112
#define WM_DESTROY 0x0002
#define WM_SETCURSOR 0x0020
#define WM_ERASEBKGND 0x0014
#define WM_ENDSESSION 0x0016
#define WM_PAINT 0x000F

#define VK_RETURN 0x0D
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F8 0x77
#define VK_F10 0x79
#define VK_F11 0x7A

#define WS_CLIPCHILDREN 0
#define WS_MINIMIZEBOX 0
#define WS_MAXIMIZEBOX 0

#define SC_MONITORPOWER 0xF170
#define SC_SCREENSAVE 0xF140

#define MB_APPLMODAL 0
#define MB_ICONSTOP 0
#define MB_ICONWARNING 0
#define MB_YESNO 0x00000004L
#define MB_ICONQUESTION 0x00000020L
#define MB_ICONINFORMATION 0x00000040L
#define MB_ICONEXCLAMATION 0x00000030L
#define IDYES 6
#define IDNO 7

#define SPI_GETSCREENSAVETIMEOUT 0
#define SPI_GETSCREENSAVEACTIVE 0

#define SND_ASYNC 0
inline void PlaySoundA(const char*, HMODULE, DWORD) {}

inline void timeBeginPeriod(int) {}
inline void timeEndPeriod(int) {}

inline LRESULT DefWindowProcA(HWND, UINT, WPARAM, LPARAM) { return 0; }
inline LRESULT DefWindowProcW(HWND, UINT, WPARAM, LPARAM) { return 0; }
#ifdef UNICODE
#define DefWindowProc DefWindowProcW
#else
#define DefWindowProc DefWindowProcA
#endif

inline void SetCapture(HWND) {}

BOOL DeleteFileA(const char* filename);
inline BOOL EnumWindows(void*, LPARAM) { return TRUE; }
inline LONG ChangeDisplaySettings(void*, DWORD) { return 0; }

inline int MessageBoxA(HWND, const char* txt, const char* title, UINT flags) {
    fprintf(stderr, "MessageBoxA: [%s] %s\n", title, txt);
    return 1;
}
inline int MessageBoxW(HWND, const wchar_t* txt, const wchar_t* title, UINT flags) {
    fwprintf(stderr, L"MessageBoxW: [%ls] %ls\n", title, txt);
    return 1;
}
#ifdef UNICODE
#define MessageBox MessageBoxW
#else
#define MessageBox MessageBoxA
#endif

inline UINT GetWindowsDirectoryA(LPSTR lpBuffer, UINT uSize) {
    if (uSize > 1) {
        strcpy(lpBuffer, ".");
        return 1;
    }
    return 0;
}
inline UINT GetWindowsDirectoryW(LPWSTR lpBuffer, UINT uSize) {
    if (uSize > 1) {
        wcscpy(lpBuffer, L".");
        return 1;
    }
    return 0;
}
#ifdef UNICODE
#define GetWindowsDirectory GetWindowsDirectoryW
#else
#define GetWindowsDirectory GetWindowsDirectoryA
#endif

inline HMODULE LoadLibraryA(const char*) { return (HMODULE)0; }
#define LoadLibrary LoadLibraryA
#ifndef _T
#define _T(x) x
#endif
inline HMODULE GetModuleHandleA(const char*) { return (HMODULE)0; }
inline void FreeLibrary(HMODULE) {}
inline void* GetProcAddress(HMODULE, const char*) { return NULL; }

inline BOOL IsIconic(HWND) { return FALSE; }
inline BOOL IsWindowVisible(HWND) { return FALSE; }

#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
inline DWORD WaitForSingleObject(HANDLE, DWORD) { return WAIT_OBJECT_0; }

#include <utility>
#define _vsnprintf vsnprintf
#define sscanf_s sscanf
template<size_t Size, typename... Args>
inline int sprintf_s(char* buffer, const char* format, Args&&... args) {
    return snprintf(buffer, Size, format, std::forward<Args>(args)...);
}
inline int sprintf_s(char* buffer, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vsnprintf(buffer, size, format, args);
    va_end(args);
    return ret;
}

#define SW_SHOWNORMAL 1
inline intptr_t ShellExecuteA(HWND, const char* op, const char* file, const char*, const char*, int) {
    if (op && strcmp(op, "open") == 0) {
        std::string cmd = std::string("xdg-open \"") + file + "\" &";
        int ret = system(cmd.c_str());
        return ret == 0 ? 33 : 0;
    }
    return 0;
}

typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((packed)) BITMAPINFOHEADER;

#define BI_RGB 0

inline void SendMessage(HWND, int, WPARAM, LPARAM) {}

template<typename T, typename U>
inline auto max(const T& a, const U& b) {
    return (a > b) ? a : b;
}

template<typename T, typename U>
inline auto min(const T& a, const U& b) {
    return (a < b) ? a : b;
}

#define ANSI_CHARSET 0

// Key modifiers & ShowWindow options
#define SW_HIDE 0
#define SW_SHOWNORMAL 1
#define SW_NORMAL 1
#define SW_SHOWMINIMIZED 2
#define SW_SHOWMAXIMIZED 3
#define SW_MAXIMIZE 3
#define SW_SHOWNOACTIVATE 4
#define SW_SHOW 5
#define SW_MINIMIZE 6
#define SW_SHOWMINNOACTIVE 7
#define SW_SHOWNA 8
#define SW_RESTORE 9
#define SW_SHOWDEFAULT 10
#define SW_FORCEMINIMIZE 11

#define GWLP_USERDATA 0
#define WS_POPUP 0
#define WS_BORDER 0
#define WS_SYSMENU 0
#define DS_MODALFRAME 0
#define WS_CAPTION 0
#define DS_SETFONT 0
#define WS_VISIBLE 0
#define WS_CHILD 0
#define WS_VSCROLL 0
#define WS_HSCROLL 0
#define LBS_NOTIFY 0
#define WS_EX_CLIENTEDGE 0
#define WS_EX_TOPMOST 0
#define IDOK 1
#define IDCANCEL 2
#define LB_SETHORIZONTALEXTENT 0
#define LB_ADDSTRING 0
#define LB_SETITEMDATA 0
#define LB_GETCURSEL 0
#define LB_GETITEMDATA 0
#define LBN_DBLCLK 0
#define BS_DEFPUSHBUTTON 0

// Message Box Options
#define MB_OK 0x00000000L
#define MB_ICONERROR 0x00000010L

// Rect and Point structures
typedef struct tagRECT {
    int left;
    int top;
    int right;
    int bottom;
} RECT, *PRECT, *LPRECT;

typedef struct tagPOINT {
    int x;
    int y;
} POINT, *PPOINT, *LPPOINT;

inline void GetCursorPos(POINT* pt) {
    if (pt) { pt->x = 0; pt->y = 0; }
}

typedef struct tagMSG {
    HWND hwnd;
    unsigned int message;
    WPARAM wParam;
    LPARAM lParam;
    unsigned int time;
    POINT pt;
} MSG, *PMSG, *LPMSG;

typedef struct tagPAINTSTRUCT {
    HDC hdc;
    BOOL fErase;
    RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

// GDI / Windowing stubs
inline HDC BeginPaint(HWND, PAINTSTRUCT*) { return (HDC)0; }
inline void EndPaint(HWND, PAINTSTRUCT*) {}
inline HWND CreateWindowExA(int, const char*, const char*, int, int, int, int, int, HWND, void*, void*, void*) { return (HWND)1; }
extern std::string gCommandLineStr;
inline char* GetCommandLineA() { return (char*)gCommandLineStr.c_str(); }
inline HWND CreateWindowExW(int, const wchar_t*, const wchar_t*, int, int, int, int, int, HWND, void*, void*, void*) { return (HWND)1; }
#ifdef UNICODE
#define CreateWindowEx CreateWindowExW
#else
#define CreateWindowEx CreateWindowExA
#endif

typedef struct {
    int lpfnWndProc;
    void* hInstance;
    const char* lpszClassName;
} WNDCLASSA;

typedef struct {
    int lpfnWndProc;
    void* hInstance;
    const wchar_t* lpszClassName;
} WNDCLASSW;

inline void RegisterClassA(WNDCLASSA*) {}
inline void RegisterClassW(WNDCLASSW*) {}
#ifdef UNICODE
#define WNDCLASS WNDCLASSW
#define RegisterClass RegisterClassW
#else
#define WNDCLASS WNDCLASSA
#define RegisterClass RegisterClassA
#endif

inline void MoveWindow(HWND, int, int, int, int, bool) {}
inline void ShowWindow(HWND, int) {}

typedef struct tagWINDOWPLACEMENT {
    UINT length;
    UINT flags;
    UINT showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT rcNormalPosition;
} WINDOWPLACEMENT;
inline bool GetWindowPlacement(HWND, WINDOWPLACEMENT*) { return true; }

#define SIZE_MINIMIZED 0
#define SIZE_MAXIMIZED 0

inline HWND WindowFromPoint(POINT) { return (HWND)0; }
inline void ReleaseCapture() {}
inline BOOL AdjustWindowRect(RECT*, DWORD, BOOL) { return TRUE; }
inline HWND GetActiveWindow() { return (HWND)1; }
inline void SetTimer(HWND, uintptr_t, UINT, void*) {}

#define SPI_GETWORKAREA 0
inline BOOL SystemParametersInfo(UINT, UINT, void*, UINT) { return TRUE; }

inline void CoInitialize(void*) {}

inline void CreateCaret(HWND, void*, int, int) {}
inline void ShowCaret(HWND) {}
inline void HideCaret(HWND) {}
inline void DestroyCaret() {}
inline void SetCaretPos(int, int) {}
inline void UpdateWindow(HWND) {}
inline void SetWindowText(HWND, const char*) {}
inline void SetWindowTextW(HWND, const wchar_t*) {}
inline void DestroyWindow(HWND) {}
inline void SetForegroundWindow(HWND) {}
inline void SetFocus(HWND) {}
inline bool GetClientRect(HWND, RECT* r) {
    if (r) { r->left = 0; r->top = 0; r->right = 800; r->bottom = 600; }
    return true;
}
inline bool GetWindowRect(HWND, RECT* r) {
    if (r) { r->left = 0; r->top = 0; r->right = 800; r->bottom = 600; }
    return true;
}
inline bool ClientToScreen(HWND, POINT*) { return true; }
inline bool ScreenToClient(HWND, POINT*) { return true; }
inline int ShowCursor(bool) { return 0; }
inline void OutputDebugStringA(const char* s) { fprintf(stderr, "%s", s); }
inline void OutputDebugStringW(const wchar_t* s) { fwprintf(stderr, L"%s", s); }
#ifdef UNICODE
#define OutputDebugString OutputDebugStringW
#else
#define OutputDebugString OutputDebugStringA
#endif

inline bool IsDebuggerPresent() { return false; }
inline void __debugbreak() { std::abort(); }


inline void SetWindowLongPtr(HWND, int, intptr_t) {}
inline intptr_t GetWindowLongPtr(HWND, int) { return 0; }

// Cursor and Module Handle Helpers
inline HCURSOR LoadCursor(HINSTANCE, const char*) { return (HCURSOR)1; }
inline void SetCursor(HCURSOR) {}
inline void DestroyCursor(HCURSOR) {}
inline HINSTANCE GetModuleHandle(const char*) { return (HINSTANCE)1; }

#define MAKEINTRESOURCE(i) ((const char*)(intptr_t)(i))
#define IDC_ARROW           ((const char*)32512)
#define IDC_IBEAM           ((const char*)32513)
#define IDC_WAIT            ((const char*)32514)
#define IDC_CROSS           ((const char*)32515)
#define IDC_UPARROW         ((const char*)32516)
#define IDC_SIZE            ((const char*)32640)
#define IDC_ICON            ((const char*)32641)
#define IDC_SIZENWSE        ((const char*)32642)
#define IDC_SIZENESW        ((const char*)32643)
#define IDC_SIZEWE          ((const char*)32644)
#define IDC_SIZENS          ((const char*)32645)
#define IDC_SIZEALL         ((const char*)32646)
#define IDC_NO              ((const char*)32648)
#define IDC_HAND            ((const char*)32649)
#define IDC_APPSTARTING     ((const char*)32650)
#define IDC_HELP            ((const char*)32651)
#define IDC_CURSOR1         101

// System Metrics
#define SM_CXFULLSCREEN 0
#define SM_CYFULLSCREEN 1
inline int GetSystemMetrics(int metric) {
    if (metric == SM_CXFULLSCREEN) return 800;
    if (metric == SM_CYFULLSCREEN) return 600;
    return 0;
}

// Module path
inline DWORD GetModuleFileNameA(HMODULE, char* lpFilename, DWORD nSize) {
    ssize_t len = readlink("/proc/self/exe", lpFilename, nSize - 1);
    if (len != -1) {
        lpFilename[len] = '\0';
        return len;
    }
    lpFilename[0] = '\0';
    return 0;
}

// Threading & Critical Section Helpers
#include <pthread.h>
typedef struct {
    pthread_mutex_t mutex;
} CRITICAL_SECTION;
inline void InitializeCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_init(&cs->mutex, nullptr);
}
inline void DeleteCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_destroy(&cs->mutex);
}
inline void EnterCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_lock(&cs->mutex);
}
inline void LeaveCriticalSection(CRITICAL_SECTION* cs) {
    pthread_mutex_unlock(&cs->mutex);
}

// LARGE_INTEGER & Performance Counters
typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    long long QuadPart;
} LARGE_INTEGER;

inline BOOL QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency) {
    if (lpFrequency) {
        lpFrequency->QuadPart = 1000000000LL;
    }
    return TRUE;
}
inline BOOL QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount) {
    if (lpPerformanceCount) {
        auto now = std::chrono::steady_clock::now();
        lpPerformanceCount->QuadPart = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }
    return TRUE;
}

// Thread Priority & Redirects
inline HANDLE GetCurrentThread() { return (HANDLE)1; }
inline int GetThreadPriority(HANDLE) { return 0; }
inline BOOL SetThreadPriority(HANDLE, int) { return TRUE; }
#define THREAD_PRIORITY_HIGHEST 2
#define THREAD_PRIORITY_ABOVE_NORMAL 1
#define THREAD_PRIORITY_NORMAL 0

// Time redirects
#define _time64 time
#define _time32 time

#include <time.h>
inline int _localtime64_s(struct tm* _Tm, const __time64_t* _Time) {
    if (!_Tm || !_Time) return 1;
    time_t t = (time_t)*_Time;
    return localtime_r(&t, _Tm) ? 0 : 1;
}

inline char* _strtime(char* buf) {
    time_t t = time(NULL);
    struct tm* tmp = localtime(&t);
    if (tmp) {
        strftime(buf, 9, "%H:%M:%S", tmp);
    } else {
        strcpy(buf, "00:00:00");
    }
    return buf;
}

// DirectDraw and GDI shims
typedef struct {
    int dwSize;
    int dwFlags;
    int dwRGBBitCount;
    int dwRGBAlphaBitMask;
    int dwRBitMask;
    int dwGBitMask;
    int dwBBitMask;
} DDPIXELFORMAT;
typedef DDPIXELFORMAT* LPDDPIXELFORMAT;

typedef struct {
    int dwSize;
    int dwFlags;
    int dwHeight;
    int dwWidth;
    int lPitch;
    void* lpSurface;
    DDPIXELFORMAT ddpfPixelFormat;
    struct {
        int dwCaps;
        int dwCaps2;
    } ddsCaps;
} DDSURFACEDESC, DDSURFACEDESC2;

// File Mapping and Finding Helpers
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <fnmatch.h>

// Case-insensitive path resolver for Linux
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <mutex>

inline bool ascii_char_case_insensitive_equal(char a, char b) {
    if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
    if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
    return a == b;
}

inline bool ascii_str_case_insensitive_equal(const std::string& s1, const std::string& s2) {
    if (s1.length() != s2.length()) return false;
    for (size_t i = 0; i < s1.length(); ++i) {
        if (!ascii_char_case_insensitive_equal(s1[i], s2[i])) return false;
    }
    return true;
}

inline std::string sexy_resolve_path(const std::string& original_path) {
    std::string path = original_path;
    std::replace(path.begin(), path.end(), '\\', '/');

    static std::unordered_map<std::string, std::string> resolution_cache;
    static std::mutex cache_mutex;
    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        auto it = resolution_cache.find(path);
        if (it != resolution_cache.end()) {
            return it->second;
        }
    }

    std::vector<std::string> components;
    size_t start = 0;
    while (start < path.length()) {
        size_t end = path.find('/', start);
        if (end == std::string::npos) {
            components.push_back(path.substr(start));
            break;
        }
        if (end > start) {
            components.push_back(path.substr(start, end - start));
        }
        start = end + 1;
    }

    std::string current_base = "";
    if (!path.empty() && path[0] == '/') {
        current_base = "/";
    } else {
        current_base = ".";
    }

    for (const auto& comp : components) {
        if (comp == "." || comp == "..") {
            current_base = current_base + "/" + comp;
            continue;
        }

        std::string exact_path = current_base + "/" + comp;
        struct stat st;
        if (stat(exact_path.c_str(), &st) == 0) {
            current_base = exact_path;
            continue;
        }

        DIR* dir = opendir(current_base.c_str());
        if (!dir) {
            current_base = current_base + "/" + comp;
            continue;
        }

        struct dirent* entry;
        bool found = false;
        while ((entry = readdir(dir)) != nullptr) {
            if (ascii_str_case_insensitive_equal(entry->d_name, comp)) {
                current_base = current_base + "/" + entry->d_name;
                found = true;
                break;
            }
        }
        closedir(dir);

        if (!found) {
            current_base = current_base + "/" + comp;
        }
    }

    std::string resolved = current_base;
    if (resolved.compare(0, 2, "./") == 0 && path.compare(0, 2, "./") != 0) {
        resolved = resolved.substr(2);
    }

    {
        std::lock_guard<std::mutex> lock(cache_mutex);
        resolution_cache[path] = resolved;
    }
    return resolved;
}

inline BOOL DeleteFileA(const char* filename) { return remove(sexy_resolve_path(filename).c_str()) == 0; }

#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
#define GENERIC_READ 0x80000000L
#define FILE_SHARE_READ 1
#define OPEN_EXISTING 3
#define PAGE_READONLY 2
#define FILE_MAP_READ 4
#define PAGE_READWRITE 4
#define FILE_MAP_ALL_ACCESS 0xF001F

#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
#endif

#define MAX_PATH 260
#define _MAX_PATH 260
#define FILE_ATTRIBUTE_DIRECTORY 0x10

typedef struct {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR  cFileName[MAX_PATH];
    CHAR  cAlternateFileName[14];
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;

typedef WIN32_FIND_DATA WIN32_FIND_DATAA;
typedef WIN32_FIND_DATA* LPWIN32_FIND_DATAA;

inline DWORD GetCurrentProcessId() {
    return (DWORD)getpid();
}

inline HANDLE CreateFile(const char* lpFileName, DWORD, DWORD, void*, DWORD, DWORD, HANDLE) {
    std::string fn = sexy_resolve_path(lpFileName);
    int fd = open(fn.c_str(), O_RDONLY);
    if (fd < 0) return INVALID_HANDLE_VALUE;
    return (HANDLE)(intptr_t)fd;
}
inline HANDLE CreateFileA(const char* lpFileName, DWORD access, DWORD share, void* sec, DWORD creation, DWORD flags, HANDLE templateFile) {
    return CreateFile(lpFileName, access, share, sec, creation, flags, templateFile);
}

inline DWORD GetFileSize(HANDLE hFile, DWORD*) {
    int fd = (int)(intptr_t)hFile;
    struct stat st;
    if (fstat(fd, &st) == 0) return st.st_size;
    return 0;
}

inline BOOL CloseHandle(HANDLE hObject) {
    int fd = (int)(intptr_t)hObject;
    if (fd >= 0) close(fd);
    return TRUE;
}

inline HANDLE CreateFileMapping(HANDLE hFile, void*, DWORD, DWORD, DWORD, const char*) {
    return hFile;
}
inline HANDLE CreateFileMappingA(HANDLE hFile, void* sec, DWORD protect, DWORD maxHigh, DWORD maxLow, const char* name) {
    return CreateFileMapping(hFile, sec, protect, maxHigh, maxLow, name);
}

inline void* MapViewOfFile(HANDLE hFileMappingObject, DWORD, DWORD, DWORD, SIZE_T dwNumberOfBytesToMap) {
    int fd = (int)(intptr_t)hFileMappingObject;
    if (fd < 0) return nullptr;
    void* addr = mmap(nullptr, dwNumberOfBytesToMap, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) return nullptr;
    return addr;
}

struct FindState {
    DIR* dir;
    std::string pattern;
    std::string directory;
};

inline HANDLE FindFirstFileA(const char* lpFileName, LPWIN32_FIND_DATA lpFindFileData) {
    if (!lpFileName || !lpFindFileData) return INVALID_HANDLE_VALUE;
    
    std::string path = lpFileName;
    std::replace(path.begin(), path.end(), '\\', '/');
    
    size_t last_slash = path.find_last_of('/');
    std::string dir_path = (last_slash == std::string::npos) ? "." : path.substr(0, last_slash);
    std::string pattern = (last_slash == std::string::npos) ? path : path.substr(last_slash + 1);
    
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) return INVALID_HANDLE_VALUE;
    
    FindState* state = new FindState();
    state->dir = dir;
    state->pattern = pattern;
    state->directory = dir_path;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (fnmatch(pattern.c_str(), entry->d_name, 0) == 0) {
            memset(lpFindFileData, 0, sizeof(*lpFindFileData));
            strncpy(lpFindFileData->cFileName, entry->d_name, sizeof(lpFindFileData->cFileName) - 1);
            
            struct stat st;
            std::string full_path = dir_path + "/" + entry->d_name;
            if (stat(full_path.c_str(), &st) == 0) {
                lpFindFileData->nFileSizeLow = st.st_size;
                if (S_ISDIR(st.st_mode)) {
                    lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
                }
            }
            return (HANDLE)state;
        }
    }
    
    closedir(dir);
    delete state;
    return INVALID_HANDLE_VALUE;
}
#define FindFirstFile FindFirstFileA

inline BOOL FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData) {
    if (hFindFile == INVALID_HANDLE_VALUE || !lpFindFileData) return FALSE;
    FindState* state = (FindState*)hFindFile;
    
    struct dirent* entry;
    while ((entry = readdir(state->dir)) != nullptr) {
        if (fnmatch(state->pattern.c_str(), entry->d_name, 0) == 0) {
            memset(lpFindFileData, 0, sizeof(*lpFindFileData));
            strncpy(lpFindFileData->cFileName, entry->d_name, sizeof(lpFindFileData->cFileName) - 1);
            
            struct stat st;
            std::string full_path = state->directory + "/" + entry->d_name;
            if (stat(full_path.c_str(), &st) == 0) {
                lpFindFileData->nFileSizeLow = st.st_size;
                if (S_ISDIR(st.st_mode)) {
                    lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}
#define FindNextFile FindNextFileA

inline BOOL FindClose(HANDLE hFindFile) {
    if (hFindFile == INVALID_HANDLE_VALUE) return FALSE;
    FindState* state = (FindState*)hFindFile;
    closedir(state->dir);
    delete state;
    return TRUE;
}

#define APIENTRY
#define REG_SZ 1
#define REG_DWORD 4
#define REG_BINARY 3

// Timing / Threading / Filesystem helpers
inline unsigned long GetTickCount() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
inline unsigned long timeGetTime() {
    return GetTickCount();
}
inline void Sleep(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
inline int _mkdir(const char* path) {
    return mkdir(path, 0777);
}
#define mkdir(x) _mkdir(x)
inline char* _getcwd(char* buf, size_t size) {
    return getcwd(buf, size);
}

// String comparisons
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define wcsicmp wcscasecmp



inline FILE* sexy_fopen(const char* filename, const char* mode) {
    std::string resolved = sexy_resolve_path(filename);
#undef fopen
    FILE* f = ::fopen(resolved.c_str(), mode);
    fprintf(stderr, "sexy_fopen: original='%s' resolved='%s' success=%d\n", filename, resolved.c_str(), f != NULL);
#define fopen sexy_fopen
    return f;
}

#define fopen sexy_fopen

// Wide-character _wfopen shim
inline FILE* _wfopen(const wchar_t* filename, const wchar_t* mode) {
    std::wstring wfn(filename);
    std::wstring wm(mode);
    std::string fn(wfn.begin(), wfn.end());
    std::string m(wm.begin(), wm.end());
    std::string resolved = sexy_resolve_path(fn);
#undef fopen
    FILE* f = ::fopen(resolved.c_str(), m.c_str());
#define fopen sexy_fopen
    return f;
}

#endif

#include "ModVal.h"

#ifdef _USE_WIDE_STRING

typedef std::wstring		SexyString;
#define _S(x)				L ##x


#define sexystrncmp			wcsncmp
#define sexystrcmp			wcscmp
#define sexystricmp			wcsicmp
#define sexysscanf			swscanf
#define sexyatoi			_wtoi
#define sexystrcpy			wcscpy

#define SexyStringToStringFast(x)	WStringToString(x)
#define SexyStringToWStringFast(x)	(x)
#define StringToSexyStringFast(x)	StringToWString(x)
#define WStringToSexyStringFast(x)	(x)

#ifndef SEXYFRAMEWORK_NO_REDEFINE_WIN_API
// Redefine the functions and structs we need to be wide-string
#undef CreateWindowEx
#undef RegisterClass
#undef MessageBox
#undef ShellExecute
#undef GetTextExtentPoint32
#undef RegisterWindowMessage
#undef CreateMutex
#undef DrawTextEx
#undef TextOut

#define CreateWindowEx				CreateWindowExW
#define RegisterClass				RegisterClassW
#define WNDCLASS					WNDCLASSW
#define MessageBox					MessageBoxW
#define ShellExecute				ShellExecuteW
#define GetTextExtentPoint32		GetTextExtentPoint32W
#define RegisterWindowMessage		RegisterWindowMessageW
#define CreateMutex					CreateMutexW
#define DrawTextEx					DrawTextExW
#define TextOut						TextOutW
#endif

#else

typedef std::string			SexyString;
#define _S(x)				x


#define sexystrncmp			strncmp
#define sexystrcmp			strcmp
#define sexystricmp			stricmp
#define sexysscanf			sscanf
#define sexyatoi			atoi
#define sexystrcpy			strcpy

#define SexyStringToStringFast(x)	(x)
#define SexyStringToWStringFast(x)	StringToWString(x)
#define StringToSexyStringFast(x)	(x)
#define WStringToSexyStringFast(x)	WStringToString(x)

#endif

#define LONG_BIGE_TO_NATIVE(l) (((l >> 24) & 0xFF) | ((l >> 8) & 0xFF00) | ((l << 8) & 0xFF0000) | ((l << 24) & 0xFF000000))
#define WORD_BIGE_TO_NATIVE(w) (((w >> 8) & 0xFF) | ((w << 8) & 0xFF00))
#define LONG_LITTLEE_TO_NATIVE(l) (l)
#define WORD_LITTLEE_TO_NATIVE(w) (w)

#define LENGTH(anyarray) (sizeof(anyarray) / sizeof(anyarray[0]))

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
#ifdef _WIN32
typedef unsigned long ulong;
#else
#define ulong unsigned int
#endif
typedef __int64 int64;

typedef std::map<std::string, std::string>		DefinesMap;
typedef std::map<std::wstring, std::wstring>	WStringWStringMap;
typedef SexyString::value_type					SexyChar;
#define HAS_SEXYCHAR

namespace Sexy
{

const ulong SEXY_RAND_MAX = 0x7FFFFFFF;

extern bool			gDebug;
extern HINSTANCE	gHInstance;

int					Rand();
int					Rand(int range);
float				Rand(float range);
void				SRand(ulong theSeed);
extern std::string	vformat(const char* fmt, va_list argPtr);
extern std::wstring	vformat(const wchar_t* fmt, va_list argPtr);
extern std::string	StrFormat(const char* fmt ...);
extern std::wstring	StrFormat(const wchar_t* fmt ...);
bool				CheckFor98Mill();
bool				CheckForVista();
std::string			GetAppDataFolder();
void				SetAppDataFolder(const std::string& thePath);
std::string			URLEncode(const std::string& theString);
std::string			StringToUpper(const std::string& theString);
std::wstring		StringToUpper(const std::wstring& theString);
std::string			StringToLower(const std::string& theString);
std::wstring		StringToLower(const std::wstring& theString);
std::wstring		StringToWString(const std::string &theString);
std::string			WStringToString(const std::wstring &theString);
SexyString			StringToSexyString(const std::string& theString);
SexyString			WStringToSexyString(const std::wstring& theString);
std::string			SexyStringToString(const SexyString& theString);
std::wstring		SexyStringToWString(const SexyString& theString);
std::string			Upper(const std::string& theData);
std::wstring		Upper(const std::wstring& theData);
std::string			Lower(const std::string& theData);
std::wstring		Lower(const std::wstring& theData);
std::string			Trim(const std::string& theString);
std::wstring		Trim(const std::wstring& theString);
bool				StringToInt(const std::string theString, int* theIntVal);
bool				StringToDouble(const std::string theString, double* theDoubleVal);
bool				StringToInt(const std::wstring theString, int* theIntVal);
bool				StringToDouble(const std::wstring theString, double* theDoubleVal);
int					StrFindNoCase(const char *theStr, const char *theFind);
bool				StrPrefixNoCase(const char *theStr, const char *thePrefix, int maxLength = 10000000);
SexyString			CommaSeperate(int theValue);
std::string			Evaluate(const std::string& theString, const DefinesMap& theDefinesMap);
std::string			XMLDecodeString(const std::string& theString);
std::string			XMLEncodeString(const std::string& theString);
std::wstring		XMLDecodeString(const std::wstring& theString);
std::wstring		XMLEncodeString(const std::wstring& theString);

bool				Deltree(const std::string& thePath);
bool				FileExists(const std::string& theFileName);
void				MkDir(const std::string& theDir);
std::string			GetFileName(const std::string& thePath, bool noExtension = false);
std::string			GetFileDir(const std::string& thePath, bool withSlash = false);
std::string			RemoveTrailingSlash(const std::string& theDirectory);
std::string			AddTrailingSlash(const std::string& theDirectory, bool backSlash = false);
time_t				GetFileDate(const std::string& theFileName);
std::string			GetCurDir();
std::string			GetFullPath(const std::string& theRelPath);
std::string			GetPathFrom(const std::string& theRelPath, const std::string& theDir);
bool				AllowAllAccess(const std::string& theFileName);


inline void			inlineUpper(std::string &theData)
{
    //std::transform(theData.begin(), theData.end(), theData.begin(), toupper);

	int aStrLen = (int) theData.length();
	for (int i = 0; i < aStrLen; i++)
	{
		theData[i] = toupper(theData[i]);
	}
}

inline void			inlineUpper(std::wstring &theData)
{
    //std::transform(theData.begin(), theData.end(), theData.begin(), toupper);

	int aStrLen = (int) theData.length();
	for (int i = 0; i < aStrLen; i++)
	{
		theData[i] = towupper(theData[i]);
	}
}

inline void			inlineLower(std::string &theData)
{
    std::transform(theData.begin(), theData.end(), theData.begin(), tolower);
}

inline void			inlineLower(std::wstring &theData)
{
    std::transform(theData.begin(), theData.end(), theData.begin(), tolower);
}

inline void			inlineLTrim(std::string &theData, const std::string& theChars = " \t\r\n")
{
    theData.erase(0, theData.find_first_not_of(theChars));
}

inline void			inlineLTrim(std::wstring &theData, const std::wstring& theChars = L" \t\r\n")
{
    theData.erase(0, theData.find_first_not_of(theChars));
}


inline void			inlineRTrim(std::string &theData, const std::string& theChars = " \t\r\n")
{
    theData.resize(theData.find_last_not_of(theChars) + 1);
}

inline void			inlineTrim(std::string &theData, const std::string& theChars = " \t\r\n")
{
	inlineRTrim(theData, theChars);
	inlineLTrim(theData, theChars);
}

struct StringLessNoCase { bool operator()(const std::string &s1, const std::string &s2) const { return _stricmp(s1.c_str(),s2.c_str())<0; } };

}

#endif //__SEXYAPPFRAMEWORK_COMMON_H__
