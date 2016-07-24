!include <ntwin32.mak>

!if "$(CPU)" == "i386"
cflags = $(cflags) -D_CRTAPI1=_cdecl -D_CRTAPI2=_cdecl
!else
cflags = $(cflags) -D_CRTAPI1= -D_CRTAPI2=
!endif

all: windiff.exe gutils.dll

# Update GUTILS.DLL
# Update the resources for GUTILS.DLL if necessary

OBJS = gutils.obj gbit.obj gmem.obj gdate.obj status.obj list.obj table.obj tprint.obj tpaint.obj tscroll.obj tree.obj utils.obj gutils.res

gutils.obj: gutils.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

gbit.obj: gbit.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

gmem.obj: gmem.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

gdate.obj: gdate.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

status.obj: status.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

table.obj: table.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

tprint.obj: tprint.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

tpaint.obj: tpaint.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

tscroll.obj: tscroll.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

list.obj: list.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

tree.obj: tree.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

utils.obj: utils.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $(scall) $*.c

 
gutils.res: gutils.rc gutils.dlg gutilsrc.h horzline.cur vertline.cur
    rc $(rcvars) -r gutils.rc

gutils.lib: $(OBJS) gutils.def
    $(implib) -machine:$(CPU) \
        -def:gutils.def \
        $(OBJS) \
        -out:gutils.lib

gutils.dll: $(OBJS) gutils.def gutils.lib
    $(link) $(linkdebug) $(dlllflags) \
    -base:0x1c000000 \
    -out:gutils.dll \
    gutils.exp $(OBJS) $(guilibsdll)
!IF ("$(TARGETLANG)" == "LANG_JAPANESE") && ("$(OS)" == "Windows_NT")
    rlman -p 932 -n 17 1 -a $*.dll $*.tok $*.dll
!ENDIF

# Update WINDIFF.EXE
# Update the resources for WINDIFF.EXE if necessary

WINOBJS = windiff.obj  \
        bar.obj \
        view.obj \
        complist.obj \
        scandir.obj \
        compitem.obj \
        section.obj \
        line.obj \
        file.obj

windiff.res: windiff.rc windiff.h windiff.ico windiff.dlg
    rc $(rcvars) -r windiff.rc

windiff.exe: $(WINOBJS)  \
        windiff.res \
        gutils.lib
  $(link) $(linkdebug) $(guiflags) -out:$*.exe $(WINOBJS) gutils.lib windiff.res $(guilibs)
!IF ("$(TARGETLANG)" == "LANG_JAPANESE") && ("$(OS)" == "Windows_NT")
    rlman -p 932 -n 17 1 -a $*.exe $*.tok $*.exe
!ENDIF

.c.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $(scall) $*.c
