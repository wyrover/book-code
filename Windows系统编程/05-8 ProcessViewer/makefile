# Some nmake macros for Win32 apps development

TARGETOS=WINNT

!include <ntwin32.mak>




# application specific C defines

cf = -DUNICODE




# make the executable

all: pviewer.exe




# Update the object file

pviewer.obj: pviewer.c perfdata.h pviewdat.h pviewdlg.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) pviewer.c

pviewdat.obj: pviewdat.c perfdata.h pviewdat.h pviewdlg.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) pviewdat.c

perfdata.obj: perfdata.c perfdata.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) perfdata.c

objdata.obj: objdata.c perfdata.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) objdata.c

instdata.obj: instdata.c perfdata.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) instdata.c

cntrdata.obj: cntrdata.c perfdata.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) cntrdata.c




# Update the resources

pview.res: pview.rc pviewdlg.h pviewdlg.dlg
    rc -r pview.rc




# Update the executable file, add the resource in.

pviewer.exe:    pviewer.obj \
                pviewdat.obj \
                perfdata.obj \
                objdata.obj \
                instdata.obj \
                cntrdata.obj \
                pview.res
    $(link) $(linkdebug) $(guiflags) -out:pviewer.exe \
                pviewer.obj \
                pviewdat.obj \
                perfdata.obj \
                objdata.obj \
                instdata.obj \
                cntrdata.obj \
                pview.res \
                $(guilibs) \
                advapi32.lib
