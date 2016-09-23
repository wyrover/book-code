/*
IoRegisterFsRegistrationChangeString         文件系统回调

IoRegisterLastChanceShutdownNotification     IopNotifyLastChanceShutdownQueueHead
            
IoRegisterShutdownNotification               IopNotifyShutdownQueueHead

IoRegisterPlugPlayNotification               即插即用

CmRegisterCallback                           注册表

KeRegisterBugCheckCallback                   错误检测
KeRegisterBugCheckReasonCallback 

PsSetCreateProcessNotifyRoutine              PspCreateProcessNotifyRoutine 
PsSetCreateThreadNotifyRoutine               PspCreateThreadNotifyRoutine
PsSetLoadImageNotifyRoutine                  PspLoadImageNotifyRoutine

SeRegisterLogonSessionTerminatedRoutine      注销用户  回调
*/
#include "SystemNotify.h"

//得到函数地址
ULONG GetFunctionAddr( IN PCWSTR FunctionName)
{
	UNICODE_STRING UniCodeFunctionName;
	RtlInitUnicodeString( &UniCodeFunctionName, FunctionName );
	return (ULONG)MmGetSystemRoutineAddress( &UniCodeFunctionName );   
}
void GetCreateProcessNotify(PSYSTEM_NOTIFY SystemNotify)
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)PsSetCreateProcessNotifyRoutine;
	PUCHAR Base;
	PUCHAR i=0,x=0;
	ULONG Address=0;
	PEX_CALLBACK_ROUTINE_BLOCK Point;
	WIN_VER_DETAIL WinVer;
	PUCHAR ulPspSetCreateProcessNotifyRoutine;

	Base=(UCHAR*)GetFunctionAddr(L"PsSetCreateProcessNotifyRoutine");

	if (DebugOn)
		KdPrint(("Base:%08x\r\n",Base));

	if (!MmIsAddressValidEx(Base))
	{
		return ;
	}
	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_2K3_SP1_SP2:
	case WINDOWS_VERSION_XP:
		/*
		805d0c27 56              push    esi
        805d0c28 57              push    edi
        805d0c29 7464            je      nt!PsSetCreateProcessNotifyRoutine+0x73 (805d0c8f)
        805d0c2b bf404a5680      mov     edi,offset nt!PspCreateProcessNotifyRoutine (80564a40)         指令bf 40 4a 56 80即包含地址80564a40
        805d0c30 57              push    edi
        */
		for(i=Base;i<i+SizeOfProc(Base);i++)
		{
			if (DebugOn)
				KdPrint(("[%08x]Base:%08x\r\n",i,Base));

			if(*i == 0xbf)
			{
				Address=*(ULONG*)(i+1);

				if (DebugOn)
					KdPrint(("[%08x]Base:%08x %08x\r\n",i,Base,Address));

				//DbgPrint("进程创建回调函数数组地址:%d\n",Address); //数组是EX_FAST_REF类型
				break;
			}
		}
		break;
	case WINDOWS_VERSION_7_7000:
	case WINDOWS_VERSION_7_7600_UP:
		/*
		83f9c821 33db            xor     ebx,ebx
        83f9c823 c7450ca0a9f483  mov     dword ptr [ebp+0Ch],offset nt!PspCreateProcessNotifyRoutine (83f4a9a0)
        83f9c82a ff750c          push    dword ptr [ebp+0Ch]
*/
		for(i=Base;i<i+SizeOfProc(Base);i++)
		{
			if(*i == 0xe8)
			{
				ulPspSetCreateProcessNotifyRoutine=*(ULONG*)(i+1)+(i+5);

				if (DebugOn)
					KdPrint(("[%08x]ulPspSetCreateProcessNotifyRoutine:%08x\r\n",i,ulPspSetCreateProcessNotifyRoutine));

				if (MmIsAddressValidEx(ulPspSetCreateProcessNotifyRoutine))
				{
					for(x=ulPspSetCreateProcessNotifyRoutine;x<x+SizeOfProc(ulPspSetCreateProcessNotifyRoutine);x++)
					{
						if (DebugOn)
							KdPrint(("[%08x]Base:%08x\r\n",x,Base));

						if (*x == 0xc7)
						{
							//DbgPrint("进程创建回调函数数组地址:%d\n",Address); //数组是EX_FAST_REF类型
							Address=*(ULONG*)(x+3);

							if (DebugOn)
								KdPrint(("[%08x]Base:%08x %08x\r\n",x,ulPspSetCreateProcessNotifyRoutine,Address));
							break;
						}
					}
				}
				break;
			}
		}
		break;
	}
     //KdPrint(("没有找到进程创建回调函数数组\n"));
	 if (!MmIsAddressValidEx(Address)){
		 return ;
	 }
	Ref=(PEX_FAST_REF)(Address);
            
	CreateProcessNum = SystemNotify->ulCount;
	for(i=0;i<8;i++)
	{
		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);//得到存有函数地址的结构
		
		if(MmIsAddressValidEx((PVOID)Point))
		{
			if (DebugOn)
				DbgPrint("回调入口:%X  类型:CreateProcess\n",(ULONG)(Point->Function));

			memset(SystemNotify->NotifyInfo[CreateProcessNum].lpwzObject,0,
				sizeof(SystemNotify->NotifyInfo[CreateProcessNum].lpwzObject));

			swprintf(SystemNotify->NotifyInfo[CreateProcessNum].lpwzObject,
				L"EX_CALLBACK_ROUTINE_BLOCK:0x%08X",Point);

			SystemNotify->NotifyInfo[CreateProcessNum].ulNotifyBase = (ULONG)(Point->Function);
			memset(SystemNotify->NotifyInfo[CreateProcessNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[CreateProcessNum].lpwzType));
			wcscat(SystemNotify->NotifyInfo[CreateProcessNum].lpwzType,L"CreateProcess");

			memset(SystemNotify->NotifyInfo[CreateProcessNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[CreateProcessNum].lpszModule));
			if (!IsAddressInSystem(
				Point->Function,
				&ulModuleBase,
				&ulModuleSize,
				SystemNotify->NotifyInfo[CreateProcessNum].lpszModule))
			{
				strcat(SystemNotify->NotifyInfo[CreateProcessNum].lpszModule,"Unknown");
			}
			CreateProcessNum++;
			SystemNotify->ulCount++;
		}
		Ref++;
	}
	return;
}

void GetCreateThreadNotify(PSYSTEM_NOTIFY SystemNotify)
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)PsSetCreateThreadNotifyRoutine;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"PsSetCreateThreadNotifyRoutine");
	int i=0;
	PEX_CALLBACK_ROUTINE_BLOCK Point;
	ULONG Address;

	if(!MmIsAddressValidEx(Base)){
		return;
	}
	Address=0;

	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0x56)&&(*(UCHAR*)(Base+1)==0xbe))
		{
			Address=*(ULONG*)(Base+2);
			if (DebugOn)
				KdPrint(("线程创建回调函数数组地址:%08x\n",Address)); //数组是EX_FAST_REF类型
			break;
		}
		Base++;
	 }
	 if(!MmIsAddressValidEx(Address)){
        return;
     }
            
	Ref=(PEX_FAST_REF)(Address);
    CreateThreadNum = SystemNotify->ulCount;

	for(i=0;i<8;i++)
	{
		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);//得到存有函数地址的结构
		
		if(MmIsAddressValidEx((PVOID)Point))
		{
			if (DebugOn)
				DbgPrint("回调入口:%X  类型:CreateThread\n",(ULONG)(Point->Function));

			memset(SystemNotify->NotifyInfo[CreateThreadNum].lpwzObject,0,
				sizeof(SystemNotify->NotifyInfo[CreateThreadNum].lpwzObject));

			swprintf(SystemNotify->NotifyInfo[CreateThreadNum].lpwzObject,
				L"EX_CALLBACK_ROUTINE_BLOCK:0x%08X",Point);

			SystemNotify->NotifyInfo[CreateThreadNum].ulNotifyBase = (ULONG)(Point->Function);
			memset(SystemNotify->NotifyInfo[CreateThreadNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[CreateThreadNum].lpwzType));
			wcscat(SystemNotify->NotifyInfo[CreateThreadNum].lpwzType,L"CreateThread");

			memset(SystemNotify->NotifyInfo[CreateThreadNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[CreateThreadNum].lpszModule));
			if (!IsAddressInSystem(
				Point->Function,
				&ulModuleBase,
				&ulModuleSize,
				SystemNotify->NotifyInfo[CreateThreadNum].lpszModule))
			{
				strcat(SystemNotify->NotifyInfo[CreateThreadNum].lpszModule,"Unknown");
			}
			CreateThreadNum++;
			SystemNotify->ulCount++;
		}
		Ref++;
	}
	return;
}

void GetLoadImageNotify(PSYSTEM_NOTIFY SystemNotify)
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)PsSetLoadImageNotifyRoutine;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"PsSetLoadImageNotifyRoutine");
	int i=0;
	ULONG Address;
	PEX_CALLBACK_ROUTINE_BLOCK Point;
	WIN_VER_DETAIL WinVer;

	Address=0;
	for(i=0;i<0x512;i++)
	{
		WinVer = GetWindowsVersion();
		switch(WinVer)
		{
		case WINDOWS_VERSION_2K3_SP1_SP2:
		case WINDOWS_VERSION_XP:
			if((*(UCHAR*)Base==0x56)&&(*(UCHAR*)(Base+1)==0xbe))
			{
				Address=*(ULONG*)(Base+2);
				if (DebugOn)
					DbgPrint("加载模块回调函数数组地址:%08x\n",Address); 
				break;
			}
			break;
		case WINDOWS_VERSION_7_7000:
		case WINDOWS_VERSION_7_7600_UP:
			if((*(UCHAR*)Base==0x25)&&(*(UCHAR*)(Base+1)==0xbe))
			{
				Address=*(ULONG*)(Base+2);
				if (DebugOn)
					DbgPrint("加载模块回调函数数组地址:%08x\n",Address); 
				break;
			}
			break;
		}
		Base++;
	 }
	 if(!MmIsAddressValidEx(Address)){
        return;
     }
            
	Ref=(PEX_FAST_REF)(Address);
	LoadImageNum = SystemNotify->ulCount;
        
	for(i=0;i<8;i++)
	{
		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);
		
		if(MmIsAddressValidEx((PVOID)Point))
		{
			if (DebugOn)
				DbgPrint("回调入口:%X  类型:LoadImage\n",(ULONG)(Point->Function));

			memset(SystemNotify->NotifyInfo[LoadImageNum].lpwzObject,0,
				sizeof(SystemNotify->NotifyInfo[LoadImageNum].lpwzObject));

			swprintf(SystemNotify->NotifyInfo[LoadImageNum].lpwzObject,
				L"EX_CALLBACK_ROUTINE_BLOCK:0x%08X",Point);

			SystemNotify->NotifyInfo[LoadImageNum].ulNotifyBase = (ULONG)(Point->Function);
			memset(SystemNotify->NotifyInfo[LoadImageNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[LoadImageNum].lpwzType));
			wcscat(SystemNotify->NotifyInfo[LoadImageNum].lpwzType,L"LoadImage");

			memset(SystemNotify->NotifyInfo[LoadImageNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[LoadImageNum].lpszModule));
			if (!IsAddressInSystem(
				Point->Function,
				&ulModuleBase,
				&ulModuleSize,
				SystemNotify->NotifyInfo[LoadImageNum].lpszModule))
			{
				strcat(SystemNotify->NotifyInfo[LoadImageNum].lpszModule,"Unknown");
			}
			LoadImageNum++;
			SystemNotify->ulCount++;
		}
		Ref++;
	}
	return;
}

void GetRegisterNotify(PSYSTEM_NOTIFY SystemNotify)
{
	PEX_FAST_REF Ref;
	//UCHAR* Base=(UCHAR*)CmRegisterCallback;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"CmRegisterCallback");
	int i=0;
	ULONG Address;
	PEX_CALLBACK_ROUTINE_BLOCK Point;

	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0xc7)&&(*(UCHAR*)(Base+1)==0x45)&&(*(UCHAR*)(Base+2)==0x08))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	if (!MmIsAddressValidEx(Address)){
		return;
	}
            
	Ref=(PEX_FAST_REF)(Address);
            
	RegNum = SystemNotify->ulCount;

	for(i=0;i<8;i++)
	{
		Point=(PEX_CALLBACK_ROUTINE_BLOCK)((Ref->Value>>3)<<3);
		
		if(MmIsAddressValidEx((PVOID)Point))
		{
			if (DebugOn)
				DbgPrint("回调入口:%X  类型:CmpCallBack\n",(ULONG)(Point->Function));

			memset(SystemNotify->NotifyInfo[RegNum].lpwzObject,0,
				sizeof(SystemNotify->NotifyInfo[RegNum].lpwzObject));

			swprintf(SystemNotify->NotifyInfo[RegNum].lpwzObject,
				L"EX_CALLBACK_ROUTINE_BLOCK:0x%08X",Point);

			SystemNotify->NotifyInfo[RegNum].ulNotifyBase = (ULONG)(Point->Function);
			memset(SystemNotify->NotifyInfo[RegNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[RegNum].lpwzType));
			wcscat(SystemNotify->NotifyInfo[RegNum].lpwzType,L"CmpCallBack");

			memset(SystemNotify->NotifyInfo[RegNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[RegNum].lpszModule));
			if (!IsAddressInSystem(
				Point->Function,
				&ulModuleBase,
				&ulModuleSize,
				SystemNotify->NotifyInfo[RegNum].lpszModule))
			{
				strcat(SystemNotify->NotifyInfo[RegNum].lpszModule,"Unknown");
			}
			RegNum++;
			SystemNotify->ulCount++;
		}
		Ref++;
	}
	return;
}
//804fa138 c7400458d75580  mov     dword ptr [eax+4],offset nt!KeBugCheckCallbackListHead (8055d758)
/*
NTKERNELAPI
BOOLEAN
KeRegisterBugCheckCallback (
    __out PKBUGCHECK_CALLBACK_RECORD CallbackRecord,
    __in PKBUGCHECK_CALLBACK_ROUTINE CallbackRoutine,
    __in PVOID Buffer,
    __in ULONG Length,
    __in PUCHAR Component
    )

	typedef struct _KBUGCHECK_CALLBACK_RECORD {
    LIST_ENTRY Entry;
    PKBUGCHECK_CALLBACK_ROUTINE CallbackRoutine;
    PVOID Buffer;
    ULONG Length;
    PUCHAR Component;
    ULONG_PTR Checksum;
    UCHAR State;
} KBUGCHECK_CALLBACK_RECORD, *PKBUGCHECK_CALLBACK_RECORD;

LIST_ENTRY KeBugCheckCallbackListHead;
LIST_ENTRY KeBugCheckReasonCallbackListHead;

*/
void GetBugCheckCallBack(PSYSTEM_NOTIFY SystemNotify)
{
	//UCHAR* Base=(UCHAR*)KeRegisterBugCheckCallback;
	PUCHAR Base=(UCHAR*)GetFunctionAddr(L"KeRegisterBugCheckCallback");
	PUCHAR i=0;
	PKBUGCHECK_CALLBACK_RECORD pBugCheckRecordPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;

	Address=0;

	if (DebugOn)
		KdPrint(("Base:%08x",Base));

	for(i=Base;i<i+SizeOfProc(Base);i++)
	{
		if (DebugOn)
			KdPrint(("i:%08x",i));

		if(*i==0xc6 &&
		   *(i+1)==0x40 &&
		   *(i+2)==0x1c &&
		   *(i+3)==0x01)
		{
			Address=*(ULONG*)(i+6);
			if (DebugOn)
				KdPrint(("Base:%08x Address:%08x\r\n",Base,Address));
			break;
		}
	 }
	 if (!MmIsAddressValidEx(Address)){
		return;
	 }

	 BugCheckNum = SystemNotify->ulCount;

	 pBugCheckRecordPack=Address;
	 pListEntry=&(pBugCheckRecordPack->Entry);
	 pListNext=pListEntry->Flink;
	 {
		pBugCheckRecordPack=pListNext;

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:BugCheckCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,
			L"KBUGCHECK_CALLBACK_RECORD:0x%08X",pBugCheckRecordPack);

		SystemNotify->NotifyInfo[BugCheckNum].ulObject = pBugCheckRecordPack;

		SystemNotify->NotifyInfo[BugCheckNum].ulNotifyBase = (ULONG)(pBugCheckRecordPack->CallbackRoutine);
		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,L"BugCheckCallBack");

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpszModule));
		if (!IsAddressInSystem(
			pBugCheckRecordPack->CallbackRoutine,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[BugCheckNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,"Unknown");
		}
		BugCheckNum++;
		SystemNotify->ulCount++;
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pBugCheckRecordPack=pListNext;

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:BugCheckCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,
			L"KBUGCHECK_CALLBACK_RECORD:0x%08X",pBugCheckRecordPack);

		SystemNotify->NotifyInfo[BugCheckNum].ulObject = pBugCheckRecordPack;

		SystemNotify->NotifyInfo[BugCheckNum].ulNotifyBase = (ULONG)(pBugCheckRecordPack->CallbackRoutine);
		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,L"BugCheckCallBack");

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpszModule));
		if (!IsAddressInSystem(
			pBugCheckRecordPack->CallbackRoutine,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[BugCheckNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,"Unknown");
		}
		BugCheckNum++;
		SystemNotify->ulCount++;
	 }
}

void GetBugCheckReasonCallBack(PSYSTEM_NOTIFY SystemNotify)
{
	//UCHAR* Base=(UCHAR*)KeRegisterBugCheckReasonCallback;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"KeRegisterBugCheckReasonCallback");
	int i=0;
	PKBUGCHECK_CALLBACK_RECORD pBugCheckRecordPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	Address=0;
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)Base==0xc7)&&(*(UCHAR*)(Base+1)==0x40)&&(*(UCHAR*)(Base+2)==0x04))
		{
			Address=*(ULONG*)(Base+3);
			break;
		}
		Base++;
	 }
	if (!MmIsAddressValidEx(Address)){
		return;
	}

	 BugCheckNum = SystemNotify->ulCount;

	 pBugCheckRecordPack=Address;
	 pListEntry=&(pBugCheckRecordPack->Entry);
	 pListNext=pListEntry->Flink;
	 {
		pBugCheckRecordPack=pListNext;

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:BugCheckReasonCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,
			L"KBUGCHECK_CALLBACK_RECORD:0x%08X",pBugCheckRecordPack);

		SystemNotify->NotifyInfo[BugCheckNum].ulObject = pBugCheckRecordPack;

		SystemNotify->NotifyInfo[BugCheckNum].ulNotifyBase = (ULONG)(pBugCheckRecordPack->CallbackRoutine);
		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,L"BugCheckReasonCallBack");

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpszModule));
		if (!IsAddressInSystem(
			pBugCheckRecordPack->CallbackRoutine,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[BugCheckNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,"Unknown");
		}
		BugCheckNum++;
		SystemNotify->ulCount++;
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pBugCheckRecordPack=pListNext;

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:BugCheckReasonCallBack  包地址:%X\n",pBugCheckRecordPack->CallbackRoutine,pBugCheckRecordPack);

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[BugCheckNum].lpwzObject,
			L"KBUGCHECK_CALLBACK_RECORD:0x%08X",pBugCheckRecordPack);

		SystemNotify->NotifyInfo[BugCheckNum].ulObject = pBugCheckRecordPack;

		SystemNotify->NotifyInfo[BugCheckNum].ulNotifyBase = (ULONG)(pBugCheckRecordPack->CallbackRoutine);
		memset(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[BugCheckNum].lpwzType,L"BugCheckReasonCallBack");

		memset(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[BugCheckNum].lpszModule));
		if (!IsAddressInSystem(
			pBugCheckRecordPack->CallbackRoutine,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[BugCheckNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[BugCheckNum].lpszModule,"Unknown");
		}
		//KdPrint(("[%d]%s\r\n",SystemNotify->ulCount,SystemNotify->NotifyInfo[BugCheckNum].lpszModule));
		BugCheckNum++;
		SystemNotify->ulCount++;
	 }
}
void GetFileSystemNotifyCallBack(PSYSTEM_NOTIFY SystemNotify)
{
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	PNOTIFICATION_PACKET pNotificationPacket;

	PDRIVER_OBJECT Driver_Object_Temp;//用于遍历文件系统回调时保存遍历到的驱动对象以便比较
	
	GetListHeadAddr();
	if (!MmIsAddressValidEx(IopFsNotifyChangeQueueHead)){
		return;
	}
	if (DebugOn)
		KdPrint(("IopFsNotifyChangeQueueHead:%08x\r\n",IopFsNotifyChangeQueueHead));

	pNotificationPacket=IopFsNotifyChangeQueueHead;
	pListEntry=&(pNotificationPacket->ListEntry);
	if (!MmIsAddressValidEx(pListEntry)){
		return;
	}
	pListEntry=pListEntry->Flink;
	if (!MmIsAddressValidEx(pListEntry)){
		return;
	}
	pListNext=pListEntry;

	FsNotifyNum = SystemNotify->ulCount;

	Driver_Object_Temp=*(ULONG*)((ULONG)(pListNext)+8);
	{
		if (!MmIsAddressValidEx(((ULONG)(pListNext)+12))){
			return;
		}
		FileSystemRoutine=*(ULONG*)((ULONG)(pListNext)+12);

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:FsNotifyChange  驱动对象:%X  驱动名称:%wZ  包地址:%X\n",FileSystemRoutine,(ULONG)Driver_Object_Temp,&Driver_Object_Temp->DriverName,(ULONG)pListNext);

		memset(SystemNotify->NotifyInfo[FsNotifyNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[FsNotifyNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[FsNotifyNum].lpwzObject,
			L"DEVICE_OBJECT:0x%08X",(ULONG)Driver_Object_Temp);

		SystemNotify->NotifyInfo[FsNotifyNum].ulNotifyBase = (ULONG)(FileSystemRoutine);
		memset(SystemNotify->NotifyInfo[FsNotifyNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[FsNotifyNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[FsNotifyNum].lpwzType,L"FsNotifyChange");

		memset(SystemNotify->NotifyInfo[FsNotifyNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[FsNotifyNum].lpszModule));
		if (!IsAddressInSystem(
			FileSystemRoutine,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[FsNotifyNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[FsNotifyNum].lpszModule,"Unknown");
		}
		FsNotifyNum++;
		SystemNotify->ulCount++;
	}
	if(!MmIsAddressValidEx((PVOID)pListNext))
	{
		return;
	}
	if(!MmIsAddressValidEx((PVOID)pListNext->Flink))
	{
		return;
	}
	if(!MmIsAddressValidEx((PVOID)pListNext->Flink->Flink))
	{
		return;
	}
	while(pListNext->Flink->Flink!=pListEntry)
	{
		pListNext=pListNext->Flink;
	    Driver_Object_Temp=*(ULONG*)((ULONG)(pListNext)+8);
		{
			if (!MmIsAddressValidEx(((ULONG)(pListNext)+12))){
				continue;
			}
			FileSystemRoutine=*(ULONG*)((ULONG)(pListNext)+12);

			if (DebugOn)
				DbgPrint("回调入口:%X  类型:FsNotifyChange  驱动对象:%X  驱动名称:%wZ  包地址:%X\n",FileSystemRoutine,(ULONG)Driver_Object_Temp,&Driver_Object_Temp->DriverName,(ULONG)pListNext);

			memset(SystemNotify->NotifyInfo[FsNotifyNum].lpwzObject,0,
				sizeof(SystemNotify->NotifyInfo[FsNotifyNum].lpwzObject));

			swprintf(SystemNotify->NotifyInfo[FsNotifyNum].lpwzObject,
				L"DEVICE_OBJECT:0x%08X",(ULONG)Driver_Object_Temp);


			SystemNotify->NotifyInfo[FsNotifyNum].ulNotifyBase = (ULONG)(FileSystemRoutine);
			memset(SystemNotify->NotifyInfo[FsNotifyNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[FsNotifyNum].lpwzType));
			wcscat(SystemNotify->NotifyInfo[FsNotifyNum].lpwzType,L"FsNotifyChange");

			memset(SystemNotify->NotifyInfo[FsNotifyNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[FsNotifyNum].lpszModule));
			if (!IsAddressInSystem(
				FileSystemRoutine,
				&ulModuleBase,
				&ulModuleSize,
				SystemNotify->NotifyInfo[FsNotifyNum].lpszModule))
			{
				strcat(SystemNotify->NotifyInfo[FsNotifyNum].lpszModule,"Unknown");
			}
			FsNotifyNum++;
			SystemNotify->ulCount++;
	    }
	}
}
/*
LIST_ENTRY IopNotifyShutdownQueueHead;
LIST_ENTRY IopNotifyLastChanceShutdownQueueHead;

typedef struct _SHUTDOWN_PACKET {
    LIST_ENTRY ListEntry;
    PDEVICE_OBJECT DeviceObject;
} SHUTDOWN_PACKET, *PSHUTDOWN_PACKET;
*/
//#define IRP_MJ_SHUTDOWN                 0x10
//根据设备对象得到ShutDwon例程地址
ULONG GetNotifyAddr(PDEVICE_OBJECT Device_Object)
{
	ULONG Addr;
	PDRIVER_OBJECT Driver_Object;
	Driver_Object=Device_Object->DriverObject;
	Addr=*(ULONG*)((ULONG)Driver_Object+0x38+0x40);
	return Addr;
}
void GetShutDownCallBack(PSYSTEM_NOTIFY SystemNotify)
{
	//UCHAR* Base=(UCHAR*)IoRegisterShutdownNotification;
	UCHAR* Base=(UCHAR*)GetFunctionAddr(L"IoRegisterShutdownNotification");
	int i=0;
	ULONG NotifyAddr;//回调函数地址
	PSHUTDOWN_PACKET pShutDownPack;
	PLIST_ENTRY pListEntry;
	PLIST_ENTRY pListNext;
	ULONG Address;
	WIN_VER_DETAIL WinVer;

	Address=0;
	for(i=0;i<0x512;i++)
	{
		WinVer = GetWindowsVersion();
		switch(WinVer)
		{
		case WINDOWS_VERSION_2K3_SP1_SP2:
		case WINDOWS_VERSION_XP:
			if((*(UCHAR*)Base==0x8b)&&(*(UCHAR*)(Base+1)==0xd7)&&(*(UCHAR*)(Base+2)==0xb9))
			{
				Address=*(ULONG*)(Base+3);
				break;
			}
			break;
		case WINDOWS_VERSION_7_7000:
			if((*(UCHAR*)Base==0xf0)&&(*(UCHAR*)(Base+1)==0xff)&&(*(UCHAR*)(Base+2)==0xbf))
			{
				Address=*(ULONG*)(Base+3);
				break;
			}
			break;
		case WINDOWS_VERSION_7_7600_UP:
			if((*(UCHAR*)Base==0xef)&&(*(UCHAR*)(Base+1)==0xff)&&(*(UCHAR*)(Base+2)==0xbf))
			{
				Address=*(ULONG*)(Base+3);
				break;
			}
			break;
		}
		Base++;
	 }
	if (!MmIsAddressValidEx(Address)){
		return;
	}
	if (DebugOn)
		DbgPrint("获取ShutDown回调数组:%08x\n",Address);

	 pShutDownPack=Address;
	 pListEntry=&(pShutDownPack->ListEntry);

	 ShutDownNum = SystemNotify->ulCount;

	 pListNext=pListEntry->Flink;
	 {
		pShutDownPack=pListNext;
		NotifyAddr=GetNotifyAddr(pShutDownPack->DeviceObject);

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:ShutDown  设备对象:%X\n",NotifyAddr,pShutDownPack->DeviceObject);

		memset(SystemNotify->NotifyInfo[ShutDownNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[ShutDownNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[ShutDownNum].lpwzObject,
			L"DEVICE_OBJECT:0x%08X",(ULONG)pShutDownPack->DeviceObject);

		SystemNotify->NotifyInfo[ShutDownNum].ulObject = (ULONG)pShutDownPack->DeviceObject;

		SystemNotify->NotifyInfo[ShutDownNum].ulNotifyBase = (ULONG)(NotifyAddr);
		memset(SystemNotify->NotifyInfo[ShutDownNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[ShutDownNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[ShutDownNum].lpwzType,L"ShutDown");

		memset(SystemNotify->NotifyInfo[ShutDownNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[ShutDownNum].lpszModule));
		if (!IsAddressInSystem(
			NotifyAddr,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[ShutDownNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[ShutDownNum].lpszModule,"Unknown");
		}
		ShutDownNum++;
		SystemNotify->ulCount++;
	 }
	 while(pListNext->Flink!=pListEntry)
	 {
		pListNext=pListNext->Flink;
	    pShutDownPack=pListNext;
		NotifyAddr=GetNotifyAddr(pShutDownPack->DeviceObject);

		if (DebugOn)
			DbgPrint("回调入口:%X  类型:ShutDown  设备对象:%X\n",NotifyAddr,pShutDownPack->DeviceObject);

		memset(SystemNotify->NotifyInfo[ShutDownNum].lpwzObject,0,
			sizeof(SystemNotify->NotifyInfo[ShutDownNum].lpwzObject));

		swprintf(SystemNotify->NotifyInfo[ShutDownNum].lpwzObject,
			L"DEVICE_OBJECT:0x%08X",(ULONG)pShutDownPack->DeviceObject);

		SystemNotify->NotifyInfo[ShutDownNum].ulObject = (ULONG)pShutDownPack->DeviceObject;


		SystemNotify->NotifyInfo[ShutDownNum].ulNotifyBase = (ULONG)(NotifyAddr);
		memset(SystemNotify->NotifyInfo[ShutDownNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[ShutDownNum].lpwzType));
		wcscat(SystemNotify->NotifyInfo[ShutDownNum].lpwzType,L"ShutDown");

		memset(SystemNotify->NotifyInfo[ShutDownNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[ShutDownNum].lpszModule));
		if (!IsAddressInSystem(
			NotifyAddr,
			&ulModuleBase,
			&ulModuleSize,
			SystemNotify->NotifyInfo[ShutDownNum].lpszModule))
		{
			strcat(SystemNotify->NotifyInfo[ShutDownNum].lpszModule,"Unknown");
		}
		ShutDownNum++;
		SystemNotify->ulCount++;
	 }
}
//注销  回调
void GetLeaveSessionCallback(PSYSTEM_NOTIFY SystemNotify)
{
	UCHAR* Base;
	int i=0;
	ULONG Address;
	
	PSEP_LOGON_SESSION_TERMINATED_NOTIFICATION CallbackList=NULL;
	PSEP_LOGON_SESSION_TERMINATED_NOTIFICATION TmpCallbackList=NULL;
	Address=0;
	Base=(UCHAR*)GetFunctionAddr(L"SeRegisterLogonSessionTerminatedRoutine");
	for(i=0;i<0x512;i++)
	{
		if((*(UCHAR*)(Base)==0xff)&&(*(UCHAR*)(Base+1)==0xa1))
		{
			Address=*(ULONG*)(Base+2);
			break;
		}
		Base++;
	 }
	if (!MmIsAddressValidEx(Address)){
		return;
	}
	 if (DebugOn)
		DbgPrint("获取注销回调:%08x\n",Address);
	 
	 CallbackList=Address;
	 if(CallbackList->Next==0)
	 {
		 ;
	 }
	 else
	 {
		 TmpCallbackList=CallbackList->Next;

		 LeaveSessionNum = SystemNotify->ulCount;

		 while(1)
		 {
			 if (DebugOn)
				 DbgPrint("回调入口:%X  类型:SeFileSystem\n",(ULONG)TmpCallbackList->CallbackRoutine);

			 memset(SystemNotify->NotifyInfo[LeaveSessionNum].lpwzObject,0,
				 sizeof(SystemNotify->NotifyInfo[LeaveSessionNum].lpwzObject));

			 swprintf(SystemNotify->NotifyInfo[LeaveSessionNum].lpwzObject,
				 L"SEP_LOGON_SESSION_TERMINATED_NOTIFICATION:0x%08X",(ULONG)TmpCallbackList);

			 SystemNotify->NotifyInfo[LeaveSessionNum].ulObject = (ULONG)TmpCallbackList;

			 SystemNotify->NotifyInfo[LeaveSessionNum].ulNotifyBase = (ULONG)(TmpCallbackList->CallbackRoutine);
			 memset(SystemNotify->NotifyInfo[LeaveSessionNum].lpwzType,0,sizeof(SystemNotify->NotifyInfo[LeaveSessionNum].lpwzType));
			 wcscat(SystemNotify->NotifyInfo[LeaveSessionNum].lpwzType,L"SeFileSystem");

			 memset(SystemNotify->NotifyInfo[LeaveSessionNum].lpszModule,0,sizeof(SystemNotify->NotifyInfo[LeaveSessionNum].lpszModule));
			 if (!IsAddressInSystem(
				 TmpCallbackList->CallbackRoutine,
				 &ulModuleBase,
				 &ulModuleSize,
				 SystemNotify->NotifyInfo[LeaveSessionNum].lpszModule))
			 {
				 strcat(SystemNotify->NotifyInfo[LeaveSessionNum].lpszModule,"Unknown");
			 }
			 LeaveSessionNum++;
			 SystemNotify->ulCount++;

			 if(TmpCallbackList->Next==0)
			 {
				 break;
			 }
			 TmpCallbackList=TmpCallbackList->Next;
		 }
	 }
}
//得到文件系统回调链表头地址
VOID GetListHeadAddr()
{ 
	PUCHAR Addr;
	PUCHAR x,i;
	UNICODE_STRING IoRegisterFsRegistrationChangeString={0};
	PVOID EnumParameter;
	WIN_VER_DETAIL WinVer; 
	ULONG ulIoRegisterFsRegistrationChangeMountAware;

	RtlInitUnicodeString(&IoRegisterFsRegistrationChangeString, L"IoRegisterFsRegistrationChange");
	Addr=MmGetSystemRoutineAddress(&IoRegisterFsRegistrationChangeString);

	IopFsNotifyChangeQueueHead = NULL;

	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_2K3_SP1_SP2:
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_7_7000:
// 		Addr=*(ULONG*)((BYTE*)Addr+25);
// 		IopFsNotifyChangeQueueHead=Addr;
		for (i=(ULONG)Addr;i < i+SizeOfProc(Addr);i++)
		{
			if (*i == 0xbe)
			{
				IopFsNotifyChangeQueueHead = *(PULONG)(i+1);
				if (MmIsAddressValidEx(IopFsNotifyChangeQueueHead))
				{
					return;
				}
			}
		}
		break;
	case WINDOWS_VERSION_7_7600_UP:
		for (i=(ULONG)Addr;i < i+SizeOfProc(Addr);i++)
		{
			if (*i == 0xe8)
			{
				ulIoRegisterFsRegistrationChangeMountAware = *(PULONG)(i+1)+(ULONG)(i+5);
				if (MmIsAddressValidEx(ulIoRegisterFsRegistrationChangeMountAware))
				{
					if (DebugOn)
						KdPrint(("ulIoRegisterFsRegistrationChangeMountAware:%08x\r\n",ulIoRegisterFsRegistrationChangeMountAware));

					for (x=(ULONG)ulIoRegisterFsRegistrationChangeMountAware;x < x+SizeOfProc(ulIoRegisterFsRegistrationChangeMountAware);x++)
					{
						if (*x == 0xbf)
						{
							if (DebugOn)
								KdPrint(("x:%08x\r\n",x));

							IopFsNotifyChangeQueueHead = *(PULONG)(x+1);
							if (MmIsAddressValidEx(IopFsNotifyChangeQueueHead))
							{
								return;
							}
							break;
						}
					}
				}
				break;
			}
		}
		break;
	}
}
//入口函数
ULONG QuerySystemNotify(PDRIVER_OBJECT DriverObj,PSYSTEM_NOTIFY SystemNotify)
{
	int i=0;

    //-----------------------------------------------------------------------//文件系统回调
	GetFileSystemNotifyCallBack(SystemNotify);
	//-----------------------------------------------------------------------//进程相关三组回调
 	GetCreateProcessNotify(SystemNotify);
 	GetCreateThreadNotify(SystemNotify);
 	GetLoadImageNotify(SystemNotify);
// 	//-----------------------------------------------------------------------//BugCheck
 	GetBugCheckCallBack(SystemNotify);
 	GetBugCheckReasonCallBack(SystemNotify);
// 	//-----------------------------------------------------------------------//关机回调
 	GetShutDownCallBack(SystemNotify);
// 	//-----------------------------------------------------------------------//注销回调
 	GetLeaveSessionCallback(SystemNotify);
	//-----------------------------------------------------------------------//
	return TRUE;
}
VOID RemoveNotifyRoutine(ULONG ImageNotify,int Type)
{
	//PsRemoveLoadImageNotifyRoutine(ImageNotify);
	switch (Type)
	{
	case 1:
		//CreateProcess
		//PsSetCreateProcessNotifyRoutine(ImageNotify,TRUE);
		ReLoadNtosCALL(&RPsSetCreateProcessNotifyRoutine,L"PsSetCreateProcessNotifyRoutine",SystemKernelModuleBase,ImageModuleBase);
		if (RPsSetCreateProcessNotifyRoutine)
		{
			RPsSetCreateProcessNotifyRoutine(ImageNotify,TRUE);
		}
		break;
	case 2:
		//CreateThread
		//PsRemoveCreateThreadNotifyRoutine(ImageNotify);
		ReLoadNtosCALL(&RPsRemoveCreateThreadNotifyRoutine,L"PsRemoveCreateThreadNotifyRoutine",SystemKernelModuleBase,ImageModuleBase);
		if (RPsRemoveCreateThreadNotifyRoutine)
		{
			RPsRemoveCreateThreadNotifyRoutine(ImageNotify);
		}
		break;
	case 3:
		//LoadImage
		//PsRemoveLoadImageNotifyRoutine(ImageNotify);
		ReLoadNtosCALL(&RPsRemoveLoadImageNotifyRoutine,L"PsRemoveLoadImageNotifyRoutine",SystemKernelModuleBase,ImageModuleBase);
		if (RPsRemoveLoadImageNotifyRoutine)
		{
			RPsRemoveLoadImageNotifyRoutine(ImageNotify);
		}
		break;
	case 4:
		//BugCheckCallBack
		//KeDeregisterBugCheckCallback(ImageNotify);
		ReLoadNtosCALL(&RKeDeregisterBugCheckCallback,L"KeDeregisterBugCheckCallback",SystemKernelModuleBase,ImageModuleBase);
		if (RKeDeregisterBugCheckCallback)
		{
			RKeDeregisterBugCheckCallback(ImageNotify);
		}
		break;
	case 5:
		//BugCheckReasonCallBack
		//KeDeregisterBugCheckReasonCallback(ImageNotify);
		ReLoadNtosCALL(&RKeDeregisterBugCheckReasonCallback,L"KeDeregisterBugCheckReasonCallback",SystemKernelModuleBase,ImageModuleBase);
		if (RKeDeregisterBugCheckReasonCallback)
		{
			RKeDeregisterBugCheckReasonCallback(ImageNotify);
		}
		break;
	case 6:
		//ShutDown
		//IoUnregisterShutdownNotification(ImageNotify);
		ReLoadNtosCALL(&RIoUnregisterShutdownNotification,L"IoUnregisterShutdownNotification",SystemKernelModuleBase,ImageModuleBase);
		if (RIoUnregisterShutdownNotification)
		{
			RIoUnregisterShutdownNotification(ImageNotify);
		}
		break;
	case 7:
		//SeFileSystem
		//SeUnregisterLogonSessionTerminatedRoutine(ImageNotify);
		ReLoadNtosCALL(&RSeUnregisterLogonSessionTerminatedRoutine,L"SeUnregisterLogonSessionTerminatedRoutine",SystemKernelModuleBase,ImageModuleBase);
		if (RSeUnregisterLogonSessionTerminatedRoutine)
		{
			RSeUnregisterLogonSessionTerminatedRoutine(ImageNotify);
		}
		break;
	case 8:
		//FsNotifyChange
		//IoUnregisterFsRegistrationChange 函数好像不行，暂时不支持吧
		break;
	}
}