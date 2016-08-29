# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MDIDemo - Win32 Unicode Debug
!MESSAGE No configuration specified.  Defaulting to MDIDemo - Win32 Unicode\
 Debug.
!ENDIF 

!IF "$(CFG)" != "MDIDemo - Win32 Release" && "$(CFG)" !=\
 "MDIDemo - Win32 Debug" && "$(CFG)" != "MDIDemo - Win32 Unicode Debug" &&\
 "$(CFG)" != "MDIDemo - Win32 Unicode Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "MDIDemo.mak" CFG="MDIDemo - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MDIDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MDIDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MDIDemo - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "MDIDemo - Win32 Unicode Release" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "MDIDemo - Win32 Unicode Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "MDIDemo - Win32 Release"

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

ALL : "$(OUTDIR)\MDIDemo.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\Edit.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\Findwnd.obj"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\Graph.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Mdi.obj"
	-@erase "$(INTDIR)\MDIDemo.res"
	-@erase "$(INTDIR)\New.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\MDIDemo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/MDIDemo.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIDemo.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/MDIDemo.pdb" /machine:I386\
 /out:"$(OUTDIR)/MDIDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\Edit.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\Findwnd.obj" \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\Graph.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Mdi.obj" \
	"$(INTDIR)\MDIDemo.res" \
	"$(INTDIR)\New.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\MDIDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"

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

ALL : "$(OUTDIR)\MDIDemo.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\Edit.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\Findwnd.obj"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\Graph.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Mdi.obj"
	-@erase "$(INTDIR)\MDIDemo.res"
	-@erase "$(INTDIR)\New.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\MDIDemo.exe"
	-@erase "$(OUTDIR)\MDIDemo.ilk"
	-@erase "$(OUTDIR)\MDIDemo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/MDIDemo.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIDemo.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/MDIDemo.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MDIDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\Edit.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\Findwnd.obj" \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\Graph.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Mdi.obj" \
	"$(INTDIR)\MDIDemo.res" \
	"$(INTDIR)\New.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\MDIDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MDIDemo_"
# PROP BASE Intermediate_Dir "MDIDemo_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode Debug"
# PROP Intermediate_Dir "Unicode Debug"
# PROP Target_Dir ""
OUTDIR=.\Unicode Debug
INTDIR=.\Unicode Debug

ALL : "$(OUTDIR)\MDIDemo.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\Edit.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\Findwnd.obj"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\Graph.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Mdi.obj"
	-@erase "$(INTDIR)\MDIDemo.res"
	-@erase "$(INTDIR)\New.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\MDIDemo.exe"
	-@erase "$(OUTDIR)\MDIDemo.ilk"
	-@erase "$(OUTDIR)\MDIDemo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "UNICODE" /D "_UNICODE" /Fp"$(INTDIR)/MDIDemo.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Debug/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIDemo.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/MDIDemo.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MDIDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\Edit.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\Findwnd.obj" \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\Graph.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Mdi.obj" \
	"$(INTDIR)\MDIDemo.res" \
	"$(INTDIR)\New.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\MDIDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MDIDemo0"
# PROP BASE Intermediate_Dir "MDIDemo0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode Release"
# PROP Intermediate_Dir "Unicode Release"
# PROP Target_Dir ""
OUTDIR=.\Unicode Release
INTDIR=.\Unicode Release

ALL : "$(OUTDIR)\MDIDemo.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\Edit.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\Findwnd.obj"
	-@erase "$(INTDIR)\frame.obj"
	-@erase "$(INTDIR)\Graph.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrameToolbar.obj"
	-@erase "$(INTDIR)\Mdi.obj"
	-@erase "$(INTDIR)\MDIDemo.res"
	-@erase "$(INTDIR)\New.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\MDIDemo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "UNICODE" /D "_UNICODE" /Fp"$(INTDIR)/MDIDemo.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=".\Unicode Release/"
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MDIDemo.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MDIDemo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/MDIDemo.pdb" /machine:I386\
 /out:"$(OUTDIR)/MDIDemo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\Edit.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\Findwnd.obj" \
	"$(INTDIR)\frame.obj" \
	"$(INTDIR)\Graph.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrameToolbar.obj" \
	"$(INTDIR)\Mdi.obj" \
	"$(INTDIR)\MDIDemo.res" \
	"$(INTDIR)\New.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\MDIDemo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "MDIDemo - Win32 Release"
# Name "MDIDemo - Win32 Debug"
# Name "MDIDemo - Win32 Unicode Debug"
# Name "MDIDemo - Win32 Unicode Release"

!IF  "$(CFG)" == "MDIDemo - Win32 Release"

!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"

!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\WinMain.c

!IF  "$(CFG)" == "MDIDemo - Win32 Release"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"

DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Custom.c
DEP_CPP_CUSTO=\
	".\edit.h"\
	".\error.h"\
	".\Extensions.h"\
	".\findwnd.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\options.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_CUSTO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Edit.c
DEP_CPP_EDIT_=\
	".\edit.h"\
	".\error.h"\
	".\Extensions.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Edit.obj" : $(SOURCE) $(DEP_CPP_EDIT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Edit.obj" : $(SOURCE) $(DEP_CPP_EDIT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Edit.obj" : $(SOURCE) $(DEP_CPP_EDIT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Edit.obj" : $(SOURCE) $(DEP_CPP_EDIT_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Error.c

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Error.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Error.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Error.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Error.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Findwnd.c

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Findwnd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Findwnd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Findwnd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Findwnd.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\frame.c
DEP_CPP_FRAME=\
	".\About.h"\
	".\custom.h"\
	".\Extensions.h"\
	".\graph.h"\
	".\MainFrameToolbar.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\new.h"\
	".\StdSDK.h"\
	".\text.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\frame.obj" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Graph.c

!IF  "$(CFG)" == "MDIDemo - Win32 Release"

DEP_CPP_GRAPH=\
	".\error.h"\
	".\Extensions.h"\
	".\findwnd.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\Graph.obj" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"

DEP_CPP_GRAPH=\
	".\error.h"\
	".\Extensions.h"\
	".\findwnd.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\Graph.obj" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"

DEP_CPP_GRAPH=\
	".\error.h"\
	".\Extensions.h"\
	".\findwnd.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\Graph.obj" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"

DEP_CPP_GRAPH=\
	".\error.h"\
	".\Extensions.h"\
	".\findwnd.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

"$(INTDIR)\Graph.obj" : $(SOURCE) $(DEP_CPP_GRAPH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrameToolbar.c
DEP_CPP_MAINF=\
	".\Extensions.h"\
	".\MainFrameToolbar.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\MainFrameToolbar.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mdi.c
DEP_CPP_MDI_C=\
	".\error.h"\
	".\Extensions.h"\
	".\frame.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Mdi.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Mdi.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Mdi.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Mdi.obj" : $(SOURCE) $(DEP_CPP_MDI_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\New.c
DEP_CPP_NEW_C=\
	".\Extensions.h"\
	".\MDIDemo.h"\
	".\options.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\New.obj" : $(SOURCE) $(DEP_CPP_NEW_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\New.obj" : $(SOURCE) $(DEP_CPP_NEW_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\New.obj" : $(SOURCE) $(DEP_CPP_NEW_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\New.obj" : $(SOURCE) $(DEP_CPP_NEW_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Options.c
DEP_CPP_OPTIO=\
	".\Extensions.h"\
	".\MDIDemo.h"\
	".\options.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdSDK.c
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c
DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c
DEP_CPP_ABOUT=\
	".\About.h"\
	".\Extensions.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MDIDemo.rc
DEP_RSC_MDIDE=\
	".\res\brjmn.ico"\
	".\res\custom.ico"\
	".\res\graph.ico"\
	".\res\mainframe.ico"\
	".\res\text.ico"\
	".\res\Toolbar.bmp"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\MDIDemo.res" : $(SOURCE) $(DEP_RSC_MDIDE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\MDIDemo.res" : $(SOURCE) $(DEP_RSC_MDIDE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\MDIDemo.res" : $(SOURCE) $(DEP_RSC_MDIDE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\MDIDemo.res" : $(SOURCE) $(DEP_RSC_MDIDE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Text.C
DEP_CPP_TEXT_=\
	".\edit.h"\
	".\error.h"\
	".\Extensions.h"\
	".\findwnd.h"\
	".\frame.h"\
	".\Initialization.h"\
	".\Mdi.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Text.obj" : $(SOURCE) $(DEP_CPP_TEXT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Text.obj" : $(SOURCE) $(DEP_CPP_TEXT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Text.obj" : $(SOURCE) $(DEP_CPP_TEXT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Text.obj" : $(SOURCE) $(DEP_CPP_TEXT_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\custom.h"\
	".\Extensions.h"\
	".\frame.h"\
	".\graph.h"\
	".\Initialization.h"\
	".\MDIDemo.h"\
	".\StdSDK.h"\
	".\text.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "MDIDemo - Win32 Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Debug"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MDIDemo - Win32 Unicode Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
