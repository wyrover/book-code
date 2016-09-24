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
// $Id: ipc.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _ipc_h_
#define _ipc_h_

/* ioctls */

#define FILE_DEVICE_TDI_FW		0x8e86

#define IOCTL_CMD_GETREQUEST	CTL_CODE(FILE_DEVICE_TDI_FW, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_CLEARCHAIN	CTL_CODE(FILE_DEVICE_TDI_FW, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_APPENDRULE	CTL_CODE(FILE_DEVICE_TDI_FW, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_SETCHAINPNAME	CTL_CODE(FILE_DEVICE_TDI_FW, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_SETPNAME		CTL_CODE(FILE_DEVICE_TDI_FW, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_ACTIVATECHAIN	CTL_CODE(FILE_DEVICE_TDI_FW, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_SET_SIDS		CTL_CODE(FILE_DEVICE_TDI_FW, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FILE_DEVICE_TDI_FW_NFO	0x8e87

#define IOCTL_CMD_ENUM_LISTEN	CTL_CODE(FILE_DEVICE_TDI_FW_NFO, 0x901, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_ENUM_TCP_CONN	CTL_CODE(FILE_DEVICE_TDI_FW_NFO, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_GET_COUNTERS	CTL_CODE(FILE_DEVICE_TDI_FW_NFO, 0x903, METHOD_BUFFERED, FILE_ANY_ACCESS)

/*
 * direction type for filter
 * for quick filter:
 *  if proto == IPPROTO_TCP (DIRECTION_IN - accept connections; DIRECTION_OUT - connect)
 *  if proto == IPPROTO_UDP (DIRECTION_IN - receive datagram; DIRECTION_OUT - send datagram)
 */
#define DIRECTION_IN	0
#define DIRECTION_OUT	1
#define DIRECTION_ANY	-1

/* filter result */
enum {
	FILTER_ALLOW = 1,
	FILTER_DENY,
	FILTER_PACKET_LOG,
	FILTER_PACKET_BAD,
	FILTER_DISCONNECT
};

/* types of request */
enum {
	TYPE_CONNECT = 1,
	TYPE_DATAGRAM,
	TYPE_RESOLVE_PID,
	TYPE_CONNECT_ERROR,
	TYPE_LISTEN,
	TYPE_NOT_LISTEN,
	TYPE_CONNECT_CANCELED,
	TYPE_CONNECT_RESET,
	TYPE_CONNECT_TIMEOUT,
	TYPE_CONNECT_UNREACH,
	TYPE_PROCESS_CREATE,		// add by tan wen
	TYPE_PROCESS_TERMINATE		// add by tan wen
};

#pragma pack(1)

#define RULE_ID_SIZE		32

/*
 * request for filter
 */
struct flt_request {
	int		struct_size;	/* should be sizeof(flt_request) */

	int		type;			/* see TYPE_xxx */
	ULONG	status;			/* for TYPE_CONNECT_xxx */

	int		result;			/* see FILTER_xxx */
	int		direction;		/* see DIRECTION_xxx */
	int		proto;			/* see IPPROTO_xxx */

	ULONG	pid;
	ULONG	sid_a_size;

	/* addr */

	struct {
		struct	sockaddr from;
		struct	sockaddr to;
		int		len;
	} addr;

	/* info from packet filter (valid for FILTER_PACKET_LOG) */
	struct {
		int		is_broadcast;	// 0 or 1 (for now unused)
		UCHAR	tcp_flags;
		UCHAR	icmp_type;
		UCHAR	icmp_code;
		int		tcp_state;		// see TCP_STATE_xxx
	} packet;
	
	/* info for logging */

	ULONG	log_skipped;
	ULONG	log_bytes_in;
	ULONG	log_bytes_out;
	char	log_rule_id[RULE_ID_SIZE];

	/* for internal use (like private:) */

	char	*pname;
	struct	_SID_AND_ATTRIBUTES *sid_a;
};

// I think 128 is a good number :-) (better than 256 :))
#define MAX_CHAINS_COUNT	128

// how many users can be assigned per rule? (MUST: MAX_SIDS_COUNT % 8 == 0 !!!)
#define MAX_SIDS_COUNT		128

/*
 * IP rule for quick filter (addr & port are in network order)
 */
struct flt_rule {
	union {
		struct	flt_rule *next;		// for internal use
		int		chain;				// useful for IOCTL_CMD_APPENDRULE
	};
	int		result;
	int		proto;
	int		direction;
	ULONG	addr_from;
	ULONG	mask_from;
	USHORT	port_from;
	USHORT	port2_from;		/* if nonzero use port range from port_from */
	ULONG	addr_to;
	ULONG	mask_to;
	USHORT	port_to;
	USHORT	port2_to;		/* if nonzero use port range from port_to */
	int		log;			/* see RULE_LOG_xxx */

	UCHAR	sid_mask[MAX_SIDS_COUNT / 8];	/* SIDs bitmask */

	char	rule_id[RULE_ID_SIZE];
};

#define RULE_LOG_NOLOG			0
#define RULE_LOG_LOG			1
#define RULE_LOG_COUNT			2

#define IPPROTO_ANY		-1

/*
 * Entry for listen info
 */
struct listen_nfo {
	int				ipproto;
	ULONG			addr;
	USHORT			port;
	ULONG			pid;
};

/*
 * TCP states
 */
enum {
	TCP_STATE_NONE,
	TCP_STATE_SYN_SENT,
	TCP_STATE_SYN_RCVD,
	TCP_STATE_ESTABLISHED_IN,
	TCP_STATE_ESTABLISHED_OUT,
	TCP_STATE_FIN_WAIT1,
	TCP_STATE_FIN_WAIT2,
	TCP_STATE_TIME_WAIT,
	TCP_STATE_CLOSE_WAIT,
	TCP_STATE_LAST_ACK,
	TCP_STATE_CLOSED,
	
	TCP_STATE_MAX
};

/*
 * Entry for connection info
 */
struct tcp_conn_nfo {
	int				state;
	ULONG			laddr;
	USHORT			lport;
	ULONG			raddr;
	USHORT			rport;
	ULONG			pid;
	ULONG			bytes_in;
	ULONG			bytes_out;
};

/*
 * traffic counters for IOCTL_CMD_GET_COUNTERS
 */
enum {
	TRAFFIC_TOTAL_IN,
	TRAFFIC_TOTAL_OUT,
	TRAFFIC_COUNTED_IN,
	TRAFFIC_COUNTED_OUT,
	
	TRAFFIC_MAX
};


#pragma pack()

#endif
