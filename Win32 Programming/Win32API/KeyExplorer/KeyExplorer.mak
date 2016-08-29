# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Key Explorer - Win32 Unicode Debug
!MESSAGE No configuration specified.  Defaulting to Key Explorer - Win32\
 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "Key Explorer - Win32 Release" && "$(CFG)" !=\
 "Key Explorer - Win32 Debug" && "$(CFG)" !=\
 "Key Explorer - Win32 Unicode Debug" && "$(CFG)" !=\
 "Key Explorer - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "KeyExplorer.mak" CFG="Key Explorer - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Key Explorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Key Explorer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Key Explorer - Win32 Unicode Debug" (based on\
 "Win32 (x86) Application")
!MESSAGE "Key Explorer - Win32 Unicode Release" (based on\
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
# PROP Target_Last_Scanned "Key Explorer - Win32 Release"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

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

ALL : "$(OUTDIR)\KeyExplorer.exe" "$(OUTDIR)\KeyExplorer.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DisplayMessage.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\KeyExplorer.pch"
	-@erase "$(INTDIR)\KeyExplorer.res"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameHeader.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\KeyExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSDK.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/KeyExplorer.pch" /YX"StdSDK.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/KeyExplorer.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/KeyExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/KeyExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/KeyExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DisplayMessage.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\KeyExplorer.res" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameHeader.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\KeyExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

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

ALL : "$(OUTDIR)\KeyExplorer.exe" "$(OUTDIR)\KeyExplorer.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DisplayMessage.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\KeyExplorer.pch"
	-@erase "$(INTDIR)\KeyExplorer.res"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameHeader.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\KeyExplorer.exe"
	-@erase "$(OUTDIR)\KeyExplorer.ilk"
	-@erase "$(OUTDIR)\KeyExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSDK.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/KeyExplorer.pch" /YX"StdSDK.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/KeyExplorer.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/KeyExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/KeyExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/KeyExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DisplayMessage.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\KeyExplorer.res" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameHeader.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\KeyExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Key_Expl"
# PROP BASE Intermediate_Dir "Key_Expl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode Debug"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Target_Dir ""
OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug

ALL : "$(OUTDIR)\KeyExplorer.exe" "$(OUTDIR)\KeyExplorer.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DisplayMessage.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\KeyExplorer.pch"
	-@erase "$(INTDIR)\KeyExplorer.res"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameHeader.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\KeyExplorer.exe"
	-@erase "$(OUTDIR)\KeyExplorer.ilk"
	-@erase "$(OUTDIR)\KeyExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"StdSDK.h" /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /YX"StdSDK.h" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "UNICODE" /D "_UNICODE" /Fp"$(INTDIR)/KeyExplorer.pch" /YX"StdSDK.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Debug/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/KeyExplorer.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/KeyExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/KeyExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/KeyExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DisplayMessage.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\KeyExplorer.res" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameHeader.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\KeyExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Key_Exp0"
# PROP BASE Intermediate_Dir "Key_Exp0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Release"
# PROP Intermediate_Dir "Unicode Release"
# PROP Target_Dir ""
OUTDIR=.\Unicode Release
INTDIR=.\Unicode Release

ALL : "$(OUTDIR)\KeyExplorer.exe" "$(OUTDIR)\KeyExplorer.pch"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\DisplayMessage.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\KeyExplorer.pch"
	-@erase "$(INTDIR)\KeyExplorer.res"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrameHeader.obj"
	-@erase "$(INTDIR)\StdSdk.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\KeyExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"StdSDK.h" /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /YX"StdSDK.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D\
 "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)/KeyExplorer.pch" /YX"StdSDK.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Release/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/KeyExplorer.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/KeyExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/KeyExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/KeyExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\DisplayMessage.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\KeyExplorer.res" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\MainFrameHeader.obj" \
	"$(INTDIR)\StdSdk.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\KeyExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Key Explorer - Win32 Release"
# Name "Key Explorer - Win32 Debug"
# Name "Key Explorer - Win32 Unicode Debug"
# Name "Key Explorer - Win32 Unicode Release"

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WinMain.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DisplayMessage.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_DISPL=\
	".\Columns.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\DisplayMessage.obj" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_DISPL=\
	".\Columns.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\DisplayMessage.obj" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_DISPL=\
	".\Columns.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\DisplayMessage.obj" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_DISPL=\
	".\Columns.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\DisplayMessage.obj" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\KeyExplorer.h"\
	".\Mainframe.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_MAINF=\
	".\About.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_MAINF=\
	".\About.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_MAINF=\
	".\About.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_MAINF=\
	".\About.h"\
	".\DisplayMessage.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrameHeader.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_MAINFR=\
	".\Columns.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameHeader.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_MAINFR=\
	".\Columns.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameHeader.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_MAINFR=\
	".\Columns.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameHeader.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_MAINFR=\
	".\Columns.h"\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\MainframeHeader.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\MainFrameHeader.obj" : $(SOURCE) $(DEP_CPP_MAINFR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSdk.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	
# ADD CPP /Yc"StdSDK.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/KeyExplorer.pch" /Yc"StdSDK.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\KeyExplorer.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	
# ADD CPP /Yc"StdSDK.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/KeyExplorer.pch" /Yc"StdSDK.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\KeyExplorer.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	
# ADD BASE CPP /Yc"StdSDK.h"
# ADD CPP /Yc"StdSDK.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "UNICODE" /D "_UNICODE" /Fp"$(INTDIR)/KeyExplorer.pch" /Yc"StdSDK.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\KeyExplorer.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	
# ADD BASE CPP /Yc"StdSDK.h"
# ADD CPP /Yc"StdSDK.h"

BuildCmds= \
	$(CPP) /nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D\
 "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)/KeyExplorer.pch" /Yc"StdSDK.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdSdk.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\KeyExplorer.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c

!IF  "$(CFG)" == "Key Explorer - Win32 Release"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"

DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c
DEP_CPP_ABOUT=\
	".\Extensions.h"\
	".\KeyExplorer.h"\
	".\StdSdk.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Key Explorer - Win32 Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\KeyExplorer.rc
DEP_RSC_KEYEX=\
	".\res\br.ico"\
	".\res\Initials.ico"\
	".\res\jmnco.ico"\
	".\res\toolbar.bmp"\
	

!IF  "$(CFG)" == "Key Explorer - Win32 Release"


"$(INTDIR)\KeyExplorer.res" : $(SOURCE) $(DEP_RSC_KEYEX) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Debug"


"$(INTDIR)\KeyExplorer.res" : $(SOURCE) $(DEP_RSC_KEYEX) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Debug"


"$(INTDIR)\KeyExplorer.res" : $(SOURCE) $(DEP_RSC_KEYEX) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Key Explorer - Win32 Unicode Release"


"$(INTDIR)\KeyExplorer.res" : $(SOURCE) $(DEP_RSC_KEYEX) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
