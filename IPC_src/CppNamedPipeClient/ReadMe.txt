========================================================================
    CONSOLE APPLICATION : CppNamedPipeClient Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

Named pipe is a mechanism for one-way or bi-directional inter-process 
communication between the pipe server and one or more pipe clients in the
local machine or across the computers in the intranet:

PIPE_ACCESS_INBOUND:
Client (GENERIC_WRITE) ---> Server (GENERIC_READ)

PIPE_ACCESS_OUTBOUND:
Client (GENERIC_READ) <--- Server (GENERIC_WRITE)

PIPE_ACCESS_DUPLEX:
Client (GENERIC_READ or GENERIC_WRITE, or both) <--> 
Server (GENERIC_READ and GENERIC_WRITE)

This sample demonstrates a named pipe client that attempts to connect to the  
pipe server, \\.\pipe\HelloWorld, with the GENERIC_READ and GENERIC_WRITE 
permissions. The client writes a message to the pipe server and receives its 
response.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CppNamedPipeClient -> CppNamedPipeServer
CppNamedPipeServer is the server end of the named pipe client.

CSNamedPipeClient - CppNamedPipeClient
CSNamedPipeClient is the .NET implementation of the named pipe client written 
in C++.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Try to open a named pipe. (CreateFile)

2. Sets the read mode and the blocking mode of the specified named pipe. 
(SetNamedPipeHandleState)

3. Send a message to the pipe server and receive its response. 
(WriteFile, ReadFile)

4. Close the pipe. (CloseHandle)


/////////////////////////////////////////////////////////////////////////////
References:

Using Pipes / Named Pipe Client
http://msdn.microsoft.com/en-us/library/aa365592.aspx

IPCWorkshop By Venkat Raman
http://www.codeproject.com/KB/threads/ipcworkshop.aspx


/////////////////////////////////////////////////////////////////////////////
