Attribute VB_Name = "modTime"
Option Explicit

' http://www.syix.com/wpsjr1/index.html

Declare Function QueryPerformanceCounter Lib "kernel32.dll" (lpPerformanceCount As Currency) As Long
Declare Function QueryPerformanceFrequency Lib "kernel32" (lpFrequency As Currency) As Long

Dim cStartTime As Currency
Dim cPerfFreq As Currency

Public Function Start() As Long
  If QueryPerformanceFrequency(cPerfFreq) = False Then
    Debug.Print "High-perf counter not supported"
  End If
  QueryPerformanceCounter cStartTime
End Function

Public Function Finish() As Long
  Dim cCurrentTime As Currency
  
  QueryPerformanceCounter cCurrentTime
  Finish = 1000 * (cCurrentTime - cStartTime) / cPerfFreq
End Function

Public Function FinishC() As Currency
  Dim cCurrentTime As Currency
  
  QueryPerformanceCounter cCurrentTime
  FinishC = cCurrentTime
End Function
