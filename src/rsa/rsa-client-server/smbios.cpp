#include "SMBios.h"
#include <stdlib.h>
#include <stdio.h>

errno_t SMBIOSParse(ULONG64 *IDEN_UUID) {
	//Parse some part of SMBIOS, from
	//https://github.com/KunYi/DumpSMBIOS/blob/master/DumpSMBIOS/DumpSMBIOS.cpp

	//Get SMBIOS
	const BYTE byteSignature[] = { 'B', 'M', 'S', 'R' };
	const DWORD Signature = *((DWORD*)byteSignature);
	DWORD needBufferSize = GetSystemFirmwareTable(Signature, 0, NULL, 0);
	LPBYTE pBuff = (LPBYTE)malloc(needBufferSize);
	if (pBuff)
	{
		GetSystemFirmwareTable(Signature, 0, pBuff, needBufferSize);

		FILE* fp = fopen("dump.bin", "wb");

		if (fp) {
			size_t ret = fwrite(pBuff, 1, needBufferSize, fp);			
			fclose(fp);			
		}
	}
	else {
		return E_OUTOFMEMORY;
	}
	//Parse
	const PRawSMBIOSData pDMIData = (PRawSMBIOSData)pBuff;
	LPBYTE p = (LPBYTE)(&(pDMIData->SMBIOSTableData));
	const DWORD lastAddress = ((DWORD)p) + pDMIData->Length;
	PSMBIOSHEADER pHeader;
	for (;;) {
		pHeader = (PSMBIOSHEADER)p;
		switch (pHeader->Type)
		{
		case 0://BIOS Info
		{
			PBIOSInfo pBiosInfo = (PBIOSInfo)(pHeader);			
			//IDEN_UUID[3] = pBiosInfo->Version;
			break;
		}
		case 1://ProcSysInfo
		{
			PSystemInfo pSysInfo = (PSystemInfo)(pHeader);
			memcpy(IDEN_UUID, pSysInfo->UUID, sizeof(pSysInfo->UUID));
			break;
		}
		case 4://CPUInfo
		{
			PProcessorInfo pProcInfo = (PProcessorInfo)(pHeader);
			IDEN_UUID[2] = pProcInfo->ID;
			break;
		}
		default:
			break;
		}
		PBYTE nt = p + pHeader->Length; // point to struct end
		while (0 != (*nt | *(nt + 1))) nt++; // skip string area
		nt += 2;
		if ((DWORD)nt > lastAddress)
			break;
		p = nt;
	}

	free(pBuff);

	return NOERROR;
}
