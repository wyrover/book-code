Attribute VB_Name = "modLinkedList"
Option Explicit

' Can use either msvbvm50 or msvbvm60
Private Declare Sub GetMem4 Lib "msvbvm50.dll" (ByRef lpPtr As Any, ByRef lpValue As Long)
Private Declare Sub PutMem4 Lib "msvbvm50.dll" (ByRef lpPtr As Any, ByVal lValue As Long)

Private Declare Function GetProcessHeap Lib "kernel32" () As Long
Private Declare Function HeapAlloc Lib "kernel32" (ByVal hHeap As Long, ByVal dwFlags As Long, ByVal dwBytes As Long) As Long
Private Declare Function HeapFree Lib "kernel32" (ByVal hHeap As Long, ByVal dwFlags As Long, lpMem As Any) As Long

' Example of how the linked list looks in memory:
Private Type Node
  value    As Long
  ptr_next As Long
End Type

Private Const ITEM_SIZE As Long = 8
Private Const OFFSET As Long = 4 ' Pointer offset

Private lHead As Long
Private lHeap As Long
Private lCount As Long

Public Function Init() As Boolean
  lHeap = GetProcessHeap
End Function

Public Sub Iterate()
  Dim i As Long
  Dim lPtr As Long
  Dim lNext As Long
  Dim lValue As Long
  
  lNext = lHead
      
  ' Iterate the list
  For i = 1 To lCount
    lPtr = lNext
    GetMem4 ByVal lPtr + OFFSET, lNext
    GetMem4 ByVal lPtr, lValue
    Debug.Print lValue
  Next i
End Sub

Public Function Add(ByVal lValue As Long, Optional ByVal lIndex As Long = 0) As Boolean
  Dim lItem As Long
  Dim lPtr As Long
  Dim i As Long
  Dim lNext As Long
  
  If lIndex > lCount Then Exit Function
  
  lItem = HeapAlloc(lHeap, 0, ITEM_SIZE)
  
  If lItem Then ' Allocation succeeded
    If lHead Then
      If lIndex = 0 Then ' Prepend Node
        PutMem4 ByVal lItem, lValue         ' Set new value
        PutMem4 ByVal lItem + OFFSET, lHead ' Link to old head
        lHead = lItem                       ' New head pointer
      Else      ' Insert/Append Node
        lNext = lHead
        
        For i = 1 To lIndex                 ' Iterate the list
          lPtr = lNext
          GetMem4 ByVal lPtr + 4, lNext
        Next i
        
        PutMem4 ByVal lPtr + OFFSET, lItem  ' Link previous with new
        PutMem4 ByVal lItem, lValue         ' Set new value
        PutMem4 ByVal lItem + OFFSET, lNext ' Link new with next (tail for append)
      End If
    Else        ' Insert Head Node
        lHead = lItem                       ' Save the head pointer
        PutMem4 ByVal lHead, lValue         ' Store the value
        PutMem4 ByVal lHead + OFFSET, 0     ' Store the tail pointer
    End If
    
    lCount = lCount + 1
    Add = True
  End If
End Function

Private Sub FreeList()
  Dim i As Long
  Dim lPtr As Long
  Dim lNext As Long
  
  lNext = lHead
      
  ' Iterate the list
  For i = 1 To lCount
    lPtr = lNext
    GetMem4 ByVal lPtr + OFFSET, lNext
    HeapFree lHeap, 0, ByVal lPtr
    If (i And &HFFFF&) = 0 Then DoEvents
  Next i
End Sub

Public Sub DeInit()
  FreeList
  lHead = 0
  lCount = 0
End Sub
