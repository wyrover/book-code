========================================================================
    WINDOWS APPLICATION : CSReceiveWM_COPYDATA Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

IPC based on the Windows message WM_COPYDATA is a mechanism for exchanging 
data among Windows applications in the local machine. The receiving 
application must be a Windows application. The data being passed must not 
contain pointers or other references to objects not accessible to the 
application receiving the data. While WM_COPYDATA is being sent, the 
referenced data must not be changed by another thread of the sending process. 
The receiving application should consider the data read-only. The lParam 
parameter is valid only during the processing of the message. The receiving 
application should not free the memory referenced by lParam. If the receiving 
application must access the data after SendMessage returns, it needs to copy 
the data into a local buffer.

CSReceiveWM_COPYDATA demonstrates how a receiving process handles the 
WM_COPYDATA message to get the passed data.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSSendWM_COPYDATA -> CSReceiveWM_COPYDATA
CSSendWM_COPYDATA sends data to CSReceiveWM_COPYDATA through the WM_COPYDATA 
message.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Override the WndProc method in the Windows Form.

	protected override void WndProc(ref Message m)
	{
	}

2.  Capture the WM_COPYDATA message in WndProc 

3. Get the COPYDATASTRUCT struct from Lparam of the WM_COPYDATA message, and 
fetch the data (MyStruct object) from COPYDATASTRUCT.lpData.

	// Get the COPYDATASTRUCT struct from LParam
	COPYDATASTRUCT cds = (COPYDATASTRUCT)m.GetLParam(
		typeof(COPYDATASTRUCT));

	if (cds.cbData == Marshal.SizeOf(typeof(MyStruct)))
	{
		// Marshal the data from the unmanaged memory block
		object data = Marshal.PtrToStructure(cds.lpData, typeof(MyStruct));
		
		// Cast the data to MyStruct
		MyStruct myStruct = (MyStruct)data;
	}

4. Display the data in the form.


/////////////////////////////////////////////////////////////////////////////
References:

WM_COPYDATA Message
http://msdn.microsoft.com/en-us/library/ms649011.aspx

Inter Process Communication using WM_COPYDATA
http://www.codersource.net/mfc_ipc_wm_copydata.aspx


/////////////////////////////////////////////////////////////////////////////
