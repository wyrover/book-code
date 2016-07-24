Pattern Matching Search


SUMMARY
=======

The PDC sample is a character mode program for searching the files in a 
directory tree for a match against a pattern. It uses multiple threads, with 
each thread processing a file at a time, accumulating its matches and 
outputting them to standard output contiguously when it is done searching 
a file.

MORE INFORMATION
================

This program demonstrates how to use many of the advanced operating system
features provided by the Win32 API. The features that this program 
demonstrates are:

  - Creating multiple threads, using synchronization objects
  - Thread termination
  - Virtual memory commitment versus reservation
  - Structured exception handling, using an exception filter procedure 
  - Enumeration of directory entries
  - File mapping
  - Asynchronous file I/O using a completion routine
  - Synchronous file I/O

The command-line syntax is:

PDC [-h] [-v] [-y] [-a | -s | -m] [-t n] SearchString [DirectoryPath]

where:
    h - Prints this message.
    v - Generates verbose output.
    y - Ignores case when doing comparisons.
    a - Specifies that the program should use asynchronous file I/O to read 
        the files being searched.
    s - Specifies that the program should use synchronous file I/O to read 
        the files being searched.
    m - Specifies that the program should use mapped file I/O to read the 
        files being searched.
    t - Specifies the number of threads to use when doing the search. 
        Default is 4 * the number of processors.
    SearchString - Specifies the text to search for. Enclose in quotes if it 
        contains spaces or punctuation.
    DirectoryPath - Specifies the root of the tree to begin the search at. 
        Defaults to the current directory.
