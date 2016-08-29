/****************************** Module Header ******************************\
* Module Name:	ServerActivatedObjectProxy.cs
* Project:		CSRemotingClient
* Copyright (c) Microsoft Corporation.
* 
* ServerActivatedObjectProxy.cs defines the proxy of the server-activated 
* types. The proxy has no implementation of the types' methods and properties.
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
using System.Text;
#endregion


namespace RemotingShared
{
    /// <summary>
    /// The proxy of the server-activated type for .NET Remoting.
    /// </summary>
    internal class ServerActivatedObject : MarshalByRefObject
    {
        public float FloatProperty
        {
            get { throw new NotImplementedException(); }
            set { throw new NotImplementedException(); }
        }

        public virtual string GetRemoteObjectType()
        {
            throw new NotImplementedException();
        }

        public void GetProcessThreadID(out uint processId, out uint threadId)
        {
            throw new NotImplementedException();
        }
    }

    internal class SingleCallObject : ServerActivatedObject
    {
    }

    internal class SingletonObject : ServerActivatedObject
    {
    }
}
