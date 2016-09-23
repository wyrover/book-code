#ifndef CXX_ANTISPY_H
#include "AntiSpy.h"
#endif

#define  DELAY_ONE_MICROSECOND  (-10)
#define  DELAY_ONE_MILLISECOND  (DELAY_ONE_MICROSECOND*1000)

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject,PUNICODE_STRING RegistryPath)
{
	NTSTATUS        Status = STATUS_UNSUCCESSFUL;
	ULONG           i = 0;
	UNICODE_STRING  uniLinkName;
	UNICODE_STRING  uniDeviceName;
	PDEVICE_OBJECT  DeviceObject;

	RtlInitUnicodeString(&uniDeviceName,DEVICE_NAME);
	RtlInitUnicodeString(&uniLinkName,LINK_NAME);


	Status = IoCreateDevice(DriverObject,0,&uniDeviceName,FILE_DEVICE_UNKNOWN,0,FALSE,&DeviceObject);
	if (!NT_SUCCESS(Status))
	{
		Status =  STATUS_UNSUCCESSFUL;
		return Status;
	}

	Status = IoCreateSymbolicLink(&uniLinkName,&uniDeviceName);

	if (!NT_SUCCESS(Status))
	{
		IoDeleteDevice(DeviceObject);

		return STATUS_UNSUCCESSFUL;
	}

	for (i=0;i<IRP_MJ_MAXIMUM_FUNCTION;i++)
	{
		DriverObject->MajorFunction[i] = DispatchFunction;
	}

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchContorRoutine;

	DriverObject->DriverUnload = DriverUnload;

	return Status;
}


VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{	
	UNICODE_STRING  uniLinkName;
	PDEVICE_OBJECT  CurrentDeviceObject;
	PDEVICE_OBJECT  NextDeviceObject;

	RtlInitUnicodeString(&uniLinkName,LINK_NAME);
	IoDeleteSymbolicLink(&uniLinkName);

	if (DriverObject->DeviceObject!=NULL)
	{
		CurrentDeviceObject = DriverObject->DeviceObject;

		while(CurrentDeviceObject!=NULL)
		{
			NextDeviceObject  = CurrentDeviceObject->NextDevice;
			IoDeleteDevice(CurrentDeviceObject);
			CurrentDeviceObject = NextDeviceObject;
		}

	}
}

NTSTATUS DispatchFunction(PDEVICE_OBJECT  DeviceObject,PIRP Irp)
{
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(Irp,IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


VOID Sleep(LONG MSec)
{
	LARGE_INTEGER Interval;
	Interval.QuadPart  = DELAY_ONE_MILLISECOND;
	Interval.QuadPart *= MSec;
	KeDelayExecutionThread(KernelMode,0,&Interval);
}

NTSTATUS  DispatchContorRoutine(PDEVICE_OBJECT  DeviceObject,PIRP Irp)
{
	NTSTATUS     Status = STATUS_SUCCESS;
	PVOID        InputBuffer  = NULL;
	PVOID        OutputBuffer = NULL;
	ULONG_PTR    InputSize  = 0;
	ULONG_PTR    OutputSize = 0;
	ULONG_PTR    IoControlCode = 0;
	PIO_STACK_LOCATION   IrpSp;

	IrpSp          = IoGetCurrentIrpStackLocation(Irp);
	InputBuffer    = IrpSp->Parameters.DeviceIoControl.Type3InputBuffer;
	OutputBuffer   = Irp->UserBuffer;
	InputSize      = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
	OutputSize     = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;

	IoControlCode  = IrpSp->Parameters.DeviceIoControl.IoControlCode;


	switch(IoControlCode)
	{
	case IOCTL_ENUMPROCESSINFOR:
		{
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID));

				Status = EnumProcess((PULONG)OutputBuffer,OutputSize); 

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}

			Irp->IoStatus.Information = 0;
			Status = Irp->IoStatus.Status = Status;

			break;
		}
	case IOCTL_ENUMHIDEPROCESSINFOR:
		{
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID));

				ForceEnumHideProcessData(*(PULONG)InputBuffer,(PULONG)OutputBuffer);

				Status = STATUS_SUCCESS;

				Irp->IoStatus.Status = Status;
				Irp->IoStatus.Information = 0;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}


			Irp->IoStatus.Information = 0;
			Status = Irp->IoStatus.Status = Status;

			break;
		}
	case CTL_KILLPROCESS:
		{
			KillProcessByProcessId(*(PULONG_PTR)InputBuffer);
			
			Irp->IoStatus.Status = Status;
			Irp->IoStatus.Information = 0;

			break;
		}
	case CTL_KILLPROCESS_AND_FILE:
		{
			PDATA Data = NULL;
			WCHAR wzNtFileFullPath[MAX_PATH] = {0};
			Data  = (PDATA)InputBuffer;

			if(KillProcessByProcessId(Data->ulProcessId)==TRUE)
			{
				Sleep(100);  

				if(DeleteFile(Data->wzImagePath)==FALSE)
				{
					WCHAR wzTemp[20] = {0};
					WCHAR* Points     =   NULL;
					Points = wcsrchr(Data->wzImagePath,'\\');
					wcscpy(wzTemp,Points+1);

					ForceDeleteProcessHandle(wzTemp);

					DeleteFile(Data->wzImagePath);
					
				}
				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
				break;
			}
		}
	case CTL_FORCE_KILL_PROCESS:
		{
			PEPROCESS EProcess;
			PsLookupProcessByProcessId((HANDLE)*(PULONG_PTR)InputBuffer,&EProcess);

			KillProcessForcedByProcessId_InsertAPC(EProcess);

			Irp->IoStatus.Status = Status;
			Irp->IoStatus.Information = 0;
			break;
		}
	case CTL_ENUMPROCESSHANDLE:
		{
			if (!MmIsAddressValid(OutputBuffer))
			{
				Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
				Irp->IoStatus.Information = 0;
				break;
			}
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = EnumProcessHandle((PVOID)*(PULONG)InputBuffer,OutputBuffer,OutputSize);      

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}
	case CTL_KILLPROCESSHANDLE:
		{
			if (!MmIsAddressValid(InputBuffer))
			{
				Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
				Irp->IoStatus.Information = 0;

				break;
			}

			Status = KillProcessHandle(InputBuffer);   

			Irp->IoStatus.Information = 0;
			Status = Irp->IoStatus.Status = Status;

			break;
		}
	case CTL_ENUMPROCESSMODULE:
		{
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID));

				EnumMoudleByNtQueryVirtualMemory(*(PULONG)InputBuffer,(PPROCESS_MODULE_INFORMATION)OutputBuffer,OutputSize);

				Status = STATUS_SUCCESS;

				Irp->IoStatus.Status = Status;
				Irp->IoStatus.Information = 0;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}

			Irp->IoStatus.Information = 0;
			Status = Irp->IoStatus.Status = Status;

			break;
		}
	case CTL_ENUMPROCESSTHREAD:
		{
			if (!MmIsAddressValid(OutputBuffer))
			{
				Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
				Irp->IoStatus.Information = 0;
				break;
			}
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = EnumProcessThread((PVOID)*(PULONG)InputBuffer,OutputBuffer,OutputSize);      

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}

	case CTL_KILLPROCESSTHREAD:
		{
			
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = KillProcessThread((PETHREAD)(*(ULONG_PTR*)InputBuffer));     

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}

	case CTL_FORCEKILLPROCESSTHREAD:
		{
			
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = ForceKillProcessThread((PETHREAD)(*(ULONG_PTR*)InputBuffer));      

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}

	case CTL_SUSPENDPROCESSTHREAD:
		{
			
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = PspSuspendOrResumeThread((PETHREAD)(*(ULONG_PTR*)InputBuffer),TRUE);     

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}

	case CTL_RESUMEPROCESSTHREAD:
		{
			
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = PspSuspendOrResumeThread((PETHREAD)(*(ULONG_PTR*)InputBuffer),FALSE);      

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}

	case CTL_JUDGETHREADSUSPEND:
		{
			if (!MmIsAddressValid(OutputBuffer))
			{
				Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
				Irp->IoStatus.Information = 0;
				break;
			}
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = JudgeThreadIsSuspend((PETHREAD)(*(ULONG_PTR*)InputBuffer),(ULONG_PTR*)OutputBuffer); 

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}
			break;
		}
	case CTL_ENUMPROCESSMEMORY:
		{
			if (!MmIsAddressValid(OutputBuffer))
			{
				Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
				Irp->IoStatus.Information = 0;
				break;
			}
			__try
			{
				ProbeForWrite(OutputBuffer,OutputSize,sizeof(PVOID)); 

				Status = EnumProcessMemory((ULONG)*(PULONG)InputBuffer,OutputBuffer,OutputSize);      

				Irp->IoStatus.Information = 0;    
				Status = Irp->IoStatus.Status = Status;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				Irp->IoStatus.Information = 0;
				Status = Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			}

			break;
		}
	case CTL_MODIFYROCESSMEMORYPROTECT:
		{
			Status = ModifyMemoryProtect(InputBuffer,InputSize);

			Irp->IoStatus.Information = 0;
			Status = Irp->IoStatus.Status = Status;
			break;
		}
	case  CTL_READROCESSMEMORY:
		{
			Status = ReadProcessMemory(InputBuffer,OutputBuffer);
			Irp->IoStatus.Information = 0;
			Status = Irp->IoStatus.Status = Status;
			break;
		}
	default:
		{
			Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
			Irp->IoStatus.Information = 0;

			break;
		}
	}

		IoCompleteRequest(Irp,IO_NO_INCREMENT);

		return Status;
}


