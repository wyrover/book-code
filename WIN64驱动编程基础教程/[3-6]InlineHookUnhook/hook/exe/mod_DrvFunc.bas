Attribute VB_Name = "mod_DrvFunc"
Option Explicit

Public DrvController As New cls_Driver

Public Sub Hook_PspTTBP()
    With DrvController
        .IoControl .CTL_CODE_GEN(&H803), 0, 0, 0, 0 '»Ö¸´¹³×Ó
    End With
End Sub
