VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "VB Implements Example"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Height          =   1815
      Left            =   0
      TabIndex        =   4
      Top             =   0
      Width           =   3915
      Begin VB.TextBox txtResult 
         Height          =   375
         Left            =   1920
         TabIndex        =   7
         Top             =   1200
         Width           =   1695
      End
      Begin VB.TextBox txtOp2 
         Height          =   375
         Left            =   1920
         TabIndex        =   6
         Top             =   660
         Width           =   1695
      End
      Begin VB.TextBox txtOp1 
         Height          =   375
         Left            =   1920
         TabIndex        =   5
         Top             =   300
         Width           =   1695
      End
      Begin VB.Label Label3 
         Caption         =   "Result:"
         Height          =   255
         Left            =   480
         TabIndex        =   10
         Top             =   1260
         Width           =   1155
      End
      Begin VB.Label Label2 
         Caption         =   "Operand Two:"
         Height          =   195
         Left            =   480
         TabIndex        =   9
         Top             =   720
         Width           =   1335
      End
      Begin VB.Label Label1 
         Caption         =   "Operand One:"
         Height          =   195
         Left            =   480
         TabIndex        =   8
         Top             =   360
         Width           =   1275
      End
   End
   Begin VB.CommandButton cmdDivide 
      Caption         =   "Divide"
      Height          =   495
      Left            =   1860
      TabIndex        =   3
      Top             =   2580
      Width           =   1335
   End
   Begin VB.CommandButton cmdMultiply 
      Caption         =   "Multiply"
      Height          =   495
      Left            =   1860
      TabIndex        =   2
      Top             =   2100
      Width           =   1335
   End
   Begin VB.CommandButton cmdSubtract 
      Caption         =   "Subtract"
      Height          =   495
      Left            =   540
      TabIndex        =   1
      Top             =   2580
      Width           =   1335
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add"
      Height          =   495
      Left            =   540
      TabIndex        =   0
      Top             =   2100
      Width           =   1335
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim objMath As New CHAPTER7_CALLBACKSERVERLib.Math
Dim objCallBack As New CallBack

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
    objMath.Advise objCallBack
End Sub

Private Sub Form_Unload(Cancel As Integer)
    objMath.UnAdvise
End Sub
