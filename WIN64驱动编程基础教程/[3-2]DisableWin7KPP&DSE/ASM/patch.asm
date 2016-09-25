; this creates patched copies of files %SystemRoot%\system32\winload.exe, %SystemRoot%\system32\ntoskrnl.exe, %SystemRoot%\system32\ntkrnlmp.exe


format PE64 console at (1 shl 32)
entry start


include '%fasminc%\win64a.inc'


section '.code' code readable executable


align 10h
start:

	push	rbx
	sub	rsp,8*(4+2)

	lea	rcx,[file0]
	call	read_system_file
	or	eax,eax
	mov	ebx,eax				; size of file
	jz	exit_failed
	mov	edx,ebx				; size of the whole file
	lea	rcx,[file0ed]			; pointer to patching data
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file3ed]
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file3ed]
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file3ed]
	call	patch_bytes
	mov	ecx,ebx				; size of the whole file
	call	reconstruct_crc
	mov	edx,ebx				; size of the whole file
	lea	rcx,[file0n]
	call	write_system_file
	cmp	eax,ebx
	jnz	exit_delete_file0n

	lea	rcx,[file1]
	call	read_system_file
	or	eax,eax
	mov	ebx,eax
	jnz	L0
	lea	rcx,[file2]
	call	read_system_file
	or	eax,eax
	mov	ebx,eax
	jz	exit_failed
L0:	mov	edx,ebx
	lea	rcx,[file1ed]
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file2ed]
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file3ed]
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file3ed]
	call	patch_bytes
	mov	edx,ebx
	lea	rcx,[file3ed]
	call	patch_bytes
	mov	ecx,ebx
	call	reconstruct_crc
	mov	edx,ebx
	lea	rcx,[file1n]
	call	write_system_file
	cmp	eax,ebx
	jz	exit_success
;	jmp	exit_delete_file1n_file0n

exit_delete_file1n_file0n:
	lea	rcx,[file1n]
	call	[DeleteFileA]

exit_delete_file0n:
	lea	rcx,[file0n]
	call	[DeleteFileA]

exit_failed:
	lea	rbx,[msg_failed]
	jmp	exit_msg

exit_success:
	lea	rbx,[msg_success]

exit_msg:
;STD_OUTPUT_HANDLE		= -11
;INVALID_HANDLE_VALUE		= -1
;	mov	rcx,STD_OUTPUT_HANDLE
	push	STD_OUTPUT_HANDLE
	pop	rcx
	call	[GetStdHandle]
	push	rax
	pop	rcx
if INVALID_HANDLE_VALUE = -1
	inc	rax
else
	cmp	rax,INVALID_HANDLE_VALUE
end if
	jz	exit

	and	qword [rsp + 8*(4+0)],0
	lea	r9,[rsp + 8*(4+1)]
	mov	r8d,msg_size
;	lea	rdx,[rbx]
	push	rbx
	pop	rdx
;	lea	rcx,[rcx]	; already set
	call	[WriteFile]

exit:	xor	eax,eax
	add	rsp,8*(4+2)
	pop	rbx
	ret


align 10h
read_system_file:
;  in: rcx pointer to filename
; out: eax file size (0 for error)
	push	rbx rbp rsi rdi
	sub	rsp,8*(4+3)

	xor	ebp,ebp				; result
	push	rcx
	pop	rsi				; filename

	lea	rdi,[filename]
	mov	ebx,MAX_PATH

	mov	edx,ebx
	push	rdi
	pop	rcx
	call	[GetSystemDirectoryA]
	cmp	eax,ebx
	jnbe	read_system_file_finish

	lea	rbx,[rdi+rax*1]

;	mov	eax,'\'
; this is 2 bytes smaller:
	push	'\'
	pop	rax

	mov	[rbx],al
	cmp	[rbx-1],al
	setnz	al
	add	rbx,rax

	movdqu	xmm0,dqword [rsi]
	movdqu	[rbx],xmm0

;	and	qword [rsp+8*(4+2)],0				; hTemplateFile
	mov	dword [rsp+8*(4+1)],FILE_ATTRIBUTE_NORMAL	; dwFlagsAndAttributes
	mov	dword [rsp+8*(4+0)],OPEN_EXISTING		; dwCreationDistribution
	xor	r9,r9						; lpSecurityAttributes
	xor	r8,r8						; dwShareMode
	mov	edx,GENERIC_READ				; dwDesiredAccess
	push	rdi
	pop	rcx						; lpFileName
	mov	[rsp+8*(4+2)],r9				; hTemplateFile
	call	[CreateFileA]

	push	rax
	pop	rsi

if INVALID_HANDLE_VALUE = -1
	inc	rax
else
	cmp	rax,INVALID_HANDLE_VALUE
end if
	jz	read_system_file_finish

	mov	edi,file_buf_size

	and	qword [rsp + 8*(4+0)],0		; lpOverlapped
	lea	r9,[rsp + 8*(4+2)]		; lpNumberOfBytesRead
	mov	r8d,edi				; nNumberOfBytesToRead
	lea	rdx,[file_buf]
	push	rsi
	pop	rcx				; hFile
	call	[ReadFile]
	or	eax,eax
	jz	read_system_file_close_finish

	mov	ebp,[rsp + 8*(4+2)]		; NumberOfBytesRead

read_system_file_close_finish:
	push	rsi
	pop	rcx
	call	[CloseHandle]

read_system_file_finish:
	mov	eax,ebp				; result into EAX
	add	rsp,8*(4+3)
	pop	rdi rsi rbp rbx
	ret


align 10h
patch_bytes:
	push	rbx rsi rdi
	push	rcx
	pop	rsi
	cld
	lodsb					; load size of bytes to find
	movzx	ebx,al
	lea	rdi,[file_buf]
	lea	r8,[rdi+rdx]			; end of file

align 10h
patch_bytes_L0:
	push	rsi rdi
	mov	ecx,ebx
	repz cmpsb
	pop	rdi rsi
	jz	patch_bytes_L4
	scasb					; rdi+1 in 1-byte instruction
	lea	rax,[rdi+rbx*1]
	cmp	rax,r8
	jbe	patch_bytes_L0
; end of file reached
	jmp	patch_bytes_L9

patch_bytes_L4:
; matching bytes found, patch them
	add	rsi,rbx
	lodsb					; load size of bytes to be written
	movzx	ecx,al
	repz movsb
patch_bytes_L9:
	pop	rdi rsi rbx
	ret


align 10h
reconstruct_crc:
; in: ecx = size of file

struct	IMAGE_DOS_HEADER
	e_magic				rw	1	; Magic number
	e_cblp				rw	1	; Bytes on last page of file
	e_cp				rw	1	; Pages in file
	e_crlc				rw	1	; Relocations
	e_cparhdr			rw	1	; Size of header in paragraphs
	e_minalloc			rw	1	; Minimum extra paragraphs needed
	e_maxalloc			rw	1	; Maximum extra paragraphs needed
	e_ss				rw	1	; Initial (relative) SS value
	e_sp				rw	1	; Initial SP value
	e_csum				rw	1	; Checksum
	e_ip				rw	1	; Initial IP value
	e_cs				rw	1	; Initial (relative) CS value
	e_lfarlc			rw	1	; File address of relocation table
	e_ovno				rw	1	; Overlay number
	e_res				rw	4	; Reserved words
	e_oemid				rw	1	; OEM identifier (for e_oeminfo)
	e_oeminfo			rw	1	; OEM information; e_oemid specific
	e_res2				rw	10	; Reserved words
	e_lfanew			rd	1	; File address of new exe header
ends
IMAGE_DOS_SIGNATURE			=	'MZ'

struct	IMAGE_FILE_HEADER
	Machine				rw	1
	NumberOfSections		rw	1
	TimeDateStamp			rd	1
	PointerToSymbolTable		rd	1
	NumberOfSymbols			rd	1
	SizeOfOptionalHeader		rw	1
	Characteristics			rw	1
ends
IMAGE_SIZEOF_FILE_HEADER		=	sizeof.IMAGE_FILE_HEADER	; = 20
IMAGE_FILE_MACHINE_AMD64		=	8664h	; AMD64 (K8)

struct	IMAGE_DATA_DIRECTORY
	VirtualAddress			rd	1
	Size				rd	1
ends
IMAGE_NUMBEROF_DIRECTORY_ENTRIES	=	16

struct	IMAGE_OPTIONAL_HEADER64
; Standard fields.
	Magic				rw	1
	MajorLinkerVersion		rb	1
	MinorLinkerVersion		rb	1
	SizeOfCode			rd	1
	SizeOfInitializedData		rd	1
	SizeOfUninitializedData		rd	1
	AddressOfEntryPoint		rd	1
	BaseOfCode			rd	1
; NT additional fields.
	ImageBase			rq	1
	SectionAlignment		rd	1
	FileAlignment			rd	1
	MajorOperatingSystemVersion	rw	1
	MinorOperatingSystemVersion	rw	1
	MajorImageVersion		rw	1
	MinorImageVersion		rw	1
	MajorSubsystemVersion		rw	1
	MinorSubsystemVersion		rw	1
	Win32VersionValue		rd	1
	SizeOfImage			rd	1
	SizeOfHeaders			rd	1
	CheckSum			rd	1
	Subsystem			rw	1
	DllCharacteristics		rw	1
	SizeOfStackReserve		rq	1
	SizeOfStackCommit		rq	1
	SizeOfHeapReserve		rq	1
	SizeOfHeapCommit		rq	1
	LoaderFlags			rd	1
	NumberOfRvaAndSizes		rd	1
;	IMAGE_DATA_DIRECTORY	DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	DataDirectory			IMAGE_DATA_DIRECTORY
					rb	(IMAGE_NUMBEROF_DIRECTORY_ENTRIES-1)*(sizeof.IMAGE_DATA_DIRECTORY)
ends
IMAGE_SIZEOF_NT_OPTIONAL64_HEADER	=	240
IMAGE_NT_OPTIONAL_HDR64_MAGIC		=	020Bh

struct	IMAGE_NT_HEADERS64
	Signature			rd	1
	FileHeader			IMAGE_FILE_HEADER
	OptionalHeader			IMAGE_OPTIONAL_HEADER64
ends
IMAGE_NT_SIGNATURE			=	'PE'


	lea	rdx,[file_buf]
	cmp	[rdx+IMAGE_DOS_HEADER.e_magic],IMAGE_DOS_SIGNATURE
	jnz	reconstruct_crc_L9
	mov	eax,[rdx+IMAGE_DOS_HEADER.e_lfanew]
	add	rax,rdx
	cmp	[rax+IMAGE_NT_HEADERS64.Signature],IMAGE_NT_SIGNATURE
	jnz	reconstruct_crc_L9
	cmp	[rax+IMAGE_NT_HEADERS64.FileHeader.Machine],IMAGE_FILE_MACHINE_AMD64
	jnz	reconstruct_crc_L9
	cmp	[rax+IMAGE_NT_HEADERS64.OptionalHeader.Magic],IMAGE_NT_OPTIONAL_HDR64_MAGIC
	jnz	reconstruct_crc_L9

; CRC
; erase original file crc
	and	[rax+IMAGE_NT_HEADERS64.OptionalHeader.CheckSum],0
; ecx = size
; rdx = file_buf
	mov	r10d,ecx
	shr	ecx,1
	xor	r9d,r9d
	xor	r8d,r8d
calculate_checksum:
	mov	r9w,[rdx]
	add	r8d,r9d
	mov	r9w,r8w
	shr	r8d,16
	add	r8d,r9d
	add	rdx,2
	loop	calculate_checksum
	add	r8d,r10d
	mov	[rax+IMAGE_NT_HEADERS64.OptionalHeader.CheckSum],r8d
; Checksum done

reconstruct_crc_L9:
	ret


write_system_file:
;  in: rcx pointer to filename
;      edx file size (0 for error)
; out: eax file size (0 for error)
	push	rbx rbp rsi rdi
	sub	rsp,8*(4+3)

	mov	ebx,edx				; size of file
	xor	ebp,ebp				; result
	push	rcx
	pop	rsi				; filename

	lea	rdi,[filename]

	mov	edx,MAX_PATH
	push	rdi
	pop	rcx
	call	[GetSystemDirectoryA]
	cmp	eax,MAX_PATH
	jnbe	read_system_file_finish

	lea	rdx,[rdi+rax*1]

;	mov	eax,'\'
; this is 2 bytes smaller:
	push	'\'
	pop	rax

	mov	[rdx],al
	cmp	[rdx-1],al
	setnz	al
	add	rdx,rax

	movdqu	xmm0,dqword [rsi]
	movdqu	[rdx],xmm0

;	and	qword [rsp+8*(4+2)],0				; hTemplateFile
	mov	dword [rsp+8*(4+1)],FILE_ATTRIBUTE_NORMAL	; dwFlagsAndAttributes
	mov	dword [rsp+8*(4+0)],CREATE_ALWAYS		; dwCreationDistribution
	xor	r9,r9						; lpSecurityAttributes
	xor	r8,r8						; dwShareMode
	mov	edx,GENERIC_WRITE				; dwDesiredAccess
;	lea	rcx,[rcx]					; lpFileName
	mov	[rsp+8*(4+2)],r9				; hTemplateFile
	call	[CreateFileA]

	push	rax
	pop	rsi

if INVALID_HANDLE_VALUE = -1
	inc	rax
else
	cmp	rax,INVALID_HANDLE_VALUE
end if
	jz	write_file_finish

	and	qword [rsp + 8*(4+0)],0		; lpOverlapped
	lea	r9,[rsp + 8*(4+2)]		; lpNumberOfBytesWritten
	mov	r8d,ebx				; nNumberOfBytesToWrite
	lea	rdx,[file_buf]
	push	rsi
	pop	rcx				; hFile
	call	[WriteFile]
	or	eax,eax
	jz	write_file_close_finish

	mov	ebp,[rsp + 8*(4+2)]		; NumberOfBytesWritten

write_file_close_finish:
	push	rsi
	pop	rcx
	call	[CloseHandle]

write_file_finish:
	mov	eax,ebp				; result into EAX
	add	rsp,8*(4+3)
	pop	rdi rsi rbp rbx
	ret

file0	db	'winload.exe',0
file0n	db	'freeload.exe',0
file1	db	'ntkrnlmp.exe',0		; kernel for SMP system (multiprocessor)
file1n	db	'goodkrnl.exe',0
file2	db	'ntoskrnl.exe',0		; kernel for 1 CPU system
; data to patch are the same as for file1 as for file2

file0ed	db	file0r_size
	file0r:
	db	 48h, 8Bh,0C4h, 53h, 55h, 57h, 41h, 54h	; bytes to find
	file0r_size = $ - file0r
	db	file0w_size
	file0w:
	mov	al,1
	ret
	; nop ; not necessary
	file0w_size = $ - file0w
file1ed	db	file1r_size
	file1r:
	db	 0Fh, 85h, 94h, 00h, 00h, 00h, 33h,0C0h	; bytes to find
	file1r_size = $ - file1r
	db	file1w_size
	file1w:
	nop
	db	0E9h				; opcode for jmp
	file1w_size = $ - file1w
file2ed	db	file2r_size
	file2r:
	db	 74h, 07h,0B0h, 01h,0E9h, 68h, 2Dh, 00h	; bytes to find
	file2r_size = $ - file2r
	db	file2w_size
	file2w:
	nop
	nop
	file2w_size = $ - file2w
file3ed	db	file3r_size
	file3r:
;nt!MiIsAddressValid:
;fffff800`01707e20 488bc1          mov     rax,rcx
;fffff800`01707e23 48c1f830        sar     rax,30h
;fffff800`01707e27 48ffc0          inc     rax
;fffff800`01707e2a 4883f801        cmp     rax,1
;fffff800`01707e2e 0f878f000000    ja      nt!MiIsAddressValid+0xa3 (fffff800`01707ec3)
;nt!MmIsSpecialPoolAddress+0x23:
;fffff800`0169fbff 488bc2          mov     rax,rdx
;fffff800`0169fc02 48c1f830        sar     rax,30h
;fffff800`0169fc06 48ffc0          inc     rax
;fffff800`0169fc09 4883f801        cmp     rax,1
;fffff800`0169fc0d 7761            ja      nt!MmIsSpecialPoolAddress+0x94 (fffff800`0169fc70)
;nt!MmAccessFault+0x29:
;fffff800`016e8c09 4c8bea          mov     r13,rdx
;fffff800`016e8c0c 48c1f830        sar     rax,30h
;fffff800`016e8c10 4c8bf9          mov     r15,rcx
;fffff800`016e8c13 48ffc0          inc     rax
;fffff800`016e8c16 4883f801        cmp     rax,1
;fffff800`016e8c1a 0f871c3a0600    ja      nt! ?? ::FNODOBFM::`string'+0x40990 (fffff800`0174c63c)
	sar	rax,48			; mistake, noncanonical addresses like 0000800000000000h, FFFF7FFFFFFFFFFFh may pass the check
;	inc	rax
;	cmp	rax,1
;	jnbe	noncanonical_va		; bytes to find
	file3r_size = $ - file3r
	db	file3w_size
	file3w:
	sar	rax,47			; virtual memory is in canonical form if bit 47 is sign extended into bits 63-48
	file3w_size = $ - file3w

msg_success	db	'SUCCESS',0Ah
msg_size	=	$ - msg_success
msg_failed	db	'FAILED!',0Ah
a		=	$ - msg_failed
if a > msg_size
msg_size = a
end if


section '.idata' import data readable writeable

				dd	0,0,0,RVA kernel_name,	RVA kernel_table
				dd	0,0,0,0,0

kernel_table:
CloseHandle			dq	RVA _CloseHandle
CreateFileA			dq	RVA _CreateFileA
DeleteFileA			dq	RVA _DeleteFileA
GetStdHandle			dq	RVA _GetStdHandle
GetSystemDirectoryA		dq	RVA _GetSystemDirectoryA
ReadFile			dq	RVA _ReadFile
WriteFile			dq	RVA _WriteFile
				dq	0

kernel_name			db	'KERNEL32.DLL',0

; kernel32.dll:
_CloseHandle			db	0,0,'CloseHandle',0
_CreateFileA			db	0,0,'CreateFileA',0
_DeleteFileA			db	0,0,'DeleteFileA',0
_GetStdHandle			db	0,0,'GetStdHandle',0
_GetSystemDirectoryA		db	0,0,'GetSystemDirectoryA',0
_ReadFile			db	0,0,'ReadFile',0
_WriteFile			db	0,0,'WriteFile',0


section '.data' data readable writeable

filename			rb	(MAX_PATH+12+16)

file_buf_size			=	16*1024*1024		; 16 MB should be enough
file_buf			rb	file_buf_size
