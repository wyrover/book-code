========================================================================
    CONSOLE APPLICATION : CSMailslotServer Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

Mailslot is a mechanism for one-way inter-process communication in the local
machine or across the computers in the intranet. Any clients can store 
messages in a mailslot. The creator of the slot, i.e. the server, retrieves 
the messages that are stored there:

Client (GENERIC_WRITE) ---> Server (GENERIC_READ)

This sample demonstrates a mailslot server, \\.\mailslot\HelloWorld. It first
creates such a mailslot, then it reads the new messages in the slot every 
five seconds.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSMailslotClient -> CSMailslotServer
CSMailslotClient is the client end of the mailslot.

CSMailslotServer - CppMailslotServer
CSMailslotServer is the .NET implementation of CppMailslotServer.


/////////////////////////////////////////////////////////////////////////////
Code Logic(P/Invoke the native APIs):

1. Create a mailslot. (CreateMailslot)

2. Check messages in the mailslot. (ReadMailslot)
 
 2.1. Check for the number of messages in the mailslot. (GetMailslotInfo)
 
 2.2. Retrieve the messages one by one from the mailslot. While reading,  
 update the number of messages that are left in the mailslot. 
 (ReadFile, GetMailslotInfo)

3. Close the handle of the mailslot instance. (CloseHandle)


/////////////////////////////////////////////////////////////////////////////
References:

Using Mailslots / Reading from a Mailslot
http://msdn.microsoft.com/en-us/library/aa365785.aspx

IPCWorkshop By Venkat Raman
http://www.codeproject.com/KB/threads/ipcworkshop.aspx


/////////////////////////////////////////////////////////////////////////////
