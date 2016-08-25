VERSION 5.00
Object = "{D45DB913-393E-4329-9226-06A029A25857}#1.0#0"; "Clock.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   1200
      TabIndex        =   1
      Top             =   2160
      Width           =   1575
   End
   Begin CLOCKLib.Clock Clock1 
      Height          =   855
      Left            =   1200
      TabIndex        =   0
      Top             =   720
      Width           =   1455
      _Version        =   65536
      _ExtentX        =   2566
      _ExtentY        =   1508
      _StockProps     =   9
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
