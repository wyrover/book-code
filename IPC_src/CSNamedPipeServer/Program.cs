/****************************** Module Header ******************************\
* Module Name:	Program.cs
* Project:		CSNamedPipeServer
* Copyright (c) Microsoft Corporation.
* 
* Named pipe is a mechanism for one-way or bi-directional inter-process 
* communication between the pipe server and one or more pipe clients in the
* local machine or across the computers in the intranet:
* 
* PIPE_ACCESS_INBOUND:
* Client (GENERIC_WRITE) ---> Server (GENERIC_READ)
* 
* PIPE_ACCESS_OUTBOUND:
* Client (GENERIC_READ) <--- Server (GENERIC_WRITE)
* 
* PIPE_ACCESS_DUPLEX:
* Client (GENERIC_READ or GENERIC_WRITE, or both) <--> 
* Server (GENERIC_READ and GENERIC_WRITE)
* 
* .NET supports named pipes in two ways:
* 
* 1. P/Invoke the native APIs.
* 
* By P/Invoke-ing the native APIs from .NET, we can mimic the code logic  
* in CppNamedPipeServer to create the named pipe server, \\.\pipe\HelloWorld,   
* that supports PIPE_ACCESS_DUPLEX.
* 
* PInvokeNativePipeServer first creates such a named pipe, then it listens to 
* the client's connection. When a client is connected, the server attempts to 
* read the client's requests from the pipe and write a response.
* 
* 2. System.IO.Pipes Namespace
* 
* In .NET Framework 3.5, the namespace System.IO.Pipes and a set of classes 
* (e.g. PipeStream, NamedPipeServerStream) are added to .NET BCL. These 
* classes make the programming of named pipe in .NET much easier and safer 
* than P/Invoke-ing the native APIs directly.
* 
* BCLSystemIOPipeServer first creates such a named pipe, then it listens to  
* the client's connection. When a client is connected, the server attempts to 
* read the client's requests from the pipe and write a response.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 1/27/2009 8:31 PM Jialiang Ge Created
* * 3/3/2009 10:10 AM Riquel Dong Added BCLSystemIOPipe
* * 3/3/2009 1:24 PM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO.Pipes;
using System.IO;
using System.Security.AccessControl;
#endregion


class Program
{
    const int BUFFER_SIZE = 4096;  // 4 KB

    static void Main(string[] args)
    {
        PInvokeNativePipeServer();

        // [-or-]

        //BCLSystemIOPipeServer();
    }

    /// <summary>
    /// Named pipe server through P/Invoke-ing the native APIs
    /// </summary>
    static void PInvokeNativePipeServer()
    {
        /////////////////////////////////////////////////////////////////////
        // Create a named pipe.
        // 

        // Prepare the pipe name
        String strPipeName = String.Format(@"\\{0}\pipe\{1}",
            ".",                // Server name
            "HelloWorld"        // Pipe name
            );

        // Prepare the security attributes

        IntPtr pSa = IntPtr.Zero;   // NULL
        SECURITY_ATTRIBUTES sa = new SECURITY_ATTRIBUTES();

        SECURITY_DESCRIPTOR sd;
        SecurityNative.InitializeSecurityDescriptor(out sd, 1);
        // DACL is set as NULL to allow all access to the object.
        SecurityNative.SetSecurityDescriptorDacl(ref sd, true, IntPtr.Zero, false);
        sa.lpSecurityDescriptor = Marshal.AllocHGlobal(Marshal.SizeOf(
            typeof(SECURITY_DESCRIPTOR)));
        Marshal.StructureToPtr(sd, sa.lpSecurityDescriptor, false);
        sa.bInheritHandle = false;              // Not inheritable
        sa.nLength = Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES));

        pSa = Marshal.AllocHGlobal(sa.nLength);
        Marshal.StructureToPtr(sa, pSa, false);

        // Create the named pipe.
        IntPtr hPipe = PipeNative.CreateNamedPipe(
            strPipeName,                        // The unique pipe name.
            PipeOpenMode.PIPE_ACCESS_DUPLEX,    // The pipe is bi-directional
            PipeMode.PIPE_TYPE_MESSAGE |        // Message type pipe 
            PipeMode.PIPE_READMODE_MESSAGE |    // Message-read mode 
            PipeMode.PIPE_WAIT,                 // Blocking mode is on
            PipeNative.PIPE_UNLIMITED_INSTANCES,// Max server instances
            BUFFER_SIZE,                        // Output buffer size
            BUFFER_SIZE,                        // Input buffer size
            PipeNative.NMPWAIT_USE_DEFAULT_WAIT,// Time-out interval
            pSa                                 // Pipe security attributes
            );

        if (hPipe.ToInt32() == PipeNative.INVALID_HANDLE_VALUE)
        {
            Console.WriteLine("Unable to create named pipe {0} w/err 0x{1:X}",
                strPipeName, PipeNative.GetLastError());
            return;
        }
        Console.WriteLine("The named pipe, {0}, is created.", strPipeName);


        /////////////////////////////////////////////////////////////////////
        // Wait for the client to connect.
        // 

        Console.WriteLine("Waiting for the client's connection...");

        bool bConnected = PipeNative.ConnectNamedPipe(hPipe, IntPtr.Zero) ?
            true : PipeNative.GetLastError() == PipeNative.ERROR_PIPE_CONNECTED;

        if (!bConnected)
        {
            Console.WriteLine(
                "Error occurred while connecting to the client: 0x{0:X}",
                PipeNative.GetLastError());
            PipeNative.CloseHandle(hPipe);      // Close the pipe handle.
            return;
        }


        /////////////////////////////////////////////////////////////////////
        // Read client requests from the pipe and write the response.
        // 

        // A byte buffer of BUFFER_SIZE bytes. The buffer should be big 
        // enough for ONE request from a client.

        string strMessage;
        byte[] bRequest = new byte[BUFFER_SIZE];// Client -> Server
        uint cbBytesRead, cbRequestBytes;
        byte[] bReply;                          // Server -> Client
        uint cbBytesWritten, cbReplyBytes;

        bool bResult;

        while (true)
        {
            // Receive one message from the pipe.

            cbRequestBytes = BUFFER_SIZE;
            bResult = PipeNative.ReadFile(      // Read from the pipe.
                hPipe,                          // Handle of the pipe
                bRequest,                       // Buffer to receive data
                cbRequestBytes,                 // Size of buffer in bytes
                out cbBytesRead,                // Number of bytes read
                IntPtr.Zero);                   // Not overlapped I/O

            if (!bResult/*Failed*/ || cbBytesRead == 0/*Finished*/)
                break;

            // Unicode-encode the byte array and trim all the '\0' chars at 
            // the end.
            strMessage = Encoding.Unicode.GetString(bRequest).TrimEnd('\0');
            Console.WriteLine("Receives {0} bytes; Message: \"{1}\"",
                cbBytesRead, strMessage);

            // Prepare the response.

            // '\0' is appended in the end because the client may be a native
            // C++ program.
            strMessage = "Default response from server\0";
            bReply = Encoding.Unicode.GetBytes(strMessage);
            cbReplyBytes = (uint)bReply.Length;

            // Write the response to the pipe.

            bResult = PipeNative.WriteFile(     // Write to the pipe.
                hPipe,                          // Handle of the pipe
                bReply,                         // Buffer to write to 
                cbReplyBytes,                   // Number of bytes to write 
                out cbBytesWritten,             // Number of bytes written 
                IntPtr.Zero);                   // Not overlapped I/O 

            if (!bResult/*Failed*/ || cbReplyBytes != cbBytesWritten/*Failed*/)
            {
                Console.WriteLine("WriteFile failed w/err 0x{0:X}",
                    PipeNative.GetLastError());
                break;
            }

            Console.WriteLine("Replies {0} bytes; Message: \"{1}\"",
                cbBytesWritten, strMessage.TrimEnd('\0'));
        }


        /////////////////////////////////////////////////////////////////////
        // Flush the pipe to allow the client to read the pipe's contents 
        // before disconnecting. Then disconnect the pipe, and close the 
        // handle to this pipe instance.
        // 

        PipeNative.FlushFileBuffers(hPipe);
        PipeNative.DisconnectNamedPipe(hPipe);
        PipeNative.CloseHandle(hPipe);
    }

    /// <summary>
    /// Named pipe server through BCL System.IO.Pipes
    /// </summary>
    static void BCLSystemIOPipeServer()
    {
        NamedPipeServerStream pipeServer = null;

        try
        {
            /////////////////////////////////////////////////////////////////
            // Create a named pipe.
            // 

            // Prepare the pipe name       
            String strPipeName = "HelloWorld";

            // Prepare the security attributes
            // Granting everyone the full control of the pipe is just for 
            // demo purpose, though it creates a security hole.
            PipeSecurity pipeSa = new PipeSecurity();
            pipeSa.SetAccessRule(new PipeAccessRule("Everyone", 
                PipeAccessRights.ReadWrite, AccessControlType.Allow));

            // Create the named pipe
            pipeServer = new NamedPipeServerStream(
                strPipeName,                    // The unique pipe name.
                PipeDirection.InOut,            // The pipe is bi-directional
                NamedPipeServerStream.MaxAllowedServerInstances,
                PipeTransmissionMode.Message,   // Message type pipe 
                PipeOptions.None,               // No additional parameters
                BUFFER_SIZE,                    // Input buffer size
                BUFFER_SIZE,                    // Output buffer size
                pipeSa,                         // Pipe security attributes
                HandleInheritability.None       // Not inheritable
                );

            Console.WriteLine("The named pipe, {0}, is created", strPipeName);
            

            /////////////////////////////////////////////////////////////////
            // Wait for the client to connect.
            // 

            Console.WriteLine("Waiting for the client's connection...");
            pipeServer.WaitForConnection();

            /////////////////////////////////////////////////////////////////
            // Read client requests from the pipe and write the response.
            // 

            // A byte buffer of BUFFER_SIZE bytes. The buffer should be big 
            // enough for ONE request from a client.

            string strMessage;
            byte[] bRequest = new byte[BUFFER_SIZE];// Client -> Server
            int cbBytesRead, cbRequestBytes;
            byte[] bReply;                          // Server -> Client
            int cbBytesWritten, cbReplyBytes;

            do
            {
                // Receive one message from the pipe.

                cbRequestBytes = BUFFER_SIZE;
                cbBytesRead = pipeServer.Read(bRequest, 0, cbRequestBytes);

                // Unicode-encode the byte array and trim all the '\0' chars 
                // at the end.
                strMessage = Encoding.Unicode.GetString(bRequest).TrimEnd('\0');
                Console.WriteLine("Receives {0} bytes; Message: \"{1}\"",
                    cbBytesRead, strMessage);

                // Prepare the response.

                // '\0' is appended in the end because the client may be a  
                // native C++ program.
                strMessage = "Default response from server\0";
                bReply = Encoding.Unicode.GetBytes(strMessage);
                cbReplyBytes = bReply.Length;

                // Write the response to the pipe.

                pipeServer.Write(bReply, 0, cbReplyBytes);
                // If no IO exception is thrown from Write, number of bytes 
                // written (cbBytesWritten) != -1.
                cbBytesWritten = cbReplyBytes;

                Console.WriteLine("Replies {0} bytes; Message: \"{1}\"",
                    cbBytesWritten, strMessage.TrimEnd('\0'));

            }
            while (!pipeServer.IsMessageComplete);


            /////////////////////////////////////////////////////////////////
            // Flush the pipe to allow the client to read the pipe's contents 
            // before disconnecting. Then disconnect the pipe.
            // 

            pipeServer.Flush();
            pipeServer.Disconnect();

        }
        catch (Exception ex)
        {
            Console.WriteLine("The server throws the error: {0}", ex.Message);
        }
        finally
        {
            if (pipeServer != null)
            {
                // Close the stream.
                pipeServer.Close();
            }
        }
    }
}

