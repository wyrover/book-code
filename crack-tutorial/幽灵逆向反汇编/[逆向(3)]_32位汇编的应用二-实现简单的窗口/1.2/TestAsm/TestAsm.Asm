.386
.model flat,stdcall
option casemap:none
include windows.inc
include kernel32.inc
include	gdi32.inc
include user32.inc
include TestAsm.inc
includelib user32.lib
includelib kernel32.lib
includelib	gdi32.lib

.data
szWindowClass db 'MyClass', 0
szWindowName db 'First Windows', 0
szWindowNameEx db 'Windows', 0
hInstance dd ?
hWnd dd ?
.code

start:
    call WinMain
    invoke ExitProcess, NULL
    ret
WinMain proc
    LOCAL @wcex:WNDCLASSEX
    LOCAL @msg:MSG
    
    
    invoke RtlZeroMemory, addr @wcex, sizeof WNDCLASSEX
    mov @wcex.cbSize, sizeof WNDCLASSEX
    mov @wcex.style, CS_HREDRAW or CS_VREDRAW
    mov @wcex.lpfnWndProc, offset WndProc
    invoke GetModuleHandle, NULL
    mov hInstance, eax
    mov @wcex.hInstance, eax
    invoke LoadCursor, 0, IDC_ARROW
    mov @wcex.hCursor, eax
    mov @wcex.hbrBackground, COLOR_WINDOW+1
    mov @wcex.lpszClassName, offset szWindowClass
    
    invoke RegisterClassEx, addr @wcex

    invoke CreateWindowEx, WS_EX_CLIENTEDGE, offset szWindowClass, \ 
        offset szWindowNameEx, \
        WS_OVERLAPPEDWINDOW, \
        100,100,600,400, NULL, NULL, hInstance, NULL
    mov hWnd, eax
    .if hWnd == 0
        ret
    .endif
    
    invoke ShowWindow, hWnd, SW_SHOWNORMAL
    invoke UpdateWindow, hWnd
    
    .while TRUE
        invoke GetMessage, addr @msg, NULL, 0, 0
        .break .if eax == 0
        invoke TranslateMessage, addr @msg
        invoke DispatchMessage, addr @msg   
    .endw

    
    ret
WinMain endp

WndProc proc hwnd:DWORD, uMsg:DWORD, wParam:DWORD, lParam:DWORD
    LOCAL @lpPaint:PAINTSTRUCT
    LOCAL @hDc
    LOCAL @lpRect:RECT
    mov eax, uMsg
    .if eax == WM_PAINT
        invoke BeginPaint, hwnd, addr @lpPaint
        mov @hDc, eax
        
        invoke GetClientRect, hwnd, addr @lpRect
        invoke DrawText, @hDc, addr szWindowName, -1, addr @lpRect, \
            DT_SINGLELINE or DT_CENTER or DT_VCENTER
        invoke EndPaint, hwnd, addr @lpPaint
    .elseif eax == WM_CLOSE
        invoke DestroyWindow, hWnd
        invoke PostQuitMessage, NULL
    .else
        invoke DefWindowProc, hwnd, uMsg, wParam, lParam
        ret
    .endif
    xor eax, eax
    ret

WndProc endp
end start 