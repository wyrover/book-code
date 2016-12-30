#ifndef __BUILDCFG_BUILDCFG_H__
#define __BUILDCFG_BUILDCFG_H__

#ifdef _WINDOWS_
#error "include buildcfg.h before include all other files"
#endif
	
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1




#ifdef FILE_2_HEX_EXE
#pragma message("\t   FILE_2_HEX_EXE")
#define VI_FILEDESCRIPTION	"file-2-hex"
#define VI_INTERNALNAME		"file-2-hex"
#define VI_ORIGINALFILENAME	"file-2-hex.exe"
#endif



#endif /* __BUILDCFG_BUILDCFG_H__ */
