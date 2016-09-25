
;Disk Address Packet
struc DAP                        
	.PacketSize     resb  1     
	.Reserved       resb  1     
	.BlockCount     resw  1  ;0~127  
	.BufferOffset   resw  1     
	.BufferSegment  resw  1   
	.LBNLow         resd  1  
	.LBNHigh        resd  1
endstruc  
 
struc SectorFrame      
	.DriveNum        resd  1
	.LBNLow          resd  1
	.LBNHigh         resd  1
	.BlockCount      resd  1
	.Buffer          resd  1   
	.ReadWrite       resd  1
endstruc

start:
	cli 
	xor  ax, ax              
	mov  ss, ax                 
	mov  sp, 7c00h        
	sti

	push 07c0h
	pop ds
	mov  ax, 07c0h        
	mov  ds, ax

	push  dword 80000h/512-8 
	push  dword 8
	push  dword 20000h
	call  LoadSector

	;ntfsboot jmp 0x2000:0000£¬fatboot jmp 0x2000:0003
	push  word 2000h
	push  word 0000h 
o16 retf


;void LoadSector(
;	int des, 
;	int src, 
;	int count);
LoadSector:
;{
	push  ebp  
	mov  ebp, esp 
	sub  esp, 0E4h 
	push  ebx  
	push  esi  
	push  edi  
	
	;int run = count / 64;
	mov  eax,dword [ebp+0Eh] 
	cdq              
	and  edx, 3Fh 
	add  eax, edx 
	sar  eax, 6 
	mov  dword [ebp-8], eax 
	
	;int mod = count % 64;
	mov  eax, dword [ebp+0Eh] 
	and  eax, 8000003Fh 
	jns  .set_mod
	dec  eax  
	or  eax,0FFFFFFC0h 
	inc  eax  
	
.set_mod: 
	mov  dword [ebp-14h], eax 

	;for(int i=0; i<run; i++)
	mov  dword [ebp-20h], 0 
	jmp  _read
	
_loop:
	mov  eax,dword [ebp-20h] 
	add  eax,1 
	mov  dword [ebp-20h],eax 
_read:   
    mov  eax,dword [ebp-20h] 
	cmp  eax,dword [ebp-8] 
	jge  _left
	;{
	;ReadWriteSector(0x80, src, 0, 64, des, 0x42);
	push  dword 42h
	mov  eax, dword [ebp+6] 
	push  eax  
	push  dword 40h  
	push  dword 0    
	mov  ecx, dword [ebp+0Ah] 
	push  ecx  
	push  dword 80h  
	call  ReadWriteSector
	add  esp, 24 

	;src = src + 64;
	mov  eax, dword [ebp+0Ah] 
	add  eax, 40h 
	mov  dword [ebp+0Ah], eax 
	;des = des + 64*512;
	mov  eax, dword [ebp+6] 
	add  eax, 8000h 
	mov  dword [ebp+6], eax 
	;}
	jmp  _loop

	;if(mod)
_left:
	cmp  dword [ebp-14h],0           
    je  .return
	;{
		;ReadWriteSector(0x80, src, 0, mod, des, 0x42);
	push  dword 42h	
	mov  eax, dword [ebp+6] 
	push  eax  
	mov  ecx, dword [ebp-14h] 
	push  ecx  
	push  dword 0    
	mov  edx, dword [ebp+0Ah] 
	push  edx  
	push  dword 80h  
	call  ReadWriteSector
	add  esp, 24
	;}
;}
.return:   
    pop  edi  
	pop  esi  
	pop  ebx  
	add  esp, 0E4h 
	mov  esp, ebp 
	pop  ebp  
	ret 12        

	
;NTSTATUS 
;(_cdecl* ReadWriteSector)(
;	ULONG DriveNum, 
;	ULONG LBNLow, 
;	ULONG LBNHigh, 
;	ULONG BlockCount, 
;	PVOID Buffer, 
;	ULONG ReadWrite); // Read=0x42,Write=0x43
	
ReadWriteSector:

    push  bp
    mov  bp, sp
    add  bp, 4

    push  ds
    push  si
    push  bx
	
    push  0
    pop  ds
    	
	sub  sp, 16
	mov  si, sp
	mov  byte [si+DAP.PacketSize], 10h        
	mov  byte [si+DAP.Reserved], 0          
	mov  al,byte [bp+SectorFrame.BlockCount]
	mov  byte[si+DAP.BlockCount], al           
	mov  byte[si+DAP.BlockCount+1], 0         
	mov  eax, dword[bp+SectorFrame.Buffer]
    mov  bx, ax
    and  bx, 0fh     
    mov  word[si+DAP.BufferOffset], bx        
    shr  eax, 4
    mov  word[si+DAP.BufferSegment], ax        
    mov  eax, dword[bp+SectorFrame.LBNLow]
    mov  dword[si+DAP.LBNLow], eax            
    mov  eax, dword[bp+SectorFrame.LBNHigh]
    mov  dword[si+DAP.LBNHigh], eax            

    mov  ah, byte [bp+SectorFrame.ReadWrite]                                          
    mov  dl, byte [bp+SectorFrame.DriveNum]          
    int  13h
    jc   .error

    xor  eax, eax
.error:

    and  eax, 0000ffffh
            
	add  sp, 16
	pop  bx
	pop  si
	pop  ds
	
	pop  bp
	retn


times 510-64-($-$$) db 0
	db  80h
	db  00
	db  02
	db  00
	db  07
	db  08
	db  08
	db  32
	dd  00000001h
	dd  00007fffh

times 510-($-$$) db 0
	dw  0AA55h	
	
;**********************************************************************
;DBR or PBR
	db 0,0,0      ;UCHAR     Jump[3];			// 0x00
	db  "NTFS    " ;UCHAR     OEMID[8];			// 0x03
	;typedef struct _BIOS_PARAMETERS_BLOCK
	;{
		dw 512;USHORT    BytesPerSector;		// 0x0B
		db 8;UCHAR     SectorsPerCluster;		// 0x0D
		db 0,0,0,0,0,0,0;UCHAR     Unused0[7];	// 0x0E, checked when volume is mounted
		db 12;UCHAR     MediaId;				// 0x15, 11=RemovableMedia  12=FixedMedia
		db 0,0;UCHAR     Unused1[2];			// 0x16
		dw 15;USHORT    SectorsPerTrack;		// 0x18
		dw 0;USHORT    Heads;					// 0x1A
		db 0,0,0,0;UCHAR     Unused2[4];		// 0x1C
		db 0,0,0,0;UCHAR     Unused3[4];		// 0x20, checked when volume is mounted
	;} BIOS_PARAMETERS_BLOCK, *PBIOS_PARAMETERS_BLOCK;
	
	;typedef struct _EXTENDED_BIOS_PARAMETERS_BLOCK
	;{
		dw 0,0;USHORT    Unknown[2];			// 0x24, always 80 00 80 00
		dq 07fffh;ULONGLONG SectorCount;		// 0x28
		dq 0;ULONGLONG MftLocation;				// 0x30
		dq 0;ULONGLONG MftMirrLocation;			// 0x38
		db 0;CHAR      ClustersPerMftRecord;	// 0x40
		db 0,0,0;UCHAR     Unused4[3];			// 0x41
		db 0;CHAR      ClustersPerIndexRecord;	// 0x44
		db 0,0,0;UCHAR     Unused5[3];			// 0x45
		dq 0;ULONGLONG SerialNumber;			// 0x48
		db 0,0,0,0;UCHAR     Checksum[4];		// 0x50
	;} EXTENDED_BIOS_PARAMETERS_BLOCK, *PEXTENDED_BIOS_PARAMETERS_BLOCK;

times 1022-($-$$) db 0
	dw  0AA55h
;**********************************************************************

 %macro LOAD_BOOT_FILE 4
 %%4:
       dd %1	;FileFlags
       dd %2	;FileStart
       dd %3	;FileSize
       db %4	;FileName[20]
       times %%4+32-$ db 0
 %endmacro
 
;\system32 
LOAD_BOOT_FILE   1, NTOSKRNL_START,  NTOSKRNL_END - NTOSKRNL_START, "ntoskrnl.exe"
LOAD_BOOT_FILE   1, HAL_START,       HAL_END - HAL_START,			"HAL.dll"
LOAD_BOOT_FILE   1, KDCOM_START,     KDCOM_END - KDCOM_START,		"kdcom.dll"
LOAD_BOOT_FILE   1, BOOTVID_START,   BOOTVID_END - BOOTVID_START,   "bootvid.dll"
LOAD_BOOT_FILE   1, NTDETECT_START,  NTDETECT_END - NTDETECT_START, "NTDETECT.COM"
LOAD_BOOT_FILE   1, C_936_START,     C_936_END - C_936_START,		"c_936.nls"
LOAD_BOOT_FILE   1, L_INTL_START,    L_INTL_END - L_INTL_START,		"l_intl.nls"
LOAD_BOOT_FILE   1, VGAOEM_START,    VGAOEM_END - VGAOEM_START,		"vgaoem.fon"
LOAD_BOOT_FILE   1, SYSTEM_START,    SYSTEM_END - SYSTEM_START,		"system"
LOAD_BOOT_FILE   0, NTDLL_START,     NTDLL_END - NTDLL_START,		"ntdll.dll"
LOAD_BOOT_FILE   0, SMSS_START,     SMSS_END - SMSS_START,		    "smss.exe"
;\system32\drivers
LOAD_BOOT_FILE   1, ACPI_START,     ACPI_END - ACPI_START,			"acpi.sys"
LOAD_BOOT_FILE   1, PCI_START,      PCI_END - PCI_START,			"pci.sys"	
LOAD_BOOT_FILE   1, UNIATA_START,    UNIATA_END - UNIATA_START,		"uniata.sys"
LOAD_BOOT_FILE   1, DISK_START,     DISK_END - DISK_START,			"disk.sys"
LOAD_BOOT_FILE   1, NTFS_START,     NTFS_END - NTFS_START,			"fastfat.sys"
LOAD_BOOT_FILE   1, SCSIPORT_START, SCSIPORT_END - SCSIPORT_START,	"scsiport.sys"	
;WRK
;LOAD_BOOT_FILE   1, ACPI_START,     ACPI_END - ACPI_START,			"acpi.sys"
;LOAD_BOOT_FILE   1, PCI_START,      PCI_END - PCI_START,			"pci.sys"		
;LOAD_BOOT_FILE   1, INTELIDE_START, INTELIDE_END - INTELIDE_START,	"intelide.sys"
;LOAD_BOOT_FILE   1, MOUNTMGR_START, MOUNTMGR_END - MOUNTMGR_START,	"mountmgr.sys"		
;LOAD_BOOT_FILE   1, FTDISK_START,   FTDISK_END - FTDISK_START,		"ftdisk.sys"		
;LOAD_BOOT_FILE   1, DMIO_START,     DMIO_END - DMIO_START,			"dmio.sys"
;LOAD_BOOT_FILE   1, VOLSNAP_START,  VOLSNAP_END - VOLSNAP_START,	"volsnap.sys"
;LOAD_BOOT_FILE   1, PARTMGR_START,  PARTMGR_END - PARTMGR_START,	"partmgr.sys"		
;LOAD_BOOT_FILE   1, ATAPI_START,    ATAPI_END - ATAPI_START,		"atapi.sys" 
;LOAD_BOOT_FILE   1, DISK_START,     DISK_END - DISK_START,			"disk.sys"			
;LOAD_BOOT_FILE   1, FLTMGR_START,   FLTMGR_END - FLTMGR_START,		"fltmgr.sys" 
;LOAD_BOOT_FILE   1, NTFS_START,     NTFS_END - NTFS_START,			"ntfs.sys"			
;LOAD_BOOT_FILE   1, WMILIB_START,   WMILIB_END - WMILIB_START,		"wmilib.sys"
;LOAD_BOOT_FILE   1, SCSIPORT_START, SCSIPORT_END - SCSIPORT_START,	"scsiport.sys"		
;LOAD_BOOT_FILE   1, CLASSPNP_START, CLASSPNP_END - CLASSPNP_START,	"classpnp.sys"		
;LOAD_BOOT_FILE   1, PCIIDEX_START,  PCIIDEX_END - PCIIDEX_START,	"pciidex.sys"		
;LOAD_BOOT_FILE   1, KSECDD_START,   KSECDD_END - KSECDD_START, 	"KSecDD.sys" 

times 4096-($-$$) db 0

;**********************************	
NTLDR_START:
incbin	"..\Debug\startup.com"
;incbin "..\system32\\startup.com"
incbin "..\Debug\osloader.exe"
NTLDR_END:
times 80000h-($-$$) db 0
;**********************************	

NTOSKRNL_START:
incbin	"..\Debug\ntoskrnl.exe"
;incbin	"E:\code\wrk-v1.2\base\ntos\build\exe\wrkx86.exe"
NTOSKRNL_END: align 16

HAL_START:
incbin	"..\reactos\system32\hal.dll"
;incbin	"..\system32\halmacpi.dll"
HAL_END: align 16

KDCOM_START:
incbin	"..\system32\kdcom.dll"
KDCOM_END: align 16

BOOTVID_START:
incbin	"..\system32\bootvid.dll"
BOOTVID_END: align 16

NTDETECT_START:
incbin	"..\system32\NTDETECT.COM"
NTDETECT_END: align 16

C_936_START:
incbin	"..\system32\c_936.nls"
C_936_END: align 16

L_INTL_START:
incbin	"..\system32\l_intl.nls"
L_INTL_END: align 16

VGAOEM_START:
incbin	"..\system32\vgaoem.fon"
VGAOEM_END: align 16

SYSTEM_START:
incbin	"..\reactos\system32\system"
;incbin	"..\system32\system"
SYSTEM_END: align 16

NTDLL_START:
incbin	"..\reactos\system32\ntdll.dll"
NTDLL_END: align 16

SMSS_START:
incbin	"..\reactos\system32\smss.exe"
SMSS_END: align 16
;****************************************

ACPI_START:
incbin	"..\Debug\acpi.sys"
ACPI_END: align 16

PCI_START:
incbin	"..\Debug\pci.sys"
PCI_END: align 16

UNIATA_START:
incbin	"..\Debug\uniata.sys"
UNIATA_END: align 16

DISK_START:
incbin	"..\Debug\disk.sys"
DISK_END: align 16

NTFS_START:
incbin	"..\Debug\ntfs.sys"
NTFS_END: align 16

SCSIPORT_START:
incbin	"..\Debug\scsiport.sys"
SCSIPORT_END: align 16

;****************************************
;ACPI_START:
;incbin	"..\system32\drivers\acpi.sys"
;ACPI_END: align 16
;
;PCI_START:
;incbin	"..\system32\drivers\pci.sys"
;PCI_END: align 16
;
;INTELIDE_START:
;incbin	"..\system32\drivers\intelide.sys"
;INTELIDE_END: align 16
;
;MOUNTMGR_START:
;incbin	"..\system32\drivers\mountmgr.sys"
;MOUNTMGR_END: align 16
;
;FTDISK_START:
;incbin	"..\system32\drivers\ftdisk.sys"
;FTDISK_END: align 16
;
;DMIO_START:
;incbin	"..\system32\drivers\dmio.sys"
;DMIO_END: align 16
;
;VOLSNAP_START:
;incbin	"..\system32\drivers\volsnap.sys"
;VOLSNAP_END: align 16
;
;PARTMGR_START:
;incbin	"..\system32\drivers\partmgr.sys"
;PARTMGR_END: align 16
;
;ATAPI_START:
;incbin	"..\system32\drivers\atapi.sys"
;ATAPI_END: align 16
;
;DISK_START:
;incbin	"..\system32\drivers\disk.sys"
;DISK_END: align 16
;
;FLTMGR_START:
;incbin	"..\system32\drivers\fltmgr.sys"
;FLTMGR_END: align 16
;
;NTFS_START:
;incbin	"..\Debug\ntfs.sys"
;NTFS_END: align 16
;
;WMILIB_START:
;incbin	"..\system32\drivers\wmilib.sys"
;WMILIB_END: align 16
;
;SCSIPORT_START:
;incbin	"..\system32\drivers\scsiport.sys"
;SCSIPORT_END: align 16
;
;CLASSPNP_START:
;incbin	"..\system32\drivers\classpnp.sys"
;CLASSPNP_END: align 16
;
;PCIIDEX_START:
;incbin	"..\system32\drivers\pciidex.sys"
;PCIIDEX_END: align 16
;
;KSECDD_START:
;incbin	"..\system32\drivers\KSecDD.sys"
;KSECDD_END: align 16


times 1000000h-($-$$) db 0