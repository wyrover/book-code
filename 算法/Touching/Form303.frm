VERSION 5.00
Begin VB.Form Form303 
   Caption         =   "Move"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   213
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   312
   StartUpPosition =   3  'Windows Default
   Begin VB.Image Water 
      Height          =   285
      Index           =   4
      Left            =   2520
      Picture         =   "Form303.frx":0000
      Stretch         =   -1  'True
      Top             =   840
      Width           =   1815
   End
   Begin VB.Image Water 
      Height          =   285
      Index           =   3
      Left            =   2280
      Picture         =   "Form303.frx":381F
      Stretch         =   -1  'True
      Top             =   1680
      Width           =   1695
   End
   Begin VB.Image Water 
      Height          =   285
      Index           =   2
      Left            =   480
      Picture         =   "Form303.frx":703E
      Stretch         =   -1  'True
      Top             =   2520
      Width           =   3015
   End
   Begin VB.Image Water 
      Height          =   1365
      Index           =   1
      Left            =   1440
      Picture         =   "Form303.frx":A85D
      Stretch         =   -1  'True
      Top             =   600
      Width           =   255
   End
   Begin VB.Image Water 
      Height          =   525
      Index           =   0
      Left            =   0
      Picture         =   "Form303.frx":E07C
      Stretch         =   -1  'True
      Top             =   1440
      Width           =   1470
   End
   Begin VB.Image Charmander 
      Height          =   480
      Left            =   120
      Picture         =   "Form303.frx":1189B
      Top             =   0
      Width           =   480
   End
End
Attribute VB_Name = "Form303"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Dim LV As Integer, Collision As Boolean
Select Case KeyCode
'Move our character forward
Case vbKeyLeft
    Charmander.Left = Charmander.Left - 8
Case vbKeyUp
    Charmander.Top = Charmander.Top - 8
Case vbKeyRight
    Charmander.Left = Charmander.Left + 8
Case vbKeyDown
    Charmander.Top = Charmander.Top + 8
End Select
For LV = 0 To Water.UBound
'Check if we will touch water.
    If Charmander.Left < Water(LV).Left + Water(LV).Width And Water(LV).Left < Charmander.Left + Charmander.Width Then
        If Charmander.Top < Water(LV).Top + Water(LV).Height And Water(LV).Top < Charmander.Top + Charmander.Height Then
            'collision!
            Collision = True
        End If
    End If
Next
If Collision Then
'Move back to original position - cannot move this way.
    Select Case KeyCode
    Case vbKeyLeft
        Charmander.Left = Charmander.Left + 8
    Case vbKeyUp
        Charmander.Top = Charmander.Top + 8
    Case vbKeyRight
        Charmander.Left = Charmander.Left - 8
    Case vbKeyDown
        Charmander.Top = Charmander.Top - 8
    End Select
End If
End Sub

