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
// $Id: tdi_fw.h,v 1.1 2009/04/28 12:53:28 tanwen Exp $

#ifndef _tdi_fw_h_
#define _tdi_fw_h_

extern PDEVICE_OBJECT g_tcpfltobj, g_udpfltobj, g_ipfltobj;

#ifndef USE_TDI_HOOKING
// original (unhooked) device objects when don't using TDI hooking
extern PDEVICE_OBJECT g_tcpoldobj, g_udpoldobj, g_ipoldobj;
#endif

extern BOOLEAN g_got_log;

NTSTATUS tdi_dispatch_complete(
	PDEVICE_OBJECT devobj, PIRP irp, int filter, PIO_COMPLETION_ROUTINE cr, PVOID context);

NTSTATUS tdi_generic_complete(
	IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);

PDEVICE_OBJECT	get_original_devobj(PDEVICE_OBJECT flt_devobj, int *proto);

// should be in ipc.h
NTSTATUS process_request(ULONG code, char *buf, ULONG *buf_len, ULONG buf_size);
NTSTATUS process_nfo_request(ULONG code, char *buf, ULONG *buf_len, ULONG buf_size);

// used for incoming or outgoing connections
void update_conn_info(PDEVICE_OBJECT devobj, PFILE_OBJECT connobj);

// traffic counters
extern unsigned __int64 g_traffic[TRAFFIC_MAX];
extern KSPIN_LOCK g_traffic_guard;

/* some Native API prototypes */

NTKERNELAPI
NTSTATUS
ObReferenceObjectByName	(
	IN PUNICODE_STRING	ObjectName,
	IN ULONG			Attributes,
	IN PACCESS_STATE	PassedAccessState OPTIONAL,
	IN ACCESS_MASK		DesiredAccess OPTIONAL,
	IN POBJECT_TYPE		ObjectType OPTIONAL,
	IN KPROCESSOR_MODE	AccessMode,
	IN OUT PVOID		ParseContext OPTIONAL,
	OUT	PVOID			*Object
);

NTSTATUS
NTAPI
ZwCreateEvent (
	OUT	PHANDLE				EventHandle,
	IN ACCESS_MASK			DesiredAccess,
	IN POBJECT_ATTRIBUTES	ObjectAttributes OPTIONAL,
	IN EVENT_TYPE			EventType,
	IN BOOLEAN				InitialState
);

NTSTATUS
NTAPI
ZwOpenThreadToken (
	IN HANDLE		ThreadHandle,
	IN ACCESS_MASK	DesiredAccess,
	IN BOOLEAN		OpenAsSelf,
	OUT	PHANDLE		TokenHandle
);

NTSTATUS
NTAPI
ZwOpenProcessToken (
	IN HANDLE       ProcessHandle,
	IN ACCESS_MASK  DesiredAccess,
	OUT PHANDLE     TokenHandle
);

typedef	enum _TOKEN_INFORMATION_CLASS {
	TokenUser =	1,
	TokenGroups,
	TokenPrivileges,
	TokenOwner,
	TokenPrimaryGroup,
	TokenDefaultDacl,
	TokenSource,
	TokenType,
	TokenImpersonationLevel,
	TokenStatistics,
	TokenRestrictedSids
} TOKEN_INFORMATION_CLASS;

NTSTATUS
NTAPI
ZwQueryInformationToken	(
	IN HANDLE					TokenHandle,
	IN TOKEN_INFORMATION_CLASS	TokenInformationClass,
	OUT	PVOID					TokenInformation,
	IN ULONG					Length,
	OUT	PULONG					ResultLength
);

typedef	enum _TOKEN_TYPE {
	TokenPrimary = 1,
	TokenImpersonation
} TOKEN_TYPE;

typedef	struct _SID_AND_ATTRIBUTES {
	PSID	Sid;
	ULONG	Attributes;
} SID_AND_ATTRIBUTES, *PSID_AND_ATTRIBUTES;

typedef	struct _TOKEN_USER {
	SID_AND_ATTRIBUTES User;
} TOKEN_USER, *PTOKEN_USER;

NTSTATUS
NTAPI
ZwWaitForSingleObject(
	IN HANDLE hObject,
	IN BOOLEAN bAlertable,
	IN PLARGE_INTEGER Timeout
);

/* for compilation under NT4 DDK */

#ifndef EVENT_ALL_ACCESS
#   define EVENT_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3)
#endif

#ifndef IoSkipCurrentIrpStackLocation
#   define IoSkipCurrentIrpStackLocation(Irp) \
    (Irp)->CurrentLocation++; \
    (Irp)->Tail.Overlay.CurrentStackLocation++;
#endif

extern POBJECT_TYPE	IoDriverObjectType;

#endif
