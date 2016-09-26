VERSION 5.00
Object = "{CFC43223-50AC-11D1-B5EC-0004ACFF171C}#1.0#0"; "PostIt.dll"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4680
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   4680
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.Label Label1 
      Caption         =   "Label1"
      BeginProperty Font 
         Name            =   "Script"
         Size            =   20.25
         Charset         =   255
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   600
      TabIndex        =   1
      Top             =   3480
      Width           =   3015
   End
   Begin POSTITLibCtl.PostItCtl PostItCtl1 
      Height          =   2655
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   4335
      Text            =   "sdfsdfasdfasdf"
      Appearance      =   1
      BorderStyle     =   0
      BackColor       =   16776960
      ForeColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Script"
         Size            =   24
         Charset         =   255
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
