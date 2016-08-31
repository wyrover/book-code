========================================================================
    CONSOLE APPLICATION : CSNamedPipeServer Project Overview
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

.NET supports named pipes in two ways:

1. P/Invoke the native APIs.

By P/Invoke-ing the native APIs from .NET, we can mimic the code logic in 
CppNamedPipeServer to create the named pipe server, \\.\pipe\HelloWorld, that  
supports PIPE_ACCESS_DUPLEX.

PInvokeNativePipeServer first creates such a named pipe, then it listens to  
the client's connection. When a client is connected, the server attempts to  
read the client's requests from the pipe and write a response.

2. System.IO.Pipes Namespace

In .NET Framework 3.5, the namespace System.IO.Pipes and a set of classes 
(e.g. PipeStream, NamedPipeServerStream) are added to .NET BCL. These classes
make the programming of named pipe in .NET much easier and safer than 
P/Invoke-ing the native APIs directly.

BCLSystemIOPipeServer first creates such a named pipe, then it listens to the 
client's connection. When a client is connected, the server attempts to read 
the client's requests from the pipe and write a response.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSNamedPipeClient -> CSNamedPipeServer
CSNamedPipeClient is the client end of the named pipe server.

CppNamedPipeServer - CSNamedPipeServer
CppNamedPipeServer is the native implementation of the named pipe server  
written in C#.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

A. Named pipe server through P/Invoke-ing the native APIs

1. Create a named pipe. (CreateNamedPipe)

2. Wait for the client to connect. (ConnectNamedPipe)

3. Read client requests from the pipe and write the response. 
(ReadFile, WriteFile)

4. Disconnect the pipe, and close the handle. 
(DisconnectNamedPipe, CloseHandle)

B. Named pipe server through BCL System.IO.Pipes

1. Create a named pipe. (NamedPipeServerStream)

2. Wait for the client to connect. (NamedPipeServerStream.WaitForConnection)

3. Read client requests from the pipe and write the response. 
(NamedPipeServerStream.Read, NamedPipeServerStream.Write)

4. Disconnect the pipe, and close the server.
(NamedPipeServerStream.Disconnect, NamedPipeServerStream.Close)


/////////////////////////////////////////////////////////////////////////////
References:

NamedPipeServerStream
http://msdn.microsoft.com/en-us/library/system.io.pipes.namedpipeserverstream.aspx

How to: Use Named Pipes to Communicate Between Processes over a Network
http://msdn.microsoft.com/en-us/library/bb546085.aspx

Introducing Pipes [Justin Van Patten]
http://blogs.msdn.com/bclteam/archive/2006/12/07/introducing-pipes-justin-van-patten.aspx

Playing with .NET's Named Pipe Streams By Benjamin Day
(with .NET-to-.NET and Win32-to-.NET samples)
http://blog.benday.com/archive/2008/06/22/23182.aspx


/////////////////////////////////////////////////////////////////////////////