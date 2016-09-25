VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "驱动级模拟盗号"
   ClientHeight    =   2175
   ClientLeft      =   45
   ClientTop       =   345
   ClientWidth     =   4470
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   4470
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton Command2 
      Caption         =   "结束进程"
      Height          =   495
      Left            =   3360
      TabIndex        =   8
      Top             =   240
      Width           =   975
   End
   Begin VB.Frame Frame2 
      Caption         =   "Base address"
      Height          =   615
      Left            =   120
      TabIndex        =   6
      Top             =   840
      Width           =   4215
      Begin VB.TextBox Text2 
         Height          =   270
         Left            =   120
         TabIndex        =   7
         Top             =   240
         Width           =   3975
      End
   End
   Begin VB.CommandButton Command1 
      Caption         =   "写内存"
      Height          =   495
      Left            =   2280
      TabIndex        =   5
      Top             =   1560
      Width           =   975
   End
   Begin VB.Frame Frame1 
      Caption         =   "PID"
      Height          =   615
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   3135
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   120
         TabIndex        =   4
         Top             =   240
         Width           =   2895
      End
   End
   Begin VB.CommandButton Command5 
      Caption         =   "读内存"
      Height          =   495
      Left            =   1200
      TabIndex        =   2
      Top             =   1560
      Width           =   975
   End
   Begin VB.CommandButton Command4 
      Caption         =   "卸载驱动"
      Height          =   495
      Left            =   3360
      TabIndex        =   1
      Top             =   1560
      Width           =   975
   End
   Begin VB.CommandButton Command3 
      Caption         =   "加载驱动"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   1560
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
    Dim pid As Long
    pid = CLng(Text1.Text)
    Dim ba As LONGLONG
    ba.high = 0
    ba.low = CLng(Text2.Text)
    Dim rw_len As Long
    rw_len = 4
    Dim dw As Long
    dw = CLng(InputBox("Input a dword:", , CStr(&HD2B)))
    With DrvController
        Call .IoControl(.CTL_CODE_GEN(&H801), VarPtr(pid), 4, 0, 0)
        Call .IoControl(.CTL_CODE_GEN(&H802), VarPtr(ba), 8, 0, 0)
        Call .IoControl(.CTL_CODE_GEN(&H803), VarPtr(rw_len), 4, 0, 0)
        Call .IoControl(.CTL_CODE_GEN(&H805), VarPtr(dw), 4, 0, 0)
    End With
End Sub

Private Sub Command2_Click()
    With DrvController
        Call .IoControl(.CTL_CODE_GEN(&H806), VarPtr(CLng(Text1.Text)), 4, 0, 0)
    End With
End Sub

Private Sub Command5_Click()
    Dim pid As Long
    pid = CLng(Text1.Text)
    Dim ba As LONGLONG
    ba.high = 0
    ba.low = CLng(Text2.Text)
    Dim rw_len As Long
    rw_len = 4
    Dim dw As Long
    dw = 0
    With DrvController
        Call .IoControl(.CTL_CODE_GEN(&H801), VarPtr(pid), 4, 0, 0)
        Call .IoControl(.CTL_CODE_GEN(&H802), VarPtr(ba), 8, 0, 0)
        Call .IoControl(.CTL_CODE_GEN(&H803), VarPtr(rw_len), 4, 0, 0)
        Call .IoControl(.CTL_CODE_GEN(&H804), 0, 4, VarPtr(dw), 4)
    End With
    MsgBox dw
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
        .szDrvFilePath = Replace(App.Path & "\KRWProcess.sys", "\\", "\")
        .szDrvLinkName = "KRWProcess"
        .szDrvDisplayName = "KRWProcess"
        .szDrvSvcName = "KRWProcess"
    End With
End Sub
