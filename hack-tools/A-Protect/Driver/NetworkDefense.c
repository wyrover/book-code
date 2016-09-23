#include "NetworkDefense.h"

//从文件路径获取文件名
BOOL GetFileName(__in WCHAR *FilePath,__in int len,__out WCHAR *FileName)
{
	int i=0;
	WCHAR lpPath[260*2];
	BOOL bRetOK = FALSE;

	//wcslen(L"x:\\")*2
	//效验合法的长度，不然产生溢出漏洞，要蓝屏哦
	if (len < 6 || len > 260)
		return bRetOK;

	memset(lpPath,0,sizeof(lpPath));
	memset(FileName,0,sizeof(FileName));

	memcpy(lpPath,FilePath,len);
	for(i=0;i<len;i++)
	{
		if (wcsstr(lpPath,L"\\") == 0)
		{
			bRetOK = TRUE;

			//FileName的最大长度能容纳lpPath的实际长度，不然蓝屏！
			if (sizeof(FileName) > wcslen(lpPath))
			{
				memcpy(FileName,lpPath,wcslen(lpPath)*2);
			}
			break;
		}
		memset(lpPath,0,sizeof(lpPath));
		memcpy(lpPath,FilePath+i,wcslen(FilePath+i)*2);
	}
	return bRetOK;
}

ULONG CheckExeFileOrDllFileBySectionHandle(HANDLE hSection)
{
	NTSTATUS status;
	PVOID BaseAddress = NULL;
	SIZE_T size=0;
	PIMAGE_NT_HEADERS PImageNtHeaders;

	if (!hSection)
		return NULL;

	ReLoadNtosCALL(&RZwMapViewOfSection,L"ZwMapViewOfSection",SystemKernelModuleBase,ImageModuleBase);
	if (!RZwMapViewOfSection)
		return NULL;

	status = RZwMapViewOfSection(
		hSection, 
		NtCurrentProcess(),
		&BaseAddress, 
		0,
		1000, 
		0,
		&size,
		(SECTION_INHERIT)1,
		MEM_TOP_DOWN, 
		PAGE_READWRITE
		); 
	if(NT_SUCCESS(status))
	{
		if (DebugOn)
			KdPrint(("ZwMapViewOfSection success"));

		PImageNtHeaders = RtlImageNtHeader(BaseAddress);
		if (PImageNtHeaders)
		{
			if (DebugOn)
				KdPrint(("Characteristics:%08x\r\n",PImageNtHeaders->FileHeader.Characteristics));

			return PImageNtHeaders->FileHeader.Characteristics;
		}
	}
	return NULL;
}
////////////////////////////////////////////////
/*
ZwCreateSection hook, DKOM type.
*/
NTSTATUS _stdcall NewZwCreateSection(
	__out     PHANDLE SectionHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in_opt  POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PLARGE_INTEGER MaximumSize,
	__in      ULONG SectionPageProtection,
	__in      ULONG AllocationAttributes,
	__in_opt  HANDLE FileHandle)
{
	NTSTATUS status;
	PEPROCESS Eprocess;
	PFILE_OBJECT FileObject;
	PVOID object_temp;
	POBJECT_HEADER ObjectHeader;
	POBJECT_TYPE FileObjectType;
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;
	BOOL bInherited = FALSE;
	int i;
	WCHAR *lpwzExeFile = NULL;
	WCHAR *lpwzExeNtFile = NULL;
	KPROCESSOR_MODE PreviousMode;
	UNICODE_STRING UnicodeDNSAPI_DLL;
	UNICODE_STRING UnicodeExeNtFilePath;
	UNICODE_STRING UnicodeFunction;
	char *lpszProName = NULL;
	BOOL bNetworkDefence = FALSE;
	BOOL bInitAPISuccess = FALSE;
	POBJECT_NAME_INFORMATION DosFullPath=NULL;
	ULONG ulExeFileCharacteristics,ulDllFileCharacteristics;
	ULONG ulIsExeDllModule;
	STRING lpszProString;
	STRING lpszSvchostString;
	STRING lpszWinlogonString;
	STRING lpszServicesString;
	STRING lpszCmdString;
	STRING lpszExplorer;
	WCHAR lpwzDirFile[260];
	WCHAR FileName[260*2];
	WCHAR SystemFile[260];
	BOOL bIsInjectDllInto3600 = FALSE;
	ULONG ulPathSize;
	ZWCREATESECTION OldZwCreateSection;

	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RPsGetCurrentProcessId,L"PsGetCurrentProcessId",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RPsGetProcessImageFileName,L"PsGetProcessImageFileName",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoQueryFileDosDeviceName,L"IoQueryFileDosDeviceName",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RRtlCompareString,L"RtlCompareString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RRtlInitString,L"RtlInitString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RRtlCompareUnicodeString,L"RtlCompareUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	if (RPsGetCurrentProcess &&
		RObReferenceObjectByHandle &&
		RPsGetCurrentProcessId &&
		RPsGetProcessImageFileName &&
		RZwClose &&
		RExAllocatePool &&
		RExFreePool &&
		RIoQueryFileDosDeviceName &&
		RRtlCompareString &&
		RRtlInitString &&
		RRtlCompareUnicodeString)
	{
		bInitAPISuccess = TRUE;
	}
	if (!bInitAPISuccess){
		return STATUS_UNSUCCESSFUL;
	}
	if (bDisCreateProcess == FALSE)  //禁止创建进程
	{
		//过滤自己啊
		if (IsExitProcess(ProtectEProcess)){
			if (RPsGetCurrentProcess() != ProtectEProcess){
				return STATUS_UNSUCCESSFUL;
			}
		}
	}
	//蛋碎，深度扫描的时候，重启OriginalServiceDescriptorTable->ServiceTable[ZwCreateSectionIndex]调用蓝屏，
	//用原始系统的却没事，尼玛啊蛋碎啊~！
	//OldZwCreateSection = OriginalServiceDescriptorTable->ServiceTable[ZwCreateSectionIndex];
	OldZwCreateSection = KeServiceDescriptorTable->ServiceTable[ZwCreateSectionIndex];
	status = OldZwCreateSection(
		SectionHandle,
		DesiredAccess,
		ObjectAttributes,
		MaximumSize,
		SectionPageProtection,
		AllocationAttributes,
		FileHandle
		);
	if (!NT_SUCCESS(status)){
		return status;
	}
	//初始化OK
	if (!bIsInitSuccess){
		return status;
	}
	if (KeGetCurrentIrql() != PASSIVE_LEVEL){
		return status;
	}
	if ((AllocationAttributes == 0x1000000) && (SectionPageProtection & PAGE_EXECUTE))
	{
		if (!ARGUMENT_PRESENT(FileHandle)){
			return status;
		}
		PreviousMode = KeGetPreviousMode();
		if (PreviousMode != KernelMode)
		{
			__try{
				ProbeForRead(FileHandle,sizeof(HANDLE),sizeof(ULONG));
			}__except (EXCEPTION_EXECUTE_HANDLER) {
				goto _FunctionRet;
			}
		}
		status = RObReferenceObjectByHandle(
			FileHandle,
			NULL,
			*IoFileObjectType,
			KernelMode,
			(PVOID *)&object_temp,
			NULL);
		if (!NT_SUCCESS(status))
		{
			//恢复这个返回值
			status = STATUS_SUCCESS;

			goto _FunctionRet;
		}
		ObDereferenceObject(object_temp);  //清除引用对象

		Eprocess = RPsGetCurrentProcess();
		//保险起见，还是在object_temp里判断type才更准确~！
		WinVer = GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:
		case WINDOWS_VERSION_2K3_SP1_SP2:
			ObjectHeader = OBJECT_TO_OBJECT_HEADER(object_temp);
			FileObjectType = ObjectHeader->Type;
			break;
		case WINDOWS_VERSION_7_7600_UP:
		case WINDOWS_VERSION_7_7000:
			RtlInitUnicodeString(&UnicodeFunction,L"ObGetObjectType");
			MyGetObjectType=(OBGETOBJECTTYPE)MmGetSystemRoutineAddress(&UnicodeFunction);  //xp~2008下无此函数，如果直接调用，则导致驱动加载失败，因此需要动态获取地址
			//MyGetObjectType = GetSystemRoutineAddress(1,L"ObGetObjectType");
			if(MyGetObjectType)
			{
				FileObjectType = MyGetObjectType((PVOID)object_temp);
			}
			break;
		}
		if (FileObjectType != *IoFileObjectType)
		{
			goto _FunctionRet;
		}
		FileObject = (PFILE_OBJECT)object_temp;
		//KdPrint(("FileObject --> %ws\n",FileObject->FileName.Buffer));

		if (MmIsAddressValidEx(FileObject) &&
			ValidateUnicodeString(&FileObject->FileName))
		{
			ulIsExeDllModule = 0;
			//xp/2003
			//0x10f = ExeFile
			//0x210e = DllFile

			//win7 下只能得到加载的dll，得不到exe，就不要win7了
			WinVer = GetWindowsVersion();
			switch (WinVer)
			{
			case WINDOWS_VERSION_XP:
			case WINDOWS_VERSION_2K3_SP1_SP2:
				ulExeFileCharacteristics = 0x10f;
				ulDllFileCharacteristics = 0x2102;
				ulIsExeDllModule = CheckExeFileOrDllFileBySectionHandle(*SectionHandle);
				break;
			}
			RRtlInitUnicodeString(&UnicodeDNSAPI_DLL,L"\\windows\\system32\\DNSapi.DLL");
			if (RRtlCompareUnicodeString(&FileObject->FileName,&UnicodeDNSAPI_DLL,TRUE) == 0)
			{
				if (LogDefenseInfo->ulCount < 1000)   //记录超过1000条，则不记录。
				{
					LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
					LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
					LogDefenseInfo->LogDefense[ulLogCount].Type = 2;
					ulLogCount++;
				}
			}
			//------------------------------------------------
			//DLL挟持的防护
			//KdPrint(("DLL --> [%x] %ws\n",ulIsExeDllModule,FileObject->FileName.Buffer));

			if (bDisDllFuck &&   //允许DLL挟持，由用户控制
				ulIsExeDllModule == ulDllFileCharacteristics &&
				IsExitProcess(ProtectEProcess) &&  //A盾初始化好之后，才开始，不然深度扫描系统重启之后，无法启动A盾
				Eprocess != ProtectEProcess &&  //排除A盾自己哦
				ulIsExeDllModule)        //排除win7
			{

				memset(lpwzDirFile,0,sizeof(lpwzDirFile));
				//拷贝相等长度目录的字符
				if (wcslen(L"\\windows\\system")*2 > FileObject->FileName.Length)
					ulPathSize = FileObject->FileName.Length;
				else
					ulPathSize = wcslen(L"\\windows\\system")*2;

				memcpy(lpwzDirFile,FileObject->FileName.Buffer,ulPathSize);
				if (_wcsnicmp(lpwzDirFile,L"\\windows\\system",wcslen(L"\\windows\\system")) != 0 &&
					_wcsnicmp(lpwzDirFile,L"\\windows\\WinSxS",wcslen(L"\\windows\\WinSxS")) != 0)  //WinSxS里面好像有控件dll，要放过
				{
					//如果当前目录不是system32，则检查system32目录下是否有跟当前路径一样相同名字的文件，有则代表dll挟持！
					if (GetFileName(FileObject->FileName.Buffer,FileObject->FileName.Length,FileName))
					{
						memset(SystemFile,0,sizeof(SystemFile));
						wcscat(SystemFile,L"\\SystemRoot\\system32\\");
						wcscat(SystemFile,FileName);   //

						if (IsFileInSystem(SystemFile))
						{
							if (DebugOn)
								KdPrint(("%ws  <-->  %ws\n",lpwzDirFile,SystemFile));

							//DLL挟持！
							LogDefenseInfo->LogDefense[ulLogCount].Type = 4; //DLL挟持

							memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
							SafeCopyMemory(FileObject->FileName.Buffer,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,FileObject->FileName.Length);
							LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
							LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
							ulLogCount++;

							//dll挟持，直接杀猪~
							RZwClose(*SectionHandle);
							return STATUS_UNSUCCESSFUL;
						}
					}
				}
			}
			//------------------------------------------------
			//记录所有这些进程作为父进程创建子进程的行为
			lpszProName = (char *)RPsGetProcessImageFileName(Eprocess);
			RRtlInitString(&lpszProString,lpszProName);

			RRtlInitString(&lpszSvchostString,"svchost.exe");
			RRtlInitString(&lpszWinlogonString,"winlogon.exe");
			RRtlInitString(&lpszServicesString,"services.exe");
			RRtlInitString(&lpszCmdString,"cmd.exe");
			RRtlInitString(&lpszExplorer,"explorer.exe");

			if (RRtlCompareString(&lpszSvchostString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszWinlogonString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszServicesString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszCmdString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszExplorer,&lpszProString,TRUE) == 0)
			{
				if (LogDefenseInfo->ulCount < 1000 &&
					ulLogCount < 1000)   //记录超过1000条，则不记录。
				{
					if (FileObject->FileName.Buffer != NULL &&
						FileObject->FileName.Length >30 &&
						RIoQueryFileDosDeviceName(FileObject,&DosFullPath) == STATUS_SUCCESS)
					{
						ulPathSize = DosFullPath->Name.Length;

						lpwzExeFile = RExAllocatePool(NonPagedPool,ulPathSize);
						if (!lpwzExeFile)
						{
							if (DosFullPath)
								RExFreePool(DosFullPath);
							goto _FunctionRet;
						}
						memset(lpwzExeFile,0,ulPathSize);
						SafeCopyMemory(DosFullPath->Name.Buffer,lpwzExeFile,ulPathSize);

						if (DosFullPath)
							RExFreePool(DosFullPath);

						//KdPrint(("EXE --> [%x]%s %ws\n",ulIsExeDllModule,lpszProName,DosFullPath->Name.Buffer));

						//排除dll，只要exe路径的字符串
						if (ulIsExeDllModule == ulExeFileCharacteristics)        //排除win7
						{
							__try
							{
								LogDefenseInfo->LogDefense[ulLogCount].Type = 3;

								memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
								SafeCopyMemory(lpwzExeFile,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,ulPathSize);

								if (DebugOn)
									KdPrint(("ExePath:%ws\r\n",LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));

								LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
								LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
								ulLogCount++;

							}__except (EXCEPTION_EXECUTE_HANDLER) {

							}

						}
						//拦截所有svchost的dll加载
						if (RRtlCompareString(&lpszSvchostString,&lpszProString,TRUE) == 0)
						{
							__try
							{
								memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
								SafeCopyMemory(lpwzExeFile,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,ulPathSize);

								if (DebugOn)
									KdPrint(("DLLPath:%ws\r\n",LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));

								LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
								LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
								LogDefenseInfo->LogDefense[ulLogCount].Type = 3;
								ulLogCount++;

							}__except (EXCEPTION_EXECUTE_HANDLER) {

							}
						}
						if (lpwzExeFile)
							RExFreePool(lpwzExeFile);
					}
				}
			}
		}
	}
_FunctionRet:
	return status;
}
BOOL InitNetworkDefence()
{
	if (SystemCallEntryTableHook(
		"ZwCreateSection",
		&ZwCreateSectionIndex,
		NewZwCreateSection) == TRUE)
	{
		if (DebugOn)
			KdPrint(("Create Control Thread success 3\r\n"));
	}
}