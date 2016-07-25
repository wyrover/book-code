VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5025
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6960
   LinkTopic       =   "Form1"
   ScaleHeight     =   5025
   ScaleWidth      =   6960
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   2160
      TabIndex        =   1
      Top             =   3600
      Width           =   1575
   End
   Begin VB.ListBox List1 
      Height          =   2985
      Left            =   1680
      TabIndex        =   0
      Top             =   120
      Width           =   2535
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
Dim conn As New ADODB.Connection
Dim rst As New ADODB.Recordset
Dim cmd As New ADODB.Command

conn.ConnectionString = "Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Initial Catalog=pubs"
conn.Open

'Set rst = conn.Execute("select * from authors")
'rst.ActiveConnection = conn
'rst.Open "select * from authors"
cmd.ActiveConnection = conn
cmd.CommandText = "select * from authors"
Set rst = cmd.Execute

Do Until rst.EOF
    List1.AddItem rst("au_lname")
    rst.MoveNext
Loop
End Sub
