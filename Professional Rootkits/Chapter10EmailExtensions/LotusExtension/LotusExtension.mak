#
#  makefile for LotusExtension.dll
#  Windows 32-bit version using Microsoft Visual C++ .NET compiler and linker.
#

# Standard Windows 32-bit make definitions
!include <ntwin32.mak>

cpuflags = -Zp
outfilename = LotusExtension
defname = LotusExtension

all : $(outfilename).dll

$(outfilename).dll : LotusExtension.obj $(defname).def
    $(link) $(linkdebug) \
        -dll -def:$(defname).def \
         -entry:_DllMainCRTStartup$(DLLENTRY) \
         -out:$(outfilename).dll \
        LotusExtension.obj \
        $(guilibs) .\lib\notes.lib


LotusExtension.obj : LotusExtension.h LotusExtension.c
    $(cc) /I .\include $(cdebug) $(cflags) $(cpuflags) /optimize -DNT $(cvars) LotusExtension.c

