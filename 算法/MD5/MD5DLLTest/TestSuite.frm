VERSION 5.00
Begin VB.Form TestSuite 
   Caption         =   "MD5 Test Suite - Uses MD5.DLL"
   ClientHeight    =   6135
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9390
   LinkTopic       =   "Form1"
   ScaleHeight     =   6135
   ScaleWidth      =   9390
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnRunTest 
      Caption         =   "Run Test"
      Height          =   375
      Left            =   3360
      TabIndex        =   35
      Top             =   5520
      Width           =   2535
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   6
      Left            =   1440
      TabIndex        =   34
      Top             =   5040
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "57edf4a22be3c955ac49da2e2107b67a"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   6
      Left            =   1440
      TabIndex        =   33
      Top             =   4800
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   6
      Left            =   360
      TabIndex        =   32
      Top             =   5040
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   6
      Left            =   360
      TabIndex        =   31
      Top             =   4800
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ('12345678901234567890123456789012345678901234567890123456789012345678901234567890')"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   6
      Left            =   120
      TabIndex        =   30
      Top             =   4560
      Width           =   9135
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   5
      Left            =   1440
      TabIndex        =   29
      Top             =   4320
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "d174ab98d277d9f5a5611c2c9f419d9f"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   5
      Left            =   1440
      TabIndex        =   28
      Top             =   4080
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   5
      Left            =   360
      TabIndex        =   27
      Top             =   4320
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   5
      Left            =   360
      TabIndex        =   26
      Top             =   4080
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   5
      Left            =   120
      TabIndex        =   25
      Top             =   3840
      Width           =   7455
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   4
      Left            =   1440
      TabIndex        =   24
      Top             =   3600
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "c3fcd3d76192e4007dfb496cca67e13b"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   4
      Left            =   1440
      TabIndex        =   23
      Top             =   3360
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   4
      Left            =   360
      TabIndex        =   22
      Top             =   3600
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   4
      Left            =   360
      TabIndex        =   21
      Top             =   3360
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ('abcdefghijklmnopqrstuvwxyz')"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   4
      Left            =   120
      TabIndex        =   20
      Top             =   3120
      Width           =   3255
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   3
      Left            =   1440
      TabIndex        =   19
      Top             =   2880
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "f96b697d7cb7938d525a2f31aaf161d0"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   3
      Left            =   1440
      TabIndex        =   18
      Top             =   2640
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   3
      Left            =   360
      TabIndex        =   17
      Top             =   2880
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   3
      Left            =   360
      TabIndex        =   16
      Top             =   2640
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ('message digest')"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   3
      Left            =   120
      TabIndex        =   15
      Top             =   2400
      Width           =   2775
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   2
      Left            =   1440
      TabIndex        =   14
      Top             =   2160
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "900150983cd24fb0d6963f7d28e17f72"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   2
      Left            =   1440
      TabIndex        =   13
      Top             =   1920
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   2
      Left            =   360
      TabIndex        =   12
      Top             =   2160
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   2
      Left            =   360
      TabIndex        =   11
      Top             =   1920
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ('abc')"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   10
      Top             =   1680
      Width           =   1095
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   1
      Left            =   1440
      TabIndex        =   9
      Top             =   1440
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "0cc175b9c0f1b6a831c399e269772661"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   1
      Left            =   1440
      TabIndex        =   8
      Top             =   1200
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   1
      Left            =   360
      TabIndex        =   7
      Top             =   1440
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   1
      Left            =   360
      TabIndex        =   6
      Top             =   1200
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ('a')"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   5
      Top             =   960
      Width           =   1815
   End
   Begin VB.Label lblResults 
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   0
      Left            =   1440
      TabIndex        =   4
      Top             =   720
      Width           =   3015
   End
   Begin VB.Label lblExpectedV 
      Caption         =   "d41d8cd98f00b204e9800998ecf8427e"
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   0
      Left            =   1440
      TabIndex        =   3
      Top             =   480
      Width           =   3015
   End
   Begin VB.Label lblActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   0
      Left            =   360
      TabIndex        =   2
      Top             =   720
      Width           =   855
   End
   Begin VB.Label lblExpected 
      Caption         =   "Expected"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   255
      Index           =   0
      Left            =   360
      TabIndex        =   1
      Top             =   480
      Width           =   975
   End
   Begin VB.Label lblTest 
      Caption         =   "MD5 ()"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   735
   End
End
Attribute VB_Name = "TestSuite"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
'/******************************************************************************
' *  Copyright (C) 2000 by Robert Hubley.                                      *
' *  All rights reserved.                                                      *
' *                                                                            *
' *  This software is provided ``AS IS'' and any express or implied            *
' *  warranties, including, but not limited to, the implied warranties of      *
' *  merchantability and fitness for a particular purpose, are disclaimed.     *
' *  In no event shall the authors be liable for any direct, indirect,         *
' *  incidental, special, exemplary, or consequential damages (including, but  *
' *  not limited to, procurement of substitute goods or services; loss of use, *
' *  data, or profits; or business interruption) however caused and on any     *
' *  theory of liability, whether in contract, strict liability, or tort       *
' *  (including negligence or otherwise) arising in any way out of the use of  *
' *  this software, even if advised of the possibility of such damage.         *
' *                                                                            *
' ******************************************************************************
'
'  Form: TestSuite
'
'  DESCRIPTION:
'   A short demonstration form which calls the MD5.DLL library using a
'   standard test suite of values.  The results are displayed next to the
'   expected return set for comparison.
'
'  AUTHOR:
'     Robert M. Hubley 12/1999
'
'
'  NOTES:
'
'
'  CHANGE HISTORY:
'
'     1.0.0  RMH    2000/1/5      Original version
'
'

Private Sub btnRunTest_Click()
    lblResults(0).Caption = DigestStrToHexStr("")
    lblResults(1).Caption = DigestStrToHexStr("a")
    lblResults(2).Caption = DigestStrToHexStr("abc")
    lblResults(3).Caption = DigestStrToHexStr("message digest")
    lblResults(4).Caption = DigestStrToHexStr("abcdefghijklmnopqrstuvwxyz")
    lblResults(5).Caption = DigestStrToHexStr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
    lblResults(6).Caption = DigestStrToHexStr("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
End Sub

