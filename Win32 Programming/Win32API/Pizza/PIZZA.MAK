# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Skeleton - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Skeleton - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Skeleton - Win32 Release" && "$(CFG)" !=\
 "Skeleton - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "pizza.mak" CFG="Skeleton - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Skeleton - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Skeleton - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "Skeleton - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Skeleton - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : ".\Release\pizza.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ABOUTBOX.OBJ"
	-@erase "$(INTDIR)\dlgtemplate.obj"
	-@erase "$(INTDIR)\Initialization.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\PIZZA.OBJ"
	-@erase "$(INTDIR)\pizza.res"
	-@erase "$(INTDIR)\StdSDK.obj"
	-@erase "$(INTDIR)\Utility.obj"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase ".\Release\pizza.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"Release/pizza.pch" /YX"StdSDK.h" /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"Release/pizza.pch" /YX"StdSDK.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/pizza.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/pizza.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:"Release/pizza.pdb" /machine:I386 /out:"Release\pizza.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=version.lib comctl32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"Release/pizza.pdb" /machine:I386 /out:"Release\pizza.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ABOUTBOX.OBJ" \
	"$(INTDIR)\dlgtemplate.obj" \
	"$(INTDIR)\Initialization.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\PIZZA.OBJ" \
	"$(INTDIR)\pizza.res" \
	"$(INTDIR)\StdSDK.obj" \
	"$(INTDIR)\Utility.obj" \
	"$(INTDIR)\WinMain.obj"

".\Release\pizza.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : ".\Debug\pizza.exe" ".\Debug\pizza.pch"

CLEAN : 
	-@erase ".\Debug\About.obj"
	-@erase ".\Debug\ABOUTBOX.OBJ"
	-@erase ".\Debug\dlgtemplate.obj"
	-@erase ".\Debug\Initialization.obj"
	-@erase ".\Debug\MainFrame.obj"
	-@erase ".\Debug\pizza.exe"
	-@erase ".\Debug\pizza.ilk"
	-@erase ".\Debug\PIZZA.OBJ"
	-@erase ".\Debug\pizza.pch"
	-@erase ".\Debug\pizza.pdb"
	-@erase ".\Debug\pizza.res"
	-@erase ".\Debug\StdSDK.obj"
	-@erase ".\Debug\Utility.obj"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\WinMain.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"Debug/pizza.pch" /YX"StdSDK.h" /Fo"Debug/" /Fd"Debug/" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"Debug/pizza.pch" /YX"StdSDK.h" /Fo"Debug/" /Fd"Debug/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"Debug/pizza.res" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"Debug/pizza.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/pizza.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:"Debug/pizza.pdb" /debug /machine:I386 /out:"Debug\pizza.exe"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=version.lib comctl32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"Debug/pizza.pdb" /debug /machine:I386 /out:"Debug\pizza.exe" 
LINK32_OBJS= \
	".\Debug\About.obj" \
	".\Debug\ABOUTBOX.OBJ" \
	".\Debug\dlgtemplate.obj" \
	".\Debug\Initialization.obj" \
	".\Debug\MainFrame.obj" \
	".\Debug\PIZZA.OBJ" \
	".\Debug\pizza.res" \
	".\Debug\StdSDK.obj" \
	".\Debug\Utility.obj" \
	".\Debug\WinMain.obj"

".\Debug\pizza.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Skeleton - Win32 Release"
# Name "Skeleton - Win32 Debug"

!IF  "$(CFG)" == "Skeleton - Win32 Release"

!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\StdSDK.c
DEP_CPP_STDSD=\
	".\Extensions.h"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"Release/pizza.pch" /YX"StdSDK.h" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"

# ADD CPP /Yc"StdSDK.h"

BuildCmds= \
	$(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"Debug/pizza.pch" /Yc"StdSDK.h" /Fo"Debug/" /Fd"Debug/" /c $(SOURCE) \
	

".\Debug\StdSDK.obj" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

".\Debug\pizza.pch" : $(SOURCE) $(DEP_CPP_STDSD) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WinMain.c
DEP_CPP_WINMA=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\WinMain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Initialization.c
DEP_CPP_INITI=\
	".\Extensions.h"\
	".\Initialization.h"\
	".\MainFrame.h"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\Initialization.obj" : $(SOURCE) $(DEP_CPP_INITI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utility.c
DEP_CPP_UTILI=\
	".\Extensions.h"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\Utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrame.c
DEP_CPP_MAINF=\
	".\About.h"\
	".\Extensions.h"\
	".\PIZZA.H"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\MainFrame.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.c
DEP_CPP_ABOUT=\
	".\About.h"\
	".\aboutbox.h"\
	".\Extensions.h"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "Skeleton - Win32 Release"

!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PIZZA.C
DEP_CPP_PIZZA=\
	".\Extensions.h"\
	".\PIZZA.H"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\PIZZA.OBJ" : $(SOURCE) $(DEP_CPP_PIZZA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\PIZZA.OBJ" : $(SOURCE) $(DEP_CPP_PIZZA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ABOUTBOX.C
DEP_CPP_ABOUTB=\
	".\dlgtemplate.h"\
	".\Extensions.h"\
	".\PizzaApp.h"\
	".\StdSDK.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\ABOUTBOX.OBJ" : $(SOURCE) $(DEP_CPP_ABOUTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\ABOUTBOX.OBJ" : $(SOURCE) $(DEP_CPP_ABOUTB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgtemplate.c
DEP_CPP_DLGTE=\
	".\dlgtemplate.h"\
	".\Extensions.h"\
	".\Utility.h"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\dlgtemplate.obj" : $(SOURCE) $(DEP_CPP_DLGTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\dlgtemplate.obj" : $(SOURCE) $(DEP_CPP_DLGTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pizza.rc
DEP_RSC_PIZZA_=\
	".\res\Initials.ico"\
	

!IF  "$(CFG)" == "Skeleton - Win32 Release"


"$(INTDIR)\pizza.res" : $(SOURCE) $(DEP_RSC_PIZZA_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Skeleton - Win32 Debug"


".\Debug\pizza.res" : $(SOURCE) $(DEP_RSC_PIZZA_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
