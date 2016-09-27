VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "CallBack"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Implements CHAPTER7_CALLBACKSERVERLib.ICallBack

Private Sub ICallBack_ComputationComplete(ByVal lResult As Long)
    frmMain.txtResult = lResult
End Sub
