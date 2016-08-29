'****************************** Module Header ******************************'
' Module Name:	ServerActivatedObject.vb
' Project:		VBRemotingServer
' Copyright (c) Microsoft Corporation.
' 
' ServerActivatedObject.vb defines the server activated types for .NET 
' Remoting. Server-activated objects are created by the server and their 
' lifetime is also managed by the server. These objects are not created when 
' a client calls "new" or Activator.GetObject; rather, the actual instance 
' of the object is created when the client actually invokes a method on proxy. 
' 
' There are two modes of server-activated type: "single call" and "singleton".
' 
' SingleCall: Such objects are created on each method call and objects are 
' not shared among clients. State should not be maintained in such objects 
' because they are destroyed after each method call. 
' 
' Singleton: Only one object will be created on the server to fulfill the 
' requests of all the clients; that means the object is shared, and the state 
' will be shared by all the clients. 
' 
' This source is subject to the Microsoft Public License.
' See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
' All other rights reserved.
' 
' History:
' * 3/8/2009 3:40 PM Jialiang Ge Created
'***************************************************************************'

#Region "Imports directives"

Imports System.Runtime.InteropServices

#End Region


Namespace RemotingShared

    ''' <summary>
    ''' A server-activated type for .NET Remoting.
    ''' </summary>
    ''' <remarks></remarks>
    Friend Class ServerActivatedObject
        Inherits MarshalByRefObject

        Protected fField As Single

        ''' <summary>
        ''' A float property
        ''' </summary>
        ''' <value></value>
        ''' <returns></returns>
        ''' <remarks></remarks>
        Public Property FloatProperty() As Single
            Get
                Return fField
            End Get
            Set(ByVal value As Single)
                fField = value
            End Set
        End Property

        ''' <summary>
        ''' Get the type of the remote object. 
        ''' ClientActivatedObject or ServerActivatedObject
        ''' </summary>
        ''' <returns></returns>
        ''' <remarks></remarks>
        Public Overridable Function GetRemoteObjectType() As String
            Return "ServerActivatedObject"
        End Function

        ''' <summary>
        ''' Get the current process ID and thread ID.
        ''' </summary>
        ''' <param name="processId">current process ID</param>
        ''' <param name="threadId">current thread ID</param>
        ''' <remarks></remarks>
        Public Sub GetProcessThreadID(ByRef processId As UInteger, _
                                      ByRef threadId As UInteger)
            processId = GetCurrentProcessId()
            threadId = GetCurrentThreadId()
        End Sub

        ''' <summary>
        ''' Get current process ID.
        ''' </summary>
        ''' <returns></returns>
        ''' <remarks></remarks>
        <DllImport("kernel32.dll", EntryPoint:="GetCurrentProcessId")> _
        Friend Shared Function GetCurrentProcessId() As UInteger
        End Function

        ''' <summary>
        ''' Get current thread ID.
        ''' </summary>
        ''' <returns></returns>
        ''' <remarks></remarks>
        <DllImport("kernel32.dll", EntryPoint:="GetCurrentThreadId")> _
        Friend Shared Function GetCurrentThreadId() As UInteger
        End Function

    End Class

    ''' <summary>
    ''' A SingleCall server-activated type for .NET Remoting.
    ''' </summary>
    ''' <remarks></remarks>
    Friend Class SingleCallObject
        Inherits ServerActivatedObject

        Public Overrides Function GetRemoteObjectType() As String
            Return "SingleCallObject"
        End Function

    End Class

    ''' <summary>
    ''' A Singleton server-activated type for .NET Remoting.
    ''' </summary>
    ''' <remarks></remarks>
    Friend Class SingletonObject
        Inherits ServerActivatedObject

        Public Overrides Function GetRemoteObjectType() As String
            Return "SingletonObject"
        End Function

    End Class
End Namespace