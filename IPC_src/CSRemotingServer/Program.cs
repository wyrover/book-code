/****************************** Module Header ******************************\
* Module Name:	Program.cs
* Project:		CSRemotingServer
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
* CSRemotingServer is a .NET Remoting server end. It exposes the following 
* remote objects:
* 
* 1. RemotingShared.SingleCallObject
* URL: tcp://localhost:6100/SingleCallService
* 
* SingleCallObject is a server-activated object (SAO) for .NET Remoting, with 
* the "SingleCall" mode. Such objects are created on each method call and 
* objects are not shared among clients. State should not be maintained in  
* such objects because they are destroyed after each method call. 
* 
* 2. RemotingShared.SingletonObject
* URL: tcp://localhost:6100/SingletonService
* 
* SingletonObject is a server-activated object (SAO) for .NET Remoting, with  
* the "Singleton" mode. Only one object will be created on the server to 
* fulfill the requests of all the clients; that means the object is shared,  
* and the state will be shared by all the clients. 
* 
* 3. RemotingShared.ClientActivatedObject defined in the shared assembly 
* CSRemotingSharedLibrary.DLL:
* URL: tcp://localhost:6100/RemotingService
* 
* ClientActivatedObject is a client-activated object (CAO) for .NET Remoting. 
* Client-activated objects are created by the server and their lifetime is 
* managed by the client. In contrast to server-activated objects, client-
* activated objects are created as soon as the client calls "new" or any  
* other object creation methods. Client-activated objects are specific to the 
* client, and objects are not shared among different clients; object instance 
* exists until the lease expires or the client destroys the object. 
* 
* There are generally two ways to create the .NET Remoting server: using a 
* configuration file or writing codes. The RemotingServerByConfig method 
* demonstrates the former, and the RemotingServerByCode method illustrates 
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
using System.Text;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters;
using System.Collections;
#endregion


class Program
{
    static void Main(string[] args)
    {
        RemotingServerByConfig();

        // [-or-]
        
        //RemotingServerByCode();

        Console.WriteLine("Press Enter to exit the Remoting server");
        Console.ReadLine();
    }

    /// <summary>
    /// Create the .NET Remoting server using configuration file.
    /// </summary>
    static void RemotingServerByConfig()
    {
        /////////////////////////////////////////////////////////////////////
        // Reads the configuration file and configures the remoting 
        // infrastructure for the server end.
        // 

        // The format for .NET Remoting configuration file:
        // http://msdn.microsoft.com/en-us/library/ms973907.aspx
        RemotingConfiguration.Configure("CSRemotingServer.exe.config", true);
    }

    /// <summary>
    /// Create the .NET Remoting server using code.
    /// </summary>
    static void RemotingServerByCode()
    {
        /////////////////////////////////////////////////////////////////////
        // Create and register a channel (TCP channel in this example) that 
        // is used to transport messages across the remoting boundary.
        // 

        // Properties of the channel
        IDictionary props = new Hashtable();
        props["port"] = 6100;   // Port of the TCP channel
        props["typeFilterLevel"] = TypeFilterLevel.Full;

        // Formatters of the messages for delivery
        BinaryClientFormatterSinkProvider clientProvider = null;
        BinaryServerFormatterSinkProvider serverProvider =
            new BinaryServerFormatterSinkProvider();
        serverProvider.TypeFilterLevel = TypeFilterLevel.Full;

        // Create a TCP channel
        TcpChannel tcpChannel = new TcpChannel(props, clientProvider,
            serverProvider);
        
        // Register the TCP channel
        ChannelServices.RegisterChannel(tcpChannel, true);


        /////////////////////////////////////////////////////////////////////
        // Register the remotable types on the service end as 
        // server-activated types (aka well-known types) or client-activated 
        // types.
        // 

        // Register RemotingShared.SingleCallObject as a SingleCall server-
        // activated type.
        RemotingConfiguration.RegisterWellKnownServiceType(
            typeof(RemotingShared.SingleCallObject),// Server-activated type
            "SingleCallService",                    // objectUri
            WellKnownObjectMode.SingleCall);        // SingleCall mode

        // Register RemotingShared.SingletonObject as a Singleton server-
        // activated type.
        RemotingConfiguration.RegisterWellKnownServiceType(
            typeof(RemotingShared.SingletonObject), // Server-activated type
            "SingletonService",                     // objectUri
            WellKnownObjectMode.Singleton);         // Singleton mode

        // Register RemotingShared.ClientActivatedObject as a client-
        // activated type.
        RemotingConfiguration.ApplicationName = "RemotingService";
        RemotingConfiguration.RegisterActivatedServiceType(
            typeof(RemotingShared.ClientActivatedObject));
    }
}