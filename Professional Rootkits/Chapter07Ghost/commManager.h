// Copyright Ric Vieler, 2006
// Support header for commManager.c

#ifndef _COMM_MANAGER_H_
#define _COMM_MANAGER_H_

// TCP device name
#define COMM_TCP_DEVICE_NAME      L"\\Device\\Tcp"

// useful macros
#define INETADDR(a, b, c, d) (a + (b<<8) + (c<<16) + (d<<24))
#define HTONL(a) (((a&0xFF)<<24) + ((a&0xFF00)<<8) + ((a&0xFF0000)>>8) + ((a&0xFF000000)>>24))  
#define HTONS(a) (((0xFF&a)<<8) + ((0xFF00&a)>>8))

#define RECEIVE_BUFFER_SIZE	1024

NTSTATUS OpenTDIConnection();
void CloseTDIConnection();
NTSTATUS SendToRemoteController( char* buffer );
VOID timerDPC( PKDPC Dpc, PVOID DeferredContext, PVOID sys1, PVOID sys2 );

#endif
