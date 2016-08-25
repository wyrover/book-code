Attribute VB_Name = "CollisionDetection"
Public Function Contact(E1 As Control, E2 As Control) As Boolean
Contact = False
If E1.Left + E1.Width >= E2.Left And E1.Left <= E2.Left + E2.Width And E1.Top + E1.Height >= E2.Top And E1.Top <= E2.Top + E2.Height Then Contact = True
End Function
Public Function Containment(Vacancy As Control, Internal As Control) As Boolean
Containment = False
If Internal.Left >= Vacancy.Left And Internal.Top >= Vacancy.Top And Internal.Left + Internal.Width <= Vacancy.Left + Vacancy.Width And Internal.Top + Internal.Height <= Vacancy.Top + Vacancy.Height Then Containment = True
End Function
Public Function Between(ByVal Value As Double, ByVal Bound1 As Double, ByVal Bound2 As Double) As Boolean
Between = False
'Checks if value lies between two bounds
If Bound1 > Bound2 Then
    If Value >= Bound2 And Value <= Bound1 Then Between = True
Else
    If Value >= Bound1 And Value <= Bound2 Then Between = True
End If
End Function
Public Function QuadContact(MC As Control, E1 As Control) As Byte
Dim M1 As Single
Dim B1 As Single
Dim M2 As Single
Dim B2 As Single
Dim Xm As Single
Dim Ym As Single
Dim UR As Boolean, UL As Boolean
M1 = -MC.Height / MC.Width
B1 = ((MC.Top + MC.Height) - M1 * MC.Left)
M2 = -M1
B2 = MC.Top - M2 * MC.Left
Xm = E1.Left + E1.Width \ 2
Ym = E1.Top + E1.Height \ 2
If Ym <= (M1 * Xm) + B1 Then UL = True
If Ym <= (M2 * Xm) + B2 Then UR = True
If UL And UR Then
    If E1.Top >= MC.Top - E1.Height Then QuadContact = 2
ElseIf UL Then
    If E1.Left >= MC.Left - E1.Width Then QuadContact = 1
ElseIf UR Then
    If E1.Left <= MC.Left + MC.Width Then QuadContact = 3
Else
    If E1.Top <= MC.Top + MC.Height Then QuadContact = 4
End If
End Function
Public Function LineContact(L1 As Line, L2 As Line) As Boolean
Dim M1 As Double, M2 As Double, B1 As Double, B2 As Double, IX As Double
'Slopes & left-intercepts, respectively
With L1
  If .X2 = .X1 Then
    M1 = 10000000 * Sgn(.Y2 - .Y1)
    'It is important to check for overflows, although the check does decrease accuracy.
  Else
    M1 = (.Y2 - .Y1) / (.X2 - .X1)
  End If
  B1 = .Y2 - M1 * .X2
End With
With L2
  If .X2 = .X1 Then
    M2 = 10000000 * Sgn(.Y2 - .Y1)
  Else
    M2 = (.Y2 - .Y1) / (.X2 - .X1)
  End If
  B2 = .Y2 - M2 * .X2
End With
If M2 - M1 = 0 Then
    If B2 = B1 Then
        LineContact = True
        'The lines are the same,although this is rare
    Else
        LineContact = False
    'Parallel lines don't intersect
    End If
Else
    'An intersection is a point that lies on both lines.
    'To find this, we set the Y equations equal and solve for X.
    'M1X+B1 =M2X+B2 -> M1X-M2X=-B1+B2 -> X = B1+B2/(M1-M2)
    IX = CInt((B2 - B1) / (M1 - M2))
    'This CInt is here to increase the accuracy of the vertical line contact
    'It gets less accurate with longer lines due to the 1000000, which is
    'a mildly decent alternative to infinity.
    If Between(IX, L1.X1, L1.X2) And Between(IX, L2.X1, L2.X2) Then
    'Checks if the intersection lies within the segment.
        LineContact = True
    Else
        LineContact = False
    End If
End If
End Function
Public Function LSContact(L1 As Line, S1 As Control) As Boolean
Dim M As Double, B As Double
'There's actually more than one way to do this... you could transform the square into lines
'and then detect from there.
'I'm just testing the endpoints to see if the line goes through the square.
M = (L1.Y2 - L1.Y1) / (L1.X2 - L1.X1)
B = L1.Y1 - M * L1.X1
If M > 0 Then
'Sloping downwards, like \
    If (S1.Top + S1.Height) > M * S1.Left + B And S1.Top < M * (S1.Left + S1.Width) + B Then
'This checks if the top right corner is above the line and the bottom left is below the line.
        If Between(S1.Left, L1.X1, L1.X2) Or Between(S1.Top, L1.X1, L1.X2) Then
        'This makes sure that the line is near the shape
            LSContact = True
        End If
    End If
Else
'Sloping upwards, like /
    If S1.Top < M * S1.Left + B And (S1.Top + S1.Height) > M * (S1.Left + S1.Width) + B Then
    'This checks if the top left corner is above the line and the bottom right corner is below the line.
        If Between(S1.Left, L1.X1, L1.X2) Or Between(S1.Top, L1.X1, L1.X2) Then
        'This makes sure that the line is near the shape
            LSContact = True
        End If
    End If
End If

End Function
