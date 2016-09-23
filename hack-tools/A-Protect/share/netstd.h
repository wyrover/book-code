#ifndef _NETSTD_H
#define _NETSTD_H 1

#ifndef _NETDEF_H
	#include "netdef.h"
#endif

typedef unsigned char	__u8;
typedef unsigned short	__u16;
typedef unsigned long	__u32;

/*
 *	This is an Ethernet frame header. (linux/if_ether.h)
 */
 
struct ethhdr
{
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	unsigned short	h_proto;			/* packet type ID field	*/
};

/*
 *	This structure defines an ethernet arp header.
 */

struct arphdr
{
	unsigned short	ar_hrd;		/* format of hardware address	*/
	unsigned short	ar_pro;		/* format of protocol address	*/
	unsigned char	ar_hln;		/* length of hardware address	*/
	unsigned char	ar_pln;		/* length of protocol address	*/
	unsigned short	ar_op;		/* ARP opcode (command)			*/

#if 1
	 /*
	  *	 Ethernet looks like this : This bit is variable sized however...
	  */
	unsigned char	ar_sha[ETH_ALEN];	/* sender hardware address	*/
	unsigned char	ar_sip[4];			/* sender IP address		*/
	unsigned char	ar_tha[ETH_ALEN];	/* target hardware address	*/
	unsigned char	ar_tip[4];			/* target IP address		*/
#endif
};

/* IP flags. */
#define IP_CE		0x8000		/* Flag: "Congestion"		*/
#define IP_DF		0x4000		/* Flag: "Don't Fragment"	*/
#define IP_MF		0x2000		/* Flag: "More Fragments"	*/
#define IP_OFFSET	0x1FFF		/* "Fragment Offset" part	*/

struct iphdr
{
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	ihl:4,
		version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	version:4,
  		ihl:4;
#else
#error "No endian type defined"
#endif
	__u8	tos;
	__u16	tot_len;
	__u16	id;
	__u16	frag_off; /* IP flags. */
	__u8	ttl;
	__u8	protocol;
	__u16	check;
	__u32	saddr;
	__u32	daddr;
	/*The options start here. */
};

/* (linux/tcp.h) */
struct tcphdr
{
	__u16	source;
	__u16	dest;
	__u32	seq;
	__u32	ack_seq;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	res1:4,
		doff:4,
		fin:1,
		syn:1,
		rst:1,
		psh:1,
		ack:1,
		urg:1,
		res2:2;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	doff:4,
		res1:4,
		res2:2,
		urg:1,
		ack:1,
		psh:1,
		rst:1,
		syn:1,
		fin:1;
#else
#error "No endian type defined"
#endif
	__u16	window;
	__u16	check;
	__u16	urg_ptr;
};

struct tcp /* BSD style */
{
    __u16	th_sport;	/* source port */
    __u16	th_dport;	/* destination port */
    __u32	th_seq;		/* sequence number */
    __u32	th_ack;		/* acknowledgement number */
#if defined(__LITTLE_ENDIAN_BITFIELD)
    __u8	th_x2:4;	/* (unused) */
    __u8	th_off:4;	/* data offset */
#elif defined(__BIG_ENDIAN_BITFIELD)
    __u8	th_off:4;	/* data offset */
    __u8	th_x2:4;	/* (unused) */
#else
#error "No endian type defined"
#endif
    __u8	th_flags;
#define TH_FIN	0x01
#define TH_SYN	0x02
#define TH_RST	0x04
#define TH_PUSH	0x08
#define TH_ACK	0x10
#define TH_URG	0x20
    __u16	th_win;		/* window */
    __u16	th_sum;		/* checksum */
    __u16	th_urp;		/* urgent pointer */
};

struct pseudo_tcphdr
{
	__u32	saddr, daddr;
	__u8	zero;
	__u8	ptcl; // protocol
	__u16	tcpl; // tcp length
}; /* RFC 793 */
/*
  The TCP Length is the TCP header length plus the data length in
  octets (this is not an explicitly transmitted quantity, but is
  computed), and it does not count the 12 octets of the pseudo header.
*/

/* (linux/udp.h) */
struct udphdr
{
	__u16	source;
	__u16	dest;
	__u16	len;
	__u16	check;
}; /* RFC 768 */

/* (linux/icmp.h) */
/*
struct icmphdr
{
	__u8	type;
	__u8	code;
	__u16	checksum;
	union
	{
		struct
		{
			__u16	id;
			__u16	sequence;
		} echo;
		__u32	gateway;
		struct
		{
			__u16	__unused;
			__u16	mtu;
		} frag;
	} un;
};
*/

struct icmphdr
{
	__u8	type;
	__u8	code;
	__u16	checksum;
	union
	{
		struct
		{
			__u16	id;
			__u16	sequence;
		};
		__u32	gateway;
		struct
		{
			__u16	__unused;
			__u16	mtu;
		};
	};
};

#define NIPQUAD(ip) ((ip)&0xff), (((ip)>>8)&0xff), (((ip)>>16)&0xff), (((ip)>>24)&0xff)

#define ___swab16(x) \
	((__u16)( \
		(((__u16)(x) & (__u16)0x00ffU) << 8) | \
		(((__u16)(x) & (__u16)0xff00U) >> 8) ))
#define ___swab32(x) \
	((__u32)( \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24) ))
#define ___swab64(x) \
	((__u64)( \
		(__u64)(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) | \
		(__u64)(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) | \
		(__u64)(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) | \
		(__u64)(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) | \
	    (__u64)(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) | \
		(__u64)(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) | \
		(__u64)(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) | \
		(__u64)(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56) ))

#if defined(__LITTLE_ENDIAN_BITFIELD)
#define __constant_htonl(x) ___swab32((x))
#define __constant_ntohl(x) ___swab32((x))
#define __constant_htons(x) ___swab16((x))
#define __constant_ntohs(x) ___swab16((x))
#elif defined(__BIG_ENDIAN_BITFIELD)
#define __constant_htonl(x) ((__u32)(x))
#define __constant_ntohl(x) ((__u32)(x))
#define __constant_htons(x) ((__u16)(x))
#define __constant_ntohs(x) ((__u16)(x))
#else
#error "No endian type defines"
#endif

#define _htonl(x)	__constant_htonl(x)
#define _ntohl(x)	__constant_ntohl(x)
#define _htons(x)	__constant_htons(x)
#define _ntohs(x)	__constant_ntohs(x)

#endif
