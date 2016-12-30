/* -------------------------------------------------------------------------
//	文件名		：	/Import/buildcfg/buildcfg.h
//	创建者		：	刘桂峰
//	创建时间	：	2008-1-6 13:27:52
//	功能描述	：	版本信息控制文件，修改自单机版2008的buildcfg.h
//
// -----------------------------------------------------------------------*/
#ifndef __BUILDCFG_BUILDCFG_H__
#define __BUILDCFG_BUILDCFG_H__

/*
0、在工程的stdafx.h中加入
	#include "buildcfg/buildcfg.h"		
	
1、打开本工程的主资源文件，删除原有的版本信息
	在 "3 TEXTINCLUDE" 段插入
	"#include ""buildcfg/VersionNo.rc2""\r\n"
	查找 "#ifndef APSTUDIO_INVOKED"，在中间接入 
	#include "buildcfg/VersionNo.rc2"

2、打开/include/buildcfg目录下对应的buildcfg.h文件
	找到该工程所在位置，请用英语定义以下三个宏，如：	

#ifdef KXE_PlATMAIN // size : 20000 工程名称的宏标记
	#pragma comment(linker, "/base:0x4DA00000")   
	#pragma message("\t    KXE_PLAT_MAIN")
	#define VI_FILEDESCRIPTION	"KXEngine Plat Main"	//该工程编译生成的文件的文件描述
	#define VI_INTERNALNAME		"KXMAIN"				//该工程编译生成的文件的内部名称
	#define VI_ORIGINALFILENAME	"kxemain.exe"			//该工程编译生成的文件的原始DLL文件名
#endif

3、在工程设置的 Resources 页的"Preprocessor definitions"中添加本工程对应的宏，如 KXE_PlATMAIN
	请注意所有配置都要加上
	在"Additionalresource include directories"中加上include目录的相对位置
	如果工程在src/xxxdll/x.proj，则需要添加引用目录 "../../include"

4、在本工程的主资源文件的首部如果有：
	#define APSTUDIO_HIDDEN_SYMBOLS
	#include "windows.h"
	#undef APSTUDIO_HIDDEN_SYMBOLS

	这样的定义，那么将其改为：
	#define APSTUDIO_HIDDEN_SYMBOLS
	#include "buildcfg/buildcfg.h"
	#include "windows.h"
	#undef APSTUDIO_HIDDEN_SYMBOLS

 */

// -------------------------------------------------------------------------
	

#ifdef _WINDOWS_
#error "include buildcfg.h before include all other files"
#endif
	
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1

/*
部分需要忽略的警告
#pragma warning(disable : 4018)
#pragma warning(disable : 4100)
#pragma warning(disable : 4189)	//local variable is initialized but not referenced
#pragma warning(disable : 4245)
#pragma warning(disable : 4995) //warning C4995: 'wcscpy': name was marked as #pragma deprecated
#pragma warning(disable : 4800)	//warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)	//'var' : conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4244)	//'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable : 4311)	//'variable' : pointer truncation from 'type' to 'type'
#pragma warning(disable : 4312)	//'operation' : conversion from 'type1' to 'type2' of greater size
#pragma warning(disable : 6248)	//Setting a SECURITY_DESCRIPTOR's DACL to NULL will result in an unprotected object
#pragma warning(disable : 6011)	//Dereferencing NULL pointer 'lpdis': Lines: 909, 911
#pragma warning(disable : 4353)	//nonstandard extension used: constant 0 as function expression. Use '__noop' function intrinsic instead
#pragma warning(disable : 4311)	//'type cast' : pointer truncation from 'void *' to 'int'
#pragma warning(disable : 6031)	//Return value ignored: 'SHGetSpecialFolderLocation')
#pragma warning(disable : 6297)	//Arithmetic overflow: 32-bit value is shifted, then cast to 64-bit value. Results might not be an expected value
#pragma warning(disable : 6211)	//Leaking memory 'pNewNode' due to an exception. Consider using a local catch block to clean up memory: Lines: 301, 30
#pragma warning(disable : 6242)	//A jump out of this try block forces local unwind. This incurs a severe performance penalty
#pragma warning(disable : 6258)	//Using TerminateThread does not allow proper thread clean up
#pragma warning(disable : 6246)	//Local declaration of 'strTaskFileName' hides declaration of the same name in outer scope. For additional information, see previous declaration at line '1156' of 'e:\kis2008\

//请打开VS 2005编译设置Advance中的“Enable Code Analysis for C/C++" (好像需要vs tream版的支持)
//下面这些一定要改
//printf("%s\n", wcs) -> printf("%S\n", wcs)
#pragma warning (error: 6303)	// Format string mismatch: wide character string passed as parameter '2' when character string is required in call to 'printf'

// i + 30 -> i += 30
#pragma warning (error: 4552)	

 // (!x & y) -> (!(x & y))
#pragma warning (error: 6290)	// Bitwise operation on logical result: ! has higher precedence than &. Use && or (!(x & y)) instead

//int a[2]; a[2] = 1; -> a[1] = 1
//#pragma warning (error: 6385)	// Invalid data: accessing 'argument 2', the readable size is 'sizeof(KAE_SCAN_FILE_STATUS)' bytes, but '404' bytes might be read: Lines: 442, 443
//#pragma warning (error: 6386)	// Buffer overrun: accessing 'argument 2', the writable size is '520' bytes, but '1040' bytes might be written: Lines: 62, 63
//GetShortPathName(lfn, sfn, sizeof(sfn)) - > GetShortPathName(lfn, sfn, countof(sfn))
//#pragma warning (error: 6057)	// Buffer overrun due to number of characters/number of bytes mismatch in call to 'GetShortPathNameW'
*/
// 主动重定位dll基地址
// 有的DLL需要的地址空间比较多，所以请不要在已有的重定位指示之间添加新的指示指令！
// DLL基地址的指定，基本按照dll的加载顺序递增

/////////////////////////////////
//exe
/*
#ifdef KISM_KAV32
	#pragma message("\t    KISM_KAV32")
	#define VI_FILEDESCRIPTION	"Kingsoft AntiVirus"
	#define VI_INTERNALNAME		"kav32"
	#define VI_ORIGINALFILENAME	"kav32.exe"
#endif
*/


/////////////////////////////////////////////////////////////
//dll
/*
#ifdef KISM_KIS
	#pragma comment(linker, "/base:0x41000000")
	#pragma message("\t    KISM_KIS")
	#define VI_FILEDESCRIPTION	"Kingsoft Antivirus Common Module"
	#define VI_INTERNALNAME		"kis"
	#define VI_ORIGINALFILENAME	"kis.dll"
#endif
*/
//////////////////////////////////////////////////////////////////////////

#ifdef DG_SERVICE_EXE
    #pragma message("\t   DG_SERVICE_EXE")
    #define VI_FILEDESCRIPTION	"驱动精灵核心服务，提供驱动安全保护、升级、以及驱动即插即用服务。依托云技术可以为您的计算机提供安全、完整的驱动解决方案。"
    #define VI_INTERNALNAME		"DgService"
    #define VI_ORIGINALFILENAME	"DgService.EXE"
#endif

#ifdef DG_PATHCORE
#pragma message("\t   DG_PATHCORE")
#define VI_FILEDESCRIPTION	"DriverGenius PatchCore"
#define VI_INTERNALNAME		"PatchCore"
#define VI_ORIGINALFILENAME	"PatchCore.DLL"
#endif

#ifdef DG_APPLICATION
#pragma message("\t   DG_APPLICATION")
#define VI_FILEDESCRIPTION	"DriverGenius Application"
#define VI_INTERNALNAME		"Application"
#define VI_ORIGINALFILENAME	"Application.exe"
#endif

#ifdef DG_NOTIFIER
#pragma message("\t   DG_NOTIFIER")
#define VI_FILEDESCRIPTION	"DriverGenius Notifier"
#define VI_INTERNALNAME		"Notifier"
#define VI_ORIGINALFILENAME	"Notifier.exe"
#endif

#ifdef DG_MOBILEGENIUS
#pragma message("\t   DG_MOBILEGENIUS")
#define VI_FILEDESCRIPTION	"DriverGenius MobileGenius"
#define VI_INTERNALNAME		"MobileGenius"
#define VI_ORIGINALFILENAME	"MobileGenius.exe"
#endif

#ifdef DG_TASKBAR_NOTIFIER
#pragma message("\t   DG_TASKBAR_NOTIFIER")
#define VI_FILEDESCRIPTION	"DriverGenius UsbPnp"
#define VI_INTERNALNAME		"UsbPnp"
#define VI_ORIGINALFILENAME	"UsbPnp.exe"
#endif

#ifdef KSZZ_DOWN_DLL
#pragma message("\t   KSZZ_DOWN_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius Kszzdl"
#define VI_INTERNALNAME		"Kszzdl"
#define VI_ORIGINALFILENAME	"Kszzdl.dll"
#endif

#ifdef DG_TEST_DRIVERDOWN
#pragma message("\t   DG_TEST_DRIVERDOWN")
#define VI_FILEDESCRIPTION	"DriverGenius TestDriverDown"
#define VI_INTERNALNAME		"TestDriverDown"
#define VI_ORIGINALFILENAME	"TestDriverDown.exe"
#endif

#ifdef PNPSUP_DLL
#pragma message("\t   PNPSUP_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius Pnpsup"
#define VI_INTERNALNAME		"Pnpsup"
#define VI_ORIGINALFILENAME	"Pnpsup.dll"
#endif

#ifdef PATCHCORE_DLL
#pragma message("\t   PATCHCORE_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius Patchcore"
#define VI_INTERNALNAME		"Patchcore"
#define VI_ORIGINALFILENAME	"Patchcore.dll"
#endif

#ifdef GPHONEIMP_DLL
#pragma message("\t   GPHONEIMP_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius GphoneImp"
#define VI_INTERNALNAME		"GphoneImp"
#define VI_ORIGINALFILENAME	"GphoneImp.dll"
#endif

#ifdef DG_MYDRIVERSI
#pragma message("\t   DG_MYDRIVERSI_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius MydriversI"
#define VI_INTERNALNAME		"MydriversI"
#define VI_ORIGINALFILENAME	"MydriversI.dll"
#endif


#ifdef DG_FileCE
#pragma message("\t   DG_FileCE_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius FileCE"
#define VI_INTERNALNAME		"FileCE"
#define VI_ORIGINALFILENAME	"FileCE.dll"
#endif


#ifdef DGROUTEMSG_DLL
#pragma message("\t   DGROUTEMSG_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius RouteMsg"
#define VI_INTERNALNAME		"DGRouteMsg"
#define VI_ORIGINALFILENAME	"DGRouteMsg.dll"
#endif


#ifdef DG_DRVDOWN_DLL
#pragma message("\t   DG_DRVDOWN_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius Drvdown"
#define VI_INTERNALNAME		"Drvdown"
#define VI_ORIGINALFILENAME	"Drvdown.dll"
#endif

#ifdef DGCHECK_DLL
#pragma message("\t   DGCHECK_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius DGCheck"
#define VI_INTERNALNAME		"DgCheck"
#define VI_ORIGINALFILENAME	"DgCheck.dll"
#endif

#ifdef DGVER_DLL
#pragma message("\t   DGVER_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius Version"
#define VI_INTERNALNAME		"DgVer"
#define VI_ORIGINALFILENAME	"DgVer.dll"
#endif

#ifdef DG_WEBCFG_DLL
#pragma message("\t   DG_WEBCFG_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius DGWebCfg"
#define VI_INTERNALNAME		"DGWebCfg"
#define VI_ORIGINALFILENAME	"DGWebCfg.dll"
#endif

#ifdef DG_LOCAL_DG_XP
#pragma message("\t   DG_LOCAL_DG_XP")
#define VI_FILEDESCRIPTION	"DriverGenius LocalDG"
#define VI_INTERNALNAME		"LocalDG"
#define VI_ORIGINALFILENAME	"LocalDG.exe"
#endif

#ifdef DG_COMPONENT_DLL
#pragma message("\t   DG_COMPONENT_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius DGComponent"
#define VI_INTERNALNAME		"DGComponent"
#define VI_ORIGINALFILENAME	"DGComponent.dll"
#endif

#ifdef DG_HELPER
#pragma message("\t   DG_HELPER")
#define VI_FILEDESCRIPTION	"DriverGenius DGHelper"
#define VI_INTERNALNAME		"DGHelper"
#define VI_ORIGINALFILENAME	"DGHelper.exe"
#endif

#ifdef DG_HELPER_DLL
#pragma message("\t   DG_HELPER_DLL")
#define VI_FILEDESCRIPTION	"DriverGenius DGHelper Lib"
#define VI_INTERNALNAME		"DGHelper Lib"
#define VI_ORIGINALFILENAME	"DGHelper.dll"
#endif

#ifdef DG_BACKUP
#pragma message("\t   DG_BACKUP")
#define VI_FILEDESCRIPTION	"DriverGenius DGBackup"
#define VI_INTERNALNAME		"DGBackup"
#define VI_ORIGINALFILENAME	"DGBackup.exe"
#endif

#ifdef DG_RES
#pragma message("\t   DG_RES")
#define VI_FILEDESCRIPTION	"DriverGenius Resources"
#define VI_INTERNALNAME		"DGRes"
#define VI_ORIGINALFILENAME	"dgres.dll"
#endif

#ifdef DG_CTRLS
#pragma message("\t   DG_CTRLS")
#define VI_FILEDESCRIPTION	"DriverGenius Controls"
#define VI_INTERNALNAME		"DGCtrls"
#define VI_ORIGINALFILENAME	"dgctrls.dll"
#endif

#ifdef DG_CORE
#pragma message("\t   DG_CORE")
#define VI_FILEDESCRIPTION	"DriverGenius Core"
#define VI_INTERNALNAME		"DGCore"
#define VI_ORIGINALFILENAME	"dgcore.dll"
#endif

#ifdef DG_MYDRIVERSC
#pragma message("\t   DG_MYDRIVERSC")
#define VI_FILEDESCRIPTION	"DriverGenius MydriversC"
#define VI_INTERNALNAME		"MydriversC"
#define VI_ORIGINALFILENAME	"MydriversC.dll"
#endif

#ifdef CACTUS_DLL
#pragma message("\t   CACTUS")
#define VI_FILEDESCRIPTION	"DriverGenius CACTUS"
#define VI_INTERNALNAME		"cactus"
#define VI_ORIGINALFILENAME	"cactus.dll"
#endif


#ifdef DG_UPDATE
#pragma message("\t   DG_UPDATE")
#define VI_FILEDESCRIPTION	"DriverGenius Update"
#define VI_INTERNALNAME		"DGUpdate"
#define VI_ORIGINALFILENAME	"driverupdate.exe"
#endif


#ifdef DG_DRVAGENT
#pragma message("\t   DG_DRVAGENT")
#define VI_FILEDESCRIPTION	"DriverGenius DrvAgent"
#define VI_INTERNALNAME		"DrvAgent"
#define VI_ORIGINALFILENAME	"DrvAgent.exe"
#endif


#ifdef DG_SHELLEXT
#pragma message("\t   DG_SHELLEXT")
#define VI_FILEDESCRIPTION	"DriverGenius Shell Extension"
#define VI_INTERNALNAME		"DGShlExt"
#define VI_ORIGINALFILENAME	"dgshlext.dll"
#endif


#ifdef DG_MAIN
#pragma message("\t   DG_MAIN")
#define VI_FILEDESCRIPTION	"DriverGenius"
#define VI_INTERNALNAME		"DriverGenius"
#define VI_ORIGINALFILENAME	"DriverGenius.exe"
#endif

#ifdef DG_UNINSTALL
#pragma message("\t   DG_UNINSTALL")
#define VI_FILEDESCRIPTION	"DriverGenius Uninstall"
#define VI_INTERNALNAME		"DGUninstall"
#define VI_ORIGINALFILENAME	"uninstall.exe"
#endif

#ifdef DG_WIN10HELPER
#pragma message("\t   DG_WIN10HELPER")
#define VI_FILEDESCRIPTION	"DriverGenius Sysopt"
#define VI_INTERNALNAME		"sysopt"
#define VI_ORIGINALFILENAME	"sysopt.exe"
#endif

#ifdef DG_SOUNDTOOL
#pragma message("\t   DG_SOUNDTOOL")
#define VI_FILEDESCRIPTION	"DriverGenius Sound Repair tool"
#define VI_INTERNALNAME		"dgsoundtool"
#define VI_ORIGINALFILENAME	"dgsoundtool.exe"
#endif

#ifdef DG_EXTPLUGIN
#pragma message("\t   DG_EXTPLUGIN")
#define VI_FILEDESCRIPTION	"DriverGeniusLite Plugin"
#define VI_INTERNALNAME		"dgext"
#define VI_ORIGINALFILENAME	"dgext.dll"
#endif

#ifdef DG_NETHELPER
#pragma message("\t   DG_NETHELPER")
#define VI_FILEDESCRIPTION	"DriverGenius NetHelper"
#define VI_INTERNALNAME		"NetHelper"
#define VI_ORIGINALFILENAME	"NetHelper.exe"
#endif

#ifdef DG_DISKDETECTION
#pragma message("\t   DG_DISKDETECTION")
#define VI_FILEDESCRIPTION	"DriverGenius HddScanner"
#define VI_INTERNALNAME		"HddScanner"
#define VI_ORIGINALFILENAME	"HddScanner.exe"
#endif

#ifdef DG_HWTOOL
#pragma message("\t   DG_HWTOOL")
#define VI_FILEDESCRIPTION	"DriverGenius HWTool"
#define VI_INTERNALNAME		"HWTool"
#define VI_ORIGINALFILENAME	"HWTool.exe"
#endif

#ifdef TINYXML_DLL
#pragma message("\t   TINYXML_DLL")
#define VI_FILEDESCRIPTION	"tinyxml"
#define VI_INTERNALNAME		"tinyxml"
#define VI_ORIGINALFILENAME	"tinyxml.dll"
#endif

#ifdef DUILIB_DLL
#pragma message("\t   DUILIB_DLL")
#define VI_FILEDESCRIPTION	"duilib"
#define VI_INTERNALNAME		"duilib"
#define VI_ORIGINALFILENAME	"duilib.dll"
#endif

#ifdef DG_SUPER_DRIVER_CONSOLE
#pragma message("\t   dgconsole")
#define VI_FILEDESCRIPTION	"dgconsole"
#define VI_INTERNALNAME		"dgconsole"
#ifdef _WIN64
	#define VI_ORIGINALFILENAME	"dgconsolex64.exe"
#else
	#define VI_ORIGINALFILENAME	"dgconsole.exe"
#endif


#endif


#ifdef FILE_2_HEX_EXE
#pragma message("\t   FILE_2_HEX_EXE")
#define VI_FILEDESCRIPTION	"file-2-hex"
#define VI_INTERNALNAME		"file-2-hex"
#define VI_ORIGINALFILENAME	"file-2-hex.exe"
#endif



#endif /* __BUILDCFG_BUILDCFG_H__ */
