VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "VB WithEvents Example"
   ClientHeight    =   3315
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4440
   LinkTopic       =   "Form1"
   ScaleHeight     =   3315
   ScaleWidth      =   4440
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add"
      Height          =   495
      Left            =   120
      TabIndex        =   10
      Top             =   1980
      Width           =   1335
   End
   Begin VB.CommandButton cmdSubtract 
      Caption         =   "Subtract"
      Height          =   495
      Left            =   120
      TabIndex        =   9
      Top             =   2460
      Width           =   1335
   End
   Begin VB.CommandButton cmdMultiply 
      Caption         =   "Multiply"
      Height          =   495
      Left            =   1440
      TabIndex        =   8
      Top             =   1980
      Width           =   1335
   End
   Begin VB.CommandButton cmdDivide 
      Caption         =   "Divide"
      Height          =   495
      Left            =   1440
      TabIndex        =   7
      Top             =   2460
      Width           =   1335
   End
   Begin VB.Frame Frame1 
      Height          =   1815
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3915
      Begin VB.TextBox txtOp1 
         Height          =   375
         Left            =   1920
         TabIndex        =   3
         Top             =   300
         Width           =   1695
      End
      Begin VB.TextBox txtOp2 
         Height          =   375
         Left            =   1920
         TabIndex        =   2
         Top             =   660
         Width           =   1695
      End
      Begin VB.TextBox txtResult 
         Height          =   375
         Left            =   1920
         TabIndex        =   1
         Top             =   1200
         Width           =   1695
      End
      Begin VB.Label Label1 
         Caption         =   "Operand One:"
         Height          =   195
         Left            =   480
         TabIndex        =   6
         Top             =   360
         Width           =   1275
      End
      Begin VB.Label Label2 
         Caption         =   "Operand Two:"
         Height          =   195
         Left            =   480
         TabIndex        =   5
         Top             =   720
         Width           =   1335
      End
      Begin VB.Label Label3 
         Caption         =   "Result:"
         Height          =   255
         Left            =   480
         TabIndex        =   4
         Top             =   1260
         Width           =   1155
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim WithEvents objMath As CHAPTER7_CPSERVERLib.Math
Attribute objMath.VB_VarHelpID = -1

Private Sub cmdAdd_Click()
    objMath.Add txtOp1, txtOp2
End Sub

Private Sub cmdDivide_Click()
    objMath.Divide txtOp1, txtOp2
End Sub

Private Sub cmdMultiply_Click()
    objMath.Multiply txtOp1, txtOp2
End Sub

Private Sub cmdSubtract_Click()
    objMath.Subtract txtOp1, txtOp2
End Sub

Private Sub Form_Load()
    Set objMath = New CHAPTER7_CPSERVERLib.Math
End Sub

Private Sub objMath_ComputationComplete(ByVal lResult As Long)
    txtResult = lResult
    MsgBox "Computation result is " & lResult
End Sub
