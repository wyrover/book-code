#pragma once

#include <ntddk.h>
#include <stdio.h>
#include <windef.h>

#pragma pack (1)
typedef struct _IOAPIC {
	UCHAR select;
	UCHAR _reserved[15]; // fill up the rest of the space
	ULONG data;
} IOAPIC, *PIOAPIC;

///////////////////////////////////////////////////
// IDT structures
///////////////////////////////////////////////////
//#pragma pack(1)

// entry in the IDT, this is sometimes called
// an "interrupt gate"
typedef struct
{
	unsigned short LowOffset;
	unsigned short selector;
	unsigned char unused_lo;
	unsigned char segment_type:4;	//0x0E is an interrupt gate
	unsigned char system_segment_flag:1;
	unsigned char DPL:2;	// descriptor privilege level 
	unsigned char P:1; /* present */
	unsigned short HiOffset;
} IDTENTRY;

/* sidt returns idt in this format */
typedef struct
{
	unsigned short IDTLimit;
	unsigned short LowIDTbase;
	unsigned short HiIDTbase;
	unsigned short reserved;
} IDTINFO;

#pragma pack()
int set_irq1(int intNumber);

#define PARAMETER_KEY           L"\\Parameters"
#define ENABLEWRITEPORT_VALUE   L"EnableReplace"
void ReadRegistery(IN PUNICODE_STRING RegistryPath);

#define DELAY_ONE_MICROSECOND   (-10)
#define DELAY_ONE_MILLISECOND   (DELAY_ONE_MICROSECOND*1000)
#define DELAY_ONE_SECOND        (DELAY_ONE_MILLISECOND*1000)

#define DEVICE_NAME L"\\Device\\KbSniffMp"
#define LINK_NAME L"\\DosDevices\\KbSniffMp"
