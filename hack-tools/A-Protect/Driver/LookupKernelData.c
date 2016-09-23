#include "LookupKernelData.h"

VOID LookupKernelDataInfo(PVOID LookupBase,ULONG ulLookupSize,PLOOKUP_KERNEL_DATA LookupKernelData)
{
	ULONG i=0;
	ULONG ulSize;
	int x=0;

	if (!MmIsAddressValidEx(LookupBase)){
		return;
	}
	if (ulLookupSize < 0x10){
		return;
	}
	ulSize = (ULONG)LookupBase+ulLookupSize;
	if (ulSize > 0xffffffff ||
		ulSize < LookupBase)  //当LookupBase+ulLookupSize的总和超过0xffffffff就会造成溢出，溢出之后有可能变小
	{
		return;
	}
	if (DebugOn)
		KdPrint(("base:%08x size:%08x\n",LookupBase,ulSize));

	for (i=(ULONG)LookupBase;i<ulSize;i+=0x10)
	{
		if (MmIsAddressRangeValid(i,0xc))
		{
			if (MmIsAddressValidEx(LookupKernelData))
			{
				//KdPrint(("0x%08x %08x %08x %08x %08x\n",i,*(PULONG)i,*(PULONG)(i+4),*(PULONG)(i+8),*(PULONG)(i+0xc)));

				LookupKernelData->KernelData[x].ulAddress = i;
				LookupKernelData->KernelData[x].ulStack1 = *(PULONG)(i);
				LookupKernelData->KernelData[x].ulStack2 = *(PULONG)(i+4);
				LookupKernelData->KernelData[x].ulStack3 = *(PULONG)(i+8);
				LookupKernelData->KernelData[x].ulStack4 = *(PULONG)(i+0xc);
				x++;
				LookupKernelData->ulCount = x;
			}
		}
	}
}
