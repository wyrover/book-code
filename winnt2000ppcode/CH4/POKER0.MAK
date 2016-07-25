# Microsoft Visual C++ generated build script - Do not modify

PROJ = POKER
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
ORIGIN = MSVCNT
ORIGIN_VER = 1.00
PROJPATH = C:\MARK\ARTICLE\FILEMAP\CODEDISK\ 
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = 
FIRSTCPP = STDAFX.CPP
RC = rc
CFLAGS_D_WEXE32 = /nologo /W3 /YX /D "_X86_" /D "_DEBUG" /D "_WINDOWS" /ML /Fp"Poker.PCH"
CFLAGS_R_WEXE32 = /nologo /G3 /W3 /YX /O2 /Oa /Ob2 /D "_X86_" /D "NDEBUG" /D "_WINDOWS" /ML /Fp"Poker.PCH"
LFLAGS_D_WEXE32 = /NOLOGO /SUBSYSTEM:windows user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib nafxcwd.lib
LFLAGS_R_WEXE32 = /NOLOGO /SUBSYSTEM:windows user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib nafxcw.lib
LFLAGS_D_LIB32 = /NOLOGO
LFLAGS_R_LIB32 = /NOLOGO
LIBS_D_WEXE32 = 
LIBS_R_WEXE32 = 
RCFLAGS32 = 
D_RCDEFINES32 = -d_DEBUG
R_RCDEFINES32 = -dNDEBUG
DEFFILE = POKER.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE32)
LFLAGS = $(LFLAGS_D_WEXE32)
LIBS = $(LIBS_D_WEXE32)
LFLAGS_LIB=$(LFLAGS_D_LIB32)
MAPFILE_OPTION = 
DEFFILE_OPTION = -def:$(DEFFILE)
RCDEFINES = $(D_RCDEFINES32)
!else
CFLAGS = $(CFLAGS_R_WEXE32)
LFLAGS = $(LFLAGS_R_WEXE32)
LIBS = $(LIBS_R_WEXE32)
MAPFILE_OPTION = 
DEFFILE_OPTION = 
LFLAGS_LIB=$(LFLAGS_R_LIB32)
RCDEFINES = $(R_RCDEFINES32)
!endif
SBRS = STDAFX.SBR \
		POKER.SBR \
		FILEMAP.SBR \
		CARDS.SBR


STDAFX_DEP =  \
	c:\mark\article\filemap\codedisk\stdafx.h


POKER_DEP =  \
	c:\mark\article\filemap\codedisk\stdafx.h \
	c:\mark\article\filemap\codedisk\poker.h \
	c:\mark\article\filemap\codedisk\filemap.h \
	c:\mark\article\filemap\codedisk\cards.h


POKER_RCDEP =  \
	c:\mark\article\filemap\codedisk\res\poker.ico \
	c:\mark\article\filemap\codedisk\res\poker.rc2


FILEMAP_DEP =  \
	c:\mark\article\filemap\codedisk\stdafx.h \
	c:\mark\article\filemap\codedisk\filemap.h


CARDS_DEP =  \
	c:\mark\article\filemap\codedisk\stdafx.h \
	c:\mark\article\filemap\codedisk\cards.h


all:	$(PROJ).EXE

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

POKER.OBJ:	POKER.CPP $(POKER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c POKER.CPP

POKER.RES:	POKER.RC $(POKER_RCDEP)
	$(RC) $(RCFLAGS32) $(RCDEFINES) -r POKER.RC

FILEMAP.OBJ:	FILEMAP.CPP $(FILEMAP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FILEMAP.CPP

CARDS.OBJ:	CARDS.CPP $(CARDS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CARDS.CPP


$(PROJ).EXE:	POKER.RES

$(PROJ).EXE:	STDAFX.OBJ POKER.OBJ FILEMAP.OBJ CARDS.OBJ $(OBJS_EXT) $(LIBS_EXT) $(DEFFILE) POKER.RES
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ 
POKER.OBJ 
FILEMAP.OBJ 
CARDS.OBJ 
$(OBJS_EXT)
-OUT:$(PROJ).EXE
$(MAPFILE_OPTION)
POKER.RES
$(LIBS)
$(LIBS_EXT)
$(DEFFILE_OPTION) -implib:$(PROJ).lib
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
