/* Copyright (c) 2002-2005 Vladislav Goncharov.
 *
 * Redistribution and use in source forms, with and without modification,
 * are permitted provided that this entire comment appears intact.
 *
 * Redistribution in binary form may occur without any restrictions.
 *
 * This software is provided ``AS IS'' without any warranties of any kind.
 */
 
// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil -*- (for GNU Emacs)
//
// $Id: packet.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * Stateful packet checking engine
 */

#ifdef USE_PACKET_ENGINE

#include <ntddk.h>
#include <ndis.h>
#include <tdikrnl.h>
#include "sock.h"

#if _WIN32_WINNT >= 0x0500
/* include pfhook.h */
typedef ULONG IPAddr, IPMask;
#include <pfhook.h>
#endif

#include "conn_state.h"
#include "filter.h"
#include "ipc.h"
#include "ndis_hk_ioctl.h"
#include "net.h"
#include "obj_tbl.h"
#include "packet.h"

/* prototypes */

static NTSTATUS	get_iface(void);

static BOOLEAN	filter_packet(int direction, int iface, PNDIS_PACKET packet, struct filter_nfo *self,
                              BOOLEAN packet_unchanged);

static void		process_ip(int direction, struct ip_hdr *ip_hdr, struct flt_request *log_nfo);

static BOOLEAN	process_transp(int direction, UCHAR proto,
							   struct ip_hdr *ip_hdr, char *pointer, UINT buffer_len,
                               struct flt_request *log_nfo);

static void		process_tcp(int direction, struct tcp_hdr *tcp_hdr, struct flt_request *log_nfo);
static void		process_udp(int direction, struct udp_hdr *udp_hdr, struct flt_request *log_nfo);
static void		process_icmp(int direction, struct icmp_hdr *icmp_hdr, struct flt_request *log_nfo);

static void		init_tcp_states(void);

static void		check_packet(struct flt_request *log_nfo);

#if _WIN32_WINNT >= 0x0500

/* for "ipfilterdriver" */

static PF_FORWARD_ACTION	hook_proc(
        unsigned char *PacketHeader, unsigned char *Packet, unsigned int PacketLength,
        unsigned int RecvInterfaceIndex, unsigned int SendInterfaceIndex,
        IPAddr RecvLinkNextHop, IPAddr SendLinkNextHop);

static NTSTATUS		set_hook(PacketFilterExtensionPtr hook_fn);

static BOOLEAN g_set_hook = FALSE;

#endif

/* globals */

static PDEVICE_OBJECT g_ndis_hk_devobj;
static PFILE_OBJECT g_ndis_hk_fileobj;

// interface of ndis_hk
struct ndis_hk_interface *g_ndis_hk;

static struct filter_nfo g_tdi_fw = {
	sizeof(g_tdi_fw),
	filter_packet,
	NULL,
	NULL,
	NULL,
	NULL
};

// here's a table with all possible tcp flags (except RST) and connection states

#define MAX_FLAGS	3

static struct {
	UCHAR	tcp_flags_set;
	UCHAR	tcp_flags_not_set;
} g_tcp_states[TCP_STATE_MAX][2][MAX_FLAGS];	// init this array in init_tcp_states


NTSTATUS
init_packet(void)
{
	NTSTATUS status;
	UNICODE_STRING devname;

	// connect with ndis_hk
	RtlInitUnicodeString(&devname, L"\\Device\\ndis_hk");
	
	status = IoGetDeviceObjectPointer(
		&devname,
		STANDARD_RIGHTS_ALL,
		&g_ndis_hk_fileobj,
		&g_ndis_hk_devobj);
	if (status == STATUS_SUCCESS) {

		/* using ndis_hk driver */

		status = get_iface();
		if (status != STATUS_SUCCESS) {
			KdPrint(("[tdi_fw] init_packet get_iface: 0x%x!\n", status));
			goto done;
		}

		// attach our filter!
		g_ndis_hk->attach_filter(&g_tdi_fw, TRUE, FALSE);	// to bottom of filter stack
	
	} else {
#if _WIN32_WINNT >= 0x0500
		/* try to use "ipfilterdriver" */

		status = set_hook(hook_proc);
		if (status != STATUS_SUCCESS) {
			KdPrint(("[tdi_fw] init_packet set_hook: 0x%x!\n", status));
			goto done;
		}

		g_set_hook = TRUE;
#endif
	}

	init_tcp_states();

done:
	if (status != STATUS_SUCCESS) {
		// cleanup
		free_packet();
	}

	return status;
}

void
free_packet(void)
{
	if (g_ndis_hk_fileobj != NULL) {
		// detach our filter!
		if (g_ndis_hk != NULL) {
			g_ndis_hk->attach_filter(&g_tdi_fw, FALSE, FALSE);
			g_ndis_hk = NULL;
		}

		ObDereferenceObject(g_ndis_hk_fileobj);
		g_ndis_hk_fileobj = NULL;
	}

#if _WIN32_WINNT >= 0x0500
	if (g_set_hook) {
		set_hook(NULL);
		g_set_hook = FALSE;
	}
#endif
}

NTSTATUS
get_iface(void)
{
	PIRP irp;
	IO_STATUS_BLOCK isb;

	irp = IoBuildDeviceIoControlRequest(IOCTL_CMD_GET_KM_IFACE,
		g_ndis_hk_devobj,
		NULL, 0,
		&g_ndis_hk, sizeof(g_ndis_hk),
		TRUE, NULL, &isb);
	if (irp == NULL) {
		KdPrint(("[tdi_fw] get_iface: IoBuildDeviceIoControlRequest!\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	return IoCallDriver(g_ndis_hk_devobj, irp);
}

BOOLEAN
filter_packet(int direction, int iface, PNDIS_PACKET packet, struct filter_nfo *self,
			  BOOLEAN packet_unchanged)
{
	BOOLEAN result = FALSE, log = TRUE;
	struct filter_nfo *child;
	struct flt_request log_nfo;
	PNDIS_BUFFER buffer;
	UINT packet_len, buffer_len, buffer_offset, hdr_len;
	void *pointer;
	struct ether_hdr *ether_hdr;
	struct ip_hdr *ip_hdr;

	KdPrint(("[tdi_fw] filter_packet: direction = %s; iface = %d; packet = 0x%x\n",
		direction == DIRECTION_IN ? "in" : "out", iface, packet));

	// get child in filter stack
	if (direction == DIRECTION_IN)
		child = self->lower;
	else
		child = self->upper;

	memset(&log_nfo, 0, sizeof(log_nfo));
	log_nfo.struct_size = sizeof(log_nfo);
	log_nfo.direction = direction;

	// parse packet

	NdisQueryPacket(packet, NULL, NULL, &buffer, &packet_len);

	if (packet_len < sizeof(struct ether_hdr)) {
		KdPrint(("[tdi_fw] filter_packet: too small packet for ether_hdr! (%u)\n", packet_len));
		goto done;
	}

	/* process ether_hdr */

	NdisQueryBuffer(buffer, &ether_hdr, &buffer_len);

	if (buffer_len < sizeof(struct ether_hdr)) {
		KdPrint(("[tdi_fw] filter_packet: too small buffer for ether_hdr! (%u)\n", buffer_len));
		goto done;
	}
	buffer_offset = 0;

#define PRINT_ETH_ADDR(addr) \
	(addr)[0], (addr)[1], (addr)[2], (addr)[3], (addr)[4], (addr)[5]

	KdPrint(("[tdi_fw] filter_packet: eth %02x-%02x-%02x-%02x-%02x-%02x -> %02x-%02x-%02x-%02x-%02x-%02x (0x%x)\n",
		PRINT_ETH_ADDR(ether_hdr->ether_shost),
		PRINT_ETH_ADDR(ether_hdr->ether_dhost),
		ether_hdr->ether_type));

	// UGLY way to determine IP broadcasts
	if (memcmp(ether_hdr->ether_dhost, "\xff\xff\xff\xff\xff\xff", 6) == 0)
		log_nfo.packet.is_broadcast = 1;

	// go to the next header
	if (buffer_len > sizeof(struct ether_hdr)) {

		pointer = (char *)ether_hdr + sizeof(struct ether_hdr);
		buffer_offset += sizeof(struct ether_hdr);

		buffer_len -= sizeof(struct ether_hdr);

	} else {
		// use next buffer in chain

		do {
			NdisGetNextBuffer(buffer, &buffer);
			NdisQueryBuffer(buffer, &pointer, &buffer_len);
		} while (buffer_len == 0);		// sometimes there're buffers with zero size in chain
		
		buffer_offset = 0;
	}

	if (ntohs(ether_hdr->ether_type) == ETHERTYPE_IP) {

		/* process ip_hdr */

		if (buffer_len < sizeof(struct ip_hdr)) {
			KdPrint(("[tdi_fw] filter_packet: too small buffer for ip_hdr! (%u)\n",
				buffer_len));
			goto done;
		}

		ip_hdr = (struct ip_hdr *)pointer;
		hdr_len = ip_hdr->ip_hl * 4;

		if (buffer_len < hdr_len) {
			KdPrint(("[tdi_fw] filter_packet: too small buffer for ip_hdr! (%u vs. %u)\n",
				buffer_len, hdr_len));
			goto done;
		}

		// check we've got the first fragment (don't work with another!)
		if ((ntohs(ip_hdr->ip_off) & IP_OFFMASK) != 0 && (ip_hdr->ip_off & IP_DF) == 0) {

			KdPrint(("[tdi_fw] filter_packet: got not first fragment\n"));

			result = TRUE;
			log = FALSE;
			goto done;
		}

		process_ip(direction, ip_hdr, &log_nfo);

		// go to the next header
		if (buffer_len > hdr_len) {

			pointer = (char *)ip_hdr + hdr_len;
			buffer_offset += hdr_len;
					
			buffer_len -= hdr_len;
			
		} else {
			// use next buffer in chain

			do {
				NdisGetNextBuffer(buffer, &buffer);
				NdisQueryBuffer(buffer, &pointer, &buffer_len);
			} while (buffer_len == 0);		// sometimes there're buffers with zero size in chain

			buffer_offset = 0;
		}

		result = process_transp(direction, ip_hdr->ip_p, ip_hdr, pointer, buffer_len, &log_nfo);
		if (!result)
			goto done;

		// using log_nfo->pid as signal to log it
		log = (log_nfo.pid == 0);
	} else
		log = FALSE;		// don't log not IP packets

	result = TRUE;		// packet headers looks like valid

done:
	if (log) {
		log_nfo.result = result ? FILTER_PACKET_LOG : FILTER_PACKET_BAD;
		log_nfo.pid = (ULONG)-1;
		log_request(&log_nfo);
	}

	if (result)
		return child->process_packet(direction, iface, packet, child, packet_unchanged);
	else
		return FALSE;
}

/* IP header */
void
process_ip(int direction, struct ip_hdr *ip_hdr, struct flt_request *log_nfo)
{
#define PRINT_IP_ADDR(addr) \
	((UCHAR *)&(addr))[0], ((UCHAR *)&(addr))[1], ((UCHAR *)&(addr))[2], ((UCHAR *)&(addr))[3]

	KdPrint(("[tdi_fw] process_ip: ip %d.%d.%d.%d -> %d.%d.%d.%d (proto = %d, ipid = 0x%x)\n",
		PRINT_IP_ADDR(ip_hdr->ip_src),
		PRINT_IP_ADDR(ip_hdr->ip_dst),
		ip_hdr->ip_p,
		ip_hdr->ip_id));

	log_nfo->proto = ip_hdr->ip_p;
	
	if (direction == DIRECTION_IN)
		log_nfo->log_bytes_in = ntohs(ip_hdr->ip_len);
	else
		log_nfo->log_bytes_out = ntohs(ip_hdr->ip_len);
	
	log_nfo->addr.len = sizeof(struct sockaddr_in);

	((struct sockaddr_in *)(&log_nfo->addr.from))->sin_family = AF_INET;
	((struct sockaddr_in *)(&log_nfo->addr.from))->sin_addr.s_addr = ip_hdr->ip_src;

	((struct sockaddr_in *)(&log_nfo->addr.to))->sin_family = AF_INET;
	((struct sockaddr_in *)(&log_nfo->addr.to))->sin_addr.s_addr = ip_hdr->ip_dst;
}

/* process TCP, UDP or ICMP header */
BOOLEAN
process_transp(int direction, UCHAR proto,
               struct ip_hdr *ip_hdr, char *pointer, UINT buffer_len,
			   struct flt_request *log_nfo)
{
	struct tcp_hdr *tcp_hdr;
	struct udp_hdr *udp_hdr;
	struct icmp_hdr *icmp_hdr;

	switch (proto) {
	case IPPROTO_TCP:

		/* process tcp_hdr */

		if (buffer_len < sizeof(struct tcp_hdr)) {
			KdPrint(("[tdi_fw] process_transp: too small buffer for tcp_hdr! (%u)\n",
				buffer_len));
			return FALSE;
		}

		tcp_hdr = (struct tcp_hdr *)pointer;

		process_tcp(direction, tcp_hdr, log_nfo);
		
		break;

	case IPPROTO_UDP:

		/* process udp_hdr */

		if (buffer_len < sizeof(struct udp_hdr)) {
			KdPrint(("[tdi_fw] process_transp: too small buffer for udp_hdr! (%u)\n",
				buffer_len));
			return FALSE;
		}

		udp_hdr = (struct udp_hdr *)pointer;

		process_udp(direction, udp_hdr, log_nfo);

		break;

	case IPPROTO_ICMP:
		
		/* process icmp_hdr */

		if (buffer_len < sizeof(struct icmp_hdr)) {
			KdPrint(("[tdi_fw] process_transp: too small buffer for icmp_hdr! (%u)\n",
				buffer_len));
			return FALSE;
		}

		icmp_hdr = (struct icmp_hdr *)pointer;

		process_icmp(direction, icmp_hdr, log_nfo);

		break;

	default:
		KdPrint(("[tdi_fw] process_transp: ipproto = %d\n", proto));
	}

	return TRUE;
}

/* process TCP header and check it against our information about connection objects */
void
process_tcp(int direction, struct tcp_hdr *tcp_hdr, struct flt_request *log_nfo)
{
#if DBG
	char flags[7];

	flags[0] = (tcp_hdr->th_flags & TH_FIN) ? 'F' : ' ';
	flags[1] = (tcp_hdr->th_flags & TH_SYN) ? 'S' : ' ';
	flags[2] = (tcp_hdr->th_flags & TH_RST) ? 'R' : ' ';
	flags[3] = (tcp_hdr->th_flags & TH_PUSH) ? 'P' : ' ';
	flags[4] = (tcp_hdr->th_flags & TH_ACK) ? 'A' : ' ';
	flags[5] = (tcp_hdr->th_flags & TH_URG) ? 'U' : ' ';
	flags[6] = '\0';

	KdPrint(("[tdi_fw] tcp %d -> %d (%s)\n",
		ntohs(tcp_hdr->th_sport),
		ntohs(tcp_hdr->th_dport),
		flags));
#endif

	log_nfo->packet.tcp_flags = tcp_hdr->th_flags;
	((struct sockaddr_in *)(&log_nfo->addr.from))->sin_port = tcp_hdr->th_sport;
	((struct sockaddr_in *)(&log_nfo->addr.to))->sin_port = tcp_hdr->th_dport;

	check_packet(log_nfo);
}

/* process UDP header and check it against our informatation about address objects */
void
process_udp(int direction, struct udp_hdr *udp_hdr, struct flt_request *log_nfo)
{
	KdPrint(("[tdi_fw] udp %d -> %d\n", ntohs(udp_hdr->uh_sport), ntohs(udp_hdr->uh_dport)));

	((struct sockaddr_in *)(&log_nfo->addr.from))->sin_port = udp_hdr->uh_sport;
	((struct sockaddr_in *)(&log_nfo->addr.to))->sin_port = udp_hdr->uh_dport;

	check_packet(log_nfo);
}

/* process ICMP header */
void
process_icmp(int direction, struct icmp_hdr *icmp_hdr, struct flt_request *log_nfo)
{
	KdPrint(("[tdi_fw] icmp (%d.%d)\n", icmp_hdr->icmp_type, icmp_hdr->icmp_code));

	log_nfo->packet.icmp_type = icmp_hdr->icmp_type;
	log_nfo->packet.icmp_code = icmp_hdr->icmp_code;

	check_packet(log_nfo);
}

void
init_tcp_states(void)
{
	// init g_tcp_states sparse array (don't define RST flags)

#define DEFINE_STATE_IN(state, index, flags_set, flags_not_set)  \
	g_tcp_states[(state)][DIRECTION_IN][(index)].tcp_flags_set = (flags_set);  \
	g_tcp_states[(state)][DIRECTION_IN][(index)].tcp_flags_not_set = (flags_not_set)

#define DEFINE_STATE_OUT(state, index, flags_set, flags_not_set)  \
	g_tcp_states[(state)][DIRECTION_OUT][(index)].tcp_flags_set = (flags_set);  \
	g_tcp_states[(state)][DIRECTION_OUT][(index)].tcp_flags_not_set = (flags_not_set)

	// -connection establishment

	// --active (outgoing) connection

	// ---SYN retransmitting + 3rd positive answer
	DEFINE_STATE_OUT(TCP_STATE_SYN_SENT, 0, TH_SYN, TH_ACK);
	DEFINE_STATE_OUT(TCP_STATE_SYN_SENT, 1, TH_ACK, TH_SYN | TH_FIN);

	// ---positive answer
	DEFINE_STATE_IN(TCP_STATE_SYN_SENT, 0, TH_SYN | TH_ACK, 0);

	// --passive (incoming) connection

	// ---positive answer
	DEFINE_STATE_OUT(TCP_STATE_SYN_RCVD, 0, TH_SYN | TH_ACK, 0);

	// ---last connect step: ACK or FIN(ACK)
	DEFINE_STATE_IN(TCP_STATE_SYN_RCVD, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_IN(TCP_STATE_SYN_RCVD, 1, TH_FIN, TH_SYN);

	// -established in/out: ACK, FIN
	DEFINE_STATE_IN(TCP_STATE_ESTABLISHED_IN, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_IN(TCP_STATE_ESTABLISHED_IN, 1, TH_FIN, TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_ESTABLISHED_IN, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_ESTABLISHED_IN, 1, TH_FIN, TH_SYN);

	DEFINE_STATE_IN(TCP_STATE_ESTABLISHED_OUT, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_IN(TCP_STATE_ESTABLISHED_OUT, 1, TH_FIN, TH_SYN);

	// --peer didn't receive 3rd ACK packet
	DEFINE_STATE_IN(TCP_STATE_ESTABLISHED_OUT, 2, TH_ACK | TH_SYN, TH_FIN);
	
	DEFINE_STATE_OUT(TCP_STATE_ESTABLISHED_OUT, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_ESTABLISHED_OUT, 1, TH_FIN, TH_SYN);

	// -connection closing
	DEFINE_STATE_IN(TCP_STATE_FIN_WAIT1, 0, TH_FIN, TH_SYN);
	DEFINE_STATE_IN(TCP_STATE_FIN_WAIT1, 1, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_FIN_WAIT1, 0, TH_FIN, TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_FIN_WAIT1, 1, TH_ACK, TH_FIN | TH_SYN);

	DEFINE_STATE_IN(TCP_STATE_FIN_WAIT2, 0, TH_FIN, TH_SYN);
	DEFINE_STATE_IN(TCP_STATE_FIN_WAIT2, 1, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_FIN_WAIT2, 0, TH_ACK, TH_FIN | TH_SYN);

	DEFINE_STATE_OUT(TCP_STATE_CLOSE_WAIT, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_LAST_ACK, 0, TH_FIN, TH_SYN);
	DEFINE_STATE_OUT(TCP_STATE_LAST_ACK, 1, TH_ACK, TH_FIN | TH_SYN); // answer on FIN retransmitting
	DEFINE_STATE_IN(TCP_STATE_LAST_ACK, 0, TH_ACK, TH_FIN | TH_SYN);
	DEFINE_STATE_IN(TCP_STATE_LAST_ACK, 1, TH_FIN, TH_SYN);		// FIN retransmitting

	// -in "closed" state all incoming packets are allowed (except single SYN)
	DEFINE_STATE_IN(TCP_STATE_CLOSED, 0, TH_ACK, 0);
	DEFINE_STATE_IN(TCP_STATE_CLOSED, 1, TH_FIN, 0);

	// -and RST out too
	DEFINE_STATE_OUT(TCP_STATE_CLOSED, 0, TH_RST, 0);

	// ??? I saw this packet sometimes (maybe there's a bug in my code?)
	DEFINE_STATE_OUT(TCP_STATE_CLOSED, 1, TH_ACK, TH_FIN | TH_SYN);
}

/*
 * check packet against state in obj_tbl
 *
 * NOTE: this function is experimental ONLY!!!
 * To make work it better I should specify interface information: IP address and network mask
 */
void
check_packet(struct flt_request *log_nfo)
{
	BOOLEAN correct = FALSE;
	struct sockaddr_in *addr_from = (struct sockaddr_in *)&log_nfo->addr.from,
		*addr_to = (struct sockaddr_in *)&log_nfo->addr.to;

	// check it

	if (log_nfo->proto == IPPROTO_TCP) {

		/* TCP protocol */

		if ((log_nfo->packet.tcp_flags & TH_SYN) &&
			!(log_nfo->packet.tcp_flags & TH_ACK) &&
			log_nfo->direction == DIRECTION_IN) {

			// active incoming connection (SYN+ ACK-): check against listening address objects
			correct = is_listen(addr_to->sin_addr.s_addr, addr_to->sin_port, IPPROTO_TCP) ||
				is_listen(0, addr_to->sin_port, IPPROTO_TCP);

		} else {
			int state;

			// just check address & port against connection table
			if (log_nfo->direction == DIRECTION_OUT) {
				state = get_tcp_conn_state(addr_from->sin_addr.s_addr, addr_from->sin_port,
							addr_to->sin_addr.s_addr, addr_to->sin_port);
				if (state == TCP_STATE_NONE)	// source addr can be 0
					state = get_tcp_conn_state(0, addr_from->sin_port, addr_to->sin_addr.s_addr, addr_to->sin_port);
			} else {				// DIRECTION_IN
				state = get_tcp_conn_state(addr_to->sin_addr.s_addr, addr_to->sin_port,
							addr_from->sin_addr.s_addr, addr_from->sin_port);
				if (state == TCP_STATE_NONE)	// dest addr can be 0
					state = get_tcp_conn_state(0, addr_to->sin_port, addr_from->sin_addr.s_addr, addr_from->sin_port);
			}
			
			if (log_nfo->packet.tcp_flags & TH_RST)
				correct = (state != TCP_STATE_NONE);
			else {

				// check connection state & header flags against g_tcp_states
				int i;

				for (i = 0; i < MAX_FLAGS; i++) {
					UCHAR tcp_flags_set = g_tcp_states[state][log_nfo->direction][i].tcp_flags_set;
					if (tcp_flags_set == 0)
						break;

					if ((tcp_flags_set & log_nfo->packet.tcp_flags) == tcp_flags_set &&
						!(g_tcp_states[state][log_nfo->direction][i].tcp_flags_not_set & log_nfo->packet.tcp_flags)) {
						correct = TRUE;
						break;
					}
				}
			}

			if (!correct && (log_nfo->packet.tcp_flags & TH_RST) && log_nfo->direction == DIRECTION_OUT) {
				// don't log RSTs on opened ports
				correct = is_listen(addr_from->sin_addr.s_addr, addr_from->sin_port, IPPROTO_TCP) ||
					is_listen(0, addr_from->sin_port, IPPROTO_TCP);
			}

			if (!correct)
				log_nfo->packet.tcp_state = state;
		}
			
	} else if (log_nfo->proto == IPPROTO_UDP) {

		// UDP: check it against listening address objects
		if (log_nfo->direction == DIRECTION_IN) {
			// incoming datagram: check "to" addr & port
			correct = is_listen(addr_to->sin_addr.s_addr, addr_to->sin_port, log_nfo->proto) ||
				is_listen(0, addr_to->sin_port, log_nfo->proto) ||
				(log_nfo->packet.is_broadcast &&
					(addr_to->sin_addr.s_addr == (ULONG)-1 ||
					is_bcast_listen(addr_to->sin_addr.s_addr, addr_to->sin_port, log_nfo->proto)));

		} else {				// DIRECTION_OUT
			// outgoing datagram: check "from" addr & port
			correct = is_listen(addr_from->sin_addr.s_addr, addr_from->sin_port, log_nfo->proto) ||
				is_listen(0, addr_from->sin_port, log_nfo->proto) ||
				(log_nfo->packet.is_broadcast && is_bcast_listen(addr_from->sin_addr.s_addr, addr_from->sin_port, log_nfo->proto));
		}
	}

	if (correct)
		log_nfo->pid = (ULONG)-1;	// don't log this packet
}

#if _WIN32_WINNT >= 0x0500
/* set or remove IP firewall hook */
NTSTATUS
set_hook(PacketFilterExtensionPtr hook_fn)
{
    UNICODE_STRING ipfilter_name;
    NTSTATUS status;
    PFILE_OBJECT fileobj = NULL;
    PDEVICE_OBJECT devobj;
    PF_SET_EXTENSION_HOOK_INFO hook_nfo;
    PIRP irp = NULL;
    IO_STATUS_BLOCK isb;
    
    RtlInitUnicodeString(&ipfilter_name, DD_IPFLTRDRVR_DEVICE_NAME);
    
    status = IoGetDeviceObjectPointer(
            &ipfilter_name,
            STANDARD_RIGHTS_ALL,
            &fileobj,
            &devobj);
    if (status != STATUS_SUCCESS)
        goto done;
    
    hook_nfo.ExtensionPointer = hook_fn;
    
    irp = IoBuildDeviceIoControlRequest(IOCTL_PF_SET_EXTENSION_POINTER,
            devobj, &hook_nfo, sizeof(hook_nfo),
            NULL, 0, FALSE, NULL, &isb);
    if (irp == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto done;
    }
    
    status = IoCallDriver(devobj, irp);
    irp = NULL;
    
done:
    /* cleanup */
    if (fileobj != NULL)
        ObDereferenceObject(fileobj);
    if (irp != NULL)
        IoFreeIrp(irp);
    
    return status;
}

/* firewall callback hook proc */
PF_FORWARD_ACTION
hook_proc(
        unsigned char *PacketHeader,
        unsigned char *Packet,
        unsigned int PacketLength,
        unsigned int RecvInterfaceIndex,
        unsigned int SendInterfaceIndex,
        IPAddr RecvLinkNextHop,
        IPAddr SendLinkNextHop)
{
	BOOLEAN log = FALSE, result = FALSE;
	int direction, iface;
	struct ip_hdr *ip_hdr;
	struct flt_request log_nfo;

	direction = (RecvInterfaceIndex != INVALID_PF_IF_INDEX) ? DIRECTION_IN : DIRECTION_OUT;
	iface = (direction == DIRECTION_IN) ? RecvInterfaceIndex : SendInterfaceIndex;

	KdPrint(("[tdi_fw] hook_proc: direction = %s; iface = 0x%x; len = %d\n",
		direction == DIRECTION_IN ? "in" : "out", iface, PacketLength));

	memset(&log_nfo, 0, sizeof(log_nfo));
	log_nfo.struct_size = sizeof(log_nfo);
	log_nfo.direction = direction;

	// ...have not access to ethernet handler

	ip_hdr = (struct ip_hdr *)PacketHeader;

	if (ip_hdr->ip_dst == 0x0100007f) {		// don't work with loopback interface
		result = TRUE;
		goto done;
	}

	// check we've got the first fragment (don't work with another!)
	if ((ntohs(ip_hdr->ip_off) & IP_OFFMASK) != 0 && (ip_hdr->ip_off & IP_DF) == 0) {

		KdPrint(("[tdi_fw] hook_proc: got not first fragment\n"));

		result = TRUE;
		log = FALSE;
		goto done;
	}

	process_ip(direction, ip_hdr, &log_nfo);

	// no way to determine broadcasts without knowing interface IP/mask :-(
	log_nfo.packet.is_broadcast = 1;	// for datagram processing code

	result = process_transp(direction, ip_hdr->ip_p, ip_hdr, Packet, PacketLength, &log_nfo);
	if (!result)
		goto done;

	// using log_nfo->pid as signal to log it
	log = (log_nfo.pid == 0);

	result = TRUE;		// packet headers looks like valid

done:
	if (log) {
		log_nfo.result = result ? FILTER_PACKET_LOG : FILTER_PACKET_BAD;
		log_nfo.pid = (ULONG)-1;
		log_request(&log_nfo);
	}

	return result ? PF_FORWARD : PF_DROP;
}
#endif /* _WIN32_WINNT >= 0x0500 */

#endif /* USE_PACKET_ENGINE */
