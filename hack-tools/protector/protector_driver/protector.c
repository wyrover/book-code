
#include "ntddk.h"

struct SYS_SERVICE_TABLE { 
	void **ServiceTable; 
	unsigned long CounterTable; 
	unsigned long ServiceLimit; 
	void **ArgumentsTable; 
}; 



const WCHAR devicename[]=L"\\Device\\Protector";
const WCHAR devicelink[]=L"\\DosDevices\\PROTECTOR";


KEVENT event; 



ULONG Index,RealCallee;
char*output;
extern struct SYS_SERVICE_TABLE *KeServiceDescriptorTable; 


//this function decides whether we should allow NtCreateSection() call to be successfull
ULONG __stdcall check(PULONG arg)
{

	HANDLE hand=0;PFILE_OBJECT file=0;POBJECT_HANDLE_INFORMATION info;ULONG a;char*buff;
	ANSI_STRING str; LARGE_INTEGER li;li.QuadPart=-10000;

//check the flags. If PAGE_EXECUTE access to the section is not requested,
//it does not make sense to be bothered about it	
if((arg[4]&0xf0)==0)return 1;
if((arg[5]&0x01000000)==0)return 1;


//get the file name via the file handle
hand=(HANDLE)arg[6];
ObReferenceObjectByHandle(hand,0,0,KernelMode,&file,&info);
if(!file)return 1;
RtlUnicodeStringToAnsiString(&str,&file->FileName,1);

a=str.Length;buff=str.Buffer;
while(1)
{
	if(buff[a]=='.'){a++;break;}
	a--;
}
ObDereferenceObject(file);

//if it is not executable, it does not make sense to be bothered about it
//return 1
if(_stricmp(&buff[a],"exe")){RtlFreeAnsiString(&str);return 1;}

//now we are going to ask user's opinion. Write file name to the buffer, and wait until
//the user indicates the response (1 as a first DWORD means we can proceed)

//synchronize access to the buffer
KeWaitForSingleObject(&event,Executive,KernelMode,0,0);


// set first 2 DWORD of a buffer to zero, copy the string into the buffer, and loop
//until the user sets first DWORD to 1. The value of the second DWORD indicates user's 
//response
strcpy(&output[8],buff);
RtlFreeAnsiString(&str);



a=1;
memmove(&output[0],&a,4);
while(1)
{
KeDelayExecutionThread(KernelMode,0,&li);
memmove(&a,&output[0],4);
if(!a)break;
}
memmove(&a,&output[4],4);
KeSetEvent(&event,0,0);

return a;
}


//just saves execution contect and calls check() 
_declspec(naked) Proxy()
{

_asm{

//save execution contect and calls check() -the rest depends upon the value check() returns
// if it is 1, proceed to the actual callee. Otherwise,return STATUS_ACCESS_DENIED
pushfd
pushad
mov ebx,esp
add ebx,40
push ebx
call check
cmp eax,1
jne block

//proceed to the actual callee
popad
popfd
jmp RealCallee

//return STATUS_ACCESS_DENIED
block:popad
mov ebx, dword ptr[esp+8]
mov dword ptr[ebx],0
mov eax,0xC0000022L
popfd
ret 32

}
}


NTSTATUS DrvDispatch(IN PDEVICE_OBJECT device,IN PIRP Irp)

{
	UCHAR*buff=0; ULONG a,base;



PIO_STACK_LOCATION loc=IoGetCurrentIrpStackLocation(Irp);

if(loc->Parameters.DeviceIoControl.IoControlCode==1000)
{
buff=(UCHAR*)Irp->AssociatedIrp.SystemBuffer;


// hook service dispatch table
memmove(&Index,buff,4);
a=4*Index+(ULONG)KeServiceDescriptorTable->ServiceTable;
base=(ULONG)MmMapIoSpace(MmGetPhysicalAddress((void*)a),4,0);
a=(ULONG)&Proxy;

_asm
{
mov eax,base
mov ebx,dword ptr[eax]
mov RealCallee,ebx
mov ebx,a
mov dword ptr[eax],ebx
}

MmUnmapIoSpace(base,4);

memmove(&a,&buff[4],4);
output=(char*)MmMapIoSpace(MmGetPhysicalAddress((void*)a),256,0);
}



Irp->IoStatus.Status=0;
IoCompleteRequest(Irp,IO_NO_INCREMENT);
return 0;


}



// nothing special
NTSTATUS DrvCreateClose(IN PDEVICE_OBJECT device,IN PIRP Irp)

{
	
Irp->IoStatus.Information=0;
Irp->IoStatus.Status=0;
IoCompleteRequest(Irp,IO_NO_INCREMENT);
return 0;

}



// nothing special -just a cleanup
void DrvUnload(IN PDRIVER_OBJECT driver)
{
UNICODE_STRING devlink;
ULONG a,base;

//unhook dispatch table
a=4*Index+(ULONG)KeServiceDescriptorTable->ServiceTable;
base=(ULONG)MmMapIoSpace(MmGetPhysicalAddress((void*)a),4,0);

_asm
{
mov eax,base
mov ebx,RealCallee
mov dword ptr[eax],ebx
}

MmUnmapIoSpace(base,4);
MmUnmapIoSpace(output,256);

RtlInitUnicodeString(&devlink,devicelink);
IoDeleteSymbolicLink(&devlink);
IoDeleteDevice(driver->DeviceObject);
}


//DriverEntry just creates our device - nothing special here
NTSTATUS DriverEntry(IN PDRIVER_OBJECT driver,IN PUNICODE_STRING path)
{

PDEVICE_OBJECT devobject=0;

UNICODE_STRING devlink,devname;

ULONG a,b;



RtlInitUnicodeString(&devname,devicename);
RtlInitUnicodeString(&devlink,devicelink);

IoCreateDevice(driver,256,&devname,FILE_DEVICE_UNKNOWN,0,TRUE,&devobject);
IoCreateSymbolicLink(&devlink,&devname);



driver->MajorFunction[IRP_MJ_DEVICE_CONTROL]=DrvDispatch;
driver->MajorFunction[IRP_MJ_CREATE]=DrvCreateClose;
driver->MajorFunction[IRP_MJ_CLOSE]=DrvCreateClose;
driver->DriverUnload=DrvUnload;
KeInitializeEvent(&event,SynchronizationEvent,1);


return 0;
}