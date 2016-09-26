VERSION 5.00
Object = "{8E27C92E-1264-101C-8A2F-040224009C02}#7.0#0"; "MSCAL.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3675
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4935
   LinkTopic       =   "Form1"
   ScaleHeight     =   3675
   ScaleWidth      =   4935
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton NextWeekCmd 
      Caption         =   "下一星期"
      Height          =   375
      Left            =   3840
      TabIndex        =   8
      Top             =   3240
      Width           =   855
   End
   Begin VB.TextBox day 
      Height          =   375
      Left            =   3000
      TabIndex        =   6
      Text            =   "8"
      Top             =   3240
      Width           =   495
   End
   Begin VB.TextBox month 
      Height          =   375
      Left            =   2160
      TabIndex        =   4
      Text            =   "8"
      Top             =   3240
      Width           =   495
   End
   Begin VB.TextBox year 
      Height          =   375
      Left            =   1200
      TabIndex        =   3
      Text            =   "2000"
      Top             =   3240
      Width           =   615
   End
   Begin VB.CommandButton SelDateCmd 
      Caption         =   "选定日期"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   3240
      Width           =   975
   End
   Begin MSACAL.Calendar m_calendar 
      Height          =   3135
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4815
      _Version        =   524288
      _ExtentX        =   8493
      _ExtentY        =   5530
      _StockProps     =   1
      BackColor       =   -2147483633
      Year            =   2000
      Month           =   9
      Day             =   11
      DayLength       =   1
      MonthLength     =   2
      DayFontColor    =   255
      FirstDay        =   1
      GridCellEffect  =   1
      GridFontColor   =   16711680
      GridLinesColor  =   -2147483632
      ShowDateSelectors=   -1  'True
      ShowDays        =   -1  'True
      ShowHorizontalGrid=   -1  'True
      ShowTitle       =   -1  'True
      ShowVerticalGrid=   -1  'True
      TitleFontColor  =   12582912
      ValueIsNull     =   0   'False
      BeginProperty DayFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "宋体"
         Size            =   8.25
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty GridFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "宋体"
         Size            =   8.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty TitleFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Label Label3 
      Caption         =   "日"
      Height          =   375
      Left            =   3480
      TabIndex        =   7
      Top             =   3360
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "月"
      Height          =   375
      Left            =   2640
      TabIndex        =   5
      Top             =   3360
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "年"
      Height          =   255
      Left            =   1800
      TabIndex        =   2
      Top             =   3360
      Width           =   255
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub m_calendar_AfterUpdate()
    year.Text = m_calendar.year
    month.Text = m_calendar.month
    day.Text = m_calendar.day
End Sub

Private Sub NextWeekCmd_Click()
    m_calendar.NextWeek
End Sub

Private Sub SelDateCmd_Click()
    m_calendar.year = year.Text
    m_calendar.month = month.Text
    m_calendar.day = day.Text
End Sub
