/****************************** Module Header ******************************\
* Module Name:	ServerActivatedObject.cs
* Project:		CSRemotingServer
* Copyright (c) Microsoft Corporation.
* 
* ServerActivatedObject.cs defines the server activated types for .NET 
* Remoting. Server-activated objects are created by the server and their 
* lifetime is also managed by the server. These objects are not created when 
* a client calls "new" or Activator.GetObject; rather, the actual instance 
* of the object is created when the client actually invokes a method on proxy. 
* 
* There are two modes of server-activated type: "single call" and "singleton".
* 
* SingleCall: Such objects are created on each method call and objects are 
* not shared among clients. State should not be maintained in such objects 
* because they are destroyed after each method call. 
* 
* Singleton: Only one object will be created on the server to fulfill the 
* requests of all the clients; that means the object is shared, and the state 
* will be shared by all the clients. 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/7/2009 2:52 PM Jialiang Ge Created
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
#endregion


namespace RemotingShared
{
    /// <summary>
    /// A server-activated type for .NET Remoting.
    /// </summary>
    internal class ServerActivatedObject : MarshalByRefObject
    {
        protected float fField;

        /// <summary>
        /// A float property.
        /// </summary>
        public float FloatProperty
        {
            get { return this.fField; }
            set { this.fField = value; }
        }

        /// <summary>
        /// Get the type of the remote object. 
        /// ClientActivatedObject or ServerActivatedObject
        /// </summary>
        /// <returns></returns>
        public virtual string GetRemoteObjectType()
        {
            return "ServerActivatedObject";
        }

        /// <summary>
        /// Get the current process ID and thread ID.
        /// </summary>
        /// <param name="processId">current process ID</param>
        /// <param name="threadId">current thread ID</param>
        public void GetProcessThreadID(out uint processId, out uint threadId)
        {
            processId = GetCurrentProcessId();
            threadId = GetCurrentThreadId();
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

    /// <summary>
    /// A SingleCall server-activated type for .NET Remoting.
    /// </summary>
    internal class SingleCallObject : ServerActivatedObject
    {
        public override string GetRemoteObjectType()
        {
            return "SingleCallObject";
        }
    }

    /// <summary>
    /// A Singleton server-activated type for .NET Remoting.
    /// </summary>
    internal class SingletonObject : ServerActivatedObject
    {
        public override string GetRemoteObjectType()
        {
            return "SingletonObject";
        }
    }
}
