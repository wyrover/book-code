
PROTECT_MODE            equ  000000001 
ENABLE_PAGING           equ  80000000h     

SU_MODULE_BASE          equ  20000h
SYSTEM_PAGE_BASE        equ  1e000h
DESCRIPTOR_ADDRESS		equ  70000h
LOADER_STACK            equ  61ffch

KeCodeSelector          equ  08h
KeDataSelector          equ  10h
KeTssSelector           equ  28h
KePcrSelector           equ  30h
SuCodeSelector          equ  58h
SuDataSelector          equ  60h

struc E820FRAME
;typedef struct _E820FRAME{
    .ErrorFlag			resd  1
    .Key				resd  1
    .Size				resd  1
    ;struct {
        .BaseAddrLow	resd  1
        .BaseAddrHigh	resd  1
        .SizeLow		resd  1
        .SizeHigh		resd  1
        .MemoryType		resd  1
    ;} Descriptor;
;} E820FRAME, *PE820FRAME;
endstruc
E820FRAME_DESCRIPTOR_SIZE	 equ	E820FRAME_size - E820FRAME.BaseAddrLow

struc MEMORY_DESCRIPTOR
    .BlockBase       resd  1
    .BlockSize       resd  1
endstruc


; Disk Address Packet  
struc DAP                        
	.PacketSize     resb  1  
	.Reserved       resb  1  
	.BlockCount     resw  1     
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

struc DetectionFrame         
    .HeapStart       resd  1
    .HeapSize        resd  1
    .ConfigTree      resd  1
    .HeapUsed        resd  1
    .LoadOptions     resd  1
    .OptionsLength   resd  1
endstruc


struc IMAGE_DOS_HEADER
;typedef struct _IMAGE_DOS_HEADER {       // DOS .EXE header
    .e_magic          resw   1;           // Magic number
    .e_cblp           resw   1;           // Bytes on last page of file
    .e_cp             resw   1;           // Pages in file
    .e_crlc           resw   1;           // Relocations
    .e_cparhdr        resw   1;           // Size of header in paragraphs
    .e_minalloc       resw   1;           // Minimum extra paragraphs needed
    .e_maxalloc       resw   1;           // Maximum extra paragraphs needed
    .e_ss             resw   1;           // Initial (relative) SS value
    .e_sp             resw   1;           // Initial SP value
    .e_csum           resw   1;           // Checksum
    .e_ip             resw   1;           // Initial IP value
    .e_cs             resw   1;           // Initial (relative) CS value
    .e_lfarlc         resw   1;           // File address of relocation table
    .e_ovno           resw   1;           // Overlay number
    .e_res            resw   4;           // e_res[4] Reserved words
    .e_oemid          resw   1;           // OEM identifier (for e_oeminfo)
    .e_oeminfo        resw   1;           // OEM information; e_oemid specific
    .e_res2           resw  10;           // e_res2[10] Reserved words
    .e_lfanew         resd   1;           // File address of new exe header
;  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
endstruc

;
; File header format.
;
struc IMAGE_FILE_HEADER
;typedef struct _IMAGE_FILE_HEADER {
    .Machine                 resw   1;
    .NumberOfSections        resw   1;
    .TimeDateStamp           resd   1;
    .PointerToSymbolTable    resd   1;
    .NumberOfSymbols         resd   1;
    .SizeOfOptionalHeader    resw   1;
    .Characteristics         resw   1;
;} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;
endstruc
%define  IMAGE_SIZEOF_FILE_HEADER   20

;
; Directory format.
;
struc IMAGE_DATA_DIRECTORY
;typedef struct _IMAGE_DATA_DIRECTORY {
    .VirtualAddress    resd   1;
    .Size              resd   1;
;} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
endstruc

%define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

;
; Optional header format.
;
struc IMAGE_OPTIONAL_HEADER
;typedef struct _IMAGE_OPTIONAL_HEADER {
    ;
    ; Standard fields.
    ;

    .Magic                             resw   1;
    .MajorLinkerVersion                resb   1;
    .MinorLinkerVersion                resb   1;
    .SizeOfCode                        resd   1;
    .SizeOfInitializedData             resd   1;
    .SizeOfUninitializedData           resd   1;
    .AddressOfEntryPoint               resd   1;
    .BaseOfCode                        resd   1;
    .BaseOfData                        resd   1;

    ;
    ; NT additional fields.
    ;

    .ImageBase                         resd   1;
    .SectionAlignment                  resd   1;
    .FileAlignment                     resd   1;
    .MajorOperatingSystemVersion       resw   1;
    .MinorOperatingSystemVersion       resw   1;
    .MajorImageVersion                 resw   1;
    .MinorImageVersion                 resw   1;
    .MajorSubsystemVersion             resw   1;
    .MinorSubsystemVersion             resw   1;
    .Win32VersionValue                 resd   1;
    .SizeOfImage                       resd   1;
    .SizeOfHeaders                     resd   1;
    .CheckSum                          resd   1;
    .Subsystem                         resw   1;
    .DllCharacteristics                resw   1;
    .SizeOfStackReserve                resd   1;
    .SizeOfStackCommit                 resd   1;
    .SizeOfHeapReserve                 resd   1;
    .SizeOfHeapCommit                  resd   1;
    .LoaderFlags                       resd   1;
    .NumberOfRvaAndSizes               resd   1;
    .DataDirectory                     resd   IMAGE_NUMBEROF_DIRECTORY_ENTRIES*2;IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
;} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;
endstruc
%define IMAGE_SIZEOF_NT_OPTIONAL_HEADER    224

struc IMAGE_NT_HEADERS
;typedef struct _IMAGE_NT_HEADERS {
    .Signature                 resd   1;
    .FileHeader                resb   20;  sizeof IMAGE_FILE_HEADER 
    .OptionalHeader            resb   224; sizeof IMAGE_OPTIONAL_HEADER
;} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;
endstruc 

;
; Section header format.
;

%define IMAGE_SIZEOF_SHORT_NAME              8
struc IMAGE_SECTION_HEADER
;typedef struct _IMAGE_SECTION_HEADER {
    .Name                   resb   IMAGE_SIZEOF_SHORT_NAME;
    ;union {
            ;ULONG   PhysicalAddress;
            .VirtualSize    resd   1;
    ;} Misc;
    .VirtualAddress         resd   1;
    .SizeOfRawData          resd   1;
    .PointerToRawData       resd   1;
    .PointerToRelocations   resd   1;
    .PointerToLinenumbers   resd   1;
    .NumberOfRelocations    resw   1;
    .NumberOfLinenumbers    resw   1;
    .Characteristics        resd   1;
;} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
endstruc
%define IMAGE_SIZEOF_SECTION_HEADER           40		



[bits 16]
[section .text]
org	0
Start:
		jmp		RealStart
	
		times 512-($-$$) db 0
	
RealStart:

		mov		ax,cs
		mov		ss,ax
		mov		ds,ax
				
		xor		eax,eax
		xor		ecx,ecx
		xor		edx,edx
		xor		ebx,ebx
		xor		esi,esi
		xor		edi,edi
		xor		ebp,ebp
		mov		esp,SuStack
		call	SuMain

SuMain:
		push	bp
		mov		bp,sp
		
		call	ConstructMemoryDescriptors

		call	EnableA20
		call	Relocatex86Structures
	
		push	word 0
		call	EnableProtectPaging
		add		sp,2

		push	dword  BootRecord.OsLoaderEnd
		push	dword  BootRecord.OsLoaderStart
		call	RelocateLoaderSections
		add		sp,8
				
		push	eax
		call	TransferToLoader
		add		sp,4
		
		mov		sp,bp
		pop		bp
		retn

;BOOLEAN ConstructMemoryDescriptors (VOID);
ConstructMemoryDescriptors:

E820Frame.ErrorFlag equ -28h
E820Frame.Key   equ -24h
E820Frame.Size  equ -20h
E820Frame.Descriptor.BaseAddrLow equ -1Ch
E820Frame.Descriptor.BaseAddrHigh equ -18h
E820Frame.Descriptor.SizeLow equ -14h
E820Frame.Descriptor.MemoryType equ -0Ch
EndAddr         equ -8
StartAddr       equ -4

		push    ebp
		mov     ebp, esp
		sub     esp, 68h
		push    ebx
		push    esi
		push    edi

;(7000H * 10H) + 0 = 70000H
		push    es
		push    DESCRIPTOR_ADDRESS >> 4
		pop     es
		mov     eax, 0

		mov     dword[eax+MEMORY_DESCRIPTOR.BlockSize], 0
		mov     eax, MemoryDescriptorList
		mov     dword[eax+MEMORY_DESCRIPTOR.BlockBase], 0
		mov     dword[ebp+E820Frame.Key], 0

.loop:                            
		mov     dword[ebp+E820Frame.Size], E820FRAME_DESCRIPTOR_SIZE
		lea     eax, [ebp+E820Frame.ErrorFlag]
		push    eax
		call    Int15E820
		add     esp, 4
		cmp     dword[ebp+E820Frame.ErrorFlag], 0
		jnz     .breakA
		cmp     dword[ebp+E820Frame.Size], E820FRAME_DESCRIPTOR_SIZE
		jnb     .GotAddr

.breakA:                               
		jmp     .return
; ---------------------------------------------------------------------------

.GotAddr:                            
		mov     eax, [ebp+E820Frame.Descriptor.BaseAddrLow]
		mov     [ebp+StartAddr], eax
		mov     eax, [ebp+E820Frame.Descriptor.SizeLow]
		mov     ecx, [ebp+E820Frame.Descriptor.BaseAddrLow]
		lea     edx, [ecx+eax-1]
		mov     [ebp+EndAddr], edx
		cmp     dword[ebp+E820Frame.Descriptor.BaseAddrHigh], 0
		jnz     .next
		mov     eax, [ebp+EndAddr]
		cmp     eax, [ebp+StartAddr]
		jnb     .MemoryType
		mov     dword[ebp+EndAddr], 0FFFFFFFFh

.MemoryType:                          
		cmp     dword[ebp+E820Frame.Descriptor.MemoryType], 1
		jnz     .next
		mov     eax, [ebp+EndAddr]
		sub     eax, [ebp+StartAddr]
		add     eax, 1
		push    eax
		mov     ecx, [ebp+StartAddr]
		push    ecx
		call    InsertDescriptor
		add     esp, 8

.next:                               
	                                     
		cmp     dword[ebp+E820Frame.Key], 0
		jnz     .loop

.return:                             
		mov     al, 1
		pop     es
		pop     edi
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		retn


;VOID
;InsertDescriptor (
;    ULONG Address,
;    ULONG BlockSize
;    );
InsertDescriptor:

CurrentList     equ -4
Address         equ  6
BlockSize       equ  0Ah

        push    ebp
        mov     ebp, esp
        sub     esp, 44h
        push    ebx
        push    esi
        push    edi
   
;(7000H * 10H) + 0 = 70000H     
        push    es
        push    DESCRIPTOR_ADDRESS >> 4
        pop     es
        mov     eax, 0
        
        mov     [ebp+CurrentList], eax

.loop: 
        mov     eax, [ebp+CurrentList]
     es	cmp     dword[eax+MEMORY_DESCRIPTOR.BlockSize], 0
        jbe     .BlockSize
        mov     eax, [ebp+Address]
        add     eax, [ebp+BlockSize]
        mov     ecx, [ebp+CurrentList]
        cmp     eax, [ecx]
        jnz     .BlockBase
        mov     eax, [ebp+CurrentList]
        mov     ecx, [ebp+Address]
        mov     [eax], ecx
        mov     eax, [ebp+CurrentList]
     es	mov     ecx, [eax+MEMORY_DESCRIPTOR.BlockSize]
        add     ecx, [ebp+BlockSize]
        mov     edx, [ebp+CurrentList]
     es	mov     [edx+MEMORY_DESCRIPTOR.BlockSize], ecx
        jmp     .BlockSize
; ---------------------------------------------------------------------------

.BlockBase:                         
        mov     eax, [ebp+CurrentList]
     es mov     ecx, [eax+MEMORY_DESCRIPTOR.BlockBase]
        mov     edx, [ebp+CurrentList]
     es add     ecx, [edx+MEMORY_DESCRIPTOR.BlockSize]
        cmp     [ebp+Address], ecx
        jnz     .next
        mov     eax, [ebp+CurrentList]
     es mov     ecx, [eax+MEMORY_DESCRIPTOR.BlockSize]
        add     ecx, [ebp+BlockSize]
        mov     edx, [ebp+CurrentList]
     es mov     [edx+MEMORY_DESCRIPTOR.BlockSize], ecx
        jmp     .BlockSize
; ---------------------------------------------------------------------------

.next:                           
        mov     eax, [ebp+CurrentList]
        add     eax, 8
        mov     [ebp+CurrentList], eax
        jmp     .loop
; ---------------------------------------------------------------------------

.BlockSize:                        
                                   
        mov     eax, [ebp+CurrentList]
     es cmp     dword[eax+MEMORY_DESCRIPTOR.BlockSize], 0
        jnz     .return
        mov     eax, [ebp+CurrentList]
        mov     ecx, [ebp+Address]
     es mov     [eax+MEMORY_DESCRIPTOR.BlockBase], ecx
        mov     eax, [ebp+CurrentList]
        mov     ecx, [ebp+BlockSize]
     es mov     [eax+MEMORY_DESCRIPTOR.BlockSize], ecx
        mov     eax, [ebp+CurrentList]
        add     eax, 8
        mov     [ebp+CurrentList], eax
        mov     eax, [ebp+CurrentList]
     es mov     dword[eax+MEMORY_DESCRIPTOR.BlockBase], 0
        mov     eax, [ebp+CurrentList]
     es mov     dword[eax+MEMORY_DESCRIPTOR.BlockSize], 0

.return:      
		pop     es                         
        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        retn

Int15E820:

		push	ebp
		mov		bp,sp
		mov		bp,[bp + 6]
		push	es
		push	edi
		push	esi
		push	ebx

		push	ss
		pop		es

		mov		ebx,[bp + E820FRAME.Key]         
		                                          
		mov		ecx,[bp + E820FRAME.Size]    
		lea		di, [bp + E820FRAME.BaseAddrLow] 
		mov		eax, 0E820h
		mov		edx, 'PAMS'      

		int		15h

		mov		[bp + E820FRAME.Key], ebx      
		mov		[bp + E820FRAME.Size], ecx  

		sbb		ecx, ecx                      
		sub		eax, 'PAMS'                   
		or		ecx, eax
		mov		[bp + E820FRAME.ErrorFlag],ecx 

		pop		ebx
		pop		esi
		pop		edi
		pop		es
		pop		ebp
		retn
										
			
EnableA20:	
		call	empty_8042     
		mov		al, 0D1h
		out		064h, al     
		call	empty_8042
		mov		al, 0DFh
		out		060h, al       
		call	empty_8042     
		ret		

empty_8042:
		jmp		$+3
		jmp		$+3
		in		al,064h
		test	al,02h
		jnz		empty_8042
		ret
										
Relocatex86Structures:

		push	bp
		mov		bp, sp
		push	si
		push	di
		push	es

		mov		si,GDTBase

		mov		ax, SYSTEM_PAGE_BASE >> 4
		mov		es,ax
		xor		di,di

		mov		cx,IDTEnd - GDTBase

.loop:
		mov		al,[si]
	es	mov		[di],al
		inc		si
		inc		di
		dec		cx
		jnz		.loop

		pop		es
		pop		di
		pop		si
		mov		sp, bp
		pop		bp
		retn	
	
																
EnableProtectPaging:

		push	dword 0
		popfd
		mov		bx,sp
		mov		dx,[bx + 2]
		xor		ax,ax
		mov		gs,ax
		mov		es,ax

		push	KePcrSelector
		pop		fs

		cli
		lgdt	[GDTregister]
		lidt	[IDTregister]

		mov		eax,cr0
			
		or		dx,dx
		jz		.set_protect
				

		or		eax,PROTECT_MODE | ENABLE_PAGING
		mov		cr0,eax
		jmp		.set_end
			           
.set_protect:
		or		eax,PROTECT_MODE
		mov		cr0,eax

.set_end:

		push	SuCodeSelector
		push	.restart
		retf

.restart:
		mov		ax,SuDataSelector
		mov		ds,ax
		mov		ss,ax

		xor		bx,bx
		lldt	bx

		or		dx,dx
		jnz		.return

		mov		bx,KeTssSelector
		ltr		bx

.return:
		retn
								


RealMode:

		sgdt	[GDTregister]
		sidt	[IDTregister]
		mov		ax,SuDataSelector
		mov		es,ax
		mov		fs,ax
		mov		gs,ax

		mov		eax,cr0
		and		eax, ~(ENABLE_PAGING | PROTECT_MODE)
		mov		cr0,eax

		mov		eax,cr3
		mov		cr3,eax
	       
		push	2000h          
		push	.real_mode 
		retf

.real_mode:
		mov		ax,SU_MODULE_BASE >> 4
		mov		ds,ax
		mov		ss,ax
		lidt	[IDTregisterZero]
								
		sti
		ret		

									      
TransferToLoader:

		mov		ebx,dword [esp + 2] 

		mov		cx, KeDataSelector
		mov		ss,cx
		mov		esp,LOADER_STACK

		mov		ds,cx
		mov		es,cx

		push	dword BootRecord + SU_MODULE_BASE
		
		push	dword 10101010h  

		push	dword KeCodeSelector
		push	ebx
	o32	retf							
				
;void *  __cdecl memmove(
;	void * _Dst, 
;	void * _Src, 
;	size_t _Size);				
memmove:
		push	bp
		mov		bp,sp
		push	ds
		push	es
		push	esi
		push	edi
		push	ecx

		mov		edi,[bp+4]
		mov		esi,[bp+8]
		mov		ecx,[bp+12]
		shr		ecx,2

		mov		ax,KeDataSelector
		mov		ds,ax
		mov		es,ax

		cld
	a32	rep movsd

		mov		ecx, dword [bp+12]
		and		ecx, 3
	a32	rep	movsb

		pop		ecx
		pop		edi
		pop		esi
		pop		es
		pop		ds
		pop		bp
		retn				
			
;ULONG
;RelocateLoaderSections(
;    OUT PULONG ImageStart,
;    OUT PULONG ImageEnd
;    );
RelocateLoaderSections:

SizeOfRawData   equ -24h
VirtualSize     equ -20h
Destination     equ -1Ch
Source          equ -18h
NumberOfSections equ -14h
SectionHeader   equ -10h
OptionalHeader  equ -0Ch
FileHeader      equ -8
DosHeader       equ -4
ImageStart      equ  6
ImageEnd        equ  0Ah

        push    ebp
        mov     ebp, esp
        sub     esp, 64h
        push    ebx
        push    esi
        push    edi
        mov     eax, FileStart
        mov     [ebp+DosHeader], eax
        mov     eax, [ebp+DosHeader]
        mov     ecx, [eax+IMAGE_DOS_HEADER.e_lfanew]
        mov     edx, FileStart
        lea     eax, [edx+ecx+4]
        mov     [ebp+FileHeader], eax
        mov     eax, [ebp+FileHeader]
        add     eax, 14h
        mov     [ebp+OptionalHeader], eax
        mov     eax, [ebp+FileHeader]       
        movzx   ecx, word[eax+IMAGE_FILE_HEADER.SizeOfOptionalHeader]
        add     ecx, [ebp+OptionalHeader]
        mov     [ebp+SectionHeader], ecx
        mov     eax, [ebp+OptionalHeader]
        mov     ecx, [eax+IMAGE_OPTIONAL_HEADER.ImageBase]
        mov     edx, [ebp+SectionHeader]
        add     ecx, [edx+IMAGE_SECTION_HEADER.VirtualAddress]
        mov     eax, [ebp+ImageStart]
        mov     [eax], ecx
        mov     eax, [ebp+ImageStart]
        mov     ecx, [eax]
        mov     edx, [ebp+SectionHeader]
        add     ecx, [edx+IMAGE_SECTION_HEADER.SizeOfRawData]
        mov     eax, [ebp+ImageEnd]
        mov     [eax], ecx
        mov     eax, [ebp+OptionalHeader]
        mov     ecx, [eax+IMAGE_OPTIONAL_HEADER.SizeOfHeaders]
        push    ecx             ; size_t
        mov     edx, FileStart + SU_MODULE_BASE
        push    edx             ; void *
        mov     eax, [ebp+OptionalHeader]
        mov     ecx, [eax+IMAGE_OPTIONAL_HEADER.ImageBase]
        push    ecx             ; void *
        call    memmove
        add     esp, 0Ch
        mov     eax, [ebp+FileHeader]
        mov     cx, [eax+IMAGE_FILE_HEADER.NumberOfSections]
        mov     [ebp+NumberOfSections], cx
        jmp     short .loop
; ---------------------------------------------------------------------------

.NextSection:                            
        mov     eax, [ebp+SectionHeader]
        add     eax, 28h
        mov     [ebp+SectionHeader], eax

.loop:                                  
        movzx   eax, word[ebp+NumberOfSections]
        mov     cx, [ebp+NumberOfSections]
        sub     cx, 1
        mov     [ebp+NumberOfSections], cx
        test    eax, eax
        jz      .return
        mov     eax, [ebp+SectionHeader]
        mov     ecx, FileStart
        add     ecx, [eax+IMAGE_SECTION_HEADER.PointerToRawData]
        mov     [ebp+Source], ecx
        mov     eax, [ebp+OptionalHeader]
        mov     ecx, [eax+IMAGE_OPTIONAL_HEADER.ImageBase]
        mov     edx, [ebp+SectionHeader]
        add     ecx, [edx+IMAGE_SECTION_HEADER.VirtualAddress]
        mov     [ebp+Destination], ecx
        mov     eax, [ebp+SectionHeader]
        mov     ecx, [eax+IMAGE_SECTION_HEADER.VirtualSize]
        mov     [ebp+VirtualSize], ecx
        mov     eax, [ebp+SectionHeader]
        mov     ecx, [eax+IMAGE_SECTION_HEADER.SizeOfRawData]
        mov     [ebp+SizeOfRawData], ecx
        cmp     dword[ebp+VirtualSize], 0
        jnz     short .Check_PointerToRawData
        mov     eax, [ebp+SizeOfRawData]
        mov     [ebp+VirtualSize], eax

.Check_PointerToRawData:                
        mov     eax, [ebp+SectionHeader]
        cmp     dword [eax+14h], 0
        jnz     short .Check_VirtualSize
        mov     dword[ebp+SizeOfRawData], 0
        jmp     short .Check_Start
; ---------------------------------------------------------------------------

.Check_VirtualSize:                    
        mov     eax, [ebp+SizeOfRawData]
        cmp     eax, [ebp+VirtualSize]
        jbe     short .Check_Start
        mov     eax, [ebp+VirtualSize]
        mov     [ebp+SizeOfRawData], eax

.Check_Start:                         
                                        
        mov     eax, [ebp+ImageStart]
        mov     ecx, [ebp+Destination]
        cmp     ecx, [eax]
        jnb     short .Check_End
        mov     eax, [ebp+ImageStart]
        mov     ecx, [ebp+Destination]
        mov     [eax], ecx

.Check_End:                         
        mov     eax, [ebp+Destination]
        add     eax, [ebp+VirtualSize]
        mov     ecx, [ebp+ImageEnd]
        cmp     eax, [ecx]
        jbe     short .CopyData
        mov     eax, [ebp+Destination]
        add     eax, [ebp+VirtualSize]
        mov     ecx, [ebp+ImageEnd]
        mov     [ecx], eax

.CopyData:                            
        cmp     dword[ebp+SizeOfRawData], 0
        jz      short .next
        mov     eax, [ebp+SizeOfRawData]
        push    eax             ; size_t
        mov     ecx, [ebp+Source]
        add     ecx, SU_MODULE_BASE
        push    ecx             ; void *
        mov     edx, [ebp+Destination]
        push    edx             ; void *
        call    memmove
        add     esp, 0Ch

.next:                                  
        jmp     .NextSection
; ---------------------------------------------------------------------------

.return:                               
                
        mov     eax,dword [ebp+OptionalHeader] 
        mov     eax,dword [eax+IMAGE_OPTIONAL_HEADER.ImageBase] 
        mov     dword [BootRecord.OsLoaderBase], eax
        mov     ecx,dword [ebp+OptionalHeader] 
        add     eax,dword [ecx+IMAGE_OPTIONAL_HEADER.AddressOfEntryPoint] 

        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        retn

%include "export.asm"										
%include "data.asm"
								 	
											