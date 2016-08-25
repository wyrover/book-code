VERSION 5.00
Begin VB.Form Bouncy 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "The ball will bounce around inside..."
   ClientHeight    =   3195
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   352
   ScaleLeft       =   57
   ScaleMode       =   0  'User
   ScaleTop        =   57
   ScaleWidth      =   394
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   75
      Left            =   2640
      Top             =   2160
   End
   Begin VB.Shape Ball 
      FillColor       =   &H00E7DA9C&
      FillStyle       =   0  'Solid
      Height          =   495
      Left            =   1800
      Shape           =   2  'Oval
      Top             =   960
      Width           =   495
   End
End
Attribute VB_Name = "Bouncy"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public Vx As Integer, Vy As Integer
'Velocity variables.

Private Sub Form_Load()
Vx = 8
Vy = 8
'Initialize the velocities... which control how fast the ball moves.
'The eccentric scalemode that I set for the form will make this movement
'look a little exaggerated.
End Sub

Private Sub Timer1_Timer()
Ball.Left = Ball.Left + Vx
Ball.Top = Ball.Top + Vy
'Move the ball.
'Of course, after every move we check for collision.
'This is form-wide collision detection.
If Ball.Left <= Me.ScaleLeft Then
'This means the ball is passing the left edge of the form.
    Vx = -Vx
    'Reverse the horizontal velocity to make it bounce off of the end.
ElseIf Ball.Left + Ball.Width >= Me.ScaleLeft + Me.ScaleWidth Then
'This means the ball is passing off of the right edge of the form.
    Vx = -Vx
    'Bounce off of this edge.
End If

'We do the same thing for the vertical, except use Top instead of Left
'Height instead of Width, and the vertical velocity reverses.
If Ball.Top <= Me.ScaleTop Then
'This means the ball is passing the left edge of the form.
    Vy = -Vy
    'Reverse the horizontal velocity to make it bounce off of the end.
ElseIf Ball.Top + Ball.Height >= Me.ScaleTop + Me.ScaleHeight Then
'This means the ball is passing off of the right edge of the form.
    Vy = -Vy
    'Bounce off of this edge.
End If

    
End Sub
