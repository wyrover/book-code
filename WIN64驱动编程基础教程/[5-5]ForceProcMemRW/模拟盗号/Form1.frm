VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "模拟盗号"
   ClientHeight    =   2190
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2190
   ScaleWidth      =   3030
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton Command3 
      Caption         =   "打开进程"
      Height          =   495
      Left            =   1680
      TabIndex        =   6
      Top             =   240
      Width           =   1215
   End
   Begin VB.Frame Frame2 
      Caption         =   "进程ID"
      Height          =   615
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   1455
      Begin VB.TextBox Text2 
         Height          =   270
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   1215
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "变量地址"
      Enabled         =   0   'False
      Height          =   1215
      Left            =   120
      TabIndex        =   0
      Top             =   840
      Width           =   2775
      Begin VB.TextBox Text1 
         Height          =   270
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   2535
      End
      Begin VB.CommandButton Command1 
         Caption         =   "读取值"
         Height          =   495
         Left            =   120
         TabIndex        =   2
         Top             =   600
         Width           =   1215
      End
      Begin VB.CommandButton Command2 
         Caption         =   "设置值"
         Height          =   495
         Left            =   1440
         TabIndex        =   1
         Top             =   600
         Width           =   1215
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function OpenProcess Lib "kernel32.dll" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal dwProcessId As Long) As Long
Private Declare Function ReadProcessMemory Lib "kernel32.dll" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long
Private Declare Function WriteProcessMemory Lib "kernel32.dll" (ByVal hProcess As Long, lpBaseAddress As Any, lpBuffer As Any, ByVal nSize As Long, lpNumberOfBytesWritten As Long) As Long

Dim h As Long

Private Sub Command1_Click()
    Dim dw As Long
    ReadProcessMemory h, ByVal CLng(Text1.Text), dw, 4, 0
    MsgBox dw, vbInformation
End Sub

Private Sub Command2_Click()
    Dim v As Long
    v = CLng(InputBox("输入您要设置的值：", , CStr(&HD2B)))
    WriteProcessMemory h, ByVal CLng(Text1.Text), v, 4, 0
End Sub

Private Sub Command3_Click()
    h = OpenProcess(&H1F0FFF, 0, CLng(Text2.Text))
    If h > 0 Then
        MsgBox "打开进程成功！句柄：" & CStr(h), vbInformation
        Frame1.Enabled = True
    Else
        MsgBox "打开进程失败！", vbCritical
    End If
End Sub
