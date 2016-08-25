Attribute VB_Name = "modSortingFunctions"
Public Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)

Public Sub QuickSort(vArray As Variant, L As Long, R As Long)
  '             Array            , LBound()     , UBound()
  Dim i As Long
  Dim j As Long
  Dim X
  Dim Y

  i = L
  j = R
  X = vArray((L + R) / 2)

  Do While (i <= j)
    DoEvents
    Do While (vArray(i) < X And i < R)
      i = i + 1
    Loop

    Do While (X < vArray(j) And j > L)
      j = j - 1
    Loop

    If (i <= j) Then
      Y = vArray(i)
      vArray(i) = vArray(j)
      vArray(j) = Y
      i = i + 1
      j = j - 1
    End If
  Loop

  If (L < j) Then QuickSort vArray, L, j
  If (i < R) Then QuickSort vArray, i, R
End Sub

Public Sub QuickSort33(vArray As Variant, AccordingTo As Integer, Dimension2Size As Integer, L As Integer, R As Integer)
  '   name of array,   sorting according to which dimension?,   size of second dimension,   lbound(),   ubound()
  Dim a As Integer, i As Integer, j As Integer  ' counters
  Dim X, Y, z   ' temporary values

  i = L
  j = R
  X = vArray((L + R) / 2, AccordingTo)
  Do While (i <= j)
    DoEvents
    Do While (vArray(i, AccordingTo) < X And i < R)
      i = i + 1
    Loop
    Do While (X < vArray(j, AccordingTo) And j > L)
      j = j - 1
    Loop
    If (i <= j) Then
      Y = vArray(i, AccordingTo)
      vArray(i, AccordingTo) = vArray(j, AccordingTo)
      vArray(j, AccordingTo) = Y
      For a = 0 To AccordingTo - 1
        z = vArray(i, a)
        vArray(i, a) = vArray(j, a)
        vArray(j, a) = z
      Next a
      For a = AccordingTo + 1 To Dimension2Size
        z = vArray(i, a)
        vArray(i, a) = vArray(j, a)
        vArray(j, a) = z
      Next a
      i = i + 1
      j = j - 1
    End If
  Loop

  If (L < j) Then QuickSort33 vArray, AccordingTo, Dimension2Size, L, j
  If (i < R) Then QuickSort33 vArray, AccordingTo, Dimension2Size, i, R
End Sub

Public Sub SelectionSort(vArray, L As Integer, R As Integer)
'    name of array,    lbound(),    ubound()
Dim i As Integer
Dim j As Integer
Dim best_value  ' smallest value in array
Dim best_j As Integer
    ' loop from left to right
    For i = L To R - 1
        DoEvents
        ' initialize lowest value
        best_value = vArray(i)
        best_j = i  ' initialize lowest value array location
        For j = i + 1 To R
            ' find the lowest value in the array in this loop
            If vArray(j) < best_value Then
                best_value = vArray(j)
                best_j = j
            End If
        Next j
        ' put the smallest value at the from (left) of the array
        ' and put the value on the left of the array in the smallest
        ' value's previous position
        vArray(best_j) = vArray(i)
        vArray(i) = best_value
    Next i
    
End Sub

Public Sub QuickSortBars(vArray As Variant, L As Integer, R As Integer, Optional SleepTime As Long = 0)
  Dim i As Integer    ' counter
  Dim j As Integer    ' counter
  Dim BarVal1         ' temporary bar value
  Dim BarVal2         ' temporary bar value

  i = L
  j = R
  BarVal1 = vArray((L + R) / 2)

  Do While (i <= j)
    DoEvents
    If SleepTime > 0 Then
      Sleep SleepTime
    End If
    Do While (vArray(i) < BarVal1 And i < R)
      i = i + 1
    Loop

    Do While (BarVal1 < vArray(j) And j > L)
      j = j - 1
    Loop

    If (i <= j) Then
      BarVal2 = vArray(i)
      vArray(i) = vArray(j)
      vArray(j) = BarVal2
      frmMain.Bar(i).Value = vArray(i)
      frmMain.Bar(j).Value = vArray(j)
      i = i + 1
      j = j - 1
    End If
  Loop

  If (L < j) Then QuickSortBars vArray, L, j, SleepTime
  If (i < R) Then QuickSortBars vArray, i, R, SleepTime
End Sub

Public Sub SelectionSortBars(vArray, L As Integer, R As Integer, Optional SleepTime As Long = 0)
  '    name of array,    lbound(),    ubound()
  Dim i As Integer    ' counter
  Dim j As Integer    ' counter
  Dim best_value  ' smallest value in array
  Dim best_j As Integer

  ' loop from left to right
  For i = L To R - 1
    DoEvents
    If SleepTime > 0 Then
      Sleep SleepTime
    End If
    ' initialize lowest value
    best_value = vArray(i)
    best_j = i  ' initialize lowest value array location
    For j = i + 1 To R
      ' find the lowest value in the array in this loop
      If vArray(j) < best_value Then
        best_value = vArray(j)
        best_j = j
      End If
    Next j
    ' put the smallest value at the from (left) of the array
    ' and put the value on the left of the array in the smallest
    ' value's previous position
    vArray(best_j) = vArray(i)
    vArray(i) = best_value
    frmMain.Bar(best_j) = vArray(best_j)
    frmMain.Bar(i) = vArray(i)
  Next i
End Sub
