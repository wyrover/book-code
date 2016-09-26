VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "Chapter 9 VB Collection"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4440
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4440
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtDirector 
      Height          =   285
      Left            =   1575
      TabIndex        =   1
      Top             =   2700
      Width           =   2445
   End
   Begin VB.ListBox lbxMovies 
      Height          =   2205
      Left            =   225
      TabIndex        =   0
      Top             =   270
      Width           =   3885
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim colMovies As New Collection

Private Sub Form_Load()
   Dim objMovie As clsMovie
   
   ' Add a movie to the collection
   Set objMovie = New clsMovie
   objMovie.Title = "Saving Private Ryan"
   objMovie.Director = "Steven Spielberg"
   colMovies.Add objMovie, objMovie.Title
   
   ' Add a movie to the collection
   Set objMovie = New clsMovie
   objMovie.Title = "Star Wars"
   objMovie.Director = "George Lucas"
   colMovies.Add objMovie, objMovie.Title
   
   ' Add a movie to the collection
   Set objMovie = New clsMovie
   objMovie.Title = "Jurassic Park"
   objMovie.Director = "Steven Spielberg"
   colMovies.Add objMovie, objMovie.Title
      
   ' Retrieve a director given a title
   txtDirector = colMovies("Star Wars").Director
      
   ' Iterate over the collection and put the movies in a listbox
   ' This syntax uses the IEnumVariant interface
   For Each objMovie In colMovies
      lbxMovies.AddItem objMovie.Title & " - " & objMovie.Director
   Next
   
   ' Iterate over the collection and put the movies in a listbox
   ' This syntax just uses the Count and Item properties
   Dim iCount As Integer
   For iCount = 1 To colMovies.Count
      Set objMovie = colMovies(iCount)
      lbxMovies.AddItem objMovie.Title & " - " & objMovie.Director
   Next
   
   ' Remove the items from the collection
   For iCount = colMovies.Count To 1 Step -1
      colMovies.Remove iCount
   Next
   
End Sub
