VERSION 5.00
Begin VB.Form Collitblt 
   AutoRedraw      =   -1  'True
   Caption         =   "Collisions"
   ClientHeight    =   5220
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7320
   LinkTopic       =   "Form1"
   ScaleHeight     =   5220
   ScaleWidth      =   7320
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox TileBox 
      AutoRedraw      =   -1  'True
      Height          =   735
      Index           =   3
      Left            =   3120
      Picture         =   "Collitblt.frx":0000
      ScaleHeight     =   675
      ScaleWidth      =   675
      TabIndex        =   5
      Top             =   720
      Visible         =   0   'False
      Width           =   735
   End
   Begin VB.PictureBox TileBox 
      AutoRedraw      =   -1  'True
      Height          =   735
      Index           =   2
      Left            =   2160
      Picture         =   "Collitblt.frx":1337
      ScaleHeight     =   675
      ScaleWidth      =   675
      TabIndex        =   4
      Top             =   720
      Visible         =   0   'False
      Width           =   735
   End
   Begin VB.PictureBox TileBox 
      AutoRedraw      =   -1  'True
      Height          =   735
      Index           =   1
      Left            =   1080
      Picture         =   "Collitblt.frx":3329
      ScaleHeight     =   675
      ScaleWidth      =   795
      TabIndex        =   3
      Top             =   720
      Visible         =   0   'False
      Width           =   855
   End
   Begin VB.PictureBox Charmask 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FFFFFF&
      Height          =   615
      Left            =   1080
      Picture         =   "Collitblt.frx":9F6B
      ScaleHeight     =   555
      ScaleWidth      =   555
      TabIndex        =   2
      Top             =   0
      Visible         =   0   'False
      Width           =   615
   End
   Begin VB.PictureBox TileBox 
      AutoRedraw      =   -1  'True
      Height          =   735
      Index           =   0
      Left            =   0
      Picture         =   "Collitblt.frx":A035
      ScaleHeight     =   675
      ScaleWidth      =   795
      TabIndex        =   1
      Top             =   720
      Visible         =   0   'False
      Width           =   855
   End
   Begin VB.PictureBox Charmander 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00000000&
      Height          =   615
      Left            =   0
      Picture         =   "Collitblt.frx":16077
      ScaleHeight     =   555
      ScaleWidth      =   555
      TabIndex        =   0
      Top             =   0
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
Private Declare Function OffsetRect Lib "user32" (lpRect As RECT, ByVal X As Long, ByVal y As Long) As Long
'So we know where I am going
Private Declare Function SetRect Lib "user32" (lpRect As RECT, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
'Initializes the rects for me
Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal X As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
'Draws the User interface for me

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Const TILESIZE As Integer = 32
'Changing this will make the tiles smaller/bigger,
'but the main character will chop off/be too small
Private Const LASTOBSTACLEINDEX As Integer = 1

Private MapArr(0 To 9, 0 To 9) As Integer
'An array of integers holds our map
Private MyX As Integer, MyY As Integer
Private BlockedSet(0 To LASTOBSTACLEINDEX) As Integer
'This will hold the set of all tiles that I want to be blocked.


Private Sub Form_Activate()
Dim I As Integer, J As Integer

'Draws the map with two for loops to reference the 2D array
For J = 0 To 9
    For I = 0 To 9
        BitBlt Me.hDC, TILESIZE * I, TILESIZE * J, TILESIZE, TILESIZE, TileBox(MapArr(I, J)).hDC, 0, 0, vbSrcCopy
    Next
Next

'Draw the main character with transparency.
BitBlt Me.hDC, TILESIZE * MyX, TILESIZE * MyY, TILESIZE, TILESIZE, Charmask.hDC, 0, 0, vbSrcAnd
BitBlt Me.hDC, TILESIZE * MyX, TILESIZE * MyY, TILESIZE, TILESIZE, Charmander.hDC, 0, 0, vbSrcPaint
Me.Refresh
End Sub


Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
Dim Collided As Boolean, LV As Integer

'Move the main character's coordinates
Select Case KeyCode
Case vbKeyLeft
    MyX = MyX - 1
Case vbKeyUp
    MyY = MyY - 1
Case vbKeyRight
    MyX = MyX + 1
Case vbKeyDown
    MyY = MyY + 1
End Select

'If I was only checking for a specific tile, I could do the next line:
'If MapArr(MyX, MyY) = 1 Then Collided = True

'I look through the blocked set to see if I find a match.  If so, then I
'have collided with a blocked tile.
For LV = 0 To LASTOBSTACLEINDEX
    If MapArr(MyX, MyY) = BlockedSet(LV) Then
        Collided = True
    End If
Next

'Move back if main character collided with something
If Collided Then
    Select Case KeyCode
    Case vbKeyLeft
        MyX = MyX + 1
    Case vbKeyUp
        MyY = MyY + 1
    Case vbKeyRight
        MyX = MyX - 1
    Case vbKeyDown
        MyY = MyY - 1
    End Select
End If

'Redraw the map.
Form_Activate
End Sub

Private Sub Form_Load()
Dim X As Integer

'My hardcoded map.
For X = 0 To 9
    MapArr(X, 0) = 1
    MapArr(0, X) = 1
    MapArr(X, 9) = 1
    MapArr(9, X) = 1
Next
MapArr(8, 1) = 1
MapArr(8, 2) = 1
MapArr(5, 5) = 3
MapArr(5, 4) = 1
MapArr(5, 3) = 3
MapArr(6, 3) = 3
MapArr(2, 6) = 1
MapArr(2, 7) = 1
MapArr(4, 6) = 3
MapArr(4, 7) = 3

MapArr(1, 2) = 2
MapArr(2, 2) = 2
MapArr(2, 3) = 2
MapArr(3, 2) = 2
MapArr(3, 3) = 2

'Main character coordinates
MyX = 1: MyY = 1

'Set up which tiles are going to be blocked.
BlockedSet(0) = 1: BlockedSet(1) = 3
End Sub
