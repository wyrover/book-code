VERSION 5.00
Begin VB.Form Formlines 
   BackColor       =   &H00FFFFFF&
   Caption         =   "Linearland"
   ClientHeight    =   5475
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7140
   LinkTopic       =   "Form1"
   ScaleHeight     =   5475
   ScaleWidth      =   7140
   StartUpPosition =   3  'Windows Default
   Begin VB.Line Ln 
      X1              =   4080
      X2              =   5640
      Y1              =   2040
      Y2              =   840
   End
   Begin VB.Line Bigline 
      BorderColor     =   &H80000002&
      X1              =   2280
      X2              =   2280
      Y1              =   1800
      Y2              =   3000
   End
End
Attribute VB_Name = "Formlines"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Dim M1 As Double, M2 As Double, B1 As Double, B2 As Double, Collided As Boolean
Dim X As Double, Y As Double
If Shift Then
    If KeyCode = vbKeyLeft Then
        Ln.X1 = Ln.X1 - 50
    ElseIf KeyCode = vbKeyUp Then
        Ln.Y1 = Ln.Y1 - 50
    ElseIf KeyCode = vbKeyRight Then
        Ln.X1 = Ln.X1 + 50
    ElseIf KeyCode = vbKeyDown Then
        Ln.Y1 = Ln.Y1 + 50
    End If
Else
    If KeyCode = vbKeyLeft Then
        Ln.X2 = Ln.X2 - 50
    ElseIf KeyCode = vbKeyUp Then
        Ln.Y2 = Ln.Y2 - 50
    ElseIf KeyCode = vbKeyRight Then
        Ln.X2 = Ln.X2 + 50
    ElseIf KeyCode = vbKeyDown Then
        Ln.Y2 = Ln.Y2 + 50
    End If
End If
If Bigline.X1 = Bigline.X2 Then
    M1 = 1000000
    B1 = 0
Else
    M1 = (Bigline.Y2 - Bigline.Y1) / (Bigline.X2 - Bigline.X1)
    B1 = Bigline.Y1 - M1 * Bigline.X1
End If
If Ln.X1 = Ln.X2 Then
    M2 = 1000000
    B2 = 0
Else
    M2 = (Ln.Y2 - Ln.Y1) / (Ln.X2 - Ln.X1)
    B2 = Ln.Y1 - M2 * Ln.X1
End If
If M1 = M2 Then
  If M2 = 1000000 Then
    If Ln.X1 = Bigline.X1 Then  'Vertical line collision.
      Collided = True
    End If
  ElseIf B1 = B2 Then  'This determines if the lines lie on top of each other.
    Collided = True
  End If
Else
    If M1 = 1000000 Then
      X = Bigline.X1  'Vertical line.
      Y = M2 * X + B2  'use the decent line.
    ElseIf M2 = 1000000 Then
      X = Ln.X1
      Y = M1 * X + B1  'use the decent line.
    Else
        X = (B1 - B2) / (M2 - M1)
        Y = M1 * X + B1
    End If
    If Between(X, Ln.X1, Ln.X2) And Between(X, Bigline.X1, Bigline.X2) And _
    Between(Y, Ln.Y1, Ln.Y2) And Between(Y, Bigline.Y1, Bigline.Y2) Then
      Collided = True
    End If
End If
If Collided Then
    Ln.BorderColor = vbRed
Else
    Ln.BorderColor = vbBlack
End If
End Sub

'A handy function for determining if a value lies between two other values.
Public Function Between(ByVal CValue As Double, ByVal Bound1 As Double, ByVal Bound2 As Double) As Boolean
    If (CValue >= Bound1 And CValue <= Bound2) Or (CValue <= Bound1 And CValue >= Bound2) Then Between = True
End Function
