VERSION 5.00
Begin VB.Form FMD5Test 
   Caption         =   "MD5 Test"
   ClientHeight    =   6540
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9300
   BeginProperty Font 
      Name            =   "Courier New"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   ScaleHeight     =   6540
   ScaleWidth      =   9300
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdTest 
      Caption         =   "Test"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   1215
   End
End
Attribute VB_Name = "FMD5Test"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'*******************************************************************************
' MODULE:       FMD5Test
' FILENAME:     C:\My Code\vb\md5\FMD5Test.frm
' AUTHOR:       Phil Fresle
' CREATED:      16-Feb-2001
' COPYRIGHT:    Copyright 2001 Frez Systems Limited. All Rights Reserved.
'
' DESCRIPTION:
' This form tests the CMD5 class for generating an MD5 digest of a string
' message. See the class module for further information.
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
' Should you wish to commission some derivative work based on this code provided
' here, or any consultancy work, please do not hesitate to contact us.
'
' Web Site:  http://www.frez.co.uk
' E-mail:    sales@frez.co.uk
'
' MODIFICATION HISTORY:
' 1.0       16-Feb-2001
'           Phil Fresle
'           Initial Version
'*******************************************************************************
Option Explicit

'*******************************************************************************
' cmdTest_Click (SUB)
'
' DESCRIPTION:
' Runs the test
'*******************************************************************************
Private Sub cmdTest_Click()
    Dim oMD5 As CMD5
    
    Set oMD5 = New CMD5

    Me.Cls
    Me.Print ""
    Me.Print ""
    
    Me.Print ""
    Me.Print "MD5("""")"
    Me.Print "Expected: d41d8cd98f00b204e9800998ecf8427e"
    Me.Print "Actual:   " & oMD5.MD5("")
    
    Me.Print ""
    Me.Print "MD5(""a"")"
    Me.Print "Expected: 0cc175b9c0f1b6a831c399e269772661"
    Me.Print "Actual:   " & oMD5.MD5("a")
    
    Me.Print ""
    Me.Print "MD5(""abc"")"
    Me.Print "Expected: 900150983cd24fb0d6963f7d28e17f72"
    Me.Print "Actual:   " & oMD5.MD5("abc")
    
    Me.Print ""
    Me.Print "MD5(""message digest"")"
    Me.Print "Expected: f96b697d7cb7938d525a2f31aaf161d0"
    Me.Print "Actual:   " & oMD5.MD5("message digest")
    
    Me.Print ""
    Me.Print "MD5(""abcdefghijklmnopqrstuvwxyz"")"
    Me.Print "Expected: c3fcd3d76192e4007dfb496cca67e13b"
    Me.Print "Actual:   " & oMD5.MD5("abcdefghijklmnopqrstuvwxyz")
    
    Me.Print ""
    Me.Print "MD5(""ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"")"
    Me.Print "Expected: d174ab98d277d9f5a5611c2c9f419d9f"
    Me.Print "Actual:   " & oMD5.MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
    
    Me.Print ""
    Me.Print "MD5(""12345678901234567890123456789012345678901234567890123456789012345678901234567890"")"
    Me.Print "Expected: 57edf4a22be3c955ac49da2e2107b67a"
    Me.Print "Actual:   " & oMD5.MD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
    
    Set oMD5 = Nothing
End Sub
