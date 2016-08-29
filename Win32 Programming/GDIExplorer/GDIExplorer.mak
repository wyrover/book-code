# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=GDI Explorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to GDI Explorer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "GDI Explorer - Win32 Release" && "$(CFG)" !=\
 "GDI Explorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "GDIExplorer.mak" CFG="GDI Explorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GDI Explorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GDI Explorer - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "GDI Explorer - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "GDI Explorer - Win32 Release"

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

ALL : "$(OUTDIR)\GDIExplorer.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\BB.obj"
	-@erase "$(INTDIR)\BitBlt.obj"
	-@erase "$(INTDIR)\bitrop.obj"
	-@erase "$(INTDIR)\Blt.obj"
	-@erase "$(INTDIR)\bmselect.obj"
	-@erase "$(INTDIR)\BMW.obj"
	-@erase "$(INTDIR)\Box.obj"
	-@erase "$(INTDIR)\BrDemo.obj"
	-@erase "$(INTDIR)\BrStyle.obj"
	-@erase "$(INTDIR)\BrushSel.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\dest.obj"
	-@erase "$(INTDIR)\dibits.obj"
	-@erase "$(INTDIR)\DrawEdge.obj"
	-@erase "$(INTDIR)\DrwButt.obj"
	-@erase "$(INTDIR)\EdgeSamp.obj"
	-@erase "$(INTDIR)\GCombo.obj"
	-@erase "$(INTDIR)\GDIExplorer.obj"
	-@erase "$(INTDIR)\GDIExplorer.pch"
	-@erase "$(INTDIR)\GDIExplorer.res"
	-@erase "$(INTDIR)\GDIExplorerDoc.obj"
	-@erase "$(INTDIR)\GDIExplorerView.obj"
	-@erase "$(INTDIR)\GenBlt.obj"
	-@erase "$(INTDIR)\HatchSmp.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\Image.obj"
	-@erase "$(INTDIR)\IntEdit.obj"
	-@erase "$(INTDIR)\IS95.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDisp.obj"
	-@erase "$(INTDIR)\MFH.obj"
	-@erase "$(INTDIR)\MskBlt.obj"
	-@erase "$(INTDIR)\PatBlt.obj"
	-@erase "$(INTDIR)\PatRop.obj"
	-@erase "$(INTDIR)\PlgBlt.obj"
	-@erase "$(INTDIR)\PolyPt.obj"
	-@erase "$(INTDIR)\random.obj"
	-@erase "$(INTDIR)\RgnCont.obj"
	-@erase "$(INTDIR)\RgnDisp.obj"
	-@erase "$(INTDIR)\RgnInt.obj"
	-@erase "$(INTDIR)\RopFind.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\ShowRgn.obj"
	-@erase "$(INTDIR)\Source.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Stock.obj"
	-@erase "$(INTDIR)\StrBlt.obj"
	-@erase "$(INTDIR)\StrMode.obj"
	-@erase "$(INTDIR)\SysColor.obj"
	-@erase "$(INTDIR)\transform.obj"
	-@erase "$(INTDIR)\Xform.obj"
	-@erase "$(OUTDIR)\GDIExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/GDIExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/GDIExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/GDIExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/GDIExplorer.pdb" /machine:I386 /out:"$(OUTDIR)/GDIExplorer.exe"\
 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\BB.obj" \
	"$(INTDIR)\BitBlt.obj" \
	"$(INTDIR)\bitrop.obj" \
	"$(INTDIR)\Blt.obj" \
	"$(INTDIR)\bmselect.obj" \
	"$(INTDIR)\BMW.obj" \
	"$(INTDIR)\Box.obj" \
	"$(INTDIR)\BrDemo.obj" \
	"$(INTDIR)\BrStyle.obj" \
	"$(INTDIR)\BrushSel.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\dest.obj" \
	"$(INTDIR)\dibits.obj" \
	"$(INTDIR)\DrawEdge.obj" \
	"$(INTDIR)\DrwButt.obj" \
	"$(INTDIR)\EdgeSamp.obj" \
	"$(INTDIR)\GCombo.obj" \
	"$(INTDIR)\GDIExplorer.obj" \
	"$(INTDIR)\GDIExplorer.res" \
	"$(INTDIR)\GDIExplorerDoc.obj" \
	"$(INTDIR)\GDIExplorerView.obj" \
	"$(INTDIR)\GenBlt.obj" \
	"$(INTDIR)\HatchSmp.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\Image.obj" \
	"$(INTDIR)\IntEdit.obj" \
	"$(INTDIR)\IS95.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDisp.obj" \
	"$(INTDIR)\MFH.obj" \
	"$(INTDIR)\MskBlt.obj" \
	"$(INTDIR)\PatBlt.obj" \
	"$(INTDIR)\PatRop.obj" \
	"$(INTDIR)\PlgBlt.obj" \
	"$(INTDIR)\PolyPt.obj" \
	"$(INTDIR)\random.obj" \
	"$(INTDIR)\RgnCont.obj" \
	"$(INTDIR)\RgnDisp.obj" \
	"$(INTDIR)\RgnInt.obj" \
	"$(INTDIR)\RopFind.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\ShowRgn.obj" \
	"$(INTDIR)\Source.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Stock.obj" \
	"$(INTDIR)\StrBlt.obj" \
	"$(INTDIR)\StrMode.obj" \
	"$(INTDIR)\SysColor.obj" \
	"$(INTDIR)\transform.obj" \
	"$(INTDIR)\Xform.obj"

"$(OUTDIR)\GDIExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GDI Explorer - Win32 Debug"

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

ALL : "$(OUTDIR)\GDIExplorer.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\BB.obj"
	-@erase "$(INTDIR)\BitBlt.obj"
	-@erase "$(INTDIR)\bitrop.obj"
	-@erase "$(INTDIR)\Blt.obj"
	-@erase "$(INTDIR)\bmselect.obj"
	-@erase "$(INTDIR)\BMW.obj"
	-@erase "$(INTDIR)\Box.obj"
	-@erase "$(INTDIR)\BrDemo.obj"
	-@erase "$(INTDIR)\BrStyle.obj"
	-@erase "$(INTDIR)\BrushSel.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\dest.obj"
	-@erase "$(INTDIR)\dibits.obj"
	-@erase "$(INTDIR)\DrawEdge.obj"
	-@erase "$(INTDIR)\DrwButt.obj"
	-@erase "$(INTDIR)\EdgeSamp.obj"
	-@erase "$(INTDIR)\GCombo.obj"
	-@erase "$(INTDIR)\GDIExplorer.obj"
	-@erase "$(INTDIR)\GDIExplorer.pch"
	-@erase "$(INTDIR)\GDIExplorer.res"
	-@erase "$(INTDIR)\GDIExplorerDoc.obj"
	-@erase "$(INTDIR)\GDIExplorerView.obj"
	-@erase "$(INTDIR)\GenBlt.obj"
	-@erase "$(INTDIR)\HatchSmp.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\Image.obj"
	-@erase "$(INTDIR)\IntEdit.obj"
	-@erase "$(INTDIR)\IS95.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MDisp.obj"
	-@erase "$(INTDIR)\MFH.obj"
	-@erase "$(INTDIR)\MskBlt.obj"
	-@erase "$(INTDIR)\PatBlt.obj"
	-@erase "$(INTDIR)\PatRop.obj"
	-@erase "$(INTDIR)\PlgBlt.obj"
	-@erase "$(INTDIR)\PolyPt.obj"
	-@erase "$(INTDIR)\random.obj"
	-@erase "$(INTDIR)\RgnCont.obj"
	-@erase "$(INTDIR)\RgnDisp.obj"
	-@erase "$(INTDIR)\RgnInt.obj"
	-@erase "$(INTDIR)\RopFind.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\ShowRgn.obj"
	-@erase "$(INTDIR)\Source.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Stock.obj"
	-@erase "$(INTDIR)\StrBlt.obj"
	-@erase "$(INTDIR)\StrMode.obj"
	-@erase "$(INTDIR)\SysColor.obj"
	-@erase "$(INTDIR)\transform.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Xform.obj"
	-@erase "$(OUTDIR)\GDIExplorer.exe"
	-@erase "$(OUTDIR)\GDIExplorer.ilk"
	-@erase "$(OUTDIR)\GDIExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/GDIExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/GDIExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/GDIExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/GDIExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/GDIExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\BB.obj" \
	"$(INTDIR)\BitBlt.obj" \
	"$(INTDIR)\bitrop.obj" \
	"$(INTDIR)\Blt.obj" \
	"$(INTDIR)\bmselect.obj" \
	"$(INTDIR)\BMW.obj" \
	"$(INTDIR)\Box.obj" \
	"$(INTDIR)\BrDemo.obj" \
	"$(INTDIR)\BrStyle.obj" \
	"$(INTDIR)\BrushSel.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\dest.obj" \
	"$(INTDIR)\dibits.obj" \
	"$(INTDIR)\DrawEdge.obj" \
	"$(INTDIR)\DrwButt.obj" \
	"$(INTDIR)\EdgeSamp.obj" \
	"$(INTDIR)\GCombo.obj" \
	"$(INTDIR)\GDIExplorer.obj" \
	"$(INTDIR)\GDIExplorer.res" \
	"$(INTDIR)\GDIExplorerDoc.obj" \
	"$(INTDIR)\GDIExplorerView.obj" \
	"$(INTDIR)\GenBlt.obj" \
	"$(INTDIR)\HatchSmp.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\Image.obj" \
	"$(INTDIR)\IntEdit.obj" \
	"$(INTDIR)\IS95.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MDisp.obj" \
	"$(INTDIR)\MFH.obj" \
	"$(INTDIR)\MskBlt.obj" \
	"$(INTDIR)\PatBlt.obj" \
	"$(INTDIR)\PatRop.obj" \
	"$(INTDIR)\PlgBlt.obj" \
	"$(INTDIR)\PolyPt.obj" \
	"$(INTDIR)\random.obj" \
	"$(INTDIR)\RgnCont.obj" \
	"$(INTDIR)\RgnDisp.obj" \
	"$(INTDIR)\RgnInt.obj" \
	"$(INTDIR)\RopFind.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\ShowRgn.obj" \
	"$(INTDIR)\Source.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Stock.obj" \
	"$(INTDIR)\StrBlt.obj" \
	"$(INTDIR)\StrMode.obj" \
	"$(INTDIR)\SysColor.obj" \
	"$(INTDIR)\transform.obj" \
	"$(INTDIR)\Xform.obj"

"$(OUTDIR)\GDIExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "GDI Explorer - Win32 Release"
# Name "GDI Explorer - Win32 Debug"

!IF  "$(CFG)" == "GDI Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "GDI Explorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "GDI Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "GDI Explorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "GDI Explorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/GDIExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\GDIExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "GDI Explorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/GDIExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\GDIExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\GDIExplorer.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "GDI Explorer - Win32 Release"

DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	
NODEP_CPP_CHILD=\
	".\uwm.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


!ELSEIF  "$(CFG)" == "GDI Explorer - Win32 Debug"

DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BB.cpp
DEP_CPP_BB_CP=\
	".\bb.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BB.obj" : $(SOURCE) $(DEP_CPP_BB_CP) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BitBlt.cpp
DEP_CPP_BITBL=\
	".\BitBlt.h"\
	".\BitRop.h"\
	".\blt.h"\
	".\bmselect.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\strmode.h"\
	

"$(INTDIR)\BitBlt.obj" : $(SOURCE) $(DEP_CPP_BITBL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\bitrop.cpp
DEP_CPP_BITRO=\
	".\BitRop.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\bitrop.obj" : $(SOURCE) $(DEP_CPP_BITRO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Blt.cpp
DEP_CPP_BLT_C=\
	".\blt.h"\
	".\bmselect.h"\
	".\bmw.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\extrops.h"\
	".\GDIExplorer.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\strmode.h"\
	

"$(INTDIR)\Blt.obj" : $(SOURCE) $(DEP_CPP_BLT_C) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\bmselect.cpp
DEP_CPP_BMSEL=\
	".\bmselect.h"\
	".\GDIExplorer.h"\
	".\image.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\bmselect.obj" : $(SOURCE) $(DEP_CPP_BMSEL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BMW.cpp
DEP_CPP_BMW_C=\
	".\StdAfx.h"\
	

"$(INTDIR)\BMW.obj" : $(SOURCE) $(DEP_CPP_BMW_C) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Box.cpp
DEP_CPP_BOX_C=\
	".\Box.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Box.obj" : $(SOURCE) $(DEP_CPP_BOX_C) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BrDemo.cpp
DEP_CPP_BRDEM=\
	".\brdemo.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BrDemo.obj" : $(SOURCE) $(DEP_CPP_BRDEM) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BrStyle.cpp
DEP_CPP_BRSTY=\
	".\BrStyle.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\BrStyle.obj" : $(SOURCE) $(DEP_CPP_BRSTY) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BrushSel.cpp
DEP_CPP_BRUSH=\
	".\BrStyle.h"\
	".\BrushSel.h"\
	".\color.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\hatchsmp.h"\
	".\idcombo.h"\
	".\IntEdit.h"\
	".\is95.h"\
	".\StdAfx.h"\
	".\stock.h"\
	".\syscolor.h"\
	

"$(INTDIR)\BrushSel.obj" : $(SOURCE) $(DEP_CPP_BRUSH) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


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
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Copy.cpp
DEP_CPP_COPY_=\
	".\copy.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Copy.obj" : $(SOURCE) $(DEP_CPP_COPY_) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dest.cpp
DEP_CPP_DEST_=\
	".\dest.h"\
	".\GDIExplorer.h"\
	".\source.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\dest.obj" : $(SOURCE) $(DEP_CPP_DEST_) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dibits.cpp
DEP_CPP_DIBIT=\
	".\bmselect.h"\
	".\DIBits.h"\
	".\GDIExplorer.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\source.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\dibits.obj" : $(SOURCE) $(DEP_CPP_DIBIT) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrawEdge.cpp
DEP_CPP_DRAWE=\
	".\DrawEdge.h"\
	".\drwbutt.h"\
	".\EdgeSamp.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DrawEdge.obj" : $(SOURCE) $(DEP_CPP_DRAWE) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DrwButt.cpp
DEP_CPP_DRWBU=\
	".\drwbutt.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DrwButt.obj" : $(SOURCE) $(DEP_CPP_DRWBU) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EdgeSamp.cpp
DEP_CPP_EDGES=\
	".\EdgeSamp.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\EdgeSamp.obj" : $(SOURCE) $(DEP_CPP_EDGES) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GCombo.cpp
DEP_CPP_GCOMB=\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\GCombo.obj" : $(SOURCE) $(DEP_CPP_GCOMB) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GDIExplorer.cpp
DEP_CPP_GDIEX=\
	".\about.h"\
	".\BitBlt.h"\
	".\BitRop.h"\
	".\blt.h"\
	".\bmselect.h"\
	".\Box.h"\
	".\brdemo.h"\
	".\BrStyle.h"\
	".\BrushSel.h"\
	".\ChildFrm.h"\
	".\color.h"\
	".\ctl.h"\
	".\dest.h"\
	".\DIBits.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\GDIExplorerDoc.h"\
	".\GDIExplorerView.h"\
	".\genblt.h"\
	".\hatchsmp.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\is95.h"\
	".\MainFrm.h"\
	".\mdisp.h"\
	".\mskblt.h"\
	".\orgext.h"\
	".\patblt.h"\
	".\patrop.h"\
	".\plgblt.h"\
	".\polypoly.h"\
	".\randstat.h"\
	".\rgncont.h"\
	".\rgndisp.h"\
	".\rgnint.h"\
	".\ropfind.h"\
	".\showrgn.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\stock.h"\
	".\strblt.h"\
	".\strmode.h"\
	".\syscolor.h"\
	".\transform.h"\
	".\xform.h"\
	

"$(INTDIR)\GDIExplorer.obj" : $(SOURCE) $(DEP_CPP_GDIEX) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GDIExplorer.rc
DEP_RSC_GDIEXP=\
	".\res\arc.ico"\
	".\res\arcto.ico"\
	".\res\bezier.ico"\
	".\res\bitmap2.bmp"\
	".\res\bmpbrush.ico"\
	".\res\chap6.ico"\
	".\res\chord.ico"\
	".\res\clip1.ico"\
	".\res\clip2.ico"\
	".\res\clip3.ico"\
	".\res\complex.ico"\
	".\res\desert.bmp"\
	".\res\desert1.bmp"\
	".\res\ellipse.ico"\
	".\res\endcaps.ico"\
	".\res\focus.ico"\
	".\res\GDIExplorer.rc2"\
	".\res\GDIExplorerDoc.ico"\
	".\res\geopen.ico"\
	".\res\graded.bmp"\
	".\res\graded1.bmp"\
	".\res\hatch.ico"\
	".\res\hstripes.bmp"\
	".\res\jmnco.ico"\
	".\res\linejoin.ico"\
	".\res\mainfram.bmp"\
	".\res\marble.bmp"\
	".\res\mask1.bmp"\
	".\res\mask2.bmp"\
	".\res\mask3.bmp"\
	".\res\mask4.bmp"\
	".\res\penstyle.ico"\
	".\res\pie.ico"\
	".\res\plaid.bmp"\
	".\res\poly.ico"\
	".\res\polypoly.ico"\
	".\res\random.ico"\
	".\res\rect.ico"\
	".\res\redbrick.bmp"\
	".\res\rgb1.bmp"\
	".\res\rgb2.bmp"\
	".\res\rgb3.bmp"\
	".\res\rgb4.bmp"\
	".\res\rgb5.bmp"\
	".\res\rgb6.bmp"\
	".\res\rrect.ico"\
	".\res\saf.ico"\
	".\res\star.bmp"\
	".\res\txform.ico"\
	".\res\xform.ico"\
	

"$(INTDIR)\GDIExplorer.res" : $(SOURCE) $(DEP_RSC_GDIEXP) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GDIExplorerDoc.cpp
DEP_CPP_GDIEXPL=\
	".\GDIExplorer.h"\
	".\GDIExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\GDIExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_GDIEXPL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GDIExplorerView.cpp
DEP_CPP_GDIEXPLO=\
	".\bb.h"\
	".\copy.h"\
	".\GDIExplorer.h"\
	".\GDIExplorerDoc.h"\
	".\GDIExplorerView.h"\
	".\is95.h"\
	".\MainFrm.h"\
	".\MFH.h"\
	".\polypoly.h"\
	".\polypt.h"\
	".\randstat.h"\
	".\select.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	".\wmf.h"\
	

"$(INTDIR)\GDIExplorerView.obj" : $(SOURCE) $(DEP_CPP_GDIEXPLO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GenBlt.cpp
DEP_CPP_GENBL=\
	".\blt.h"\
	".\bmselect.h"\
	".\bmw.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\genblt.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\is95.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\strmode.h"\
	

"$(INTDIR)\GenBlt.obj" : $(SOURCE) $(DEP_CPP_GENBL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HatchSmp.cpp
DEP_CPP_HATCH=\
	".\hatchsmp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HatchSmp.obj" : $(SOURCE) $(DEP_CPP_HATCH) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IDCombo.cpp
DEP_CPP_IDCOM=\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\IDCombo.obj" : $(SOURCE) $(DEP_CPP_IDCOM) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Image.cpp
DEP_CPP_IMAGE=\
	".\image.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Image.obj" : $(SOURCE) $(DEP_CPP_IMAGE) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IntEdit.cpp
DEP_CPP_INTED=\
	".\IntEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\IntEdit.obj" : $(SOURCE) $(DEP_CPP_INTED) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IS95.cpp
DEP_CPP_IS95_=\
	".\StdAfx.h"\
	

"$(INTDIR)\IS95.obj" : $(SOURCE) $(DEP_CPP_IS95_) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MDisp.cpp
DEP_CPP_MDISP=\
	".\GDIExplorer.h"\
	".\mdisp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MDisp.obj" : $(SOURCE) $(DEP_CPP_MDISP) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MFH.cpp
DEP_CPP_MFH_C=\
	".\GDIExplorer.h"\
	".\MFH.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MFH.obj" : $(SOURCE) $(DEP_CPP_MFH_C) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MskBlt.cpp
DEP_CPP_MSKBL=\
	".\BitRop.h"\
	".\blt.h"\
	".\bmselect.h"\
	".\bmw.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\genblt.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\mskblt.h"\
	".\orgext.h"\
	".\source.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MskBlt.obj" : $(SOURCE) $(DEP_CPP_MSKBL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PatBlt.cpp
DEP_CPP_PATBL=\
	".\blt.h"\
	".\bmselect.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\patblt.h"\
	".\patrop.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\strmode.h"\
	

"$(INTDIR)\PatBlt.obj" : $(SOURCE) $(DEP_CPP_PATBL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PatRop.cpp
DEP_CPP_PATRO=\
	".\idcombo.h"\
	".\patrop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PatRop.obj" : $(SOURCE) $(DEP_CPP_PATRO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PlgBlt.cpp
DEP_CPP_PLGBL=\
	".\blt.h"\
	".\bmselect.h"\
	".\bmw.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\genblt.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\plgblt.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\strmode.h"\
	

"$(INTDIR)\PlgBlt.obj" : $(SOURCE) $(DEP_CPP_PLGBL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PolyPt.cpp
DEP_CPP_POLYP=\
	".\GDIExplorer.h"\
	".\polypoly.h"\
	".\polypt.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PolyPt.obj" : $(SOURCE) $(DEP_CPP_POLYP) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\random.cpp
DEP_CPP_RANDO=\
	".\GDIExplorer.h"\
	".\random.h"\
	".\randstat.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\random.obj" : $(SOURCE) $(DEP_CPP_RANDO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\about.h"\
	".\GDIExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\GDIExplorer.hpj

!IF  "$(CFG)" == "GDI Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "GDI Explorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\transform.cpp
DEP_CPP_TRANS=\
	".\ctl.h"\
	".\GDIExplorer.h"\
	".\mdisp.h"\
	".\StdAfx.h"\
	".\transform.h"\
	".\xform.h"\
	

"$(INTDIR)\transform.obj" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Stock.cpp
DEP_CPP_STOCK=\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	".\stock.h"\
	

"$(INTDIR)\Stock.obj" : $(SOURCE) $(DEP_CPP_STOCK) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RopFind.cpp
DEP_CPP_ROPFI=\
	".\BitRop.h"\
	".\Box.h"\
	".\idcombo.h"\
	".\ropfind.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RopFind.obj" : $(SOURCE) $(DEP_CPP_ROPFI) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RgnDisp.cpp
DEP_CPP_RGNDI=\
	".\GDIExplorer.h"\
	".\rgndisp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RgnDisp.obj" : $(SOURCE) $(DEP_CPP_RGNDI) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RgnInt.CPP
DEP_CPP_RGNIN=\
	".\GDIExplorer.h"\
	".\rgndisp.h"\
	".\rgnint.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RgnInt.obj" : $(SOURCE) $(DEP_CPP_RGNIN) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RgnCont.cpp
DEP_CPP_RGNCO=\
	".\GDIExplorer.h"\
	".\rgncont.h"\
	".\rgndisp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RgnCont.obj" : $(SOURCE) $(DEP_CPP_RGNCO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ShowRgn.cpp
DEP_CPP_SHOWR=\
	".\GDIExplorer.h"\
	".\rgncont.h"\
	".\rgndisp.h"\
	".\rgnint.h"\
	".\showrgn.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ShowRgn.obj" : $(SOURCE) $(DEP_CPP_SHOWR) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Source.cpp
DEP_CPP_SOURC=\
	".\bmw.h"\
	".\GDIExplorer.h"\
	".\source.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Source.obj" : $(SOURCE) $(DEP_CPP_SOURC) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysColor.cpp
DEP_CPP_SYSCO=\
	".\color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	".\syscolor.h"\
	

"$(INTDIR)\SysColor.obj" : $(SOURCE) $(DEP_CPP_SYSCO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StrBlt.CPP
DEP_CPP_STRBL=\
	".\BitRop.h"\
	".\blt.h"\
	".\bmselect.h"\
	".\bmw.h"\
	".\brdemo.h"\
	".\dest.h"\
	".\extrops.h"\
	".\gcombo.h"\
	".\GDIExplorer.h"\
	".\genblt.h"\
	".\idcombo.h"\
	".\image.h"\
	".\IntEdit.h"\
	".\orgext.h"\
	".\source.h"\
	".\StdAfx.h"\
	".\strblt.h"\
	".\strmode.h"\
	

"$(INTDIR)\StrBlt.obj" : $(SOURCE) $(DEP_CPP_STRBL) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\select.cpp
DEP_CPP_SELEC=\
	".\GDIExplorer.h"\
	".\is95.h"\
	".\random.h"\
	".\randstat.h"\
	".\select.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\select.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Xform.cpp
DEP_CPP_XFORM=\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\Xform.obj" : $(SOURCE) $(DEP_CPP_XFORM) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StrMode.CPP
DEP_CPP_STRMO=\
	".\idcombo.h"\
	".\StdAfx.h"\
	".\strmode.h"\
	

"$(INTDIR)\StrMode.obj" : $(SOURCE) $(DEP_CPP_STRMO) "$(INTDIR)"\
 "$(INTDIR)\GDIExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
