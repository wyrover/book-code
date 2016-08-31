'****************************** Module Header ******************************'
' Module Name:	MainModule.vb
' Project:		VBRemotingClient
' Copyright (c) Microsoft Corporation.
' 
' .NET Remoting is a mechanism for one-way inter-process communication and   
' RPC between .NET applications in the local machine or across the computers 
' in the intranet and internet.
' 
' .NET Remoting allows an application to make an remotable object available 
' across remoting boundaries, which includes different appdomains, processes 
' or even different computers connected by a network. .NET Remoting makes a 
' reference of a remotable object available to a client application, which  
' then instantiates and uses a remotable object as if it were a local object. 
' However, the actual code execution happens at the server-side. Any requests 
' to the remotable object are proxied by the .NET Remoting runtime over  
' Channel objects, that encapsulate the actual transport mode, including TCP 
' streams, HTTP streams and named pipes. As a result, by instantiating proper 
' Channel objects, a .NET Remoting application can be made to support 
' different communication protocols without recompiling the application. The  
' runtime itself manages the act of serialization and marshalling of objects 
' across the client and server appdomains.
' 
' VBRemotingClient is a .NET Remoting client end. It accesses the remote 
' objects exposed by the .NET Remoting server end, VBRemotingServer.
' 
' There are generally two ways to create the .NET Remoting client: using a 
' configuration file or writing codes. The RemotingClientByConfig method 
' demonstrates the former, and the RemotingClientByCode method illustrates  
' the latter method.
' 
' This source is subject to the Microsoft Public License.
' See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
' All other rights reserved.
' 
' History:
' * 3/8/2009 3:40 PM Jialiang Ge Created
'***************************************************************************'

#Region "Imports directives"

Imports System.Runtime.Remoting
Imports System.Runtime.InteropServices
Imports System.Runtime.Serialization.Formatters
Imports System.Runtime.Remoting.Channels
Imports System.Runtime.Remoting.Channels.Tcp

Imports RemotingShared

#End Region


Module MainModule

    Sub Main()
        RemotingClientByConfig()

        ' [-or-]

        'RemotingClientByCode()
    End Sub

    ''' <summary>
    ''' Access the .NET Remoting server using configuration file.
    ''' </summary>
    ''' <remarks></remarks>
    Sub RemotingClientByConfig()

        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Reads the configuration file and configures the remoting 
        ' infrastructure for the client end.
        ' 

        ' The format for .NET Remoting configuration file:
        ' http://msdn.microsoft.com/en-us/library/ms973907.aspx
        RemotingConfiguration.Configure("VBRemotingClient.exe.config", True)


        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Create a remotable object.
        ' 

        ' Create a SingleCall server-activated object
        Dim remoteObj As New SingleCallObject()

        ' [-or-] a Singleton server-activated object
        'Dim remoteObj As New SingletonObject()

        ' [-or-] a client-activated object
        'Dim remoteObj As New ClientActivatedObject()


        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Use the remotable object as if it were a local object.
        ' 

        Dim remoteType As String = remoteObj.GetRemoteObjectType()
        Console.WriteLine("Call GetRemoteObjectType => {0}", remoteType)

        Console.WriteLine("The client process and thread: {0}, {1}", _
            GetCurrentProcessId(), GetCurrentThreadId())

        Dim processId, threadId As UInteger
        remoteObj.GetProcessThreadID(processId, threadId)
        Console.WriteLine("Call GetProcessThreadID => {0} {1}", _
                          processId, threadId)

        Console.WriteLine("Set FloatProperty += {0}", 1.2F)
        remoteObj.FloatProperty += 1.2F

        Console.WriteLine("Get FloatProperty = {0}", remoteObj.FloatProperty)

    End Sub

    ''' <summary>
    ''' Access the .NET Remoting server using code.
    ''' </summary>
    ''' <remarks></remarks>
    Sub RemotingClientByCode()

        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Create and register a channel (TCP channel in this example) that 
        ' is used to transport messages across the remoting boundary.

        ' Properties of the channel
        Dim props As New Hashtable()
        props("typeFilterLevel") = TypeFilterLevel.Full

        ' Formatters of the messages for delivery
        Dim clientProvider As New BinaryClientFormatterSinkProvider()
        Dim serverProvider As New BinaryServerFormatterSinkProvider()
        serverProvider.TypeFilterLevel = TypeFilterLevel.Full

        ' Create a TCP channel
        Dim tcpChannel As New TcpChannel(props, clientProvider, serverProvider)

        ' Register the TCP channel
        ChannelServices.RegisterChannel(tcpChannel, True)


        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Create a remotable object.
        ' 

        ' Create a SingleCall server-activated object
        Dim remoteObj As SingleCallObject = Activator.GetObject( _
        GetType(SingleCallObject), "tcp://localhost:6100/SingleCallService")

        ' [-or-] a Singleton server-activated object
        'Dim remoteObj As SingletonObject = Activator.GetObject( _
        'GetType(SingletonObject), "tcp://localhost:6100/SingletonService")

        ' [-or-] a client-activated object
        'RemotingConfiguration.RegisterActivatedClientType( _
        'GetType(ClientActivatedObject), "tcp://localhost:6100/RemotingService")
        'Dim remoteObj As New ClientActivatedObject()


        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        ' Use the remotable object as if it were a local object.
        ' 

        Dim remoteType As String = remoteObj.GetRemoteObjectType()
        Console.WriteLine("Call GetRemoteObjectType => {0}", remoteType)

        Console.WriteLine("The client process and thread: {0}, {1}", _
            GetCurrentProcessId(), GetCurrentThreadId())

        Dim processId, threadId As UInteger
        remoteObj.GetProcessThreadID(processId, threadId)
        Console.WriteLine("Call GetProcessThreadID => {0} {1}", _
                          processId, threadId)

        Console.WriteLine("Set FloatProperty += {0}", 1.2F)
        remoteObj.FloatProperty += 1.2F

        Console.WriteLine("Get FloatProperty = {0}", remoteObj.FloatProperty)

    End Sub


    ''' <summary>
    ''' Get current process ID.
    ''' </summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    <DllImport("kernel32.dll", EntryPoint:="GetCurrentProcessId")> _
    Friend Function GetCurrentProcessId() As UInteger
    End Function

    ''' <summary>
    ''' Get current thread ID.
    ''' </summary>
    ''' <returns></returns>
    ''' <remarks></remarks>
    <DllImport("kernel32.dll", EntryPoint:="GetCurrentThreadId")> _
    Friend Function GetCurrentThreadId() As UInteger
    End Function

End Module
