// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <map>
#include <winternl.h>

#include "console.h"
#include "../../Library/udis86/udis86.h"
#include "../../Library/tinyxml/tinyxml.h"
#include "../../Hooklib/VirtualSelf.h"
#include "../../Hooklib/hooklib.h"
#include "SafeLocker.h"
#include "nedmalloc/nedmalloc.h"
#include "mmCom.h"
#include "../mmDebug/HookedDbgAPIs.h"
