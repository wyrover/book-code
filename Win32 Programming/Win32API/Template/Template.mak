# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Template - Win32 Unicode Debug
!MESSAGE No configuration specified.  Defaulting to Template - Win32 Unicode\
 Debug.
!ENDIF 

!IF "$(CFG)" != "Template - Win32 Release" && "$(CFG)" !=\
 "Template - Win32 Debug" && "$(CFG)" != "Template - Win32 Unicode Debug" &&\
 "$(CFG)" != "Template - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Template.mak" CFG="Template - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Template - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Template - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Template - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "Template - Win32 Unicode Release" (based on\
 "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Template - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Template - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Template.exe" "$(OUTDIR)\Template.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\Mainframe.obj"
	-@erase "$(INTDIR)\MainframeToolbar.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Template.pch"
	-@erase "$(INTDIR)\Template.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Template.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/Template.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Template.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Template.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Template.pdb" /machine:I386\
 /out:"$(OUTDIR)/Template.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\Mainframe.obj" \
	"$(INTDIR)\MainframeToolbar.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Template.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Template.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Template - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Template.exe" "$(OUTDIR)\Template.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\Mainframe.obj"
	-@erase "$(INTDIR)\MainframeToolbar.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Template.pch"
	-@erase "$(INTDIR)\Template.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Template.exe"
	-@erase "$(OUTDIR)\Template.ilk"
	-@erase "$(OUTDIR)\Template.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/Template.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Template.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Template.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Template.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Template.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\Mainframe.obj" \
	"$(INTDIR)\MainframeToolbar.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Template.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Template.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Template"
# PROP BASE Intermediate_Dir "Template"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode Debug"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Target_Dir ""
OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug

ALL : "$(OUTDIR)\Template.exe" "$(OUTDIR)\Template.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\Mainframe.obj"
	-@erase "$(INTDIR)\MainframeToolbar.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Template.pch"
	-@erase "$(INTDIR)\Template.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Template.exe"
	-@erase "$(OUTDIR)\Template.ilk"
	-@erase "$(OUTDIR)\Template.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Template.pch" /YX"StdSdk.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Debug/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Template.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Template.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Template.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Template.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\Mainframe.obj" \
	"$(INTDIR)\MainframeToolbar.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Template.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Template.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Templat0"
# PROP BASE Intermediate_Dir "Templat0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Release"
# PROP Intermediate_Dir "Unicode Release"
# PROP Target_Dir ""
OUTDIR=.\Unicode Release
INTDIR=.\Unicode Release

ALL : "$(OUTDIR)\Template.exe" "$(OUTDIR)\Template.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\Mainframe.obj"
	-@erase "$(INTDIR)\MainframeToolbar.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Template.pch"
	-@erase "$(INTDIR)\Template.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Template.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Template.pch" /YX"StdSdk.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Release/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Template.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Template.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Template.pdb" /machine:I386\
 /out:"$(OUTDIR)/Template.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\Mainframe.obj" \
	"$(INTDIR)\MainframeToolbar.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Template.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Template.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Template - Win32 Release"
# Name "Template - Win32 Debug"
# Name "Template - Win32 Unicode Debug"
# Name "Template - Win32 Unicode Release"

!IF  "$(CFG)" == "Template - Win32 Release"

!ELSEIF  "$(CFG)" == "Template - Win32 Debug"

!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WinMain.c
DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Template - Win32 Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Template - Win32 Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mainframe.c
DEP_CPP_MAINF=\
	".\About.h"\
	".\Extensions.h"\
	".\MainframeToolbar.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Template - Win32 Release"


"$(INTDIR)\Mainframe.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"


"$(INTDIR)\Mainframe.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"


"$(INTDIR)\Mainframe.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"


"$(INTDIR)\Mainframe.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainframeToolbar.c
DEP_CPP_MAINFR=\
	".\Extensions.h"\
	".\MainframeToolbar.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Template - Win32 Release"


"$(INTDIR)\MainframeToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"


"$(INTDIR)\MainframeToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"


"$(INTDIR)\MainframeToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"


"$(INTDIR)\MainframeToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSdk.c
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Template - Win32 Release"

# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS"\
 /Fp"$(INTDIR)/Template.pch" /Yc"StdSdk.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Template.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Template - Win32 Debug"

# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/Template.pch" /Yc"StdSdk.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Template.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"

# ADD BASE CPP /Yc"StdSdk.h"
# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Template.pch" /Yc"StdSdk.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Template.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"

# ADD BASE CPP /Yc"StdSdk.h"
# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Template.pch" /Yc"StdSdk.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Template.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Template.rc
DEP_RSC_TEMPL=\
	".\res\Initials.ico"\
	".\res\toolbar.bmp"\
	

!IF  "$(CFG)" == "Template - Win32 Release"


"$(INTDIR)\Template.res" : $(SOURCE) $(DEP_RSC_TEMPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"


"$(INTDIR)\Template.res" : $(SOURCE) $(DEP_RSC_TEMPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"


"$(INTDIR)\Template.res" : $(SOURCE) $(DEP_RSC_TEMPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"


"$(INTDIR)\Template.res" : $(SOURCE) $(DEP_RSC_TEMPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c

!IF  "$(CFG)" == "Template - Win32 Release"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c

!IF  "$(CFG)" == "Template - Win32 Release"

DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Debug"

DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Debug"

DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Template - Win32 Unicode Release"

DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
