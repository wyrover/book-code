========================================================================
    CONSOLE APPLICATION : CSFileMappingClient Project Overview
========================================================================

/////////////////////////////////////////////////////////////////////////////
Use:

File mapping is a mechanism for one-way or bi-directional inter-process 
communication among two or more processes in the local machine. To share a 
file or memory, all of the processes must use the name or the handle of the 
same file mapping object.

To share a file, the first process creates or opens a file by using the 
CreateFile function. Next, it creates a file mapping object by using the 
CreateFileMapping function, specifying the file handle and a name for the 
file mapping object. The names of event, semaphore, mutex, waitable timer, 
job, and file mapping objects share the same name space. Therefore, the 
CreateFileMapping and OpenFileMapping functions fail if they specify a name
 that is in use by an object of another type.

To share memory that is not associated with a file, a process must use the 
CreateFileMapping function and specify INVALID_HANDLE_VALUE as the hFile 
parameter instead of an existing file handle. The corresponding file mapping 
object accesses memory backed by the system paging file. You must specify 
a size greater than zero when you specify an hFile of INVALID_HANDLE_VALUE 
in a call to CreateFileMapping.

Processes that share files or memory must create file views by using the 
MapViewOfFile or MapViewOfFileEx function. They must coordinate their access 
using semaphores, mutexes, events, or some other mutual exclusion technique.

This sample demonstrates a named shared memory client, 
Local\HelloWorld, that can access the string written to the shared 
memory by the first process. The process display this string as it would any 
other string. In this example, the message box displayed contains the message 
"Message from first process" that was written by the first process.

.NET only supports P/Invoke native APIs currently. By P/Invoke, .NET codes  
can simulate the behaviors of native codes.


/////////////////////////////////////////////////////////////////////////////
Project Relation:

CSFileMappingClient -> CSFileMappingServer
CSFileMappingClient is the client end of the file mapping server.


/////////////////////////////////////////////////////////////////////////////
Code Logic:

1. Try to open a named file mapping. (OpenFileMapping)

2. Maps the view of the file mapping into the address space of the current 
process. (MapViewOfFile)

3. Read message from the view of the shared memory.

4. Unmap the file view and close the file mapping objects.
(UnmapViewOfFile, CloseHandle)


/////////////////////////////////////////////////////////////////////////////
References:

Creating Named Shared Memory
http://msdn.microsoft.com/en-us/library/aa366551.aspx

IPCWorkshop By Venkat Raman
http://www.codeproject.com/KB/threads/ipcworkshop.aspx


/////////////////////////////////////////////////////////////////////////////
