VERSION 5.00
Begin VB.Form Collitblt 
   AutoRedraw      =   -1  'True
   Caption         =   "Collisions"
   ClientHeight    =   6390
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10800
   LinkTopic       =   "Form1"
   ScaleHeight     =   6390
   ScaleWidth      =   10800
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox Wallbox 
      AutoRedraw      =   -1  'True
      Height          =   3015
      Left            =   7440
      Picture         =   "Collitblt.frx":0000
      ScaleHeight     =   2955
      ScaleWidth      =   3075
      TabIndex        =   1
      Top             =   840
      Visible         =   0   'False
      Width           =   3135
   End
   Begin VB.PictureBox Charmander 
      AutoRedraw      =   -1  'True
      Height          =   615
      Left            =   7440
      Picture         =   "Collitblt.frx":1C70
      ScaleHeight     =   555
      ScaleWidth      =   555
      TabIndex        =   0
      Top             =   120
      Visible         =   0   'False
      Width           =   615
   End
End
Attribute VB_Name = "Collitblt"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function IntersectRect Lib "user32" (lpDestRect As RECT, lpSrc1Rect As RECT, lpSrc2Rect As RECT) As Long
'Determines collision
Private Declare Function OffsetRect Lib "user32" (lpRect As RECT, ByVal x As Long, ByVal y As Long) As Long
'So we know where I am going
Private Declare Function SetRect Lib "user32" (lpRect As RECT, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
'Initializes the rects for me
Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
'Draws the User interface for me

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Const LASTWALLNUMBER As Integer = 6
Private Walls(0 To 9) As RECT
'I can't walk into these.  The last wall 9 is a buffer for IntersectRect
Private Mainchar As RECT
'This is the main character, here

Private Sub Form_Activate()
Dim LV As Integer 'Loop variable
Me.Cls 'Clear form so I can start over
For LV = 0 To LASTWALLNUMBER
    BitBlt Me.hDC, Walls(LV).Left, Walls(LV).Top, Walls(LV).Right - Walls(LV).Left, Walls(LV).Bottom - Walls(LV).Top, Wallbox.hDC, 0, 0, vbSrcCopy
    'draws the walls ^^^ left and top ^^^       ^^^ width is the right minus left, ^^^ height is bottom minus top,  ^^^ this is the wall
    'the 0 0 just means we are looking at the 'top left' corner of the picturebox to be where the  painted image comes from.
Next
BitBlt Me.hDC, Mainchar.Left, Mainchar.Top, Mainchar.Right - Mainchar.Left, Mainchar.Bottom - Mainchar.Top, Charmander.hDC, 0, 0, vbSrcCopy
'Draws the Charmander.  left,    top ,      unless Charmander will dynamically change size, you can just use 32 as these 4th and 5th parameters.
'You'd also have to shrink the image.
'And, no, Charmander does not have a Transparency although it would only require an extra call to BitBlt, and a mask.

End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Dim LV As Integer, Collided As Boolean
Select Case KeyCode
Case vbKeyLeft
    OffsetRect Mainchar, -8, 0
Case vbKeyUp
    OffsetRect Mainchar, 0, -8
Case vbKeyRight
    OffsetRect Mainchar, 8, 0
Case vbKeyDown
    OffsetRect Mainchar, 0, 8
End Select
'I use offset rect here
For LV = 0 To LASTWALLNUMBER
    If IntersectRect(Walls(9), Mainchar, Walls(LV)) Then
        Collided = True
    End If
Next
'Checks for collision by testing the return value of intersect rect
If Collided Then
    Select Case KeyCode
    Case vbKeyLeft
        OffsetRect Mainchar, 8, 0
    Case vbKeyUp
        OffsetRect Mainchar, 0, 8
    Case vbKeyRight
        OffsetRect Mainchar, -8, 0
    Case vbKeyDown
        OffsetRect Mainchar, 0, -8
    End Select
End If
'Move the rect back if we collided with something
Call Form_Activate
'After everything has been done,
'my pictures are redrawn... I drew them in form_activate
End Sub

Private Sub Form_Load()
SetRect Walls(0), 40, 40, 120, 80
SetRect Walls(1), 120, 40, 160, 160
SetRect Walls(2), 0, 240, 200, 360
SetRect Walls(3), 240, 0, 320, 160
SetRect Walls(4), 240, 160, 280, 320
SetRect Walls(5), 200, 360, 400, 400
SetRect Walls(6), 400, 160, 440, 360
SetRect Mainchar, 0, 0, 32, 32
'All of the walls are set in multiples of 40 so that things look nice and tiley.
End Sub
