A Graphical Difference Program


SUMMARY
=======

The WINDIFF tool takes two files, or two directories, and does a 
byte-for-byte file comparison. It then displays the results in a scrollable 
window, lining up the identical parts, and marking with color the parts of 
the files that differ. This application can be useful for doing source level 
management of code. 

This multithread tool allows you to interact with the window while the file 
comparisons occur in the background.

MORE INFORMATION
================

Usage:

WINDIFF [paths] [saveoption]

paths

Can be either of the following forms:
   
    path 
        Compare what's in the path with what's in the current directory.
   
    path1 path2
        Compare what's in path1 with what's in path2.

saveoption

Can be any combination of the following flags:

    -s: Write the names of files that appear in both paths.
    -l: Write the names of files that appear in only the left-hand path.
    -r: Write the names of files that appear in only the right-hand path.
    -d: Write the names of files that appear in both paths, but the files 
        are different.

Colors:

RED designates text from the left-hand file.
YELLOW designates text from the right-hand file.
BLUE designates a moved line.
BLACK is used for everything else.
