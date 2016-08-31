/****************************** Module Header ******************************\
* Module Name:	Program.cs
* Project:		CSFileMappingClient
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
* This sample demonstrates a named shared memory client, Local\HelloWorld, 
* that can access the string written to the shared memory by the first 
* process. The process display this string as it would any other string. In 
* this example, the message box displayed contains the message 
* "Message from first process" that was written by the first process.
*
* .NET only supports P/Invoke native APIs currently. By P/Invoke, .NET codes  
* can simulate the behaviors of native codes.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 7:29 PM Hongye Sun Created
* * 3/15/2009 10:22 PM Jialiang Ge Reviewed
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
        // Try to open a named file mapping.
        //

        string strMapFileName = "Local\\HelloWorld";

        // Open the named file mapping.
        IntPtr hMapFile = FileMappingNative.OpenFileMapping(
            FileMapAccess.FILE_MAP_ALL_ACCESS,
            false,
            strMapFileName);

        if (hMapFile == IntPtr.Zero)
        {
            Console.WriteLine("Unable to open file mapping object w/err 0x{0:X}",
                FileMappingNative.GetLastError());
            return;
        }
        Console.WriteLine("The file mapping object, {0}, is opened.", 
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
            FileMappingNative.CloseHandle(hMapFile);
            return;
        }
        Console.WriteLine("The file view is created.");


        /////////////////////////////////////////////////////////////////////
        // Show the message in shared memory.
        // 

        // Copy all characters up to the first null char to strMessage
        string strMessage = Marshal.PtrToStringUni(pBuf);
        Console.WriteLine("The following message is read from the shared memory:");
        Console.WriteLine("\"{0}\"", strMessage);


        /////////////////////////////////////////////////////////////////////
        // Unmap the file view and close the file mapping objects.
        // 

        FileMappingNative.UnmapViewOfFile(pBuf);
        FileMappingNative.CloseHandle(hMapFile);
    }
}