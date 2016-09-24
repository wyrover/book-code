

.386
.model flat,c

apiHookProc PROTO stdcall lpCallRet:PTR,lpRet:PTR,lpEsp:PTR
apiHookProcNull PROTO stdcall lpCallRet:PTR,lpRet:PTR,lpEsp:PTR
Hook_KiUserExceptionDispatcher PROTO stdcall arg1:PTR,arg2:PTR
OEPEntryCallBack PROTO stdcall lpEsp:PTR
HookedThreadCreate PROTO stdcall lpEntry:PTR
HookedProcessExit PROTO stdcall hProcess:PTR,dwExitCode:PTR
checkForPendingPacket PROTO  stdcall lpCallRet:PTR,lpRet:PTR,lpEsp:PTR

extern glpSysEnter:FAR
extern glpThreadEntry:FAR
extern glpProcessExit:FAR
extern glpOrgEntry:FAR
extern gSyscStkDelta:FAR
extern glpTIBSysCallOrg:FAR
extern glpFastSystemCallOrg:FAR
extern glpIntSystemCallOrg:FAR


snapshotEntryProc PROTO stdcall lpRet:PTR,lpEsp:PTR
.code

apiStubEntry proc
    pushfd
    pushad
	cld
    push esp
    push [esp+028h]
    push [esp+030h]
    call apiHookProc
apiStubEntry endp

apiStubEntryNull proc
    pushfd
    pushad
	cld
    push esp
    push [esp+028h]
    push [esp+030h]
    call apiHookProcNull
apiStubEntryNull endp

nullTIBSyscallRet proc
	jmp dword ptr ss:[esp]
nullTIBSyscallRet endp

nullTIBSyscallRetDelta4	proc
	ret
nullTIBSyscallRetDelta4 endp

SyscallRetRedirect proc
	sub esp,dword ptr ds:[gSyscStkDelta]
	jmp dword ptr ds:[glpTIBSysCallOrg]
SyscallRetRedirect endp

fastSyscallRetRedirect proc
	sub esp,4
	jmp dword ptr ds:[glpFastSystemCallOrg]
fastSyscallRetRedirect endp
intSyscallRetRedirect proc
	sub esp,4
	jmp dword ptr ds:[glpIntSystemCallOrg]
intSyscallRetRedirect endp

nullSyscallRet proc
	ret
nullSyscallRet endp

apiOEPEntry proc
    push eax
    push eax
    pushfd
    pushad
	cld
    push esp
    call OEPEntryCallBack
apiOEPEntry endp

snapshotEntry proc
    push eax
    pushfd
    pushad
	cld
    push esp
    push [esp+028h]
    call snapshotEntryProc
snapshotEntry endp
apiStubEnd proc lpesp:PTR,retaddr:PTR
        mov eax,retaddr
        mov esp,lpesp
        mov dword ptr[esp+024h],eax
        popad
        popfd
        db 0C3h
		ret
apiStubEnd endp

Hooked_KiUserExceptionDispatcherEntry proc
    pushfd
    pushad
	cld
    push [esp+028h]
    push [esp+028h]
    call Hook_KiUserExceptionDispatcher
    mov [esp+01Ch],eax
    popad
    popfd
    jmp eax
Hooked_KiUserExceptionDispatcherEntry endp
foreverLoop proc
    jmp $
    ret
foreverLoop endp
setProcessHeap proc lpHeap:PTR
    xor eax,eax
    assume fs:nothing
    mov eax,fs:[eax+018h]
    mov eax,[eax+030h]
    mov ecx,lpHeap
    mov [eax+018h],ecx
    mov eax,ecx
    ret
setProcessHeap endp


getThreadTEB proc
    xor eax,eax
    assume fs:nothing
    mov eax,fs:[eax+018h]
    ret
getThreadTEB endp

getPEB proc
    xor eax,eax
    assume fs:nothing
    mov eax,fs:[eax+018h]
    mov eax,[eax+030h]
    ret
getPEB endp

getMainModuleHandle proc
    xor eax,eax
    assume fs:nothing
    mov eax,fs:[eax+018h]
    mov eax,[eax+030h]
    mov eax,[eax+8h]
    ret
getMainModuleHandle endp

getTIBSystemCall proc
    xor eax,eax
    assume fs:nothing
    mov eax,fs:[eax+0C0h]
    ret
getTIBSystemCall endp

clearDebugPresentFlag proc
	xor eax,eax
	assume fs:nothing
	mov eax,dword ptr fs:[018h]
	mov eax,dword ptr [eax+030h]
	mov byte ptr [eax+02h],0
	retn
clearDebugPresentFlag endp

sysEnterStubEntry proc
    pushfd
    pushad
	cld
    push esp
    push [esp+028h]
    push [esp+030h]
    call apiHookProc
sysEnterStubEntry endp

HookSysEnterStub proc
    xor eax,eax
    assume fs:nothing
    mov eax,fs:[eax+0C0h]
    mov dword ptr ds:[glpSysEnter],eax
    mov fs:[eax+0C0h],sysEnterStubEntry
HookSysEnterStub endp

sysenterHookStub proc
    jmp dword ptr ds:[glpSysEnter]
sysenterHookStub endp

sysInt3BreakPoint proc
    int 3
    ret
sysInt3BreakPoint endp

sysBreakBeforeEntry proc
    push dword ptr ds:[glpOrgEntry]
    jmp sysInt3BreakPoint
sysBreakBeforeEntry endp

TEBGetThreadId proc
    xor eax,eax
    assume fs:nothing
    mov eax, fs:[eax+18h]
    mov eax, [eax+ 24h]
    ret
TEBGetThreadId endp

TEBGetProcessId proc
    xor eax,eax
    assume fs:nothing
    mov eax, fs:[eax+18h]
    mov eax, [eax+ 20h]
    ret
TEBGetProcessId endp


HookedThreadEntry proc
    pushfd
    pushad
	cld
    test ecx,ecx
    jnz $outhand
    push dword ptr ds:[esp+04h]
    call HookedThreadCreate
    $outhand:
    popad
    popfd
    push dword ptr ds:[glpThreadEntry]
    ret
HookedThreadEntry endp

HookedProcessExitEntry proc
    pushfd
    pushad
	cld
    push dword ptr ds:[esp+02Ch]
    push dword ptr ds:[esp+02Ch]
    call HookedProcessExit
    popad
    popfd
    push dword ptr ds:[glpProcessExit]
    ret
HookedProcessExitEntry endp

checkPendingPacketEntry proc
	pushfd
	pushad
	cld
	push esp
    push [esp+028h]
    push [esp+030h]
	call checkForPendingPacket
checkPendingPacketEntry endp

checkPendingPacketOuter proc lpesp:PTR,retaddr:PTR,realEsp:PTR
    mov eax,retaddr
	cmp eax,0
	jz lab_getouter
	xor ebx,ebx
	assume fs:nothing
	mov fs:[ebx+0110h],eax
	mov ecx,realEsp
	mov esp,lpesp
	mov dword ptr[esp+024h],ecx
    mov dword ptr[esp+028h],eax
	popad
    popfd
    pop esp
lab_outer:
    jmp dword ptr fs:[0110h]
	ret
lab_getouter:
	mov eax,lab_outer
	ret
checkPendingPacketOuter endp

setPendingPacketExecState proc state:DWORD
	push ebx
	xor ebx,ebx
	assume fs:nothing
	mov eax,state
	mov fs:[ebx+0118h],eax
	pop ebx
	ret
setPendingPacketExecState endp


getSysCallStackDelta proc syscaddr:PTR
	push ecx
	push ebx
	mov ebx,syscaddr
	push 0
	push lab_out
	push lab_out
	push lab_out
	push lab_out
	xor eax,eax
	xor ecx,ecx
	jmp ebx
lab_out:
	mov eax,10h
lab_loop:
	cmp dword ptr ss:[esp],0
	jz lab_end
	sub eax,4
	add esp,4
	jmp lab_loop
lab_end:
	add esp,4
	pop ebx
	pop ecx
	ret
getSysCallStackDelta endp

end

