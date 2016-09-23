#ifndef CXX_FUNCTION_H
#define CXX_FUNCTION_H

#include <ntifs.h>
CHAR ChangePreMode(PETHREAD EThread);
VOID RecoverPreMode(PETHREAD EThread, CHAR PreMode);
VOID WPOFF();
VOID WPON();

typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation,     //内存基本信息
	MemoryWorkingSetList,
	MemorySectionName           //内存映射文件名信息
}MEMORY_INFORMATION_CLASS;

#endif