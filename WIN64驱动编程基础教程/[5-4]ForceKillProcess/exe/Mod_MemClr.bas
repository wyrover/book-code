Attribute VB_Name = "Mod_MemClr"
Option Explicit

Private Declare Function OpenProcess Lib "kernel32" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function TerminateProcess Lib "kernel32" (ByVal hProcess As Long, ByVal uExitCode As Long) As Long
Private Declare Function ZwClose Lib "NTDLL.DLL" (ByVal ObjectHandle As Long) As Long
Private Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long
Private Declare Function NtUnmapViewOfSection Lib "NTDLL.DLL" (ByVal ProcessHandle As Long, ByVal BaseAddress As Long) As Long
Private Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)
Private Const PAGE_EXECUTE_READWRITE& = &H40
Private Const STANDARD_RIGHTS_REQUIRED = &HF0000
Private Const SYNCHRONIZE = &H100000
Private Const SPECIFIC_RIGHTS_ALL = &HFFFF
Private Const STANDARD_RIGHTS_ALL = &H1F0000
Private Const PROCESS_ALL_ACCESS = STANDARD_RIGHTS_REQUIRED Or SYNCHRONIZE Or &HFFF

Public Sub UnloadNtdll(ByVal Pid As Long)
    Dim hProcess As Long, i As Long, ntst As Long
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, False, Pid)
    If hProcess <= 0 Then
        MsgBox "打开进程失败！"
        Exit Sub
    End If
    ntst = NtUnmapViewOfSection(hProcess, GetModuleHandle("ntdll.dll"))
    'MsgBox "0x" & Hex$(ntst), , "NTSTATUS of NtUnmapViewOfSection"
    ZwClose hProcess
    Sleep 1000
End Sub

Public Sub KillMyProcess(ByVal Pid As Long)
    Dim hProcess As Long
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, False, Pid)
    If hProcess = 0 Then Exit Sub
    TerminateProcess hProcess, 0
    ZwClose hProcess
End Sub
