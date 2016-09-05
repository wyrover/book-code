This directory contains code to let you test the improved performance
of using redundant #include guards. It's not setup to use CMake, and
it's really tuned to running from a Unix shell using GCC/make, i.e.,
Linux, Mac OS X, or Cygwin. Here's how you can use it:

  Step 1: run "./generate_includes.py"

This will create two directories, src_fast_100 and src_slow_100. The
first has code that uses redundant includes, whereas the second does
not. Each directory has a Makefile to compile the sources.

You can time how long it takes to compile each of these as follows:

  Step 2: run "cd src_fast_100 ; /usr/bin/time -p make"

  Step 3: run "cd src_slow_100 ; /usr/bin/time -p make"

Feel free to check out the actual sources and tweak the generator
script to your heart's content. Note, you can pass two optional
parameters to generate_includes.py to specify how many files to
generate, and how many classes to generate per header.
