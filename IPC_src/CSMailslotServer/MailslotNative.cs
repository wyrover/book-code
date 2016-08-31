/****************************** Module Header ******************************\
* Module Name:	MailSlotNative.cs
* Project:		CSMailSlotServer
* Copyright (c) Microsoft Corporation.
* 
* The P/Invoke signatures of some native Mailslot APIs.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 9:15 AM Riquel Dong Created
* * 3/12/2009 11:16 AM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Security;
#endregion


#region Mailslot

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
/// The class exposes kernel32.dll methods for MailSlot communication.
/// </summary>
[SuppressUnmanagedCodeSecurity]
public class MailslotNative
{
    /// <summary>
    /// mailslot waits forever for a message 
    /// </summary>
    public const int MAILSLOT_WAIT_FOREVER = -1;

    /// <summary>
    /// Invalid operating system handle.
    /// </summary>
    public const int INVALID_HANDLE_VALUE = -1;

    /// <summary>
    /// There is no next message
    /// </summary>
    public const int MAILSLOT_NO_MESSAGE = -1;

    /// <summary>
    /// Reads data from a file, and starts at the position that the file 
    /// pointer indicates
    /// </summary>
    /// <param name="hFile">A handle to the file to be read</param>
    /// <param name="lpBuffer">A buffer that receives the data read from a 
    /// file</param>
    /// <param name="nNumberOfBytesToRead">The maximum number of bytes to be 
    /// read</param>
    /// <param name="lpNumberOfBytesRead">the number of bytes read</param>
    /// <param name="lpOverlapped">A pointer to an OVERLAPPED structure.
    /// </param>
    /// <returns>The ReadFile functiona returns when one of the following 
    /// conditions is met: A write operation completes on the write end of
    /// the pipe; The number of bytes requested is read; An error occurs.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool ReadFile(
        IntPtr hFile,                   // Handle to file
        byte[] lpBuffer,                // Data buffer
        int nNumberOfBytesToRead,       // Number of bytes to read
        out int lpNumberOfBytesRead,    // Number of bytes read
        IntPtr lpOverlapped             // Overlapped buffer
        );

    /// <summary>
    /// Writes data to the specified file at the position specified by
    /// the file pointer. 
    /// </summary>
    /// <param name="handle">A handle to the file</param>
    /// <param name="lpBuffer">A buffer containing the data to be written to 
    /// the file.</param>
    /// <param name="nNumberOfBytesToWrite">The number of bytes to be written 
    /// to the file.</param>
    /// <param name="lpNumberOfBytesWritten">the number of bytes written.
    /// </param>
    /// <param name="lpOverlapped">A pointer to an OVERLAPPED structure.
    /// </param> 
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool WriteFile(
       IntPtr handle,                  // Handle to file
       byte[] lpBuffer,                // Data buffer
       int nNumberOfBytesToWrite,      // Number of bytes will be written
       out int lpNumberOfBytesWritten, // Number of bytes written            
       IntPtr lpOverlapped             // Overlapped buffer
       );

    /// <summary>
    /// Closes an open object handle
    /// </summary>
    /// <param name="hObject">A valid handle to an open object</param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// </returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static extern bool CloseHandle(
        IntPtr hObject                 //// handle to kernel object
        );

    /// <summary>
    /// Retrieves information about the specified mailslot
    /// </summary>
    /// <param name="hMailslot">A handle to a mailslot</param>
    /// <param name="lpMaxMessageSize">The maximum message size, in bytes,
    /// allowed for this mailslot</param>
    /// <param name="lpNextSize">The size of the next message in bytes
    /// </param>
    /// <param name="lpMessageCount">The total number of messages waiting 
    /// to be read</param>
    /// <param name="lpReadTimeout">The amount of time, in milliseconds, 
    /// a read operation can wait for a message to be written to the mailslot 
    /// before a time-out occurs. </param>
    /// <returns></returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool GetMailslotInfo(
        IntPtr hMailslot,           // Handle to mailslot
        IntPtr lpMaxMessageSize,    // Max message size in bytes
        out int lpNextSize,         // Size of next message in bytes.
        out int lpMessageCount,     // Number of messages 
        IntPtr lpReadTimeout        // Timeout of operation
        );

    /// <summary>
    /// Creates an instance of a mailslot and returns a handle for subsequent 
    /// operations.
    /// </summary>
    /// <param name="lpName">mailslot name</param>
    /// <param name="nMaxMessageSize">The maximum size of a single message
    /// </param>
    /// <param name="lReadTimeout">The time a read operation can wait for a 
    /// message</param>
    /// <param name="lpSecurityAttributes">Security attributes</param>
    /// <returns>If the function succeeds, the return value is a handle to 
    /// the server end of a mailslot instance.</returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateMailslot(
        string lpName,              // Mailslot name
        uint nMaxMessageSize,       // Max size of a single message in bytes
        int lReadTimeout,           // Timeout of a read operation
        IntPtr lpSecurityAttributes // Security attributes
        );

    /// <summary>
    /// open an instance of a mailslot and returns a handle for subsequent 
    /// operations.
    /// </summary>
    /// <param name="lpFileName">mailslot name</param>
    /// <param name="dwDesiredAccess">The access to the object</param>
    /// <param name="dwShareMode">The sharing mode of an object</param>
    /// <param name="SecurityAttributes">Security attributes</param>
    /// <param name="dwCreationDisposition">An action to take on files that
    /// exist and do not exist</param>
    /// <param name="dwFlagsAndAttributes">The file attributes and flags
    /// </param>
    /// <param name="hTemplateFile">A handle to a template file with the 
    /// GENERIC_READ access right</param>
    /// <returns>If the function succeeds, the return value is a handle to 
    /// the server end of a mailslot instance</returns>
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern IntPtr CreateFile(
        string lpFileName,                  // File name
        FileDesiredAccess dwDesiredAccess,  // Access mode
        FileShareMode dwShareMode,          // Share mode
        IntPtr SecurityAttributes,          // Security attributes
        FileCreationDisposition dwCreationDisposition,// How to create
        uint dwFlagsAndAttributes,          // File attributes
        IntPtr hTemplateFile                // Handle to template file
        );

    /// <summary>
    /// Sets the time-out value used by the specified mailslot for 
    /// a read operation</summary>
    /// <param name="hMailslot">A handle to a mailslot</param>
    /// <param name="lReadTimeout">The time a read operation can wait 
    /// for a message to be written to the mailslot before a time-out
    /// occurs, in milliseconds</param>
    /// <returns>If the function succeeds, the return value is nonzero.
    /// If the function fails, the return value is zero</returns>
    [DllImport("kernel32.dll")]
    static extern bool SetMailslotInfo(
        IntPtr hMailslot,           // Mailslot handle
        uint lReadTimeout           // Timeout of a read operation
        );
}

#endregion


#region Security

/// <summary>
/// Security Descriptor structure
/// </summary>
[StructLayoutAttribute(LayoutKind.Sequential)]
public struct SECURITY_DESCRIPTOR
{
    public byte revision;
    public byte size;
    public short control;
    public IntPtr owner;
    public IntPtr group;
    public IntPtr sacl;
    public IntPtr dacl;
}

/// <summary>
/// Security Attributes structure.
/// </summary>
[StructLayout(LayoutKind.Sequential)]
public struct SECURITY_ATTRIBUTES
{
    public int nLength;
    public IntPtr lpSecurityDescriptor;
    public bool bInheritHandle;
}

/// <summary>
/// 
/// </summary>
[SuppressUnmanagedCodeSecurity]
public class SecurityNative
{
    /// <summary>
    /// Sets the security descriptor attributes.
    /// </summary>
    /// <param name="sd">Reference to a SECURITY_DESCRIPTOR structure.
    /// </param>
    /// <param name="bDaclPresent">A flag that indicates the presence of 
    /// a DACL in the security descriptor.</param>
    /// <param name="Dacl">A pointer to an ACL structure that specifies
    /// the DACL for the security descriptor.</param>
    /// <param name="bDaclDefaulted">A flag that indicates the source 
    /// of the DACL.</param>
    /// <returns>If the function succeeds, the function returns nonzero.
    /// </returns>
    [DllImport("Advapi32.dll", SetLastError = true)]
    public static extern bool SetSecurityDescriptorDacl(
        ref SECURITY_DESCRIPTOR sd,         // A pointer to the SECURITY_DESCRIPTOR struct
        bool bDaclPresent,
        IntPtr Dacl,                        // A pointer to an ACL struct
        bool bDaclDefaulted                 // The source of the DACL
        );

    /// <summary>
    /// Initializes a SECURITY_DESCRIPTOR structure.
    /// </summary>
    /// <param name="sd"></param>
    /// <param name="dwRevision"></param>
    /// <returns></returns>
    [DllImport("Advapi32.dll", SetLastError = true)]
    public static extern bool InitializeSecurityDescriptor(
        out SECURITY_DESCRIPTOR sd,
        int dwRevision
        );

} // class SecurityNative

#endregion
