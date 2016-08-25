VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CRijndael"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'*******************************************************************************
' MODULE:       CRijndael
' FILENAME:     CRijndael.cls
' AUTHOR:       Phil Fresle
' CREATED:      16-Feb-2001
' COPYRIGHT:    Copyright 2001 Phil Fresle
' EMAIL:        phil@frez.co.uk
' WEB:          http://www.frez.co.uk
'
' DESCRIPTION:
' Implementation of the AES Rijndael Block Cipher. Inspired by Mike Scott's
' implementation in C. Permission for free direct or derivative use is granted
' subject to compliance with any conditions that the originators of the
' algorithm place on its exploitation.
'
' MODIFICATION HISTORY:
' 16-Feb-2001   Phil Fresle     Initial Version
' 03-Apr-2001   Phil Fresle     Added EncryptData and DecryptData functions to
'                               make it easier to use by VB developers for
'                               encrypting and decrypting strings. These procs
'                               take large byte arrays, the resultant encoded
'                               data includes the message length inserted on
'                               the front four bytes prior to encryption.
' 19-Apr-2001   Phil Fresle     Thanks to Paolo Migliaccio for finding a bug
'                               with 256 bit key. Problem was in the gkey
'                               function. Now properly matches NIST values.
'*******************************************************************************
Option Explicit

Private m_lOnBits(30)   As Long
Private m_l2Power(30)   As Long
Private m_bytOnBits(7)  As Byte
Private m_byt2Power(7)  As Byte

Private m_InCo(3) As Byte

Private m_fbsub(255)    As Byte
Private m_rbsub(255)    As Byte
Private m_ptab(255)     As Byte
Private m_ltab(255)     As Byte
Private m_ftable(255)   As Long
Private m_rtable(255)   As Long
Private m_rco(29)       As Long

Private m_Nk        As Long
Private m_Nb        As Long
Private m_Nr        As Long
Private m_fi(23)    As Byte
Private m_ri(23)    As Byte
Private m_fkey(119) As Long
Private m_rkey(119) As Long

Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" _
    (ByVal Destination As Any, ByVal Source As Any, ByVal Length As Long)

'*******************************************************************************
' Class_Initialize (SUB)
'*******************************************************************************
Private Sub Class_Initialize()
    m_InCo(0) = &HB
    m_InCo(1) = &HD
    m_InCo(2) = &H9
    m_InCo(3) = &HE
    
    ' Could have done this with a loop calculating each value, but simply
    ' assigning the values is quicker - BITS SET FROM RIGHT
    m_bytOnBits(0) = 1          ' 00000001
    m_bytOnBits(1) = 3          ' 00000011
    m_bytOnBits(2) = 7          ' 00000111
    m_bytOnBits(3) = 15         ' 00001111
    m_bytOnBits(4) = 31         ' 00011111
    m_bytOnBits(5) = 63         ' 00111111
    m_bytOnBits(6) = 127        ' 01111111
    m_bytOnBits(7) = 255        ' 11111111
    
    ' Could have done this with a loop calculating each value, but simply
    ' assigning the values is quicker - POWERS OF 2
    m_byt2Power(0) = 1          ' 00000001
    m_byt2Power(1) = 2          ' 00000010
    m_byt2Power(2) = 4          ' 00000100
    m_byt2Power(3) = 8          ' 00001000
    m_byt2Power(4) = 16         ' 00010000
    m_byt2Power(5) = 32         ' 00100000
    m_byt2Power(6) = 64         ' 01000000
    m_byt2Power(7) = 128        ' 10000000
    
    ' Could have done this with a loop calculating each value, but simply
    ' assigning the values is quicker - BITS SET FROM RIGHT
    m_lOnBits(0) = 1            ' 00000000000000000000000000000001
    m_lOnBits(1) = 3            ' 00000000000000000000000000000011
    m_lOnBits(2) = 7            ' 00000000000000000000000000000111
    m_lOnBits(3) = 15           ' 00000000000000000000000000001111
    m_lOnBits(4) = 31           ' 00000000000000000000000000011111
    m_lOnBits(5) = 63           ' 00000000000000000000000000111111
    m_lOnBits(6) = 127          ' 00000000000000000000000001111111
    m_lOnBits(7) = 255          ' 00000000000000000000000011111111
    m_lOnBits(8) = 511          ' 00000000000000000000000111111111
    m_lOnBits(9) = 1023         ' 00000000000000000000001111111111
    m_lOnBits(10) = 2047        ' 00000000000000000000011111111111
    m_lOnBits(11) = 4095        ' 00000000000000000000111111111111
    m_lOnBits(12) = 8191        ' 00000000000000000001111111111111
    m_lOnBits(13) = 16383       ' 00000000000000000011111111111111
    m_lOnBits(14) = 32767       ' 00000000000000000111111111111111
    m_lOnBits(15) = 65535       ' 00000000000000001111111111111111
    m_lOnBits(16) = 131071      ' 00000000000000011111111111111111
    m_lOnBits(17) = 262143      ' 00000000000000111111111111111111
    m_lOnBits(18) = 524287      ' 00000000000001111111111111111111
    m_lOnBits(19) = 1048575     ' 00000000000011111111111111111111
    m_lOnBits(20) = 2097151     ' 00000000000111111111111111111111
    m_lOnBits(21) = 4194303     ' 00000000001111111111111111111111
    m_lOnBits(22) = 8388607     ' 00000000011111111111111111111111
    m_lOnBits(23) = 16777215    ' 00000000111111111111111111111111
    m_lOnBits(24) = 33554431    ' 00000001111111111111111111111111
    m_lOnBits(25) = 67108863    ' 00000011111111111111111111111111
    m_lOnBits(26) = 134217727   ' 00000111111111111111111111111111
    m_lOnBits(27) = 268435455   ' 00001111111111111111111111111111
    m_lOnBits(28) = 536870911   ' 00011111111111111111111111111111
    m_lOnBits(29) = 1073741823  ' 00111111111111111111111111111111
    m_lOnBits(30) = 2147483647  ' 01111111111111111111111111111111
    
    ' Could have done this with a loop calculating each value, but simply
    ' assigning the values is quicker - POWERS OF 2
    m_l2Power(0) = 1            ' 00000000000000000000000000000001
    m_l2Power(1) = 2            ' 00000000000000000000000000000010
    m_l2Power(2) = 4            ' 00000000000000000000000000000100
    m_l2Power(3) = 8            ' 00000000000000000000000000001000
    m_l2Power(4) = 16           ' 00000000000000000000000000010000
    m_l2Power(5) = 32           ' 00000000000000000000000000100000
    m_l2Power(6) = 64           ' 00000000000000000000000001000000
    m_l2Power(7) = 128          ' 00000000000000000000000010000000
    m_l2Power(8) = 256          ' 00000000000000000000000100000000
    m_l2Power(9) = 512          ' 00000000000000000000001000000000
    m_l2Power(10) = 1024        ' 00000000000000000000010000000000
    m_l2Power(11) = 2048        ' 00000000000000000000100000000000
    m_l2Power(12) = 4096        ' 00000000000000000001000000000000
    m_l2Power(13) = 8192        ' 00000000000000000010000000000000
    m_l2Power(14) = 16384       ' 00000000000000000100000000000000
    m_l2Power(15) = 32768       ' 00000000000000001000000000000000
    m_l2Power(16) = 65536       ' 00000000000000010000000000000000
    m_l2Power(17) = 131072      ' 00000000000000100000000000000000
    m_l2Power(18) = 262144      ' 00000000000001000000000000000000
    m_l2Power(19) = 524288      ' 00000000000010000000000000000000
    m_l2Power(20) = 1048576     ' 00000000000100000000000000000000
    m_l2Power(21) = 2097152     ' 00000000001000000000000000000000
    m_l2Power(22) = 4194304     ' 00000000010000000000000000000000
    m_l2Power(23) = 8388608     ' 00000000100000000000000000000000
    m_l2Power(24) = 16777216    ' 00000001000000000000000000000000
    m_l2Power(25) = 33554432    ' 00000010000000000000000000000000
    m_l2Power(26) = 67108864    ' 00000100000000000000000000000000
    m_l2Power(27) = 134217728   ' 00001000000000000000000000000000
    m_l2Power(28) = 268435456   ' 00010000000000000000000000000000
    m_l2Power(29) = 536870912   ' 00100000000000000000000000000000
    m_l2Power(30) = 1073741824  ' 01000000000000000000000000000000
End Sub

'*******************************************************************************
' LShift (FUNCTION)
'*******************************************************************************
Private Function LShift(ByVal lValue As Long, _
                        ByVal iShiftBits As Integer) As Long
    If iShiftBits = 0 Then
        LShift = lValue
        Exit Function
    ElseIf iShiftBits = 31 Then
        If lValue And 1 Then
            LShift = &H80000000
        Else
            LShift = 0
        End If
        Exit Function
    ElseIf iShiftBits < 0 Or iShiftBits > 31 Then
        Err.Raise 6
    End If
    
    If (lValue And m_l2Power(31 - iShiftBits)) Then
        LShift = ((lValue And m_lOnBits(31 - (iShiftBits + 1))) * _
            m_l2Power(iShiftBits)) Or &H80000000
    Else
        LShift = ((lValue And m_lOnBits(31 - iShiftBits)) * _
            m_l2Power(iShiftBits))
    End If
End Function

'*******************************************************************************
' RShift (FUNCTION)
'*******************************************************************************
Private Function RShift(ByVal lValue As Long, _
                        ByVal iShiftBits As Integer) As Long
    If iShiftBits = 0 Then
        RShift = lValue
        Exit Function
    ElseIf iShiftBits = 31 Then
        If lValue And &H80000000 Then
            RShift = 1
        Else
            RShift = 0
        End If
        Exit Function
    ElseIf iShiftBits < 0 Or iShiftBits > 31 Then
        Err.Raise 6
    End If
    
    RShift = (lValue And &H7FFFFFFE) \ m_l2Power(iShiftBits)
    
    If (lValue And &H80000000) Then
        RShift = (RShift Or (&H40000000 \ m_l2Power(iShiftBits - 1)))
    End If
End Function

'*******************************************************************************
' LShiftByte (FUNCTION)
'*******************************************************************************
Private Function LShiftByte(ByVal bytValue As Byte, _
                            ByVal bytShiftBits As Byte) As Byte
    If bytShiftBits = 0 Then
        LShiftByte = bytValue
        Exit Function
    ElseIf bytShiftBits = 7 Then
        If bytValue And 1 Then
            LShiftByte = &H80
        Else
            LShiftByte = 0
        End If
        Exit Function
    ElseIf bytShiftBits < 0 Or bytShiftBits > 7 Then
        Err.Raise 6
    End If
    
    LShiftByte = ((bytValue And m_bytOnBits(7 - bytShiftBits)) * _
        m_byt2Power(bytShiftBits))
End Function

'*******************************************************************************
' RShiftByte (FUNCTION)
'*******************************************************************************
Private Function RShiftByte(ByVal bytValue As Byte, _
                            ByVal bytShiftBits As Byte) As Byte
    If bytShiftBits = 0 Then
        RShiftByte = bytValue
        Exit Function
    ElseIf bytShiftBits = 7 Then
        If bytValue And &H80 Then
            RShiftByte = 1
        Else
            RShiftByte = 0
        End If
        Exit Function
    ElseIf bytShiftBits < 0 Or bytShiftBits > 7 Then
        Err.Raise 6
    End If
    
    RShiftByte = bytValue \ m_byt2Power(bytShiftBits)
End Function

'*******************************************************************************
' RotateLeft (FUNCTION)
'*******************************************************************************
Private Function RotateLeft(ByVal lValue As Long, _
                            ByVal iShiftBits As Integer) As Long
    RotateLeft = LShift(lValue, iShiftBits) Or RShift(lValue, (32 - iShiftBits))
End Function

''*******************************************************************************
'' RotateLeftByte (FUNCTION)
'*******************************************************************************
Private Function RotateLeftByte(ByVal bytValue As Byte, _
                                ByVal bytShiftBits As Byte) As Byte
    RotateLeftByte = LShiftByte(bytValue, bytShiftBits) Or _
        RShiftByte(bytValue, (8 - bytShiftBits))
End Function

'*******************************************************************************
' Pack (FUNCTION)
'*******************************************************************************
Private Function Pack(b() As Byte) As Long
    Dim lCount As Long
    Dim lTemp  As Long
    
    For lCount = 0 To 3
        lTemp = b(lCount)
        Pack = Pack Or LShift(lTemp, (lCount * 8))
    Next
End Function

'*******************************************************************************
' PackFrom (FUNCTION)
'*******************************************************************************
Private Function PackFrom(b() As Byte, _
                          ByVal k As Long) As Long
    Dim lCount As Long
    Dim lTemp  As Long
    
    For lCount = 0 To 3
        lTemp = b(lCount + k)
        PackFrom = PackFrom Or LShift(lTemp, (lCount * 8))
    Next
End Function

'*******************************************************************************
' Unpack (SUB)
'*******************************************************************************
Private Sub Unpack(ByVal a As Long, _
                   b() As Byte)
    b(0) = a And m_lOnBits(7)
    b(1) = RShift(a, 8) And m_lOnBits(7)
    b(2) = RShift(a, 16) And m_lOnBits(7)
    b(3) = RShift(a, 24) And m_lOnBits(7)
End Sub

'*******************************************************************************
' UnpackFrom (SUB)
'*******************************************************************************
Private Sub UnpackFrom(ByVal a As Long, _
                       b() As Byte, _
                       ByVal k As Long)
    b(0 + k) = a And m_lOnBits(7)
    b(1 + k) = RShift(a, 8) And m_lOnBits(7)
    b(2 + k) = RShift(a, 16) And m_lOnBits(7)
    b(3 + k) = RShift(a, 24) And m_lOnBits(7)
End Sub

'*******************************************************************************
' xtime (FUNCTION)
'*******************************************************************************
Private Function xtime(ByVal a As Byte) As Byte
    Dim b As Byte
    
    If (a And &H80) Then
        b = &H1B
    Else
        b = 0
    End If
    
    a = LShiftByte(a, 1)
    a = a Xor b
    
    xtime = a
End Function

'*******************************************************************************
' bmul (FUNCTION)
'*******************************************************************************
Private Function bmul(ByVal x As Byte, _
                      y As Byte) As Byte
    If x <> 0 And y <> 0 Then
        bmul = m_ptab((CLng(m_ltab(x)) + CLng(m_ltab(y))) Mod 255)
    Else
        bmul = 0
    End If
End Function

'*******************************************************************************
' SubByte (FUNCTION)
'*******************************************************************************
Private Function SubByte(ByVal a As Long) As Long
    Dim b(3) As Byte
    
    Unpack a, b
    b(0) = m_fbsub(b(0))
    b(1) = m_fbsub(b(1))
    b(2) = m_fbsub(b(2))
    b(3) = m_fbsub(b(3))
    
    SubByte = Pack(b)
End Function

'*******************************************************************************
' product (FUNCTION)
'*******************************************************************************
Private Function product(ByVal x As Long, _
                         ByVal y As Long) As Long
    Dim xb(3) As Byte
    Dim yb(3) As Byte
    
    Unpack x, xb
    Unpack y, yb
    product = bmul(xb(0), yb(0)) Xor bmul(xb(1), yb(1)) Xor bmul(xb(2), yb(2)) _
        Xor bmul(xb(3), yb(3))
End Function

'*******************************************************************************
' InvMixCol (FUNCTION)
'*******************************************************************************
Private Function InvMixCol(ByVal x As Long) As Long
    Dim y       As Long
    Dim m       As Long
    Dim b(3)    As Byte
    
    m = Pack(m_InCo)
    b(3) = product(m, x)
    m = RotateLeft(m, 24)
    b(2) = product(m, x)
    m = RotateLeft(m, 24)
    b(1) = product(m, x)
    m = RotateLeft(m, 24)
    b(0) = product(m, x)
    y = Pack(b)
    
    InvMixCol = y
End Function

'*******************************************************************************
' ByteSub (FUNCTION)
'*******************************************************************************
Private Function ByteSub(ByVal x As Byte) As Byte
    Dim y As Byte
    
    y = m_ptab(255 - m_ltab(x))
    x = y
    x = RotateLeftByte(x, 1)
    y = y Xor x
    x = RotateLeftByte(x, 1)
    y = y Xor x
    x = RotateLeftByte(x, 1)
    y = y Xor x
    x = RotateLeftByte(x, 1)
    y = y Xor x
    y = y Xor &H63
    
    ByteSub = y
End Function

'*******************************************************************************
' gentables (SUB)
'*******************************************************************************
Public Sub gentables()
    Dim i       As Long
    Dim y       As Byte
    Dim b(3)    As Byte
    Dim ib      As Byte
    
    m_ltab(0) = 0
    m_ptab(0) = 1
    m_ltab(1) = 0
    m_ptab(1) = 3
    m_ltab(3) = 1
    
    For i = 2 To 255
        m_ptab(i) = m_ptab(i - 1) Xor xtime(m_ptab(i - 1))
        m_ltab(m_ptab(i)) = i
    Next
    
    m_fbsub(0) = &H63
    m_rbsub(&H63) = 0
    
    For i = 1 To 255
        ib = i
        y = ByteSub(ib)
        m_fbsub(i) = y
        m_rbsub(y) = i
    Next
    
        y = 1
    For i = 0 To 29
        m_rco(i) = y
        y = xtime(y)
    Next
    
    For i = 0 To 255
        y = m_fbsub(i)
        b(3) = y Xor xtime(y)
        b(2) = y
        b(1) = y
        b(0) = xtime(y)
        m_ftable(i) = Pack(b)
        
        y = m_rbsub(i)
        b(3) = bmul(m_InCo(0), y)
        b(2) = bmul(m_InCo(1), y)
        b(1) = bmul(m_InCo(2), y)
        b(0) = bmul(m_InCo(3), y)
        m_rtable(i) = Pack(b)
    Next
End Sub

'*******************************************************************************
' gkey (SUB)
'*******************************************************************************
Public Sub gkey(ByVal nb As Long, _
                ByVal nk As Long, _
                KEY() As Byte)
                
    Dim i               As Long
    Dim j               As Long
    Dim k               As Long
    Dim m               As Long
    Dim N               As Long
    Dim C1              As Long
    Dim C2              As Long
    Dim C3              As Long
    Dim CipherKey(7)    As Long
    
    m_Nb = nb
    m_Nk = nk
    
    If m_Nb >= m_Nk Then
        m_Nr = 6 + m_Nb
    Else
        m_Nr = 6 + m_Nk
    End If
    
    C1 = 1
    If m_Nb < 8 Then
        C2 = 2
        C3 = 3
    Else
        C2 = 3
        C3 = 4
    End If
    
    For j = 0 To nb - 1
        m = j * 3
        
        m_fi(m) = (j + C1) Mod nb
        m_fi(m + 1) = (j + C2) Mod nb
        m_fi(m + 2) = (j + C3) Mod nb
        m_ri(m) = (nb + j - C1) Mod nb
        m_ri(m + 1) = (nb + j - C2) Mod nb
        m_ri(m + 2) = (nb + j - C3) Mod nb
    Next
    
    N = m_Nb * (m_Nr + 1)
    
    For i = 0 To m_Nk - 1
        j = i * 4
        CipherKey(i) = PackFrom(KEY, j)
    Next
    
    For i = 0 To m_Nk - 1
        m_fkey(i) = CipherKey(i)
    Next
    
    j = m_Nk
    k = 0
    Do While j < N
        m_fkey(j) = m_fkey(j - m_Nk) Xor _
            SubByte(RotateLeft(m_fkey(j - 1), 24)) Xor m_rco(k)
        If m_Nk <= 6 Then
            i = 1
            Do While i < m_Nk And (i + j) < N
                m_fkey(i + j) = m_fkey(i + j - m_Nk) Xor _
                    m_fkey(i + j - 1)
                i = i + 1
            Loop
        Else
            ' Problem fixed here
            i = 1
            Do While i < 4 And (i + j) < N
                m_fkey(i + j) = m_fkey(i + j - m_Nk) Xor _
                    m_fkey(i + j - 1)
                i = i + 1
            Loop
            If j + 4 < N Then
                m_fkey(j + 4) = m_fkey(j + 4 - m_Nk) Xor _
                    SubByte(m_fkey(j + 3))
            End If
            i = 5
            Do While i < m_Nk And (i + j) < N
                m_fkey(i + j) = m_fkey(i + j - m_Nk) Xor _
                    m_fkey(i + j - 1)
                i = i + 1
            Loop
        End If
        
        j = j + m_Nk
        k = k + 1
    Loop
    
    For j = 0 To m_Nb - 1
        m_rkey(j + N - nb) = m_fkey(j)
    Next
    
    i = m_Nb
    Do While i < N - m_Nb
        k = N - m_Nb - i
        For j = 0 To m_Nb - 1
            m_rkey(k + j) = InvMixCol(m_fkey(i + j))
        Next
        i = i + m_Nb
    Loop
    
    j = N - m_Nb
    Do While j < N
        m_rkey(j - N + m_Nb) = m_fkey(j)
        j = j + 1
    Loop
End Sub

'*******************************************************************************
' encrypt (SUB)
'*******************************************************************************
Public Sub Encrypt(buff() As Byte)
    Dim i       As Long
    Dim j       As Long
    Dim k       As Long
    Dim m       As Long
    Dim a(7)    As Long
    Dim b(7)    As Long
    Dim x()     As Long
    Dim y()     As Long
    Dim t()     As Long
    
    For i = 0 To m_Nb - 1
        j = i * 4
        
        a(i) = PackFrom(buff, j)
        a(i) = a(i) Xor m_fkey(i)
    Next
    
    k = m_Nb
    x = a
    y = b
    
    For i = 1 To m_Nr - 1
        For j = 0 To m_Nb - 1
            m = j * 3
            y(j) = m_fkey(k) Xor m_ftable(x(j) And m_lOnBits(7)) Xor _
                RotateLeft(m_ftable(RShift(x(m_fi(m)), 8) And m_lOnBits(7)), 8) Xor _
                RotateLeft(m_ftable(RShift(x(m_fi(m + 1)), 16) And m_lOnBits(7)), 16) Xor _
                RotateLeft(m_ftable(RShift(x(m_fi(m + 2)), 24) And m_lOnBits(7)), 24)
            k = k + 1
        Next
        t = x
        x = y
        y = t
    Next
    
    For j = 0 To m_Nb - 1
        m = j * 3
        y(j) = m_fkey(k) Xor m_fbsub(x(j) And m_lOnBits(7)) Xor _
            RotateLeft(m_fbsub(RShift(x(m_fi(m)), 8) And m_lOnBits(7)), 8) Xor _
            RotateLeft(m_fbsub(RShift(x(m_fi(m + 1)), 16) And m_lOnBits(7)), 16) Xor _
            RotateLeft(m_fbsub(RShift(x(m_fi(m + 2)), 24) And m_lOnBits(7)), 24)
        k = k + 1
    Next
    
    For i = 0 To m_Nb - 1
        j = i * 4
        UnpackFrom y(i), buff, j
        x(i) = 0
        y(i) = 0
    Next
End Sub

'*******************************************************************************
' decrypt (SUB)
'*******************************************************************************
Public Sub Decrypt(buff() As Byte)
    Dim i As Long
    Dim j As Long
    Dim k As Long
    Dim m As Long
    Dim a(7) As Long
    Dim b(7) As Long
    Dim x() As Long
    Dim y() As Long
    Dim t() As Long
    
    For i = 0 To m_Nb - 1
        j = i * 4
        a(i) = PackFrom(buff, j)
        a(i) = a(i) Xor m_rkey(i)
    Next
    
    k = m_Nb
    x = a
    y = b
    
    For i = 1 To m_Nr - 1
        For j = 0 To m_Nb - 1
            m = j * 3
            y(j) = m_rkey(k) Xor m_rtable(x(j) And m_lOnBits(7)) Xor _
                RotateLeft(m_rtable(RShift(x(m_ri(m)), 8) And m_lOnBits(7)), 8) Xor _
                RotateLeft(m_rtable(RShift(x(m_ri(m + 1)), 16) And m_lOnBits(7)), 16) Xor _
                RotateLeft(m_rtable(RShift(x(m_ri(m + 2)), 24) And m_lOnBits(7)), 24)
            k = k + 1
        Next
        t = x
        x = y
        y = t
    Next
    
    For j = 0 To m_Nb - 1
        m = j * 3
        
        y(j) = m_rkey(k) Xor m_rbsub(x(j) And m_lOnBits(7)) Xor _
            RotateLeft(m_rbsub(RShift(x(m_ri(m)), 8) And m_lOnBits(7)), 8) Xor _
            RotateLeft(m_rbsub(RShift(x(m_ri(m + 1)), 16) And m_lOnBits(7)), 16) Xor _
            RotateLeft(m_rbsub(RShift(x(m_ri(m + 2)), 24) And m_lOnBits(7)), 24)
        k = k + 1
    Next
    
    For i = 0 To m_Nb - 1
        j = i * 4
        
        UnpackFrom y(i), buff, j
        x(i) = 0
        y(i) = 0
    Next
End Sub

''*******************************************************************************
'' CopyBytesASP (SUB)
''
'' Slower non-API function you can use to copy array data
''*******************************************************************************
'Private Sub CopyBytesASP(bytDest() As Byte, _
'                         lDestStart As Long, _
'                         bytSource() As Byte, _
'                         lSourceStart As Long, _
'                         lLength As Long)
'    Dim lCount As Long
'
'    lCount = 0
'    Do
'        bytDest(lDestStart + lCount) = bytSource(lSourceStart + lCount)
'        lCount = lCount + 1
'    Loop Until lCount = lLength
'End Sub

'*******************************************************************************
' IsInitialized (FUNCTION)
'*******************************************************************************
Private Function IsInitialized(ByRef vArray As Variant) As Boolean
    On Error Resume Next
    
    IsInitialized = IsNumeric(UBound(vArray))
End Function

'*******************************************************************************
' EncryptData (FUNCTION)
'
' Takes the message, whatever the size, and password in one call and does
' everything for you to return an encoded/encrypted message
'*******************************************************************************
Public Function EncryptData(bytMessage() As Byte, _
                            bytPassword() As Byte) As Byte()
    Dim bytKey(31)      As Byte
    Dim bytIn()         As Byte
    Dim bytOut()        As Byte
    Dim bytTemp(31)     As Byte
    Dim lCount          As Long
    Dim lLength         As Long
    Dim lEncodedLength  As Long
    Dim bytLen(3)       As Byte
    Dim lPosition       As Long
    
    If Not IsInitialized(bytMessage) Then
        Exit Function
    End If
    If Not IsInitialized(bytPassword) Then
        Exit Function
    End If
    
    ' Use first 32 bytes of the password for the key
    For lCount = 0 To UBound(bytPassword)
        bytKey(lCount) = bytPassword(lCount)
        If lCount = 31 Then
            Exit For
        End If
    Next
    
    ' Prepare the key; assume 256 bit block and key size
    gentables
    gkey 8, 8, bytKey
    
    ' We are going to put the message size on the front of the message
    ' in the first 4 bytes. If the length is more than a max int we are
    ' in trouble
    lLength = UBound(bytMessage) + 1
    lEncodedLength = lLength + 4
    
    ' The encoded length includes the 4 bytes stuffed on the front
    ' and is padded out to be modulus 32
    If lEncodedLength Mod 32 <> 0 Then
        lEncodedLength = lEncodedLength + 32 - (lEncodedLength Mod 32)
    End If
    ReDim bytIn(lEncodedLength - 1)
    ReDim bytOut(lEncodedLength - 1)
    
    ' Put the length on the front
    '* Unpack lLength, bytIn
    CopyMemory VarPtr(bytIn(0)), VarPtr(lLength), 4
    ' Put the rest of the message after it
    '* CopyBytesASP bytIn, 4, bytMessage, 0, lLength
    CopyMemory VarPtr(bytIn(4)), VarPtr(bytMessage(0)), lLength

    ' Encrypt a block at a time
    For lCount = 0 To lEncodedLength - 1 Step 32
        '* CopyBytesASP bytTemp, 0, bytIn, lCount, 32
        CopyMemory VarPtr(bytTemp(0)), VarPtr(bytIn(lCount)), 32
        Encrypt bytTemp
        '* CopyBytesASP bytOut, lCount, bytTemp, 0, 32
        CopyMemory VarPtr(bytOut(lCount)), VarPtr(bytTemp(0)), 32
    Next
    
    EncryptData = bytOut
End Function

'*******************************************************************************
' DecryptData (FUNCTION)
'
' Opposite of Encryptdata
'*******************************************************************************
Public Function DecryptData(bytIn() As Byte, _
                            bytPassword() As Byte) As Byte()
    Dim bytMessage()    As Byte
    Dim bytKey(31)      As Byte
    Dim bytOut()        As Byte
    Dim bytTemp(31)     As Byte
    Dim lCount          As Long
    Dim lLength         As Long
    Dim lEncodedLength  As Long
    Dim bytLen(3)       As Byte
    Dim lPosition       As Long
    
    If Not IsInitialized(bytIn) Then
        Exit Function
    End If
    If Not IsInitialized(bytPassword) Then
        Exit Function
    End If
    
    lEncodedLength = UBound(bytIn) + 1
    
    If lEncodedLength Mod 32 <> 0 Then
        Exit Function
    End If
    
    ' Use first 32 bytes of the password for the key
    For lCount = 0 To UBound(bytPassword)
        bytKey(lCount) = bytPassword(lCount)
        If lCount = 31 Then
            Exit For
        End If
    Next
    
    ' Prepare the key; assume 256 bit block and key size
    gentables
    gkey 8, 8, bytKey

    ' The output array needs to be the same size as the input array
    ReDim bytOut(lEncodedLength - 1)
    
    ' Decrypt a block at a time
    For lCount = 0 To lEncodedLength - 1 Step 32
        '* CopyBytesASP bytTemp, 0, bytIn, lCount, 32
        CopyMemory VarPtr(bytTemp(0)), VarPtr(bytIn(lCount)), 32
        Decrypt bytTemp
        '* CopyBytesASP bytOut, lCount, bytTemp, 0, 32
        CopyMemory VarPtr(bytOut(lCount)), VarPtr(bytTemp(0)), 32
    Next

    ' Get the original length of the string from the first 4 bytes
    '* lLength = Pack(bytOut)
    CopyMemory VarPtr(lLength), VarPtr(bytOut(0)), 4
    
    ' Make sure the length is consistent with our data
    If lLength > lEncodedLength - 4 Then
        Exit Function
    End If
    
    ' Prepare the output message byte array
    ReDim bytMessage(lLength - 1)
    '* CopyBytesASP bytMessage, 0, bytOut, 4, lLength
    CopyMemory VarPtr(bytMessage(0)), VarPtr(bytOut(4)), lLength
    
    DecryptData = bytMessage
End Function
