/****************************** Module Header ******************************\
* Module Name:	CSMailslotServer.cs
* Project:		CSMailslotServer
* Copyright (c) Microsoft Corporation.
* 
* Mailslot is a mechanism for one-way inter-process communication in the 
* local machine or across the computers in the intranet. Any clients can  
* store messages in a mailslot. The creator of the slot, i.e. the server,  
* retrieves the messages that are stored there:
* 
* Client (GENERIC_WRITE) ---> Server (GENERIC_READ)
* 
* This sample demonstrates a mailslot server, \\.\mailslot\HelloWorld. It 
* first creates such a mailslot, then it reads the new messages in the slot  
* every 5 seconds.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/10/2009 11:38 AM Riquel Dong
* * 3/12/2009 11:16 AM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Security;
#endregion


class Program
{
    static void Main(string[] args)
    {
        /////////////////////////////////////////////////////////////////////
        // Create the mailslot.
        // 

        // Prepare the slot name
        String strMailslotName = String.Format(@"\\{0}\mailslot\{1}",
            ".",                // Server name
            "HelloWorld"        // Pipe name
        );

        // Prepare the security attributes
        IntPtr pSa = IntPtr.Zero;   // NULL
        SECURITY_ATTRIBUTES sa = new SECURITY_ATTRIBUTES();
        SECURITY_DESCRIPTOR sd;
        SecurityNative.InitializeSecurityDescriptor(out sd, 1);

        // ACL is set as NULL to allow all access to the object.
        SecurityNative.SetSecurityDescriptorDacl(
            ref sd, true, IntPtr.Zero, false);
        sa.lpSecurityDescriptor = Marshal.AllocHGlobal(Marshal.SizeOf(
            typeof(SECURITY_DESCRIPTOR)));
        Marshal.StructureToPtr(sd, sa.lpSecurityDescriptor, false);
        sa.bInheritHandle = false;  // Not inheritable
        sa.nLength = Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES));
        pSa = Marshal.AllocHGlobal(sa.nLength);
        Marshal.StructureToPtr(sa, pSa, false);

        // Create the mailslot.
        IntPtr hMailslot = MailslotNative.CreateMailslot(
            strMailslotName,            // The name of the mailslot
            0,                          // No maximum message size 
            MailslotNative.MAILSLOT_WAIT_FOREVER,
            pSa);                       // Security attributes

        if (hMailslot.ToInt32() == MailslotNative.INVALID_HANDLE_VALUE)
        {
            Console.WriteLine("CreateMailslot failed w/err 0x{0:X}",
                Marshal.GetLastWin32Error());
            return;
        }
        
        Console.WriteLine("Mailslot {0} created successfully.", strMailslotName);


        /////////////////////////////////////////////////////////////////////
        // Check messages in the mailslot.
        // 

        try
        {
            // In a loop, check for new messages every 5 seconds.
            while (true)
            {
                Console.WriteLine("Checking new messages...");
                ReadMailslot(hMailslot);

                System.Threading.Thread.Sleep(5000);
            }
        }
        finally
        {
            /////////////////////////////////////////////////////////////////
            // Close the handle of the mailslot instance.
            // 

            MailslotNative.CloseHandle(hMailslot);
        }
    }

    /// <summary>
    /// Read the messages from a mailslot by using the mailslot handle
    /// in a call to the ReadFile function. 
    /// </summary>
    /// <param name="hMailslot">The handle of the mailslot</param>
    /// <returns> If the function succeeds, the return value is true
    /// </returns>
    static bool ReadMailslot(IntPtr hMailslot)
    {
        int cbMessage = 0;      // Size of the message in bytes
        int cMessages = 0;      // Number of messages in the slot
        int cbRead = 0;         // Number of bytes read from the mailslot
        int nMessageId = 0;     // Message ID

        bool bResult;


        /////////////////////////////////////////////////////////////////////
        // Check for the number of messages in the mailslot.
        // 

        bResult = MailslotNative.GetMailslotInfo(
            hMailslot,              // Mailslot handle 
            IntPtr.Zero,            // No maximum message size 
            out cbMessage,          // Size of next message 
            out cMessages,          // Number of messages 
            IntPtr.Zero             // No read time-out
            );

        if (!bResult)
        {
            Console.WriteLine("GetMailslotInfo failed w/err 0x{0:X}",
                Marshal.GetLastWin32Error());
            return false;
        }

        if (cbMessage == MailslotNative.MAILSLOT_NO_MESSAGE)
        {
            // There are no new messages in the mailslot at present
            Console.WriteLine("No new messages.");
            return true;
        }


        /////////////////////////////////////////////////////////////////////
        // Retrieve the messages one by one from the mailslot.
        //  

        // If there are still un-read messages in the slot
        while (cMessages != 0)
        {
            nMessageId++;

            // Declare a byte array to fetch the data
            byte[] bBuffer = new byte[cbMessage];

            bResult = MailslotNative.ReadFile(
                hMailslot,          // Handle of mailslot
                bBuffer,            // Buffer to receive data
                cbMessage,          // Size of buffer in bytes
                out cbRead,         // Number of bytes read from mailslot
                IntPtr.Zero);       // Not overlapped I/O

            if (!bResult)
            {
                Console.WriteLine("ReadFile failed w/err 0x{0:X}",
                    Marshal.GetLastWin32Error());
                return false;
            }

            // Display the message. 
            Console.WriteLine(("Message #{0}: {1}"), nMessageId, 
                Encoding.Unicode.GetString(bBuffer));

            // Get the current number of un-read messages in the slot. The 
            // number may not equal the initial message number because new 
            // messages may arrive while we are reading the items in the slot.

            bResult = MailslotNative.GetMailslotInfo(
                hMailslot,          // Handle of the mailslot
                IntPtr.Zero,        // No maximum message size 
                out cbMessage,      // Size of next message 
                out cMessages,      // Number of messages 
                IntPtr.Zero         // No read time-out 
                );

            if (!bResult)
            {
                Console.WriteLine("GetMailslotInfo failed w/err 0x{0:X}",
                    Marshal.GetLastWin32Error());
                return false;
            }
        }

        return true;
    }
}