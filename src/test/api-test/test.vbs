strComputer = "."
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
Set colItems = objWMIService.ExecQuery("Select * from Win32_ShadowCopy")
For Each objItem in colItems
    Wscript.Echo "ID: " & objItem.ID
    Wscript.Echo "Client accessible: " & objItem.ClientAccessible
    Wscript.Echo "Count: " & objItem.Count
    Wscript.Echo "Device object: " & objItem.DeviceObject
    Wscript.Echo "Differnetial: " & objItem.Differential
    Wscript.Echo "Exposed locally: " & objItem.ExposedLocally
    Wscript.Echo "Exposed name: " & objItem.ExposedName
    Wscript.Echo "Exposed remotely: " & objItem.ExposedRemotely
    Wscript.Echo "Hardware assisted: " & objItem.HardwareAssisted
    Wscript.Echo "Imported: " & objItem.Imported
    Wscript.Echo "No auto release: " & objItem.NoAutoRelease
    Wscript.Echo "Not surfaced: " & objItem.NotSurfaced
    Wscript.Echo "No writers: " & objItem.NoWriters
    Wscript.Echo "Originating machine: " & objItem.OriginatingMachine
    Wscript.Echo "Persistent: " & objItem.Persistent
    Wscript.Echo "Plex: " & objItem.Plex
    Wscript.Echo "Provider ID: " & objItem.ProviderID
    Wscript.Echo "Service machine: " & objItem.ServiceMachine
    Wscript.Echo "Set ID: " & objItem.SetID
    Wscript.Echo "State: " & objItem.State
    Wscript.Echo "Transportable: " & objItem.Transportable
    Wscript.Echo "Volume name: " & objItem.VolumeName
    Wscript.Echo
Next