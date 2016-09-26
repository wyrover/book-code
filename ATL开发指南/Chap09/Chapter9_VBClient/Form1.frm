VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "VB Collection Client"
   ClientHeight    =   3615
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7380
   LinkTopic       =   "Form1"
   ScaleHeight     =   3615
   ScaleWidth      =   7380
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "Add a Movie"
      Height          =   3015
      Left            =   4200
      TabIndex        =   3
      Top             =   240
      Width           =   3015
      Begin VB.TextBox txtDirector 
         Height          =   285
         Left            =   960
         TabIndex        =   8
         Top             =   1920
         Width           =   1695
      End
      Begin VB.TextBox txtTitle 
         Height          =   285
         Left            =   960
         TabIndex        =   7
         Top             =   1440
         Width           =   1695
      End
      Begin VB.CommandButton cmdAdd 
         Caption         =   "Add"
         Height          =   495
         Left            =   240
         TabIndex        =   4
         Top             =   480
         Width           =   1455
      End
      Begin VB.Label Label2 
         Caption         =   "Director:"
         Height          =   255
         Left            =   240
         TabIndex        =   6
         Top             =   1920
         Width           =   735
      End
      Begin VB.Label Label1 
         Caption         =   "Title:"
         Height          =   255
         Left            =   240
         TabIndex        =   5
         Top             =   1440
         Width           =   735
      End
   End
   Begin VB.ListBox lstMovies 
      Height          =   2010
      Left            =   360
      TabIndex        =   2
      Top             =   1080
      Width           =   3615
   End
   Begin VB.CommandButton cmdRemove 
      Caption         =   "Remove"
      Height          =   615
      Left            =   2280
      TabIndex        =   1
      Top             =   240
      Width           =   1695
   End
   Begin VB.CommandButton cmdList 
      Caption         =   "List Movies"
      Height          =   615
      Left            =   360
      TabIndex        =   0
      Top             =   240
      Width           =   1695
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim objApp As Object
Private Sub cmdAdd_Click()
    Dim obj As Object
    Set obj = objApp.CreateMovie
    obj.Director = txtDirector.Text
    obj.Title = txtTitle.Text
    objApp.Movies.Add obj
    cmdList_Click
End Sub
Private Sub cmdList_Click()
    lstMovies.Clear
    For Each obj In objApp.Movies
        lstMovies.AddItem obj.Title & " - " & obj.Director
    Next
End Sub
Private Sub cmdRemove_Click()
    If lstMovies.ListIndex > -1 Then
        objApp.Movies.Remove lstMovies.ListIndex + 1
        cmdList_Click
    End If
End Sub
Private Sub Form_Load()
    Set objApp = CreateObject("Chapter9_Collection.Application.1")
End Sub

