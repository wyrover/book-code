OBJFILE= lua_int64.o lua_mtutil.o lua_tstring.o luawin_dllerror.o win_privileges.o win_registry.o win_trace.o winreg.o

LIBS= -lscite -ladvapi32 -lkernel32 -luser32
LIBDIR= -L ../lib/

OUTFILE= winreg.dll

LUA= ../../src/scite/lua/include

CUFLAGS= -DUNICODE -D_UNICODE -UMBS -U_MBS

CDEBUG= -Os -DNDEBUG -s

COPT= -DWIN32 -DWINDOWS -DWIN32_LEAN_AND_MEAN -DWINDLL -DUSRDLL

CFLAGS= $(COPT) $(CDEBUG) $(CUFLAGS) -I. -I$(LUA)

CC= gcc

winreg.dll: $(OBJFILE)
	$(CC) $(CFLAGS) -shared -o winreg.dll $(OBJFILE) $(LIBDIR) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: winreg.dll

clean:
	-del *.o

lua_int64.o: lua_int64.c
lua_mtutil.o: lua_mtutil.c
lua_tstring.o: lua_tstring.c
luawin_dllerror.o: luawin_dllerror.c
win_privileges.o: win_privileges.c
win_registry.o: win_registry.c
win_trace.o: win_trace.c
winreg.o: winreg.c
