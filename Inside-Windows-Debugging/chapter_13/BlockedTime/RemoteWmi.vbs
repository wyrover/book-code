Option Explicit

Dim objLocator, objServer

Set objLocator = CreateObject("WbemScripting.SWbemLocator")
Set objServer = objLocator.ConnectServer("MissingComputer", "\root\cimv2", "UserName", "Password")
