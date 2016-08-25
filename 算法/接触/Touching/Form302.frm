VERSION 5.00
Begin VB.Form Form302 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   213
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   312
   StartUpPosition =   3  'Windows Default
   Begin VB.Image Charmander 
      Height          =   480
      Left            =   480
      Picture         =   "Form302.frx":0000
      Top             =   2280
      Width           =   480
   End
   Begin VB.Image Statue 
      Height          =   480
      Left            =   3480
      Picture         =   "Form302.frx":0487
      Top             =   840
      Width           =   480
   End
End
Attribute VB_Name = "Form302"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
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
If Charmander.Left < Statue.Left + Statue.Width And Statue.Left < Charmander.Left + Charmander.Width Then
    If Charmander.Top < Statue.Top + Statue.Height And Statue.Top < Charmander.Top + Charmander.Height Then
        'collision!
        Statue.Visible = False
    End If
End If
End Sub
