VERSION 5.00
Begin VB.Form FTestRijndael 
   Caption         =   "Test Rijndael"
   ClientHeight    =   5985
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7500
   LinkTopic       =   "Form1"
   ScaleHeight     =   5985
   ScaleWidth      =   7500
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkHex 
      Caption         =   "Hex Output?"
      Height          =   495
      Left            =   1560
      TabIndex        =   11
      Top             =   3900
      Value           =   1  'Checked
      Width           =   2715
   End
   Begin VB.TextBox txtPlainAgain 
      Height          =   315
      Left            =   1440
      TabIndex        =   9
      Top             =   5520
      Width           =   4155
   End
   Begin VB.TextBox txtEncrypted 
      Height          =   315
      Left            =   1440
      TabIndex        =   7
      Top             =   5160
      Width           =   4155
   End
   Begin VB.TextBox txtKey 
      Height          =   315
      Left            =   1440
      TabIndex        =   5
      Text            =   "Key"
      Top             =   4800
      Width           =   4155
   End
   Begin VB.TextBox txtPlain 
      Height          =   315
      Left            =   1440
      TabIndex        =   3
      Text            =   "Plain text"
      Top             =   4440
      Width           =   4155
   End
   Begin VB.CommandButton cmdTest2 
      Caption         =   "Test 2"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   3900
      Width           =   1215
   End
   Begin VB.TextBox txtResults 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   3015
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   1
      Top             =   720
      Width           =   7275
   End
   Begin VB.CommandButton cmdTest 
      Caption         =   "Test 1"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   60
      Width           =   1215
   End
   Begin VB.Label Label4 
      Caption         =   "Plain Again"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   5580
      Width           =   1215
   End
   Begin VB.Label Label3 
      Caption         =   "Encrypted"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   5220
      Width           =   1215
   End
   Begin VB.Label Label2 
      Caption         =   "Key"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   4860
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Plain"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   4500
      Width           =   1215
   End
End
Attribute VB_Name = "FTestRijndael"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'*******************************************************************************
' MODULE:       FTestRijndael
' FILENAME:     FTestRijndael.frm
' AUTHOR:       Phil Fresle
' CREATED:      19-Feb-2001
' COPYRIGHT:    Copyright 2001 Phil Fresle
' EMAIL:        phil@frez.co.uk
' WEB:          http://www.frez.co.uk
'
' DESCRIPTION:
' Tests out the Rijndael classes attached. Do not expect this to be blisteringly
' fast. Implement in C if you want speed.
'
' MODIFICATION HISTORY:
' 19-Feb-2001   Phil Fresle     Initial Version
' 03-Apr-2001   Phil Fresle     Also tests EncryptData and DecryptData functions
'*******************************************************************************
Option Explicit

'*******************************************************************************
' cmdTest_Click (SUB)
'*******************************************************************************
Private Sub cmdTest_Click()
    Dim oRijndael   As CRijndael
    Dim i           As Long
    Dim nb          As Long
    Dim nk          As Long
    Dim KEY(31)     As Byte
    Dim block(31)   As Byte
    Dim sTemp       As String
    Dim sResults    As String
    
    Set oRijndael = New CRijndael

    oRijndael.gentables

    For i = 0 To 31
        KEY(i) = 0
    Next

    KEY(0) = 1

    For i = 0 To 31
        block(i) = i
    Next

    sResults = ""
    
    For nb = 4 To 8 Step 2
        For nk = 4 To 8 Step 2

            oRijndael.gkey nb, nk, KEY
            sResults = sResults & "Block Size=" & nb * 32 & ",  Key size=" & nk * 32 & vbCrLf
            Debug.Print "Block Size=" & nb * 32 & ",  Key size=" & nk * 32

            sResults = sResults & "Plain" & vbCrLf
            Debug.Print "Plain"
            sTemp = ""
            For i = 0 To (nb * 4) - 1
                sTemp = sTemp & Right("0" & Hex(block(i)), 2)
            Next
            Debug.Print sTemp
            sResults = sResults & sTemp & vbCrLf

            oRijndael.Encrypt block
            sResults = sResults & "Encrypt" & vbCrLf
            Debug.Print "encrypt"
            sTemp = ""
            For i = 0 To (nb * 4) - 1
                sTemp = sTemp & Right("0" & Hex(block(i)), 2)
            Next
            Debug.Print sTemp
            sResults = sResults & sTemp & vbCrLf

            oRijndael.Decrypt block
            sTemp = ""
            Debug.Print "decrypt"
            sResults = sResults & "Decrypt" & vbCrLf
            For i = 0 To (nb * 4) - 1
                sTemp = sTemp & Right("0" & Hex(block(i)), 2)
            Next
            Debug.Print sTemp
            sResults = sResults & sTemp & vbCrLf
            sResults = sResults & vbCrLf
        Next
    Next
    txtResults.Text = sResults
End Sub

'*******************************************************************************
' cmdTest2_Click (SUB)
'
' Tests out string input into Rijndael
'*******************************************************************************
Private Sub cmdTest2_Click()
    Dim oTest           As CRijndael
    Dim sTemp           As String
    Dim bytIn()         As Byte
    Dim bytOut()        As Byte
    Dim bytPassword()   As Byte
    Dim bytClear()      As Byte
    Dim lCount          As Long
    Dim lLength         As Long
    
    Set oTest = New CRijndael
    
    ' Do a quick and dirty conversion of message and password to byte arrays, as the
    ' string is Unicode we will get two bytes per character. You might want to loop through
    ' instead if you are only dealing in ASCII using the ASC() function so you get one
    ' byte per character.
    ' NOTE: You need to be very careful here if you are encrypting on a system
    ' with one character set and then expecting to decrypt on a different system
    ' with a different character set (e.g. Japanese to US English). It will not be
    ' a problem if you are only using the ASCII range 0-127, but remember, we are
    ' encrypting/decrypting bytes not characters, the byte encryption/decryption
    ' will be correct, but your conversion between bytes and characters needs to be
    ' tested out.
    bytIn = txtPlain.Text
    bytPassword = txtKey.Text
    
    ' This is the alternate way for single bytes...
'    sTemp = txtPlain.Text
'    lLength = Len(sTemp)
'    ReDim bytIn(lLength - 1)
'    For lCount = 1 To lLength
'        bytIn(lCount - 1) = AscB(Mid(sTemp, lCount, 1))
'    Next
'    sTemp = txtKey.Text
'    lLength = Len(sTemp)
'    ReDim bytPassword(lLength - 1)
'    For lCount = 1 To lLength
'        bytPassword(lCount - 1) = AscB(Mid(sTemp, lCount, 1))
'    Next
    
    ' Encrypt the data
    bytOut = oTest.EncryptData(bytIn, bytPassword)
    
    ' Display in hex or not
    If chkHex.Value = vbChecked Then
        sTemp = ""
        For lCount = 0 To UBound(bytOut)
            sTemp = sTemp & Right("0" & Hex(bytOut(lCount)), 2)
        Next
        txtEncrypted.Text = sTemp
    Else
        sTemp = bytOut
    '   Alternate way...
'    lLength = UBound(bytOut) + 1
'    sTemp = String(lLength, " ")
'    For lCount = 1 To lLength
'        Mid(sTemp, lCount, 1) = Chr(bytOut(lCount - 1))
'    Next
        txtEncrypted.Text = sTemp
    End If
    
    ' NOTE: we are sending bytOut to be decrypted here. However, it is likely
    ' that we will need to reconstruct bytOut, say from the file it has been dumped
    ' in as a string. If it was dumped out in hex it can be reconstructed like this
    ' where sTemp is the string containing the encrypted data...
    If chkHex.Value = vbChecked Then
        lLength = Len(sTemp)
        ReDim bytOut((lLength \ 2) - 1)
        For lCount = 1 To lLength Step 2
          bytOut(lCount \ 2) = CByte("&H" & Mid(sTemp, lCount, 2))
        Next
    Else
        bytOut = sTemp
        ' Alternate way
'        lLength = Len(sTemp)
'        ReDim bytOut(lLength - 1)
'        For lCount = 1 To lLength
'          bytOut(lCount - 1) = AscB(Mid(sTemp, lCount, 1))
'        Next
    End If
    
    ' Decrypt
    bytClear = oTest.DecryptData(bytOut, bytPassword)
    
    ' Quick and dirty conversion back to a string. If we earlier looped using the ASC() function
    ' to get one byte per character, we will now need to do the opposite and loop using
    ' the CHR() function to put the string back together again.
    txtPlainAgain.Text = bytClear

    ' This is the alternate way for single bytes...
'    lLength = UBound(bytClear) + 1
'    sTemp = String(lLength, " ")
'    For lCount = 1 To lLength
'        Mid(sTemp, lCount, 1) = Chr(bytClear(lCount - 1))
'    Next
'    txtPlainAgain.Text = sTemp
End Sub

