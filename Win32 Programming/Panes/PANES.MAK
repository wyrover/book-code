# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=panes - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to panes - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "panes - Win32 Release" && "$(CFG)" != "panes - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "panes.mak" CFG="panes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "panes - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "panes - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "panes - Win32 Release"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "panes - Win32 Release"

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

ALL : "$(OUTDIR)\panes.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\CHILD.OBJ"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Panes.res"
	-@erase "$(INTDIR)\POPUP.OBJ"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\panes.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/panes.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Panes.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/panes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/panes.pdb" /machine:I386\
 /out:"$(OUTDIR)/panes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\CHILD.OBJ" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Panes.res" \
	"$(INTDIR)\POPUP.OBJ" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\panes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

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

ALL : "$(OUTDIR)\panes.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\CHILD.OBJ"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Panes.res"
	-@erase "$(INTDIR)\POPUP.OBJ"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\panes.exe"
	-@erase "$(OUTDIR)\panes.ilk"
	-@erase "$(OUTDIR)\panes.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/panes.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Panes.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/panes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/panes.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/panes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\CHILD.OBJ" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Panes.res" \
	"$(INTDIR)\POPUP.OBJ" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\panes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "panes - Win32 Release"
# Name "panes - Win32 Debug"

!IF  "$(CFG)" == "panes - Win32 Release"

!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WinMain.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSdk.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrameToolbar.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_MAINF=\
	".\Extensions.h"\
	".\MainframeToolbar.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_MAINF=\
	".\Extensions.h"\
	".\MainframeToolbar.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_MAINFR=\
	".\About.h"\
	".\Extensions.h"\
	".\MainframeToolbar.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_MAINFR=\
	".\About.h"\
	".\Extensions.h"\
	".\MainframeToolbar.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_INITI=\
	".\Child.h"\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\Popup.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_INITI=\
	".\Child.h"\
	".\Extensions.h"\
	".\Initialization.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\Popup.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\POPUP.C

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_POPUP=\
	".\Extensions.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\POPUP.OBJ" : $(SOURCE) $(DEP_CPP_POPUP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_POPUP=\
	".\Extensions.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\POPUP.OBJ" : $(SOURCE) $(DEP_CPP_POPUP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CHILD.C

!IF  "$(CFG)" == "panes - Win32 Release"

DEP_CPP_CHILD=\
	".\Extensions.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\CHILD.OBJ" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "panes - Win32 Debug"

DEP_CPP_CHILD=\
	".\Extensions.h"\
	".\Mainframe.h"\
	".\Panes.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\CHILD.OBJ" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Panes.rc
DEP_RSC_PANES=\
	".\res\br.ico"\
	".\res\child.ico"\
	".\res\Initials.ico"\
	".\res\jmnco.ico"\
	".\res\popup.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\Panes.res" : $(SOURCE) $(DEP_RSC_PANES) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
