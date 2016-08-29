========================================================================
    CONSOLE APPLICATION : CSNamedPipeClient Project Overview
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
CppNamedPipeClient to connect to the pipe server.

PInvokeNativePipeClient first connects to the named pipe, \\.\pipe\HelloWorld, 
with the GENERIC_READ and GENERIC_WRITE permissions. The client writes a 
message to the pipe server and receives its response.

2. System.IO.Pipes Namespace

In .NET Framework 3.5, the namespace System.IO.Pipes and a set of classes 
(e.g. PipeStream, NamedPipeClientStream) are added to .NET BCL. These classes
make the programming of named pipe in .NET much easier and safer than 
P/Invoke-ing the native APIs directly.

BCLSystemIOPipeClient first connects a named pipe,\\.\pipe\HelloWorld.Then 
writes a message to the pipe server and receives its response.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSNamedPipeClient -> CSNamedPipeServer
CSNamedPipeServer is the server end of the named pipe client.

CppNamedPipeClient - CSNamedPipeClient
CppNamedPipeClient is the native implementation of the named pipe client  
written in C#.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

A. Named pipe client through P/Invoke-ing the native APIs

1. Try to open a named pipe. (CreateFile)

2. Sets the read mode and the blocking mode of the specified named pipe. 
(SetNamedPipeHandleState)

3. Send a message to the pipe server and receive its response. 
(WriteFile, ReadFile)

4. Close the pipe. (CloseHandle)

B. Named pipe client through BCL System.IO.Pipes

1. Create a NamedPipeClientStream variable. (NamedPipeClientStream)

2. Connect to one Named pipe. (NamedPipeClientStream.Connect)

3. Send a message to the pipe server and receive its response. 
(NamedPipeClientStream.Read, NamedPipeClientStream.Write)

4. Close the client.
(NamedPipeServerStream.Close)


/////////////////////////////////////////////////////////////////////////////
References:

NamedPipeClientStream
http://msdn.microsoft.com/en-us/library/system.io.pipes.namedpipeclientstream.aspx

Playing with .NET's Named Pipe Streams By Benjamin Day
(with .NET-to-.NET and Win32-to-.NET samples)
http://blog.benday.com/archive/2008/06/22/23182.aspx

Using Named Pipes in C# / Windows
http://www.dijksterhuis.org/using-named-pipes-in-c-windows/


/////////////////////////////////////////////////////////////////////////////
