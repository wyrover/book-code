========================================================================
    WIN32 APPLICATION : CppSendWM_COPYDATA Project Overview
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

CppSendWM_COPYDATA demonstrates how a sending process passes the data to  
other Windows applications by using SendMessage and WM_COPYDATA.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CppSendWM_COPYDATA -> CppReceiveWM_COPYDATA
CppSendWM_COPYDATA sends data to CppReceiveWM_COPYDATA through the  
WM_COPYDATA message.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Find the handle of the receiving Window (FindWindow)

2. Prepare the COPYDATASTRUCT struct with the data to be sent. 
(COPYDATASTRUCT)

3. Send the COPYDATASTRUCT struct through the WM_COPYDATA message to the 
receiving Window.


/////////////////////////////////////////////////////////////////////////////
References:

MSDN: WM_COPYDATA Message
http://msdn.microsoft.com/en-us/library/ms649011.aspx

MSDN: Using Data Copy
http://msdn.microsoft.com/en-us/library/ms649009.aspx

Inter Process Communication using WM_COPYDATA
http://www.codersource.net/mfc_ipc_wm_copydata.aspx


/////////////////////////////////////////////////////////////////////////////
