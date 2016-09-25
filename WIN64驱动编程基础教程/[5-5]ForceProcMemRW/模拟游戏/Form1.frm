VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "模拟游戏"
   ClientHeight    =   1935
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3015
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   1935
   ScaleWidth      =   3015
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame Frame1 
      Height          =   1335
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2775
      Begin VB.CommandButton Command3 
         Caption         =   "Copy"
         Height          =   255
         Left            =   2040
         TabIndex        =   4
         Top             =   0
         Width           =   615
      End
      Begin VB.CommandButton Command2 
         Caption         =   "设置值"
         Height          =   495
         Left            =   1440
         TabIndex        =   3
         Top             =   720
         Width           =   1215
      End
      Begin VB.CommandButton Command1 
         Caption         =   "读取值"
         Height          =   495
         Left            =   120
         TabIndex        =   2
         Top             =   720
         Width           =   1215
      End
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   120
         TabIndex        =   1
         Text            =   "999"
         Top             =   360
         Width           =   2535
      End
   End
   Begin VB.Label Label1 
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Label1"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   1560
      Width           =   2775
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function GetCurrentProcessId Lib "kernel32.dll" () As Long

Dim dw As Long

Private Sub Command1_Click()
    MsgBox dw, vbInformation
End Sub

Private Sub Command2_Click()
    If IsNumeric(Text1.Text) = False Or Trim$(Text1.Text) = "" Then
        Text1.Text = ""
        Exit Sub
    End If
    If CDbl(Text1.Text) > &H7FFFFFFF Or CDbl(Text1.Text) < 0 Then
        MsgBox "值异常！请设置0x0至0x7FFFFFFF之间的值！", vbCritical
        Exit Sub
    End If
    dw = CLng(Text1.Text)
    MsgBox "值设置成功！", vbInformation
End Sub

Private Sub Command3_Click()
    Clipboard.Clear
    Clipboard.SetText CStr(VarPtr(dw))
End Sub

Private Sub Form_Load()
    Label1.Caption = " PID: " & CStr(GetCurrentProcessId)
    Frame1.Caption = "变量地址：" & CStr(VarPtr(dw))
End Sub
