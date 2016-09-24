// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil -*- (for GNU Emacs)
//
// $Id: events.h,v 1.1 2009/04/28 12:53:27 tanwen Exp $

#ifndef _events_h_
#define _events_h_

#include "obj_tbl.h"

NTSTATUS tdi_event_connect(
    IN PVOID TdiEventContext,
    IN LONG RemoteAddressLength,
    IN PVOID RemoteAddress,
    IN LONG UserDataLength,
    IN PVOID UserData,
    IN LONG OptionsLength,
    IN PVOID Options,
    OUT CONNECTION_CONTEXT *ConnectionContext,
    OUT PIRP *AcceptIrp);

NTSTATUS tdi_event_disconnect(
    IN PVOID TdiEventContext,
    IN CONNECTION_CONTEXT ConnectionContext,
    IN LONG DisconnectDataLength,
    IN PVOID DisconnectData,
    IN LONG DisconnectInformationLength,
    IN PVOID DisconnectInformation,
    IN ULONG DisconnectFlags);

NTSTATUS tdi_event_receive_datagram(
    IN PVOID TdiEventContext,
    IN LONG SourceAddressLength,
    IN PVOID SourceAddress,
    IN LONG OptionsLength,
    IN PVOID Options,
    IN ULONG ReceiveDatagramFlags,
    IN ULONG BytesIndicated,
    IN ULONG BytesAvailable,
    OUT ULONG *BytesTaken,
    IN PVOID Tsdu,
    OUT PIRP *IoRequestPacket);

NTSTATUS tdi_event_receive(
    IN PVOID TdiEventContext,
    IN CONNECTION_CONTEXT ConnectionContext,
    IN ULONG ReceiveFlags,
    IN ULONG BytesIndicated,
    IN ULONG BytesAvailable,
    OUT ULONG *BytesTaken,
    IN PVOID Tsdu,
    OUT PIRP *IoRequestPacket);

NTSTATUS tdi_event_chained_receive(
    IN PVOID TdiEventContext,
    IN CONNECTION_CONTEXT ConnectionContext,
    IN ULONG ReceiveFlags,
    IN ULONG ReceiveLength,
    IN ULONG StartingOffset,
    IN PMDL  Tsdu,
    IN PVOID TsduDescriptor);

// helper struct for calling one of the handlers
struct tdi_event_handler {
	LONG	event;
	PVOID	handler;
};

extern struct tdi_event_handler g_tdi_event_handlers[];

#endif
