#ifndef _NETDEF_H
#define _NETDEF_H

#ifdef _M_IX86
	#ifndef __LITTLE_ENDIAN_BITFIELD
		#define __LITTLE_ENDIAN_BITFIELD
	#endif
#endif

/*
 *	IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
 *	and FCS/CRC (frame check sequence). 
 */

#define ETH_ALEN		6		/* Octets in one ethernet addr	 */
#define ETH_HLEN		14		/* Total octets in header.		 */
#define ETH_ZLEN		60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN	1500	/* Max. octets in payload		 */
#define ETH_FRAME_LEN	1514	/* Max. octets in frame sans FCS */

/*
 *	These are the defined Ethernet Protocol ID's.
 */

#define ETH_P_LOOP		0x0060		/* Ethernet Loopback packet		*/
#define ETH_P_ECHO		0x0200		/* Ethernet Echo packet			*/
#define ETH_P_PUP		0x0400		/* Xerox PUP packet				*/
#define ETH_P_IP		0x0800		/* Internet Protocol packet		*/
#define ETH_P_X25		0x0805		/* CCITT X.25					*/
#define ETH_P_ARP		0x0806		/* Address Resolution packet	*/
#define	ETH_P_BPQ		0x08FF		/* G8BPQ AX.25 Ethernet Packet	[ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_P_DEC       0x6000      /* DEC Assigned proto           */
#define ETH_P_DNA_DL    0x6001      /* DEC DNA Dump/Load            */
#define ETH_P_DNA_RC    0x6002      /* DEC DNA Remote Console       */
#define ETH_P_DNA_RT    0x6003      /* DEC DNA Routing              */
#define ETH_P_LAT       0x6004      /* DEC LAT                      */
#define ETH_P_DIAG      0x6005      /* DEC Diagnostics              */
#define ETH_P_CUST      0x6006      /* DEC Customer use             */
#define ETH_P_SCA       0x6007      /* DEC Systems Comms Arch       */
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet		*/
#define ETH_P_ATALK		0x809B		/* Appletalk DDP				*/
#define ETH_P_AARP		0x80F3		/* Appletalk AARP				*/
#define ETH_P_IPX		0x8137		/* IPX over DIX					*/
#define ETH_P_IPV6		0x86DD		/* IPv6 over bluebook			*/
#define ETH_P_ATMMPOA	0x884c		/* MultiProtocol Over ATM		*/
#define ETH_P_ATMFATE	0x8884		/* Frame-based ATM Transport over Ethernet */

/*
 *	Non DIX types. Won't clash for 1500 types.
 */
 
#define ETH_P_802_3		0x0001		/* Dummy type for 802.3 frames  */
#define ETH_P_AX25		0x0002		/* Dummy protocol id for AX.25  */
#define ETH_P_ALL		0x0003		/* Every packet (be careful!!!) */
#define ETH_P_802_2		0x0004		/* 802.2 frames 				*/
#define ETH_P_SNAP		0x0005		/* Internal only				*/
#define ETH_P_DDCMP     0x0006      /* DEC DDCMP: Internal only     */
#define ETH_P_WAN_PPP   0x0007      /* Dummy type for WAN PPP frames*/
#define ETH_P_PPP_MP    0x0008      /* Dummy type for PPP MP frames */
#define ETH_P_LOCALTALK 0x0009		/* Localtalk pseudo type		*/
#define ETH_P_PPPTALK	0x0010		/* Dummy type for Atalk over PPP*/
#define ETH_P_TR_802_2	0x0011		/* 802.2 frames 				*/
#define ETH_P_MOBITEX	0x0015		/* Mobitex (kaz@cafe.net)		*/
#define ETH_P_CONTROL	0x0016		/* Card specific control frames */
#define ETH_P_IRDA		0x0017		/* Linux/IR						*/

/* ARP protocol HARDWARE identifiers. */
#define ARPHRD_NETROM	0		/* from KA9Q: NET/ROM pseudo	*/
#define ARPHRD_ETHER 	1		/* Ethernet 10Mbps		*/
#define	ARPHRD_EETHER	2		/* Experimental Ethernet	*/
#define	ARPHRD_AX25		3		/* AX.25 Level 2		*/
#define	ARPHRD_PRONET	4		/* PROnet token ring		*/
#define	ARPHRD_CHAOS	5		/* Chaosnet			*/
#define	ARPHRD_IEEE802	6		/* IEEE 802.2 Ethernet/TR/TB	*/
#define	ARPHRD_ARCNET	7		/* ARCnet			*/
#define	ARPHRD_APPLETLK	8		/* APPLEtalk			*/
#define ARPHRD_DLCI		15		/* Frame Relay DLCI		*/
#define ARPHRD_ATM		19		/* ATM 				*/
#define ARPHRD_METRICOM	23		/* Metricom STRIP (new IANA id)	*/
#define	ARPHRD_IEEE1394	24		/* IEEE 1394 IPv4 - RFC 2734	*/
#define ARPHRD_EUI64	27		/* EUI-64                       */

/* Dummy types for non ARP hardware */
#define ARPHRD_SLIP		256
#define ARPHRD_CSLIP	257
#define ARPHRD_SLIP6	258
#define ARPHRD_CSLIP6	259
#define ARPHRD_RSRVD	260		/* Notional KISS type 		*/
#define ARPHRD_ADAPT	264
#define ARPHRD_ROSE		270
#define ARPHRD_X25		271		/* CCITT X.25			*/
#define ARPHRD_HWX25	272		/* Boards with X.25 in firmware	*/
#define ARPHRD_PPP		512
#define ARPHRD_CISCO	513		/* Cisco HDLC	 		*/
#define ARPHRD_HDLC		ARPHRD_CISCO
#define ARPHRD_LAPB		516		/* LAPB				*/
#define ARPHRD_DDCMP    517		/* Digital's DDCMP protocol     */
#define ARPHRD_RAWHDLC	518		/* Raw HDLC			*/

#define ARPHRD_TUNNEL	768		/* IPIP tunnel			*/
#define ARPHRD_TUNNEL6	769		/* IPIP6 tunnel			*/
#define ARPHRD_FRAD		770             /* Frame Relay Access Device    */
#define ARPHRD_SKIP		771		/* SKIP vif			*/
#define ARPHRD_LOOPBACK	772		/* Loopback device		*/
#define ARPHRD_LOCALTLK 773		/* Localtalk device		*/
#define ARPHRD_FDDI		774		/* Fiber Distributed Data Interface */
#define ARPHRD_BIF      775             /* AP1000 BIF                   */
#define ARPHRD_SIT		776		/* sit0 device - IPv6-in-IPv4	*/
#define ARPHRD_IPDDP	777		/* IP over DDP tunneller	*/
#define ARPHRD_IPGRE	778		/* GRE over IP			*/
#define ARPHRD_PIMREG	779		/* PIMSM register interface	*/
#define ARPHRD_HIPPI	780		/* High Performance Parallel Interface */
#define ARPHRD_ASH		781		/* Nexus 64Mbps Ash		*/
#define ARPHRD_ECONET	782		/* Acorn Econet			*/
#define ARPHRD_IRDA 	783		/* Linux-IrDA			*/
/* ARP works differently on different FC media .. so  */
#define ARPHRD_FCPP		784		/* Point to point fibrechannel	*/
#define ARPHRD_FCAL		785		/* Fibrechannel arbitrated loop */
#define ARPHRD_FCPL		786		/* Fibrechannel public loop	*/
#define ARPHRD_FCFABRIC	787		/* Fibrechannel fabric		*/
	/* 787->799 reserved for fibrechannel media types */
#define ARPHRD_IEEE802_TR	800		/* Magic type ident for TR	*/
#define ARPHRD_IEEE80211	801		/* IEEE 802.11			*/
#define ARPHRD_IEEE80211_PRISM 802	/* IEEE 802.11 + Prism2 header  */

#define ARPHRD_VOID		0xFFFF	/* Void type, nothing is known */

/* ARP protocol opcodes. */
#define	ARPOP_REQUEST	1		/* ARP request			*/
#define	ARPOP_REPLY		2		/* ARP reply			*/
#define	ARPOP_RREQUEST	3		/* RARP request			*/
#define	ARPOP_RREPLY	4		/* RARP reply			*/

/* ICMP protocol */
#define ICMP_ECHOREPLY		0	/* Echo Reply				*/
#define ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
#define ICMP_SOURCE_QUENCH	4	/* Source Quench			*/
#define ICMP_REDIRECT		5	/* Redirect (change route)	*/
#define ICMP_ECHO			8	/* Echo Request				*/
#define ICMP_TIME_EXCEEDED	11	/* Time Exceeded			*/
#define ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
#define ICMP_TIMESTAMP		13	/* Timestamp Request		*/
#define ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply			*/
#define ICMP_INFO_REQUEST	15	/* Information Request		*/
#define ICMP_INFO_REPLY		16	/* Information Reply		*/
#define ICMP_ADDRESS		17	/* Address Mask Request		*/
#define ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
#define NR_ICMP_TYPES		18


/* Codes for UNREACH. */
#define ICMP_NET_UNREACH	0	/* Network Unreachable			*/
#define ICMP_HOST_UNREACH	1	/* Host Unreachable				*/
#define ICMP_PROT_UNREACH	2	/* Protocol Unreachable			*/
#define ICMP_PORT_UNREACH	3	/* Port Unreachable				*/
#define ICMP_FRAG_NEEDED	4	/* Fragmentation Needed/DF set	*/
#define ICMP_SR_FAILED		5	/* Source Route failed			*/
#define ICMP_NET_UNKNOWN	6
#define ICMP_HOST_UNKNOWN	7
#define ICMP_HOST_ISOLATED	8
#define ICMP_NET_ANO		9
#define ICMP_HOST_ANO		10
#define ICMP_NET_UNR_TOS	11
#define ICMP_HOST_UNR_TOS	12
#define ICMP_PKT_FILTERED	13	/* Packet filtered						 */
#define ICMP_PREC_VIOLATION	14	/* Precedence violation					 */
#define ICMP_PREC_CUTOFF	15	/* Precedence cut off					 */
#define NR_ICMP_UNREACH		15	/* instead of hardcoding immediate value */

/* Codes for REDIRECT. */
#define ICMP_REDIR_NET		0	/* Redirect Net				*/
#define ICMP_REDIR_HOST		1	/* Redirect Host			*/
#define ICMP_REDIR_NETTOS	2	/* Redirect Net for TOS		*/
#define ICMP_REDIR_HOSTTOS	3	/* Redirect Host for TOS	*/

/* Codes for TIME_EXCEEDED. */
#define ICMP_EXC_TTL		0	/* TTL count exceeded			*/
#define ICMP_EXC_FRAGTIME	1	/* Fragment Reass time exceeded	*/

#define ETHADDR_BROADCAST	"\xFF\xFF\xFF\xFF\xFF\xFF"
#define ETHADDR_BROADCAST2	"\x00\x00\x00\x00\x00\x00"

#endif
