Debug Event Browser


The DEB sample demonstrates the Win32 debug functions. This sample is a 
simple debugger that can debug both a process that it starts or a currently 
running process.

DEB is not a debugger in the traditional sense; it is a browser, as its name 
implies. DEB displays the debug events, and their relevant properties, as 
they occur and invokes the default handlers supplied either by the debuggee 
or the system. Only minimal debug event handling is imposed, so that the 
debug events are displayed and the debuggee continues on its normal course 
of execution.

The sample is source code compatible for the Intel 80x86, the MIPS R4x00, 
and the DEC Alpha AXP Windows NT platforms.
