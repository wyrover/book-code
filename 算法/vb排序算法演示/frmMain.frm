VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Sorting Demo Application"
   ClientHeight    =   7590
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9375
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7590
   ScaleWidth      =   9375
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame fraBars 
      Caption         =   "Progress Bars"
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   7335
      Left            =   2400
      TabIndex        =   5
      Top             =   120
      Width           =   6855
      Begin MSComctlLib.ProgressBar Bar 
         Height          =   50
         Index           =   0
         Left            =   120
         TabIndex        =   10
         Top             =   660
         Width           =   6615
         _ExtentX        =   11668
         _ExtentY        =   79
         _Version        =   393216
         BorderStyle     =   1
         Appearance      =   0
         Scrolling       =   1
      End
      Begin VB.CommandButton cmdBubbleSort 
         Caption         =   "Bubble Sort"
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   5160
         TabIndex        =   9
         Top             =   240
         Width           =   1575
      End
      Begin VB.CommandButton cmdSelectionSortOnBars 
         Caption         =   "Selection Sort"
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   3480
         TabIndex        =   8
         Top             =   240
         Width           =   1575
      End
      Begin VB.CommandButton cmdQuickSortOnBars 
         Caption         =   "Quick Sort"
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   1800
         TabIndex        =   7
         Top             =   240
         Width           =   1575
      End
      Begin VB.CommandButton cmdShuffle 
         Caption         =   "Shuffle"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   120
         TabIndex        =   6
         Top             =   240
         Width           =   1575
      End
   End
   Begin VB.Frame fraList 
      Caption         =   "List Box"
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   7335
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2175
      Begin VB.ListBox lstSort 
         BeginProperty Font 
            Name            =   "Courier New"
            Size            =   9.75
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   5580
         Left            =   120
         TabIndex        =   4
         Top             =   1680
         Width           =   1935
      End
      Begin VB.CommandButton cmdLoad 
         Caption         =   "Load List"
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   1935
      End
      Begin VB.CommandButton cmdSelectionSortOnList 
         Caption         =   "Selection Sort"
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   120
         TabIndex        =   2
         Top             =   1200
         Width           =   1935
      End
      Begin VB.CommandButton cmdQuickSortOnList 
         Caption         =   "Quick Sort"
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "Verdana"
            Size            =   8.25
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   120
         TabIndex        =   1
         Top             =   720
         Width           =   1935
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function GetTickCount Lib "kernel32.dll" () As Long

Dim aList() As String   ' List to be sorted

Private Sub cmdBubbleSort_Click()
  Dim X As Integer      ' counter
  Dim i As Integer      ' counter
  Dim StartTime As Long ' start time of sort
  Dim EndTime As Long   ' end time of sort
  Dim BarVal As Integer ' temporary bar value

  StartTime = GetTickCount
  For X = 0 To Bar.Count - 2
    For i = 0 To Bar.Count - 2  ' loop through max index - 1
      DoEvents
      If Bar(i).Value > Bar(i + 1).Value Then
        BarVal = Bar(i).Value
        Bar(i).Value = Bar(i + 1).Value
        Bar(i + 1).Value = BarVal
      End If
    Next i
  Next X
  EndTime = GetTickCount

  MsgBox "Time for sort: " & EndTime - StartTime & " milliseconds" & Chr(13) & _
         "Note: bar value loading was done while sorting for visual effect."
End Sub

Private Sub cmdLoad_Click()
  Dim X As Long  ' counter

  lstSort.Clear
  ' loop to make a list of randomly generated strings
  For X = 0 To 9999
    ReDim Preserve aList(X) As String

    aList(X) = RandomString(5)
    lstSort.AddItem aList(X)
  Next X
  ' enable sort buttons
  cmdQuickSortOnList.Enabled = True
  cmdSelectionSortOnList.Enabled = True
End Sub

Private Function RandomString(iStringLength As Integer) As String
  Dim X As Integer  ' counter
  
  Randomize
  ' loop to generate random string (ASCII between 65 and 90; A - Z)
  For X = 1 To iStringLength
    RandomString = RandomString & Chr(90 - (Rnd * 25))
  Next X
End Function

Private Sub cmdQuickSortOnBars_Click()
  Dim X As Integer              ' counter
  Dim StartTime As Long         ' start time of sort
  Dim EndTime As Long           ' end time of sort
  Dim aBarValues() As Integer   ' array of bar values
  Dim SleepTime As Long         ' does the user want to slow down the sort?

  For X = 0 To Bar.Count - 1
    ReDim Preserve aBarValues(X)
    aBarValues(X) = Bar(X).Value
  Next X

  If MsgBox("Do you want to slow down the sort for visualization purposes?", vbYesNo + vbQuestion) = vbYes Then
    SleepTime = 10
  End If
  StartTime = GetTickCount()
  QuickSortBars aBarValues, 0, Bar.Count - 1, SleepTime
  EndTime = GetTickCount()

  MsgBox "Time for sort: " & EndTime - StartTime & " milliseconds" & Chr(13) & _
         "Note: bar value loading was done while sorting for visual effect."

  Erase aBarValues
End Sub

Private Sub cmdQuickSortOnList_Click()
  Dim StartTime As Long ' start time of sort
  Dim EndTime As Long   ' end time of sort
  Dim X As Integer      ' counter

  If lstSort.ListCount = 0 Then
    MsgBox "You must load a list before sorting."
    Exit Sub
  End If
  Screen.MousePointer = vbArrowHourglass
  lstSort.Clear
  StartTime = GetTickCount()
  Call QuickSort(aList, 0, UBound(aList))
  EndTime = GetTickCount()
  For X = 0 To UBound(aList)
    lstSort.AddItem aList(X)
  Next X
  Screen.MousePointer = vbArrow
  MsgBox "Time for sort: " & EndTime - StartTime & " milliseconds" & Chr(13) & _
         "Note: time does not include time for ListBox value loading."
End Sub

Private Sub cmdSelectionSortOnBars_Click()
  Dim X As Integer              ' counter
  Dim aBarValues() As Integer   ' array of bar values
  Dim StartTime As Long         ' start time of sort
  Dim EndTime As Long           ' end time of sort
  Dim SleepTime As Long         ' does the user want to slow down the sort?

  For X = 0 To Bar.Count - 1
    ReDim Preserve aBarValues(X) As Integer
    aBarValues(X) = Bar(X).Value
  Next X

  If MsgBox("Do you want to slow down the sort for visualization purposes?", vbYesNo + vbQuestion) = vbYes Then
    SleepTime = 10
  End If
  StartTime = GetTickCount()
  Call SelectionSortBars(aBarValues, 0, UBound(aBarValues), SleepTime)
  EndTime = GetTickCount()

  MsgBox "Time for sort: " & EndTime - StartTime & " milliseconds" & Chr(13) & _
         "Note: bar value loading was done while sorting for visual effect."

  Erase aBarValues
End Sub

Private Sub cmdSelectionSortOnList_Click()
  Dim StartTime As Long ' start time of sort
  Dim EndTime As Long   ' end time of sort
  Dim X As Integer      ' counter

  If lstSort.ListCount = 0 Then
    MsgBox "You must load a list before sorting."
    Exit Sub
  End If
  Screen.MousePointer = vbArrowHourglass
  lstSort.Clear
  StartTime = GetTickCount()
  Call SelectionSort(aList, 0, UBound(aList))
  EndTime = GetTickCount()
  For X = 0 To UBound(aList)
    lstSort.AddItem aList(X)
  Next X
  Screen.MousePointer = vbArrow
  MsgBox "Time for sort: " & EndTime - StartTime & " milliseconds" & Chr(13) & _
         "Note: time does not include time for ListBox value loading."
End Sub

Private Sub cmdShuffle_Click()
  ' Shuffle progress bars
  Dim X As Integer  ' counter

  Randomize
  For X = 0 To Bar.Count - 1
    Bar(X).Min = 0
    Bar(X).Max = 1000
    Bar(X).Value = Rnd * 1000
  Next X
  ' enable sort buttons
  cmdQuickSortOnBars.Enabled = True
  cmdSelectionSortOnBars.Enabled = True
  cmdBubbleSort.Enabled = True
End Sub

Private Sub Form_Load()
  Dim X As Integer  ' counter

  ' loop through and load all the progress bars
  For X = 1 To 219
    Load Bar(X)
    Bar(X).Container = Bar(X - 1).Container
    Bar(X).Height = Bar(X - 1).Height
    Bar(X).Visible = True
    Bar(X).Left = Bar(X - 1).Left
    Bar(X).Top = Bar(X - 1).Top + 30
  Next X
End Sub

Private Sub Form_Unload(Cancel As Integer)
Erase aList
End Sub

