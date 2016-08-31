========================================================================
    CONSOLE APPLICATION : CppMailslotClient Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

Mailslot is a mechanism for one-way inter-process communication in the local
machine or across the computers in the intranet. Any clients can store 
messages in a mailslot. The creator of the slot, i.e. the server, retrieves 
the messages that are stored there:

Client (GENERIC_WRITE) ---> Server (GENERIC_READ)

This sample demonstrates a mailslot client that connects and writes to the 
mailslot \\.\mailslot\HelloWorld. 


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CppMailslotClient -> CppMailslotServer
CppMailslotServer is the creator of the mailslot.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Open the mailslot. (CreateFile)

2. Write messages to the mailslot. (WriteMailslot, WriteFile)

3. Close the slot. (CloseHandle)


/////////////////////////////////////////////////////////////////////////////
References:

Using Mailslots / Writing to a Mailslot
http://msdn.microsoft.com/en-us/library/aa365802.aspx

IPCWorkshop By Venkat Raman
http://www.codeproject.com/KB/threads/ipcworkshop.aspx


/////////////////////////////////////////////////////////////////////////////
