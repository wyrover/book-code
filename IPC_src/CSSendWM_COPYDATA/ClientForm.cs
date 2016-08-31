/****************************** Module Header ******************************\
* Module Name:	ClientForm.cs
* Project:		CSReceiveWM_COPYDATA
* Copyright (c) Microsoft Corporation.
* 
* IPC based on the Windows message WM_COPYDATA is a mechanism for exchanging 
* data among Windows applications in the local machine. The receiving 
* application must be a Windows application. The data being passed must not 
* contain pointers or other references to objects not accessible to the 
* application receiving the data. While WM_COPYDATA is being sent, the 
* referenced data must not be changed by another thread of the sending 
* process. The receiving application should consider the data read-only. The 
* lParam parameter is valid only during the processing of the message. The 
* receiving application should not free the memory referenced by lParam. If 
* the receiving application must access the data after SendMessage returns, 
* it needs to copy the data into a local buffer.
* 
* CSSendWM_COPYDATA demonstrates how a sending process passes the data to 
* other Windows applications by using SendMessage and WM_COPYDATA.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
* All other rights reserved.
* 
* History:
* * 3/17/2009 08:50 PM Riquel Dong Created
* * 3/19/2009 12:16 AM Jialiang Ge Reviewed
\***************************************************************************/

#region Using directives
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
#endregion


namespace CSSendWM_COPYDATA
{
    public partial class ClientForm : Form
    {
        public ClientForm()
        {
            InitializeComponent();
        }

        private void bnSendMessage_Click(object sender, EventArgs e)
        {
            int nNumber;
            // Validate the parameter value in tbNumber
            if (!int.TryParse(this.tbNumber.Text, out nNumber))
            {
                MessageBox.Show("Invalid value of nNumber!");
                return;
            }


            /////////////////////////////////////////////////////////////////
            // Find the target Window Handle.
            // 

            IntPtr hTargetWnd = WindowNative.FindWindow(null, "CSReceiveWM_COPYDATA");

            // Validate the Window Handle.
            if (hTargetWnd == IntPtr.Zero)
            {
                MessageBox.Show("Unable to find the target Window!");
                return;
            }


            /////////////////////////////////////////////////////////////////
            // Prepare the COPYDATASTRUCT struct with the data to be sent.
            // 

            // Declare the MyStruct struct to hold the message
            MyStruct myStruct;
            myStruct.nNumber = nNumber;
            myStruct.strMessage = this.tbMessage.Text;

            // Marshals from the managed object to a native block of memory
            IntPtr ptrMyStruct = Marshal.AllocHGlobal(Marshal.SizeOf(myStruct));
            Marshal.StructureToPtr(myStruct, ptrMyStruct, true);

            // Declare the COPYDATASTRUCT struct for the WM_COPYDATA message.
            COPYDATASTRUCT cds = new COPYDATASTRUCT();
            cds.cbData = Marshal.SizeOf(myStruct);
            cds.lpData = ptrMyStruct;


            /////////////////////////////////////////////////////////////////
            // Send the COPYDATASTRUCT struct through the WM_COPYDATA message 
            // to the receiving Window.
            // 

            // Send the WM_COPYDATA message
            // The application must use SendMessage, instead of PostMessage 
            // to send WM_COPYDATA because the receiving application must 
            // accept while it is guaranteed to be valid.
            WindowNative.SendMessage(
                hTargetWnd,                 // Handle of the target Window
                WindowNative.WM_COPYDATA,   // WM_COPYDATE message
                this.Handle,                // Handle of the current Window
                ref cds                     // COPYDATASTRUCT structure
                );

            // Check error of SendMessage
            int result = Marshal.GetLastWin32Error();
            if (result != 0)
            {
                MessageBox.Show(String.Format(
                    "SendMessage failed w/err 0x{0:X}", result));
            }


            /////////////////////////////////////////////////////////////////
            // Clean up.
            // 

            // Frees the native memory allocated for MyStruct
            Marshal.FreeHGlobal(ptrMyStruct);
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    internal struct MyStruct
    {
        public int nNumber;

        /// <summary>
        /// TCHAR[256]
        /// </summary>
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string strMessage;
    }
}
