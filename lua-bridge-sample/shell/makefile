LIBS= -lscite -lshlwapi -lgdi32
LIBDIR= -L ../lib/

OUTFILE= shell.dll

LUA= ../../src/scite/lua/include

CFLAGS = -O2 -s -I$(LUA) -DUNICODE

CXX = g++

$(OUTFILE): resfile.o
	$(CXX) $(CFLAGS) -shared -o $(OUTFILE) shell.cpp utf.cpp ibox/ibox.cpp utils/luaargs.cpp resfile.o $(LIBDIR) $(LIBS)

resfile.o: shell.rc
	windres -o resfile.o shell.rc

all: $(OUTFILE)

clean:
	-del *.o

