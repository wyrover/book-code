VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMovie"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Private m_strTitle As String
Private m_strDirector As String

Public Property Get Title() As Variant
   Title = m_strTitle
End Property

Public Property Let Title(ByVal vNewValue As Variant)
   m_strTitle = vNewValue
End Property

Public Property Get Director() As Variant
   Director = m_strDirector
End Property

Public Property Let Director(ByVal vNewValue As Variant)
   m_strDirector = vNewValue
End Property
