#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <commctrl.h>
#include "resource.h"
#include "Objects.h"
#include "Additional.h"
#include "SmartFinder.h"
#include "SmartStringLoad.h"

#pragma comment(lib, "ComCtl32.Lib")
#define Copy false
#define Paste true

static HINSTANCE hInst;
static const char* wnd_class = "Finder";

unsigned short MyRegisterClass(HINSTANCE);
bool InitInstance(HINSTANCE, int);
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
bool __stdcall DlgInfo(HWND, UINT, WPARAM, LPARAM);
bool __stdcall DlgAbout(HWND, UINT, WPARAM, LPARAM);