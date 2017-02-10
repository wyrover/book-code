<%@ Language = VBScript %>
<%@ Category = Locks %>
<%@ Description = Dump out all locked critical sections %>

<%  
Set DataFiles = Manager.DataFiles
For each DataFile in DataFiles
    Manager.Write "<B>Analyzing dump file " & DataFile & "</B><BR>"

    Set Debugger = Manager.GetDebugger(DataFile)
    Set CritSecInfo = Debugger.CritSecInfo
    CritSecCount = CritSecInfo.Count
    if CritSecCount = 0 Then
        Manager.Write "No locked critical sections found<BR>"
    else
        For i = 0 to CritSecCount-1
            Set DbgCritSec = CritSecInfo.Item(i)
            Manager.Write "The owner of critsec: " & DbgCritSec.Address & " is thread: " & DbgCritSec.OwnerThreadID & "<BR>"
        Next
    End If
    Set Debugger = Nothing
Next
 %>