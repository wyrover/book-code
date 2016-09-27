# Microsoft Developer Studio Generated NMAKE File, Based on scribble.dsp
!IF "$(CFG)" == ""
CFG=Scribble - Win32 Release
!MESSAGE No configuration specified. Defaulting to Scribble - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "Scribble - Win32 Release" && "$(CFG)" != "Scribble - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "scribble.mak" CFG="Scribble - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Scribble - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Scribble - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Scribble - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

ALL : ".\.\Release\scribble.hlp" "$(OUTDIR)\scribble.exe"


CLEAN :
	-@erase "$(INTDIR)\childfrm.obj"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\pendlg.obj"
	-@erase "$(INTDIR)\scribble.obj"
	-@erase "$(INTDIR)\scribble.pch"
	-@erase "$(INTDIR)\scribble.res"
	-@erase "$(INTDIR)\scribdoc.obj"
	-@erase "$(INTDIR)\scribvw.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\scribble.exe"
	-@erase ".\.\Release\scribble.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\scribble.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\scribble.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\scribble.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\scribble.pdb" /machine:I386 /out:"$(OUTDIR)\scribble.exe" 
LINK32_OBJS= \
	"$(INTDIR)\childfrm.obj" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\pendlg.obj" \
	"$(INTDIR)\scribble.obj" \
	"$(INTDIR)\scribdoc.obj" \
	"$(INTDIR)\scribvw.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\scribble.res"

"$(OUTDIR)\scribble.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Scribble - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

ALL : ".\.\Debug\scribble.hlp" "$(OUTDIR)\scribble.exe"


CLEAN :
	-@erase "$(INTDIR)\childfrm.obj"
	-@erase "$(INTDIR)\mainfrm.obj"
	-@erase "$(INTDIR)\pendlg.obj"
	-@erase "$(INTDIR)\scribble.obj"
	-@erase "$(INTDIR)\scribble.pch"
	-@erase "$(INTDIR)\scribble.res"
	-@erase "$(INTDIR)\scribdoc.obj"
	-@erase "$(INTDIR)\scribvw.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\scribble.exe"
	-@erase "$(OUTDIR)\scribble.ilk"
	-@erase "$(OUTDIR)\scribble.pdb"
	-@erase ".\.\Debug\scribble.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\scribble.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\scribble.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\scribble.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\scribble.pdb" /debug /machine:I386 /out:"$(OUTDIR)\scribble.exe" 
LINK32_OBJS= \
	"$(INTDIR)\childfrm.obj" \
	"$(INTDIR)\mainfrm.obj" \
	"$(INTDIR)\pendlg.obj" \
	"$(INTDIR)\scribble.obj" \
	"$(INTDIR)\scribdoc.obj" \
	"$(INTDIR)\scribvw.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\scribble.res"

"$(OUTDIR)\scribble.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("scribble.dep")
!INCLUDE "scribble.dep"
!ELSE 
!MESSAGE Warning: cannot find "scribble.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Scribble - Win32 Release" || "$(CFG)" == "Scribble - Win32 Debug"
SOURCE=.\childfrm.cpp

"$(INTDIR)\childfrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scribble.pch"


SOURCE=.\mainfrm.cpp

"$(INTDIR)\mainfrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scribble.pch"


SOURCE=.\pendlg.cpp

"$(INTDIR)\pendlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scribble.pch"


SOURCE=.\scribble.cpp

"$(INTDIR)\scribble.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scribble.pch"


SOURCE=.\hlp\scribble.hpj

!IF  "$(CFG)" == "Scribble - Win32 Release"

OutDir=.\.\Release
TargetName=scribble
InputPath=.\hlp\scribble.hpj

"$(OutDir)\$(TargetName).hlp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	makehelp.bat
<< 
	

!ELSEIF  "$(CFG)" == "Scribble - Win32 Debug"

OutDir=.\.\Debug
TargetName=scribble
InputPath=.\hlp\scribble.hpj

"$(OutDir)\$(TargetName).hlp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	makehelp.bat
<< 
	

!ENDIF 

SOURCE=.\scribble.rc

"$(INTDIR)\scribble.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\scribdoc.cpp

"$(INTDIR)\scribdoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scribble.pch"


SOURCE=.\scribvw.cpp

"$(INTDIR)\scribvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scribble.pch"


SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "Scribble - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\scribble.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\scribble.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Scribble - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\scribble.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\scribble.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

