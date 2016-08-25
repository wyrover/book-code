<HTML>
<%
' This VBScript ASP file tests the accompanying rijndael.asp for
' encryption using the Rijndael AES block cipher algorithm
%>
<!--#include file="rijndael.asp"-->
<%
    Dim i
    Dim nb
    Dim nk
    Dim key(31)
    Dim block(31)
    Dim sTemp
    Dim sPlain
    Dim sPassword
    Dim bytIn()
    Dim bytPassword()
    Dim lCount
    
    gentables

    For i = 0 To 31
        key(i) = 0
    Next

    key(0) = 1

    For i = 0 To 31
        block(i) = i
    Next

    For nb = 4 To 8 Step 2
        For nk = 4 To 8 Step 2

            gkey nb, nk, key
            Response.Write "Block Size=" & nb * 32 & ",  Key size=" & nk * 32 & "<BR>"

            Response.Write "Plain<BR>"
            sTemp = ""
            For i = 0 To (nb * 4) - 1
                sTemp = sTemp & Right("0" & Hex(block(i)), 2)
            Next
            Response.Write sTemp & "<BR>"

            encrypt block
            Response.Write "Encrypt<BR>"
            sTemp = ""
            For i = 0 To (nb * 4) - 1
                sTemp = sTemp & Right("0" & Hex(block(i)), 2)
            Next
            Response.Write sTemp & "<BR>"

            decrypt block
            sTemp = ""
            Response.Write "Decrypt<BR>"
            For i = 0 To (nb * 4) - 1
                sTemp = sTemp & Right("0" & Hex(block(i)), 2)
            Next
            Response.Write sTemp & "<BR>"
            Response.Write "<BR>"
        Next
    Next

    Response.Write "<BR>"
    Response.Write "<BR>"

    sPlain = "Plain text"
    sPassword = "Key"

    Response.Write "Message=" & sPlain & "<BR>"
    Response.Write "Key=" & sPassword & "<BR>"

    lLength = Len(sPlain)
    ReDim bytIn(lLength-1)
    For lCount = 1 To lLength
        bytIn(lCount-1)=CByte(AscB(Mid(sPlain,lCount,1)))
    Next
    lLength = Len(sPassword)
    ReDim bytPassword(lLength-1)
    For lCount = 1 To lLength
        bytPassword(lCount-1)=CByte(AscB(Mid(sPassword,lCount,1)))
    Next
    
    bytOut = EncryptData(bytIn, bytPassword)
    
    sTemp = ""
    For lCount = 0 To UBound(bytOut)
        sTemp = sTemp & Right("0" & Hex(bytOut(lCount)), 2)
    Next
    Response.Write "Encrypted=" & sTemp & "<BR>"
    
    bytClear = DecryptData(bytOut, bytPassword)

    lLength = UBound(bytClear) + 1
    sTemp = ""
    For lCount = 0 To lLength - 1
        sTemp = sTemp & Chr(bytClear(lCount))
    Next
    Response.Write "Decrypted=" & sTemp & "<BR>"

    Response.Write "<BR>"
%>
</HTML>
