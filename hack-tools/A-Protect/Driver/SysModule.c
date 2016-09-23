#include "SysModule.h"

BOOL QueryDriverFromPsLoadedModuleList(PDRIVER_OBJECT DriverObject,PSYSINFO SysModuleInfo)
{
	PLDR_DATA_TABLE_ENTRY LdrDataTable,HideLdrDataTable;
	BOOL bRetOK = FALSE;
	int i = 0;

	__try
	{
		LdrDataTable=(PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
		do 
		{

			//	KdPrint(("%wZ\n",&LdrDataTable->BaseDllName));
			if (LdrDataTable->BaseDllName.Length>0&&LdrDataTable->BaseDllName.Buffer!=NULL)
			{
				if(MmIsAddressValidEx(&LdrDataTable->BaseDllName.Buffer[LdrDataTable->BaseDllName.Length/2-1]))
				{
					SysModuleInfo->SysInfo[i].DriverObject = 0;
					SysModuleInfo->SysInfo[i].SizeOfImage = LdrDataTable->SizeOfImage;
					SysModuleInfo->SysInfo[i].ulSysBase = LdrDataTable->DllBase;
					SysModuleInfo->SysInfo[i].IntHideType = TRUE;

					if (ValidateUnicodeString(&LdrDataTable->FullDllName) &&
						LdrDataTable->FullDllName.Buffer != 0 &&
						LdrDataTable->FullDllName.Length > 0)
					{
						SafeCopyMemory(LdrDataTable->FullDllName.Buffer,SysModuleInfo->SysInfo[i].lpwzFullSysName,LdrDataTable->FullDllName.Length);
					}else{
						memcpy(SysModuleInfo->SysInfo[i].lpwzFullSysName,L"Unknown",wcslen(L"Unknown")*2);
					}

					if (ValidateUnicodeString(&LdrDataTable->BaseDllName) &&
						LdrDataTable->BaseDllName.Buffer != 0 &&
						LdrDataTable->BaseDllName.Length > 0)
					{
						SafeCopyMemory(LdrDataTable->BaseDllName.Buffer,SysModuleInfo->SysInfo[i].lpwzBaseSysName,LdrDataTable->BaseDllName.Length);

					}else{
						memcpy(SysModuleInfo->SysInfo[i].lpwzBaseSysName,L"Unknown",wcslen(L"Unknown")*2);
					}
					SysModuleInfo->ulCount = i;
					i++;
				}
			}
			LdrDataTable=(PLDR_DATA_TABLE_ENTRY)LdrDataTable->InLoadOrderLinks.Flink;

		} while ((PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection!=LdrDataTable&&LdrDataTable!=NULL);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	return bRetOK;
}
//TRUE 正常驱动
//FALSE 隐藏驱动
BOOL MmQueryDriverPath(PDRIVER_OBJECT DriverObject,ULONG ulDriverStart,WCHAR *BaseSysName,WCHAR *FullSysName)
{
	PLDR_DATA_TABLE_ENTRY LdrDataTable,HideLdrDataTable;
	BOOL bRetOK = FALSE;
	int i = 0;

	__try
	{
		LdrDataTable=(PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
		do 
		{

			//	KdPrint(("%wZ\n",&LdrDataTable->BaseDllName));
			if (LdrDataTable->BaseDllName.Length>0&&LdrDataTable->BaseDllName.Buffer!=NULL)
			{
				if(MmIsAddressValidEx(&LdrDataTable->BaseDllName.Buffer[LdrDataTable->BaseDllName.Length/2-1]))
				{
					if (LdrDataTable->DllBase == ulDriverStart)
					{
						if (ValidateUnicodeString(&LdrDataTable->FullDllName) &&
							LdrDataTable->FullDllName.Buffer != 0 &&
							LdrDataTable->FullDllName.Length > 0)
						{
							SafeCopyMemory(LdrDataTable->FullDllName.Buffer,FullSysName,LdrDataTable->FullDllName.Length);
						}else{
							memcpy(FullSysName,L"Unknown",wcslen(L"Unknown")*2);
						}

						if (ValidateUnicodeString(&LdrDataTable->BaseDllName) &&
							LdrDataTable->BaseDllName.Buffer != 0 &&
							LdrDataTable->BaseDllName.Length > 0)
						{
							SafeCopyMemory(LdrDataTable->BaseDllName.Buffer,BaseSysName,LdrDataTable->BaseDllName.Length);

						}else{
							memcpy(BaseSysName,L"Unknown",wcslen(L"Unknown")*2);
						}
						bRetOK = TRUE;
						break;
					}
				}
			}
			LdrDataTable=(PLDR_DATA_TABLE_ENTRY)LdrDataTable->InLoadOrderLinks.Flink;

		} while ((PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection!=LdrDataTable&&LdrDataTable!=NULL);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
	return bRetOK;
}
VOID ScanDriverObject(PDRIVER_OBJECT PDriverObject,PDRIVER_OBJECT_STRUCT DriverStruct)
{
	ULONG ulScan=0;
	ULONG ulCount=0;
	PDRIVER_OBJECT pTmpDriObject;
	PKDDEBUGGER_DATA64 pKdData64;
	BOOL bRetOK = FALSE;
	POBJECT_TYPE pObjectType;
	ULONG ulKey;
	WIN_VER_DETAIL WinVer;
	ULONG ulKeyOffset = 0;
	int i=0,x=0,y=0;
	BOOL bRealDriver = TRUE;
	//KIRQL oldIrql;
	//KSPIN_LOCK    DpcSpinLock;

	//没有重启之前，ulSearchStart作为起始地址应该不会变，所以第二次就不想要重新获取了
	if (bIsRealSearch &&
		MmIsAddressValidEx(ulSearchStart) &&
		ulSearchEnd == ulSearchStart + 0xf000000)
	{
		goto search;
	}
	pKdData64 = KdGetDebuggerDataBlock();
	if (MmIsAddressValidEx(pKdData64)){
		if (MmIsAddressValidEx(pKdData64->MmNonPagedPoolStart)){
				ulSearchStart = *(PULONG)pKdData64->MmNonPagedPoolStart;

				//MmSizeOfPagedPoolInBytes的大小跟start相加之后，总大小会超过0xFFFFFFFF，这样就导致各种问题。
				//因此不用MmSizeOfPagedPoolInBytes作为判断，这样也不效率。
				//任何driver_object的地址绝对不会大于 MmNonPagedPoolStart + 0xf000000，因此取此值作为end

				if (MmIsAddressValidEx(ulSearchStart)){
					ulSearchEnd = ulSearchStart + 0xf000000;
					bRetOK = TRUE;
				}
		}
	}
	if (!bRetOK){
		return;
	}
search:
	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_2K3:
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ulKeyOffset = 0xac;
		break;
	case WINDOWS_VERSION_7_7000:
	case WINDOWS_VERSION_7_7600_UP:
		ulKeyOffset = 0x7c;
		break;
	}

	/*
	lkd> dt_OBJECT_TYPE
	nt!_OBJECT_TYPE
	+0x000 Mutex            : _ERESOURCE
	+0x038 TypeList         : _LIST_ENTRY
	+0x040 Name             : _UNICODE_STRING
	+0x048 DefaultObject    : Ptr32 Void
	+0x04c Index            : Uint4B
	+0x050 TotalNumberOfObjects : Uint4B
	+0x054 TotalNumberOfHandles : Uint4B
	+0x058 HighWaterNumberOfObjects : Uint4B
	+0x05c HighWaterNumberOfHandles : Uint4B
	+0x060 TypeInfo         : _OBJECT_TYPE_INITIALIZER
	+0x0ac Key              : Uint4B
	+0x0b0 ObjectLocks      : [4] _ERESOURCE
	*/
	pObjectType=(POBJECT_TYPE)(*(PULONG)IoDriverObjectType);
	if (!MmIsAddressValidEx((ULONG)pObjectType + ulKeyOffset)){
		return;
	}
	ulKey=*(PULONG)((ULONG)pObjectType + ulKeyOffset)|0x80000000;

	if (DebugOn)
		KdPrint(("start:%08x %08x\n",ulSearchStart,ulSearchEnd));

	i = 0;

	//抢断
	//oldIrql = KeRaiseIrqlToDpcLevel();
	//KeAcquireSpinLockAtDpcLevel(&DpcSpinLock);

	__try
	{
		for(ulScan=ulSearchStart;ulScan< ulSearchEnd;ulScan+=4)
		{
			//这里其实也可以用MmIsAddressValidEx，但是效率好慢，只能用原版RMmIsAddressValid了
			if (MmIsAddressValidEx((PVOID)ulScan))
			{
				if(*(PULONG)ulScan == ulKey)
				{
					//KdPrint(("Search:%s\n",PsGetProcessImageFileName(PsGetCurrentProcess())));

					//假设都是真的驱动对象
					bRealDriver = TRUE;

					pTmpDriObject=NULL;
					pTmpDriObject=(PDRIVER_OBJECT)(ulScan+DriMagic);
					if (!MmIsAddressValidEx(pTmpDriObject) ||
						!MmIsAddressValidEx((pTmpDriObject + 0x38)) ){  //0x38 是 MajorFunction成员的偏移，也是driver_object最后成员，如果可以访问，其他成员也可以
						continue;
					}
					//所有驱动对象的MajorFunction[28]个都是可以访问的，只要有一个不能访问，说明该驱动对象错误
					if (MmIsAddressValidEx(pTmpDriObject->MajorFunction)){
						for (x=0;x<MAX_IRP_MJ_DISPATCH;x++)
						{
							if (!MmIsAddressValidEx(pTmpDriObject->MajorFunction[x])){
								bRealDriver = FALSE;
							}
						}
					}
					if(*(PULONG)(pTmpDriObject) == 0x00a80004 ||
						bRealDriver == TRUE)
					{
						//当这里为TRUE，说明此时的ulSearchStart是正确的
						bIsRealSearch = TRUE;

						//到这里可以确定是驱动了
						if (DebugOn)
							KdPrint(("RealDriver:[%d]%08x\n",i,pTmpDriObject));

						//避免溢出
						if (i > 700)
							goto FuncRet;

						DriverStruct->Struct[i].ulDriverObject = pTmpDriObject;
						i++;
						DriverStruct->ulCount = i;
					}
					ulScan += sizeof(DRIVER_OBJECT);
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		goto FuncRet;
	}
FuncRet:
	//KeReleaseSpinLockFromDpcLevel(&DpcSpinLock);
	//KeLowerIrql(oldIrql);

	return;
}
VOID EnumKernelModule(PDRIVER_OBJECT PDriverObject,PSYSINFO SysModuleInfo)
{
	PDRIVER_OBJECT pTmpDriObject;
	PDRIVER_OBJECT_STRUCT DriverObjectStruct = NULL;
	int i=0;
	int x=0;
	int fix=0;
	BOOL bFoundDriver = FALSE;
	WIN_VER_DETAIL WinVer;
	PEPROCESS VMwareEProcess;

	//先从这里取
	QueryDriverFromPsLoadedModuleList(PDriverObject,SysModuleInfo);
	fix = SysModuleInfo->ulCount;

	//从被动防御结构里取出所有驱动的加载
	for (i=0;i<LogDefenseInfo->ulCount;i++)
	{
		if (LogDefenseInfo->LogDefense[i].Type == 6)
		{
			for (x=0;x<SysModuleInfo->ulCount;x++)
			{
				//驱动的基址要存在，并且存在于系统里
				if (SysModuleInfo->SysInfo[x].ulSysBase == LogDefenseInfo->LogDefense[i].EProcess)
				{
					bFoundDriver = TRUE;
					break;
				}
			}
			//好了，发现隐藏驱动了
			if (!bFoundDriver &&
				MmIsAddressValidEx(LogDefenseInfo->LogDefense[i].EProcess))  //驱动还木有卸载
			{
				fix++;
				SysModuleInfo->SysInfo[fix].DriverObject = 0;
				SysModuleInfo->SysInfo[fix].SizeOfImage = LogDefenseInfo->LogDefense[ulLogCount].ulPID;
				SysModuleInfo->SysInfo[fix].ulSysBase = LogDefenseInfo->LogDefense[i].EProcess;

				SysModuleInfo->SysInfo[fix].IntHideType = FALSE;  //提示无法识别的驱动对象
				memcpy(SysModuleInfo->SysInfo[fix].lpwzFullSysName,L"*",wcslen(L"*")*2);
				memcpy(SysModuleInfo->SysInfo[fix].lpwzBaseSysName,L"*",wcslen(L"*")*2);
				memcpy(SysModuleInfo->SysInfo[fix].lpwzServiceName,L"*",wcslen(L"*")*2);
				SysModuleInfo->ulCount = fix;
			}
		}
	}
// 	//***************************************************************************************
// 	//不是虚拟机的，都不暴力搜索了，草，暴力搜索卡死的还是无法解决，蛋碎一地~~~！！！！
// 	//***************************************************************************************
// 	if (LookupProcessByName("VMwareTray.exe",&VMwareEProcess) != STATUS_SUCCESS)
// 	{
// 		if (DebugOn)
// 			KdPrint(("Is not in VMware\r\n"));
// 
// 		return;
// 	}

	bFoundDriver = FALSE;

	if (DebugOn)
		KdPrint(("KeGetCurrentIrql -> %d\n",KeGetCurrentIrql()));

	if (KeGetCurrentIrql() != PASSIVE_LEVEL){
		return;
	}
	if (DebugOn)
		KdPrint(("%d\n",SysModuleInfo->ulCount));

	//再次暴力枚举driver_object，并且保存到内存，
	//执行到这里，说明RExAllocatePool地址是正确的，直接调用了
	DriverObjectStruct = (PDRIVER_OBJECT_STRUCT)RExAllocatePool(NonPagedPool,sizeof(DRIVER_OBJECT_STRUCT)*264);
	if (!DriverObjectStruct){
		return;
	}
	memset(DriverObjectStruct,0,sizeof(DRIVER_OBJECT_STRUCT)*264);
	ScanDriverObject(PDriverObject,DriverObjectStruct);

	if (DriverObjectStruct->ulCount)
	{
		for (i=0;i<DriverObjectStruct->ulCount;i++)
		{
			pTmpDriObject = (PDRIVER_OBJECT)(DriverObjectStruct->Struct[i].ulDriverObject);
			bFoundDriver = FALSE;

			for (x=0;x<SysModuleInfo->ulCount;x++)
			{
				//找到了
				if (SysModuleInfo->SysInfo[x].ulSysBase == pTmpDriObject->DriverStart)
				{
					SysModuleInfo->SysInfo[x].DriverObject = pTmpDriObject;  //填充driver_object
					//填充服务
					if (ValidateUnicodeString(&pTmpDriObject->DriverName) &&
						pTmpDriObject->DriverName.Buffer != 0 &&
						pTmpDriObject->DriverName.Length > 0)
					{
						SafeCopyMemory(pTmpDriObject->DriverName.Buffer,SysModuleInfo->SysInfo[x].lpwzServiceName,pTmpDriObject->DriverName.Length);
					}else{
						memcpy(SysModuleInfo->SysInfo[x].lpwzServiceName,L"Unknown",wcslen(L"Unknown")*2);
					}
					SysModuleInfo->SysInfo[x].IntHideType = TRUE;
					bFoundDriver = TRUE;
					break;
				}
			}
			//吊你公龟，木有找到
			if (!bFoundDriver){
				fix++;
				SysModuleInfo->SysInfo[fix].DriverObject = pTmpDriObject;
				SysModuleInfo->SysInfo[fix].SizeOfImage = pTmpDriObject->DriverSize;
				SysModuleInfo->SysInfo[fix].ulSysBase = pTmpDriObject->DriverStart;

				SysModuleInfo->SysInfo[fix].IntHideType = FALSE;  //提示无法识别的驱动对象
				memcpy(SysModuleInfo->SysInfo[fix].lpwzFullSysName,L"-",wcslen(L"-")*2);
				memcpy(SysModuleInfo->SysInfo[fix].lpwzBaseSysName,L"-",wcslen(L"-")*2);

				if (ValidateUnicodeString(&pTmpDriObject->DriverName) &&
					pTmpDriObject->DriverName.Buffer != 0 &&
					pTmpDriObject->DriverName.Length > 0)
				{
					SafeCopyMemory(pTmpDriObject->DriverName.Buffer,SysModuleInfo->SysInfo[fix].lpwzServiceName,pTmpDriObject->DriverName.Length);
				}else{
					memcpy(SysModuleInfo->SysInfo[fix].lpwzServiceName,L"Unknown",wcslen(L"Unknown")*2);
				}
				if (DebugOn)
					KdPrint(("can't found :[%d]%x-%ws\n",fix,SysModuleInfo->SysInfo[fix].DriverObject,SysModuleInfo->SysInfo[fix].lpwzServiceName));
			}
		}
		SysModuleInfo->ulCount = fix;
	}
/*
	for (i=0;i<SysModuleInfo->ulCount;i++)
	{
		KdPrint(("[%d]SysModule\r\n"
			"对象:%08x\r\n"
			"基址:%08x\r\n"
			"大小:%x\r\n"
			"驱动名:%ws\r\n"
			"驱动路径:%ws\r\n"
			"服务:%ws\r\n"
			"隐藏类型:%d\r\n\r\n",
			i,
			SysModuleInfo->SysInfo[i].DriverObject,
			SysModuleInfo->SysInfo[i].ulSysBase,
			SysModuleInfo->SysInfo[i].SizeOfImage,
			SysModuleInfo->SysInfo[i].lpwzBaseSysName,
			SysModuleInfo->SysInfo[i].lpwzFullSysName,
			SysModuleInfo->SysInfo[i].lpwzServiceName,
			SysModuleInfo->SysInfo[i].IntHideType
			));
	}
*/
	RExFreePool(DriverObjectStruct);
	return;
}