VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "VB Math Test Application"
   ClientHeight    =   4080
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5550
   LinkTopic       =   "Form1"
   ScaleHeight     =   4080
   ScaleWidth      =   5550
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Height          =   1815
      Left            =   300
      TabIndex        =   7
      Top             =   300
      Width           =   4515
      Begin VB.TextBox txtResult 
         Height          =   375
         Left            =   1920
         TabIndex        =   10
         Top             =   1200
         Width           =   2175
      End
      Begin VB.TextBox txtOp2 
         Height          =   375
         Left            =   1920
         TabIndex        =   9
         Top             =   660
         Width           =   2175
      End
      Begin VB.TextBox txtOp1 
         Height          =   375
         Left            =   1920
         TabIndex        =   8
         Top             =   300
         Width           =   2175
      End
      Begin VB.Label Label3 
         Caption         =   "Result:"
         Height          =   255
         Left            =   480
         TabIndex        =   13
         Top             =   1260
         Width           =   1155
      End
      Begin VB.Label Label2 
         Caption         =   "Operand Two:"
         Height          =   195
         Left            =   480
         TabIndex        =   12
         Top             =   720
         Width           =   1335
      End
      Begin VB.Label Label1 
         Caption         =   "Operand One:"
         Height          =   195
         Left            =   480
         TabIndex        =   11
         Top             =   360
         Width           =   1275
      End
   End
   Begin VB.CommandButton cmdFibonacci 
      Caption         =   "Fibonacci"
      Height          =   495
      Left            =   1680
      TabIndex        =   6
      Top             =   3420
      Width           =   1335
   End
   Begin VB.CommandButton cmdFactorial 
      Caption         =   "Factorial"
      Height          =   495
      Left            =   360
      TabIndex        =   5
      Top             =   3420
      Width           =   1335
   End
   Begin VB.CommandButton cmdSum 
      Caption         =   "Sum"
      Height          =   495
      Left            =   3480
      TabIndex        =   4
      Top             =   2460
      Width           =   1335
   End
   Begin VB.CommandButton cmdDivide 
      Caption         =   "Divide"
      Height          =   495
      Left            =   1680
      TabIndex        =   3
      Top             =   2940
      Width           =   1335
   End
   Begin VB.CommandButton cmdMultiply 
      Caption         =   "Multiply"
      Height          =   495
      Left            =   1680
      TabIndex        =   2
      Top             =   2460
      Width           =   1335
   End
   Begin VB.CommandButton cmdSubtract 
      Caption         =   "Subtract"
      Height          =   495
      Left            =   360
      TabIndex        =   1
      Top             =   2940
      Width           =   1335
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add"
      Height          =   495
      Left            =   360
      TabIndex        =   0
      Top             =   2460
      Width           =   1335
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' Dim for early binding
Dim objMath As New CHAPTER6_SERVERLib.Math
Dim objMath2 As CHAPTER6_SERVERLib.IMath2
Dim objAdvancedMath As CHAPTER6_SERVERLib.IAdvancedMath
'Dim for late binding
'Dim objMath As Object

Private Sub cmdAdd_Click()
    txtResult = objMath.Add(txtOp1, txtOp2)
End Sub

Private Sub cmdDivide_Click()
    txtResult = objMath.Divide(txtOp1, txtOp2)
End Sub

Private Sub cmdFactorial_Click()
    Set objAdvancedMath = objMath
    txtResult = objAdvancedMath.Factorial(txtOp1)
End Sub

Private Sub cmdFibonacci_Click()
    Set objAdvancedMath = objMath
    txtResult = objAdvancedMath.Fibonacci(txtOp1)
End Sub

Private Sub cmdMultiply_Click()
    txtResult = objMath.Multiply(txtOp1, txtOp2)
End Sub

Private Sub cmdSubtract_Click()
    txtResult = objMath.Subtract(txtOp1, txtOp2)
End Sub

Private Sub cmdSum_Click()
    Set objMath2 = objMath
    'Build a safe array of longs
    Dim longArray(-4 To 4) As Long
    For i = -4 To 4
        longArray(i) = i
    Next
    txtResult = objMath2.Sum(longArray)
End Sub


Private Sub Form_Load()
    'Used for NativeServer
    'Set ObjMath = CreateObject("Chapter6.Math.1")
End Sub
