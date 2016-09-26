VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   2130
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4065
   LinkTopic       =   "Form1"
   ScaleHeight     =   2130
   ScaleWidth      =   4065
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "确认"
      Height          =   375
      Left            =   1320
      TabIndex        =   3
      Top             =   1320
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   2052
         SubFormatType   =   1
      EndProperty
      Height          =   375
      Left            =   960
      TabIndex        =   1
      Text            =   "0"
      Top             =   600
      Width           =   1815
   End
   Begin VB.Label Label2 
      Caption         =   "元"
      Height          =   375
      Left            =   2880
      TabIndex        =   2
      Top             =   600
      Width           =   255
   End
   Begin VB.Label Label1 
      Caption         =   "借/贷金额（负为借，正为贷）"
      Height          =   255
      Left            =   720
      TabIndex        =   0
      Top             =   240
      Width           =   2895
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim m_oAccount As Object

Private Sub Command1_Click()
    Dim lAmount As Long
    lAmount = CLng(Text1.Text)
    Dim sRes As String
    sRes = m_oAccount.Post(lAmount)
    MsgBox sRes
End Sub

Private Sub Form_Load()
Set m_oAccount = CreateObject("ATLSample.Account.1")
End Sub
Private Sub Form_Unload(Cancel As Integer)
Set m_oAccount = Nothing
End Sub
