VERSION 5.00
Begin VB.Form frm_Main 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "win64kih"
   ClientHeight    =   1215
   ClientLeft      =   45
   ClientTop       =   345
   ClientWidth     =   2775
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   1215
   ScaleWidth      =   2775
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton Command7 
      Caption         =   "Hook PsLookupProcessByProcessId"
      Height          =   975
      Left            =   1440
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
   Begin VB.CommandButton Command4 
      Caption         =   "卸驱动"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   600
      Width           =   1215
   End
   Begin VB.CommandButton Command3 
      Caption         =   "加驱动"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
End
Attribute VB_Name = "frm_Main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Command3_Click()
    With DrvController
        .InstDrv
        .StartDrv
        .OpenDrv
    End With
    If DrvController.OpenDrv = False Then MsgBox "驱动加载失败", 64, ""
    If DrvController.OpenDrv = True Then Command3.Enabled = False
End Sub

Private Sub Command4_Click()
    With DrvController
        .StopDrv
        .DelDrv
    End With
    Command3.Enabled = True
End Sub

Private Sub Command7_Click()
    Hook_PspTTBP
    Command7.Enabled = False
End Sub

Private Sub Form_Load()
    'load and inst drv
    With DrvController
        .szDrvFilePath = Replace(App.Path & "\hook_ps.sys", "\\", "\")
        .szDrvLinkName = "hook_ps"
        .szDrvDisplayName = "hook_ps"
        .szDrvSvcName = "hook_ps"
    End With
End Sub

Private Sub Form_Unload(cancel As Integer)
    With DrvController
        .StopDrv
        .DelDrv
    End With
End Sub
