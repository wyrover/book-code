/****************************** Module Header ******************************\
* Module Name:	FileMappingNative.cs
* Project:		CSFileMappingServer
* Copyright (c) Microsoft Corporation.
* 
* The P/Invoke signatures for native File Mapping APIs
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 7:32 PM Hongye Sun Created
* * 3/15/2009 10:20 PM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
#endregion


/// <summary>
/// Memory Protection Constants
/// http://msdn.microsoft.com/en-us/library/aa366786.aspx
/// </summary>
[Flags]
public enum FileProtection : uint
{
    PAGE_NOACCESS = 0x01,
    PAGE_READONLY = 0x02,
    PAGE_READWRITE = 0x04,
    PAGE_WRITECOPY = 0x08,
    PAGE_EXECUTE = 0x10,
    PAGE_EXECUTE_READ = 0x20,
    PAGE_EXECUTE_READWRITE = 0x40,
    PAGE_EXECUTE_WRITECOPY = 0x80,
    PAGE_GUARD = 0x100,
    PAGE_NOCACHE = 0x200,
    PAGE_WRITECOMBINE = 0x400,
    SEC_FILE = 0x800000,
    SEC_IMAGE = 0x1000000,
    SEC_RESERVE = 0x4000000,
    SEC_COMMIT = 0x8000000,
    SEC_NOCACHE = 0x10000000
}

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
    /// Creates or opens a named or unnamed file mapping object for a 
    /// specified file.
    /// </summary>
    /// <param name="hFile">
    /// A handle to the file from which to create a file mapping object.
    /// </param>
    /// <param name="lpAttributes">
    /// A pointer to a SECURITY_ATTRIBUTES structure that determines whether 
    /// a returned handle can be inherited by child processes.
    /// </param>
    /// <param name="flProtect">
    /// Specifies the page protection of the file mapping object. All mapped 
    /// views of the object must be compatible with this protection.
    /// </param>
    /// <param name="dwMaximumSizeHigh">
    /// The high-order DWORD of the maximum size of the file mapping object.
    /// </param>
    /// <param name="dwMaximumSizeLow">
    /// The low-order DWORD of the maximum size of the file mapping object.
    /// </param>
    /// <param name="lpName">
    /// The name of the file mapping object.
    /// </param>
    /// <returns>
    /// If the function succeeds, the return value is a handle to the newly 
    /// created file mapping object.
    /// </returns>
    [DllImport("Kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateFileMapping(
        IntPtr hFile,                   // Handle to the file
        IntPtr lpAttributes,            // Security Attributes
        FileProtection flProtect,       // File protection
        uint dwMaximumSizeHigh,         // High-order DWORD of size
        uint dwMaximumSizeLow,          // Low-order DWORD of size
        string lpName                   // File mapping object name
        );

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

