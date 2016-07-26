//////////////////////////////////////////////////////////
// FilterTest.CPPÎÄ¼þ


#include <windows.h>
#include <stdio.h>
#include <ntddndis.h>
#include "IOCOMMON.h"
#include "ptutils.h"

void ShowStats(PPassthruStatistics pStats)
{
	printf(" Total Packets Sent        : %d\n", pStats->nMPSendPktsCt);
	printf("    Send Packets Blocked   : %d\n", pStats->nMPSendPktsDropped);
	printf(" Total Packets Received    : %d\n", pStats->nPTRcvPktCt + pStats->nPTRcvCt);
	printf("    Receive Packets Blocked: %d\n", pStats->nPTRcvDropped + pStats->nPTRcvPktDropped);
}

int main()
{
	HANDLE hControlDevice = PtOpenControlDevice();
	CIMAdapters adapters;
	if(!adapters.EnumAdapters(hControlDevice))
	{
		printf(" EnumAdapters failed \n");
		return -1;
	}

	printf(" Driver Bindings: \n");
	for(int i=0; i<adapters.m_nAdapters; i++)
	{
		// ÏÔÊ¾ÐéÄâÊÊÅäÆ÷Ãû³Æ
		printf("    \"%ws\"\n", adapters.m_pwszVirtualName[i]);
		// ÏÔÊ¾ÏÂ²ãÊÊÅäÆ÷Ãû³Æ
		printf("       \"%ws\"\n", adapters.m_pwszAdapterName[i]);

		// ²éÑ¯´ËÊÊÅäÆ÷µÄÍøÂç»î¶¯×´Ì¬
		HANDLE hLowerAdapter = PtOpenAdapter(adapters.m_pwszAdapterName[i]);
		if(hLowerAdapter != INVALID_HANDLE_VALUE)
		{
			PassthruStatistics stats;
			if(PtQueryStatistics(hLowerAdapter, &stats))
			{
				ShowStats(&stats);
			}
			::CloseHandle(hLowerAdapter);
		}	
		
		printf(" \n");
	}
	::CloseHandle(hControlDevice);
	return 0;
}



