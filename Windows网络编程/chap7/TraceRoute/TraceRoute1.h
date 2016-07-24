// TraceRoute1.h: interface for the CTraceRoute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACEROUTE1_H__1391ADE4_E306_44BB_8EA0_89DD3045AD6D__INCLUDED_)
#define AFX_TRACEROUTE1_H__1391ADE4_E306_44BB_8EA0_89DD3045AD6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(4)

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <stdlib.h>


//
// Defines for ICMP message types
//
#define ICMP_ECHOREPLY      0
#define ICMP_DESTUNREACH    3
#define ICMP_SRCQUENCH      4
#define ICMP_REDIRECT       5
#define ICMP_ECHO           8
#define ICMP_TIMEOUT       11
#define ICMP_PARMERR       12

#define MAX_HOPS           30

#define ICMP_MIN 8    // Minimum 8 byte icmp packet (just header)

//
// IP Header
//
typedef struct iphdr 
{
    unsigned int   h_len:4;        // Length of the header
    unsigned int   version:4;      // Version of IP
    unsigned char  tos;            // Type of service
    unsigned short total_len;      // Total length of the packet
    unsigned short ident;          // Unique identifier
    unsigned short frag_and_flags; // Flags
    unsigned char  ttl;            // Time to live
    unsigned char  proto;          // Protocol (TCP, UDP etc)
    unsigned short checksum;       // IP checksum
    unsigned int   sourceIP;       // Source IP
    unsigned int   destIP;         // Destination IP
} IpHeader;

//
// ICMP header
//
typedef struct _ihdr 
{
    BYTE   i_type;               // ICMP message type
    BYTE   i_code;               // Sub code
    USHORT i_cksum;              
    USHORT i_id;                 // Unique id
    USHORT i_seq;                // Sequence number
    // This is not the std header, but we reserve space for time
    ULONG timestamp;
} IcmpHeader;

#define DEF_PACKET_SIZE         32
#define MAX_PACKET            1024


class CTraceRoute  
{
public:
	CTraceRoute();
	virtual ~CTraceRoute();
public:
	int decode_resp(char *buf, int bytes, SOCKADDR_IN *from, int ttl);
	void Cleanup();
	USHORT checksum(USHORT *buffer, int size);
	int set_ttl(SOCKET s, int nTimeToLive);
	void fill_icmp_data(char * icmp_data, int datasize);
	void ConnectToHost(char* strHost);
	SOCKET m_sockRaw;
	SOCKADDR_IN m_addrDest;
	SOCKADDR_IN m_addrFrom;
	int         m_nDatasize;
    int         m_nMaxhops;
	int         m_nTTL;
	char*       m_IcmpData;
	char*       m_RcvBuffer;
	int         m_nSeqno;
	int         m_nTimeout;
	BOOL        m_bDone;
};

#endif // !defined(AFX_TRACEROUTE1_H__1391ADE4_E306_44BB_8EA0_89DD3045AD6D__INCLUDED_)
