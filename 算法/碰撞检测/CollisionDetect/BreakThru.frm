VERSION 5.00
Begin VB.Form BreakThru 
   BackColor       =   &H00C00000&
   BorderStyle     =   0  'None
   Caption         =   "Form1"
   ClientHeight    =   5805
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6525
   LinkTopic       =   "Form1"
   ScaleHeight     =   5805
   ScaleWidth      =   6525
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   60
      Left            =   5760
      Top             =   720
   End
   Begin VB.Shape Border 
      BorderColor     =   &H0000FFFF&
      FillColor       =   &H00FF8080&
      Height          =   5775
      Left            =   0
      Top             =   0
      Width           =   6495
   End
   Begin VB.Shape Brk 
      FillColor       =   &H00FFFF00&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   0
      Left            =   480
      Top             =   480
      Width           =   495
   End
   Begin VB.Shape Boundary 
      FillStyle       =   0  'Solid
      Height          =   495
      Index           =   2
      Left            =   480
      Top             =   0
      Width           =   4815
   End
   Begin VB.Shape Boundary 
      FillStyle       =   0  'Solid
      Height          =   5775
      Index           =   1
      Left            =   5280
      Top             =   0
      Width           =   1215
   End
   Begin VB.Shape Boundary 
      FillStyle       =   0  'Solid
      Height          =   5775
      Index           =   0
      Left            =   0
      Top             =   0
      Width           =   495
   End
   Begin VB.Shape Shape1 
      FillColor       =   &H000000FF&
      FillStyle       =   0  'Solid
      Height          =   135
      Left            =   720
      Shape           =   3  'Circle
      Top             =   2760
      Width           =   135
   End
   Begin VB.Image Image1 
      DragMode        =   1  'Automatic
      Height          =   345
      Left            =   2880
      Picture         =   "BreakThru.frx":0000
      Stretch         =   -1  'True
      Top             =   5400
      Width           =   1770
   End
End
Attribute VB_Name = "BreakThru"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim Vx As Single, Vy As Single
Const VELMAX As Single = 80

Private Sub Form_Click()
Unload Me
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
If KeyCode = 37 Then Image1.Left = Image1.Left - 50
If KeyCode = 39 Then Image1.Left = Image1.Left + 50
End Sub

Private Sub Form_Load()
Dim LV As Byte
Vx = VELMAX
Vy = VELMAX
For LV = 1 To 49
    Load Brk(LV)
    Brk(LV).Move 480 * (LV Mod 10) + 480, 240 * (LV \ 10) + 480
    Brk(LV).Visible = True
Next
End Sub

Private Sub Form_DragDrop(Source As Control, X As Single, Y As Single)
Source.Move X - Source.Width / 2, Y - Source.Height / 2
'If Contact(Source, Shape1) Then MsgBox "Contact!"
'MsgBox QuadContact(Source, Shape1)
End Sub

Private Sub Timer1_Timer()
Dim QS As Byte, LV As Byte, Angle As Single, OX As Control
Dim NegateX As Boolean, NegateY As Boolean
Shape1.Left = Shape1.Left + Vx
Shape1.Top = Shape1.Top + Vy
For Each OX In Brk
    QS = QuadContact(OX, Shape1)
    If QS And 7 Then
        If OX.Index > 0 Then Unload Brk(OX.Index)
        Select Case QS
        Case 1, 3
            NegateX = True
        Case 2, 4
            NegateY = True
        End Select
    End If
Next
For LV = 0 To 2
    QS = QuadContact(Boundary(LV), Shape1)
    If QS And 7 Then
        Select Case QS
        Case 1, 3
            NegateX = True
        Case 2, 4
            NegateY = True
        End Select
    End If
Next
If NegateX Then Vx = -Vx
If NegateY Then Vy = -Vy
If Contact(Shape1, Image1) Then
    Angle = 120 * ((Shape1.Left - Image1.Left - Shape1.Width) / (Image1.Width + Shape1.Width)) - 90
    Vy = -VELMAX * Cos((Angle + 45) * 3.14159265258979 / 180)
    Vx = VELMAX * Sin((Angle + 45) * 3.14159265258979 / 180)
End If
If Containment(Border, Shape1) = False Then MsgBox "Game over!": Unload Me
End Sub
