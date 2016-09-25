VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Win64 Force Kill Process"
   ClientHeight    =   1455
   ClientLeft      =   45
   ClientTop       =   345
   ClientWidth     =   4470
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   1455
   ScaleWidth      =   4470
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton Command1 
      Caption         =   "PspTerminateProcess"
      Height          =   495
      Left            =   1200
      TabIndex        =   4
      Top             =   840
      Width           =   2055
   End
   Begin VB.Frame Frame1 
      Caption         =   "PID"
      Height          =   615
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4215
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   3975
      End
   End
   Begin VB.CommandButton Command4 
      Caption         =   "UnLdDrv"
      Height          =   495
      Left            =   3360
      TabIndex        =   1
      Top             =   840
      Width           =   975
   End
   Begin VB.CommandButton Command3 
      Caption         =   "LoadDrv"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   840
      Width           =   975
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim DrvController As New cls_Driver

Private Sub Command1_Click()
    Dim pppPID As Long, tarPID As Long, hProcess As Long
    tarPID = CLng(Text1.Text)
    With DrvController
        Call .IoControl(.CTL_CODE_GEN(&H806), VarPtr(tarPID), 4, 0, 0)
    End With
End Sub

Private Sub Form_Unload(cancel As Integer)
    End
End Sub

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
    Command4.Enabled = False
End Sub

Private Sub Form_Load()
    'load and inst drv
    With DrvController
        .szDrvFilePath = Replace(App.Path & "\Win7x64Drv.sys", "\\", "\")
        .szDrvLinkName = "Win7x64Drv"
        .szDrvDisplayName = "Win7x64Drv"
        .szDrvSvcName = "Win7x64Drv"
    End With
End Sub
