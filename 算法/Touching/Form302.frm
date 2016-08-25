VERSION 5.00
Begin VB.Form Form302 
   Caption         =   "Collect the Statues"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   213
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   312
   StartUpPosition =   3  'Windows Default
   Begin VB.Image Statue 
      Height          =   1440
      Index           =   8
      Left            =   0
      Picture         =   "Form302.frx":0000
      Stretch         =   -1  'True
      Top             =   960
      Width           =   1320
   End
   Begin VB.Image Statue 
      Height          =   480
      Index           =   7
      Left            =   1680
      Picture         =   "Form302.frx":05B8
      Stretch         =   -1  'True
      Top             =   0
      Width           =   480
   End
   Begin VB.Image Statue 
      Height          =   360
      Index           =   6
      Left            =   120
      Picture         =   "Form302.frx":0B70
      Stretch         =   -1  'True
      Top             =   360
      Width           =   1080
   End
   Begin VB.Image Statue 
      Height          =   240
      Index           =   5
      Left            =   1440
      Picture         =   "Form302.frx":1128
      Stretch         =   -1  'True
      Top             =   2160
      Width           =   240
   End
   Begin VB.Image Statue 
      Height          =   480
      Index           =   4
      Left            =   3360
      Picture         =   "Form302.frx":16E0
      Stretch         =   -1  'True
      Top             =   0
      Width           =   480
   End
   Begin VB.Image Statue 
      Height          =   720
      Index           =   3
      Left            =   2040
      Picture         =   "Form302.frx":1C98
      Stretch         =   -1  'True
      Top             =   2280
      Width           =   360
   End
   Begin VB.Image Statue 
      Height          =   840
      Index           =   2
      Left            =   2640
      Picture         =   "Form302.frx":2250
      Stretch         =   -1  'True
      Top             =   1200
      Width           =   840
   End
   Begin VB.Image Statue 
      Height          =   240
      Index           =   1
      Left            =   3840
      Picture         =   "Form302.frx":2808
      Stretch         =   -1  'True
      Top             =   1320
      Width           =   240
   End
   Begin VB.Image Charmander 
      Height          =   480
      Left            =   0
      Picture         =   "Form302.frx":2DC0
      Top             =   2640
      Width           =   480
   End
   Begin VB.Image Statue 
      Height          =   960
      Index           =   0
      Left            =   3960
      Picture         =   "Form302.frx":3247
      Stretch         =   -1  'True
      Top             =   1920
      Width           =   120
   End
End
Attribute VB_Name = "Form302"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Click()
Unload Me
Load Form303
Form303.Visible = True
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Dim LV As Integer
Select Case KeyCode
Case vbKeyLeft
    Charmander.Left = Charmander.Left - 8
Case vbKeyUp
    Charmander.Top = Charmander.Top - 8
Case vbKeyRight
    Charmander.Left = Charmander.Left + 8
Case vbKeyDown
    Charmander.Top = Charmander.Top + 8
End Select
For LV = 0 To Statue.UBound
    If Charmander.Left < Statue(LV).Left + Statue(LV).Width And Statue(LV).Left < Charmander.Left + Charmander.Width Then
        If Charmander.Top < Statue(LV).Top + Statue(LV).Height And Statue(LV).Top < Charmander.Top + Charmander.Height Then
            'collision!
            Statue(LV).Visible = False
        End If
    End If
Next
End Sub
