VERSION 5.00
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "¼ì²é¼æÈÝÄ£Ê½"
   ClientHeight    =   3150
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4575
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   3150
   ScaleWidth      =   4575
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton Command1 
      Caption         =   "Get Info"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   2520
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Type OSVERSIONINFO
    dwOSVersionInfoSize As Long
    dwMajorVersion As Long
    dwMinorVersion As Long
    dwBuildNumber As Long
    dwPlatformId As Long
    szCSDVersion As String * 128 ' Maintenance string for PSS usage
End Type

Private Declare Function GetVersionExA Lib "kernel32.dll" (lpVersionInformation As OSVERSIONINFO) As Long
Private Declare Function RtlGetVersion Lib "ntdll.dll" (lpVersionInformation As OSVERSIONINFO) As Long

Private Sub Command1_Click()
    Dim osv1 As OSVERSIONINFO, osv2 As OSVERSIONINFO
    'way 1
    osv1.dwOSVersionInfoSize = Len(osv1)
    Call GetVersionExA(osv1)
    Print "Get Build Number by GetVersionExA:"; osv1.dwBuildNumber
    'way 2
    osv2.dwOSVersionInfoSize = Len(osv2)
    Call RtlGetVersion(osv2)
    Print "Get Build Number by RtlGetVersion:"; osv2.dwBuildNumber
End Sub

