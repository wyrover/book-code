// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <objbase.h>
#include "UIlib.h"

//根据不同的版本连接不同的库
//#ifdef _DEBUG
//# pragma comment(lib, "duilib_d.lib")
//#else
//# pragma comment(lib, "duilib.lib")
//#endif

// TODO: 在此处引用程序需要的其他头文件
