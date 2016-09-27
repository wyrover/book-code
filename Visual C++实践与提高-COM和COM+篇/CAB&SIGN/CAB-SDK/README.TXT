
                       Cabinet Software Development Kit
                                March 20, 1997
                        ------------------------------

    The Cabinet Software Development Kit provides developers with the
    components needed to utilize Microsoft's cabinet file technology
    within other applications, or to build cabinet file management tools.
    Microsoft is committed to making cabinet files an open technology.

    This release of the Cabinet Software Development Kit (formerly called
    the Cabinet Resource Kit) provides complete documentation of cabinet
    file internals and the various data compression formats used in
    cabinet files, including the new LZX data compression technology.

    The executable and library files in this release are for Microsoft
    Windows 95 and Microsoft (R) Windows (R) NT on Intel (R) i386 (tm)
    platforms.  Some libraries for 16-bit applications on Intel x86
    platforms and other 32-bit Windows NT platforms are available.

    This package will be updated periodically.  Check for the latest at:

        http://www.microsoft.com/workshop/prog/cab

    Microsoft also offer cabinet file classes for JAVA (tm) applications.
    The cabinet file package and sample applications are provided in the
    Microsoft SDK for JAVA.

    Refer to the file REDIST.TXT for the license agreement for the
    Cabinet Software Development Kit including redistribution rights.

        README.TXT      This file
        REDIST.TXT      The Cabinet SDK License Agreement

        BIN\            Cabinet file utilities
          CABARC.EXE    Basic cabinet file create/view/extract utility
          MAKECAB.EXE   More-powerful cabinet file creation tool
          EXTRACT.EXE   Cabinet file extract utility
          CABINET.DLL   Cabinet create/extract libraries in a DLL

        LIB\
          FCI.LIB       File Compression Interface library
          FDI.LIB       File Decompression Interface library

        INCLUDE\
          FCI.H         File Compression Interface header
          FDI.H         File Decompression Interface header

        DOCS\
          CABARC.DOC    User's Guide for CABARC.EXE
          MAKECAB.DOC   User's Guide for MAKECAB.EXE
          FCI-FDI.DOC   FCI/FDI Library Description
          CABFMT.DOC    Cabinet File Format
          MSZIPFMT.DOC  MSZIP Data Compression Format
          LZXFMT.DOC    LZX Data Compression Format

        SAMPLES\
          DLL\          Sample DLL interface functions
          TESTFCI\      Sample file compression application
          TESTFDI\      Sample file decompression application

    The DLL interface functions allow an application to utilize
    CABINET.DLL, dynamically loading/unloading the DLL as FCI or FDI
    library functions are used.

    The TESTFCI sample creates a cabinet file (set) from a collection of
    files named on the command line.  TESTFCI limits each .CAB file to
    300K compressed, to demonstrate files spanning across multiple
    cabinet or multiple diskettes.

    The TESTFDI sample extracts the files from a cabinet, including files
    that span multiple cabinets.  It provides details on the handling the
    callback messages from FDI.

    Comments and questions may be sent via email addressed to
    javacab@microsoft.com.

    Microsoft, MS, and MS-DOS are registered trademarks, and Windows,
    Windows NT, Win32, and Visual C++ are trademarks of Microsoft
    Corporation in the USA and other countries.  Intel is a registered
    trademark, and i386 is a trademark of Intel Corporation.  Unicode is
    a trademark of Unicode, Incorporated.  JAVA is a trademark of Sun
    Microsystems, Inc.
