/****************************** Module Header ******************************\
* Module Name:	CSMailslotClient.cs
* Project:		CSMailslotClient
* Copyright (c) Microsoft Corporation.
* 
* Mailslot is a mechanism for one-way inter-process communication in the 
* local machine or across the computers in the intranet. Any clients can  
* store messages in a mailslot. The creator of the slot, i.e. the server,  
* retrieves the messages that are stored there:
* 
* Client (GENERIC_WRITE) ---> Server (GENERIC_READ)
* 
* This sample demonstrates a mailslot client that connects and writes to the 
* mailslot \\.\mailslot\HelloWorld. 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 1:30 PM Riquel Dong Created
* * 3/12/2009 11:16 AM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
using System.Security;
#endregion


class Program
{
    static void Main(string[] args)
    {
        /////////////////////////////////////////////////////////////////////
        // Open the mailslot.
        // 

        // Prepare the slot name
        String strMailslotName = String.Format(@"\\{0}\mailslot\{1}",
            ".",                // Server name
            "HelloWorld"        // Pipe name
            );

        IntPtr hMailslot = MailslotNative.CreateFile(
            strMailslotName,                    // Mailslot name
            FileDesiredAccess.GENERIC_WRITE,    // Write access 
            FileShareMode.FILE_SHARE_READ,      // Share mode
            IntPtr.Zero,                        // Default security attributes
            FileCreationDisposition.OPEN_EXISTING, // Opens existing mailslot 
            0,                                  // No other attributes set
            IntPtr.Zero                         // No template file
            );

        if (hMailslot.ToInt32() == MailslotNative.INVALID_HANDLE_VALUE)
        {
            Console.WriteLine("CreateFile failed w/err 0x{0:X}",
                Marshal.GetLastWin32Error());
            return;
        }


        /////////////////////////////////////////////////////////////////////
        // Write messages to the mailslot.
        // 

        // Append '\0' at the end of each message considering the native C++ 
        // Mailslot server (CppMailslotServer).
        WriteMailslot(hMailslot, "Message 1 for mailslot\0");
        WriteMailslot(hMailslot, "Message 2 for mailslot\0");

        Thread.Sleep(3000); // Sleep 3 seconds

        WriteMailslot(hMailslot, "Message 3 for mailslot\0");


        /////////////////////////////////////////////////////////////////////
        // Close the slot.
        // 

        MailslotNative.CloseHandle(hMailslot);

        return;
    }

    /// <summary>
    ///  Write a message to the specified mailslot
    /// </summary>
    /// <param name="hMailslot">Handle to the mailslot</param>
    /// <param name="lpszMessage">The message to be written to the slot
    /// </param>
    /// <returns>If the function succeeds, the return value is true
    /// </returns>
    static bool WriteMailslot(IntPtr hMailslot, String strMessage)
    {
        bool bResult;
        int cbBytesWritten;     // Number of bytes written to the slot

        byte[] bMessage = Encoding.Unicode.GetBytes(strMessage);

        bResult = MailslotNative.WriteFile(
            hMailslot,          // Handle to the mailslot
            bMessage,           // Message to be written
            bMessage.Length,    // Number of bytes to write
            out cbBytesWritten, // Number of bytes written
            IntPtr.Zero);       // Not overlapped 

        if (!bResult || bMessage.Length != cbBytesWritten)
        {
            Console.WriteLine("WriteFile failed w/err 0x{0:X}\n",
                Marshal.GetLastWin32Error());
            return false;
        }

        Console.WriteLine("The message, {0}, is written to the slot.", 
            strMessage);

        return true;
    }
}