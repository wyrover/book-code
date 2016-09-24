// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#define Debug 1             //注释掉即可消去日志打印信息

// Windows 头文件:
#include <windows.h>
#include "stdio.h"
#include "wchar.h"
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include "Mscat.h"
#include "Tlhelp32.h"

#include "ioctl.h"
#include "PPspt.h"


#pragma comment(lib,"Advapi32.lib")
#pragma comment (lib, "wintrust")

// TODO: 在此处引用程序需要的其他头文件

extern "C"
{
#include "miracl.h"
}

#pragma comment(lib,"ms32.lib")

#pragma comment(linker,"/NODEFAULTLIB:LIBCMT")
#pragma comment(linker,"/NODEFAULTLIB:LIBC")
