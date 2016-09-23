#include "win32k.h"

//由base得到shadowssdt
// .data段第一个就是
BOOL MmQueryShadowSSDTAddr(ULONG ImageBase,DWORD *ShadowSSDTAddress)
{
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS NtHeader;
	PIMAGE_SECTION_HEADER Section;
	int i=0;
	BOOL bRetOK = FALSE;

	//KdPrint(("ImageBase %08x\n",ImageBase));

	DosHeader=(PIMAGE_DOS_HEADER)ImageBase;
	if (DosHeader->e_magic!=IMAGE_DOS_SIGNATURE){
		return bRetOK;
	}
	NtHeader=(PIMAGE_NT_HEADERS)((PBYTE)ImageBase+DosHeader->e_lfanew);
	if (NtHeader->Signature!=IMAGE_NT_SIGNATURE){
		return bRetOK;
	}
	Section = (PIMAGE_SECTION_HEADER) ((ULONG)NtHeader + sizeof(IMAGE_NT_HEADERS));
	for (i = 0; i < NtHeader->FileHeader.NumberOfSections; i++)
	{
		if (DebugOn)
			KdPrint(("Section %s\n",Section->Name));

		if (_strnicmp(Section->Name,".data",strlen(".data")) == 0){

			*ShadowSSDTAddress = ImageBase+Section->VirtualAddress;

			if (DebugOn)
				KdPrint(("Section %08x\n",*ShadowSSDTAddress));

			if (MmIsAddressValidEx(*ShadowSSDTAddress)){
				bRetOK = TRUE;
				break;
			}
		}
		Section++;
	}
	return bRetOK;
}
BOOL FixOriginalW32pTable(PDWORD w32pTable,PVOID ImageBase,DWORD Win32kBase)
{
	PIMAGE_NT_HEADERS NtHeaders;
	DWORD FuctionCount;
	DWORD Index;
	DWORD ImageSize;

	NtHeaders=RtlImageNtHeader(ImageBase);
	if (NtHeaders)
	{
		ImageSize=NtHeaders->OptionalHeader.SizeOfImage;
		ImageSize=AlignSize(ImageSize,NtHeaders->OptionalHeader.SectionAlignment);
	}
	FuctionCount = ShadowSSDTTable[1].TableSize;

	if (DebugOn)
		KdPrint(("FuctionCount:%d"));

	for (Index=0;Index<FuctionCount;Index++)
	{
		w32pTable[Index]=w32pTable[Index]-Win32kBase+(DWORD)ImageBase;
		if (!MmIsAddressValidEx(w32pTable[Index])){
			return FALSE;
		}
	}
	return TRUE;
}
BOOL InitReloadWin32K(PDRIVER_OBJECT pDriverObject,PSERVICE_DESCRIPTOR_TABLE ShadowSSDT,ULONG Win32kBase)
{
	UNICODE_STRING FileName;
	HANDLE hSection;
	PDWORD OriginalKiServiceTable;
	PDWORD CsRootkitOriginalKiServiceTable;
	int i=0;

	if (!PeLoad(L"\\SystemRoot\\System32\\win32k.sys",&Win32kImageModuleBase,pDriverObject,Win32kBase)){
		if (DebugOn)
			KdPrint(("Safe->PeLoad failed\n"));
		return FALSE;
	}

	if (DebugOn)
		KdPrint(("Safe->ModuleBase:%08x\r\n",Win32kImageModuleBase));

	OriginalKiServiceTable = ExAllocatePool(NonPagedPool,ShadowSSDT[1].TableSize*sizeof(DWORD));
	if (!OriginalKiServiceTable){
		if (DebugOn)
			KdPrint(("OriginalKiServiceTable Failed\n"));
		return FALSE;
	}
	//获取原始OriginalKiServiceTable
	if(!MmQueryShadowSSDTAddr(Win32kImageModuleBase,&OriginalKiServiceTable)){
		if (DebugOn)
			KdPrint(("Safe->Get Original KiServiceTable Failed\n"));

		ExFreePool(OriginalKiServiceTable);
		return FALSE;
	}
	//重定位reload原始SSDT表里面的每一个原始函数
	if (!FixOriginalW32pTable(OriginalKiServiceTable,(DWORD)Win32kImageModuleBase,Win32kBase)){
		if (DebugOn)
			KdPrint(("FixOriginalW32pTable Failed\n"));

		ExFreePool(OriginalKiServiceTable);
		return FALSE;
	}
	if (DebugOn)
		KdPrint(("Safe->OriginalKiServiceTable %x-%x\n",OriginalKiServiceTable,ShadowSSDT[1].ServiceTable));

	OriginalShadowServiceDescriptorTable=ExAllocatePool(NonPagedPool,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	if (!OriginalShadowServiceDescriptorTable){
		ExFreePool(OriginalKiServiceTable);
		return FALSE;
	}
	RtlZeroMemory(OriginalShadowServiceDescriptorTable,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	//这是一个干净的原始表，每个表里所对应的SSDT函数的地址都是有效的~
	OriginalShadowServiceDescriptorTable->ServiceTable=(PDWORD)OriginalKiServiceTable;
	OriginalShadowServiceDescriptorTable->CounterTable=ShadowSSDT[1].CounterTable;
	OriginalShadowServiceDescriptorTable->TableSize=ShadowSSDT[1].TableSize;
	OriginalShadowServiceDescriptorTable->ArgumentTable=ShadowSSDT[1].ArgumentTable;

	CsRootkitOriginalKiServiceTable=ExAllocatePool(NonPagedPool,ShadowSSDT[1].TableSize*sizeof(DWORD));
	if (!CsRootkitOriginalKiServiceTable){
		ExFreePool(OriginalShadowServiceDescriptorTable);
		ExFreePool(OriginalKiServiceTable);
		return FALSE;

	}
	RtlZeroMemory(CsRootkitOriginalKiServiceTable,ShadowSSDT[1].TableSize*sizeof(DWORD));

	Safe_ServiceDescriptorShadowSSDTTable=ExAllocatePool(NonPagedPool,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	if (!Safe_ServiceDescriptorShadowSSDTTable){
		ExFreePool(OriginalShadowServiceDescriptorTable);
		ExFreePool(CsRootkitOriginalKiServiceTable);
		ExFreePool(OriginalKiServiceTable);
		return FALSE;
	}
	//这是一个干净的原始表，每个表里所对应的SSDT函数的地址都是原始函数
	RtlZeroMemory(Safe_ServiceDescriptorShadowSSDTTable,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	//填充原始函数地址
	for (i=0;i<ShadowSSDT[1].TableSize;i++)
	{
		CsRootkitOriginalKiServiceTable[i] = OriginalShadowServiceDescriptorTable->ServiceTable[i];
	}
	Safe_ServiceDescriptorShadowSSDTTable->ServiceTable = (PDWORD)CsRootkitOriginalKiServiceTable;
	Safe_ServiceDescriptorShadowSSDTTable->CounterTable=ShadowSSDT[1].CounterTable;
	Safe_ServiceDescriptorShadowSSDTTable->TableSize=ShadowSSDT[1].TableSize;
	Safe_ServiceDescriptorShadowSSDTTable->ArgumentTable=ShadowSSDT[1].ArgumentTable;
	return TRUE;
}
NTSTATUS ReloadWin32K()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG ulKeAddSystemServiceTable;
	UNICODE_STRING UnicodeFunctionName;
	PULONG p;
	int i = 0;

	if (IsExitProcess(AttachGuiEProcess))
	{
		KeAttachProcess(AttachGuiEProcess);

		RtlInitUnicodeString(&UnicodeFunctionName,L"KeAddSystemServiceTable");
		ulKeAddSystemServiceTable = (DWORD)MmGetSystemRoutineAddress(&UnicodeFunctionName);
		if (MmIsAddressValidEx(ulKeAddSystemServiceTable))
		{
			p = (PULONG)((ULONG)ulKeAddSystemServiceTable + 0x1a + 0x2);
			if (MmIsAddressValidEx(p))
			{
				ShadowSSDTTable = (PSERVICE_DESCRIPTOR_TABLE)(PULONG)(*p);

				if (DebugOn)
					KdPrint(("ShadowSSDTTable:%x\n",ShadowSSDTTable[1]));
			}
		}
		ulWin32kBase = LookupKernelModuleByName(PDriverObject,"win32k.sys",&ulWin32kSize);
		if (MmIsAddressValidEx(ShadowSSDTTable) &&
			ulWin32kBase)
		{
			if (DebugOn)
				KdPrint(("LookupKernelModuleByName success\n"));

			if (InitReloadWin32K(PDriverObject,ShadowSSDTTable,ulWin32kBase))
			{
				if (DebugOn)
					KdPrint(("InitReloadWin32K success"));

				ShadowTable = ShadowSSDTTable[1].ServiceTable;

				InitShadowSSDTHook();  //SHADOW SSDT hook

				status = STATUS_SUCCESS;
			}
		}
		KeDetachProcess();
	}
	return status;
}
////////--------------------------