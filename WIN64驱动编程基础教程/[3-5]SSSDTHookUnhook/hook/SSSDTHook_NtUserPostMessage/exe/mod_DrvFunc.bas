Attribute VB_Name = "mod_DrvFunc"
Option Explicit

Public DrvController As New cls_Driver

Public Declare Sub CopyMemory Lib "ntdll" Alias "RtlMoveMemory" (ByVal pDst As Long, ByVal pSrc As Long, ByVal BLen As Long)
Public Declare Function GetCurrentProcessId Lib "kernel32" () As Long

Public Sub HOOK_SSSDT()
    With DrvController
        .IoControl .CTL_CODE_GEN(&H800), 0, 0, 0, 0
    End With
End Sub

Public Sub UNHOOK_SSSDT()
    With DrvController
        .IoControl .CTL_CODE_GEN(&H801), 0, 0, 0, 0
    End With
End Sub

