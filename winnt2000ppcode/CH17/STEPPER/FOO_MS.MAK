#
#   Microsoft NMAKE file for FOO
#   NOTE:  Microsoft's RTL requires you to change the getch()
#   command in FOO.C to _getch().

foo.exe: foo.obj

CFLAGS = -c -Od -W3 -Z7 -D_X86_=1 -DWIN32 -DWINVER=0x0400
LFLAGS = -incremental:no -pdb:none -nodefaultlib -debug:full -debugtype:COFF
SUBSYS = -subsystem:console,4
LIBS = libc.lib kernel32.lib 

.c.obj:
     cl $(CFLAGS) $*.c

.obj.exe:
    link $(LFLAGS) $(SUBSYS) -out:$*.exe -map:$*.map $*.obj $(LIBS)


