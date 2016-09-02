#include <iostream>

#define AU_DATA_BUF     1024
#define  PIPENAME       _T("\\\\.\\pipe\\CodeProjectPipe")
#define LOG std::wcout


// Pipe communication events
#define AU_ERROR        100
#define AU_READ         101 // Data has been read from pipe
#define AU_WRITE        102 // Data has been written to pipe
#define AU_CLOSE        103 // Close pipe communication
#define AU_INIT         104 // Init phase
#define AU_SERV_RUN    105 // Pipe server running
#define AU_CLNT_CONN    106 // Pipe client connected to server
#define AU_THRD_RUN     107 // Pipe thread launched and running
#define AU_CLNT_WAIT    108 // Waiting for client to get connected
#define AU_CLNT_TRY     109 // Trying to connect to server
#define AU_TERMINATE    110 // Terminate pipe thread


#define AU_IOREAD       120
#define AU_IOWRITE      121
#define AU_IOPENDING    122 // Pending I/O request (thread waits for read/write event)
#define AU_IOWRITECLOSE 123 // Write and then close pipe
