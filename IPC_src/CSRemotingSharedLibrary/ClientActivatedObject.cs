/****************************** Module Header ******************************\
* Module Name:	ClientActivatedObject.cs
* Project:		CSRemotingSharedLibrary
* Copyright (c) Microsoft Corporation.
* 
* ClientActivatedObject.cs defines a client-activated type for .NET Remoting.
* Client-activated objects are created by the server and their lifetime is 
* managed by the client. In contrast to server-activated objects, client-
* activated objects are created as soon as the client calls "new" or any 
* other object creation methods. Client-activated objects are specific to the 
* client, and objects are not shared among different clients; object instance 
* exists until the lease expires or the client destroys the object. 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/7/2009 10:50 PM Jialiang Ge Created
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
#endregion


namespace RemotingShared
{
    /// <summary>
    /// A client-activated type for .NET Remoting.
    /// </summary>
    public class ClientActivatedObject : MarshalByRefObject
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
            return "ClientActivatedObject";
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
}
