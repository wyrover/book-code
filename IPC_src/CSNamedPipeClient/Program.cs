/****************************** Module Header ******************************\
* Module Name:	Program.cs
* Project:		CSNamedPipeClient
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
* in CppNamedPipeClient to connect to the pipe server.
* 
* This sample first connects to the named pipe, \\.\pipe\HelloWorld, with 
* the GENERIC_READ and GENERIC_WRITE permissions. The client writes a 
* message to the pipe server and receives its response.
* 
* 2. System.IO.Pipes Namespace
* 
* In .NET Framework 3.5, the namespace System.IO.Pipes and a set of classes 
* (e.g. PipeStream, NamedPipeClientStream) are added to .NET BCL. These 
* classes make the programming of named pipe in .NET much easier and safer  
* than P/Invoke-ing the native APIs directly.
* 
* BCLSystemIOPipeClient first connects a named pipe,\\.\pipe\HelloWorld.Then 
* writes a message to the pipe server and receives its response.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 1/27/2009 8:31 PM Jialiang Ge Created
* * 3/3/2009 10:10 AM Riquel Dong Added BCLSystemIOPipeClient
* * 3/4/2009 7:51 PM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Pipes;
using System.Security.Principal;
#endregion


class Program
{
    const int BUFFER_SIZE = 1024;  // 1 KB

    /// <summary>
    /// Main
    /// </summary>
    static void Main(string[] args)
    {
        //PInvokeNativePipeClient();

        // [-or-]

        BCLSystemIOPipeClient();
    }

    static void PInvokeNativePipeClient()
    {
        /////////////////////////////////////////////////////////////////////
        // Try to open a named pipe.
        // 

        // Prepare the pipe name
        String strPipeName = String.Format(@"\\{0}\pipe\{1}",
            ".",                // Server name
            "HelloWorld"        // Pipe name
            );

        IntPtr hPipe;
        while (true)
        {
            hPipe = PipeNative.CreateFile(
                strPipeName,                    // Pipe name 
                FileDesiredAccess.GENERIC_READ |// Read and write access
                FileDesiredAccess.GENERIC_WRITE,

                FileShareMode.Zero,             // No sharing 
                IntPtr.Zero,                    // Default security attributes
                FileCreationDisposition.OPEN_EXISTING,  // Opens existing pipe
                0,                              // Default attributes 
                0);                             // No template file 

            // Break if the pipe handle is valid. 
            if (hPipe.ToInt32() != PipeNative.INVALID_HANDLE_VALUE)
                break;

            if (// Exit if an error other than ERROR_PIPE_BUSY occurs
                PipeNative.GetLastError() != PipeNative.ERROR_PIPE_BUSY
                ||
                // All pipe instances are busy, so wait for five seconds
                !PipeNative.WaitNamedPipe(strPipeName, 5000))
            {
                Console.WriteLine("Unable to open named pipe {0} w/err 0x{1:X}",
                    strPipeName, PipeNative.GetLastError());
                return;
            }
        }
        Console.WriteLine("The named pipe, {0}, is connected.", strPipeName);


        /////////////////////////////////////////////////////////////////////
        // The pipe connected; change to message-read mode. 
        // 

        PipeMode mode = PipeMode.PIPE_READMODE_MESSAGE;
        bool bResult = PipeNative.SetNamedPipeHandleState(
            hPipe, ref mode, IntPtr.Zero, IntPtr.Zero);
        if (!bResult)
        {
            Console.WriteLine("SetNamedPipeHandleState failed w/err 0x{0:X}",
                PipeNative.GetLastError());
            return;
        }


        /////////////////////////////////////////////////////////////////////
        // Send a message to the pipe server and receive its response.
        // 

        // A byte buffer of BUFFER_SIZE bytes. The buffer should be big 
        // enough for ONE request to the server.

        string strMessage;
        byte[] bRequest;                        // Client -> Server
        uint cbBytesWritten, cbRequestBytes;
        byte[] bReply = new byte[BUFFER_SIZE];  // Server -> Client
        uint cbBytesRead, cbReplyBytes;

        // Send one message to the pipe.

        // '\0' is appended in the end because the client may be a native
        // C++ program.
        strMessage = "Default request from client\0";
        bRequest = Encoding.Unicode.GetBytes(strMessage);
        cbRequestBytes = (uint)bRequest.Length;

        bResult = PipeNative.WriteFile(         // Write to the pipe.
            hPipe,                              // Handle of the pipe
            bRequest,                           // Message to be written
            cbRequestBytes,                     // Number of bytes to write
            out cbBytesWritten,                 // Number of bytes written
            IntPtr.Zero);                       // Not overlapped 

        if (!bResult/*Failed*/ || cbRequestBytes != cbBytesWritten/*Failed*/)
        {
            Console.WriteLine("WriteFile failed w/err 0x{0:X}",
                PipeNative.GetLastError());
            return;
        }

        Console.WriteLine("Sends {0} bytes; Message: \"{1}\"",
            cbBytesWritten, strMessage.TrimEnd('\0'));

        // Receive the response from the server.

        cbReplyBytes = BUFFER_SIZE;
        do
        {
            bResult = PipeNative.ReadFile(      // Read from the pipe.
                hPipe,                          // Handle of the pipe
                bReply,                         // Buffer to receive the reply
                cbReplyBytes,                   // Size of buffer 
                out cbBytesRead,                // Number of bytes read 
                IntPtr.Zero);                   // Not overlapped 

            if (!bResult &&
                PipeNative.GetLastError() != PipeNative.ERROR_MORE_DATA)
            {
                Console.WriteLine("ReadFile failed w/err 0x{0:X}",
                    PipeNative.GetLastError());
                break;
            }

            strMessage = Encoding.Unicode.GetString(bReply).TrimEnd('\0');
            Console.WriteLine("Receives {0} bytes; Message: \"{1}\"",
                cbBytesRead, strMessage);

        } while (!bResult);  // Repeat loop if ERROR_MORE_DATA 


        /////////////////////////////////////////////////////////////////////
        // Close the pipe.
        // 

        PipeNative.CloseHandle(hPipe);
    }

    static void BCLSystemIOPipeClient()
    {
        /////////////////////////////////////////////////////////////////////
        // Try to open a named pipe.
        // 

        // Prepare the pipe name
        String strServerName = ".";
        String strPipeName = "HelloWorld";

        NamedPipeClientStream pipeClient = null;

        try
        {
            pipeClient = new NamedPipeClientStream(
                strServerName,              // The server name
                strPipeName,                // The unique pipe name
                PipeDirection.InOut,        // The pipe is bi-directional   
                PipeOptions.None,           // No additional parameters

                //The server process cannot obtain identification information about 
                //the client, and it cannot impersonate the client.
                TokenImpersonationLevel.Anonymous);

            pipeClient.Connect(60000); // set TimeOut for connection
            pipeClient.ReadMode = PipeTransmissionMode.Message;

            Console.WriteLine(@"The named pipe, \\{0}\{1}, is connected.",
                strServerName, strPipeName);


            /////////////////////////////////////////////////////////////////
            // Send a message to the pipe server and receive its response.
            //                

            // A byte buffer of BUFFER_SIZE bytes. The buffer should be big 
            // enough for ONE request to the client

            string strMessage;
            byte[] bRequest;                        // Client -> Server
            int cbRequestBytes;
            byte[] bReply = new byte[BUFFER_SIZE];  // Server -> Client
            int cbBytesRead, cbReplyBytes;

            // Send one message to the pipe.

            // '\0' is appended in the end because the client may be a native
            // C++ program.
            strMessage = "Default request from client\0";
            bRequest = Encoding.Unicode.GetBytes(strMessage);
            cbRequestBytes = bRequest.Length;
            if (pipeClient.CanWrite)
            {
                pipeClient.Write(bRequest, 0, cbRequestBytes);
            }
            pipeClient.Flush();

            Console.WriteLine("Sends {0} bytes; Message: \"{1}\"",
                cbRequestBytes, strMessage.TrimEnd('\0'));

            // Receive one message from the pipe.

            cbReplyBytes = BUFFER_SIZE;
            do
            {
                if (pipeClient.CanRead)
                {
                    cbBytesRead = pipeClient.Read(bReply, 0, cbReplyBytes);

                    // Unicode-encode the byte array and trim all the '\0' chars 
                    // at the end.
                    strMessage = Encoding.Unicode.GetString(bReply).TrimEnd('\0');
                    Console.WriteLine("Receives {0} bytes; Message: \"{1}\"",
                        cbBytesRead, strMessage);
                }
            }
            while (!pipeClient.IsMessageComplete);

        }
        catch (TimeoutException ex)
        {
            Console.WriteLine("Unable to open named pipe {0}\\{1}",
               strServerName, strPipeName);
            Console.WriteLine(ex.Message);
        }
        catch (Exception ex)
        {
            Console.WriteLine("The client throws the error: {0}", ex.Message);
        }
        finally
        {
            /////////////////////////////////////////////////////////////////
            // Close the pipe.
            // 

            if (pipeClient != null)
                pipeClient.Close();
        }
    }
}