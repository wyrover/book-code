;msgbx.asm file.
.386p
.model flat, stdcall
option casemap:none

include \masm32\include\windows.inc
include \masm32\include\user32.inc
includelib \masm32\lib\user32.lib 

.code

start:
    push MB_ICONINFORMATION or MB_OK
    call Func1
    db "Test",0
Func1:
    call Func2
    db "Hello",0
Func2:
    push NULL    
    call MessageBoxA
;    ret
end start
