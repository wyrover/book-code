// TraceRoute1.cpp: implementation of the CTraceRoute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TraceRoute1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceRoute::CTraceRoute()
{
   m_nTTL = 1;
   m_nMaxhops = MAX_HOPS;
   m_sockRaw = INVALID_SOCKET;
   m_RcvBuffer = NULL;
   m_IcmpData  = NULL;
   m_nTimeout  = 1000;
   m_bDone     = FALSE;
      
   WSADATA wsaData;
   if (WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
   {
	   printf("WSAStartup() failed: %d/n", WSAGetLastError());
   }
     
}

CTraceRoute::~CTraceRoute()
{
    
}

void CTraceRoute::ConnectToHost(char* strHost)
{
    m_sockRaw = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP,
                          NULL, 0,WSA_FLAG_OVERLAPPED);	                     
    if (m_sockRaw == INVALID_SOCKET) 
    {   
		printf("WSASocket() failed: %d\n", WSAGetLastError());
        ExitProcess(-1);
    }
    
	int ret = setsockopt(m_sockRaw, SOL_SOCKET, SO_RCVTIMEO, 
                        (char *)&m_nTimeout, sizeof(m_nTimeout));
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt(SO_RCVTIMEO) failed: %d\n", WSAGetLastError());
        return ;
    }
    
	ret = setsockopt(m_sockRaw, SOL_SOCKET, SO_SNDTIMEO, 
        (char *)&m_nTimeout, sizeof(m_nTimeout));
    if (ret == SOCKET_ERROR)
    {   
		printf("setsockopt(SO_SNDTIMEO) failed: %d\n", WSAGetLastError());
        return ;   
    }

	m_addrDest.sin_family = AF_INET;
    if ((m_addrDest.sin_addr.s_addr = inet_addr(strHost)) == INADDR_NONE)
    {   
		HOSTENT* hp;
        hp = gethostbyname(strHost);
        if (hp)
            memcpy(&(m_addrDest.sin_addr), hp->h_addr, hp->h_length);
        else
        {
            printf("gethostbyname failed: %d\n", WSAGetLastError());
            return ;    
        }
    }

	int bOpt = TRUE;
    if (setsockopt(m_sockRaw, SOL_SOCKET, SO_DONTROUTE, (char *)&bOpt, 
            sizeof(BOOL)) == SOCKET_ERROR)
	{
			printf("setsockopt failed: %d\n", WSAGetLastError());
            return ;
	}
    m_nDatasize = DEF_PACKET_SIZE;
	m_nDatasize += sizeof(IcmpHeader);  
    //
    // Allocate the sending and receiving buffers for ICMP packets
    //
    m_IcmpData = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PACKET);
    m_RcvBuffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PACKET);

   if ((!m_IcmpData) || (!m_RcvBuffer))
    {
        printf("HeapAlloc() failed: %d\n", WSAGetLastError());
        return ;
    }
    // Set the socket to bypass the standard routing mechanisms 
    //  i.e. use the local protocol stack to the appropriate network
    //       interface
    //
    
    //  
    // Here we are creating and filling in an ICMP header that is the 
    // core of trace route.
    //
   
     memset(m_IcmpData, 0, MAX_PACKET);
     printf("\nTracing route to %s over a maximum of %d hops:\n\n", 
     strHost, m_nMaxhops);
    fill_icmp_data(m_IcmpData, m_nDatasize);
	
	for(m_nTTL = 1; ((m_nTTL < m_nMaxhops) && (!m_bDone)); m_nTTL++)
    {
        int bwrote;

        // Set the time to live option on the socket
        //
        set_ttl(m_sockRaw, m_nTTL);

        //
        // Fill in some more data in the ICMP header
        //
        ((IcmpHeader*)m_IcmpData)->i_cksum = 0;
        ((IcmpHeader*)m_IcmpData)->timestamp = GetTickCount();

        ((IcmpHeader*)m_IcmpData)->i_seq = m_nSeqno++;
        ((IcmpHeader*)m_IcmpData)->i_cksum = checksum((USHORT*)m_IcmpData, 
            m_nDatasize);
        //
        // Send the ICMP packet to the destination
        //
        bwrote = sendto(m_sockRaw, m_IcmpData, m_nDatasize, 0, 
                    (SOCKADDR *)&m_addrDest, sizeof(m_addrDest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("%2d  Send request timed out.\n", m_nTTL);
                continue;
            }
            printf("sendto() failed: %d\n", WSAGetLastError());
            return ;
        }
        // Read a packet back from the destination or a router along 
        // the way.
        //
		int fromlen;
		fromlen = sizeof(SOCKADDR);
        ret = recvfrom(m_sockRaw, m_RcvBuffer, MAX_PACKET, 0, 
            (struct sockaddr*)&m_addrFrom, &fromlen);
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("%2d  Receive Request timed out.\n", m_nTTL);
                continue;
            }
            printf("recvfrom() failed: %d\n", WSAGetLastError());
            return ;
        }
        //
        // Decode the response to see if the ICMP response is from a 
        // router along the way or whether it has reached the destination.
        //
        m_bDone = decode_resp(m_RcvBuffer, ret, &m_addrFrom, m_nTTL);
        Sleep(1000);
    }   

 }

void CTraceRoute::fill_icmp_data(char * icmp_data, int datasize)
{
     IcmpHeader *icmp_hdr;
    char       *datapart;

    icmp_hdr = (IcmpHeader*)icmp_data;

    icmp_hdr->i_type = ICMP_ECHO;
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id   = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
  
    datapart = icmp_data + sizeof(IcmpHeader);
    //
    // Place some junk in the buffer. Don't care about the data...
    //
    memset(datapart,'E', datasize - sizeof(IcmpHeader));
}



int CTraceRoute::set_ttl(SOCKET s, int nTimeToLive)
{
    int     nRet;
    
    nRet = setsockopt(s, IPPROTO_IP, IP_TTL, (LPSTR)&nTimeToLive,
                sizeof(int));
    if (nRet == SOCKET_ERROR)
    {
        printf("setsockopt(IP_TTL) failed: %d\n", 
            WSAGetLastError());
        return 0;
    }
    return 1;
}

USHORT CTraceRoute::checksum(USHORT *buffer, int size)
{
    unsigned long cksum=0;
	
    while(size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if(size )
        cksum += *(UCHAR*)buffer;
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
	
    return (USHORT)(~cksum);
}

void CTraceRoute::Cleanup()
{
    HeapFree(GetProcessHeap(), 0, m_RcvBuffer);
    HeapFree(GetProcessHeap(), 0, m_IcmpData);
	if (m_sockRaw != NULL)
		closesocket(m_sockRaw);
	WSACleanup();
}

int CTraceRoute::decode_resp(char *buf, int bytes, SOCKADDR_IN *from, int ttl)
{
  IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    struct hostent *lpHostent = NULL;
    struct in_addr  inaddr = from->sin_addr;

    iphdr = (IpHeader *)buf;
    // Number of 32-bit words * 4 = bytes
	iphdrlen = iphdr->h_len * 4; 

    if (bytes < iphdrlen + ICMP_MIN) 
        printf("Too few bytes from %s\n",
            inet_ntoa(from->sin_addr));

    icmphdr = (IcmpHeader*)(buf + iphdrlen);

    switch (icmphdr->i_type)
    {
        case ICMP_ECHOREPLY:     // Response from destination
            lpHostent = gethostbyaddr((const char *)&from->sin_addr, 
                AF_INET, sizeof(struct in_addr));
            if (lpHostent != NULL)
                printf("%2d  %s (%s)\n", ttl, lpHostent->h_name, 
                    inet_ntoa(inaddr));
            return 1;
            break;
        case ICMP_TIMEOUT:      // Response from router along the way
            printf("%2d  %s\n", ttl, inet_ntoa(inaddr));
            return 0;
            break;
        case ICMP_DESTUNREACH:  // Can't reach the destination at all
            printf("%2d  %s  reports: Host is unreachable\n", ttl, 
                inet_ntoa(inaddr));
            return 1;
            break;
        default:
            printf("non-echo type %d recvd\n", icmphdr->i_type);
            return 1;
            break;
    }
    return 0;
}
