#pragma once
#define _SILENCE_TR2_SYS_NAMESPACE_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <filesystem>
#include <commctrl.h>
#include "resource.h"
#include "Objects.h"
#include "Additional.h"
#include "SmartFinder.h"
#include "SmartStringLoad.h"

#pragma comment(lib, "ComCtl32.Lib")
#define Copy false
#define Paste true

__int64 __stdcall MainFunc(HWND, UINT, WPARAM, LPARAM);
bool __stdcall DlgInfo(HWND, UINT, WPARAM, LPARAM);
bool __stdcall DlgAbout(HWND, UINT, WPARAM, LPARAM);