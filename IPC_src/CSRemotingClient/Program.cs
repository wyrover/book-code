/****************************** Module Header ******************************\
* Module Name:	Program.cs
* Project:		CSRemotingClient
* Copyright (c) Microsoft Corporation.
* 
* .NET Remoting is a mechanism for one-way inter-process communication and   
* RPC between .NET applications in the local machine or across the computers 
* in the intranet and internet.
* 
* .NET Remoting allows an application to make an remotable object available 
* across remoting boundaries, which includes different appdomains, processes 
* or even different computers connected by a network. .NET Remoting makes a 
* reference of a remotable object available to a client application, which  
* then instantiates and uses a remotable object as if it were a local object. 
* However, the actual code execution happens at the server-side. Any requests 
* to the remotable object are proxied by the .NET Remoting runtime over  
* Channel objects, that encapsulate the actual transport mode, including TCP 
* streams, HTTP streams and named pipes. As a result, by instantiating proper 
* Channel objects, a .NET Remoting application can be made to support 
* different communication protocols without recompiling the application. The  
* runtime itself manages the act of serialization and marshalling of objects 
* across the client and server appdomains.
* 
* CSRemotingClient is a .NET Remoting client end. It accesses the remote 
* objects exposed by the .NET Remoting server end, CSRemotingServer.
* 
* There are generally two ways to create the .NET Remoting client: using a 
* configuration file or writing codes. The RemotingClientByConfig method 
* demonstrates the former, and the RemotingClientByCode method illustrates  
* the latter method.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/7/2009 2:50 PM Jialiang Ge Created
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting;
using System.Runtime.Serialization.Formatters;
using System.Runtime.InteropServices;

using RemotingShared;
#endregion


class Program
{
    static void Main(string[] args)
    {
        //RemotingClientByConfig();

        // [-or-]

        RemotingClientByCode();
    }

    /// <summary>
    /// Access the .NET Remoting server using configuration file.
    /// </summary>
    static void RemotingClientByConfig()
    {
        /////////////////////////////////////////////////////////////////////
        // Reads the configuration file and configures the remoting 
        // infrastructure for the client end.
        // 

        // The format for .NET Remoting configuration file:
        // http://msdn.microsoft.com/en-us/library/ms973907.aspx
        RemotingConfiguration.Configure("CSRemotingClient.exe.config", true);


        /////////////////////////////////////////////////////////////////////
        // Create a remotable object.
        // 

        // Create a SingleCall server-activated object
        SingleCallObject remoteObj = new SingleCallObject();

        // [-or-] a Singleton server-activated object
        //SingletonObject remoteObj = new SingletonObject();
        
        // [-or-] a client-activated object
        //ClientActivatedObject remoteObj = new ClientActivatedObject();


        /////////////////////////////////////////////////////////////////////
        // Use the remotable object as if it were a local object.
        // 

        string remoteType = remoteObj.GetRemoteObjectType();
        Console.WriteLine("Call GetRemoteObjectType => {0}", remoteType);

        Console.WriteLine("The client process and thread: {0}, {1}",
            GetCurrentProcessId(), GetCurrentThreadId());

        uint processId, threadId;
        remoteObj.GetProcessThreadID(out processId, out threadId);
        Console.WriteLine("Call GetProcessThreadID => {0} {1}", processId, threadId);

        Console.WriteLine("Set FloatProperty += {0}", 1.2f);
        remoteObj.FloatProperty += 1.2f;

        Console.WriteLine("Get FloatProperty = {0}", remoteObj.FloatProperty);
    }

    /// <summary>
    /// Access the .NET Remoting server using code.
    /// </summary>
    static void RemotingClientByCode()
    {
        /////////////////////////////////////////////////////////////////////
        // Create and register a channel (TCP channel in this example) that 
        // is used to transport messages across the remoting boundary.
        // 

        // Properties of the channel
        IDictionary props = new Hashtable();
        props["typeFilterLevel"] = TypeFilterLevel.Full;

        // Formatters of the messages for delivery
        BinaryClientFormatterSinkProvider clientProvider =
            new BinaryClientFormatterSinkProvider();
        BinaryServerFormatterSinkProvider serverProvider =
            new BinaryServerFormatterSinkProvider();
        serverProvider.TypeFilterLevel = TypeFilterLevel.Full;

        // Create a TCP channel
        TcpChannel tcpChannel = new TcpChannel(props, clientProvider,
            serverProvider);

        // Register the TCP channel
        ChannelServices.RegisterChannel(tcpChannel, true);


        /////////////////////////////////////////////////////////////////////
        // Create a remotable object.
        // 

        // Create a SingleCall server-activated object
        SingleCallObject remoteObj = (SingleCallObject)Activator.GetObject(
            typeof(SingleCallObject),
            "tcp://localhost:6100/SingleCallService");
        
        // [-or-] a Singleton server-activated object
        //SingletonObject remoteObj = (SingletonObject)Activator.GetObject(
        //    typeof(SingletonObject),
        //    "tcp://localhost:6100/SingletonService");

        // [-or-] a client-activated object
        //RemotingConfiguration.RegisterActivatedClientType(
        //    typeof(ClientActivatedObject),
        //    "tcp://localhost:6100/RemotingService");
        //ClientActivatedObject remoteObj = new ClientActivatedObject();


        /////////////////////////////////////////////////////////////////////
        // Use the remotable object as if it were a local object.
        // 

        string remoteType = remoteObj.GetRemoteObjectType();
        Console.WriteLine("Call GetRemoteObjectType => {0}", remoteType);

        Console.WriteLine("The client process and thread: {0}, {1}",
            GetCurrentProcessId(), GetCurrentThreadId());

        uint processId, threadId;
        remoteObj.GetProcessThreadID(out processId, out threadId);
        Console.WriteLine("Call GetProcessThreadID => {0} {1}", processId, threadId);

        Console.WriteLine("Set FloatProperty += {0}", 1.2f);
        remoteObj.FloatProperty += 1.2f;

        Console.WriteLine("Get FloatProperty = {0}", remoteObj.FloatProperty);
    }


    /// <summary>
    /// Get current thread ID.
    /// </summary>
    /// <returns></returns>
    [DllImport("kernel32.dll")]
    internal static extern uint GetCurrentThreadId();

    /// <summary>
    /// Get current process ID.
    /// </summary>
    [DllImport("kernel32.dll")]
    internal static extern uint GetCurrentProcessId();
}