# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=DC Explorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to DC Explorer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DC Explorer - Win32 Release" && "$(CFG)" !=\
 "DC Explorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "DCExplorer.mak" CFG="DC Explorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DC Explorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DC Explorer - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "DC Explorer - Win32 Release"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "DC Explorer - Win32 Release"

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

ALL : "$(OUTDIR)\DCExplorer.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\AlgnSamp.obj"
	-@erase "$(INTDIR)\Align.obj"
	-@erase "$(INTDIR)\BitCombo.obj"
	-@erase "$(INTDIR)\Bkg.obj"
	-@erase "$(INTDIR)\BkgSamp.obj"
	-@erase "$(INTDIR)\Bricks.obj"
	-@erase "$(INTDIR)\BrStyle.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\DCExplorer.obj"
	-@erase "$(INTDIR)\DCExplorer.pch"
	-@erase "$(INTDIR)\DCExplorer.res"
	-@erase "$(INTDIR)\DCExplorerDoc.obj"
	-@erase "$(INTDIR)\DCExplorerView.obj"
	-@erase "$(INTDIR)\DCProps.obj"
	-@erase "$(INTDIR)\DCVals.obj"
	-@erase "$(INTDIR)\devcaps.obj"
	-@erase "$(INTDIR)\DrawBrk.obj"
	-@erase "$(INTDIR)\EndCaps.obj"
	-@erase "$(INTDIR)\EnumEdit.obj"
	-@erase "$(INTDIR)\FltEdit.obj"
	-@erase "$(INTDIR)\GCombo.obj"
	-@erase "$(INTDIR)\Hatch.obj"
	-@erase "$(INTDIR)\HatchSmp.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\IntEdit.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\LineJoin.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mapping.obj"
	-@erase "$(INTDIR)\MapSamp.obj"
	-@erase "$(INTDIR)\Matgraf.obj"
	-@erase "$(INTDIR)\MatSamp.obj"
	-@erase "$(INTDIR)\MatText.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\pCombo.obj"
	-@erase "$(INTDIR)\PenSamp.obj"
	-@erase "$(INTDIR)\PenSel.obj"
	-@erase "$(INTDIR)\penstyle.obj"
	-@erase "$(INTDIR)\Poly.obj"
	-@erase "$(INTDIR)\Polydraw.obj"
	-@erase "$(INTDIR)\Rop2.obj"
	-@erase "$(INTDIR)\RopCodes.obj"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\scombo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Stock.obj"
	-@erase "$(INTDIR)\SysColor.obj"
	-@erase "$(INTDIR)\Sysmsg.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\XForm.obj"
	-@erase "$(OUTDIR)\DCExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/DCExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/DCExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/DCExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/DCExplorer.pdb" /machine:I386 /out:"$(OUTDIR)/DCExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\AlgnSamp.obj" \
	"$(INTDIR)\Align.obj" \
	"$(INTDIR)\BitCombo.obj" \
	"$(INTDIR)\Bkg.obj" \
	"$(INTDIR)\BkgSamp.obj" \
	"$(INTDIR)\Bricks.obj" \
	"$(INTDIR)\BrStyle.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\DCExplorer.obj" \
	"$(INTDIR)\DCExplorer.res" \
	"$(INTDIR)\DCExplorerDoc.obj" \
	"$(INTDIR)\DCExplorerView.obj" \
	"$(INTDIR)\DCProps.obj" \
	"$(INTDIR)\DCVals.obj" \
	"$(INTDIR)\devcaps.obj" \
	"$(INTDIR)\DrawBrk.obj" \
	"$(INTDIR)\EndCaps.obj" \
	"$(INTDIR)\EnumEdit.obj" \
	"$(INTDIR)\FltEdit.obj" \
	"$(INTDIR)\GCombo.obj" \
	"$(INTDIR)\Hatch.obj" \
	"$(INTDIR)\HatchSmp.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\IntEdit.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\LineJoin.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Mapping.obj" \
	"$(INTDIR)\MapSamp.obj" \
	"$(INTDIR)\Matgraf.obj" \
	"$(INTDIR)\MatSamp.obj" \
	"$(INTDIR)\MatText.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\pCombo.obj" \
	"$(INTDIR)\PenSamp.obj" \
	"$(INTDIR)\PenSel.obj" \
	"$(INTDIR)\penstyle.obj" \
	"$(INTDIR)\Poly.obj" \
	"$(INTDIR)\Polydraw.obj" \
	"$(INTDIR)\Rop2.obj" \
	"$(INTDIR)\RopCodes.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\scombo.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Stock.obj" \
	"$(INTDIR)\SysColor.obj" \
	"$(INTDIR)\Sysmsg.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\XForm.obj"

"$(OUTDIR)\DCExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DC Explorer - Win32 Debug"

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

ALL : "$(OUTDIR)\DCExplorer.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\AlgnSamp.obj"
	-@erase "$(INTDIR)\Align.obj"
	-@erase "$(INTDIR)\BitCombo.obj"
	-@erase "$(INTDIR)\Bkg.obj"
	-@erase "$(INTDIR)\BkgSamp.obj"
	-@erase "$(INTDIR)\Bricks.obj"
	-@erase "$(INTDIR)\BrStyle.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\DCExplorer.obj"
	-@erase "$(INTDIR)\DCExplorer.pch"
	-@erase "$(INTDIR)\DCExplorer.res"
	-@erase "$(INTDIR)\DCExplorerDoc.obj"
	-@erase "$(INTDIR)\DCExplorerView.obj"
	-@erase "$(INTDIR)\DCProps.obj"
	-@erase "$(INTDIR)\DCVals.obj"
	-@erase "$(INTDIR)\devcaps.obj"
	-@erase "$(INTDIR)\DrawBrk.obj"
	-@erase "$(INTDIR)\EndCaps.obj"
	-@erase "$(INTDIR)\EnumEdit.obj"
	-@erase "$(INTDIR)\FltEdit.obj"
	-@erase "$(INTDIR)\GCombo.obj"
	-@erase "$(INTDIR)\Hatch.obj"
	-@erase "$(INTDIR)\HatchSmp.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\IntEdit.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\LineJoin.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mapping.obj"
	-@erase "$(INTDIR)\MapSamp.obj"
	-@erase "$(INTDIR)\Matgraf.obj"
	-@erase "$(INTDIR)\MatSamp.obj"
	-@erase "$(INTDIR)\MatText.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\pCombo.obj"
	-@erase "$(INTDIR)\PenSamp.obj"
	-@erase "$(INTDIR)\PenSel.obj"
	-@erase "$(INTDIR)\penstyle.obj"
	-@erase "$(INTDIR)\Poly.obj"
	-@erase "$(INTDIR)\Polydraw.obj"
	-@erase "$(INTDIR)\Rop2.obj"
	-@erase "$(INTDIR)\RopCodes.obj"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\scombo.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Stock.obj"
	-@erase "$(INTDIR)\SysColor.obj"
	-@erase "$(INTDIR)\Sysmsg.obj"
	-@erase "$(INTDIR)\Text.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\XForm.obj"
	-@erase "$(OUTDIR)\DCExplorer.exe"
	-@erase "$(OUTDIR)\DCExplorer.ilk"
	-@erase "$(OUTDIR)\DCExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/DCExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/DCExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/DCExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/DCExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/DCExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\AlgnSamp.obj" \
	"$(INTDIR)\Align.obj" \
	"$(INTDIR)\BitCombo.obj" \
	"$(INTDIR)\Bkg.obj" \
	"$(INTDIR)\BkgSamp.obj" \
	"$(INTDIR)\Bricks.obj" \
	"$(INTDIR)\BrStyle.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\DCExplorer.obj" \
	"$(INTDIR)\DCExplorer.res" \
	"$(INTDIR)\DCExplorerDoc.obj" \
	"$(INTDIR)\DCExplorerView.obj" \
	"$(INTDIR)\DCProps.obj" \
	"$(INTDIR)\DCVals.obj" \
	"$(INTDIR)\devcaps.obj" \
	"$(INTDIR)\DrawBrk.obj" \
	"$(INTDIR)\EndCaps.obj" \
	"$(INTDIR)\EnumEdit.obj" \
	"$(INTDIR)\FltEdit.obj" \
	"$(INTDIR)\GCombo.obj" \
	"$(INTDIR)\Hatch.obj" \
	"$(INTDIR)\HatchSmp.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\IntEdit.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\LineJoin.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Mapping.obj" \
	"$(INTDIR)\MapSamp.obj" \
	"$(INTDIR)\Matgraf.obj" \
	"$(INTDIR)\MatSamp.obj" \
	"$(INTDIR)\MatText.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\pCombo.obj" \
	"$(INTDIR)\PenSamp.obj" \
	"$(INTDIR)\PenSel.obj" \
	"$(INTDIR)\penstyle.obj" \
	"$(INTDIR)\Poly.obj" \
	"$(INTDIR)\Polydraw.obj" \
	"$(INTDIR)\Rop2.obj" \
	"$(INTDIR)\RopCodes.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\scombo.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Stock.obj" \
	"$(INTDIR)\SysColor.obj" \
	"$(INTDIR)\Sysmsg.obj" \
	"$(INTDIR)\Text.obj" \
	"$(INTDIR)\XForm.obj"

"$(OUTDIR)\DCExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "DC Explorer - Win32 Release"
# Name "DC Explorer - Win32 Debug"

!IF  "$(CFG)" == "DC Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "DC Explorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "DC Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "DC Explorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCExplorer.cpp
DEP_CPP_DCEXP=\
	".\about.h"\
	".\ChildFrm.h"\
	".\DCExplorer.h"\
	".\DCExplorerDoc.h"\
	".\DCExplorerView.h"\
	".\dcvals.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DCExplorer.obj" : $(SOURCE) $(DEP_CPP_DCEXP) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "DC Explorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/DCExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\DCExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "DC Explorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/DCExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\DCExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\bricks.h"\
	".\DCExplorer.h"\
	".\drawbrk.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\DCExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCExplorerView.cpp
DEP_CPP_DCEXPL=\
	".\AlgnSamp.h"\
	".\align.h"\
	".\bitcombo.h"\
	".\bkg.h"\
	".\BrStyle.h"\
	".\color.h"\
	".\DCExplorer.h"\
	".\DCExplorerDoc.h"\
	".\DCExplorerView.h"\
	".\DCProps.h"\
	".\dcvals.h"\
	".\degrad.h"\
	".\devcaps.h"\
	".\endcaps.h"\
	".\enumedit.h"\
	".\fltedit.h"\
	".\gcombo.h"\
	".\hatch.h"\
	".\hatchsmp.h"\
	".\idcombo.h"\
	".\intedit.h"\
	".\is95.h"\
	".\LineJoin.h"\
	".\mapping.h"\
	".\mapsamp.h"\
	".\MatGraf.h"\
	".\MatSamp.h"\
	".\MatText.h"\
	".\Message.h"\
	".\numericedit.h"\
	".\palette.h"\
	".\pcombo.h"\
	".\PenSamp.h"\
	".\pensel.h"\
	".\PenStyle.h"\
	".\poly.h"\
	".\polydraw.h"\
	".\ropcodes.h"\
	".\StdAfx.h"\
	".\stock.h"\
	".\SysColor.h"\
	".\SysMsg.h"\
	".\text.h"\
	".\xform.h"\
	

"$(INTDIR)\DCExplorerView.obj" : $(SOURCE) $(DEP_CPP_DCEXPL) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCExplorerDoc.cpp
DEP_CPP_DCEXPLO=\
	".\DCExplorer.h"\
	".\DCExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DCExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_DCEXPLO) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\DCExplorer.hpj

!IF  "$(CFG)" == "DC Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "DC Explorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCExplorer.rc
DEP_RSC_DCEXPLOR=\
	".\res\bricks.bmp"\
	".\res\candump.cur"\
	".\res\cursor1.cur"\
	".\res\DCExplorer.ico"\
	".\res\DCExplorer.rc2"\
	".\res\DCExplorerDoc.ico"\
	".\res\dumped.cur"\
	".\res\dumping1.cur"\
	".\res\dumping2.cur"\
	".\res\dumping3.cur"\
	".\res\jmnco.ico"\
	".\res\self.cur"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\DCExplorer.res" : $(SOURCE) $(DEP_RSC_DCEXPLOR) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Is95.cpp

"$(INTDIR)\Is95.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\XForm.cpp
DEP_CPP_XFORM=\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\degrad.h"\
	".\fltedit.h"\
	".\MatGraf.h"\
	".\MatSamp.h"\
	".\MatText.h"\
	".\Message.h"\
	".\StdAfx.h"\
	".\SysMsg.h"\
	".\uwm.h"\
	".\xform.h"\
	

"$(INTDIR)\XForm.obj" : $(SOURCE) $(DEP_CPP_XFORM) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AlgnSamp.cpp
DEP_CPP_ALGNS=\
	".\AlgnSamp.h"\
	".\DCExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AlgnSamp.obj" : $(SOURCE) $(DEP_CPP_ALGNS) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Align.cpp
DEP_CPP_ALIGN=\
	".\AlgnSamp.h"\
	".\align.h"\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\is95.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Align.obj" : $(SOURCE) $(DEP_CPP_ALIGN) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BitCombo.CPP
DEP_CPP_BITCO=\
	".\bitcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BitCombo.obj" : $(SOURCE) $(DEP_CPP_BITCO) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bkg.cpp
DEP_CPP_BKG_C=\
	".\bkg.h"\
	".\bkgsamp.h"\
	".\BrStyle.h"\
	".\color.h"\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\gcombo.h"\
	".\hatch.h"\
	".\hatchsmp.h"\
	".\idcombo.h"\
	".\intedit.h"\
	".\is95.h"\
	".\poly.h"\
	".\rop2.h"\
	".\ropcodes.h"\
	".\sample.h"\
	".\StdAfx.h"\
	".\stock.h"\
	".\SysColor.h"\
	

"$(INTDIR)\Bkg.obj" : $(SOURCE) $(DEP_CPP_BKG_C) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BkgSamp.cpp
DEP_CPP_BKGSA=\
	".\bkgsamp.h"\
	".\DCExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BkgSamp.obj" : $(SOURCE) $(DEP_CPP_BKGSA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bricks.cpp
DEP_CPP_BRICK=\
	".\bricks.h"\
	".\copy.h"\
	".\DCExplorer.h"\
	".\drawbrk.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Bricks.obj" : $(SOURCE) $(DEP_CPP_BRICK) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BrStyle.CPP
DEP_CPP_BRSTY=\
	".\BrStyle.h"\
	".\DCExplorer.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BrStyle.obj" : $(SOURCE) $(DEP_CPP_BRSTY) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Color.cpp
DEP_CPP_COLOR=\
	".\color.h"\
	".\colors.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Color.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Copy.CPP
DEP_CPP_COPY_=\
	".\StdAfx.h"\
	

"$(INTDIR)\Copy.obj" : $(SOURCE) $(DEP_CPP_COPY_) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCProps.cpp
DEP_CPP_DCPRO=\
	".\DCExplorer.h"\
	".\DCProps.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DCProps.obj" : $(SOURCE) $(DEP_CPP_DCPRO) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DCVals.CPP
DEP_CPP_DCVAL=\
	".\dcvals.h"\
	

"$(INTDIR)\DCVals.obj" : $(SOURCE) $(DEP_CPP_DCVAL) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\devcaps.cpp
DEP_CPP_DEVCA=\
	".\bitcombo.h"\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\devcaps.h"\
	".\enumedit.h"\
	".\Message.h"\
	".\numericedit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\devcaps.obj" : $(SOURCE) $(DEP_CPP_DEVCA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawBrk.cpp
DEP_CPP_DRAWB=\
	".\DCExplorer.h"\
	".\drawbrk.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DrawBrk.obj" : $(SOURCE) $(DEP_CPP_DRAWB) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EndCaps.CPP
DEP_CPP_ENDCA=\
	".\DCExplorer.h"\
	".\endcaps.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EndCaps.obj" : $(SOURCE) $(DEP_CPP_ENDCA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EnumEdit.CPP
DEP_CPP_ENUME=\
	".\enumedit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EnumEdit.obj" : $(SOURCE) $(DEP_CPP_ENUME) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FltEdit.cpp
DEP_CPP_FLTED=\
	".\DCExplorer.h"\
	".\fltedit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FltEdit.obj" : $(SOURCE) $(DEP_CPP_FLTED) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MapSamp.cpp
DEP_CPP_MAPSA=\
	".\DCExplorer.h"\
	".\mapsamp.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\MapSamp.obj" : $(SOURCE) $(DEP_CPP_MAPSA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Matgraf.cpp
DEP_CPP_MATGR=\
	".\DCExplorer.h"\
	".\MatGraf.h"\
	".\MatSamp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Matgraf.obj" : $(SOURCE) $(DEP_CPP_MATGR) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MatSamp.cpp
DEP_CPP_MATSA=\
	".\DCExplorer.h"\
	".\MatSamp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MatSamp.obj" : $(SOURCE) $(DEP_CPP_MATSA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MatText.cpp
DEP_CPP_MATTE=\
	".\DCExplorer.h"\
	".\MatSamp.h"\
	".\MatText.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\MatText.obj" : $(SOURCE) $(DEP_CPP_MATTE) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message.cpp
DEP_CPP_MESSA=\
	".\Message.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Message.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NumericEdit.cpp
DEP_CPP_NUMER=\
	".\numericedit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NumericEdit.obj" : $(SOURCE) $(DEP_CPP_NUMER) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Palette.cpp
DEP_CPP_PALET=\
	".\color.h"\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\palette.h"\
	".\pcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Palette.obj" : $(SOURCE) $(DEP_CPP_PALET) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\pCombo.cpp
DEP_CPP_PCOMB=\
	".\color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\pcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\pCombo.obj" : $(SOURCE) $(DEP_CPP_PCOMB) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PenSamp.cpp
DEP_CPP_PENSA=\
	".\DCExplorer.h"\
	".\degrad.h"\
	".\PenSamp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PenSamp.obj" : $(SOURCE) $(DEP_CPP_PENSA) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PenSel.cpp
DEP_CPP_PENSE=\
	".\BrStyle.h"\
	".\color.h"\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\endcaps.h"\
	".\fltedit.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\intedit.h"\
	".\LineJoin.h"\
	".\PenSamp.h"\
	".\pensel.h"\
	".\PenStyle.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PenSel.obj" : $(SOURCE) $(DEP_CPP_PENSE) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\penstyle.cpp
DEP_CPP_PENST=\
	".\DCExplorer.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\is95.h"\
	".\PenStyle.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\penstyle.obj" : $(SOURCE) $(DEP_CPP_PENST) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Poly.cpp
DEP_CPP_POLY_=\
	".\DCExplorer.h"\
	".\poly.h"\
	".\polydraw.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Poly.obj" : $(SOURCE) $(DEP_CPP_POLY_) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Polydraw.cpp
DEP_CPP_POLYD=\
	".\polydraw.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Polydraw.obj" : $(SOURCE) $(DEP_CPP_POLYD) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Rop2.cpp
DEP_CPP_ROP2_=\
	".\DCExplorer.h"\
	".\idcombo.h"\
	".\rop2.h"\
	".\ropcodes.h"\
	".\sample.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\Rop2.obj" : $(SOURCE) $(DEP_CPP_ROP2_) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RopCodes.cpp
DEP_CPP_ROPCO=\
	".\DCExplorer.h"\
	".\idcombo.h"\
	".\ropcodes.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RopCodes.obj" : $(SOURCE) $(DEP_CPP_ROPCO) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sample.cpp
DEP_CPP_SAMPL=\
	".\DCExplorer.h"\
	".\idcombo.h"\
	".\sample.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\Sample.obj" : $(SOURCE) $(DEP_CPP_SAMPL) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\scombo.cpp
DEP_CPP_SCOMB=\
	".\DCExplorer.h"\
	".\scombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\scombo.obj" : $(SOURCE) $(DEP_CPP_SCOMB) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Stock.cpp
DEP_CPP_STOCK=\
	".\DCExplorer.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	".\stock.h"\
	

"$(INTDIR)\Stock.obj" : $(SOURCE) $(DEP_CPP_STOCK) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysColor.cpp
DEP_CPP_SYSCO=\
	".\color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	".\SysColor.h"\
	

"$(INTDIR)\SysColor.obj" : $(SOURCE) $(DEP_CPP_SYSCO) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sysmsg.cpp
DEP_CPP_SYSMS=\
	".\Message.h"\
	".\StdAfx.h"\
	".\SysMsg.h"\
	

"$(INTDIR)\Sysmsg.obj" : $(SOURCE) $(DEP_CPP_SYSMS) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Text.cpp
DEP_CPP_TEXT_=\
	".\color.h"\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\intedit.h"\
	".\StdAfx.h"\
	".\text.h"\
	

"$(INTDIR)\Text.obj" : $(SOURCE) $(DEP_CPP_TEXT_) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.CPP
DEP_CPP_ABOUT=\
	".\about.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IntEdit.CPP
DEP_CPP_INTED=\
	".\DCExplorer.h"\
	".\intedit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\IntEdit.obj" : $(SOURCE) $(DEP_CPP_INTED) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Hatch.cpp
DEP_CPP_HATCH=\
	".\DCExplorer.h"\
	".\hatch.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Hatch.obj" : $(SOURCE) $(DEP_CPP_HATCH) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GCombo.CPP
DEP_CPP_GCOMB=\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\GCombo.obj" : $(SOURCE) $(DEP_CPP_GCOMB) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mapping.cpp
DEP_CPP_MAPPI=\
	".\DCExplorer.h"\
	".\dcvals.h"\
	".\fltedit.h"\
	".\idcombo.h"\
	".\intedit.h"\
	".\mapping.h"\
	".\mapsamp.h"\
	".\Message.h"\
	".\StdAfx.h"\
	".\SysMsg.h"\
	".\uwm.h"\
	

"$(INTDIR)\Mapping.obj" : $(SOURCE) $(DEP_CPP_MAPPI) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HatchSmp.cpp
DEP_CPP_HATCHS=\
	".\DCExplorer.h"\
	".\hatchsmp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HatchSmp.obj" : $(SOURCE) $(DEP_CPP_HATCHS) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IDCombo.cpp
DEP_CPP_IDCOM=\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\IDCombo.obj" : $(SOURCE) $(DEP_CPP_IDCOM) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\LineJoin.CPP

!IF  "$(CFG)" == "DC Explorer - Win32 Release"

DEP_CPP_LINEJ=\
	".\DCExplorer.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\LineJoin.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LineJoin.obj" : $(SOURCE) $(DEP_CPP_LINEJ) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


!ELSEIF  "$(CFG)" == "DC Explorer - Win32 Debug"

DEP_CPP_LINEJ=\
	".\DCExplorer.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\LineJoin.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\LineJoin.obj" : $(SOURCE) $(DEP_CPP_LINEJ) "$(INTDIR)"\
 "$(INTDIR)\DCExplorer.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
