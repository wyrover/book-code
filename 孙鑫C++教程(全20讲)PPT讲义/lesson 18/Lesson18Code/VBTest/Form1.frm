VERSION 5.00
Object = "{C85F44F3-3BA3-493E-851A-1EE5040E1D3B}#1.0#0"; "Clock.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5055
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6735
   LinkTopic       =   "Form1"
   ScaleHeight     =   5055
   ScaleWidth      =   6735
   StartUpPosition =   3  'Windows Default
   Begin CLOCKLib.Clock Clock1 
      Height          =   1095
      Left            =   1560
      TabIndex        =   1
      Top             =   480
      Width           =   1815
      _Version        =   65536
      _ExtentX        =   3201
      _ExtentY        =   1931
      _StockProps     =   9
      ForeColor       =   16711680
      BackColor       =   65535
      Interval        =   2000
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   735
      Left            =   2760
      TabIndex        =   0
      Top             =   3000
      Width           =   1335
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Clock1_Click()
    MsgBox "control is clicked"
End Sub

Private Sub Clock1_NewMinute()
    MsgBox "new minute"
End Sub

Private Sub Command1_Click()
    Clock1.Hello
End Sub
