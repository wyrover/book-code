VERSION 5.00
Begin VB.Form Form154 
   Caption         =   "Form154"
   ClientHeight    =   7035
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9525
   LinkTopic       =   "Form1"
   ScaleHeight     =   7035
   ScaleWidth      =   9525
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command5 
      Caption         =   "Line and Square contact test... move the red line and red shape during run time."
      Height          =   975
      Left            =   2640
      TabIndex        =   4
      Top             =   2400
      Width           =   1815
   End
   Begin VB.CommandButton Command4 
      Caption         =   """BreakOut"""
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   240
      Width           =   1335
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Quadrant Collision Detection - move the red shapes around during Design Time."
      Height          =   1095
      Left            =   7080
      TabIndex        =   2
      Top             =   3000
      Width           =   1935
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Move the squares at Design Time... then at Run Time, click this button to see if they Intersect."
      Height          =   735
      Left            =   1320
      TabIndex        =   1
      Top             =   6000
      Width           =   3135
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Move the lines at Design Time... then at Run Time, click this button to see if they intersect."
      Height          =   975
      Left            =   360
      TabIndex        =   0
      Top             =   3960
      Width           =   3375
   End
   Begin VB.Shape Shape4 
      FillColor       =   &H000000C0&
      FillStyle       =   0  'Solid
      Height          =   495
      Left            =   5880
      Shape           =   4  'Rounded Rectangle
      Top             =   2760
      Width           =   495
   End
   Begin VB.Shape Shape3 
      BorderColor     =   &H000000FF&
      FillColor       =   &H000000FF&
      FillStyle       =   6  'Cross
      Height          =   1575
      Left            =   5040
      Top             =   3120
      Width           =   1455
   End
   Begin VB.Shape Shape2 
      BorderColor     =   &H00FF8080&
      FillColor       =   &H00FF8080&
      FillStyle       =   7  'Diagonal Cross
      Height          =   735
      Left            =   5520
      Top             =   5520
      Width           =   1455
   End
   Begin VB.Shape Shape1 
      FillColor       =   &H00FF0000&
      FillStyle       =   0  'Solid
      Height          =   735
      Left            =   6360
      Top             =   5160
      Width           =   1455
   End
   Begin VB.Line Line2 
      BorderColor     =   &H000000FF&
      X1              =   6840
      X2              =   5760
      Y1              =   4560
      Y2              =   1440
   End
   Begin VB.Line Line1 
      X1              =   6600
      X2              =   6600
      Y1              =   5640
      Y2              =   1680
   End
End
Attribute VB_Name = "Form154"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type
Private Declare Function IntersectRect Lib "user32" (lpDestRect As RECT, lpSrc1Rect As RECT, lpSrc2Rect As RECT) As Long

Private Sub Command1_Click()
MsgBox LineContact(Line1, Line2)

End Sub


Private Sub Command2_Click()
Dim R1 As RECT, R2 As RECT, RF As RECT, bRet As Boolean
'IntersectRect provides an intersect routine also... though converting controls to rects
'takes a while...
MsgBox Contact(Shape1, Shape2)
With Shape1
    R1.Left = .Left
    R1.Top = .Top
    R1.Right = .Left + .Width
    R1.Bottom = .Top + .Height
End With
With Shape2
    R2.Left = .Left
    R2.Top = .Top
    R2.Right = .Left + .Width
    R2.Bottom = .Top + .Height
End With
'This gets really fast when you are using something such as DirectX
'where you use Rectangles to draw.
bRet = CBool(IntersectRect(RF, R1, R2))
MsgBox CBool((RF.Right - RF.Left) And bRet)
End Sub

Private Sub Command3_Click()
MsgBox QuadContact(Shape3, Shape4)
'A couple of things... the first is a dominant object... this is where the quadrants are
'divided... the second one is what we are trying to find the quadrant of.
'This type of contact is especially good for platform games.
End Sub

Private Sub Command4_Click()
Unload Me
Load BreakThru
BreakThru.Visible = True
End Sub

Private Sub Command5_Click()
MsgBox LSContact(Line2, Shape4)
End Sub
