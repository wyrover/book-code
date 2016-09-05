#!/usr/bin/python
#
# generate_includes.py
#
# A script to generate a series of C++ source files that demonstrate
# the improved performance of using redundant #include guards when you
# have large include dependencies. E.g.,
#
#    #ifndef INC0_H
#    #include "inc0.h"
#    #endif
#
# instead of simply,
#
#    #include "inc0.h"
#
# Usage:
#    generate_includes.py [<file-count>] [<classes-per-file>]
#
# Copyright (c) 2010, Martin Reddy. All rights reserved.
# Distributed under the X11/MIT License. See LICENSE.txt.
# See http://APIBook.com/ for the latest version.
#

import os
import sys
import shutil

DIR_NORMAL       = "src_slow"
DIR_FAST         = "src_fast"
COMPILER         = "g++"
NUMBER_OF_FILES  = 100;
CLASSES_PER_FILE = 10;

def open_output_file(filename):
    '''
    Helper routine to try to open a file for writing
    '''
    try:
        fp = open(filename, "w")
    except Exception, e:
        sys.exit("Couldn't create file: %s\n%s" % (filename, e))
    return fp

def generate_include(dir, curr, max, redundant_guards):
    '''
    Generate an include file that includes a bunch of other headers
    and includes various class definitions for the compiler to parse.
    Pass True for redundant_guards to generate guards around each
    #include statement.
    '''
    fp = open_output_file(os.path.join(dir, "inc%d.h" % curr))
    
    fp.write("#ifndef INC%d_H\n" % curr)
    fp.write("#define INC%d_H\n\n" % curr)

    for i in range(0, max):
        if i == curr:
            continue
        if redundant_guards:
            fp.write("#ifndef INC%d_H\n" % i)
            fp.write("#include \"inc%d.h\"\n" % i)
            fp.write("#endif\n")
        else:
            fp.write("#include \"inc%d.h\"\n" % i)

    for i in range(0, CLASSES_PER_FILE):
        name = "Foo%d_%d" % (curr, i)
        fp.write("\nclass %s\n{\n" % name)
        fp.write("public:\n")
        fp.write("    %s();\n" % name)
        fp.write("    ~%s();\n" % name)
        fp.write("    int GetValue() const;\n")
        fp.write("    void SetValue(int val);\n\n")
        fp.write("private:\n");
        fp.write("    int mValue;\n");
        fp.write("};\n");

    fp.write("#endif\n")
    fp.close()

def generate_cpp(dir, curr):
    '''
    Write a .cpp file that just includes the relevant header file
    '''
    fp = open_output_file(os.path.join(dir, "inc%d.cpp" % curr))
    
    fp.write("#include \"inc%d.h\"\n" % curr)
    fp.close();

def generate_makefile(dir, max):
    '''
    Create the Makefile to compile the set of all generated files
    '''
    fp = open_output_file(os.path.join(dir, "Makefile"))

    fp.write("default:\n")
    for i in range(0, max):
        name = "inc%d" % i
        fp.write("\t%s -c %s.cpp -o %s.o\n" % (COMPILER, name, name))

    fp.close()

def generate_dir(dir, redundant_guards):
    '''
    Create a directory of source files that do or do not use redundant guards
    '''
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.mkdir(dir)
    
    for i in range(0, NUMBER_OF_FILES):
        generate_include(dir, i, NUMBER_OF_FILES, redundant_guards)
        generate_cpp(dir, i)

    generate_makefile(dir, NUMBER_OF_FILES)


if __name__ == "__main__":

    # support optional args of [<file-count>] [<classes-per-file>]
    if len(sys.argv) > 1:
        NUMBER_OF_FILES = int(sys.argv[1])
    if len(sys.argv) > 2:
        CLASSES_PER_FILE = int(sys.argv[2])

    # generate sources with and without redundant guards
    ext = "_%03d" % NUMBER_OF_FILES
    generate_dir(DIR_NORMAL + ext, False)
    generate_dir(DIR_FAST + ext, True)
