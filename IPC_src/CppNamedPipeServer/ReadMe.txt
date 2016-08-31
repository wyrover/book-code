========================================================================
    CONSOLE APPLICATION : CppNamedPipeServer Project Overview
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

This sample demonstrates a named pipe server, \\.\pipe\HelloWorld, that 
supports PIPE_ACCESS_DUPLEX. It first creates such a named pipe, then it 
listens to the client's connection. When a client is connected, the server 
attempts to read the client's requests from the pipe and write a response.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CppNamedPipeClient -> CppNamedPipeServer
CppNamedPipeClient is the client end of the named pipe server.

CSNamedPipeServer - CppNamedPipeServer
CSNamedPipeServer is the .NET implementation of the named pipe server written 
in C++.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Create a named pipe. (CreateNamedPipe)

2. Wait for the client to connect. (ConnectNamedPipe)

3. Read client requests from the pipe and write the response. 
(ReadFile, WriteFile)

4. Disconnect the pipe, and close the handle. 
(DisconnectNamedPipe, CloseHandle)


/////////////////////////////////////////////////////////////////////////////
References:

Using Pipes / Multithreaded Pipe Server
http://msdn.microsoft.com/en-us/library/aa365588.aspx

IPCWorkshop By Venkat Raman
http://www.codeproject.com/KB/threads/ipcworkshop.aspx


/////////////////////////////////////////////////////////////////////////////
