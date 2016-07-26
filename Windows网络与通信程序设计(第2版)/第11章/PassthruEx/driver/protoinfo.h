//////////////////////////////////////////////////
// protoinfo.h文件

/*

定义协议格式
定义协议中使用的宏

  */


#ifndef __PROTOINFO_H__
#define __PROTOINFO_H__



////////////////////////////////////////////
// 宏定义










/////////////////////////////////////////////////////////
// 协议格式

#define ETHERTYPE_IP    0x0800
#define ETHERTYPE_ARP   0x0806

typedef struct _ETHeader         // 14 bytes
{
	UCHAR	dhost[6];			// 目的MAC地址destination mac address
	UCHAR	shost[6];			// 源MAC地址source mac address
	USHORT	type;				// 下层协议类型，如IP（ETHERTYPE_IP）、ARP（ETHERTYPE_ARP）等
} ETHeader, *PETHeader;


#define ARPHRD_ETHER 	1

// ARP协议opcodes
#define	ARPOP_REQUEST	1		// ARP 请求	
#define	ARPOP_REPLY		2		// ARP 响应



typedef struct _ARPHeader		// 28字节的ARP头
{
	USHORT	hrd;				//	硬件地址空间，以太网中为ARPHRD_ETHER
	USHORT	eth_type;			//  以太网类型，ETHERTYPE_IP ？？
	UCHAR	maclen;				//	MAC地址的长度，为6
	UCHAR	iplen;				//	IP地址的长度，为4
	USHORT	opcode;				//	操作代码，ARPOP_REQUEST为请求，ARPOP_REPLY为响应
	UCHAR	smac[6];			//	源MAC地址
	UCHAR	saddr[4];			//	源IP地址
	UCHAR	dmac[6];			//	目的MAC地址
	UCHAR	daddr[4];			//	目的IP地址
} ARPHeader, *PARPHeader;



//Protocol
#define PROTO_ICMP    1
#define PROTO_IGMP    2
#define PROTO_TCP     6
#define PROTO_UDP     17

typedef struct _IPHeader		// 20
{
    UCHAR     iphVerLen;      // 版本号和头长度（各占4位）
    UCHAR     ipTOS;          // 服务类型 
    USHORT    ipLength;       // 封包总长度，即整个IP报的长度
    USHORT    ipID;			  // 封包标识，惟一标识发送的每一个数据报
    USHORT    ipFlags;	      // 标志
    UCHAR     ipTTL;	      // 生存时间，就是TTL
    UCHAR     ipProtocol;     // 协议，可能是TCP、UDP、ICMP等
    USHORT    ipChecksum;     // 校验和
    ULONG     ipSource;       // 源IP地址
    ULONG     ipDestination;  // 目标IP地址
} IPHeader, *PIPHeader; 


//  define the tcp flags....
#define   TCP_FIN   0x01
#define   TCP_SYN   0x02
#define   TCP_RST   0x04
#define   TCP_PSH   0x08
#define   TCP_ACK   0x10
#define   TCP_URG   0x20
#define   TCP_ACE   0x40
#define   TCP_CWR   0x80

typedef struct _TCPHeader	 //20 bytes
{
	USHORT			sourcePort;		// 16位源端口号
	USHORT			destinationPort;	// 16位目的端口号
	ULONG			sequenceNumber;		// 32位序列号
	ULONG			acknowledgeNumber;	// 32位确认号


	UCHAR			dataoffset;		// 高4位表示数据偏移
	UCHAR			flags;			// 6位标志位
								//FIN - 0x01
								//SYN - 0x02
								//RST - 0x04 
								//PUSH- 0x08
								//ACK- 0x10
								//URG- 0x20
								//ACE- 0x40
								//CWR- 0x80


	USHORT			windows;		// 16位窗口大小
	USHORT			checksum;		// 16位校验和
	USHORT			urgentPointer;		// 16位紧急数据偏移量 
} TCPHeader, *PTCPHeader;

typedef struct _UDPHeader
{
	USHORT			sourcePort;		// 源端口号		
	USHORT			destinationPort;// 目的端口号		
	USHORT			len;			// 封包长度
	USHORT			checksum;		// 校验和
} UDPHeader, *PUDPHeader;


typedef struct _ICMPHeader
{
    UCHAR   type;
    UCHAR   code;
    USHORT  checksum;
    USHORT  id;
    USHORT  sequence;
    ULONG   timestamp;
} ICMPHeader, *PICMPHeader;


#endif // __PROTOINFO_H__




/*





#define ETHER_LENGTH  14
#define IP_LENGTH     20
#define TCP_LENGTH    20
#define UDP_LENGTH    8
#define ICMP_LENGTH   8
#define ARP_LENGTH    28
#define PSEUDO_LENGTH 12
#define DATA_LENGTH   32


typedef struct ether_hdr // 14 bytes
{
	u_char dhost[6];			//destination mac address
	u_char shost[6];			//source mac address
	u_short type;               //IP ,ARP , RARP       Next Layer Protocol IP:0800 ARP:0806
} ETHER_HDR, *PETHER_HDR;

// 4 byte ip address
typedef struct ip_addr
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
} IP_ADDR, *PIP_ADDR;


typedef struct arp_hdr   //28 bytes ARP_LENGTH
{
	u_short hrd;       //hardware address space=0x0001
	u_short eth_type;  //Ethernet type ....=0x0800
	u_char maclen;     //Length of mac address=6
	u_char iplen;      //Length of ip addres=4
	u_short opcode;    //Request =1 Reply=2 (highbyte)
	u_char smac[6];		//source mac address
	ip_addr saddr;		//Source ip address
	u_char dmac[6];    //Destination mac address
	ip_addr daddr;		//Destination ip address

} ARP_HDR, *PARP_HDR;



typedef struct ip_hdr
{
    unsigned char  ip_verlen;        // 4-bit IPv4 version
	// 4-bit header length (in 32-bit words)
    unsigned char  ip_tos;           // IP type of service
    unsigned short ip_totallength;   // Total length
    unsigned short ip_id;            // Unique identifier 
    unsigned short ip_offset;        // Fragment offset field
    unsigned char  ip_ttl;           // Time to live
    unsigned char  ip_protocol;      // Protocol(TCP,UDP etc)
    unsigned short ip_checksum;      // IP checksum
    unsigned int   ip_srcaddr;       // Source address
    unsigned int   ip_destaddr;      // Source address
} IPV4_HDR, *PIPV4_HDR, FAR * LPIPV4_HDR;

typedef	struct tcp_hdr  //20 bytes
{
	u_short sport;      //Source port
	u_short dport;      //Destination port
	u_long seqno;       //Sequence no
	u_long ackno;       //Ack no
	u_char offset;      //Higher level 4 bit indicates data offset
	u_char flag;        //Message flag
					//FIN - 0x01
				//SYN - 0x02
				//RST - 0x04 
				//PUSH- 0x08
				//ACK- 0x10
				//URG- 0x20
				//ACE- 0x40
				//CWR- 0x80
	
	u_short win;
	u_short checksum;
	u_short uptr;
} TCP_HDR, *PTCP_HDR;


//For checksum calculation purpose
struct pseudo_hdr  //12 bytes 
{
    unsigned int  saddr;      // Source address
    unsigned int  daddr;      // Destination address
	u_char zero;
	u_char  proto;          // Protocol
	u_short tcp_len;
	tcp_hdr tcp;
};

typedef struct icmp_hdr
{
    unsigned char   icmp_type;
    unsigned char   icmp_code;
    unsigned short  icmp_checksum;
    unsigned short  icmp_id;
    unsigned short  icmp_sequence;
    unsigned long   icmp_timestamp;
} ICMP_HDR, *PICMP_HDR, FAR *LPICMP_HDR;




  */