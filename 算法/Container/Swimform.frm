VERSION 5.00
Begin VB.Form Swimform 
   BackColor       =   &H00FF0000&
   Caption         =   "Container collision detection."
   ClientHeight    =   3930
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4095
   FillColor       =   &H00FF0000&
   LinkTopic       =   "Form1"
   ScaleHeight     =   262
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   273
   StartUpPosition =   3  'Windows Default
   Begin VB.Shape Coral 
      BackColor       =   &H00C0C000&
      BackStyle       =   1  'Opaque
      FillColor       =   &H000000FF&
      FillStyle       =   7  'Diagonal Cross
      Height          =   375
      Left            =   2880
      Top             =   2760
      Width           =   375
   End
   Begin VB.Image Squirtle 
      Height          =   480
      Left            =   1560
      Picture         =   "Swimform.frx":0000
      Top             =   1080
      Width           =   480
   End
   Begin VB.Shape SeaBox 
      FillColor       =   &H00DC995C&
      FillStyle       =   0  'Solid
      Height          =   3255
      Left            =   360
      Top             =   240
      Width           =   3255
   End
End
Attribute VB_Name = "Swimform"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Dim OutofBound As Boolean, Collision As Boolean
Select Case KeyCode
Case vbKeyLeft
    Squirtle.Left = Squirtle.Left - 8
Case vbKeyUp
    Squirtle.Top = Squirtle.Top - 8
Case vbKeyRight
    Squirtle.Left = Squirtle.Left + 8
Case vbKeyDown
    Squirtle.Top = Squirtle.Top + 8
End Select
'Here is where I do boundary crossing detection
'It looks a little like the collision detection
'Except it uses ors and I am comparing left with left,
'top with top, etc.
If Squirtle.Left < SeaBox.Left Or Squirtle.Left + Squirtle.Width > SeaBox.Left + SeaBox.Width _
  Or Squirtle.Top < SeaBox.Top Or Squirtle.Top + Squirtle.Height > SeaBox.Top + SeaBox.Height Then
    OutofBound = True
End If

'A collision detection test.
If Squirtle.Left < Coral.Left + Coral.Width And Coral.Left < Squirtle.Left + Squirtle.Width Then
    If Squirtle.Top < Coral.Top + Coral.Height And Coral.Top < Squirtle.Top + Squirtle.Height Then
        Collision = True
    End If
End If

'In boundary crossing, you can set the main character's coordinates
'directly to the boundary coordinates, which will give you a
'"snap to" effect.

If OutofBound Then
    Select Case KeyCode
    Case vbKeyLeft
        Squirtle.Left = SeaBox.Left
    Case vbKeyUp
        Squirtle.Top = SeaBox.Top
    Case vbKeyRight
        Squirtle.Left = SeaBox.Left + SeaBox.Width - Squirtle.Width
    Case vbKeyDown
        Squirtle.Top = SeaBox.Top + SeaBox.Height - Squirtle.Height
    End Select
End If
'Collision
If Collision Then
    Coral.Visible = False
End If

End Sub

