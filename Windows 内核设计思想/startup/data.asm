
;G|D/B|L|AVL|Segment limit 19:16
%define G_0   00h  ;0000 0000
%define G_1   80h  ;1000 0000

%define DB_16 00h  ;0000 0000
%define DB_32 40h  ;0100 0000

%define L_0   00h  ;0000 0000
%define L_1   20h  ;0010 0000

%define AVL_0 00h  ;0000 0000
%define AVL_1 10h  ;0001 0000

;P|DPL|S|Type
%define P_0   00h  ;0000 0000
%define P_1   80h  ;1000 0000

%define DPL_0 00h  ;0000 0000
%define DPL_3 60h  ;0110 0000

%define S_0   00h  ;0000 0000
%define S_1   10h  ;0001 0000


;S=1
%define DataType0(Read_Only)                         00h  ;0000 0000
%define DataType1(Read_Only_accessed)                01h  ;0000 0001
%define DataType2(Read_Write)                        02h  ;0000 0010
%define DataType3(Read_Write_accessed)               03h  ;0000 0011
%define DataType4(Read_Only_expand_down)             04h  ;0000 0100
%define DataType5(Read_Only_expand_down_accessed)    05h  ;0000 0101
%define DataType6(Read_Write_expand_down)            06h  ;0000 0110
%define DataType7(Read_Write_expand_down_accessed)   07h  ;0000 0111
%define CodeType8(Execute_Only)                      08h  ;0000 1000
%define CodeType9(Execute_Only_accessed)             09h  ;0000 1001
%define CodeType10(Execute_Read)                     0Ah  ;0000 1010
%define CodeType11(Execute_Read_accessed)            0Bh  ;0000 1011
%define CodeType12(Execute_Only_conforming)          0Ch  ;0000 1100
%define CodeType13(Execute_Only_conforming_accessed) 0Dh  ;0000 1101
%define CodeType14(Execute_Read_conforming)          0Eh  ;0000 1110
%define CodeType15(Execute_Read_conforming_accessed) 0Fh  ;0000 1111

;S=0
%define SystemType0(Reserved)                        00h  ;0000 0000
%define SystemType1(_16_bit_TSS_Available)           01h  ;0000 0001
%define SystemType2(LDT)                             02h  ;0000 0010
%define SystemType3(_16_bit_TSS_Busy)                03h  ;0000 0011
%define SystemType4(_16_bit_Call_Gate)               04h  ;0000 0100
%define SystemType5(Task_Gate)                       05h  ;0000 0101
%define SystemType6(_16_bit_Interrupt_Gate)          06h  ;0000 0110
%define SystemType7(_16_bit_Trap_Gate)               07h  ;0000 0111
%define SystemType8(Reserved)                        08h  ;0000 1000
%define SystemType9(_32_bit_TSS_Available)           09h  ;0000 1001
%define SystemType10(Reserved)                       0Ah  ;0000 1010
%define SystemType11(_32_bit_TSS_Busy)               0Bh  ;0000 1011
%define SystemType12(_32_bit_Call_Gate)              0Ch  ;0000 1100
%define SystemType13(Reserved)                       0Dh  ;0000 1101
%define SystemType14(_32_bit_Interrupt_Gate)         0Eh  ;0000 1110
%define SystemType15(_32_bit_Trap_Gate)              0Fh  ;0000 1111

%macro GDTDesc 6
    dw %6  ;Segment limit 15:00
    dw %5  ;Base Address 15:00
    db %4  ;Base Address 23:16
    db %3  ;P|DPL|S|Type
    db %2  ;G|D/B|L|AVL|Segment limit 19:16
    db %1  ;Base 31:24
%endmacro


align 16
DataBase:
GDTBase:

	GDTDesc 00, 00, 00, 00, \
			0000, 0000

	GDTDesc 00, G_1|DB_32|L_0|AVL_0|0fh, P_1|DPL_0|S_1|CodeType10(Execute_Read), 00, \
			0000,0ffffh

	GDTDesc 00, G_1|DB_32|L_0|AVL_0|0fh, P_1|DPL_0|S_1|DataType2(Read_Write), 00, \
			0000,0ffffh

	GDTDesc 00, G_1|DB_32|L_0|AVL_0|0fh, P_1|DPL_3|S_1|CodeType10(Execute_Read), 00, \
			0000,0ffffh

	GDTDesc 00, G_1|DB_32|L_0|AVL_0|0fh, P_1|DPL_3|S_1|DataType2(Read_Write), 00, \
			0000,0ffffh

	GDTDesc 00, G_0|DB_16|L_0|AVL_0|00, P_1|DPL_0|S_0|SystemType9(_32_bit_TSS_Available), 02, \
			TssBase, TssEnd - TssBase - 1

	GDTDesc 00, G_1|DB_32|L_0|AVL_0|00, P_1|DPL_0|S_1|DataType2(Read_Write), 00, \
			00000h, 0001h

	GDTDesc 00,  G_0|DB_32|L_0|AVL_0|00, P_1|DPL_3|S_1|DataType2(Read_Write), 00, \
			0000, 0fffh

	GDTDesc 00, 00, 00, 00, \
			0000, 0000

	GDTDesc 00, 00, 00, 00, \
			0000, 0000

	GDTDesc 00, 00, 00, 00, \
			0000, 0000

	GDTDesc 00, G_0|DB_16|L_0|AVL_0|00, P_1|DPL_0|S_1|CodeType10(Execute_Read), 02, \
			0000, 0ffffh

	GDTDesc 00, G_0|DB_16|L_0|AVL_0|00, P_1|DPL_0|S_1|DataType2(Read_Write), 02, \
			0000, 0ffffh

	GDTDesc 00, 00, 00, 00, \
			0000, 0000

	GDTDesc 0ffh, G_0|DB_16|L_0|AVL_0|00, P_1|DPL_0|S_0|DataType2(Read_Write), 0ffh, \
			7000h, GDTEnd - GDTBase - 1
        
	times GDTBase+1024-$ db 0	
GDTEnd:


IDTBase:
	times 256 dq 0	
IDTEnd:

SuStackBase:
	times 1024 dw 0
SuStack:

TssBase:
	times 60 dw 0
TssEnd:

align 4
GDTregister:
	dw	GDTEnd - GDTBase - 1
	dd	SU_MODULE_BASE + GDTBase
		
align 4
IDTregister:
	dw	IDTEnd - IDTBase - 1
	dd	SU_MODULE_BASE + IDTBase

align 4
IDTregisterZero:
	dw  0ffffh
	dd  0

align 4
MemoryDescriptorList:
	.Offset		dw	16
	.Segment	dw	DESCRIPTOR_ADDRESS >> 4

FsContextRecord:
;typedef struct _FSCONTEXT_RECORD {
    .BootDrive    dd 80h
    .Reserved	  dd 0
    .Reserved2    dd 0
;} FSCONTEXT_RECORD, *PFSCONTEXT_RECORD;

ExportServicesTable:
;typedef struct _EXTERNAL_SERVICES_TABLE {
    dd	0
    dd	0
    dd	0
    dd	0
    dd	0
    dd	SU_MODULE_BASE + DetectHardware
    dd	0
    dd	0
    dd	0
    dd	0
    dd	0
    dd	SU_MODULE_BASE + GetMemoryDescriptor
    dd	SU_MODULE_BASE + ReadWriteSector
    dd	0
;} EXTERNAL_SERVICES_TABLE, *PEXTERNAL_SERVICES_TABLE;

BootRecord:
;typedef struct _BOOT_CONTEXT {
    .FsContextRecord		dd  SU_MODULE_BASE + FsContextRecord
    .ExternalServicesTable	dd  SU_MODULE_BASE + ExportServicesTable
    .MemoryDescriptorList	dd  DESCRIPTOR_ADDRESS
    .MachineType			dd  0
    .OsLoaderStart			dd  0
    .OsLoaderEnd			dd  0
    .ResourceDirectory		dd  0
    .ResourceOffset			dd  0
    .OsLoaderBase			dd  0
    .OsLoaderExports		dd  0
;} BOOT_CONTEXT, *PBOOT_CONTEXT;

times 02000h-($-$$) db 0

DataEnd:
FileStart:		