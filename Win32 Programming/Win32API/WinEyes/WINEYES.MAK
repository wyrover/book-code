# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=eyehook - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to eyehook - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "wineyes - Win32 Release" && "$(CFG)" !=\
 "wineyes - Win32 Debug" && "$(CFG)" != "eyehook - Win32 Release" && "$(CFG)" !=\
 "eyehook - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "wineyes.mak" CFG="eyehook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wineyes - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "wineyes - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "eyehook - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "eyehook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "wineyes - Win32 Debug"

!IF  "$(CFG)" == "wineyes - Win32 Release"

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

ALL : "eyehook - Win32 Release" "$(OUTDIR)\wineyes.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\EYES.OBJ"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Template.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\wineyes.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/wineyes.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Template.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/wineyes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib debug\EyeHook.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib debug\EyeHook.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/wineyes.pdb" /machine:I386\
 /out:"$(OUTDIR)/wineyes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\EYES.OBJ" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Template.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj" \
	"$(OUTDIR)\eyehook.lib"

"$(OUTDIR)\wineyes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "wineyes - Win32 Debug"

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

ALL : "eyehook - Win32 Debug" "$(OUTDIR)\wineyes.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\EYES.OBJ"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Template.res"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\wineyes.exe"
	-@erase "$(OUTDIR)\wineyes.ilk"
	-@erase "$(OUTDIR)\wineyes.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/wineyes.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Template.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/wineyes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib version.lib debug\EyeHook.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib comctl32.lib version.lib debug\EyeHook.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/wineyes.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/wineyes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\EYES.OBJ" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Template.res" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj" \
	"$(OUTDIR)\eyehook.lib"

"$(OUTDIR)\wineyes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "eyehook - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "eyehook\Release"
# PROP BASE Intermediate_Dir "eyehook\Release"
# PROP BASE Target_Dir "eyehook"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "eyehook\Release"
# PROP Intermediate_Dir "eyehook\Release"
# PROP Target_Dir "eyehook"
OUTDIR=.\eyehook\Release
INTDIR=.\eyehook\Release

ALL : ".\Release\eyehook.dll"

CLEAN : 
	-@erase ".\Release\eyehook.dll"
	-@erase ".\Release\eyehook.exp"
	-@erase ".\Release\eyehook.lib"
	-@erase ".\Release\SETHOOK.OBJ"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"Release/eyehook.pch" /YX /Fo"Release/" /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"Release/eyehook.pch" /YX /Fo"Release/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/eyehook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /pdb:"Release/eyehook.pdb" /machine:I386 /out:"Release/eyehook.dll" /implib:"Release/eyehook.lib"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll\
 /incremental:no /pdb:"Release/eyehook.pdb" /machine:I386\
 /out:"Release/eyehook.dll" /implib:"Release/eyehook.lib" 
LINK32_OBJS= \
	".\Release\SETHOOK.OBJ"

".\Release\eyehook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "eyehook - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "eyehook\Debug"
# PROP BASE Intermediate_Dir "eyehook\Debug"
# PROP BASE Target_Dir "eyehook"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "eyehook\Debug"
# PROP Intermediate_Dir "eyehook\Debug"
# PROP Target_Dir "eyehook"
OUTDIR=.\eyehook\Debug
INTDIR=.\eyehook\Debug

ALL : ".\Debug\eyehook.dll"

CLEAN : 
	-@erase "..\..\Debug\vc40.idb"
	-@erase "..\..\Debug\vc40.pdb"
	-@erase ".\Debug\eyehook.dll"
	-@erase ".\Debug\eyehook.exp"
	-@erase ".\Debug\eyehook.ilk"
	-@erase ".\Debug\eyehook.lib"
	-@erase ".\Debug\eyehook.pdb"
	-@erase ".\Debug\SETHOOK.OBJ"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"Debug/eyehook.pch" /YX /Fo"Debug/" /Fd"\Debug/" /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"Debug/eyehook.pch" /YX /Fo"Debug/" /Fd"\Debug/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/eyehook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /pdb:"Debug/eyehook.pdb" /debug /machine:I386 /out:"Debug/eyehook.dll" /implib:"Debug/eyehook.lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib uuid.lib /nologo /entry:"DllEntryPoint"\
 /subsystem:windows /dll /incremental:yes /pdb:"Debug/eyehook.pdb" /debug\
 /machine:I386 /out:"Debug/eyehook.dll" /implib:"Debug/eyehook.lib" 
LINK32_OBJS= \
	".\Debug\SETHOOK.OBJ"

".\Debug\eyehook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "wineyes - Win32 Release"
# Name "wineyes - Win32 Debug"

!IF  "$(CFG)" == "wineyes - Win32 Release"

!ELSEIF  "$(CFG)" == "wineyes - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WinMain.c
DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\StdSDK.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c
DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Template.rc
DEP_RSC_TEMPL=\
	".\res\brjmn.ico"\
	".\res\Initials.ico"\
	".\res\TOOLBAR.BMP"\
	

"$(INTDIR)\Template.res" : $(SOURCE) $(DEP_RSC_TEMPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSDK.c
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c
DEP_CPP_MAINF=\
	".\About.h"\
	".\Extensions.h"\
	".\EYES.H"\
	".\StdSDK.h"\
	".\Template.h"\
	".\Utility.h"\
	".\WINEYES.H"\
	

"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\Extensions.h"\
	".\EYES.H"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\StdSDK.h"\
	".\Template.h"\
	".\Utility.h"\
	".\WINEYES.H"\
	

"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c
DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\StdSDK.h"\
	".\Template.h"\
	".\Utility.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EYES.C

"$(INTDIR)\EYES.OBJ" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Project Dependency

# Project_Dep_Name "eyehook"

!IF  "$(CFG)" == "wineyes - Win32 Release"

"eyehook - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\wineyes.mak" CFG="eyehook - Win32 Release" 

!ELSEIF  "$(CFG)" == "wineyes - Win32 Debug"

"eyehook - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\wineyes.mak" CFG="eyehook - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "eyehook - Win32 Release"
# Name "eyehook - Win32 Debug"

!IF  "$(CFG)" == "eyehook - Win32 Release"

!ELSEIF  "$(CFG)" == "eyehook - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\SETHOOK.C

!IF  "$(CFG)" == "eyehook - Win32 Release"

DEP_CPP_SETHO=\
	".\WINEYES.H"\
	

".\Release\SETHOOK.OBJ" : $(SOURCE) $(DEP_CPP_SETHO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "eyehook - Win32 Debug"

DEP_CPP_SETHO=\
	".\WINEYES.H"\
	

".\Debug\SETHOOK.OBJ" : $(SOURCE) $(DEP_CPP_SETHO) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
