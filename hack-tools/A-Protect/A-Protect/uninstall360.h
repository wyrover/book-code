#include <Windows.h>
#include "stdafx.h"
#include <stdio.h>
#include <shlwapi.h>
#include <tlhelp32.h>

#pragma comment(lib,"shlwapi.lib")

void enum_path(char *cpath);
BOOL GetAntiVirusBin(char *lpszKey,char *lpszValue,char *lpDirectory);
VOID Uninstall360();