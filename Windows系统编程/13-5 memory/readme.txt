How to Share Memory Between Processes.


SUMMARY
=======

The MEMORY sample demonstrates the use of the file mapping functions to 
create named shared memory.

MORE INFORMATION
================

This sample consists of a client and a server.

Server
------

The server sets up the named share memory. A server can be created by 
selecting Server from the Create menu. This opens up a MDI child and swaps 
the menu bar to the server menu bar. To set up the equivalent of a named 
shared memory:

  - Select Create File from the Server menu. This brings up the File Type 
    dialog box. The user may create either a memory page file or a physical 
    file by clicking on the Page File or the Map File button respectively to 
    back up the soon-to-be-created named shared memory. If the Map File 
    button is selected, the named shared memory will be backed up by a 
    physical file on the disk. On the other hand, if the Page File button 
    is clicked, the memory will be backed up by the memory page file.

  - Select Create File Mapping from the Server menu. This brings up the Map 
    Name dialog box. The user then specifies a name for the file-mapping 
    object which will be created for the file created in step 1 above. This 
    name will be used to identify the shared memory by the clients in the 
    other processes.

  - Select Map View of File from the Server menu. This maps the file-mapping 
    object created in the step above into the process virtual address space.

  - Select Access from the Server menu. This creates a MLE inside the MDI 
    Server child. Whatever was written in the MLE is put in the map file 
    object.

Client
------

The client connects to the named shared memory created by the server 
process. A client can be created by selecting Client from the Create menu. 
This opens up an MDI child and swaps the menu bar to the client menu bar. To 
set up the connection to the named share memory:

  - Select Open File Mapping from the Client menu. This brings up the Map 
    Name dialog box. The user can then enter the name of the file-mapping 
    object which the client wanted to connect to.

  - Select Map View of File from the Client menu. This maps the file-mapping
    object opened in the step above into the process virtual address space.

  - Select Access from the Client menu. This creates a MLE inside the MDI 
    Server child. Whatever was written in the file-mapping object by the 
    server will be shown in this MLE. The client synchronizes with the 
    server at regular intervals.

  - Select Refresh Now from the Client menu to refresh the contents of the 
    file-mapping object immediately.
