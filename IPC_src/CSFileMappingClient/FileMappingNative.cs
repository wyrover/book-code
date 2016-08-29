/****************************** Module Header ******************************\
* Module Name:	FileMappingNative.cs
* Project:		CSFileMappingClient
* Copyright (c) Microsoft Corporation.
* 
* The P/Invoke signatures for native File Mapping APIs
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 19:24 PM Hongye Sun Created
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
#endregion


/// <summary>
/// Access rights for file mapping objects
/// http://msdn.microsoft.com/en-us/library/aa366559.aspx
/// </summary>
[Flags]
public enum FileMapAccess
{
    FILE_MAP_COPY = 0x0001,
    FILE_MAP_WRITE = 0x0002,
    FILE_MAP_READ = 0x0004,
    FILE_MAP_ALL_ACCESS = 0x000F001F
}

public class FileMappingNative
{
    public const int INVALID_HANDLE_VALUE = -1;

    /// <summary>
    /// Maps a view of a file mapping into the address space of a calling
    /// process.
    /// </summary>
    /// <param name="hFileMappingObject">
    /// A handle to a file mapping object. The CreateFileMapping and 
    /// OpenFileMapping functions return this handle.
    /// </param>
    /// <param name="dwDesiredAccess">
    /// The type of access to a file mapping object, which determines the 
    /// protection of the pages.
    /// </param>
    /// <param name="dwFileOffsetHigh">
    /// A high-order DWORD of the file offset where the view begins.
    /// </param>
    /// <param name="dwFileOffsetLow">
    /// A low-order DWORD of the file offset where the view is to begin.
    /// </param>
    /// <param name="dwNumberOfBytesToMap">
    /// The number of bytes of a file mapping to map to the view. All bytes 
    /// must be within the maximum size specified by CreateFileMapping.
    /// </param>
    /// <returns>
    /// If the function succeeds, the return value is the starting address 
    /// of the mapped view.
    /// </returns>
    [DllImport("Kernel32.dll", SetLastError = true)]
    public static extern IntPtr MapViewOfFile(
        IntPtr hFileMappingObject,      // Handle to the file mapping object
        FileMapAccess dwDesiredAccess,  // Desired access to file mapping object
        uint dwFileOffsetHigh,          // High-order DWORD of file offset
        uint dwFileOffsetLow,           // Low-order DWORD of file offset
        uint dwNumberOfBytesToMap       // Number of bytes map to the view
        );

    /// <summary>
    /// Opens a named file mapping object.
    /// </summary>
    /// <param name="dwDesiredAccess">
    /// The access to the file mapping object. This access is checked against 
    /// any security descriptor on the target file mapping object.
    /// </param>
    /// <param name="bInheritHandle">
    /// If this parameter is TRUE, a process created by the CreateProcess 
    /// function can inherit the handle; otherwise, the handle cannot be 
    /// inherited.
    /// </param>
    /// <param name="lpName">
    /// The name of the file mapping object to be opened.
    /// </param>
    /// <returns>
    /// If the function succeeds, the return value is an open handle to the 
    /// specified file mapping object.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr OpenFileMapping(
      FileMapAccess dwDesiredAccess,    // Access mode
      bool bInheritHandle,              // Inherit flag
      string lpName                     // File mapping object name
      );

    /// <summary>
    /// Unmaps a mapped view of a file from the calling process's address space.
    /// </summary>
    /// <param name="lpBaseAddress">
    /// A pointer to the base address of the mapped view of a file that is to
    /// be unmapped.
    /// </param>
    /// <returns></returns>
    [DllImport("Kernel32.dll", SetLastError = true)]
    public static extern bool UnmapViewOfFile(
        IntPtr lpBaseAddress             // Base address of mapped view
        );

    /// <summary>
    /// Closes an open object handle.
    /// </summary>
    /// <param name="hHandle">Handle to an open object.</param>
    /// <returns>
    /// If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool CloseHandle(IntPtr hHandle);

    /// <summary>
    /// Retrieves the calling thread's last-error code value.
    /// </summary>
    /// <returns>
    /// The return value is the calling thread's last-error code value.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern uint GetLastError();
}

