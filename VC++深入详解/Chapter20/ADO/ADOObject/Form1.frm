VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3915
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5175
   LinkTopic       =   "Form1"
   ScaleHeight     =   3915
   ScaleWidth      =   5175
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   1680
      TabIndex        =   1
      Top             =   3000
      Width           =   1695
   End
   Begin VB.ListBox List1 
      Height          =   2400
      Left            =   1320
      TabIndex        =   0
      Top             =   360
      Width           =   2295
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

conn.ConnectionString = "Provider=SQLOLEDB.1;Password=sa;Persist Security Info=True;User ID=sa;Initial Catalog=pubs"
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
