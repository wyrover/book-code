# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Towers of Hanoi - Win32 Unicode Debug
!MESSAGE No configuration specified.  Defaulting to Towers of Hanoi - Win32\
 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "Towers of Hanoi - Win32 Release" && "$(CFG)" !=\
 "Towers of Hanoi - Win32 Debug" && "$(CFG)" !=\
 "Towers of Hanoi - Win32 Unicode Debug" && "$(CFG)" !=\
 "Towers of Hanoi - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Towers.mak" CFG="Towers of Hanoi - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Towers of Hanoi - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Towers of Hanoi - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Towers of Hanoi - Win32 Unicode Debug" (based on\
 "Win32 (x86) Application")
!MESSAGE "Towers of Hanoi - Win32 Unicode Release" (based on\
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
# PROP Target_Last_Scanned "Towers of Hanoi - Win32 Unicode Release"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"

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

ALL : "$(OUTDIR)\Towers.exe" "$(OUTDIR)\Towers.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DiskHandling.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Towers.pch"
	-@erase "$(INTDIR)\Towers.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Towers.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Towers.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Towers.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Towers.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Towers.pdb" /machine:I386\
 /out:"$(OUTDIR)/Towers.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DiskHandling.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Towers.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Towers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"

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

ALL : "$(OUTDIR)\Towers.exe" "$(OUTDIR)\Towers.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DiskHandling.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Towers.pch"
	-@erase "$(INTDIR)\Towers.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Towers.exe"
	-@erase "$(OUTDIR)\Towers.ilk"
	-@erase "$(OUTDIR)\Towers.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Towers.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Towers.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Towers.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Towers.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Towers.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DiskHandling.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Towers.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Towers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Towers_o"
# PROP BASE Intermediate_Dir "Towers_o"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode Debug"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Target_Dir ""
OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug

ALL : "$(OUTDIR)\Towers.exe" "$(OUTDIR)\Towers.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DiskHandling.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Towers.pch"
	-@erase "$(INTDIR)\Towers.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Towers.exe"
	-@erase "$(OUTDIR)\Towers.ilk"
	-@erase "$(OUTDIR)\Towers.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Towers.pch" /YX"StdSdk.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Debug/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Towers.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Towers.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Towers.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Towers.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DiskHandling.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Towers.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Towers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Towers_0"
# PROP BASE Intermediate_Dir "Towers_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Release"
# PROP Intermediate_Dir "Unicode Release"
# PROP Target_Dir ""
OUTDIR=.\Unicode Release
INTDIR=.\Unicode Release

ALL : "$(OUTDIR)\Towers.exe" "$(OUTDIR)\Towers.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DiskHandling.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Towers.pch"
	-@erase "$(INTDIR)\Towers.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Towers.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Towers.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=".\Unicode Release/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Towers.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Towers.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Towers.pdb" /machine:I386\
 /out:"$(OUTDIR)/Towers.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DiskHandling.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Towers.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Towers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Towers of Hanoi - Win32 Release"
# Name "Towers of Hanoi - Win32 Debug"
# Name "Towers of Hanoi - Win32 Unicode Debug"
# Name "Towers of Hanoi - Win32 Unicode Release"

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WinMain.c
DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\DiskHandling.h"\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c
DEP_CPP_MAINF=\
	".\About.h"\
	".\DiskHandling.h"\
	".\Extensions.h"\
	".\Initialization.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSdk.c
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"

# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Towers.pch" /Yc"StdSdk.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Towers.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"

# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Towers.pch" /Yc"StdSdk.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Towers.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"

# ADD BASE CPP /Yc"StdSdk.h"
# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Towers.pch" /Yc"StdSdk.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Towers.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"

# ADD BASE CPP /Yc"StdSdk.h"
# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Towers.pch" /Yc"StdSdk.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Towers.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c
DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c
DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DiskHandling.c
DEP_CPP_DISKH=\
	".\About.h"\
	".\DiskHandling.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Towers.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\DiskHandling.obj" : $(SOURCE) $(DEP_CPP_DISKH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\DiskHandling.obj" : $(SOURCE) $(DEP_CPP_DISKH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\DiskHandling.obj" : $(SOURCE) $(DEP_CPP_DISKH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\DiskHandling.obj" : $(SOURCE) $(DEP_CPP_DISKH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Towers.rc
DEP_RSC_TOWER=\
	".\res\br.ico"\
	".\res\Initials.ico"\
	".\res\toolbar.bmp"\
	

!IF  "$(CFG)" == "Towers of Hanoi - Win32 Release"


"$(INTDIR)\Towers.res" : $(SOURCE) $(DEP_RSC_TOWER) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Debug"


"$(INTDIR)\Towers.res" : $(SOURCE) $(DEP_RSC_TOWER) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Debug"


"$(INTDIR)\Towers.res" : $(SOURCE) $(DEP_RSC_TOWER) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Towers of Hanoi - Win32 Unicode Release"


"$(INTDIR)\Towers.res" : $(SOURCE) $(DEP_RSC_TOWER) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
