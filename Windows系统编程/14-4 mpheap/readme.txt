Multithreaded Heap Manager


Summary
-------

The MPHEAP sample is a DLL that provides an implementation of multiple heaps
and serializes access to the heaps.  It will only work on Windows NT 4.0 and
later.

Many multithreaded applications that use the standard memory allocation
routines pay a significant performance penalty when running on a
multiprocessor machine. This is due to the serialization used by the
default heap. On a multiprocessor machine, more than one thread may try to 
allocate memory simultaneously. One thread will block on the critical section
guarding the heap. The other thread must then signal the critical section
when it is finished to release the waiting thread. This adds significant
overhead.

By providing multiple heaps, MPHEAP.DLL allows simultaneous operations on
each heap. A thread on processor 0 can allocate memory from one heap
at the same time that a thread on processor 1 is allocating from a
different heap. The additional overhead in this DLL is offset by
drastically reducing the number of times a thread must wait for heap access.

The TMPHEAP sample is a simple program that demonstrates the functionality of
MPHEAP.DLL.

More Information
----------------

The basic scheme is to attempt to lock each heap in turn with the
TryEnterCriticalSection function. This will enter the critical section if
it is unowned. If the critical section is owned by a different thread,
TryEnterCriticalSection returns failure instead of blocking until the
other thread leaves the critical section.

Another trick to increase performance is the use of a lookaside list to
satisfy frequent allocations. By using InterlockedExchange to remove
lookaside list entries and InterlockedCompareExchange to add lookaside
list entries, allocations and frees can be completed without needing a
critical section lock.

The final trick is the use of delayed frees. If a chunk of memory is
being freed, and the required lock is already held by a different
thread, the free block is simply added to a delayed free list and the
function completes immediately. The next thread to acquire the heap lock
will free everything on the list.

Every application uses memory allocation routines in different ways.
To allow better tuning, MpHeapGetStatistics allows an application to monitor 
the contention. Increasing the number of heaps increases the potential
concurrency, but also increases memory overhead. Some experimentation
is recommended to determine the optimal settings for a given number of 
processors.

Some applications can benefit from additional techniques. For example,
per-thread lookaside lists for common allocation sizes can be very
effective. No locking is required for a per-thread structure, since no
other thread will ever be accessing it. Because each thread reuses the
same memory, per-thread structures also improve locality of reference.
