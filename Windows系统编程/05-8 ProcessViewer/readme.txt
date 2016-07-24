Process Viewer


SUMMARY
=======

The PView tool reads the information (counters) from the registry that 
relate to the threads and processes running on your machine. It also allows 
you to change the priority of the processes. There is about a third of the 
code necessary to do the same thing on a thread-by-thread basis. If you wish 
to learn more, you may want to try to complete the code.

MORE INFORMATION
================

Two list boxes present themselves on startup. One list box is the list of 
processes. When a particular process is selected, the other list box will 
contain the threads that are associated with the selected process. Clicking 
on the Memory Details button brings up another dialog box that contains more 
detail on how the memory associated with that process is being used. Another 
list box allows you to see how the memory usage breaks down at the DLL 
level.

NOTE: There is also code in the program to allow you to look at the threads 
on other machines. The implementation and activation of this feature is left 
as an exercise for the user.

The way the program works may not be obvious at first. It is important to 
note that when the programmer named a function PaintAddressSpace, it wasn't 
named this way because it fills the monitored process/thread's memory. 
PaintAddressSpace fetches all of the information associated with the 
specified process/thread.

