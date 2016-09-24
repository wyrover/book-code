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
// $Id: obj_tbl.c,v 1.1 2009/04/28 12:53:28 tanwen Exp $

/*
 * Working with connection objects, address objects and links between them
 */

#include <ntddk.h>
#include <tdikrnl.h>
#include "sock.h"

#include "conn_state.h"
#include "memtrack.h"
#include "obj_tbl.h"
#include "sids.h"
#include "tdi_fw.h"

// for searching objects information by file object

#define HASH_SIZE	0x1000
#define CALC_HASH(fileobj)  (((ULONG)(fileobj) >> 5) % HASH_SIZE)

static struct ot_entry **g_ot_hash;
KSPIN_LOCK g_ot_hash_guard;

// for searching connection objects by address object & connection context

struct ctx_entry {
	struct ctx_entry *next;
	PFILE_OBJECT addrobj;
	CONNECTION_CONTEXT conn_ctx;
	PFILE_OBJECT connobj;
};

static struct ctx_entry **g_cte_hash;
KSPIN_LOCK g_cte_hash_guard;

//----------------------------------------------------------------------------

NTSTATUS
ot_init(void)
{
	g_ot_hash = (struct ot_entry **)malloc_np(sizeof(*g_ot_hash) * HASH_SIZE);
	if (g_ot_hash == NULL) {
		KdPrint(("[tdi_fw] ot_init: malloc_np\n"));
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	memset(g_ot_hash, 0, sizeof(*g_ot_hash) * HASH_SIZE);

	KeInitializeSpinLock(&g_ot_hash_guard);

	g_cte_hash = (struct ctx_entry **)malloc_np(sizeof(*g_cte_hash) * HASH_SIZE);
	if (g_cte_hash == NULL) {
		KdPrint(("[tdi_fw] ot_init: malloc_np\n"));
		free(g_ot_hash);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	memset(g_cte_hash, 0, sizeof(*g_cte_hash) * HASH_SIZE);

	KeInitializeSpinLock(&g_cte_hash_guard);

	return STATUS_SUCCESS;
}

void
ot_free(void)
{
	KIRQL irql;
	int i;
	struct ot_entry *ote;

	if (g_ot_hash == NULL)
		return;					// have nothing to do

	// do cleanup for address & connection objects
	for (i = 0; i < HASH_SIZE; i++) {

		for (;;) {			// do it again and again
			PFILE_OBJECT connobj = NULL, event_addrobj;
			PVOID event_handler = NULL, event_context;
			int event_type;
			PDEVICE_OBJECT devobj;

			KeAcquireSpinLock(&g_ot_hash_guard, &irql);

			for (ote = g_ot_hash[i]; ote != NULL; ote = ote->next) {

				if (ote->fileobj == NULL)
					continue;				// skip processed items

#ifdef _USE_TDI_HOOKING
				devobj = ote->devobj;
#else
				devobj = get_original_devobj(ote->devobj, NULL);
#endif

				if (ote->type == FILEOBJ_ADDROBJ) {
					// find at least one event handler & remove it @ PASSIVE level
					int j;
					
					event_addrobj = ote->fileobj;
					
					for (j = 0; j < MAX_EVENT; j++)
						if (ote->ctx[j].old_handler != NULL) {
	
							event_type = j;
							event_handler = ote->ctx[j].old_handler;
							event_context = ote->ctx[j].old_context;

							KdPrint(("[tdi_fw] ot_free: got event handler to restore (addrobj: 0x%x; type: %d; handler: 0x%x; context: 0x%x\n",
								event_addrobj,
								event_type,
								event_handler,
								event_context));

							ote->ctx[j].old_handler = NULL;

							break;
						}

					if (event_handler != NULL)
						break;

					KdPrint(("[tdi_fw] ot_free: no event handlers for addrobj: 0x%x\n",
						ote->fileobj));

					// remove this addrobj from "LISTEN" state
					if (ote->listen_entry != NULL) {
						del_listen_obj(ote->listen_entry, FALSE);
						ote->listen_entry = NULL;
					}

					// no event handlers
					ote->fileobj = NULL;

				} else if (ote->type == FILEOBJ_CONNOBJ) {
					// check connobj is connected (remote addr is not 0)
					TA_ADDRESS *remote_addr = (TA_ADDRESS *)(ote->remote_addr);

					if (((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr == 0) {
						KdPrint(("[tdi_fw] ot_free: connobj 0x%x is not connected\n", connobj));

					} else {
						// disconnect this connection using TDI_DISCONNECT @ PASSIVE level
						connobj = ote->fileobj;
						
						KdPrint(("[tdi_fw] ot_free: got connobj 0x%x (%x:%x) to disconnect\n",
							connobj,
							((TDI_ADDRESS_IP *)(remote_addr->Address))->in_addr,
							((TDI_ADDRESS_IP *)(remote_addr->Address))->sin_port));
					}

					// remove this connobj from "CONNECTED" state
					if (ote->conn_entry != NULL) {
						del_tcp_conn_obj(ote->conn_entry, FALSE);
						ote->conn_entry = NULL;

						// TODO: check if state TCP_STATE_TIME_WAIT don't delete it
					}

					// skip this object next time
					ote->fileobj = NULL;
				}
			}

			KeReleaseSpinLock(&g_ot_hash_guard, irql);

			// we're at PASSIVE level

			if (event_handler != NULL) {
				// set old event handler
				PIRP query_irp;
				NTSTATUS status;

				KdPrint(("[tdi_fw] ot_free: got event handler to restore (addrobj: 0x%x; type: %d; handler: 0x%x; context: 0x%x\n",
					event_addrobj,
					event_type,
					event_handler,
					event_context));

				query_irp = TdiBuildInternalDeviceControlIrp(TDI_SET_EVENT_HANDLER,
					devobj, event_addrobj, NULL, NULL);
				if (query_irp == NULL) {
					KdPrint(("[tdi_fw] ot_free: TdiBuildInternalDeviceControlIrp\n"));
					continue;
				}

				TdiBuildSetEventHandler(query_irp, devobj, event_addrobj, NULL, NULL,
					event_type,	event_handler, event_context);

				status = IoCallDriver(devobj, query_irp);
				if (status != STATUS_SUCCESS)
					KdPrint(("[tdi_fw] ot_free: IoCallDriver(set_event_handler): 0x%x\n", status));

			} else if (connobj != NULL) {
				// disconnect connection
				PIRP query_irp;
				NTSTATUS status;

				KdPrint(("[tdi_fw] ot_free: disconnecting connobj 0x%x\n", connobj));

				query_irp = TdiBuildInternalDeviceControlIrp(TDI_DISCONNECT,
					devobj, connobj, NULL, NULL);
				if (query_irp == NULL) {
					KdPrint(("[tdi_fw] ot_free: TdiBuildInternalDeviceControlIrp\n"));
					continue;
				}

				// using TDI_DISCONNECT_RELEASE to make ClientEventDisconnect to be called
				TdiBuildDisconnect(query_irp, devobj, connobj, NULL, NULL, NULL,
					TDI_DISCONNECT_RELEASE, NULL, NULL);

				status = IoCallDriver(devobj, query_irp);
				if (status != STATUS_SUCCESS)
					KdPrint(("[tdi_fw] ot_free: IoCallDriver(disconnect): 0x%x\n", status));

			} else {
				
				// no objects to process. break!
				
				break;
			}
		}
	}

	// clear it!
	KeAcquireSpinLock(&g_ot_hash_guard, &irql);

	for (i = 0; i < HASH_SIZE; i++) {
		struct ot_entry *ote = g_ot_hash[i];
		while (ote != NULL) {
			struct ot_entry *ote2 = ote->next;

			KdPrint(("[tdi_fw] ot_free: Warning! fileobj 0x%x type %d exists!\n",
				ote->fileobj, ote->type));
			
			if (ote->sid_a != NULL)
				free(ote->sid_a);
			free(ote);

			ote = ote2;
		}
	}
	free(g_ot_hash);
	g_ot_hash = NULL;
	
	KeReleaseSpinLock(&g_ot_hash_guard, irql);

	// and cleanup cte_hash
	if (g_cte_hash != NULL) {
		KeAcquireSpinLock(&g_cte_hash_guard, &irql);

		for (i = 0; i < HASH_SIZE; i++) {
			struct ctx_entry *cte = g_cte_hash[i];
			while (cte) {
				struct ctx_entry *cte2 = cte->next;
				free(cte);
				cte = cte2;
			}
		}
		free(g_cte_hash);
		g_cte_hash = NULL;

		KeReleaseSpinLock(&g_cte_hash_guard, irql);
	}
}

//----------------------------------------------------------------------------

NTSTATUS
ot_add_fileobj(PDEVICE_OBJECT devobj, PFILE_OBJECT fileobj, int fileobj_type, int ipproto,
			   CONNECTION_CONTEXT conn_ctx)		// must be called at PASSIVE_LEVEL!
{
	ULONG hash = CALC_HASH(fileobj);
	KIRQL irql;
	struct ot_entry *ote;
	NTSTATUS status;
	int i;
	SID_AND_ATTRIBUTES *sid_a;
	ULONG sid_a_size;

	if (fileobj == NULL)
		return STATUS_INVALID_PARAMETER_2;

	// while we're at PASSIVE_LEVEL get SID & attributes
	sid_a = get_current_sid_a(&sid_a_size);

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);
	
	for (ote = g_ot_hash[hash]; ote != NULL; ote = ote->next)
		if (ote->fileobj == fileobj)
			break;

	if (ote == NULL) {
		ote = (struct ot_entry *)malloc_np(sizeof(*ote));
		if (ote == NULL) {
			KdPrint(("[tdi_fw] ot_add_fileobj: malloc_np\n"));
			status = STATUS_INSUFFICIENT_RESOURCES;
			goto done;
		}
		memset(ote, 0, sizeof(*ote));

		ote->next = g_ot_hash[hash];
		g_ot_hash[hash] = ote;

		ote->fileobj = fileobj;
		for (i = 0; i < MAX_EVENT; i++)
			ote->ctx[i].fileobj = fileobj;

	} else {
		KdPrint(("[tdi_fw] ot_add_fileobj: reuse fileobj 0x%x\n", fileobj));

        ot_cleanup_ote(ote);
	}

	ote->signature = 'OTE ';
	ote->pid = (ULONG)PsGetCurrentProcessId();

	// save SID & attributes
	ote->sid_a = sid_a;
	ote->sid_a_size = sid_a_size;
	sid_a = NULL;

	ote->devobj = devobj;

	ote->type = fileobj_type;
	ote->ipproto = ipproto;
	ote->conn_ctx = conn_ctx;

	status = STATUS_SUCCESS;

done:
	// cleanup
	KeReleaseSpinLock(&g_ot_hash_guard, irql);
	if (sid_a != NULL)
		free(sid_a);

	return status;
}

NTSTATUS
ot_del_fileobj(PFILE_OBJECT fileobj, int *fileobj_type)
{
	ULONG hash = CALC_HASH(fileobj);
	KIRQL irql;
	struct ot_entry *ote, *prev_ote;
	NTSTATUS status;

	if (fileobj == NULL)
		return STATUS_INVALID_PARAMETER_1;

	KeAcquireSpinLock(&g_ot_hash_guard, &irql);

	prev_ote = NULL;
	for (ote = g_ot_hash[hash]; ote; ote = ote->next) {
		if (ote->fileobj == fileobj)
			break;
		prev_ote = ote;
	}

	if (ote == NULL) {
		KdPrint(("[tdi_fw] ot_del_fileobj: fileobj 0x%x not found!\n", fileobj));
		status = STATUS_OBJECT_NAME_NOT_FOUND;
		goto done;
	}

	if (ote->type == FILEOBJ_ADDROBJ && ote->listen_entry != NULL)
		del_listen_obj(ote->listen_entry, FALSE);
	else if (ote->type == FILEOBJ_CONNOBJ && ote->conn_entry != NULL) {
		if (ote->ipproto == IPPROTO_TCP && ote->log_disconnect)
			log_disconnect(ote);
		
		del_tcp_conn_obj(ote->conn_entry, FALSE);
	}

	if (fileobj_type != NULL)
		*fileobj_type = ote->type;

	if (prev_ote != NULL)
		prev_ote->next = ote->next;
	else
		g_ot_hash[hash] = ote->next;

	if (ote->sid_a != NULL)
		free(ote->sid_a);

	free(ote);

	status = STATUS_SUCCESS;

done:
	KeReleaseSpinLock(&g_ot_hash_guard, irql);

	return status;
}

struct ot_entry *
ot_find_fileobj(PFILE_OBJECT fileobj, KIRQL *irql)
{
	ULONG hash = CALC_HASH(fileobj);
	struct ot_entry *ote;

	if (fileobj == NULL)
		return NULL;

	if (irql != NULL)
		KeAcquireSpinLock(&g_ot_hash_guard, irql);

	for (ote = g_ot_hash[hash]; ote != NULL; ote = ote->next)
		if (ote->fileobj == fileobj)
			break;

	if (ote == NULL) {
		KdPrint(("[tdi_fw] ot_find_fileobj: fileobj 0x%x not found!\n", fileobj));
		if (irql != NULL)
			KeReleaseSpinLock(&g_ot_hash_guard, *irql);
	}

	return ote;
}

void
ot_cleanup_ote(struct ot_entry *ote)
{
	struct ot_entry *saved_next;
    PFILE_OBJECT saved_fileobj;
    unsigned int i;

    // set all fields to zero except "next" and "fileobj" (cleanup listen/conn_entry if any)

    saved_next = ote->next;
    saved_fileobj = ote->fileobj;

    if (ote->type == FILEOBJ_ADDROBJ && ote->listen_entry != NULL) {

		del_listen_obj(ote->listen_entry, FALSE);
    
    } else if (ote->type == FILEOBJ_CONNOBJ && ote->conn_entry != NULL) {
		if (ote->ipproto == IPPROTO_TCP && ote->log_disconnect)
			log_disconnect(ote);
    	
		del_tcp_conn_obj(ote->conn_entry, FALSE);
	}

	memset(ote, 0, sizeof(*ote));

    ote->next = saved_next;
    ote->fileobj = saved_fileobj;

    // restore fileobjs
	for (i = 0; i < MAX_EVENT; i++)
		ote->ctx[i].fileobj = saved_fileobj;

}

//----------------------------------------------------------------------------

#define CALC_HASH_2(addrobj, conn_ctx)	CALC_HASH((ULONG)(addrobj) ^ (ULONG)(conn_ctx))

NTSTATUS
ot_add_conn_ctx(PFILE_OBJECT addrobj, CONNECTION_CONTEXT conn_ctx, PFILE_OBJECT connobj)
{
	ULONG hash = CALC_HASH_2(addrobj, conn_ctx);
	KIRQL irql;
	struct ctx_entry *cte;
	NTSTATUS status;

	KeAcquireSpinLock(&g_cte_hash_guard, &irql);
	
	for (cte = g_cte_hash[hash]; cte != NULL; cte = cte->next)
		if (cte->addrobj == addrobj && cte->conn_ctx == conn_ctx)
			break;

	if (cte == NULL) {
		KdPrint(("[tdi_fw] ot_add_fileobj: reuse addrobj 0x%x, conn_ctx 0x%x\n",
			addrobj, conn_ctx));

		cte = (struct ctx_entry *)malloc_np(sizeof(*cte));
		if (cte == NULL) {
			KdPrint(("[tdi_fw] ot_add_conn_ctx: malloc_np\n"));
			status = STATUS_INSUFFICIENT_RESOURCES;
			goto done;
		}
		cte->next = g_cte_hash[hash];
		g_cte_hash[hash] = cte;
	
		cte->addrobj = addrobj;
		cte->conn_ctx = conn_ctx;
	}

	cte->connobj = connobj;
	
	status = STATUS_SUCCESS;
done:

	KeReleaseSpinLock(&g_cte_hash_guard, irql);
	return status;
}

NTSTATUS
ot_del_conn_ctx(PFILE_OBJECT addrobj, CONNECTION_CONTEXT conn_ctx)
{
	ULONG hash = CALC_HASH_2(addrobj, conn_ctx);
	KIRQL irql;
	struct ctx_entry *cte, *prev_cte;
	NTSTATUS status;

	KeAcquireSpinLock(&g_cte_hash_guard, &irql);

	prev_cte = NULL;
	for (cte = g_cte_hash[hash]; cte != NULL; cte = cte->next) {
		if (cte->addrobj == addrobj && cte->conn_ctx == conn_ctx)
			break;
		prev_cte = cte;
	}

	if (cte == NULL) {
		KdPrint(("[tdi_fw] ot_del_conn_ctx: addrobj 0x%x not found!\n", addrobj));
		status = STATUS_OBJECT_NAME_NOT_FOUND;
		goto done;
	}

	if (prev_cte != NULL)
		prev_cte->next = cte->next;
	else
		g_cte_hash[hash] = cte->next;

	free(cte);

	status = STATUS_SUCCESS;
done:

	KeReleaseSpinLock(&g_cte_hash_guard, irql);
	return status;
}

PFILE_OBJECT
ot_find_conn_ctx(PFILE_OBJECT addrobj, CONNECTION_CONTEXT conn_ctx)
{
	ULONG hash = CALC_HASH_2(addrobj, conn_ctx);
	KIRQL irql;
	struct ctx_entry *cte;
	PFILE_OBJECT result = NULL;

	KeAcquireSpinLock(&g_cte_hash_guard, &irql);
	
	for (cte = g_cte_hash[hash]; cte != NULL; cte = cte->next)
		if (cte->addrobj == addrobj && cte->conn_ctx == conn_ctx) {
			result = cte->connobj;
			break;
		}

	KeReleaseSpinLock(&g_cte_hash_guard, irql);
	return result;
}
