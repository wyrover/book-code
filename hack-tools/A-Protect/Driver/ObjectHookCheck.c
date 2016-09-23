#include "ObjectHookCheck.h"

VOID ObjectHookCheckInfo(ULONG ulRealProcedure,ULONG ulProcedure,PVOID object_type,int i,POBJECTHOOKINFO ObjectHookInfo,char *lpszFunction,char *lpszObjectTypeName,int HookType)
{
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	PUCHAR ulHookAddress;

	ObjectHookInfo->ObjectHook[i].ulMemoryFunctionBase = ulRealProcedure;
	ObjectHookInfo->ObjectHook[i].ulMemoryHookBase = ulProcedure;
	ObjectHookInfo->ObjectHook[i].ulHookType = HookType;
	ObjectHookInfo->ObjectHook[i].ulObjectTypeBase = (ULONG)object_type;
	memset(ObjectHookInfo->ObjectHook[i].lpszFunction,0,sizeof(ObjectHookInfo->ObjectHook[i].lpszFunction));
	strcat(ObjectHookInfo->ObjectHook[i].lpszFunction,lpszFunction);

	memset(ObjectHookInfo->ObjectHook[i].lpszObjectTypeName,0,sizeof(ObjectHookInfo->ObjectHook[i].lpszObjectTypeName));
	strcat(ObjectHookInfo->ObjectHook[i].lpszObjectTypeName,lpszObjectTypeName);

	//简单处理下头部hook
	if (HookType == IsObjectInlineHook)
	{
		ulHookAddress = *(PULONG)(ulProcedure+1)+(ULONG)(ulProcedure+5);

		//简单处理一下二级跳
		if (*ulHookAddress == 0xe9 ||
			*ulHookAddress == 0xe8)
		{
			if (DebugOn)
				KdPrint(("ulHookAddress == 0xe9"));
			ulHookAddress = *(PULONG)(ulHookAddress+1)+(ULONG)(ulHookAddress+5);
		}
		ulProcedure = ulHookAddress;
	}

	memset(ObjectHookInfo->ObjectHook[i].lpszHookModuleImage,0,sizeof(ObjectHookInfo->ObjectHook[i].lpszHookModuleImage));
	if (!IsAddressInSystem(
		ulProcedure,
		&ulHookModuleBase,
		&ulHookModuleSize,
		ObjectHookInfo->ObjectHook[i].lpszHookModuleImage))
	{
		strcat(ObjectHookInfo->ObjectHook[i].lpszHookModuleImage,"Unknown");
	}
}
PVOID GetRegObejctType()
{
	UNICODE_STRING RegPath;
	OBJECT_ATTRIBUTES oba;
	HANDLE RegKeyHandle;
	NTSTATUS status;
	PVOID KeyObject;
	PVOID CmpKeyObjectType = NULL;
	WIN_VER_DETAIL WinVer;
	UNICODE_STRING uniFunctionName;
	OBGETOBJECTTYPE MyGetObjectType;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwOpenKey,L"ZwOpenKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RMmGetSystemRoutineAddress,L"MmGetSystemRoutineAddress",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RZwOpenKey &&
		RObReferenceObjectByHandle &&
		RZwClose &&
		RMmGetSystemRoutineAddress)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	RRtlInitUnicodeString(&RegPath, L"\\Registry\\Machine\\System");
	InitializeObjectAttributes( 
		&oba , 
		&RegPath , 
		OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE , 
		0 , 
		0
		);

	RegKeyHandle=0;
	status=RZwOpenKey(
		&RegKeyHandle,
		KEY_QUERY_VALUE,
		&oba
		);
	if (!NT_SUCCESS(status ))
	{
		KdPrint(("open the system key failed!\n"));
		return NULL;
	}
	status=RObReferenceObjectByHandle(
		RegKeyHandle,
		GENERIC_READ,
		NULL,
		KernelMode,
		&KeyObject,
		0
		);
	if (!NT_SUCCESS(status ))
	{
		KdPrint(("reference the key object failed!\n"));
		RZwClose(RegKeyHandle);
		return NULL;
	}
	WinVer=GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		RRtlInitUnicodeString(&uniFunctionName,L"ObGetObjectType");
		MyGetObjectType=(OBGETOBJECTTYPE)RMmGetSystemRoutineAddress(&uniFunctionName);  //xp~2008下无此函数，如果直接调用，则导致驱动加载失败，因此需要动态获取地址
		if(MyGetObjectType)
		{
			CmpKeyObjectType = (ULONG)MyGetObjectType((PVOID)KeyObject);
			/*
			lkd> dt_OBJECT_TYPE 855c1660
			nt!_OBJECT_TYPE
			+0x000 TypeList         : _LIST_ENTRY [ 0x855c1660 - 0x855c1660 ]
			+0x008 Name             : _UNICODE_STRING "Key"
			+0x010 DefaultObject    : 0x83d57ba0 
			+0x014 Index            : 0x23 '#'
			+0x018 TotalNumberOfObjects : 0x3cc
			+0x01c TotalNumberOfHandles : 0x3c7
			+0x020 HighWaterNumberOfObjects : 0x45a
			+0x024 HighWaterNumberOfHandles : 0x455
			+0x028 TypeInfo         : _OBJECT_TYPE_INITIALIZER
			+0x078 TypeLock         : _EX_PUSH_LOCK
			+0x07c Key              : 0x2079654b
			+0x080 CallbackList     : _LIST_ENTRY [ 0x855c16e0 - 0x855c16e0 ]
			*/
		}
	}else
	{
		__try
		{
			_asm
			{
				push eax
				mov eax,KeyObject
				mov eax,[eax-0x10]
				mov CmpKeyObjectType,eax
				pop eax
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("GetRegObejctType:%08x\r\n",GetExceptionCode()));
			CmpKeyObjectType = NULL;
		}
	}
	ObDereferenceObject(KeyObject);
	RZwClose(RegKeyHandle);
	return CmpKeyObjectType;
}
VOID IoFileObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo)
{
	WIN_VER_DETAIL WinVer;
	ULONG ulFileCloseProcedure;
	ULONG ulFileDeleteProcedure;
	ULONG ulFileParseProcedure;
	ULONG ulFileQueryNameProcedure;

	int i = 0;
	BOOL bObjectHooked = FALSE;
	ULONG ulReloadObjectType;
	POBJECT_TYPE object_type;
	POBJECT_TYPE_WIN7 object_type_win7;

	ULONG ulRealFileCloseProcedure;
	ULONG ulReloadFileCloseProcedure;

	ULONG ulRealFileDeleteProcedure;
	ULONG ulReloadFileDeleteProcedure;

	ULONG ulRealFileProcedure;
	ULONG ulReloadFileProcedure;

	ULONG ulRealFileQueryNameProcedure;
	ULONG ulReloadFileQueryNameProcedure;
	int hooktype;

	WinVer = GetWindowsVersion();

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		object_type = *IoFileObjectType;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		object_type_win7 = *IoFileObjectType;
	}
//--------------------------------------------------------------------------
		bObjectHooked = FALSE;
		ulObjectHookCount = 0;

		if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			//计算reload后的ObjectType
			ulFileCloseProcedure = object_type->TypeInfo.CloseProcedure;
		}
		else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		{
			ulFileCloseProcedure = object_type_win7->TypeInfo.CloseProcedure;
		}
		ulReloadFileCloseProcedure = ulFileCloseProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

		if (ulFileCloseProcedure > SystemKernelModuleBase &&
			ulFileCloseProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
		{
			bObjectHooked = TRUE;
		}
		hooktype = NoObjectHook;
		ulRealFileCloseProcedure = ulFileCloseProcedure;

		if (bObjectHooked == FALSE)
		{
			hooktype = IsObjectHook;
			ulRealFileCloseProcedure = 0;

		}else
		{
			//检查是否inline hook
			if (GetFunctionCodeSize(ulFileCloseProcedure) == GetFunctionCodeSize(ulReloadFileCloseProcedure) &&
				memcmp(ulFileCloseProcedure,ulReloadFileCloseProcedure,GetFunctionCodeSize(ulFileCloseProcedure)) != NULL)
			{
				hooktype = IsObjectInlineHook;
			}
		}
		if (!object_type)
		{
			object_type = object_type_win7;
		}
		ObjectHookCheckInfo(ulRealFileCloseProcedure,ulFileCloseProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopCloseProcedure","IoFileObjectType",hooktype);

//----------------------------------------------------------------------------------------
		bObjectHooked = FALSE;
		ulObjectHookCount++;

		if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			ulFileDeleteProcedure = object_type->TypeInfo.DeleteProcedure;
		}
		else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		{
			ulFileDeleteProcedure = object_type_win7->TypeInfo.DeleteProcedure;

		}
		ulReloadFileDeleteProcedure = ulFileDeleteProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

		if (ulFileDeleteProcedure > SystemKernelModuleBase &&
			ulFileDeleteProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
		{
			bObjectHooked = TRUE;
		}
		hooktype = NoObjectHook;
		ulRealFileDeleteProcedure = ulFileDeleteProcedure;

		if (bObjectHooked == FALSE)
		{
			hooktype = IsObjectHook;
			ulRealFileDeleteProcedure = 0;

		}else
		{
			if (GetFunctionCodeSize(ulFileDeleteProcedure) == GetFunctionCodeSize(ulReloadFileCloseProcedure) &&
				memcmp(ulFileDeleteProcedure,ulReloadFileDeleteProcedure,GetFunctionCodeSize(ulFileDeleteProcedure)) != NULL)
			{
				hooktype = IsObjectInlineHook;
			}
		}
		if (!object_type)
		{
			object_type = object_type_win7;
		}
		ObjectHookCheckInfo(ulRealFileDeleteProcedure,ulFileDeleteProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopDeleteProcedure","IoFileObjectType",hooktype);
//-----------------------------------------------------------------------------------------------
		bObjectHooked = FALSE;
		ulObjectHookCount++;

		if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			ulFileParseProcedure = object_type->TypeInfo.ParseProcedure;
		}
		else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		{
			ulFileParseProcedure = object_type_win7->TypeInfo.ParseProcedure;
		}
		ulReloadFileProcedure = ulFileParseProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

		if (ulFileParseProcedure > SystemKernelModuleBase &&
			ulFileParseProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
		{
			bObjectHooked = TRUE;
		}
		hooktype = NoObjectHook;
		ulRealFileProcedure = ulFileParseProcedure;

		if (bObjectHooked == FALSE)
		{
			hooktype = IsObjectHook;
			ulRealFileProcedure = 0;

		}else
		{
			if (GetFunctionCodeSize(ulFileParseProcedure) == GetFunctionCodeSize(ulReloadFileProcedure) &&
				memcmp(ulFileParseProcedure,ulReloadFileProcedure,GetFunctionCodeSize(ulFileParseProcedure)) != NULL)
			{
				hooktype = IsObjectInlineHook;
			}
		}
		if (!object_type)
		{
			object_type = object_type_win7;
		}
		ObjectHookCheckInfo(ulRealFileProcedure,ulFileParseProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopProcedure","IoFileObjectType",hooktype);
//-----------------------------------------------------------------------------------------------
		bObjectHooked = FALSE;
		ulObjectHookCount++;

		if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			ulFileQueryNameProcedure = object_type->TypeInfo.QueryNameProcedure;
		}
		else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		{
			ulFileQueryNameProcedure = object_type_win7->TypeInfo.QueryNameProcedure;
		}
		ulReloadFileQueryNameProcedure = ulFileQueryNameProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

		if (ulFileQueryNameProcedure > SystemKernelModuleBase &&
			ulFileQueryNameProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
		{
			bObjectHooked = TRUE;
		}
		hooktype = NoObjectHook;
		ulRealFileQueryNameProcedure = ulFileQueryNameProcedure;

		if (bObjectHooked == FALSE)
		{
			hooktype = IsObjectHook;
			ulRealFileQueryNameProcedure = 0;

		}else
		{
			if (GetFunctionCodeSize(ulFileQueryNameProcedure) == GetFunctionCodeSize(ulReloadFileQueryNameProcedure) &&
				memcmp(ulFileQueryNameProcedure,ulFileQueryNameProcedure,GetFunctionCodeSize(ulFileQueryNameProcedure)) != NULL)
			{
				hooktype = IsObjectInlineHook;
			}
		}
		if (!object_type)
		{
			object_type = object_type_win7;
		}
		ObjectHookCheckInfo(ulRealFileQueryNameProcedure,ulFileQueryNameProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopQueryName","IoFileObjectType",hooktype);
///////////////////////////////////////////////////////////////////////////////////////////////////////
		ulObjectHookCount++;
		ObjectHookInfo->ulCount = ulObjectHookCount;
}
VOID IoDeviceObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo)
{
	WIN_VER_DETAIL WinVer;

	ULONG ulDeviceDeleteProcedure;
	ULONG ulDeviceParseProcedure;

	int i = 0;
	BOOL bObjectHooked = FALSE;
	ULONG ulReloadObjectType;
	POBJECT_TYPE object_type;
	POBJECT_TYPE_WIN7 object_type_win7;

	ULONG ulRealDeviceDeleteProcedure;
	ULONG ulReloadDeviceDeleteProcedure;

	ULONG ulRealDeviceProcedure;
	ULONG ulReloadDeviceProcedure;

	int hooktype;

	WinVer = GetWindowsVersion();

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		object_type = *IoDeviceObjectType;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		object_type_win7 = *IoDeviceObjectType;
	}
//--------------------------------------------------------------------------
	bObjectHooked = FALSE;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		//计算reload后的ObjectType
		ulDeviceDeleteProcedure = object_type->TypeInfo.DeleteProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulDeviceDeleteProcedure = object_type_win7->TypeInfo.DeleteProcedure;
	}
	ulReloadDeviceDeleteProcedure = ulDeviceDeleteProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulDeviceDeleteProcedure > SystemKernelModuleBase &&
		ulDeviceDeleteProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealDeviceDeleteProcedure = ulDeviceDeleteProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealDeviceDeleteProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulDeviceDeleteProcedure) == GetFunctionCodeSize(ulReloadDeviceDeleteProcedure) &&
			memcmp(ulDeviceDeleteProcedure,ulReloadDeviceDeleteProcedure,GetFunctionCodeSize(ulDeviceDeleteProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealDeviceDeleteProcedure,ulDeviceDeleteProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopDeleteDevice","IoDeviceObjectType",hooktype);
//--------------------------------------------------------------------------
	bObjectHooked = FALSE;
	ulObjectHookCount++;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		//计算reload后的ObjectType
		ulDeviceParseProcedure = object_type->TypeInfo.ParseProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulDeviceParseProcedure = object_type_win7->TypeInfo.ParseProcedure;
	}
	ulReloadDeviceProcedure = ulDeviceParseProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulDeviceParseProcedure > SystemKernelModuleBase &&
		ulDeviceParseProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealDeviceProcedure = ulDeviceParseProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealDeviceProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulDeviceParseProcedure) == GetFunctionCodeSize(ulReloadDeviceProcedure) &&
			memcmp(ulDeviceParseProcedure,ulReloadDeviceProcedure,GetFunctionCodeSize(ulDeviceParseProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealDeviceProcedure,ulDeviceParseProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopParseDevice","IoDeviceObjectType",hooktype);

//---------------------------------------------------------------
	ulObjectHookCount++;
	ObjectHookInfo->ulCount = ulObjectHookCount;
}
VOID IoDriverObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo)
{
	WIN_VER_DETAIL WinVer;

	ULONG ulDriverDeleteProcedure;

	int i = 0;
	BOOL bObjectHooked = FALSE;
	ULONG ulReloadObjectType;
	POBJECT_TYPE object_type;
	POBJECT_TYPE_WIN7 object_type_win7;

	ULONG ulRealDriverDeleteProcedure;
	ULONG ulReloadDriverDeleteProcedure;


	int hooktype;

	WinVer = GetWindowsVersion();

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		object_type = *IoDriverObjectType;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		object_type_win7 = *IoDriverObjectType;
	}
	//--------------------------------------------------------------------------
	bObjectHooked = FALSE;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		//计算reload后的ObjectType
		ulDriverDeleteProcedure = object_type->TypeInfo.DeleteProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulDriverDeleteProcedure = object_type_win7->TypeInfo.DeleteProcedure;
	}
	ulReloadDriverDeleteProcedure = ulDriverDeleteProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulDriverDeleteProcedure > SystemKernelModuleBase &&
		ulDriverDeleteProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealDriverDeleteProcedure = ulDriverDeleteProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealDriverDeleteProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulDriverDeleteProcedure) == GetFunctionCodeSize(ulReloadDriverDeleteProcedure) &&
			memcmp(ulDriverDeleteProcedure,ulReloadDriverDeleteProcedure,GetFunctionCodeSize(ulDriverDeleteProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealDriverDeleteProcedure,ulDriverDeleteProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"IopDeleteDriver","IoDriverObjectType",hooktype);
	//--------------------------------------------------------------------------
	ulObjectHookCount++;
	ObjectHookInfo->ulCount = ulObjectHookCount;
}
///////////////////////////////////////////////////
VOID CmpKeyObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo)
{
	WIN_VER_DETAIL WinVer;
	ULONG ulCmpKeyCloseProcedure;
	ULONG ulCmpKeyDeleteProcedure;
	ULONG ulCmpKeyParseProcedure;
	ULONG ulCmpKeyQueryNameProcedure;

	int i = 0;
	BOOL bObjectHooked = FALSE;
	ULONG ulReloadObjectType;
	POBJECT_TYPE object_type;
	POBJECT_TYPE_WIN7 object_type_win7;

	ULONG ulRealCmpKeyCloseProcedure;
	ULONG ulReloadCmpKeyCloseProcedure;

	ULONG ulRealCmpKeyDeleteProcedure;
	ULONG ulReloadCmpKeyDeleteProcedure;

	ULONG ulRealCmpKeyProcedure;
	ULONG ulReloadCmpKeyProcedure;

	ULONG ulRealCmpKeyQueryNameProcedure;
	ULONG ulReloadCmpKeyQueryNameProcedure;
	int hooktype;

	WinVer = GetWindowsVersion();

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		object_type = (POBJECT_TYPE)GetRegObejctType();
		if (!object_type)
			return;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		object_type_win7 = (POBJECT_TYPE_WIN7)GetRegObejctType();
		if (!object_type_win7)
			return;
	}
	//--------------------------------------------------------------------------
	bObjectHooked = FALSE;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		//计算reload后的ObjectType
		ulCmpKeyCloseProcedure = object_type->TypeInfo.CloseProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulCmpKeyCloseProcedure = object_type_win7->TypeInfo.CloseProcedure;
	}
	ulReloadCmpKeyCloseProcedure = ulCmpKeyCloseProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulCmpKeyCloseProcedure > SystemKernelModuleBase &&
		ulCmpKeyCloseProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealCmpKeyCloseProcedure = ulCmpKeyCloseProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealCmpKeyCloseProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulCmpKeyCloseProcedure) == GetFunctionCodeSize(ulReloadCmpKeyCloseProcedure) &&
			memcmp(ulCmpKeyCloseProcedure,ulReloadCmpKeyCloseProcedure,GetFunctionCodeSize(ulCmpKeyCloseProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealCmpKeyCloseProcedure,ulCmpKeyCloseProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"CmpCloseKeyObject","CmpKeyObjectType",hooktype);

	//----------------------------------------------------------------------------------------
	bObjectHooked = FALSE;
	ulObjectHookCount++;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		ulCmpKeyDeleteProcedure = object_type->TypeInfo.DeleteProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulCmpKeyDeleteProcedure = object_type_win7->TypeInfo.DeleteProcedure;

	}
	ulReloadCmpKeyDeleteProcedure = ulCmpKeyDeleteProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulCmpKeyDeleteProcedure > SystemKernelModuleBase &&
		ulCmpKeyDeleteProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealCmpKeyDeleteProcedure = ulCmpKeyDeleteProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealCmpKeyDeleteProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulCmpKeyDeleteProcedure) == GetFunctionCodeSize(ulReloadCmpKeyCloseProcedure) &&
			memcmp(ulCmpKeyDeleteProcedure,ulReloadCmpKeyDeleteProcedure,GetFunctionCodeSize(ulCmpKeyDeleteProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealCmpKeyDeleteProcedure,ulCmpKeyDeleteProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"CmpDeleteKeyObject","CmpKeyObjectType",hooktype);
	//-----------------------------------------------------------------------------------------------
	bObjectHooked = FALSE;
	ulObjectHookCount++;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		ulCmpKeyParseProcedure = object_type->TypeInfo.ParseProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulCmpKeyParseProcedure = object_type_win7->TypeInfo.ParseProcedure;
	}
	ulReloadCmpKeyProcedure = ulCmpKeyParseProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulCmpKeyParseProcedure > SystemKernelModuleBase &&
		ulCmpKeyParseProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealCmpKeyProcedure = ulCmpKeyParseProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealCmpKeyProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulCmpKeyParseProcedure) == GetFunctionCodeSize(ulReloadCmpKeyProcedure) &&
			memcmp(ulCmpKeyParseProcedure,ulReloadCmpKeyProcedure,GetFunctionCodeSize(ulCmpKeyParseProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealCmpKeyProcedure,ulCmpKeyParseProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"CmpParseKey","CmpKeyObjectType",hooktype);
	//-----------------------------------------------------------------------------------------------
	bObjectHooked = FALSE;
	ulObjectHookCount++;

	if (WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
	{
		ulCmpKeyQueryNameProcedure = object_type->TypeInfo.QueryNameProcedure;
	}
	else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		ulCmpKeyQueryNameProcedure = object_type_win7->TypeInfo.QueryNameProcedure;
	}
	ulReloadCmpKeyQueryNameProcedure = ulCmpKeyQueryNameProcedure - SystemKernelModuleBase + (ULONG)ImageModuleBase;

	if (ulCmpKeyQueryNameProcedure > SystemKernelModuleBase &&
		ulCmpKeyQueryNameProcedure < SystemKernelModuleBase + SystemKernelModuleSize)
	{
		bObjectHooked = TRUE;
	}
	hooktype = NoObjectHook;
	ulRealCmpKeyQueryNameProcedure = ulCmpKeyQueryNameProcedure;

	if (bObjectHooked == FALSE)
	{
		hooktype = IsObjectHook;
		ulRealCmpKeyQueryNameProcedure = 0;

	}else
	{
		if (GetFunctionCodeSize(ulCmpKeyQueryNameProcedure) == GetFunctionCodeSize(ulReloadCmpKeyQueryNameProcedure) &&
			memcmp(ulCmpKeyQueryNameProcedure,ulReloadCmpKeyQueryNameProcedure,GetFunctionCodeSize(ulCmpKeyQueryNameProcedure)) != NULL)
		{
			hooktype = IsObjectInlineHook;
		}
	}
	if (!object_type)
	{
		object_type = object_type_win7;
	}
	ObjectHookCheckInfo(ulRealCmpKeyQueryNameProcedure,ulCmpKeyQueryNameProcedure,object_type,ulObjectHookCount,ObjectHookInfo,"CmpKeyObjectType","CmpKeyObjectType",hooktype);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	ulObjectHookCount++;
	ObjectHookInfo->ulCount = ulObjectHookCount;
}