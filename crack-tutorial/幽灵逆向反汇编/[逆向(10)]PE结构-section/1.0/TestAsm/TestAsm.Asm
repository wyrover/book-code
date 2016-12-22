.586
.model flat,stdcall
option casemap:none
include windows.inc
include kernel32.inc
include user32.inc
include TestAsm.inc
include msvcrt.inc
includelib user32.lib
includelib kernel32.lib
includelib msvcrt.lib
.data
szFormat db '节区名称:%s, 节区RVA:0x%08x, 节区RVA Size:0x%08x, 节区RAW:0x%08x, 节区RAW Size:0x%08x',0
.code
szFileName db '1.exe', 0
start:
    
    call ReadPe
    ret
ReadPe proc
    LOCAL @hFile:DWORD
    LOCAL @pFileMemory:DWORD
    LOCAL @lpNumberOfBytesRead:DWORD
    LOCAL @dwSize:DWORD
    LOCAL @dwNumberOfSections:DWORD
    LOCAL @szSectionName[8]:BYTE
    LOCAL @Buffer[256]:BYTE
    LOCAL @SectionRva:DWORD
    LOCAL @SectionRvaSize:DWORD
    LOCAL @SectionRaw:DWORD
    LOCAL @SectionRawSize:DWORD
    

    ; 1.打开文件
    invoke CreateFile, offset szFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    mov @hFile, eax
    .if @hFile == INVALID_HANDLE_VALUE
        ret
    .endif
    ; 2.把文件整个读入内存中
    invoke GetFileSize, @hFile, NULL
    push eax
    .if eax == 0
        jmp CLOSEFILE
    .endif
    invoke VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
    mov @pFileMemory, eax
    .if @pFileMemory == NULL
        
        jmp CLOSEFILE
    .endif
    pop eax
    push eax
    invoke RtlZeroMemory, @pFileMemory, eax
    pop eax
    mov edi, eax
    mov @dwSize, edi
    mov @lpNumberOfBytesRead, 0
    invoke ReadFile, @hFile, @pFileMemory, edi, addr @lpNumberOfBytesRead, NULL
    .if eax == FALSE
        jmp FREE
        ret
    .endif
    
    mov edi, @pFileMemory
    assume edi:ptr IMAGE_DOS_HEADER
    ; 判断是否为MZ头部
    .if [edi].e_magic != 05a4dh
        jmp FREE
    .endif
    ; 获得PE头部
    mov eax, [edi].e_lfanew
    add eax, @pFileMemory
    mov edi, eax
    assume edi:ptr IMAGE_NT_HEADERS
    ; 判断是否是PE头部
    .if [edi].Signature != 04550h
        jmp FREE
    .endif
    movzx eax, word ptr [edi].FileHeader.NumberOfSections
    mov @dwNumberOfSections, eax
    xor eax, eax
    add eax, sizeof DWORD
    add eax, sizeof IMAGE_FILE_HEADER
    movzx ebx,[edi].FileHeader.SizeOfOptionalHeader
    add eax, ebx
    add edi, eax
    ; 获得节区首地址
    assume edi:ptr IMAGE_SECTION_HEADER
    xor eax, eax
    .while eax < @dwNumberOfSections
        ; 获得节区名称
        push eax
        lea ecx, [edi].Name1
        invoke crt_memcpy, addr @szSectionName, ecx, 8
        mov eax, [edi].VirtualAddress
        mov @SectionRva, eax
        
        mov eax, [edi].Misc.VirtualSize
        mov @SectionRvaSize, eax
        
        mov eax, [edi].PointerToRawData
        mov @SectionRaw, eax
        
        mov eax, [edi].SizeOfRawData
        mov @SectionRawSize, eax
        
        invoke crt_sprintf, addr @Buffer, offset szFormat, \
            addr @szSectionName, @SectionRva, @SectionRvaSize, \
            @SectionRaw, @SectionRawSize
        invoke MessageBox, NULL, addr @Buffer, NULL, MB_OK
        add edi, sizeof IMAGE_SECTION_HEADER
        pop eax
        inc eax
    .endw
    
   
FREE:
    invoke VirtualFree, @pFileMemory, @dwSize, MEM_DECOMMIT
CLOSEFILE:
    invoke CloseHandle, @hFile
    ret
    
    ret

ReadPe endp
end start 