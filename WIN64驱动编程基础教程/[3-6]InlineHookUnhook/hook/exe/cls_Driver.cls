VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cls_Driver"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
  
Private Declare Function OpenSCManager Lib "advapi32.dll" Alias "OpenSCManagerA" (ByVal lpMachineName As String, ByVal lpDatabaseName As String, ByVal dwDesiredAccess As Long) As Long
Private Declare Function OpenService Lib "advapi32.dll" Alias "OpenServiceA" (ByVal hSCManager As Long, ByVal lpServiceName As String, ByVal dwDesiredAccess As Long) As Long
Private Declare Function StartService Lib "advapi32.dll" Alias "StartServiceA" (ByVal hService As Long, ByVal dwNumServiceArgs As Long, ByVal lpServiceArgVectors As Long) As Long
Private Declare Function CreateService Lib "advapi32.dll" Alias "CreateServiceA" (ByVal hSCManager As Long, ByVal lpServiceName As String, ByVal lpDisplayName As String, ByVal dwDesiredAccess As Long, ByVal dwServiceType As Long, ByVal dwStartType As Long, ByVal dwErrorControl As Long, ByVal lpBinaryPathName As String, ByVal lpLoadOrderGroup As Long, ByVal lpdwTagId As Long, ByVal lpDependencies As Long, ByVal lp As Long, ByVal lpPassword As Long) As Long
Private Declare Function ControlService Lib "advapi32.dll" (ByVal hService As Long, ByVal dwControl As Long, ByVal lpServiceStatus As Long) As Long
Private Declare Function DeleteService Lib "advapi32.dll" (ByVal hService As Long) As Long
Private Declare Function CloseServiceHandle Lib "advapi32.dll" (ByVal hSCObject As Long) As Long
  
Private Const SC_MANAGER_CONNECT = &H1
Private Const SC_MANAGER_CREATE_SERVICE = &H2
Private Const SC_MANAGER_ENUMERATE_SERVICE = &H4
Private Const SC_MANAGER_LOCK = &H8
Private Const SC_MANAGER_QUERY_LOCK_STATUS = &H10
Private Const SC_MANAGER_MODIFY_BOOT_CONFIG = &H20
Private Const STANDARD_RIGHTS_REQUIRED = &HF0000
Private Const SC_MANAGER_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED Or SC_MANAGER_CONNECT Or SC_MANAGER_CREATE_SERVICE Or SC_MANAGER_ENUMERATE_SERVICE Or SC_MANAGER_LOCK Or SC_MANAGER_QUERY_LOCK_STATUS Or SC_MANAGER_MODIFY_BOOT_CONFIG)
  
Private Const SERVICE_QUERY_CONFIG = &H1
Private Const SERVICE_CHANGE_CONFIG = &H2
Private Const SERVICE_QUERY_STATUS = &H4
Private Const SERVICE_ENUMERATE_DEPENDENTS = &H8
Private Const SERVICE_START = &H10
Private Const SERVICE_STOP = &H20
Private Const SERVICE_PAUSE_CONTINUE = &H40
Private Const SERVICE_INTERROGATE = &H80
Private Const SERVICE_USER_DEFINED_CONTROL = &H100
Private Const SERVICE_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED Or SERVICE_QUERY_CONFIG Or SERVICE_CHANGE_CONFIG Or SERVICE_QUERY_STATUS Or SERVICE_ENUMERATE_DEPENDENTS Or SERVICE_START Or SERVICE_STOP Or SERVICE_PAUSE_CONTINUE Or SERVICE_INTERROGATE Or SERVICE_USER_DEFINED_CONTROL)
  
Private Const SERVICE_KERNEL_DRIVER As Long = &H1
  
Private Const SERVICE_DEMAND_START As Long = &H3
  
Private Const SERVICE_ERROR_NORMAL As Long = &H1
  
Private Const SERVICE_CONTROL_STOP = &H1
  
Private Declare Function QueryServiceStatus Lib "advapi32.dll" (ByVal hService As Long, ByRef lpServiceStatus As SERVICE_STATUS) As Long
  
Private Type SERVICE_STATUS
         dwServiceType As Long
         dwCurrentState As Long
         dwControlsAccepted As Long
         dwWin32ExitCode As Long
         dwServiceSpecificExitCode As Long
         dwCheckPoint As Long
         dwWaitHint As Long
End Type
Private Const SERVICE_START_PENDING As Long = &H2
Private Const SERVICE_RUNNING As Long = &H4
Private Const SERVICE_RUNS_IN_SYSTEM_PROCESS As Long = &H1
Private Const SERVICE_STOP_PENDING As Long = &H3
Private Const SERVICE_STOPPED As Long = &H1
  
  
Private Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)
  
Private Declare Function CreateFile Lib "kernel32.dll" Alias "CreateFileA" (ByVal lpFileName As String, ByVal dwDesiredAccess As Long, ByVal dwShareMode As Long, ByVal lpSecurityAttributes As Long, ByVal dwCreationDisposition As Long, ByVal dwFlagsAndAttributes As Long, ByVal hTemplateFile As Long) As Long
Private Declare Function CloseHandle Lib "kernel32.dll" (ByVal hObject As Long) As Long
Private Const GENERIC_READ As Long = &H80000000
Private Const GENERIC_WRITE As Long = &H40000000
Private Const OPEN_EXISTING As Long = 3
Private Const FILE_ATTRIBUTE_NORMAL As Long = &H80
Private Const FILE_FLAG_OVERLAPPED As Long = &H40000000
Private Const FILE_FLAG_DELETE_ON_CLOSE As Long = &H4000000
Private Const FILE_SHARE_READ As Long = &H1
Private Const FILE_SHARE_WRITE As Long = &H2
  
Private Const INVALID_HANDLE_VALUE As Long = (-1)
  
Private Declare Function DeviceIoControl Lib "kernel32" (ByVal hDevice As Long, ByVal dwIoControlCode As Long, ByVal lpInBuffer As Long, ByVal nInBufferSize As Long, ByVal lpOutBuffer As Long, ByVal nOutBufferSize As Long, lpBytesReturned As Long, ByVal lpOverlapped As Long) As Long
  
Private Const FILE_DEVICE_UNKNOWN As Long = &H22
Private Const METHOD_BUFFERED As Long = 0
Private Const FILE_ANY_ACCESS As Long = 0
  
Private Declare Function GetLastError Lib "kernel32.dll" () As Long
Private Const ERROR_SERVICE_EXISTS As Long = 1073&
Private Const ERROR_IO_PENDING As Long = 997
Private Const ERROR_SERVICE_MARKED_FOR_DELETE As Long = 1072&
  
Public szDrvSvcName As String
Public szDrvDisplayName As String
Public szDrvFilePath As String
Public szDrvLinkName As String 'e.g. "\\.\TestDrv"
'Public szDrvDeviceName As String 'e.g."\Device\TestDrv"
  
Dim hSvcHandle As Long
Dim scHandle As Long
Dim hDrvHandle As Long
  
Public Function InstDrv() As Boolean
Dim ret&
Static nTry As Integer
         scHandle = OpenSCManager(vbNullString, vbNullString, SC_MANAGER_ALL_ACCESS)
         If (Not CBool(scHandle)) Then DelDrv: Exit Function
         hSvcHandle = CreateService(scHandle, szDrvSvcName, szDrvDisplayName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, szDrvFilePath, 0, 0, 0, 0, 0)
         If (Not CBool(hSvcHandle)) Then
                 'If ((GetLastError = ERROR_SERVICE_EXISTS) Or (GetLastError = ERROR_SERVICE_MARKED_FOR_DELETE)) Then
                         If (nTry > 5) Then InstDrv = False: nTry = 0: Exit Function
                         hSvcHandle = OpenService(scHandle, szDrvSvcName, SERVICE_ALL_ACCESS)
                         DelDrv
                         nTry = nTry + 1
                         InstDrv
                 'Else
                         'DelDrv
                         'Exit Function
                 'End If
         End If
         InstDrv = True
End Function
  
'lpInBuf 和 lpOutBuf 都是指向缓冲区的指针
Public Function IoControl(ByVal dwIoControlCode As Long, ByVal lpInBuffer As Long, ByVal nInBufferSize As Long, ByVal lpOutBuffer As Long, ByVal nOutBufferSize As Long, Optional ByRef lpBytesReturned As Long = 0) As Long
Dim lDrvRetSize As Long
         IoControl = DeviceIoControl(hDrvHandle, dwIoControlCode, ByVal lpInBuffer, nInBufferSize, ByVal lpOutBuffer, nOutBufferSize, lDrvRetSize, ByVal 0)
         lpBytesReturned = lDrvRetSize
End Function
  
Public Function StartDrv() As Boolean
Dim ret&
Dim ss As SERVICE_STATUS
Call QueryServiceStatus(hSvcHandle, ss)
'If (ss.dwCurrentState = SERVICE_RUNS_IN_SYSTEM_PROCESS) Then StartDrv = True: Exit Function
ret = StartService(hSvcHandle, 0, 0)
         If (ret) Then
                 Dim nTry As Integer: nTry = 0
                 Call QueryServiceStatus(hSvcHandle, ss)
                 While ((ss.dwCurrentState = SERVICE_START_PENDING) And (nTry < 80))
                     Sleep (50)
                     nTry = nTry + 1
                     Call QueryServiceStatus(hSvcHandle, ss)
                 Wend
         End If
         StartDrv = ret
End Function
  
Public Function OpenDrv() As Boolean
         If (hDrvHandle <> INVALID_HANDLE_VALUE) Then OpenDrv = True: Exit Function
         hDrvHandle = CreateFile(IIf(Left(szDrvLinkName, Len("\\.\")) = "\\.\", szDrvLinkName, "\\.\" & szDrvLinkName), _
                 GENERIC_READ Or GENERIC_WRITE, _
                 0, _
                 0, _
                 OPEN_EXISTING, _
                 FILE_ATTRIBUTE_NORMAL, _
                 0)
         OpenDrv = (hDrvHandle <> INVALID_HANDLE_VALUE)
End Function
  
'Public Function OpenDrvByDeviceName() As Boolean
'         If (hDrvHandle <> INVALID_HANDLE_VALUE) Then OpenDrvByDeviceName = True: Exit Function
'         hDrvHandle = CreateFile(IIf(Left(szDrvDeviceName, Len("\Device\")) = "\Device\", szDrvDeviceName, "\Device\" & szDrvDeviceName), _
'                 GENERIC_READ Or GENERIC_WRITE, _
'                 0, _
'                 0, _
'                 OPEN_EXISTING, _
'                 FILE_ATTRIBUTE_NORMAL, _
'                 0)
'         OpenDrvByDeviceName = (hDrvHandle <> INVALID_HANDLE_VALUE)
'End Function
  
Public Function StopDrv() As Boolean
Dim ret&
Dim ss As SERVICE_STATUS
Dim lpSt As Long
         StopDrv = ControlService(hSvcHandle, SERVICE_CONTROL_STOP, VarPtr(lpSt))
         If (ret) Then
                 Dim nTry As Integer: nTry = 0
                 Call QueryServiceStatus(hSvcHandle, ss)
                 While ((ss.dwCurrentState = SERVICE_STOP_PENDING) And (nTry < 80))
                         Sleep (50)
                         nTry = nTry + 1
                         Call QueryServiceStatus(hSvcHandle, ss)
                 Wend
         End If
End Function
  
Public Function DelDrv() As Boolean
         Call CloseHandle(hDrvHandle)
         Call StopDrv
         Call DeleteService(hSvcHandle)
         Call CloseServiceHandle(hSvcHandle)
         Call CloseServiceHandle(scHandle)
         hSvcHandle = 0
         scHandle = 0
         DelDrv = True
End Function
  
'构造函数
Private Sub Class_Initialize()
         hSvcHandle = 0
         scHandle = 0
         hDrvHandle = INVALID_HANDLE_VALUE
End Sub
  
'构析函数
Private Sub Class_Terminate()
         DelDrv
End Sub
  
Public Function CTL_CODE(lngDevFileSys As Long, lngFunction As Long, lngMethod As Long, lngAccess As Long) As Long
CTL_CODE = (lngDevFileSys * (2 ^ 16)) Or (lngAccess * (2 ^ 14)) Or (lngFunction * (2 ^ 2)) Or lngMethod
End Function
  
Public Function CTL_CODE_GEN(ByVal lngFunction As Long) As Long
CTL_CODE_GEN = (FILE_DEVICE_UNKNOWN * (2 ^ 16)) Or (FILE_ANY_ACCESS * (2 ^ 14)) Or (lngFunction * (2 ^ 2)) Or METHOD_BUFFERED
End Function


