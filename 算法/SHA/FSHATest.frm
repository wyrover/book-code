VERSION 5.00
Begin VB.Form FSHATest 
   Caption         =   "SHA Test"
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
   Begin VB.CommandButton cmdTest256 
      Caption         =   "Test SHA-256"
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
Attribute VB_Name = "FSHATest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'*******************************************************************************
' MODULE:       FSHATest
' FILENAME:     FSHATest.frm
' AUTHOR:       Phil Fresle
' CREATED:      10-Apr-2001
' COPYRIGHT:    Copyright 2001 Phil Fresle. All Rights Reserved.
'
' DESCRIPTION:
' This form tests the CSHA256 class for generating a SHA-256 digest of a string
' message. See the class module for further details
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
' here, or any consultancy work, please do not hesitate to contact me.
'
' Web Site:  http://www.frez.co.uk
' E-mail:    sales@frez.co.uk
'
' MODIFICATION HISTORY:
' 10-Apr-2001   Phil Fresle     Initial Version
'*******************************************************************************
Option Explicit

'*******************************************************************************
' cmdTest256_Click (SUB)
'
' DESCRIPTION:
' Runs the test
'*******************************************************************************
Private Sub cmdTest256_Click()
    Dim oSHA256 As CSHA256
    
    Set oSHA256 = New CSHA256

    Me.Cls
    Me.Print ""
    Me.Print ""
    
    Me.Print ""
    Me.Print "SHA256(""abc"")"
    Me.Print "Expected: ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"
    Me.Print "Actual:   " & oSHA256.SHA256("abc")
        
    Me.Print ""
    Me.Print "SHA256(""abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"")"
    Me.Print "Expected: 248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"
    Me.Print "Actual:   " & oSHA256.SHA256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")

    Set oSHA256 = Nothing
End Sub
