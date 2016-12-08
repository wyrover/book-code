.586
.model flat,stdcall
option casemap:none
include windows.inc
include user32.inc
include TestAsm.inc
includelib user32.lib
.data
szText db 'Hello, word', 0
szCatption db 'win32汇编', 0
szBuffer db 32 dup (0h)
.code

start:
    invoke MessageBox, NULL, offset szText, offset szCatption, MB_OK
    ; 实现一个strlen
    push offset szText
    call myStrLen_1
    push offset szText
    push offset szBuffer
    call mystrcopy_1
    ret

myStrLen proc lpStart:DWORD
    .if lpStart == 0
        mov eax, 0
        ret
    .endif
    mov eax, lpStart
    mov ecx, 0
WHILEBEGIN:
    mov bl, byte ptr [eax]
    cmp bl, 0
    jz WHILEEND
    add ecx, 1
    add eax, 1
    jmp WHILEBEGIN
WHILEEND:
    mov eax, ecx
    ret

myStrLen endp

myStrLen_1 proc lpStart:DWORD
    pushad
    .if lpStart == 0
        mov eax, 0
        ret
    .endif
    mov edi, lpStart
    xor eax,eax
    mov ecx, 0
    dec ecx
    repnz scasb
    not ecx
    dec ecx
    mov eax, ecx
    popfd
    ret
myStrLen_1 endp

mystrcopy proc lpDes:DWORD, lpSou:DWORD
    .if lpDes == 0
        ret
    .endif
    .if lpSou == 0
        ret
    .endif
    
    mov eax, lpSou
    mov edi, lpDes
    mov ecx, 0
WHILEBEGIN:
    mov bl, byte ptr [eax]
    cmp bl, 0
    jz WHILEEND
    mov byte ptr [edi],bl 
    inc edi
    add ecx, 1
    add eax, 1
    jmp WHILEBEGIN
WHILEEND:

    ret

mystrcopy endp

mystrcopy_1 proc lpDes:DWORD, lpSou:DWORD
    .if lpDes == 0
        ret
    .endif
    .if lpSou == 0
        ret
    .endif
    
    mov esi, lpSou
    mov edi, lpDes
    push lpSou
    call myStrLen_1
    mov ecx, eax
    rep movsb

    ret

mystrcopy_1 endp

end start 