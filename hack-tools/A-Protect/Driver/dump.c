#include "dump.h"

//if (DumpMemory((PVOID)ulDumpKernelBase,KernelBuffer,ulDumpKernelSize) == STATUS_SUCCESS)

NTSTATUS DumpMemory(PUCHAR SrcAddr, PUCHAR DstAddr, ULONG Size)
{
	ULONG Remaining, Temp;

	__try
	{
		Remaining = Size;
		Temp = 0x1000 - (((ULONG)SrcAddr) & 0xfff);
		if (Remaining < Temp) Temp = Remaining;

		if (Temp != 0x1000) 
		{
			if (MmIsAddressValidEx(SrcAddr)) SafeCopyMemory(SrcAddr, DstAddr, Temp);
			SrcAddr += Temp;
			DstAddr += Temp;
			Remaining -= Temp;
		}

		while (Remaining > 0x1000) 
		{
			if (MmIsAddressValidEx(SrcAddr)) SafeCopyMemory(SrcAddr, DstAddr, 0x1000);
			SrcAddr += 0x1000;
			DstAddr += 0x1000;
			Remaining -= 0x1000;
		}

		if (Remaining != 0) 
			if (MmIsAddressValidEx(SrcAddr)) SafeCopyMemory(SrcAddr, DstAddr, Remaining);

	}__except(EXCEPTION_EXECUTE_HANDLER){
		
	}
	return STATUS_SUCCESS;
}

