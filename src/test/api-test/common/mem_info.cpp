#include "mem_info.h"
#include <stdio.h>
#include <atlstr.h>
#include <boost/config.hpp> // for BOOST_STRINGIZE
#include <boost/version.hpp> // for BOOST_VERSION

static SYSTEM_INFO sysInfo;

void currentSystemMemoryInformation()
{
	GetNativeSystemInfo(&sysInfo); // 64bit

	unsigned long long installedMemory = 0;
	GetPhysicallyInstalledSystemMemory(&installedMemory);

	MEMORYSTATUSEX memstatus; memstatus.dwLength = sizeof(memstatus);
	GlobalMemoryStatusEx(&memstatus);

	printf("\nInstalled Memory : %lld Gigabytes",((installedMemory/1024)/1024));
	printf("\nPhysical Usable memory %lld, Gigabytes",((memstatus.ullTotalPhys/1024)/1024));
	printf("\nCurrent memory in use : %d%", memstatus.dwMemoryLoad);

	printf("\nPage Size : %d", sysInfo.dwPageSize); // Page Size
	printf("\nNumber of Logical Processors : %d", sysInfo.dwNumberOfProcessors);
}


void idProcessorArchitecture()
{
	char *pArchType;
	switch(sysInfo.wProcessorArchitecture)
	{
	case 9:
		{
			pArchType = "64 bit (x86_64)";
		} break;
	case 0:
		{
			pArchType = "32 bit (x86)";
		} break;
	case 5:
		{
			pArchType = "ARM processor";
		} break;
	case 1:
		{
			pArchType = "MIPS processor";
		} break;
	case 2:
		{
			pArchType = "ALPHA processor";
		} break;
	case 3:
		{
			pArchType = "PPC processor";
		} break;
	case 4:
		{
			pArchType = "SHX processor";
		} break;
	case 7:
		{
			pArchType = "ALPHA64 processor";
		} break;
	case 8:
		{
			pArchType = "MSIL processor";
		} break;
	case 10:
		{
			pArchType = "IA32_ON_WIN64 processor";
		} break;
	case 6:
		{
			pArchType = "Intel Itanium-based processor";
		} break;
	case 0xffff:
		{
			pArchType = "Unknown processor";
		} break;
	}

	printf("\nProcessor Architecture : %s", pArchType);
}



CString BoostLibraryVersion()
{
	CString cszVer;
	cszVer.Format(_T("Boost %u.%u.%u"),
		(BOOST_VERSION / 100000),
		((BOOST_VERSION / 100) % 1000),
		(BOOST_VERSION % 100) );
	return cszVer;
}


CString GetCompilerNameDisplayString()
{
	CString cszVer;
#if defined(_MSC_VER)
#if _MSC_VER > 1800
	cszVer = _T("Microsoft Visual C++ > 2013");
#elif _MSC_VER == 1800
	cszVer = _T("Microsoft Visual C++ 2013");
#elif _MSC_VER >= 1700
	cszVer = _T("Microsoft Visual C++ 2012");
#elif _MSC_VER >= 1600
	cszVer = _T("Microsoft Visual C++ 2010");
#elif _MSC_VER >= 1500
	cszVer = _T("Microsoft Visual C++ 2008");
#elif _MSC_VER >= 1400
	cszVer = _T("Microsoft Visual C++ 2005");
#elif _MSC_VER >= 1310
	cszVer = _T("Microsoft Visual C++ .NET 2003");
#elif _MSC_VER >= 1300
	cszVer = _T("Microsoft Visual C++ .NET 2002");
#elif _MSC_VER >= 1200
	cszVer = _T("Microsoft Visual C++ 6.0");
#else
	cszVer = _T("Microsoft Visual C++ 5.0 or lower");
#endif

	cszVer += _T(" (");
	USES_CONVERSION;
	cszVer += A2CT(BOOST_STRINGIZE(_MSC_VER));
	cszVer += _T(")");

#elif defined(__GNUC__)
	cszVer = CStringConversion::ConvertToTString(
		"gcc " BOOST_STRINGIZE(__GNUC__) "."
		BOOST_STRINGIZE(__GNUC_MINOR__) "."
		BOOST_STRINGIZE(__GNUC_PATCHLEVEL__));
#else
#error SystemInfo.cpp not ported to other compilers!
#endif

	return cszVer;
}