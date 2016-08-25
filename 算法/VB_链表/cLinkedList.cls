VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "cLinkedList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

' Zero-based linked list example

' http://www.syix.com/wpsjr1/index.html

' Linked lists are handy for fast insertion or deletion of items.
' Memory allocation is definitely not efficient in this implementation.

' Can use either msvbvm50 or msvbvm60
' GetMem4 and PutMem4 are much more efficient than RtlMoveMemory _
' for writing/reading four bytes at a time

' Declares remmed out as TLB is referenced
' Private Declare Sub GetMem4 Lib "msvbvm50.dll" (ByRef lpPtr As Any, ByRef lpValue As Long)
' Private Declare Sub PutMem4 Lib "msvbvm50.dll" (ByRef lpPtr As Any, ByVal lValue As Long)
'
' Private Declare Function GetProcessHeap Lib "kernel32" () As Long
' Private Declare Function HeapAlloc Lib "kernel32" (ByVal hHeap As Long, ByVal dwFlags As Long, ByVal dwBytes As Long) As Long
' Private Declare Function HeapFree Lib "kernel32" (ByVal hHeap As Long, ByVal dwFlags As Long, lpMem As Any) As Long
' Private Declare Function IsBadReadPtr Lib "kernel32.dll" (ByRef lp As Any, ByVal ucb As Long) As Long

' Example of how the linked list looks in memory:
Private Type Node
  value    As Long ' This could be any data type with four bytes.  Single, BSTR pointer, etc. Might require some alteration.
  ptr_next As Long
End Type

Private Const ITEM_SIZE As Long = 8
Private Const OFFSET As Long = 4 ' Pointer offset

Private m_lHead As Long
Private m_lHeap As Long
Private m_lCount As Long

Private Sub Class_Initialize()
  m_lHeap = GetProcessHeap
End Sub

' Fast list iteration
' To begin, pass the headptr
' Input:
' lPtrItem - Pointer to the item you are currently iterating
' lValue - Byref parameter which will contain the node value
'
' Return:
' Pointer to the next node
Public Function Iterate(ByVal lPtrItem As Long, ByRef lValue As Long) As Long
  If IsBadReadPtr(ByVal lPtrItem, ITEM_SIZE) = False Then ' Handle null and bad pointers
    GetMem4 ByVal lPtrItem, lValue
    GetMem4 ByVal lPtrItem + OFFSET, Iterate
  End If
End Function

' Add items anywhere in the list
' Input:
' lValue - The value of the node
' [lIndex] - Optional index for insertion
'
' To insert before the head node, pass 0 (Default)
' To insert anywhere in the list pass the index before which you wish to insert
' To insert at the end of the list, pass list.Count as the insertion point
'
' Return: True on success
Public Function Add(ByVal lValue As Long, Optional ByVal lIndex As Long = 0) As Boolean
  Dim lItem As Long
  Dim lPtr As Long
  Dim i As Long
  Dim lNext As Long
  
  If lIndex > m_lCount Then Exit Function
  
  lItem = HeapAlloc(m_lHeap, 0, ITEM_SIZE)
  
  If lItem Then ' Allocation succeeded
    If m_lHead Then
      If lIndex = 0 Then ' Prepend Node
        PutMem4 ByVal lItem, lValue           ' Set new value
        PutMem4 ByVal lItem + OFFSET, m_lHead ' Link to old head
        m_lHead = lItem                       ' New head pointer
      Else      ' Insert/Append Node
        lNext = m_lHead
        
        For i = 1 To lIndex                   ' Iterate the list
          lPtr = lNext
          GetMem4 ByVal lPtr + 4, lNext
        Next i
        
        PutMem4 ByVal lPtr + OFFSET, lItem    ' Link previous with new
        PutMem4 ByVal lItem, lValue           ' Set new value
        PutMem4 ByVal lItem + OFFSET, lNext   ' Link new with next (tail for append)
      End If
    Else        ' Insert Head Node
        m_lHead = lItem                       ' Save the head pointer
        PutMem4 ByVal m_lHead, lValue         ' Store the value
        PutMem4 ByVal m_lHead + OFFSET, 0     ' Store the tail pointer
    End If
    
    m_lCount = m_lCount + 1
    Add = True
  End If
End Function

' Clear the entire list and deallocate memory
Public Sub Clear()
  Dim i As Long
  Dim lPtr As Long
  Dim lNext As Long
  
  lNext = m_lHead
      
  ' Iterate the list
  For i = 1 To m_lCount
    lPtr = lNext
    GetMem4 ByVal lPtr + OFFSET, lNext
    HeapFree m_lHeap, 0, ByVal lPtr
    If (i And &HFFFF&) = 0 Then DoEvents
  Next i
  
  m_lHead = 0
  m_lCount = 0
End Sub

' Number of items in the list
Public Property Get Count() As Long
  Count = m_lCount
End Property

' Pointer to the first item in the list
' Used for iteration
Public Property Get HeadPtr() As Long
  HeadPtr = m_lHead
End Property

' Store a value to the list
' Overwrites the previous value
' lIndex is zero-based
Public Property Let Item(ByVal lIndex As Long, ByVal lValue As Long)
  Dim i As Long
  Dim lPtr As Long
  Dim lNext As Long
  
  If lIndex >= 0 And lIndex < m_lCount Then
  
    lNext = m_lHead
      
    ' Iterate the list
    For i = 0 To lIndex
      lPtr = lNext
      GetMem4 ByVal lPtr + OFFSET, lNext
    Next i
    
    PutMem4 ByVal lPtr, lValue
  End If
End Property

' Get the value of an item in the list
' lIndex is zero-based
Public Property Get Item(ByVal lIndex As Long) As Long
  Dim i As Long
  Dim lPtr As Long
  Dim lNext As Long
  
  If lIndex >= 0 And lIndex < m_lCount Then
  
    lNext = m_lHead
      
    ' Iterate the list
    For i = 0 To lIndex
      lPtr = lNext
      GetMem4 ByVal lPtr + OFFSET, lNext
    Next i
    
    GetMem4 ByVal lPtr, Item
  End If
End Property

' Remove an item from the list
' lIndex is zero-based
Public Sub Remove(ByVal lIndex As Long)
  Dim i As Long
  Dim lPtr As Long
  Dim lNext As Long
  Dim lPrev As Long
  
  If lIndex >= m_lCount Then Exit Sub
  
  If lIndex = 0 Then  ' Delete Head Node
    GetMem4 ByVal m_lHead + OFFSET, lNext
    HeapFree m_lHeap, 0, ByVal m_lHead
    m_lHead = lNext
  Else                ' Delete any other Nodes
    lNext = m_lHead
      
    ' Iterate the list
    For i = 0 To lIndex - 1
      lPtr = lNext
      GetMem4 ByVal lPtr + OFFSET, lNext
    Next i
    
    lPrev = lPtr
    lPtr = lNext
    GetMem4 ByVal lPtr + OFFSET, lNext  ' Get the node pointed to by the node to be removed
    PutMem4 ByVal lPrev + OFFSET, lNext ' Set the previous node to the next node
    HeapFree m_lHeap, 0, ByVal lPtr     ' Get memory back from deleted node
  End If
  
  m_lCount = m_lCount - 1
End Sub

Private Sub Class_Terminate()
  If m_lCount Then Clear
End Sub
