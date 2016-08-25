Attribute VB_Name = "MD5Driver"
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
'  Module: MD5Driver
'
'  DESCRIPTION:
'     This is a set of support routines which call MD5 Message Digest functions.
'     These functions are written in C and compiled to a DLL (md5.dll) which
'     must be located in the system32 or program director in order for these
'     routines to work.
'
'     MD5 algorithm produces a 128 bit digital fingerprint (signature) from an
'     dataset of arbitrary length.  For details see RFC 1321.
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

'=
'= Compiler Flags
'=
Option Explicit


'=
'= User Defined Types
'=
Private Type MD5_CONTEXT
    State(3) As Long
    Count(1) As Long
    buffer(63) As Byte
End Type


'=
'= API Declarations
'=
Private Declare Sub MD5Init Lib "md5.dll" _
    (lpContext As MD5_CONTEXT)

Private Declare Sub MD5Update Lib "md5.dll" _
    (lpContext As MD5_CONTEXT, lpBuffer As Any, ByVal hInputLength As Long)

Private Declare Sub MD5Final Lib "md5.dll" _
    (lpDigest As Any, lpContext As MD5_CONTEXT)


'
' Digest a file to a hex string
'
Public Function DigestFileToHexStr(FileName As String) As String
    Dim udtContext As MD5_CONTEXT
    Dim bytDigest(15) As Byte
    Dim bytData(63) As Byte
    Open FileName For Binary Access Read As #1
    MD5Init udtContext
    Do While Not EOF(1)
        Get #1, , bytData
        If Loc(1) < LOF(1) Then
            MD5Update udtContext, bytData(0), 64
        End If
    Loop
    MD5Update udtContext, bytData(0), LOF(1) Mod 64
    Close #1
    MD5Final bytDigest(0), udtContext
    DigestFileToHexStr = DigestToString(bytDigest)
End Function


'
' Function to digest a text string and output the result as a string
' of hexadecimal characters.
'
Public Function DigestStrToHexStr(SourceString As String) As String
    Dim udtContext As MD5_CONTEXT
    Dim bytDigest(15) As Byte
    Dim bytData() As Byte
    
    MD5Init udtContext
    bytData = StringToArray(SourceString)
    MD5Update udtContext, bytData(0), Len(SourceString)
    MD5Final bytDigest(0), udtContext
    DigestStrToHexStr = DigestToString(bytDigest)
End Function


'
' Run the MD5 test suite
'
Private Sub RunMD5TestSuite()
    Dim strOutput As String
    
    Debug.Print "MD5 Test Suite:"
    Debug.Print "MD5 ()"
    Debug.Print "    Actual   = " & DigestStrToHexStr("")
    Debug.Print "    Expected = d41d8cd98f00b204e9800998ecf8427e"
    Debug.Print "MD5 ('a')"
    Debug.Print "    Actual   = " & DigestStrToHexStr("a")
    Debug.Print "    Expected = 0cc175b9c0f1b6a831c399e269772661"
    Debug.Print "MD5 ('abc')"
    Debug.Print "    Actual   = " & DigestStrToHexStr("abc")
    Debug.Print "    Expected = 900150983cd24fb0d6963f7d28e17f72"
    Debug.Print "MD5 ('message digest')"
    Debug.Print "    Actual   = " & DigestStrToHexStr("message digest")
    Debug.Print "    Expected = f96b697d7cb7938d525a2f31aaf161d0"
    Debug.Print "MD5 ('abcdefghijklmnopqrstuvwxyz')"
    Debug.Print "    Actual   = " & DigestStrToHexStr("abcdefghijklmnopqrstuvwxyz")
    Debug.Print "    Expected = c3fcd3d76192e4007dfb496cca67e13b"
    Debug.Print "MD5 ('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')"
    Debug.Print "    Actual   = " & DigestStrToHexStr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
    Debug.Print "    Expected = d174ab98d277d9f5a5611c2c9f419d9f"
    Debug.Print "MD5 ('12345678901234567890123456789012345678901234567890123456789012345678901234567890')"
    Debug.Print "    Actual   = " & DigestStrToHexStr("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
    Debug.Print "    Expected = 57edf4a22be3c955ac49da2e2107b67a"
    
End Sub


'
' A utility function which converts a string into an array of
' bytes.
'
Private Function StringToArray(InString As String) As Byte()
    Dim lngI As Long
    Dim bytBuffer() As Byte
    ReDim bytBuffer(Len(InString))
    For lngI = 0 To Len(InString) - 1
        bytBuffer(lngI) = Asc(Mid(InString, lngI + 1, 1))
    Next lngI
    StringToArray = bytBuffer
End Function

'
' Return a HEX string representation of a byte array
'
Private Function DigestToString(Digest() As Byte) As String
    Dim lngI As Long
    For lngI = 0 To UBound(Digest)
        ' Pad with a "0" character if the HEX byte is less than 16 decimal
        If Digest(lngI) < 16 Then
            DigestToString = DigestToString & "0" & Hex(Digest(lngI))
        Else
            DigestToString = DigestToString & LCase(Hex(Digest(lngI)))
        End If
    Next lngI
End Function







