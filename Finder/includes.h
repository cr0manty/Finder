#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <commctrl.h>
#include "resource.h"
#include "Local.h"

#pragma comment(lib, "ComCtl32.Lib")

static HINSTANCE hInst;
static LPCTSTR wnd_class = "Finder";

ATOM MyRegisterClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
__int64 __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL __stdcall DlgInfo(HWND, UINT, WPARAM, LPARAM);
BOOL __stdcall DlgAbout(HWND, UINT, WPARAM, LPARAM);