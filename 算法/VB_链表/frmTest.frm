VERSION 5.00
Begin VB.Form frmTest 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Linked List Example"
   ClientHeight    =   2544
   ClientLeft      =   36
   ClientTop       =   276
   ClientWidth     =   3744
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2544
   ScaleWidth      =   3744
   StartUpPosition =   2  'CenterScreen
   Begin VB.HScrollBar hsbItems 
      Height          =   252
      Left            =   1830
      TabIndex        =   8
      Top             =   840
      Value           =   499
      Width           =   1812
   End
   Begin VB.PictureBox picStatus 
      Align           =   2  'Align Bottom
      AutoRedraw      =   -1  'True
      Height          =   288
      Left            =   0
      ScaleHeight     =   240
      ScaleWidth      =   3696
      TabIndex        =   7
      Top             =   2256
      Width           =   3744
   End
   Begin VB.OptionButton optOperation 
      Caption         =   "Add Items"
      Height          =   252
      Index           =   0
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   1572
   End
   Begin VB.OptionButton optOperation 
      Caption         =   "Item Removal"
      Height          =   252
      Index           =   3
      Left            =   120
      TabIndex        =   2
      Top             =   1200
      Width           =   1572
   End
   Begin VB.OptionButton optOperation 
      Caption         =   "Iterator Iteration"
      Height          =   252
      Index           =   2
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   1572
   End
   Begin VB.OptionButton optOperation 
      Caption         =   "Index Iteration"
      Height          =   252
      Index           =   1
      Left            =   120
      TabIndex        =   0
      Top             =   480
      Width           =   1572
   End
   Begin VB.Label lblNumItems 
      Alignment       =   2  'Center
      Caption         =   "999"
      Height          =   252
      Left            =   1800
      TabIndex        =   9
      Top             =   1150
      Width           =   1932
   End
   Begin VB.Label lblItems 
      Alignment       =   2  'Center
      Caption         =   "Number of Items"
      Height          =   252
      Left            =   1920
      TabIndex        =   6
      Top             =   600
      Width           =   1692
   End
   Begin VB.Label lblListInfo 
      Alignment       =   2  'Center
      Height          =   372
      Left            =   1920
      TabIndex        =   5
      Top             =   1680
      Width           =   1692
   End
   Begin VB.Label lblCollInfo 
      Alignment       =   2  'Center
      Height          =   372
      Left            =   120
      TabIndex        =   4
      Top             =   1680
      Width           =   1572
   End
End
Attribute VB_Name = "frmTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' Linked List example and comparison to the VB Collection
' Make sure to compile first and test on the EXE.

' http://www.syix.com/wpsjr1/index.html

Dim list As cLinkedList
Dim coll As Collection
Dim lItems As Long

Private Sub collAdd()
  Dim i As Long
  
  For i = 1 To lItems
    coll.Add i
  Next i
End Sub

Private Sub listAdd()
  Dim i As Long
  
  For i = 1 To lItems
    list.Add i
  Next i
End Sub

' The VB collection is linked list based too, _
' so it suffers from the same speed problem here as the list
Private Function collIterIndex() As Long
  Dim i As Long
  
  For i = 1 To coll.Count
    collIterIndex = collIterIndex + coll.Item(i)
  Next i
End Function

' Indexed list iteration is slow due to how a list is constructed.
' Without any external knowledge of where an item is, you must _
' loop through the entire list each time to get an item.
' As you might suspect, retriving items farther into the _
' list gets progressively slower.

' Note that this is the same as getting an item from a VB collection _
' with an index (not a string key).  Read: S L O W
Private Function listIterIndex() As Long
  Dim i As Long
  
  For i = 0 To list.Count - 1
    listIterIndex = listIterIndex + list.Item(i)
  Next i
End Function

Private Function collIterIterator() As Long
  Dim v As Variant
  
  For Each v In coll
    collIterIterator = collIterIterator + v
  Next
End Function

' Iterate quickly through the linked list by using raw pointers
' Note that this is similar to a For/Each loop on a VB collection.
Private Function listIterIterator() As Long
  Dim i As Long
  Dim lPtr As Long    ' Pointer to the next list item
  Dim lValue As Long  ' Variable passed ByRef to retrieve the value
  
  lPtr = list.HeadPtr ' Get the pointer to the first item in the list
  
  For i = 1 To list.Count
    lPtr = list.Iterate(lPtr, lValue) ' Pass the current pointer and get the next
    listIterIterator = listIterIterator + lValue
  Next i
End Function

Private Sub collRemoveItems()
  Dim i As Long
  
  For i = 1 To coll.Count
    coll.Remove 1
  Next i
End Sub

' Could use list.Clear too
Private Sub listRemoveItems()
  Dim i As Long
  
  For i = 1 To list.Count
    list.Remove 0
  Next i
End Sub

Private Sub Form_Load()
  hsbItems_Change
  optOperation(0).value = True
End Sub

Private Sub Form_Resize()
  If WindowState <> vbMinimized Then
    picStatus.Move 0, picStatus.Top, ScaleWidth
  End If
End Sub

Private Sub hsbItems_Change()
  lItems = (hsbItems.value * 2&) + 1
  lblNumItems.Caption = CStr(lItems)
  optOperation(1).Enabled = (lItems <= 20000)
End Sub

Private Sub hsbItems_Scroll()
  hsbItems_Change
End Sub

Private Sub optOperation_Click(Index As Integer)
  Select Case Index
    Case 0 ' Add Items
      Set coll = Nothing
      Set list = Nothing
      Set coll = New Collection
      Set list = New cLinkedList
      
      UpdateStatus " " & CStr(lItems) & " Items"
      
      Start
      collAdd
      lblCollInfo.Caption = "Collection:" & vbCrLf & CStr(Finish) & " ms"
        
      Start
      listAdd
      lblListInfo.Caption = "Linked List:" & vbCrLf & CStr(Finish) & " ms"
    
    Case 1 ' Index Iteration
      MousePointer = vbHourglass
      Start
      collIterIndex
      lblCollInfo.Caption = "Collection:" & vbCrLf & CStr(Finish) & " ms"
      
      Start
      listIterIndex
      lblListInfo.Caption = "Linked List:" & vbCrLf & CStr(Finish) & " ms"
      MousePointer = vbDefault
    
    Case 2 ' Iterator Iteration
      Start
      collIterIterator
      lblCollInfo.Caption = "Collection:" & vbCrLf & CStr(Finish) & " ms"
      
      Start
      listIterIterator
      lblListInfo.Caption = "Linked List:" & vbCrLf & CStr(Finish) & " ms"
    
    Case 3 ' Item Removal
      Start
      collRemoveItems
      lblCollInfo.Caption = "Collection:" & vbCrLf & CStr(Finish) & " ms"
      
      Start
      list.Clear
      lblListInfo.Caption = "Linked List:" & vbCrLf & CStr(Finish) & " ms"
      
      UpdateStatus " 0 Items"
      
    End Select
End Sub

Private Sub UpdateStatus(ByVal sText As String)
  picStatus.Cls
  picStatus.Print sText
End Sub
