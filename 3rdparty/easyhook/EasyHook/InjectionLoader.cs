/*
    EasyHook - The reinvention of Windows API hooking
 
    Copyright (C) 2009 Christoph Husse

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Please visit http://www.codeplex.com/easyhook for more information
    about the project and latest updates.
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Runtime.InteropServices;

namespace EasyHook
{
#pragma warning disable 1591
#pragma warning disable 0618

    public class InjectionLoader
    {
        [StructLayout(LayoutKind.Sequential)]
        class REMOTE_ENTRY_INFO : RemoteHooking.IContext
        {
            public Int32 m_HostPID;
            public IntPtr UserData;
            public Int32 UserDataSize;

            public Int32 HostPID { get { return m_HostPID; } }
        };

        static List<String> ConnectedChannels = new List<String>();

        private static void Release(Type InEntryPoint)
        {
            if (InEntryPoint == null)
                return;

            LocalHook.Release();
        }

        #pragma warning disable 0028
        public static int Main(String InParam)
        {
            HelperServiceInterface Interface;
            Assembly UserAssembly = null;
            Type EntryPoint = null;
            ManagedRemoteInfo RemoteInfo;
            REMOTE_ENTRY_INFO UnmanagedInfo = new REMOTE_ENTRY_INFO();

            if (InParam == null)
                return 0;

            /*
             * We will now connect to our hooking host. This is to provide extensive
             * error information.
             */
            try
            {

                Marshal.PtrToStructure(
                    (IntPtr)Int64.Parse(InParam, System.Globalization.NumberStyles.HexNumber),
                    UnmanagedInfo);

                Byte[] PassThruBytes = new Byte[UnmanagedInfo.UserDataSize];
                MemoryStream PassThruStream = new MemoryStream();
                BinaryFormatter Format = new BinaryFormatter();

                Marshal.Copy(UnmanagedInfo.UserData, PassThruBytes, 0, UnmanagedInfo.UserDataSize);
              
                PassThruStream.Write(PassThruBytes, 0, PassThruBytes.Length);
                PassThruStream.Position = 0;

                RemoteInfo = (ManagedRemoteInfo)Format.Deserialize(PassThruStream);

                Interface = RemoteHooking.IpcConnectClient<HelperServiceInterface>(RemoteInfo.ChannelName);

                // ensure connection...
                Interface.Ping();

                if (!ConnectedChannels.Contains(RemoteInfo.ChannelName))
                {
                    ConnectedChannels.Add(RemoteInfo.ChannelName);

                    return 1;
                }
            }
            catch (Exception ExtInfo)
            {
                Config.PrintError(ExtInfo.ToString());

                return 0;
            }

            try
            {
                
                // adjust host PID in case of WOW64 bypass and service help...
                UnmanagedInfo.m_HostPID = RemoteInfo.HostPID;

                // prepare parameter array
                Object[] ParamArray = new Object[1 + RemoteInfo.UserParams.Length];

                ParamArray[0] = (RemoteHooking.IContext)UnmanagedInfo;

                for (int i = 0; i < RemoteInfo.UserParams.Length; i++)
                {
                    ParamArray[i + 1] = RemoteInfo.UserParams[i];
                }


                /*
                 * After this we are ready to load the user supplied library.
                 */
                Object Instance = null;

                try
                {
                    UserAssembly = Assembly.LoadWithPartialName(RemoteInfo.UserLibrary);

                    // search for user library entry point...
                    Type[] ExportedTypes = UserAssembly.GetExportedTypes();

                    for (int iType = 0; iType < ExportedTypes.Length; iType++)
                    {
                        EntryPoint = ExportedTypes[iType];

                        if (EntryPoint.GetInterface("EasyHook.IEntryPoint") != null)
                            break;

                        EntryPoint = null;
                    }

                    if (EntryPoint == null)
                        throw new EntryPointNotFoundException("The given user library does not export a proper type implementing the 'EasyHook.IEntryPoint' interface.");

                    // test for strictly typed Run() method
                    MethodInfo Run = EntryPoint.GetMethod("Run", BindingFlags.Public | BindingFlags.Instance);

                    if (Run != null)
                    {
                        if (ParamArray.Length != Run.GetParameters().Length)
                            throw new MissingMethodException("The given user library does export a Run() method in the 'EasyHook.IEntryPoint' interface, but the parameter count does not match!");

                        for(int i = 0; i < ParamArray.Length; i++){
                            if (!Run.GetParameters()[i].ParameterType.IsInstanceOfType(ParamArray[i]))
                                throw new MissingMethodException("The given user library does export a Run() method in the 'EasyHook.IEntryPoint' interface, but the parameter types do not match!");
                        }
                    } else
                        throw new MissingMethodException("The given user library does not export a proper Run() method in the 'EasyHook.IEntryPoint' interface.");
    
                    // assemble user parameters and initialize library...
                    Instance = EntryPoint.InvokeMember(null, BindingFlags.Public | BindingFlags.Instance |
                        BindingFlags.CreateInstance, null, null, ParamArray);

                    // notify the host about successful injection...
                    Interface.InjectionCompleted(RemoteHooking.GetCurrentProcessId());
                }
                catch (Exception ExtInfo)
                {
                    // we will now get extensive error information on host side...
                    try
                    {
                        Interface.InjectionException(RemoteHooking.GetCurrentProcessId(), ExtInfo);
                    }
                    finally
                    {
                        Instance = null;

                        Release(EntryPoint);
                    }

                    return -1;
                }

                try
                {
                    /* 
                     * After this it is safe to enter the main method, which will block until assembly unloading...
                     * From now on the user library has to take care about error reporting!
                     */
                    EntryPoint.InvokeMember("Run", BindingFlags.Public | BindingFlags.Instance | BindingFlags.ExactBinding |
                         BindingFlags.InvokeMethod, null, Instance, ParamArray);
                }   
                finally
                {
                    Instance = null;

                    Release(EntryPoint);
                }
            }
            catch (Exception ExtInfo)
            {
                Config.PrintWarning(ExtInfo.ToString());

                return -1;
            }
            finally
            {
                if (ConnectedChannels.Contains(RemoteInfo.ChannelName))
                    ConnectedChannels.Remove(RemoteInfo.ChannelName);
            }

            return 0;
        }
    }
}
