' List All Existing Restore Points


Set dtmConvertedDate = CreateObject("WbemScripting.SWbemDateTime")

strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
& "{impersonationLevel=impersonate}!\\" & strComputer & "\root\default")

Set colItems = objWMIService.ExecQuery("Select * from SystemRestore")
If colItems.Count = 0 Then
WScript.Echo "No restore point in system."
Else
For Each objItem in colItems
Wscript.Echo "Name: " & objItem.Description
Wscript.Echo "Number: " & objItem.SequenceNumber
Select Case objItem.RestorePointType
Case 0 strRestoreType = "Application installation"
Case 1 strRestoreType = "Application uninstall"
Case 6 strRestoreType = "Restore"
Case 7 strRestoreType = "Checkpoint"
Case 10 strRestoreType = "Device drive installation"
Case 11 strRestoreType = "First run"
Case 12 strRestoreType = "Modify settings"
Case 13 strRestoreType = "Cancelled operation"
Case 14 strRestoreType = "Backup recovery"
Case Else strRestoreType = "Unknown"
End Select
Wscript.Echo "Restore Point Type: " & strRestoreType

dtmConvertedDate.Value = objItem.CreationTime
dtmCreationTime = dtmConvertedDate.GetVarDate
Wscript.Echo "Time: " & dtmCreationTime
Next
End If