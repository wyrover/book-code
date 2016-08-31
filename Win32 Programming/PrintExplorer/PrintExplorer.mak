# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=PrintDLL - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to PrintDLL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PrintExplorer - Win32 Release" && "$(CFG)" !=\
 "PrintExplorer - Win32 Debug" && "$(CFG)" != "PrintDLL - Win32 Release" &&\
 "$(CFG)" != "PrintDLL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "PrintExplorer.mak" CFG="PrintDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PrintExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PrintExplorer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "PrintDLL - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PrintDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "PrintDLL - Win32 Debug"

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\PrintExplorer.exe" "$(OUTDIR)\PrintExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\Abort.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\BitCombo.obj"
	-@erase "$(INTDIR)\Center.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\commerr.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\devcapa.obj"
	-@erase "$(INTDIR)\DevCaps.obj"
	-@erase "$(INTDIR)\DevMode.obj"
	-@erase "$(INTDIR)\DevNames.obj"
	-@erase "$(INTDIR)\Enumedit.obj"
	-@erase "$(INTDIR)\HandleEdit.obj"
	-@erase "$(INTDIR)\HDevMode.obj"
	-@erase "$(INTDIR)\Hdng.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\MsgLog.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\Paper.obj"
	-@erase "$(INTDIR)\Papers.obj"
	-@erase "$(INTDIR)\Pdata.obj"
	-@erase "$(INTDIR)\Pglog.obj"
	-@erase "$(INTDIR)\PgSetup.obj"
	-@erase "$(INTDIR)\PName.obj"
	-@erase "$(INTDIR)\Printdlg.obj"
	-@erase "$(INTDIR)\PrintDlgSetup.obj"
	-@erase "$(INTDIR)\PrintExplorer.hlp"
	-@erase "$(INTDIR)\PrintExplorer.obj"
	-@erase "$(INTDIR)\PrintExplorer.pch"
	-@erase "$(INTDIR)\PrintExplorer.res"
	-@erase "$(INTDIR)\PrintExplorerDoc.obj"
	-@erase "$(INTDIR)\PrintExplorerView.obj"
	-@erase "$(INTDIR)\PrintFlags.obj"
	-@erase "$(INTDIR)\PrintHook.obj"
	-@erase "$(INTDIR)\Printing.obj"
	-@erase "$(INTDIR)\Prt.obj"
	-@erase "$(INTDIR)\PSizes.obj"
	-@erase "$(INTDIR)\resname.obj"
	-@erase "$(INTDIR)\scombo.obj"
	-@erase "$(INTDIR)\SetDev.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\PrintExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/PrintExplorer.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/PrintExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/PrintExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib Debug\PrintDLL.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib Debug\PrintDLL.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/PrintExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/PrintExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Abort.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\BitCombo.obj" \
	"$(INTDIR)\Center.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\commerr.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\devcapa.obj" \
	"$(INTDIR)\DevCaps.obj" \
	"$(INTDIR)\DevMode.obj" \
	"$(INTDIR)\DevNames.obj" \
	"$(INTDIR)\Enumedit.obj" \
	"$(INTDIR)\HandleEdit.obj" \
	"$(INTDIR)\HDevMode.obj" \
	"$(INTDIR)\Hdng.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\MsgLog.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\Paper.obj" \
	"$(INTDIR)\Papers.obj" \
	"$(INTDIR)\Pdata.obj" \
	"$(INTDIR)\Pglog.obj" \
	"$(INTDIR)\PgSetup.obj" \
	"$(INTDIR)\PName.obj" \
	"$(INTDIR)\Printdlg.obj" \
	"$(INTDIR)\PrintDlgSetup.obj" \
	"$(INTDIR)\PrintExplorer.obj" \
	"$(INTDIR)\PrintExplorer.res" \
	"$(INTDIR)\PrintExplorerDoc.obj" \
	"$(INTDIR)\PrintExplorerView.obj" \
	"$(INTDIR)\PrintFlags.obj" \
	"$(INTDIR)\PrintHook.obj" \
	"$(INTDIR)\Printing.obj" \
	"$(INTDIR)\Prt.obj" \
	"$(INTDIR)\PSizes.obj" \
	"$(INTDIR)\resname.obj" \
	"$(INTDIR)\scombo.obj" \
	"$(INTDIR)\SetDev.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\PrintExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\PrintExplorer.exe" "$(OUTDIR)\PrintExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\Abort.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\BitCombo.obj"
	-@erase "$(INTDIR)\Center.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\commerr.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\devcapa.obj"
	-@erase "$(INTDIR)\DevCaps.obj"
	-@erase "$(INTDIR)\DevMode.obj"
	-@erase "$(INTDIR)\DevNames.obj"
	-@erase "$(INTDIR)\Enumedit.obj"
	-@erase "$(INTDIR)\HandleEdit.obj"
	-@erase "$(INTDIR)\HDevMode.obj"
	-@erase "$(INTDIR)\Hdng.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\MsgLog.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\Paper.obj"
	-@erase "$(INTDIR)\Papers.obj"
	-@erase "$(INTDIR)\Pdata.obj"
	-@erase "$(INTDIR)\Pglog.obj"
	-@erase "$(INTDIR)\PgSetup.obj"
	-@erase "$(INTDIR)\PName.obj"
	-@erase "$(INTDIR)\Printdlg.obj"
	-@erase "$(INTDIR)\PrintDlgSetup.obj"
	-@erase "$(INTDIR)\PrintExplorer.hlp"
	-@erase "$(INTDIR)\PrintExplorer.obj"
	-@erase "$(INTDIR)\PrintExplorer.pch"
	-@erase "$(INTDIR)\PrintExplorer.res"
	-@erase "$(INTDIR)\PrintExplorerDoc.obj"
	-@erase "$(INTDIR)\PrintExplorerView.obj"
	-@erase "$(INTDIR)\PrintFlags.obj"
	-@erase "$(INTDIR)\PrintHook.obj"
	-@erase "$(INTDIR)\Printing.obj"
	-@erase "$(INTDIR)\Prt.obj"
	-@erase "$(INTDIR)\PSizes.obj"
	-@erase "$(INTDIR)\resname.obj"
	-@erase "$(INTDIR)\scombo.obj"
	-@erase "$(INTDIR)\SetDev.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\PrintExplorer.exe"
	-@erase "$(OUTDIR)\PrintExplorer.ilk"
	-@erase "$(OUTDIR)\PrintExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/PrintExplorer.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/PrintExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/PrintExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib Debug\PrintDLL.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib Debug\PrintDLL.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/PrintExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/PrintExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Abort.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\BitCombo.obj" \
	"$(INTDIR)\Center.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\commerr.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\devcapa.obj" \
	"$(INTDIR)\DevCaps.obj" \
	"$(INTDIR)\DevMode.obj" \
	"$(INTDIR)\DevNames.obj" \
	"$(INTDIR)\Enumedit.obj" \
	"$(INTDIR)\HandleEdit.obj" \
	"$(INTDIR)\HDevMode.obj" \
	"$(INTDIR)\Hdng.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\MsgLog.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\Paper.obj" \
	"$(INTDIR)\Papers.obj" \
	"$(INTDIR)\Pdata.obj" \
	"$(INTDIR)\Pglog.obj" \
	"$(INTDIR)\PgSetup.obj" \
	"$(INTDIR)\PName.obj" \
	"$(INTDIR)\Printdlg.obj" \
	"$(INTDIR)\PrintDlgSetup.obj" \
	"$(INTDIR)\PrintExplorer.obj" \
	"$(INTDIR)\PrintExplorer.res" \
	"$(INTDIR)\PrintExplorerDoc.obj" \
	"$(INTDIR)\PrintExplorerView.obj" \
	"$(INTDIR)\PrintFlags.obj" \
	"$(INTDIR)\PrintHook.obj" \
	"$(INTDIR)\Printing.obj" \
	"$(INTDIR)\Prt.obj" \
	"$(INTDIR)\PSizes.obj" \
	"$(INTDIR)\resname.obj" \
	"$(INTDIR)\scombo.obj" \
	"$(INTDIR)\SetDev.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\PrintExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PrintDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PrintDLL\Release"
# PROP BASE Intermediate_Dir "PrintDLL\Release"
# PROP BASE Target_Dir "PrintDLL"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PrintDLL\Release"
# PROP Intermediate_Dir "PrintDLL\Release"
# PROP Target_Dir "PrintDLL"
OUTDIR=.\PrintDLL\Release
INTDIR=.\PrintDLL\Release

ALL : ".\Release\PrintDLL.dll" ".\PrintDLL\PrintDLL\Printmc.h"\
 ".\PrintDLL\PrintDLL\Printmc.rc"

CLEAN : 
	-@erase "$(INTDIR)\center.obj"
	-@erase "$(INTDIR)\printit.obj"
	-@erase ".\PrintDLL\PrintDLL\Printmc.h"
	-@erase ".\PrintDLL\PrintDLL\Printmc.rc"
	-@erase ".\Release\PrintDLL.dll"
	-@erase ".\Release\PrintDLL.exp"
	-@erase ".\Release\PrintDLL.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/PrintDLL.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\PrintDLL\Release/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/PrintDLL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:"Release/PrintDLL.pdb" /machine:I386 /out:"Release/PrintDLL.dll" /implib:"Release/PrintDLL.lib"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"Release/PrintDLL.pdb" /machine:I386 /out:"Release/PrintDLL.dll"\
 /implib:"Release/PrintDLL.lib" 
LINK32_OBJS= \
	"$(INTDIR)\center.obj" \
	"$(INTDIR)\printit.obj"

".\Release\PrintDLL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PrintDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PrintDLL\Debug"
# PROP BASE Intermediate_Dir "PrintDLL\Debug"
# PROP BASE Target_Dir "PrintDLL"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PrintDLL\Debug"
# PROP Intermediate_Dir "PrintDLL\Debug"
# PROP Target_Dir "PrintDLL"
OUTDIR=.\PrintDLL\Debug
INTDIR=.\PrintDLL\Debug

ALL : ".\Debug\PrintDLL.dll" ".\PrintDLL\PrintDLL\Printmc.h"\
 ".\PrintDLL\PrintDLL\Printmc.rc"

CLEAN : 
	-@erase "$(INTDIR)\center.obj"
	-@erase "$(INTDIR)\printit.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase ".\Debug\PrintDLL.dll"
	-@erase ".\Debug\PrintDLL.exp"
	-@erase ".\Debug\PrintDLL.ilk"
	-@erase ".\Debug\PrintDLL.lib"
	-@erase ".\Debug\PrintDLL.pdb"
	-@erase ".\PrintDLL\PrintDLL\Printmc.h"
	-@erase ".\PrintDLL\PrintDLL\Printmc.rc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)/PrintDLL.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\PrintDLL\Debug/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/PrintDLL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:"Debug/PrintDLL.pdb" /debug /machine:I386 /out:"Debug/PrintDLL.dll" /implib:"Debug/PrintDLL.lib"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"Debug/PrintDLL.pdb" /debug /machine:I386 /out:"Debug/PrintDLL.dll"\
 /implib:"Debug/PrintDLL.lib" 
LINK32_OBJS= \
	"$(INTDIR)\center.obj" \
	"$(INTDIR)\printit.obj"

".\Debug\PrintDLL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "PrintExplorer - Win32 Release"
# Name "PrintExplorer - Win32 Debug"

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintExplorer.cpp
DEP_CPP_PRINT=\
	".\About.h"\
	".\BitCombo.h"\
	".\ChildFrm.h"\
	".\commerr.h"\
	".\devcapa.h"\
	".\DevCaps.h"\
	".\DevMode.h"\
	".\DevNames.h"\
	".\EnumEdit.h"\
	".\HandleEdit.h"\
	".\HDevMode.h"\
	".\Hdng.h"\
	".\MainFrm.h"\
	".\Message.h"\
	".\MsgLog.h"\
	".\NumericEdit.h"\
	".\Papers.h"\
	".\Pdata.h"\
	".\PgSetup.h"\
	".\PName.h"\
	".\PrintDlgSetup.h"\
	".\printDLL\printit.h"\
	".\PrintExplorer.h"\
	".\PrintExplorerDoc.h"\
	".\PrintExplorerView.h"\
	".\PrintFlags.h"\
	".\Prt.h"\
	".\PSizes.h"\
	".\resname.h"\
	".\scombo.h"\
	".\SetDev.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PrintExplorer.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/PrintExplorer.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PrintExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/PrintExplorer.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\PrintExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\MainFrm.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintExplorerDoc.cpp
DEP_CPP_PRINTE=\
	".\PrintExplorer.h"\
	".\PrintExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PrintExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_PRINTE) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintExplorerView.cpp
DEP_CPP_PRINTEX=\
	".\PrintExplorer.h"\
	".\PrintExplorerDoc.h"\
	".\PrintExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PrintExplorerView.obj" : $(SOURCE) $(DEP_CPP_PRINTEX) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintExplorer.rc
DEP_RSC_PRINTEXP=\
	".\res\jmnco.ico"\
	".\res\printer.ico"\
	".\res\PrintExplorer.ico"\
	".\res\PrintExplorer.rc2"\
	".\res\PrintExplorerDoc.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\PrintExplorer.res" : $(SOURCE) $(DEP_RSC_PRINTEXP) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\PrintExplorer.hpj

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=PrintExplorer
InputPath=.\hlp\PrintExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=PrintExplorer
InputPath=.\hlp\PrintExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SetDev.cpp
DEP_CPP_SETDE=\
	".\Message.h"\
	".\PrintExplorer.h"\
	".\SetDev.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\SetDev.obj" : $(SOURCE) $(DEP_CPP_SETDE) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BitCombo.cpp
DEP_CPP_BITCO=\
	".\BitCombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BitCombo.obj" : $(SOURCE) $(DEP_CPP_BITCO) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Center.cpp
DEP_CPP_CENTE=\
	".\StdAfx.h"\
	

"$(INTDIR)\Center.obj" : $(SOURCE) $(DEP_CPP_CENTE) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\commerr.cpp
DEP_CPP_COMME=\
	".\commerr.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\commerr.obj" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Custom.c
DEP_CPP_CUSTO=\
	".\Custom.h"\
	".\printDLL\printit.h"\
	

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\devcapa.cpp
DEP_CPP_DEVCA=\
	".\BitCombo.h"\
	".\devcapa.h"\
	".\DevNames.h"\
	".\EnumEdit.h"\
	".\HandleEdit.h"\
	".\HDevMode.h"\
	".\Message.h"\
	".\NumericEdit.h"\
	".\Papers.h"\
	".\Pdata.h"\
	".\PName.h"\
	".\PrintExplorer.h"\
	".\PSizes.h"\
	".\scombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\devcapa.obj" : $(SOURCE) $(DEP_CPP_DEVCA) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DevCaps.cpp
DEP_CPP_DEVCAP=\
	".\BitCombo.h"\
	".\DevCaps.h"\
	".\EnumEdit.h"\
	".\Message.h"\
	".\NumericEdit.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DevCaps.obj" : $(SOURCE) $(DEP_CPP_DEVCAP) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DevMode.cpp
DEP_CPP_DEVMO=\
	".\DevMode.h"\
	".\EnumEdit.h"\
	".\Message.h"\
	".\NumericEdit.h"\
	".\Paper.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DevMode.obj" : $(SOURCE) $(DEP_CPP_DEVMO) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DevNames.cpp
DEP_CPP_DEVNA=\
	".\DevNames.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DevNames.obj" : $(SOURCE) $(DEP_CPP_DEVNA) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Enumedit.cpp
DEP_CPP_ENUME=\
	".\EnumEdit.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Enumedit.obj" : $(SOURCE) $(DEP_CPP_ENUME) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HandleEdit.cpp
DEP_CPP_HANDL=\
	".\HandleEdit.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HandleEdit.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HDevMode.cpp
DEP_CPP_HDEVM=\
	".\HandleEdit.h"\
	".\HDevMode.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HDevMode.obj" : $(SOURCE) $(DEP_CPP_HDEVM) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Hdng.cpp
DEP_CPP_HDNG_=\
	".\Hdng.h"\
	".\printDLL\printit.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Hdng.obj" : $(SOURCE) $(DEP_CPP_HDNG_) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message.cpp
DEP_CPP_MESSA=\
	".\Message.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Message.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MsgLog.cpp
DEP_CPP_MSGLO=\
	".\MsgLog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MsgLog.obj" : $(SOURCE) $(DEP_CPP_MSGLO) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NumericEdit.cpp
DEP_CPP_NUMER=\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NumericEdit.obj" : $(SOURCE) $(DEP_CPP_NUMER) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Paper.cpp
DEP_CPP_PAPER=\
	".\EnumEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Paper.obj" : $(SOURCE) $(DEP_CPP_PAPER) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Papers.cpp
DEP_CPP_PAPERS=\
	".\Papers.h"\
	".\Pdata.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Papers.obj" : $(SOURCE) $(DEP_CPP_PAPERS) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pdata.cpp
DEP_CPP_PDATA=\
	".\EnumEdit.h"\
	".\Paper.h"\
	".\Pdata.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Pdata.obj" : $(SOURCE) $(DEP_CPP_PDATA) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pglog.cpp
DEP_CPP_PGLOG=\
	".\PAPER1.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Pglog.obj" : $(SOURCE) $(DEP_CPP_PGLOG) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PgSetup.cpp
DEP_CPP_PGSET=\
	".\commerr.h"\
	".\DevNames.h"\
	".\HandleEdit.h"\
	".\HDevMode.h"\
	".\Message.h"\
	".\MsgLog.h"\
	".\NumericEdit.h"\
	".\PgLog.h"\
	".\PgSetup.h"\
	".\printDLL\printit.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PgSetup.obj" : $(SOURCE) $(DEP_CPP_PGSET) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PName.cpp
DEP_CPP_PNAME=\
	".\Pdata.h"\
	".\PName.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PName.obj" : $(SOURCE) $(DEP_CPP_PNAME) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Printdlg.cpp
DEP_CPP_PRINTD=\
	".\PrintDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Printdlg.obj" : $(SOURCE) $(DEP_CPP_PRINTD) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintDlgSetup.cpp
DEP_CPP_PRINTDL=\
	".\commerr.h"\
	".\DevNames.h"\
	".\HandleEdit.h"\
	".\HDevMode.h"\
	".\NumericEdit.h"\
	".\PrintDlg.h"\
	".\PrintDlgSetup.h"\
	".\PrintExplorer.h"\
	".\PrintHook.h"\
	".\resname.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PrintDlgSetup.obj" : $(SOURCE) $(DEP_CPP_PRINTDL) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintFlags.cpp
DEP_CPP_PRINTF=\
	".\commerr.h"\
	".\NumericEdit.h"\
	".\PrintDlg.h"\
	".\PrintExplorer.h"\
	".\PrintFlags.h"\
	".\PrintHook.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PrintFlags.obj" : $(SOURCE) $(DEP_CPP_PRINTF) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintHook.C

!IF  "$(CFG)" == "PrintExplorer - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\PrintHook.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /I ".\printDLL" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "PrintExplorer - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

"$(INTDIR)\PrintHook.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".\printDLL" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Printing.cpp
DEP_CPP_PRINTI=\
	".\PrintExplorer.h"\
	".\Printing.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Printing.obj" : $(SOURCE) $(DEP_CPP_PRINTI) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Prt.cpp
DEP_CPP_PRT_C=\
	".\Custom.h"\
	".\Hdng.h"\
	".\NumericEdit.h"\
	".\printDLL\printit.h"\
	".\PrintExplorer.h"\
	".\Prt.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Prt.obj" : $(SOURCE) $(DEP_CPP_PRT_C) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PSizes.cpp
DEP_CPP_PSIZE=\
	".\MMIN.h"\
	".\Pdata.h"\
	".\PrintExplorer.h"\
	".\PSizes.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PSizes.obj" : $(SOURCE) $(DEP_CPP_PSIZE) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\resname.cpp
DEP_CPP_RESNA=\
	".\PrintExplorer.h"\
	".\resname.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\resname.obj" : $(SOURCE) $(DEP_CPP_RESNA) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\scombo.cpp
DEP_CPP_SCOMB=\
	".\PrintExplorer.h"\
	".\scombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\scombo.obj" : $(SOURCE) $(DEP_CPP_SCOMB) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Abort.cpp
DEP_CPP_ABORT=\
	".\Abort.h"\
	".\PrintExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Abort.obj" : $(SOURCE) $(DEP_CPP_ABORT) "$(INTDIR)"\
 "$(INTDIR)\PrintExplorer.pch"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "PrintDLL - Win32 Release"
# Name "PrintDLL - Win32 Debug"

!IF  "$(CFG)" == "PrintDLL - Win32 Release"

!ELSEIF  "$(CFG)" == "PrintDLL - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\PrintDLL\print.rc
# PROP Exclude_From_Build 1
# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintDLL\printit.c
DEP_CPP_PRINTIT=\
	".\PrintDLL\Center.h"\
	".\printDLL\printit.h"\
	".\PrintDLL\Printmc.h"\
	

"$(INTDIR)\printit.obj" : $(SOURCE) $(DEP_CPP_PRINTIT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintDLL\center.c

"$(INTDIR)\center.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PrintDLL\Printmc.mc

!IF  "$(CFG)" == "PrintDLL - Win32 Release"

# Begin Custom Build
ProjDir=.\PrintDLL
InputPath=.\PrintDLL\Printmc.mc
InputName=Printmc

BuildCmds= \
	mc -v -h $(ProjDir) -r $(ProjDir) PrintDLL\$(InputName).mc \
	

"$(ProjDir)\PrintDLL\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\PrintDLL\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "PrintDLL - Win32 Debug"

# Begin Custom Build - Compiling MessageTable
ProjDir=.\PrintDLL
InputPath=.\PrintDLL\Printmc.mc
InputName=Printmc

BuildCmds= \
	mc -v -h $(ProjDir) -r $(ProjDir) PrintDLL\$(InputName).mc \
	

"$(ProjDir)\PrintDLL\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\PrintDLL\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
