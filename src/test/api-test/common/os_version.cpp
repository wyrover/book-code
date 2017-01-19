#include "os_version.h"
#include <Windows.h>


int  getOSLevel()
{
	int oslevel = OSLEVEL_UNKNOWN;
	OSVERSIONINFO verInfo;
	ZeroMemory(&verInfo,sizeof(verInfo));
	verInfo.dwOSVersionInfoSize=sizeof(verInfo);

	if(GetVersionEx((FAR OSVERSIONINFO *) &verInfo))
	{
		if(verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if(verInfo.dwMajorVersion <= 4)
			{
				/* This means we've got NT 3.51 or 4 */
				oslevel = OSLEVEL_WINNT;
			}
			else if(   (verInfo.dwMajorVersion == 5)
				&& (verInfo.dwMinorVersion == 0))
			{
				/* We've got Win2k */
				oslevel = OSLEVEL_WIN2K;
			}
			else if(   (verInfo.dwMajorVersion == 5)
				&& (verInfo.dwMinorVersion == 1))
			{
				/* We've got Windows XP */
				oslevel = OSLEVEL_WINXP;
			}
			else if(   (verInfo.dwMajorVersion == 5)
				&& (verInfo.dwMinorVersion == 2))
			{
				/* We've got Windows 2003 Server */
				oslevel = OSLEVEL_WIN2K3;
			}
			else if(   (verInfo.dwMajorVersion == 6)
				&& (verInfo.dwMinorVersion == 0))
			{
				/* We've got Windows Vista */
				oslevel = OSLEVEL_WINVISTA;
			}
			else if(   (verInfo.dwMajorVersion == 6)
				&& (verInfo.dwMinorVersion == 1))
			{
				/* We've got Windows 7 */
				oslevel = OSLEVEL_WIN7;
			}
			else if ((verInfo.dwMajorVersion == 6)
				&& (verInfo.dwMinorVersion == 2))
			{
				/* We've got Windows 10 */
				oslevel = OSLEVEL_WIN10;
			}
			else
			{
				/* Must be some crazy new microsoft product =) default to NT so we treat it as and OS with services */
				oslevel = OSLEVEL_WINNT;
			}
		}
		else if(verInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			if(   (verInfo.dwMajorVersion == 4)
				&& (verInfo.dwMinorVersion == 0))
			{
				/* win95, win95 OSR2 2.5, etc*/
				oslevel = OSLEVEL_WIN95;
			}
			else if(   (verInfo.dwMajorVersion == 4)
				&& (verInfo.dwMinorVersion == 10))
			{
				/* win98, win98 SE */
				oslevel = OSLEVEL_WIN98;
			}
			else if(   (verInfo.dwMajorVersion == 4)
				&& (verInfo.dwMinorVersion == 90))
			{
				/* This would be win ME */
				oslevel = OSLEVEL_WINME;
			}
			else
			{
				/* unknown - treat as win95 for now*/
				oslevel = OSLEVEL_WIN95;
			}
		}
		else
		{
			/* we don't know what this version is, could be Win32s (like win3.11 uggghhh)  */
			/* we don't support that so, set it to unknown */
			oslevel = OSLEVEL_UNKNOWN;
		}
	}
	return oslevel;
}


/* DO NOT MODIFY RETURNED STRINGS */
char *getNameForOSLevel(int oslevel)
{
	static char win95[]="Windows 95";
	static char win98[]="Windows 98";
	static char winME[]="Windows ME";
	static char winNT[]="Windows NT";
	static char win2K[]="Windows 2000";
	static char winXP[]="Windows XP";
	static char win2K3[]="Windows 2003 Server";
	static char winVista[]="Windows Vista";
	static char win7[]="Windows 7";
	static char win10[]="Windows 10";
	static char unknown[]="UNKNOWN OS VERSION";

	switch(oslevel)
	{
	case OSLEVEL_WIN95:
		return win95;
	case OSLEVEL_WIN98:
		return win98;
	case OSLEVEL_WINME:
		return winME;
	case OSLEVEL_WINNT:
		return winNT;
	case OSLEVEL_WIN2K:
		return win2K;
	case OSLEVEL_WINXP:
		return winXP;
	case OSLEVEL_WIN2K3:
		return win2K3;
	case OSLEVEL_WINVISTA:
		return winVista;
	case OSLEVEL_WIN7:
		return win7;
	case OSLEVEL_WIN10:
		return win10;
	default:
		return unknown;
	}
}