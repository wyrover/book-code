VERSION 5.00
Begin VB.Form Form224 
   Caption         =   "Form224"
   ClientHeight    =   4680
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6135
   LinkTopic       =   "Form1"
   ScaleHeight     =   312
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   409
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   40
      Left            =   5760
      Top             =   3960
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   13
      Left            =   4320
      Top             =   1920
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   12
      Left            =   5160
      Top             =   2760
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   11
      Left            =   4680
      Top             =   2760
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   10
      Left            =   5640
      Top             =   2760
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   9
      Left            =   1080
      Top             =   360
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   8
      Left            =   3960
      Top             =   3600
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   7
      Left            =   1080
      Top             =   1920
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   6
      Left            =   3480
      Top             =   480
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   5
      Left            =   2160
      Top             =   960
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   4
      Left            =   600
      Top             =   3480
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   3
      Left            =   2880
      Top             =   3120
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   2
      Left            =   1680
      Top             =   3840
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   1
      Left            =   3120
      Top             =   1320
      Width           =   495
   End
   Begin VB.Shape Box 
      FillColor       =   &H00FF8080&
      FillStyle       =   0  'Solid
      Height          =   255
      Index           =   0
      Left            =   0
      Top             =   2400
      Width           =   495
   End
   Begin VB.Shape BBall 
      FillColor       =   &H00565CD6&
      FillStyle       =   0  'Solid
      Height          =   255
      Left            =   2280
      Shape           =   3  'Circle
      Top             =   1800
      Width           =   255
   End
End
Attribute VB_Name = "Form224"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim VelocX As Integer, VelocY As Integer


Private Sub Form_Load()
VelocX = -5
VelocY = -5
End Sub

Private Sub Timer1_Timer()

Dim LV As Integer
Dim HCollision As Boolean, VCollision As Boolean

BBall.Left = BBall.Left + VelocX
For LV = Box.LBound To Box.UBound
'Collided with a box.
    If BBall.Left < Box(LV).Left + Box(LV).Width And Box(LV).Left < BBall.Left + BBall.Width Then
        If BBall.Top < Box(LV).Top + Box(LV).Height And Box(LV).Top < BBall.Top + BBall.Height Then
            HCollision = True
        End If
    End If
Next
If BBall.Left + BBall.Width >= Me.ScaleWidth Or BBall.Left < 0 Then
'Off the edge of the screen.
    HCollision = True
End If

'Can handle horizontal collisions here.
    
BBall.Top = BBall.Top + VelocY
For LV = Box.LBound To Box.UBound
'Collided with a box.
    If BBall.Left < Box(LV).Left + Box(LV).Width And Box(LV).Left < BBall.Left + BBall.Width Then
        If BBall.Top < Box(LV).Top + Box(LV).Height And Box(LV).Top < BBall.Top + BBall.Height Then
            VCollision = True
        End If
    End If
Next
If BBall.Top + BBall.Height >= Me.ScaleHeight Or BBall.Top < 0 Then
'Off the edge of the screen.
    VCollision = True
End If

If HCollision Then
    VelocX = -VelocX
    BBall.Left = BBall.Left + VelocX
ElseIf VCollision Then
    VelocY = -VelocY
    BBall.Top = BBall.Top + VelocY
End If

    
    
End Sub
