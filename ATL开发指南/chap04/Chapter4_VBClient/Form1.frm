VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "VB Math Test Application"
   ClientHeight    =   4410
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5550
   LinkTopic       =   "Form1"
   ScaleHeight     =   4410
   ScaleWidth      =   5550
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdFibonacci 
      Caption         =   "Fibonacci"
      Height          =   495
      Left            =   3960
      TabIndex        =   10
      Top             =   2400
      Width           =   1335
   End
   Begin VB.CommandButton cmdSum 
      Caption         =   "Sum"
      Height          =   495
      Left            =   2640
      TabIndex        =   9
      Top             =   2400
      Width           =   1335
   End
   Begin VB.CommandButton cmdFactorial 
      Caption         =   "Factorial"
      Height          =   495
      Left            =   3960
      TabIndex        =   8
      Top             =   1920
      Width           =   1335
   End
   Begin VB.CommandButton cmdCompute 
      Caption         =   "Compute"
      Height          =   495
      Left            =   2640
      TabIndex        =   7
      Top             =   1920
      Width           =   1335
   End
   Begin VB.TextBox txtResult 
      Height          =   375
      Left            =   360
      TabIndex        =   6
      Top             =   1320
      Width           =   1815
   End
   Begin VB.TextBox txtOp2 
      Height          =   375
      Left            =   360
      TabIndex        =   5
      Top             =   840
      Width           =   1815
   End
   Begin VB.TextBox txtOp1 
      Height          =   375
      Left            =   360
      TabIndex        =   4
      Top             =   480
      Width           =   1815
   End
   Begin VB.CommandButton cmdDivide 
      Caption         =   "Divide"
      Height          =   495
      Left            =   3960
      TabIndex        =   3
      Top             =   960
      Width           =   1335
   End
   Begin VB.CommandButton cmdMultiply 
      Caption         =   "Multiply"
      Height          =   495
      Left            =   3960
      TabIndex        =   2
      Top             =   480
      Width           =   1335
   End
   Begin VB.CommandButton cmdSubtract 
      Caption         =   "Subtract"
      Height          =   495
      Left            =   2640
      TabIndex        =   1
      Top             =   960
      Width           =   1335
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add"
      Height          =   495
      Left            =   2640
      TabIndex        =   0
      Top             =   480
      Width           =   1335
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim iMath As New CHAPTER4_SERVERLib.Math
Dim iAdvMath As CHAPTER4_SERVERLib.IAdvancedMath
Dim IMath2 As CHAPTER4_SERVERLib.IMath2

Dim objMath As Object
Private Sub cmdAdd_Click()
    txtResult = iMath.Add(txtOp1, txtOp2)
End Sub

Private Sub cmdCompute_Click()
    Set IMath2 = iMath
    txtResult = IMath2.Compute(mathAdd, txtOp1, txtOp2)
End Sub

Private Sub cmdDivide_Click()
    txtResult = iMath.Divide(txtOp1, txtOp2)
End Sub

Private Sub cmdFactorial_Click()
    Set iAdvMath = iMath
    txtResult = iAdvMath.Factorial(txtOp1)
End Sub

Private Sub cmdFibonacci_Click()
    Set iAdvMath = iMath
    txtResult = iAdvMath.Fibonacci(txtOp1)
End Sub

Private Sub cmdMultiply_Click()
    txtResult = iMath.Multiply(txtOp1, txtOp2)
End Sub

Private Sub cmdSubtract_Click()
    txtResult = iMath.Subtract(txtOp1, txtOp2)
End Sub

Private Sub cmdSum_Click()
    Set IMath2 = iMath
    Dim sArray(3) As Integer
    sArrary(1) = 3
    sArrary(2) = 4
    sArrary(3) = 5
    txtResult = IMath2.Sum(3, sArray(1))
End Sub
