// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil -*- (for GNU Emacs)
//
// $Id: net.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _net_h_
#define _net_h_

/*
 * network definitions
 */

#pragma pack(1)

struct ether_hdr {
	UCHAR	ether_dhost[6];
	UCHAR	ether_shost[6];
	USHORT	ether_type;
};

#define	ETHERTYPE_IP		0x0800	/* IP protocol */

#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */

struct ip_hdr {
	UCHAR	ip_hl:4;	/* header length */
	UCHAR	ip_v:4;		/* version */
	UCHAR	ip_tos;		/* type of service */
	USHORT	ip_len;		/* total length */
	USHORT	ip_id;		/* identification */
	USHORT	ip_off;		/* fragment offset field */
	UCHAR	ip_ttl;		/* time to live */
	UCHAR	ip_p;		/* protocol */
	USHORT	ip_sum;		/* checksum */
	ULONG	ip_src;		/* source address */
	ULONG	ip_dst;		/* dest address */
};

struct icmp_hdr {
	UCHAR	icmp_type;		/* type of message */
	UCHAR	icmp_code;		/* type sub code */
	USHORT	icmp_cksum;		/* ones complement cksum of struct */
};

struct udp_hdr {
	USHORT	uh_sport;		/* source port */
	USHORT	uh_dport;		/* destination port */
	USHORT	uh_ulen;		/* udp length */
	USHORT	uh_sum;			/* udp checksum */
};

#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20

struct tcp_hdr {
	USHORT	th_sport;		/* source port */
	USHORT	th_dport;		/* destination port */
	ULONG	th_seq;			/* sequence number */
	ULONG	th_ack;			/* acknowledgement number */

	UCHAR	th_x2:4;		/* (unused) */
	UCHAR	th_off:4;		/* data offset */
	UCHAR	th_flags;

	USHORT	th_win;			/* window */
	USHORT	th_sum;			/* checksum */
	USHORT	th_urp;			/* urgent pointer */
};

#pragma pack()

#endif
