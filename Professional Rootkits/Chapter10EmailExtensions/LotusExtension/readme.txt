PROGRAM - LotusExtension.dll - Lotus Notes Mail Extension Manager

To build: enter "nmake LotusExtension.mak" from a command prompt...

FILES
-----
LotusExtension.c    - Main C source code file for Lotus Extension Manager
LotusExtension.h    - Constants and function prototypes.
LotusExtension.mak  - Make file for Windows 32-bit.
LotusExtension.def  - Module definition file for Windows 32-bit.
readme.txt          - This file.

To install and run this program:

    1.  Exit Notes if it is running.

    2.  Copy LotusExtension.dll to the Notes program directory.
        (usually C:\Program Files\Lotus\Notes)

    3.  Edit the notes.ini file.
        If an EXTMGR_ADDINS entry does not exist, add

        EXTMGR_ADDINS=LotusExtension.dll

        If an EXTMGR_ADDINS entry already exists, add
        ",LotusExtension.dll" to the end of the existing line.

        Save the modified notes.ini file.

    4.  Launch Notes.

    5.  Lotus email will now be filtered through LotusExtension.dll
