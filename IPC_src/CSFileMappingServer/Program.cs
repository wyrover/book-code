/****************************** Module Header ******************************\
* Module Name:	Program.cs
* Project:		CSFileMappingServer
* Copyright (c) Microsoft Corporation.
* 
* File mapping is a mechanism for one-way or bi-directional inter-process 
* communication among two or more processes in the local machine. To share a 
* file or memory, all of the processes must use the name or the handle of the 
* same file mapping object.
* 
* To share a file, the first process creates or opens a file by using the 
* CreateFile function. Next, it creates a file mapping object by using the 
* CreateFileMapping function, specifying the file handle and a name for the 
* file mapping object. The names of event, semaphore, mutex, waitable timer, 
* job, and file mapping objects share the same name space. Therefore, the 
* CreateFileMapping and OpenFileMapping functions fail if they specify a name
* that is in use by an object of another type.
* 
* To share memory that is not associated with a file, a process must use the 
* CreateFileMapping function and specify INVALID_HANDLE_VALUE as the hFile 
* parameter instead of an existing file handle. The corresponding file  
* mapping object accesses memory backed by the system paging file. You must  
* specify a size greater than zero when you specify an hFile of  
* INVALID_HANDLE_VALUE in a call to CreateFileMapping.
* 
* Processes that share files or memory must create file views by using the 
* MapViewOfFile or MapViewOfFileEx function. They must coordinate their  
* access using semaphores, mutexes, events, or some other mutual exclusion 
* technique.
* 
* This example demonstrates a named shared memory server, Local\HelloWorld, 
* which creates the file mapping object with INVALID_HANDLE_VALUE. By using 
* the PAGE_READWRITE flag, the process has read/write permission to the 
* memory through any file views that are created.
*
* .NET only supports P/Invoke native APIs currently. By P/Invoke, .NET codes 
* can simulate the behaviors of native codes.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 7:32 PM Hongye Sun Created
* * 3/15/2009 8:58 PM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Text;
using System.Runtime.InteropServices;
#endregion


class Program
{
    const uint BUFFER_SIZE = 256;

    static void Main(string[] args)
    {
        /////////////////////////////////////////////////////////////////////
        // Create a file mapping.
        // 

        string strMapFileName = "Local\\HelloWorld";

        // Create the file mapping object
        IntPtr hMapFile = FileMappingNative.CreateFileMapping(
            (IntPtr)FileMappingNative.INVALID_HANDLE_VALUE,
            IntPtr.Zero,
            FileProtection.PAGE_READWRITE,
            0,
            BUFFER_SIZE,
            strMapFileName);

        if (hMapFile == IntPtr.Zero)
        {
            Console.WriteLine("Unable to create file mapping object w/err 0x{0:X}", 
                FileMappingNative.GetLastError());
            return;
        }
        Console.WriteLine("The file mapping object, {0}, is created.", 
            strMapFileName);


        /////////////////////////////////////////////////////////////////////
        // Maps the view of the file mapping into the address space of the 
        // current process.
        // 

        // Create file view from the file mapping object.
        IntPtr pBuf = FileMappingNative.MapViewOfFile(
            hMapFile,
            FileMapAccess.FILE_MAP_ALL_ACCESS,
            0,
            0,
            BUFFER_SIZE);

        if (pBuf == IntPtr.Zero)
        {
            Console.WriteLine("Unable to map view of file w/err 0x{0:X}", 
                FileMappingNative.GetLastError());
            return;
        }
        Console.WriteLine("The file view is created.");


        /////////////////////////////////////////////////////////////////////
        // Write message to the file view.
        // 

        // Append '\0' at the end of the message considering the native C++ 
        // FileMapping client (CppFileMappingClient).
        string strMessage = "Message from first process.\0";
        byte[] bMessage = Encoding.Unicode.GetBytes(strMessage);

        // Write message to the file view.
        Marshal.Copy(bMessage, 0, pBuf, bMessage.Length);

        Console.WriteLine("The following message is written to the shared memory:");
        Console.WriteLine("\"{0}\"", strMessage);

        // Wait to stop the server.
        Console.WriteLine("Press any key to stop the server.");
        Console.Read();


        /////////////////////////////////////////////////////////////////////
        // Unmap the file view and close the file mapping objects.
        // 

        FileMappingNative.UnmapViewOfFile(pBuf);
        FileMappingNative.CloseHandle(hMapFile);
    }
}
