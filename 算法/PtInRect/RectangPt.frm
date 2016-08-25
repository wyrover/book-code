VERSION 5.00
Begin VB.Form RectangPt 
   BackColor       =   &H00000000&
   Caption         =   "The bubbles move when they are in the rect."
   ClientHeight    =   4590
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   12435
   LinkTopic       =   "Form1"
   ScaleHeight     =   306
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   829
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   60
      Left            =   5040
      Top             =   120
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   63
      Left            =   480
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   62
      Left            =   360
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   61
      Left            =   360
      Shape           =   3  'Circle
      Top             =   600
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   60
      Left            =   720
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   59
      Left            =   240
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   58
      Left            =   600
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   57
      Left            =   600
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   56
      Left            =   120
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   55
      Left            =   360
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   54
      Left            =   120
      Shape           =   3  'Circle
      Top             =   0
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   53
      Left            =   720
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   52
      Left            =   600
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   51
      Left            =   600
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   50
      Left            =   360
      Shape           =   3  'Circle
      Top             =   0
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   49
      Left            =   480
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   48
      Left            =   0
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   47
      Left            =   720
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   46
      Left            =   240
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   45
      Left            =   240
      Shape           =   3  'Circle
      Top             =   600
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   44
      Left            =   600
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   43
      Left            =   120
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   42
      Left            =   480
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   41
      Left            =   480
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   40
      Left            =   360
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   39
      Left            =   600
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   38
      Left            =   120
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   37
      Left            =   120
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   36
      Left            =   480
      Shape           =   3  'Circle
      Top             =   0
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   35
      Left            =   0
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   34
      Left            =   360
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   33
      Left            =   360
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   32
      Left            =   720
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   31
      Left            =   120
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   30
      Left            =   480
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   29
      Left            =   480
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   28
      Left            =   0
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   27
      Left            =   360
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   26
      Left            =   600
      Shape           =   3  'Circle
      Top             =   0
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   25
      Left            =   720
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   24
      Left            =   240
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   23
      Left            =   480
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   22
      Left            =   240
      Shape           =   3  'Circle
      Top             =   0
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   21
      Left            =   360
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   20
      Left            =   120
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   19
      Left            =   720
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   18
      Left            =   600
      Shape           =   3  'Circle
      Top             =   600
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   17
      Left            =   600
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   16
      Left            =   120
      Shape           =   3  'Circle
      Top             =   600
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   15
      Left            =   0
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   14
      Left            =   360
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   13
      Left            =   360
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   12
      Left            =   720
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   11
      Left            =   240
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   10
      Left            =   600
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   9
      Left            =   600
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   8
      Left            =   480
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   7
      Left            =   720
      Shape           =   3  'Circle
      Top             =   480
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   6
      Left            =   240
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   5
      Left            =   240
      Shape           =   3  'Circle
      Top             =   720
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   4
      Left            =   600
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   3
      Left            =   120
      Shape           =   3  'Circle
      Top             =   360
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   2
      Left            =   480
      Shape           =   3  'Circle
      Top             =   600
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   1
      Left            =   480
      Shape           =   3  'Circle
      Top             =   240
      Width           =   75
   End
   Begin VB.Shape Bubble 
      BorderColor     =   &H00C0FFC0&
      FillColor       =   &H0000C000&
      FillStyle       =   0  'Solid
      Height          =   75
      Index           =   0
      Left            =   240
      Shape           =   3  'Circle
      Top             =   120
      Width           =   75
   End
   Begin VB.Shape Descendant 
      BackColor       =   &H00FF8080&
      BorderColor     =   &H00944527&
      BorderWidth     =   5
      FillColor       =   &H00FFC0C0&
      FillStyle       =   4  'Upward Diagonal
      Height          =   3975
      Left            =   3480
      Top             =   0
      Width           =   2895
   End
   Begin VB.Shape Ascendant 
      BackColor       =   &H008080FF&
      BorderColor     =   &H00565CD6&
      BorderWidth     =   5
      FillColor       =   &H00C0C0FF&
      FillStyle       =   5  'Downward Diagonal
      Height          =   3735
      Left            =   8760
      Top             =   720
      Width           =   3495
   End
End
Attribute VB_Name = "RectangPt"
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
Private Type Vect2D   'This is a custom type that I created that stores a 2D location.
    I As Integer  'Left location.
    J As Integer  'Top location.
End Type

Private Declare Function PtInRect Lib "user32" (lpRect As RECT, ByVal x As Long, ByVal y As Long) As Long
Private Declare Function SetRect Lib "user32" (lpRect As RECT, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
Private Declare Function OffsetRect Lib "user32" (lpRect As RECT, ByVal x As Long, ByVal y As Long) As Long
Dim Ascend As RECT, Descend As RECT
'Ascendant is the red shape that makes the bubbles rise.
'Descendant is the blue shape.
Dim Bubbles() As Vect2D
'Yes, there are 63 of them.


Private Sub Form_Load()
Dim LV As Integer
ReDim Bubbles(0 To Bubble.UBound)
'As long as there are no missing bubbles, you can add as many bubbles as you want.
For LV = 0 To Bubble.UBound
'Make the bubbles go to random locations.
'Regardless of where they are on the form.
    Bubbles(LV).I = Int(Rnd * 290)
    Bubbles(LV).J = Int(Rnd * 250)
    Bubble(LV).Move Bubbles(LV).I, Bubbles(LV).J
Next
'Set the rectangles based off of the location that the controls are at on the form...
'that you specify at design time.
SetRect Ascend, Ascendant.Left, Ascendant.Top, Ascendant.Left + Ascendant.Width, Ascendant.Top + Ascendant.Height
SetRect Descend, Descendant.Left, Descendant.Top, Descendant.Left + Descendant.Width, Descendant.Top + Descendant.Height

End Sub

Private Sub Timer1_Timer()
Dim LV As Integer
For LV = 0 To Bubble.UBound
    With Bubbles(LV)
    'If bubble is in ascending rect, it goes up.
        If PtInRect(Ascend, .I, .J) Then
            .J = .J - 4
        End If
        If PtInRect(Descend, .I, .J) Then
            .J = .J + 4
        End If
    'If bubble is in descending rect, it goes down.
    
    'Update bubble control position.
        Bubble(LV).Move .I, .J
    End With
Next
    
OffsetRect Ascend, -8, 0
OffsetRect Descend, -8, 0

If Ascend.Right < 0 Then
'If the rectangle is off of the form, then it should move back to the other end.
    OffsetRect Ascend, Me.ScaleWidth, 0
End If
If Descend.Right < 0 Then

    OffsetRect Descend, 700, 0
    'I hope 500 is less than the scalewidth, so that the blue descendant
    'comes more often than the ascendant.
End If

'Update the controls location.
Ascendant.Left = Ascend.Left
Descendant.Left = Descend.Left

End Sub
