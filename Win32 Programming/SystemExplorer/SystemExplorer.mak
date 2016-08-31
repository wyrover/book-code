# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=System Explorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to System Explorer - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "System Explorer - Win32 Release" && "$(CFG)" !=\
 "System Explorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "System Explorer.mak" CFG="System Explorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "System Explorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "System Explorer - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "System Explorer - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "System Explorer - Win32 Release"

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

ALL : "$(OUTDIR)\System Explorer.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DevCaps.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Scroll.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\SysExp.res"
	-@erase "$(INTDIR)\SysMetrics.obj"
	-@erase "$(INTDIR)\SysType.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\Winmain.obj"
	-@erase "$(OUTDIR)\System Explorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/System Explorer.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SysExp.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/System Explorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/System Explorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/System Explorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DevCaps.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Scroll.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\SysExp.res" \
	"$(INTDIR)\SysMetrics.obj" \
	"$(INTDIR)\SysType.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Winmain.obj"

"$(OUTDIR)\System Explorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "System Explorer - Win32 Debug"

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

ALL : "$(OUTDIR)\System Explorer.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DevCaps.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Scroll.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\SysExp.res"
	-@erase "$(INTDIR)\SysMetrics.obj"
	-@erase "$(INTDIR)\SysType.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Winmain.obj"
	-@erase "$(OUTDIR)\System Explorer.exe"
	-@erase "$(OUTDIR)\System Explorer.ilk"
	-@erase "$(OUTDIR)\System Explorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/System Explorer.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SysExp.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/System Explorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/System Explorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/System Explorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DevCaps.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Scroll.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\SysExp.res" \
	"$(INTDIR)\SysMetrics.obj" \
	"$(INTDIR)\SysType.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\Winmain.obj"

"$(OUTDIR)\System Explorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "System Explorer - Win32 Release"
# Name "System Explorer - Win32 Debug"

!IF  "$(CFG)" == "System Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "System Explorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Winmain.c
DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\Utility.h"\
	

"$(INTDIR)\Winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DevCaps.C
DEP_CPP_DEVCA=\
	".\Extensions.h"\
	".\scroll.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\systype.h"\
	".\Utility.h"\
	

"$(INTDIR)\DevCaps.obj" : $(SOURCE) $(DEP_CPP_DEVCA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\devcaps.h"\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\scroll.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\sysmetrics.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c
DEP_CPP_MAINF=\
	".\About.h"\
	".\devcaps.h"\
	".\Extensions.h"\
	".\MainFrameToolbar.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\sysmetrics.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrameToolbar.c
DEP_CPP_MAINFR=\
	".\Extensions.h"\
	".\MainFrameToolbar.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Scroll.C
DEP_CPP_SCROL=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\scroll.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\Utility.h"\
	

"$(INTDIR)\Scroll.obj" : $(SOURCE) $(DEP_CPP_SCROL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSDK.C
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\Utility.h"\
	

"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysExp.RC
DEP_RSC_SYSEX=\
	".\res\br.ico"\
	".\res\devcaps.ico"\
	".\res\Initials.ico"\
	".\res\jmnco.ico"\
	".\res\sysmetrics.ico"\
	".\res\toolbar.bmp"\
	

"$(INTDIR)\SysExp.res" : $(SOURCE) $(DEP_RSC_SYSEX) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysMetrics.C
DEP_CPP_SYSME=\
	".\Extensions.h"\
	".\scroll.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\systype.h"\
	".\Utility.h"\
	

"$(INTDIR)\SysMetrics.obj" : $(SOURCE) $(DEP_CPP_SYSME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysType.C
DEP_CPP_SYSTY=\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\systype.h"\
	".\Utility.h"\
	

"$(INTDIR)\SysType.obj" : $(SOURCE) $(DEP_CPP_SYSTY) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c
DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.C
DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\SysExp.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
