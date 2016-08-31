# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Version - Win32 Unicode Debug
!MESSAGE No configuration specified.  Defaulting to Version - Win32 Unicode\
 Debug.
!ENDIF 

!IF "$(CFG)" != "Version - Win32 Release" && "$(CFG)" !=\
 "Version - Win32 Debug" && "$(CFG)" != "Version - Win32 Unicode Debug" &&\
 "$(CFG)" != "Version - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Version.mak" CFG="Version - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Version - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Version - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Version - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "Version - Win32 Unicode Release" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "Version - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Version - Win32 Release"

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

ALL : "$(OUTDIR)\Version.exe" "$(OUTDIR)\Version.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Version.pch"
	-@erase "$(INTDIR)\Version.res"
	-@erase "$(INTDIR)\VersionInfo.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Version.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Version.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Version.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Version.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Version.pdb" /machine:I386\
 /out:"$(OUTDIR)/Version.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Version.res" \
	"$(INTDIR)\VersionInfo.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Version.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Version - Win32 Debug"

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

ALL : "$(OUTDIR)\Version.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Version.res"
	-@erase "$(INTDIR)\VersionInfo.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Version.exe"
	-@erase "$(OUTDIR)\Version.ilk"
	-@erase "$(OUTDIR)\Version.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Version.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Version.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Version.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Version.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Version.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Version.res" \
	"$(INTDIR)\VersionInfo.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Version.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Version_"
# PROP BASE Intermediate_Dir "Version_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode Debug"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Target_Dir ""
OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug

ALL : "$(OUTDIR)\Version.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Version.res"
	-@erase "$(INTDIR)\VersionInfo.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Version.exe"
	-@erase "$(OUTDIR)\Version.ilk"
	-@erase "$(OUTDIR)\Version.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Version.pch" /YX"StdSdk.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Debug/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Version.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Version.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/Version.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/Version.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Version.res" \
	"$(INTDIR)\VersionInfo.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Version.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Version0"
# PROP BASE Intermediate_Dir "Version0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Release"
# PROP Intermediate_Dir "Unicode Release"
# PROP Target_Dir ""
OUTDIR=.\Unicode Release
INTDIR=.\Unicode Release

ALL : "$(OUTDIR)\Version.exe" "$(OUTDIR)\Version.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Version.pch"
	-@erase "$(INTDIR)\Version.res"
	-@erase "$(INTDIR)\VersionInfo.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\Version.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSdk.h" /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /YX"StdSdk.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Version.pch" /YX"StdSdk.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Release/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Version.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Version.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/Version.pdb" /machine:I386\
 /out:"$(OUTDIR)/Version.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Version.res" \
	"$(INTDIR)\VersionInfo.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\Version.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Version - Win32 Release"
# Name "Version - Win32 Debug"
# Name "Version - Win32 Unicode Debug"
# Name "Version - Win32 Unicode Release"

!IF  "$(CFG)" == "Version - Win32 Release"

!ELSEIF  "$(CFG)" == "Version - Win32 Debug"

!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\VersionInfo.c
DEP_CPP_VERSI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	".\VersionInfo.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\VersionInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\VersionInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\VersionInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\VersionInfo.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c
DEP_CPP_MAINF=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSdk.c
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"

# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Version.pch" /Yc"StdSdk.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Version.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Version.pch" /YX"StdSdk.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Version.pch" /YX"StdSdk.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"

# ADD BASE CPP /Yc"StdSdk.h"
# ADD CPP /Yc"StdSdk.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_UNICODE" /D "UNICODE" /Fp"$(INTDIR)/Version.pch" /Yc"StdSdk.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Version.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c
DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Version.RC
DEP_RSC_VERSIO=\
	".\res\Initials.ico"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\Version.res" : $(SOURCE) $(DEP_RSC_VERSIO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\Version.res" : $(SOURCE) $(DEP_RSC_VERSIO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\Version.res" : $(SOURCE) $(DEP_RSC_VERSIO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\Version.res" : $(SOURCE) $(DEP_RSC_VERSIO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c
DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WinMain.c
DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	".\Version.h"\
	

!IF  "$(CFG)" == "Version - Win32 Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Debug"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Debug"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Version - Win32 Unicode Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
