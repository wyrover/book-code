Attribute VB_Name = "MStringSortRoutines"
'*******************************************************************************
' MODULE:       MStringSortRoutines
' FILENAME:     C:\My Code\vb\Sorting\MStringSortRoutines.bas
' AUTHOR:       Phil Fresle
' CREATED:      01-Dec-1999
' COPYRIGHT:    Copyright 1999 Frez Systems Limited. All Rights Reserved.
'
' DESCRIPTION:
' Generic string sort routines, prefer to use the 'non-pure' Quick Sort unless
' you have a good reason to choose another routine.  Here are the routines in
' order of efficiency.
'
' Quick Sort     - Fast for large arrays - delegates to insert sort when called
'                  with a small array and to sort small chunks of the large array
'                  This 'non-pure' quick sort is therefore quicker by not
'                  recursing for small chunks where a simple brute-force iteration
'                  is quicker.
' Merge Sort     - Fast for large arrays - larger memory footprint than QuickSort
'                  but will sort faster if the data is not completely random. Delegates
'                  to insert sort for small arrays.
' Insert Sort    - Fast for small arrays (say less than 60 values)
' Selection Sort - Fast for small arrays (say less than 60 values)
'
' NOTE: Due to the recursive nature of Quick and Merge sort they are not very efficient
' for small arrays which is why the routines delegate to a more brute force insert
' sort for small arrays.
'
' This is 'free' software with the following restrictions:
'
' You may not redistribute this code as a 'sample' or 'demo'. However, you are free
' to use the source code in your own code, but you may not claim that you created
' the sample code. It is expressly forbidden to sell or profit from this source code
' other than by the knowledge gained or the enhanced value added by your own code.
'
' Use of this software is also done so at your own risk. The code is supplied as
' is without warranty or guarantee of any kind.
'
' Should you wish to commission some derivative work based on the add-in provided
' here, or any consultancy work, please do not hesitate to contact us.
'
' Web Site:  http://www.frez.co.uk
' E-mail:    sales@frez.co.uk
'
' MODIFICATION HISTORY:
' 1.0       01-Dec-1999
'           Phil Fresle
'           Initial Version
' 1.1       22-Mar-2000
'           Phil Fresle
'           Added Merge Sort
'*******************************************************************************
Option Explicit


'*******************************************************************************
' SelectionSortStrings (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()    - String  - Array to sort
' (In)     - bAscending     - Boolean - True to sort ascending, false descending
' (In)     - bCaseSensitive - Boolean - True for a case sensitive sort, false
'                                       for an insensitive one
'
' DESCRIPTION:
' Simple Selection Sort routine for strings, fast for small arrays (say less
' than 60 values).
'*******************************************************************************
Public Sub SelectionSortStrings(ListArray() As String, _
                                Optional ByVal bAscending As Boolean = True, _
                                Optional ByVal bCaseSensitive As Boolean = False)
    
    Dim sSmallest       As String
    Dim lSmallest       As Long
    Dim lCount1         As Long
    Dim lCount2         As Long
    Dim lMin            As Long
    Dim lMax            As Long
    Dim lCompareType    As Long
    Dim lOrder          As Long
    
    lMin = LBound(ListArray)
    lMax = UBound(ListArray)
    
    If lMin = lMax Then
        Exit Sub
    End If
    
    ' Order Ascending or Descending?
    lOrder = IIf(bAscending, -1, 1)
    
    ' Case sensitive search or not?
    lCompareType = IIf(bCaseSensitive, vbBinaryCompare, vbTextCompare)
    
    ' Loop through array swapping the smallest\largest (determined by lOrder)
    ' item with the current item
    For lCount1 = lMin To lMax - 1
        sSmallest = ListArray(lCount1)
        lSmallest = lCount1
        
        ' Find the smallest\largest item in the array
        For lCount2 = lCount1 + 1 To lMax
            If StrComp(ListArray(lCount2), sSmallest, lCompareType) = lOrder Then
                sSmallest = ListArray(lCount2)
                lSmallest = lCount2
            End If
        Next
        
        ' Just swap them, even if we are swapping it with itself,
        ' as it is generally quicker to do this than test first
        ' each time if we are already the smallest with a
        ' test like: If lSmallest <> lCount1 Then
        ListArray(lSmallest) = ListArray(lCount1)
        ListArray(lCount1) = sSmallest
    Next
End Sub


'*******************************************************************************
' InsertSortStringsStart (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()    - String  - Array to sort
' (In)     - bAscending     - Boolean - True to sort ascending, false descending
' (In)     - bCaseSensitive - Boolean - True for a case sensitive sort, false
'                                       for an insensitive one
'
' DESCRIPTION:
' User friendly entry point for InsertSortStrings
'*******************************************************************************
Public Sub InsertSortStringsStart(ListArray() As String, _
                                  Optional ByVal bAscending As Boolean = True, _
                                  Optional ByVal bCaseSensitive As Boolean = False)

    Dim lMin            As Long
    Dim lMax            As Long
    Dim lOrder          As Long
    Dim lCompareType    As Long

    lMin = LBound(ListArray)
    lMax = UBound(ListArray)
    
    If lMin = lMax Then
        Exit Sub
    End If
    
    ' Order Ascending or Descending?
    lOrder = IIf(bAscending, -1, 1)
    
    ' Case sensitive search or not?
    lCompareType = IIf(bCaseSensitive, vbBinaryCompare, vbTextCompare)
    
    InsertSortStrings ListArray, lMin, lMax, lOrder, lCompareType
End Sub


'*******************************************************************************
' InsertSortStrings (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()  - String - Array to sort
' (In)     - lMin         - Long   - Start of sorting region within array
' (In)     - lMax         - Long   - End of sorting region within array
' (In)     - lOrder       - Long   - Ascending is -1, Descending is +1, used
'                                    for comparison in StrComp
' (In)     - lCompareType - Long   - Either vbBinaryCompare or vbTextCompare,
'                                    used in StrComp function
'
' DESCRIPTION:
' Simple Insert Sort routine for strings, fast for small arrays as there is no
' recursion (say less than 60 values)
'*******************************************************************************
Private Sub InsertSortStrings(ListArray() As String, _
                              ByVal lMin As Long, _
                              ByVal lMax As Long, _
                              ByVal lOrder As Long, _
                              ByVal lCompareType As Long)
    
    Dim sValue  As String
    Dim lCount1 As Long
    Dim lCount2 As Long
    
    ' Loop through array shifting elements down to their correct place
    For lCount1 = lMin + 1 To lMax
        sValue = ListArray(lCount1)
        
        ' Find the place to put it
        For lCount2 = lCount1 - 1 To lMin Step -1
            If StrComp(ListArray(lCount2), sValue, lCompareType) <> lOrder Then
                Exit For
            End If
            ListArray(lCount2 + 1) = ListArray(lCount2)
        Next lCount2
        
        ' Insert it
        ListArray(lCount2 + 1) = sValue
    Next
End Sub


'*******************************************************************************
' QuickSortStringsStart (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()    - String  - Array to sort
' (In)     - bAscending     - Boolean - True to sort ascending, false descending
' (In)     - bCaseSensitive - Boolean - True for a case sensitive sort, false
'                                       for an insensitive one
'
' DESCRIPTION:
' User friendly entry point for QuickSortStrings
'*******************************************************************************
Public Sub QuickSortStringsStart(ListArray() As String, _
                                 Optional ByVal bAscending As Boolean = True, _
                                 Optional ByVal bCaseSensitive As Boolean = False)

    Dim lMin            As Long
    Dim lMax            As Long
    Dim lOrder          As Long
    Dim lCompareType    As Long

    lMin = LBound(ListArray)
    lMax = UBound(ListArray)
    
    If lMin = lMax Then
        Exit Sub
    End If
    
    ' Order Ascending or Descending?
    lOrder = IIf(bAscending, 1, -1)
    
    ' Case sensitive search or not?
    lCompareType = IIf(bCaseSensitive, vbBinaryCompare, vbTextCompare)
    
    QuickSortStrings ListArray, lMin, lMax, lOrder, lCompareType
End Sub


'*******************************************************************************
' QuickSortStrings (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()  - String - Array to sort
' (In)     - lLowerPoint  - Long   - Start of sorting region within array
' (In)     - lUpperPoint  - Long   - End of sorting region within array
' (In)     - lOrder       - Long   - Ascending is -1, Descending is +1, used
'                                    for comparison in StrComp
' (In)     - lCompareType - Long   - Either vbBinaryCompare or vbTextCompare,
'                                    used in StrComp function
'
' DESCRIPTION:
' Quick for large arrays, delegates to Insert Sort for small arrays and when
' partition is small
'*******************************************************************************
Private Sub QuickSortStrings(ListArray() As String, _
                             ByVal lLowerPoint As Long, _
                             ByVal lUpperPoint As Long, _
                             ByVal lOrder As Long, _
                             ByVal lCompareType As Long)
    
    Const DELEGATE_POINT As Long = 60
    
    Dim lMidPoint As Long
    
    ' Delegate to an insert sort if it is a small array (this is what makes this
    ' routine so much quicker than a standard quick sort routine).  The delegation
    ' point could be tuned if necessary.
    If (lUpperPoint - lLowerPoint) <= DELEGATE_POINT Then
        InsertSortStrings ListArray, lLowerPoint, lUpperPoint, lOrder, lCompareType
        Exit Sub
    End If

    ' Do the quick sort
    Do While lLowerPoint < lUpperPoint
        ' Find a mid point (split the array into partitions)
        lMidPoint = QuickSortStringsPartition(ListArray, lLowerPoint, lUpperPoint, lOrder, lCompareType)
        
        ' Recurively sort the smaller partition
        If (lMidPoint - lLowerPoint) <= (lUpperPoint - lMidPoint) Then
            QuickSortStrings ListArray, lLowerPoint, lMidPoint - 1, lOrder, lCompareType
            lLowerPoint = lMidPoint + 1
        Else
            QuickSortStrings ListArray, lMidPoint + 1, lUpperPoint, lOrder, lCompareType
            lUpperPoint = lMidPoint - 1
        End If
    Loop
End Sub


'*******************************************************************************
' QuickSortStringsPartition (FUNCTION)
'
' PARAMETERS:
' (In/Out) - ListArray()  - String - Array to sort
' (In)     - lLow         - Long   - Start of sorting region within array
' (In)     - lHigh        - Long   - End of sorting region within array
' (In)     - lOrder       - Long   - Ascending is -1, Descending is +1, used
'                                    for comparison in StrComp
' (In)     - lCompareType - Long   - Either vbBinaryCompare or vbTextCompare,
'                                    used in StrComp function
'
' RETURN VALUE:
' Long - New pivot point
'
' DESCRIPTION:
' Selects a pivot point and moves smaller entries to one side of it and larger
' entries to the other side of it.  Returns the position of the pivot point
' when finished.
'*******************************************************************************
Private Function QuickSortStringsPartition(ListArray() As String, _
                                           ByVal lLow As Long, _
                                           ByVal lHigh As Long, _
                                           ByVal lOrder As Long, _
                                           ByVal lCompareType As Long) As Long

    Dim lPivot      As Long
    Dim sPivot      As String
    Dim lLowCount   As Long
    Dim lHighCount  As Long
    Dim sTemp       As String

    ' Select pivot point and exchange with first element
    lPivot = lLow + (lHigh - lLow) \ 2
    sPivot = ListArray(lPivot)
    ListArray(lPivot) = ListArray(lLow)
    
    lLowCount = lLow + 1
    lHighCount = lHigh
    
    ' Continually loop moving entries smaller than pivot to one side and
    ' larger than pivot to other side
    Do
        Do While lLowCount < lHighCount
            If StrComp(sPivot, ListArray(lLowCount), lCompareType) <> lOrder Then
                Exit Do
            Else
                lLowCount = lLowCount + 1
            End If
        Loop
        
        Do While lHighCount >= lLowCount
            If StrComp(ListArray(lHighCount), sPivot, lCompareType) <> lOrder Then
                Exit Do
            Else
                lHighCount = lHighCount - 1
            End If
        Loop
        
        If lLowCount >= lHighCount Then
            Exit Do
        End If
        
        ' Swap the items
        sTemp = ListArray(lLowCount)
        ListArray(lLowCount) = ListArray(lHighCount)
        ListArray(lHighCount) = sTemp
        
        lHighCount = lHighCount - 1
        lLowCount = lLowCount + 1
    Loop
    
    ListArray(lLow) = ListArray(lHighCount)
    ListArray(lHighCount) = sPivot
    QuickSortStringsPartition = lHighCount
End Function


'*******************************************************************************
' MergeSortStringsStart (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()    - String  - Array to sort
' (In)     - bAscending     - Boolean - True to sort ascending, false descending
' (In)     - bCaseSensitive - Boolean - True for a case sensitive sort, false
'                                       for an insensitive one
'
' DESCRIPTION:
' User friendly entry point for MergeSortStrings
'*******************************************************************************
Public Sub MergeSortStringsStart(ListArray() As String, _
                                 Optional ByVal bAscending As Boolean = True, _
                                 Optional ByVal bCaseSensitive As Boolean = False)

    Dim lMin            As Long
    Dim lMax            As Long
    Dim lOrder          As Long
    Dim lCompareType    As Long

    Const DELEGATE_POINT As Long = 60
    
    lMin = LBound(ListArray)
    lMax = UBound(ListArray)
    
    If lMin = lMax Then
        Exit Sub
    End If
    
    ' Order Ascending or Descending?
    lOrder = IIf(bAscending, 1, -1)
    
    ' Case sensitive search or not?
    lCompareType = IIf(bCaseSensitive, vbBinaryCompare, vbTextCompare)
    
    ' Delegate to insert sort for very small arrays for speed
    If (lMax - lMin) > DELEGATE_POINT Then
        MergeSortStrings ListArray, lMin, lMax, lOrder, lCompareType
    Else
        InsertSortStrings ListArray, lMin, lMax, lOrder, lCompareType
    End If
End Sub


'*******************************************************************************
' MergeSortStrings (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()  - String - Array to sort
' (In)     - lLowerPoint  - Long   - Start of sorting region within array
' (In)     - lUpperPoint  - Long   - End of sorting region within array
' (In)     - lOrder       - Long   - Ascending is -1, Descending is +1, used
'                                    for comparison in StrComp
' (In)     - lCompareType - Long   - Either vbBinaryCompare or vbTextCompare,
'                                    used in StrComp function
'
' DESCRIPTION:
' Quick for large arrays
'*******************************************************************************
Private Sub MergeSortStrings(ListArray() As String, _
                             ByVal lLowerPoint As Long, _
                             ByVal lUpperPoint As Long, _
                             ByVal lOrder As Long, _
                             ByVal lCompareType As Long)
    
    Dim lMidPoint As Long
    
    If lUpperPoint > lLowerPoint Then
        ' Split the array up recursively and sort (divide and conquer)
        lMidPoint = (lUpperPoint + lLowerPoint) \ 2
        MergeSortStrings ListArray, lLowerPoint, lMidPoint, lOrder, lCompareType
        MergeSortStrings ListArray, lMidPoint + 1, lUpperPoint, lOrder, lCompareType
        
        ' Merge to sort
        MergeStrings ListArray, lLowerPoint, lMidPoint, lUpperPoint, lOrder, lCompareType
    End If
End Sub


'*******************************************************************************
' MergeStrings (SUB)
'
' PARAMETERS:
' (In/Out) - ListArray()  - String - Array to sort
' (In)     - lLowerPoint  - Long   - Start of sorting region within array
' (In)     - lMidPoint    - Long   - Mid point
' (In)     - lUpperPoint  - Long   - End of sorting region within array
' (In)     - lOrder       - Long   - Ascending is -1, Descending is +1, used
'                                    for comparison in StrComp
' (In)     - lCompareType - Long   - Either vbBinaryCompare or vbTextCompare,
'                                    used in StrComp function
'
' DESCRIPTION:
' Merge part of the MergeSort that merges the two sorted parts lLowerPoint to
' lMidPoint and lMidPoint+1 to lUpperPoint
'*******************************************************************************
Private Sub MergeStrings(ListArray() As String, _
                         ByVal lLowerPoint As Long, _
                         ByVal lMidPoint As Long, _
                         ByVal lUpperPoint As Long, _
                         ByVal lOrder As Long, _
                         ByVal lCompareType As Long)
    
    Dim TempList()      As String
    Dim lcount          As Long
    Dim lBottomPointer  As Long
    Dim lTopPointer     As Long
    Dim lCurrentPointer As Long
    
    ' Prepare temporary array
    ReDim TempArray(lLowerPoint To lUpperPoint)
    
    ' Make a temporary copy of the array
    For lcount = lLowerPoint To lUpperPoint
        TempArray(lcount) = ListArray(lcount)
    Next
    
    ' Initialise pointers that will be used to move through array
    lBottomPointer = lLowerPoint
    lTopPointer = lMidPoint + 1
    lCurrentPointer = lLowerPoint
    
    ' Loop until we have got to the end of one section
    Do While (lBottomPointer <= lMidPoint And lTopPointer <= lUpperPoint)
        If StrComp(TempArray(lBottomPointer), TempArray(lTopPointer), lCompareType) <> lOrder Then
            ListArray(lCurrentPointer) = TempArray(lBottomPointer)
            lBottomPointer = lBottomPointer + 1
        Else
            ListArray(lCurrentPointer) = TempArray(lTopPointer)
            lTopPointer = lTopPointer + 1
        End If
        lCurrentPointer = lCurrentPointer + 1
    Loop
    
    ' Copy the rest of the uncompleted section onto the end
    Do While lBottomPointer <= lMidPoint
        ListArray(lCurrentPointer) = TempArray(lBottomPointer)
        lBottomPointer = lBottomPointer + 1
        lCurrentPointer = lCurrentPointer + 1
    Loop
    Do While lTopPointer <= lUpperPoint
        ListArray(lCurrentPointer) = TempArray(lTopPointer)
        lTopPointer = lTopPointer + 1
        lCurrentPointer = lCurrentPointer + 1
    Loop
End Sub

