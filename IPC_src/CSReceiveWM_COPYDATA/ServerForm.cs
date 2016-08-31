/****************************** Module Header ******************************\
* Module Name:	ServerForm.cs
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
* CSReceiveWM_COPYDATA demonstrates how a receiving process handles the 
* WM_COPYDATA message to get the passed data.
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


namespace CSReceiveWM_COPYDATA
{
    public partial class ServerForm : Form
    {
        public ServerForm()
        {
            InitializeComponent();
        }

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WindowNative.WM_COPYDATA:

                    // Get the COPYDATASTRUCT struct from LParam
                    COPYDATASTRUCT cds = (COPYDATASTRUCT)m.GetLParam(
                        typeof(COPYDATASTRUCT));

                    if (cds.cbData == Marshal.SizeOf(typeof(MyStruct)))
                    {
                        // Marshal the data from the unmanaged memory block
                        object data = Marshal.PtrToStructure(cds.lpData, 
                            typeof(MyStruct));
                        
                        // Cast the data to MyStruct
                        MyStruct myStruct = (MyStruct)data;

                        // Display the MyStruct data
                        this.lbNumber.Text = myStruct.nNumber.ToString();
                        this.lbMessage.Text = myStruct.strMessage;
                    }
                    break;
            }

            base.WndProc(ref m);
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
