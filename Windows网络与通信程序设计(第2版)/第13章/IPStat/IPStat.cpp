//////////////////////////////////////////////////////////////////////
// IPStat.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")


int main()
{
	// 获取IP统计数据
	MIB_IPSTATS IpStats;
	if(::GetIpStatistics(&IpStats) == NO_ERROR)
	{
		MIB_IPSTATS *pStats = &IpStats;
        printf("\nIP Statistics:\n");
        
        printf("\
				  dwForwarding       = %lu\n\
				  dwDefaultTTL       = %lu\n\
				  dwInReceives       = %lu\n\
				  dwInHdrErrors      = %lu\n\
				  dwInAddrErrors     = %lu\n\
				  dwForwDatagrams    = %lu\n\
				  dwInUnknownProtos  = %lu\n\
				  dwInDiscards       = %lu\n\
				  dwInDelivers       = %lu\n\
				  dwOutRequests      = %lu\n\
				  dwRoutingDiscards  = %lu\n\
				  dwOutDiscards      = %lu\n\
				  dwOutNoRoutes      = %lu\n\
				  dwReasmTimeout     = %lu\n\
				  dwReasmReqds       = %lu\n\
				  dwReasmOks         = %lu\n\
				  dwReasmFails       = %lu\n\
				  dwFragOks          = %lu\n\
				  dwFragFails        = %lu\n\
				  dwFragCreates      = %lu\n\
				  dwNumIf            = %lu\n\
				  dwNumAddr          = %lu\n\
				  dwNumRoutes        = %lu\n",
                  pStats->dwForwarding,
                  pStats->dwDefaultTTL,
                  pStats->dwInReceives,
                  pStats->dwInHdrErrors,
                  pStats->dwInAddrErrors,
                  pStats->dwForwDatagrams,
                  pStats->dwInUnknownProtos,
                  pStats->dwInDiscards,
                  pStats->dwInDelivers,
                  pStats->dwOutRequests,
                  pStats->dwRoutingDiscards,
                  pStats->dwOutDiscards,
                  pStats->dwOutNoRoutes,
                  pStats->dwReasmTimeout,
                  pStats->dwReasmReqds,
                  pStats->dwReasmOks,
                  pStats->dwReasmFails,
                  pStats->dwFragOks,
                  pStats->dwFragFails,
                  pStats->dwFragCreates,
                  pStats->dwNumIf,
                  pStats->dwNumAddr,
                  pStats->dwNumRoutes);
    }

	// 获取ICMP统计数据
	MIB_ICMP IcmpStats;
	if(::GetIcmpStatistics(&IcmpStats) == NO_ERROR)
	{
		// MIB_ICMP结构中包含了MIBICMPINFO结构
		MIBICMPINFO *pStats = (MIBICMPINFO*)&IcmpStats;
		printf("\n%20s %10s %10s\n","ICMP Statistics", "IN", "OUT");
        printf("%20s %10s %10s\n","---------------", "------", "------");
        printf("%20s %10lu %10lu\n", 
			"dwMsgs", pStats->icmpInStats.dwMsgs, pStats->icmpOutStats.dwMsgs);
        printf("%20s %10lu %10lu\n", 
			"dwErrors", pStats->icmpInStats.dwErrors, pStats->icmpOutStats.dwErrors);
        printf("%20s %10lu %10lu\n", 
			"dwDestUnreachs", pStats->icmpInStats.dwDestUnreachs, pStats->icmpOutStats.dwDestUnreachs);
        printf("%20s %10lu %10lu\n", 
			"dwTimeExcds", pStats->icmpInStats.dwTimeExcds, pStats->icmpOutStats.dwTimeExcds);
        printf("%20s %10lu %10lu\n", 
			"dwParmProbs", pStats->icmpInStats.dwParmProbs, pStats->icmpOutStats.dwParmProbs);
        printf("%20s %10lu %10lu\n", 
			"dwSrcQuenchs", pStats->icmpInStats.dwSrcQuenchs, pStats->icmpOutStats.dwSrcQuenchs);
        printf("%20s %10lu %10lu\n", 
			"dwRedirects", pStats->icmpInStats.dwRedirects, pStats->icmpOutStats.dwRedirects);
        printf("%20s %10lu %10lu\n", 
			"dwEchos", pStats->icmpInStats.dwEchos, pStats->icmpOutStats.dwEchos);
        printf("%20s %10lu %10lu\n", 
			"dwEchoReps", pStats->icmpInStats.dwEchoReps, pStats->icmpOutStats.dwEchoReps);
        printf("%20s %10lu %10lu\n", 
			"dwTimestamps", pStats->icmpInStats.dwTimestamps, pStats->icmpOutStats.dwTimestamps);
        printf("%20s %10lu %10lu\n", 
			"dwTimestampReps", pStats->icmpInStats.dwTimestampReps, pStats->icmpOutStats.dwTimestampReps);
        printf("%20s %10lu %10lu\n", 
			"dwAddrMasks", pStats->icmpInStats.dwAddrMasks, pStats->icmpOutStats.dwAddrMasks);
        printf("%20s %10lu %10lu\n", 
			"dwAddrMaskReps", pStats->icmpInStats.dwAddrMaskReps, pStats->icmpOutStats.dwAddrMaskReps);
	}

	// 获取TCP统计数据
	MIB_TCPSTATS TcpStats;
	if(::GetTcpStatistics(&TcpStats) == NO_ERROR)
	{
		MIB_TCPSTATS *pStats = &TcpStats;
        printf("\nTCP Statistics\n");
        printf("\
				  dwRtoAlgorithm     = %lu\n\
				  dwRtoMin           = %lu\n\
				  dwRtoMax           = %lu\n\
				  dwMaxConn          = %lu\n\
				  dwActiveOpens      = %lu\n\
				  dwPassiveOpens     = %lu\n\
				  dwAttemptFails     = %lu\n\
				  dwEstabResets      = %lu\n\
				  dwCurrEstab        = %lu\n\
				  dwInSegs           = %lu\n\
				  dwOutSegs          = %lu\n\
				  dwRetransSegs      = %lu\n\
				  dwInErrs           = %lu\n\
				  dwOutRsts          = %lu\n\
				  dwNumConns         = %lu\n",
				  pStats->dwRtoAlgorithm,
				  pStats->dwRtoMin,
				  pStats->dwRtoMax,
				  pStats->dwMaxConn,
				  pStats->dwActiveOpens,
				  pStats->dwPassiveOpens,
				  pStats->dwAttemptFails,
				  pStats->dwEstabResets,
				  pStats->dwCurrEstab,
				  pStats->dwInSegs,
				  pStats->dwOutSegs,
				  pStats->dwRetransSegs,
				  pStats->dwInErrs,
				  pStats->dwOutRsts,
				  pStats->dwNumConns);
		
	}

	// 获取UDP统计数据
	MIB_UDPSTATS UdpStats;
	if(::GetUdpStatistics(&UdpStats) == NO_ERROR)
	{
		MIB_UDPSTATS *pStats = &UdpStats;
		printf("\nUDP Statistics\n");
        printf("\
				  dwInDatagrams      = %lu\n\
				  dwNoPorts          = %lu\n\
				  dwInErrors         = %lu\n\
				  dwOutDatagrams     = %lu\n\
				  dwNumAddrs         = %lu\n",
				  pStats->dwInDatagrams,
				  pStats->dwNoPorts,
				  pStats->dwInErrors,
				  pStats->dwOutDatagrams,
				  pStats->dwNumAddrs);
	}

	return 0;
}


