VERSION 5.00
Begin VB.Form RBBB 
   Caption         =   "Form1"
   ClientHeight    =   5295
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5895
   LinkTopic       =   "Form1"
   ScaleHeight     =   353
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   393
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   360
      Top             =   4680
   End
   Begin VB.Shape Rubberball 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   495
      Left            =   2280
      Shape           =   3  'Circle
      Top             =   600
      Width           =   495
   End
   Begin VB.Line InnerBorder 
      Index           =   3
      X1              =   200
      X2              =   272
      Y1              =   192
      Y2              =   192
   End
   Begin VB.Line InnerBorder 
      Index           =   2
      X1              =   272
      X2              =   272
      Y1              =   192
      Y2              =   128
   End
   Begin VB.Line InnerBorder 
      Index           =   1
      X1              =   272
      X2              =   200
      Y1              =   128
      Y2              =   128
   End
   Begin VB.Line InnerBorder 
      Index           =   0
      X1              =   200
      X2              =   200
      Y1              =   128
      Y2              =   192
   End
   Begin VB.Shape BigBlock 
      BorderColor     =   &H00000000&
      FillColor       =   &H0089BCE2&
      FillStyle       =   0  'Solid
      Height          =   1215
      Left            =   2880
      Top             =   1800
      Width           =   1335
   End
   Begin VB.Shape Cornfield 
      FillColor       =   &H00C0C000&
      FillStyle       =   0  'Solid
      Height          =   4335
      Left            =   840
      Shape           =   1  'Square
      Top             =   240
      Width           =   4455
   End
End
Attribute VB_Name = "RBBB"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Vx As Integer, Vy As Integer
'The velocity of the ball's movement.


Private Sub Form_Load()
Vx = 10
Vy = 10
End Sub

Private Sub Timer1_Timer()
Dim BCDSide As Integer, RCDSide As Integer
'The boundary collision detection can respond to each four side collision by breaking the original formula into each
'part.

'Prediction... results are only valid if we make a collision.
If Rubberball.Left < BigBlock.Left Then
    RCDSide = 0
ElseIf Rubberball.Left + Rubberball.Width > BigBlock.Left + BigBlock.Width Then
    RCDSide = 2
ElseIf Rubberball.Top < BigBlock.Top Then
    RCDSide = 1
ElseIf Rubberball.Top + Rubberball.Height > BigBlock.Top + BigBlock.Height Then
    RCDSide = 3
End If


'Movement.
Rubberball.Move Rubberball.Left + Vx, Rubberball.Top + Vy



'Check for collision.
If Rubberball.Left < BigBlock.Left + BigBlock.Width And BigBlock.Left < Rubberball.Left + Rubberball.Width Then
    If Rubberball.Top < BigBlock.Top + BigBlock.Height And BigBlock.Top < Rubberball.Top + Rubberball.Height Then
        'collision.
        Select Case RCDSide
        Case 0, 2  'Hit the wall on the horizontal.
            Vx = -Vx
        Case 1, 3
            Vy = -Vy
        End Select
    End If
End If


'Check for boundaries.
If Rubberball.Left < Cornfield.Left Then
    BCDSide = 0
    Vx = -Vx
ElseIf Rubberball.Left + Rubberball.Width > Cornfield.Left + Cornfield.Width Then
    BCDSide = 2
    Vx = -Vx
ElseIf Rubberball.Top < Cornfield.Top Then
    BCDSide = 1
    Vy = -Vy
ElseIf Rubberball.Top + Rubberball.Height > Cornfield.Top + Cornfield.Height Then
    BCDSide = 3
    Vy = -Vy
End If

End Sub
