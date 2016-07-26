
#pragma once

// 以下宏定义了最低所需平台。最低所需平台
// 是具备运行应用程序所需功能的最低版本的 Windows、Internet Explorer 等。
// 这些宏的工作方式是，启用平台版本上的所有可用功能
// 并包括指定的版本。

// 如果目标平台必须早于下面指定的版本，请修改以下定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。

#include <WinSDKVer.h>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_MAXVER      // 将其更改为适当的值，从而以 Windows 的其他版本为目标。
#endif

#include <SDKDDKVer.h>
