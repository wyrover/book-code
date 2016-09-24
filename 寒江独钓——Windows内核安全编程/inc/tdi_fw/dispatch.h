// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil -*- (for GNU Emacs)
//
// $Id: dispatch.h,v 1.1 2009/04/28 12:53:27 tanwen Exp $

#ifndef _dispatch_h_
#define _dispatch_h_

// information about completion routine
struct completion {
	PIO_COMPLETION_ROUTINE	routine;
	PVOID					context;
};

/* 
 * TDI ioctl dispatcher function
 * returns FILTER_xxx
 */
typedef int tdi_ioctl_fn_t(PIRP irp, PIO_STACK_LOCATION irps, struct completion *completion);

// IRP_MJ_CREATE, IRP_MJ_CLEANUP dispatch routines
extern tdi_ioctl_fn_t tdi_create, tdi_cleanup;

// IRP_MJ_INTERNAL_DEVICE_CONTROL ioctl dispatch routines
extern tdi_ioctl_fn_t
	tdi_associate_address,
	tdi_connect,
	tdi_disassociate_address,
	tdi_set_event_handler,
	tdi_send_datagram,
	tdi_receive_datagram,
	tdi_disconnect,
	tdi_send,
	tdi_receive,
	tdi_deny_stub;

// helper struct for calling of TDI ioctls
struct tdi_ioctl {
	UCHAR			MinorFunction;
	tdi_ioctl_fn_t	*fn;

#if DBG
	// for debugging
	const char		*desc;
#endif
};

extern struct tdi_ioctl g_tdi_ioctls[];

#endif
