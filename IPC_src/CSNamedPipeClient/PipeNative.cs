/****************************** Module Header ******************************\
* Module Name:	PipeNative.cs
* Project:		CSNamedPipeClient
* Copyright (c) Microsoft Corporation.
* 
* The P/Invoke signatures of some native Named Pipe APIs.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 1/27/2009 9:21 PM Jialiang Ge Created
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Security;
#endregion


#region Named Pipe

/// <summary>
/// Desired Access of File/Device
/// </summary>
[Flags]
public enum FileDesiredAccess : uint
{
    GENERIC_READ = 0x80000000,
    GENERIC_WRITE = 0x40000000,
    GENERIC_EXECUTE = 0x20000000,
    GENERIC_ALL = 0x10000000
}

/// <summary>
/// File share mode
/// </summary>
[Flags]
public enum FileShareMode : uint
{
    Zero = 0x00000000,                  // No sharing.
    FILE_SHARE_DELETE = 0x00000004,
    FILE_SHARE_READ = 0x00000001,
    FILE_SHARE_WRITE = 0x00000002
}

/// <summary>
/// File Creation Disposition
/// </summary>
[Flags]
public enum FileCreationDisposition : uint
{
    CREATE_NEW = 1,
    CREATE_ALWAYS = 2,
    OPEN_EXISTING = 3,
    OPEN_ALWAYS = 4,
    TRUNCATE_EXISTING = 5
}

/// <summary>
/// Named Pipe Open Modes
/// http://msdn.microsoft.com/en-us/library/aa365596.aspx
/// </summary>
[Flags]
public enum PipeOpenMode : uint
{
    PIPE_ACCESS_INBOUND = 0x00000001,   // Inbound pipe access.
    PIPE_ACCESS_OUTBOUND = 0x00000002,  // Outbound pipe access.
    PIPE_ACCESS_DUPLEX = 0x00000003     // Duplex pipe access.
}

/// <summary>
/// Named Pipe Type, Read, and Wait Modes
/// http://msdn.microsoft.com/en-us/library/aa365605.aspx
/// </summary>
public enum PipeMode : uint
{
    // Type Mode
    PIPE_TYPE_BYTE = 0x00000000,        // Byte pipe type.
    PIPE_TYPE_MESSAGE = 0x00000004,     // Message pipe type.

    // Read Mode
    PIPE_READMODE_BYTE = 0x00000000,    // Read mode of type Byte.
    PIPE_READMODE_MESSAGE = 0x00000002, // Read mode of type Message.

    // Wait Mode
    PIPE_WAIT = 0x00000000,             // Pipe blocking mode.
    PIPE_NOWAIT = 0x00000001            // Pipe non-blocking mode.
}

/// <summary>
/// The class exposes kernel32.dll methods for named pipes communication.
/// </summary>
[SuppressUnmanagedCodeSecurity]
public class PipeNative
{
    /// <summary>
    /// Unlimited server pipe instances.
    /// </summary>
    public const uint PIPE_UNLIMITED_INSTANCES = 255;


    /// <summary>
    /// The operation completed successfully.
    /// </summary>
    public const ulong ERROR_SUCCESS = 0;

    /// <summary>
    /// The system cannot find the file specified.
    /// </summary>
    public const ulong ERROR_CANNOT_CONNECT_TO_PIPE = 2;

    /// <summary>
    /// All pipe instances are busy.
    /// </summary>
    public const ulong ERROR_PIPE_BUSY = 231;

    /// <summary>
    /// The pipe is being closed.
    /// </summary>
    public const ulong ERROR_NO_DATA = 232;

    /// <summary>
    /// No process is on the other end of the pipe.
    /// </summary>
    public const ulong ERROR_PIPE_NOT_CONNECTED = 233;

    /// <summary>
    /// More data is available.
    /// </summary>
    public const ulong ERROR_MORE_DATA = 234;

    /// <summary>
    /// There is a process on other end of the pipe.
    /// </summary>
    public const ulong ERROR_PIPE_CONNECTED = 535;

    /// <summary>
    /// Waiting for a process to open the other end of the pipe.
    /// </summary>
    public const ulong ERROR_PIPE_LISTENING = 536;


    /// <summary>
    /// Waits indefinitely when connecting to a pipe.
    /// </summary>
    public const uint NMPWAIT_WAIT_FOREVER = 0xffffffff;

    /// <summary>
    /// Does not wait for the named pipe.
    /// </summary>
    public const uint NMPWAIT_NOWAIT = 0x00000001;

    /// <summary>
    /// Uses the default time-out specified in a call to the 
    /// CreateNamedPipe method.
    /// </summary>
    public const uint NMPWAIT_USE_DEFAULT_WAIT = 0x00000000;

    /// <summary>
    /// Invalid operating system handle.
    /// </summary>
    public const int INVALID_HANDLE_VALUE = -1;


    /// <summary>
    /// Retrieves the calling thread's last-error code value.
    /// </summary>
    /// <returns>The return value is the calling thread's last-error 
    /// code value.</returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern uint GetLastError();

    /// <summary>
    /// Creates an instance of a named pipe and returns a handle for 
    /// subsequent pipe operations.
    /// </summary>
    /// <param name="lpName">Pipe name</param>
    /// <param name="dwOpenMode">Pipe open mode</param>
    /// <param name="dwPipeMode">Pipe-specific modes</param>
    /// <param name="nMaxInstances">Maximum number of instances</param>
    /// <param name="nOutBufferSize">Output buffer size</param>
    /// <param name="nInBufferSize">Input buffer size</param>
    /// <param name="nDefaultTimeOut">Time-out interval</param>
    /// <param name="pipeSecurityAttributes">Security attributes</param>
    /// <returns>If the function succeeds, the return value is a handle 
    /// to the server end of a named pipe instance.</returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateNamedPipe(
        String lpName,                      // Pipe name
        PipeOpenMode dwOpenMode,            // Pipe open mode
        PipeMode dwPipeMode,                // Pipe-specific modes
        uint nMaxInstances,                 // Maximum number of instances
        uint nOutBufferSize,                // Output buffer size
        uint nInBufferSize,                 // Input buffer size
        uint nDefaultTimeOut,               // Time-out interval
        IntPtr pipeSecurityAttributes       // Security Attributes
        );

    /// <summary>
    /// Enables a named pipe server process to wait for a client 
    /// process to connect to an instance of a named pipe.
    /// </summary>
    /// <param name="hHandle">Handle to the server end of a named pipe 
    /// instance.</param>
    /// <param name="lpOverlapped">Pointer to an Overlapped object.
    /// </param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool ConnectNamedPipe(
        IntPtr hHandle,                     // Handle to named pipe
        IntPtr lpOverlapped                 // Overlapped structure
        );

    /// <summary>
    /// Waits until either a time-out interval elapses or an instance 
    /// of the specified named pipe is available for connection.
    /// </summary>
    /// <param name="name">Pointer to a null-terminated string that  
    /// specifies the name of the named pipe.</param>
    /// <param name="timeout">Number of milliseconds that the function  
    /// willwait for an instance of the named pipe to be available.
    /// </param>
    /// <returns>If an instance of the pipe is available before the 
    /// time-out interval elapses, the return value is nonzero.</param>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool WaitNamedPipe(
        String name,
        int timeout);

    /// <summary>
    /// Sets the read mode and the blocking mode of the specified named pipe.
    /// </summary>
    /// <remarks>
    /// If the specified handle is to the client end of a named pipe and if
    /// the named pipe server process is on a remote computer, the function
    /// can also be used to control local buffering.
    /// </remarks>
    /// <param name="hHandle">Handle to the named pipe instance.</param>
    /// <param name="lpMode">Pointer to a variable that supplies the new mode.
    /// </param>
    /// <param name="lpMaxCollectionCount">Pointer to a variable that 
    /// specifies the maximum number of bytes collected on the client computer
    /// before transmission to the server.</param>
    /// <param name="lpCollectDataTimeout">Pointer to a variable that 
    /// specifies the maximum time, in milliseconds, that can pass before
    /// a remote named pipe transfers information over the network.</param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool SetNamedPipeHandleState(
        IntPtr hHandle,
        ref PipeMode lpMode,
        IntPtr lpMaxCollectionCount,
        IntPtr lpCollectDataTimeout);

    /// <summary>
    /// Creates or opens a file, directory, physical disk, volume, console
    /// buffer, tape drive, communications resource, mailslot, or named 
    /// pipe.
    /// </summary>
    /// <param name="lpFileName">File name.</param>
    /// <param name="dwDesiredAccess">Access to the object (reading, 
    /// writing, or both).</param>
    /// <param name="dwShareMode">Sharing mode of the object (reading, 
    /// writing, both, or neither).</param>
    /// <param name="fileSecurityAttributes">Pointer to a 
    /// SecurityAttributes object that determines whether the returned 
    /// handle can be inherited by child processes.</param>
    /// <param name="dwCreationDisposition">Action to take on files that 
    /// exist, and which action to take when files do not exist</param>
    /// <param name="dwFlagsAndAttributes">File attributes and flags.
    /// </param>
    /// <param name="hTemplateFile">Handle to a template file.</param>
    /// <returns>If the function succeeds, the return value is an open 
    /// handle to the specified file.</returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateFile(
        String lpFileName,                  // File name
        FileDesiredAccess dwDesiredAccess,  // Access mode
        FileShareMode dwShareMode,          // Share mode
        IntPtr fileSecurityAttributes,      // Security Attributes
        FileCreationDisposition dwCreationDisposition,// How to create
        uint dwFlagsAndAttributes,          // File attributes
        uint hTemplateFile);                // Handle to template file

    /// <summary>
    /// Reads data from a file, starting at the position indicated by the 
    /// file pointer.
    /// </summary>
    /// <param name="hHandle">Handle to the file to be read.</param>
    /// <param name="lpBuffer">Pointer to the buffer that receives the 
    /// data read from the file.</param>
    /// <param name="nNumberOfBytesToRead">Number of bytes to be read from
    /// the file.</param>
    /// <param name="lpNumberOfBytesRead">Pointer to the variable that 
    /// receives the number of bytes read.</param>
    /// <param name="lpOverlapped">Pointer to an Overlapped object.</param> 
    /// <returns>The ReadFile function returns when one of the following 
    /// conditions is met: a write operation completes on the write end 
    /// of the pipe, the number of bytes requested has been read, or an
    /// error occurs.</returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool ReadFile(
        IntPtr hHandle,                     // Handle to file
        byte[] lpBuffer,                    // Data buffer
        uint nNumberOfBytesToRead,          // Number of bytes to read
        out uint lpNumberOfBytesRead,       // Number of bytes read
        IntPtr lpOverlapped                 // Overlapped buffer
        );

    /// <summary>
    /// Writes data to a file at the position specified by the file pointer.
    /// </summary>
    /// <param name="hHandle">Handle to the file.</param>
    /// <param name="lpBuffer">Pointer to the buffer containing the data to 
    /// be written to the file.</param>
    /// <param name="nNumberOfBytesToWrite">Number of bytes to be write to 
    /// the file.</param>
    /// <param name="lpNumberOfBytesWritten">Pointer to the variable that
    /// receives the number of bytes written.</param>
    /// <param name="lpOverlapped">Pointer to an Overlapped object.
    /// </param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool WriteFile(
        IntPtr hHandle,                     // Handle to file
        byte[] lpBuffer,                    // Data buffer
        uint nNumberOfBytesToWrite,         // Number of bytes to write
        out uint lpNumberOfBytesWritten,    // number of bytes written
        IntPtr lpOverlapped                 // Overlapped buffer
        );

    /// <summary>
    /// Closes an open object handle.
    /// </summary>
    /// <param name="hHandle">Handle to an open object.</param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool CloseHandle(IntPtr hHandle);

    /// <summary>
    /// Flushes the buffers of the specified file and causes all buffered
    /// data to be written to the file.
    /// </summary>
    /// <param name="hHandle">Handle to an open file.</param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool FlushFileBuffers(IntPtr hHandle);

    /// <summary>
    /// Disconnects the server end of a named pipe instance from a client
    /// process.
    /// </summary>
    /// <param name="hHandle">Handle to an instance of a named pipe.
    /// </param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool DisconnectNamedPipe(IntPtr hHandle);

} // class PipeNative

#endregion


