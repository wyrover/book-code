# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=ControlExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ControlExplorer - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "ControlExplorer - Win32 Release" && "$(CFG)" !=\
 "ControlExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ControlExplorer.mak" CFG="ControlExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ControlExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ControlExplorer - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "ControlExplorer - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "ControlExplorer - Win32 Release"

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

ALL : "$(OUTDIR)\ControlExplorer.exe" "$(OUTDIR)\ControlExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\3D.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Animate.obj"
	-@erase "$(INTDIR)\Bitcombo.obj"
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\bmselect.obj"
	-@erase "$(INTDIR)\Botany.obj"
	-@erase "$(INTDIR)\BtnMsg.obj"
	-@erase "$(INTDIR)\ButtTest.obj"
	-@erase "$(INTDIR)\Callback.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Comboops.obj"
	-@erase "$(INTDIR)\ControlExplorer.hlp"
	-@erase "$(INTDIR)\ControlExplorer.obj"
	-@erase "$(INTDIR)\ControlExplorer.pch"
	-@erase "$(INTDIR)\ControlExplorer.res"
	-@erase "$(INTDIR)\ControlExplorerDoc.obj"
	-@erase "$(INTDIR)\ControlExplorerView.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\Ddndx.obj"
	-@erase "$(INTDIR)\DDState.obj"
	-@erase "$(INTDIR)\DDText.obj"
	-@erase "$(INTDIR)\Demo.obj"
	-@erase "$(INTDIR)\DlgCode.obj"
	-@erase "$(INTDIR)\DlgEdit.obj"
	-@erase "$(INTDIR)\Drawing.obj"
	-@erase "$(INTDIR)\DropDown.obj"
	-@erase "$(INTDIR)\EditCont.obj"
	-@erase "$(INTDIR)\EditMap.obj"
	-@erase "$(INTDIR)\Editndx.obj"
	-@erase "$(INTDIR)\EditOps.obj"
	-@erase "$(INTDIR)\EditRect.obj"
	-@erase "$(INTDIR)\EditScrl.obj"
	-@erase "$(INTDIR)\EditStat.obj"
	-@erase "$(INTDIR)\EditTabs.obj"
	-@erase "$(INTDIR)\EditTest.obj"
	-@erase "$(INTDIR)\Effects.obj"
	-@erase "$(INTDIR)\HdrLayout.obj"
	-@erase "$(INTDIR)\HdrMsg.obj"
	-@erase "$(INTDIR)\Header.obj"
	-@erase "$(INTDIR)\HexDisp.obj"
	-@erase "$(INTDIR)\Image.obj"
	-@erase "$(INTDIR)\Indexpg.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\ixable.obj"
	-@erase "$(INTDIR)\listbox.obj"
	-@erase "$(INTDIR)\ListDir.obj"
	-@erase "$(INTDIR)\Listndx.obj"
	-@erase "$(INTDIR)\Listops.obj"
	-@erase "$(INTDIR)\ListQ.obj"
	-@erase "$(INTDIR)\ListRect.obj"
	-@erase "$(INTDIR)\ListStat.obj"
	-@erase "$(INTDIR)\ListStr.obj"
	-@erase "$(INTDIR)\ListTabs.obj"
	-@erase "$(INTDIR)\Locales.obj"
	-@erase "$(INTDIR)\Loggable.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MsgLog.obj"
	-@erase "$(INTDIR)\MsgSheet.obj"
	-@erase "$(INTDIR)\MyButton.obj"
	-@erase "$(INTDIR)\Mystatic.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\NumList.obj"
	-@erase "$(INTDIR)\Progress.obj"
	-@erase "$(INTDIR)\RecEdit.obj"
	-@erase "$(INTDIR)\Resize.obj"
	-@erase "$(INTDIR)\ScParent.obj"
	-@erase "$(INTDIR)\scrltest.obj"
	-@erase "$(INTDIR)\sdemo.obj"
	-@erase "$(INTDIR)\StatTest.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Ticks.obj"
	-@erase "$(INTDIR)\TinyButt.obj"
	-@erase "$(INTDIR)\TrackBar.obj"
	-@erase "$(INTDIR)\tracklog.obj"
	-@erase "$(INTDIR)\Trackndx.obj"
	-@erase "$(INTDIR)\Trackops.obj"
	-@erase "$(INTDIR)\TrackPos.obj"
	-@erase "$(INTDIR)\TrackRc.obj"
	-@erase "$(INTDIR)\TrackSel.obj"
	-@erase "$(INTDIR)\Tree.obj"
	-@erase "$(INTDIR)\TreeGetItem.obj"
	-@erase "$(INTDIR)\TreeIns.obj"
	-@erase "$(INTDIR)\TreeMisc.obj"
	-@erase "$(INTDIR)\Treendx.obj"
	-@erase "$(INTDIR)\Treeops.obj"
	-@erase "$(INTDIR)\TVImgLst.obj"
	-@erase "$(INTDIR)\Tvitem.obj"
	-@erase "$(INTDIR)\UDAccel.obj"
	-@erase "$(INTDIR)\UDBuddy.obj"
	-@erase "$(INTDIR)\Udops.obj"
	-@erase "$(INTDIR)\UDPos.obj"
	-@erase "$(INTDIR)\UDZ.obj"
	-@erase "$(INTDIR)\UpDown.obj"
	-@erase "$(INTDIR)\Xform.obj"
	-@erase "$(OUTDIR)\ControlExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ControlExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ControlExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ControlExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ControlExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/ControlExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\3D.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Animate.obj" \
	"$(INTDIR)\Bitcombo.obj" \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\bmselect.obj" \
	"$(INTDIR)\Botany.obj" \
	"$(INTDIR)\BtnMsg.obj" \
	"$(INTDIR)\ButtTest.obj" \
	"$(INTDIR)\Callback.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Comboops.obj" \
	"$(INTDIR)\ControlExplorer.obj" \
	"$(INTDIR)\ControlExplorer.res" \
	"$(INTDIR)\ControlExplorerDoc.obj" \
	"$(INTDIR)\ControlExplorerView.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\Ddndx.obj" \
	"$(INTDIR)\DDState.obj" \
	"$(INTDIR)\DDText.obj" \
	"$(INTDIR)\Demo.obj" \
	"$(INTDIR)\DlgCode.obj" \
	"$(INTDIR)\DlgEdit.obj" \
	"$(INTDIR)\Drawing.obj" \
	"$(INTDIR)\DropDown.obj" \
	"$(INTDIR)\EditCont.obj" \
	"$(INTDIR)\EditMap.obj" \
	"$(INTDIR)\Editndx.obj" \
	"$(INTDIR)\EditOps.obj" \
	"$(INTDIR)\EditRect.obj" \
	"$(INTDIR)\EditScrl.obj" \
	"$(INTDIR)\EditStat.obj" \
	"$(INTDIR)\EditTabs.obj" \
	"$(INTDIR)\EditTest.obj" \
	"$(INTDIR)\Effects.obj" \
	"$(INTDIR)\HdrLayout.obj" \
	"$(INTDIR)\HdrMsg.obj" \
	"$(INTDIR)\Header.obj" \
	"$(INTDIR)\HexDisp.obj" \
	"$(INTDIR)\Image.obj" \
	"$(INTDIR)\Indexpg.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\ixable.obj" \
	"$(INTDIR)\listbox.obj" \
	"$(INTDIR)\ListDir.obj" \
	"$(INTDIR)\Listndx.obj" \
	"$(INTDIR)\Listops.obj" \
	"$(INTDIR)\ListQ.obj" \
	"$(INTDIR)\ListRect.obj" \
	"$(INTDIR)\ListStat.obj" \
	"$(INTDIR)\ListStr.obj" \
	"$(INTDIR)\ListTabs.obj" \
	"$(INTDIR)\Locales.obj" \
	"$(INTDIR)\Loggable.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MsgLog.obj" \
	"$(INTDIR)\MsgSheet.obj" \
	"$(INTDIR)\MyButton.obj" \
	"$(INTDIR)\Mystatic.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\NumList.obj" \
	"$(INTDIR)\Progress.obj" \
	"$(INTDIR)\RecEdit.obj" \
	"$(INTDIR)\Resize.obj" \
	"$(INTDIR)\ScParent.obj" \
	"$(INTDIR)\scrltest.obj" \
	"$(INTDIR)\sdemo.obj" \
	"$(INTDIR)\StatTest.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Ticks.obj" \
	"$(INTDIR)\TinyButt.obj" \
	"$(INTDIR)\TrackBar.obj" \
	"$(INTDIR)\tracklog.obj" \
	"$(INTDIR)\Trackndx.obj" \
	"$(INTDIR)\Trackops.obj" \
	"$(INTDIR)\TrackPos.obj" \
	"$(INTDIR)\TrackRc.obj" \
	"$(INTDIR)\TrackSel.obj" \
	"$(INTDIR)\Tree.obj" \
	"$(INTDIR)\TreeGetItem.obj" \
	"$(INTDIR)\TreeIns.obj" \
	"$(INTDIR)\TreeMisc.obj" \
	"$(INTDIR)\Treendx.obj" \
	"$(INTDIR)\Treeops.obj" \
	"$(INTDIR)\TVImgLst.obj" \
	"$(INTDIR)\Tvitem.obj" \
	"$(INTDIR)\UDAccel.obj" \
	"$(INTDIR)\UDBuddy.obj" \
	"$(INTDIR)\Udops.obj" \
	"$(INTDIR)\UDPos.obj" \
	"$(INTDIR)\UDZ.obj" \
	"$(INTDIR)\UpDown.obj" \
	"$(INTDIR)\Xform.obj"

"$(OUTDIR)\ControlExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ControlExplorer - Win32 Debug"

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

ALL : "$(OUTDIR)\ControlExplorer.exe" "$(OUTDIR)\ControlExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\3D.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Animate.obj"
	-@erase "$(INTDIR)\Bitcombo.obj"
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\bmselect.obj"
	-@erase "$(INTDIR)\Botany.obj"
	-@erase "$(INTDIR)\BtnMsg.obj"
	-@erase "$(INTDIR)\ButtTest.obj"
	-@erase "$(INTDIR)\Callback.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Comboops.obj"
	-@erase "$(INTDIR)\ControlExplorer.hlp"
	-@erase "$(INTDIR)\ControlExplorer.obj"
	-@erase "$(INTDIR)\ControlExplorer.pch"
	-@erase "$(INTDIR)\ControlExplorer.res"
	-@erase "$(INTDIR)\ControlExplorerDoc.obj"
	-@erase "$(INTDIR)\ControlExplorerView.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\Ddndx.obj"
	-@erase "$(INTDIR)\DDState.obj"
	-@erase "$(INTDIR)\DDText.obj"
	-@erase "$(INTDIR)\Demo.obj"
	-@erase "$(INTDIR)\DlgCode.obj"
	-@erase "$(INTDIR)\DlgEdit.obj"
	-@erase "$(INTDIR)\Drawing.obj"
	-@erase "$(INTDIR)\DropDown.obj"
	-@erase "$(INTDIR)\EditCont.obj"
	-@erase "$(INTDIR)\EditMap.obj"
	-@erase "$(INTDIR)\Editndx.obj"
	-@erase "$(INTDIR)\EditOps.obj"
	-@erase "$(INTDIR)\EditRect.obj"
	-@erase "$(INTDIR)\EditScrl.obj"
	-@erase "$(INTDIR)\EditStat.obj"
	-@erase "$(INTDIR)\EditTabs.obj"
	-@erase "$(INTDIR)\EditTest.obj"
	-@erase "$(INTDIR)\Effects.obj"
	-@erase "$(INTDIR)\HdrLayout.obj"
	-@erase "$(INTDIR)\HdrMsg.obj"
	-@erase "$(INTDIR)\Header.obj"
	-@erase "$(INTDIR)\HexDisp.obj"
	-@erase "$(INTDIR)\Image.obj"
	-@erase "$(INTDIR)\Indexpg.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\ixable.obj"
	-@erase "$(INTDIR)\listbox.obj"
	-@erase "$(INTDIR)\ListDir.obj"
	-@erase "$(INTDIR)\Listndx.obj"
	-@erase "$(INTDIR)\Listops.obj"
	-@erase "$(INTDIR)\ListQ.obj"
	-@erase "$(INTDIR)\ListRect.obj"
	-@erase "$(INTDIR)\ListStat.obj"
	-@erase "$(INTDIR)\ListStr.obj"
	-@erase "$(INTDIR)\ListTabs.obj"
	-@erase "$(INTDIR)\Locales.obj"
	-@erase "$(INTDIR)\Loggable.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MsgLog.obj"
	-@erase "$(INTDIR)\MsgSheet.obj"
	-@erase "$(INTDIR)\MyButton.obj"
	-@erase "$(INTDIR)\Mystatic.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\NumList.obj"
	-@erase "$(INTDIR)\Progress.obj"
	-@erase "$(INTDIR)\RecEdit.obj"
	-@erase "$(INTDIR)\Resize.obj"
	-@erase "$(INTDIR)\ScParent.obj"
	-@erase "$(INTDIR)\scrltest.obj"
	-@erase "$(INTDIR)\sdemo.obj"
	-@erase "$(INTDIR)\StatTest.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Ticks.obj"
	-@erase "$(INTDIR)\TinyButt.obj"
	-@erase "$(INTDIR)\TrackBar.obj"
	-@erase "$(INTDIR)\tracklog.obj"
	-@erase "$(INTDIR)\Trackndx.obj"
	-@erase "$(INTDIR)\Trackops.obj"
	-@erase "$(INTDIR)\TrackPos.obj"
	-@erase "$(INTDIR)\TrackRc.obj"
	-@erase "$(INTDIR)\TrackSel.obj"
	-@erase "$(INTDIR)\Tree.obj"
	-@erase "$(INTDIR)\TreeGetItem.obj"
	-@erase "$(INTDIR)\TreeIns.obj"
	-@erase "$(INTDIR)\TreeMisc.obj"
	-@erase "$(INTDIR)\Treendx.obj"
	-@erase "$(INTDIR)\Treeops.obj"
	-@erase "$(INTDIR)\TVImgLst.obj"
	-@erase "$(INTDIR)\Tvitem.obj"
	-@erase "$(INTDIR)\UDAccel.obj"
	-@erase "$(INTDIR)\UDBuddy.obj"
	-@erase "$(INTDIR)\Udops.obj"
	-@erase "$(INTDIR)\UDPos.obj"
	-@erase "$(INTDIR)\UDZ.obj"
	-@erase "$(INTDIR)\UpDown.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Xform.obj"
	-@erase "$(OUTDIR)\ControlExplorer.exe"
	-@erase "$(OUTDIR)\ControlExplorer.ilk"
	-@erase "$(OUTDIR)\ControlExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ControlExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ControlExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ControlExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/ControlExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ControlExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\3D.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Animate.obj" \
	"$(INTDIR)\Bitcombo.obj" \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\bmselect.obj" \
	"$(INTDIR)\Botany.obj" \
	"$(INTDIR)\BtnMsg.obj" \
	"$(INTDIR)\ButtTest.obj" \
	"$(INTDIR)\Callback.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Comboops.obj" \
	"$(INTDIR)\ControlExplorer.obj" \
	"$(INTDIR)\ControlExplorer.res" \
	"$(INTDIR)\ControlExplorerDoc.obj" \
	"$(INTDIR)\ControlExplorerView.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\Ddndx.obj" \
	"$(INTDIR)\DDState.obj" \
	"$(INTDIR)\DDText.obj" \
	"$(INTDIR)\Demo.obj" \
	"$(INTDIR)\DlgCode.obj" \
	"$(INTDIR)\DlgEdit.obj" \
	"$(INTDIR)\Drawing.obj" \
	"$(INTDIR)\DropDown.obj" \
	"$(INTDIR)\EditCont.obj" \
	"$(INTDIR)\EditMap.obj" \
	"$(INTDIR)\Editndx.obj" \
	"$(INTDIR)\EditOps.obj" \
	"$(INTDIR)\EditRect.obj" \
	"$(INTDIR)\EditScrl.obj" \
	"$(INTDIR)\EditStat.obj" \
	"$(INTDIR)\EditTabs.obj" \
	"$(INTDIR)\EditTest.obj" \
	"$(INTDIR)\Effects.obj" \
	"$(INTDIR)\HdrLayout.obj" \
	"$(INTDIR)\HdrMsg.obj" \
	"$(INTDIR)\Header.obj" \
	"$(INTDIR)\HexDisp.obj" \
	"$(INTDIR)\Image.obj" \
	"$(INTDIR)\Indexpg.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\ixable.obj" \
	"$(INTDIR)\listbox.obj" \
	"$(INTDIR)\ListDir.obj" \
	"$(INTDIR)\Listndx.obj" \
	"$(INTDIR)\Listops.obj" \
	"$(INTDIR)\ListQ.obj" \
	"$(INTDIR)\ListRect.obj" \
	"$(INTDIR)\ListStat.obj" \
	"$(INTDIR)\ListStr.obj" \
	"$(INTDIR)\ListTabs.obj" \
	"$(INTDIR)\Locales.obj" \
	"$(INTDIR)\Loggable.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MsgLog.obj" \
	"$(INTDIR)\MsgSheet.obj" \
	"$(INTDIR)\MyButton.obj" \
	"$(INTDIR)\Mystatic.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\NumList.obj" \
	"$(INTDIR)\Progress.obj" \
	"$(INTDIR)\RecEdit.obj" \
	"$(INTDIR)\Resize.obj" \
	"$(INTDIR)\ScParent.obj" \
	"$(INTDIR)\scrltest.obj" \
	"$(INTDIR)\sdemo.obj" \
	"$(INTDIR)\StatTest.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Ticks.obj" \
	"$(INTDIR)\TinyButt.obj" \
	"$(INTDIR)\TrackBar.obj" \
	"$(INTDIR)\tracklog.obj" \
	"$(INTDIR)\Trackndx.obj" \
	"$(INTDIR)\Trackops.obj" \
	"$(INTDIR)\TrackPos.obj" \
	"$(INTDIR)\TrackRc.obj" \
	"$(INTDIR)\TrackSel.obj" \
	"$(INTDIR)\Tree.obj" \
	"$(INTDIR)\TreeGetItem.obj" \
	"$(INTDIR)\TreeIns.obj" \
	"$(INTDIR)\TreeMisc.obj" \
	"$(INTDIR)\Treendx.obj" \
	"$(INTDIR)\Treeops.obj" \
	"$(INTDIR)\TVImgLst.obj" \
	"$(INTDIR)\Tvitem.obj" \
	"$(INTDIR)\UDAccel.obj" \
	"$(INTDIR)\UDBuddy.obj" \
	"$(INTDIR)\Udops.obj" \
	"$(INTDIR)\UDPos.obj" \
	"$(INTDIR)\UDZ.obj" \
	"$(INTDIR)\UpDown.obj" \
	"$(INTDIR)\Xform.obj"

"$(OUTDIR)\ControlExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "ControlExplorer - Win32 Release"
# Name "ControlExplorer - Win32 Debug"

!IF  "$(CFG)" == "ControlExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "ControlExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "ControlExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "ControlExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ControlExplorer.cpp
DEP_CPP_CONTR=\
	".\About.h"\
	".\ChildFrm.h"\
	".\ControlExplorer.h"\
	".\ControlExplorerDoc.h"\
	".\ControlExplorerView.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ControlExplorer.obj" : $(SOURCE) $(DEP_CPP_CONTR) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "ControlExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ControlExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ControlExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "ControlExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ControlExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ControlExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\3D.h"\
	".\ButtTest.h"\
	".\ControlExplorer.h"\
	".\demo.h"\
	".\DlgEdit.h"\
	".\Drawing.h"\
	".\DropDown.h"\
	".\EditTest.h"\
	".\Header.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\ListBox.h"\
	".\loggable.h"\
	".\MainFrm.h"\
	".\msglog.h"\
	".\mybutton.h"\
	".\MyStatic.h"\
	".\NumericEdit.h"\
	".\Progress.h"\
	".\recedit.h"\
	".\Resize.h"\
	".\ScParent.h"\
	".\scrltest.h"\
	".\sdemo.h"\
	".\StatTest.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\TrackBar.h"\
	".\tracklog.h"\
	".\Tree.h"\
	".\UDZ.h"\
	".\UpDown.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\ControlExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ControlExplorerDoc.cpp
DEP_CPP_CONTRO=\
	".\ControlExplorer.h"\
	".\ControlExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ControlExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_CONTRO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ControlExplorerView.cpp
DEP_CPP_CONTROL=\
	".\ControlExplorer.h"\
	".\ControlExplorerDoc.h"\
	".\ControlExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ControlExplorerView.obj" : $(SOURCE) $(DEP_CPP_CONTROL) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ControlExplorer.rc
DEP_RSC_CONTROLE=\
	".\res\1pixel.bmp"\
	".\res\bitmap1.bmp"\
	".\res\bitmap10.bmp"\
	".\res\bitmap2.bmp"\
	".\res\bitmap3.bmp"\
	".\res\bitmap4.bmp"\
	".\res\bitmap5.bmp"\
	".\res\bitmap6.bmp"\
	".\res\bitmap7.bmp"\
	".\res\bitmap8.bmp"\
	".\res\bitmap9.bmp"\
	".\res\botlist.bmp"\
	".\res\ControlExplorer.ico"\
	".\res\ControlExplorer.rc2"\
	".\res\ControlExplorerDoc.ico"\
	".\res\cur300.cur"\
	".\res\cur301.cur"\
	".\res\cur302.cur"\
	".\res\cur303.cur"\
	".\res\ico00001.ico"\
	".\res\ico00002.ico"\
	".\res\icon1.ico"\
	".\res\icon2.ico"\
	".\res\icon3.ico"\
	".\res\icon4.ico"\
	".\res\icon5.ico"\
	".\res\icon6.ico"\
	".\res\jmnco.ico"\
	".\res\lattice.bmp"\
	".\res\leaf.bmp"\
	".\res\od1D.ico"\
	".\res\od1U.ico"\
	".\res\od1X.ico"\
	".\res\slowdown.ico"\
	".\res\Toolbar.bmp"\
	".\res\treebk.bmp"\
	".\res\treebook.bmp"\
	".\res\treechec.bmp"\
	".\res\treefold.bmp"\
	".\res\treehelp.bmp"\
	".\res\treepic.bmp"\
	

"$(INTDIR)\ControlExplorer.res" : $(SOURCE) $(DEP_RSC_CONTROLE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\ControlExplorer.hpj

!IF  "$(CFG)" == "ControlExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=ControlExplorer
InputPath=.\hlp\ControlExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "ControlExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=ControlExplorer
InputPath=.\hlp\ControlExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Xform.cpp
DEP_CPP_XFORM=\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\Xform.obj" : $(SOURCE) $(DEP_CPP_XFORM) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Animate.cpp
DEP_CPP_ANIMA=\
	".\animate.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Animate.obj" : $(SOURCE) $(DEP_CPP_ANIMA) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bitcombo.cpp
DEP_CPP_BITCO=\
	".\BitCombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Bitcombo.obj" : $(SOURCE) $(DEP_CPP_BITCO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bitmap.cpp
DEP_CPP_BITMA=\
	".\bitmap.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Bitmap.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\bmselect.cpp
DEP_CPP_BMSEL=\
	".\bmSelect.h"\
	".\image.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\bmselect.obj" : $(SOURCE) $(DEP_CPP_BMSEL) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Botany.cpp
DEP_CPP_BOTAN=\
	".\botany.h"\
	".\demo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Botany.obj" : $(SOURCE) $(DEP_CPP_BOTAN) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\BtnMsg.cpp
DEP_CPP_BTNMS=\
	".\BtnMsg.h"\
	".\ControlExplorer.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\BtnMsg.obj" : $(SOURCE) $(DEP_CPP_BTNMS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ButtTest.cpp
DEP_CPP_BUTTT=\
	".\animate.h"\
	".\BtnMsg.h"\
	".\ButtTest.h"\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\DlgCode.h"\
	".\dlgcodes.h"\
	".\Is95.h"\
	".\msglog.h"\
	".\mybutton.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\xform.h"\
	

"$(INTDIR)\ButtTest.obj" : $(SOURCE) $(DEP_CPP_BUTTT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Callback.cpp
DEP_CPP_CALLB=\
	".\Callback.h"\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\treeOps.h"\
	".\tvitem.h"\
	

"$(INTDIR)\Callback.obj" : $(SOURCE) $(DEP_CPP_CALLB) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Comboops.cpp
DEP_CPP_COMBO=\
	".\comboops.h"\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Comboops.obj" : $(SOURCE) $(DEP_CPP_COMBO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Copy.cpp
DEP_CPP_COPY_=\
	".\StdAfx.h"\
	

"$(INTDIR)\Copy.obj" : $(SOURCE) $(DEP_CPP_COPY_) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Ddndx.cpp
DEP_CPP_DDNDX=\
	".\ControlExplorer.h"\
	".\ddNdx.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Ddndx.obj" : $(SOURCE) $(DEP_CPP_DDNDX) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DDState.cpp
DEP_CPP_DDSTA=\
	".\comboops.h"\
	".\ControlExplorer.h"\
	".\DDState.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\locales.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\DDState.obj" : $(SOURCE) $(DEP_CPP_DDSTA) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DDText.cpp
DEP_CPP_DDTEX=\
	".\comboops.h"\
	".\ControlExplorer.h"\
	".\DDText.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\DDText.obj" : $(SOURCE) $(DEP_CPP_DDTEX) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DlgCode.cpp
DEP_CPP_DLGCO=\
	".\ControlExplorer.h"\
	".\DlgCode.h"\
	".\dlgcodes.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DlgCode.obj" : $(SOURCE) $(DEP_CPP_DLGCO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DlgEdit.cpp
DEP_CPP_DLGED=\
	".\ControlExplorer.h"\
	".\dlgcodes.h"\
	".\DlgEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DlgEdit.obj" : $(SOURCE) $(DEP_CPP_DLGED) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Drawing.cpp
DEP_CPP_DRAWI=\
	".\ControlExplorer.h"\
	".\Drawing.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Drawing.obj" : $(SOURCE) $(DEP_CPP_DRAWI) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DropDown.cpp
DEP_CPP_DROPD=\
	".\comboops.h"\
	".\ControlExplorer.h"\
	".\ddNdx.h"\
	".\DDState.h"\
	".\DDText.h"\
	".\DropDown.h"\
	".\effects.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\locales.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\DropDown.obj" : $(SOURCE) $(DEP_CPP_DROPD) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditCont.cpp
DEP_CPP_EDITC=\
	".\ControlExplorer.h"\
	".\EditCont.h"\
	".\EditOps.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditCont.obj" : $(SOURCE) $(DEP_CPP_EDITC) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditMap.cpp
DEP_CPP_EDITM=\
	".\ControlExplorer.h"\
	".\EditMap.h"\
	".\EditOps.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditMap.obj" : $(SOURCE) $(DEP_CPP_EDITM) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Editndx.cpp
DEP_CPP_EDITN=\
	".\ControlExplorer.h"\
	".\Editndx.h"\
	".\EditOps.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Editndx.obj" : $(SOURCE) $(DEP_CPP_EDITN) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditOps.cpp
DEP_CPP_EDITO=\
	".\ControlExplorer.h"\
	".\EditOps.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditOps.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditRect.cpp
DEP_CPP_EDITR=\
	".\ControlExplorer.h"\
	".\EditOps.h"\
	".\EditRect.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditRect.obj" : $(SOURCE) $(DEP_CPP_EDITR) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditScrl.cpp
DEP_CPP_EDITS=\
	".\ControlExplorer.h"\
	".\EditOps.h"\
	".\EditScrl.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditScrl.obj" : $(SOURCE) $(DEP_CPP_EDITS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditStat.cpp
DEP_CPP_EDITST=\
	".\ControlExplorer.h"\
	".\EditOps.h"\
	".\EditStat.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditStat.obj" : $(SOURCE) $(DEP_CPP_EDITST) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditTabs.cpp
DEP_CPP_EDITT=\
	".\ControlExplorer.h"\
	".\EditOps.h"\
	".\EditTabs.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\numlist.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditTabs.obj" : $(SOURCE) $(DEP_CPP_EDITT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditTest.cpp

!IF  "$(CFG)" == "ControlExplorer - Win32 Release"

DEP_CPP_EDITTE=\
	".\animate.h"\
	".\ControlExplorer.h"\
	".\DlgCode.h"\
	".\dlgcodes.h"\
	".\DlgEdit.h"\
	".\EditCont.h"\
	".\EditMap.h"\
	".\Editndx.h"\
	".\EditOps.h"\
	".\EditRect.h"\
	".\EditScrl.h"\
	".\EditStat.h"\
	".\EditTabs.h"\
	".\EditTest.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\numlist.h"\
	".\recedit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditTest.obj" : $(SOURCE) $(DEP_CPP_EDITTE) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


!ELSEIF  "$(CFG)" == "ControlExplorer - Win32 Debug"

DEP_CPP_EDITTE=\
	".\animate.h"\
	".\ControlExplorer.h"\
	".\DlgCode.h"\
	".\dlgcodes.h"\
	".\DlgEdit.h"\
	".\EditCont.h"\
	".\EditMap.h"\
	".\Editndx.h"\
	".\EditOps.h"\
	".\EditRect.h"\
	".\EditScrl.h"\
	".\EditStat.h"\
	".\EditTabs.h"\
	".\EditTest.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\numlist.h"\
	".\recedit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\EditTest.obj" : $(SOURCE) $(DEP_CPP_EDITTE) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Effects.cpp
DEP_CPP_EFFEC=\
	".\comboops.h"\
	".\ControlExplorer.h"\
	".\effects.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Effects.obj" : $(SOURCE) $(DEP_CPP_EFFEC) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HdrLayout.cpp
DEP_CPP_HDRLA=\
	".\BitCombo.h"\
	".\ControlExplorer.h"\
	".\HdrLayout.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\HdrLayout.obj" : $(SOURCE) $(DEP_CPP_HDRLA) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HdrMsg.cpp
DEP_CPP_HDRMS=\
	".\bitmap.h"\
	".\bmSelect.h"\
	".\ControlExplorer.h"\
	".\HdrMsg.h"\
	".\image.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\HdrMsg.obj" : $(SOURCE) $(DEP_CPP_HDRMS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Header.cpp
DEP_CPP_HEADE=\
	".\BitCombo.h"\
	".\bmSelect.h"\
	".\ControlExplorer.h"\
	".\HdrLayout.h"\
	".\HdrMsg.h"\
	".\Header.h"\
	".\image.h"\
	".\IndexPg.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Header.obj" : $(SOURCE) $(DEP_CPP_HEADE) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HexDisp.cpp
DEP_CPP_HEXDI=\
	".\ControlExplorer.h"\
	".\HexDisp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HexDisp.obj" : $(SOURCE) $(DEP_CPP_HEXDI) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Image.cpp
DEP_CPP_IMAGE=\
	".\image.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Image.obj" : $(SOURCE) $(DEP_CPP_IMAGE) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Indexpg.cpp
DEP_CPP_INDEX=\
	".\ControlExplorer.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Indexpg.obj" : $(SOURCE) $(DEP_CPP_INDEX) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Intedit.cpp
DEP_CPP_INTED=\
	".\IntEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Intedit.obj" : $(SOURCE) $(DEP_CPP_INTED) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Is95.cpp
DEP_CPP_IS95_=\
	".\StdAfx.h"\
	

"$(INTDIR)\Is95.obj" : $(SOURCE) $(DEP_CPP_IS95_) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ixable.cpp
DEP_CPP_IXABL=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ixable.obj" : $(SOURCE) $(DEP_CPP_IXABL) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\listbox.cpp
DEP_CPP_LISTB=\
	".\animate.h"\
	".\ControlExplorer.h"\
	".\dlgcodes.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\ListBox.h"\
	".\ListDir.h"\
	".\listndx.h"\
	".\listops.h"\
	".\ListQ.h"\
	".\ListRect.h"\
	".\ListStat.h"\
	".\ListStr.h"\
	".\ListTabs.h"\
	".\locales.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\numlist.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\xform.h"\
	

"$(INTDIR)\listbox.obj" : $(SOURCE) $(DEP_CPP_LISTB) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListDir.cpp
DEP_CPP_LISTD=\
	".\ControlExplorer.h"\
	".\Is95.h"\
	".\ixable.h"\
	".\ListDir.h"\
	".\listops.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\ListDir.obj" : $(SOURCE) $(DEP_CPP_LISTD) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Listndx.cpp
DEP_CPP_LISTN=\
	".\ControlExplorer.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\listndx.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Listndx.obj" : $(SOURCE) $(DEP_CPP_LISTN) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Listops.cpp
DEP_CPP_LISTO=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\listops.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Listops.obj" : $(SOURCE) $(DEP_CPP_LISTO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListQ.cpp
DEP_CPP_LISTQ=\
	".\ControlExplorer.h"\
	".\IndexPg.h"\
	".\ixable.h"\
	".\listops.h"\
	".\ListQ.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ListQ.obj" : $(SOURCE) $(DEP_CPP_LISTQ) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListRect.cpp
DEP_CPP_LISTR=\
	".\ControlExplorer.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\listops.h"\
	".\ListRect.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\ListRect.obj" : $(SOURCE) $(DEP_CPP_LISTR) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListStat.cpp
DEP_CPP_LISTS=\
	".\ControlExplorer.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\listops.h"\
	".\ListStat.h"\
	".\locales.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\ListStat.obj" : $(SOURCE) $(DEP_CPP_LISTS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListStr.cpp
DEP_CPP_LISTST=\
	".\ControlExplorer.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\listops.h"\
	".\ListStr.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\xform.h"\
	

"$(INTDIR)\ListStr.obj" : $(SOURCE) $(DEP_CPP_LISTST) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListTabs.cpp
DEP_CPP_LISTT=\
	".\ControlExplorer.h"\
	".\HexDisp.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\listops.h"\
	".\ListTabs.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\numlist.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\ListTabs.obj" : $(SOURCE) $(DEP_CPP_LISTT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Locales.cpp
DEP_CPP_LOCAL=\
	".\ControlExplorer.h"\
	".\locales.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Locales.obj" : $(SOURCE) $(DEP_CPP_LOCAL) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Loggable.cpp
DEP_CPP_LOGGA=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\Loggable.obj" : $(SOURCE) $(DEP_CPP_LOGGA) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MsgLog.cpp
DEP_CPP_MSGLO=\
	".\msglog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MsgLog.obj" : $(SOURCE) $(DEP_CPP_MSGLO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MsgSheet.cpp
DEP_CPP_MSGSH=\
	".\ControlExplorer.h"\
	".\msgsheet.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MsgSheet.obj" : $(SOURCE) $(DEP_CPP_MSGSH) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyButton.cpp
DEP_CPP_MYBUT=\
	".\ControlExplorer.h"\
	".\dlgcodes.h"\
	".\mybutton.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MyButton.obj" : $(SOURCE) $(DEP_CPP_MYBUT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NumericEdit.cpp
DEP_CPP_NUMER=\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NumericEdit.obj" : $(SOURCE) $(DEP_CPP_NUMER) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NumList.cpp
DEP_CPP_NUMLI=\
	".\ControlExplorer.h"\
	".\numlist.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NumList.obj" : $(SOURCE) $(DEP_CPP_NUMLI) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Progress.cpp
DEP_CPP_PROGR=\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\Progress.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\Progress.obj" : $(SOURCE) $(DEP_CPP_PROGR) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RecEdit.cpp
DEP_CPP_RECED=\
	".\ControlExplorer.h"\
	".\recedit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RecEdit.obj" : $(SOURCE) $(DEP_CPP_RECED) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Resize.cpp
DEP_CPP_RESIZ=\
	".\ControlExplorer.h"\
	".\Is95.h"\
	".\Resize.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Resize.obj" : $(SOURCE) $(DEP_CPP_RESIZ) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ScParent.cpp
DEP_CPP_SCPAR=\
	".\ControlExplorer.h"\
	".\ScParent.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\ScParent.obj" : $(SOURCE) $(DEP_CPP_SCPAR) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\scrltest.cpp
DEP_CPP_SCRLT=\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\IntEdit.h"\
	".\Is95.h"\
	".\msglog.h"\
	".\ScParent.h"\
	".\scrltest.h"\
	".\StdAfx.h"\
	".\uwm.h"\
	

"$(INTDIR)\scrltest.obj" : $(SOURCE) $(DEP_CPP_SCRLT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sdemo.cpp
DEP_CPP_SDEMO=\
	".\ControlExplorer.h"\
	".\Drawing.h"\
	".\NumericEdit.h"\
	".\sdemo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\sdemo.obj" : $(SOURCE) $(DEP_CPP_SDEMO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StatTest.cpp
DEP_CPP_STATT=\
	".\animate.h"\
	".\ControlExplorer.h"\
	".\DlgCode.h"\
	".\dlgcodes.h"\
	".\Is95.h"\
	".\msglog.h"\
	".\MyStatic.h"\
	".\StatTest.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\StatTest.obj" : $(SOURCE) $(DEP_CPP_STATT) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Ticks.cpp
DEP_CPP_TICKS=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\Ticks.h"\
	".\tinybutt.h"\
	".\TrackOps.h"\
	

"$(INTDIR)\Ticks.obj" : $(SOURCE) $(DEP_CPP_TICKS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TinyButt.cpp
DEP_CPP_TINYB=\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\TinyButt.obj" : $(SOURCE) $(DEP_CPP_TINYB) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TrackBar.cpp
DEP_CPP_TRACK=\
	".\animate.h"\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\IndexPg.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\Ticks.h"\
	".\tinybutt.h"\
	".\TrackBar.h"\
	".\tracklog.h"\
	".\trackndx.h"\
	".\TrackOps.h"\
	".\trackpos.h"\
	".\trackrc.h"\
	".\tracksel.h"\
	

"$(INTDIR)\TrackBar.obj" : $(SOURCE) $(DEP_CPP_TRACK) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tracklog.cpp
DEP_CPP_TRACKL=\
	".\ControlExplorer.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tracklog.h"\
	

"$(INTDIR)\tracklog.obj" : $(SOURCE) $(DEP_CPP_TRACKL) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trackndx.cpp
DEP_CPP_TRACKN=\
	".\ControlExplorer.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\trackndx.h"\
	

"$(INTDIR)\Trackndx.obj" : $(SOURCE) $(DEP_CPP_TRACKN) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Trackops.cpp
DEP_CPP_TRACKO=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\TrackOps.h"\
	

"$(INTDIR)\Trackops.obj" : $(SOURCE) $(DEP_CPP_TRACKO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TrackPos.cpp
DEP_CPP_TRACKP=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\TrackOps.h"\
	".\trackpos.h"\
	

"$(INTDIR)\TrackPos.obj" : $(SOURCE) $(DEP_CPP_TRACKP) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TrackRc.cpp
DEP_CPP_TRACKR=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\TrackOps.h"\
	".\trackrc.h"\
	

"$(INTDIR)\TrackRc.obj" : $(SOURCE) $(DEP_CPP_TRACKR) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TrackSel.cpp
DEP_CPP_TRACKS=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\TrackOps.h"\
	".\tracksel.h"\
	

"$(INTDIR)\TrackSel.obj" : $(SOURCE) $(DEP_CPP_TRACKS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Tree.cpp
DEP_CPP_TREE_=\
	".\botany.h"\
	".\Callback.h"\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\demo.h"\
	".\IndexPg.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\NumericEdit.h"\
	".\Resize.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\Tree.h"\
	".\TreeGetItem.h"\
	".\treeins.h"\
	".\treemisc.h"\
	".\treendx.h"\
	".\treeOps.h"\
	".\tvimglst.h"\
	".\tvitem.h"\
	

"$(INTDIR)\Tree.obj" : $(SOURCE) $(DEP_CPP_TREE_) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TreeGetItem.cpp
DEP_CPP_TREEG=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\TreeGetItem.h"\
	".\treeOps.h"\
	".\tvitem.h"\
	

"$(INTDIR)\TreeGetItem.obj" : $(SOURCE) $(DEP_CPP_TREEG) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TreeIns.cpp
DEP_CPP_TREEI=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\treeins.h"\
	".\treeOps.h"\
	".\tvitem.h"\
	

"$(INTDIR)\TreeIns.obj" : $(SOURCE) $(DEP_CPP_TREEI) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TreeMisc.cpp
DEP_CPP_TREEM=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\treemisc.h"\
	".\treeOps.h"\
	".\tvitem.h"\
	

"$(INTDIR)\TreeMisc.obj" : $(SOURCE) $(DEP_CPP_TREEM) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Treendx.cpp
DEP_CPP_TREEN=\
	".\ControlExplorer.h"\
	".\HexDisp.h"\
	".\IndexPg.h"\
	".\IntEdit.h"\
	".\ixable.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\treendx.h"\
	

"$(INTDIR)\Treendx.obj" : $(SOURCE) $(DEP_CPP_TREEN) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Treeops.cpp
DEP_CPP_TREEO=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\treeOps.h"\
	

"$(INTDIR)\Treeops.obj" : $(SOURCE) $(DEP_CPP_TREEO) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TVImgLst.cpp
DEP_CPP_TVIMG=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\treeOps.h"\
	".\tvimglst.h"\
	

"$(INTDIR)\TVImgLst.obj" : $(SOURCE) $(DEP_CPP_TVIMG) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Tvitem.cpp
DEP_CPP_TVITE=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\treeOps.h"\
	".\tvitem.h"\
	

"$(INTDIR)\Tvitem.obj" : $(SOURCE) $(DEP_CPP_TVITE) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UDAccel.cpp
DEP_CPP_UDACC=\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\UDAccel.h"\
	".\UDOps.h"\
	

"$(INTDIR)\UDAccel.obj" : $(SOURCE) $(DEP_CPP_UDACC) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UDBuddy.cpp
DEP_CPP_UDBUD=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\UDBuddy.h"\
	".\UDOps.h"\
	".\UDZ.h"\
	

"$(INTDIR)\UDBuddy.obj" : $(SOURCE) $(DEP_CPP_UDBUD) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Udops.cpp
DEP_CPP_UDOPS=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\UDOps.h"\
	

"$(INTDIR)\Udops.obj" : $(SOURCE) $(DEP_CPP_UDOPS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UDPos.cpp
DEP_CPP_UDPOS=\
	".\ControlExplorer.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\UDOps.h"\
	".\UDPos.h"\
	

"$(INTDIR)\UDPos.obj" : $(SOURCE) $(DEP_CPP_UDPOS) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UDZ.cpp
DEP_CPP_UDZ_C=\
	".\ControlExplorer.h"\
	".\StdAfx.h"\
	".\UDZ.h"\
	

"$(INTDIR)\UDZ.obj" : $(SOURCE) $(DEP_CPP_UDZ_C) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\UpDown.cpp
DEP_CPP_UPDOW=\
	".\animate.h"\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\IndexPg.h"\
	".\ixable.h"\
	".\loggable.h"\
	".\msglog.h"\
	".\msgsheet.h"\
	".\NumericEdit.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\UDAccel.h"\
	".\UDBuddy.h"\
	".\UDOps.h"\
	".\UDPos.h"\
	".\UDZ.h"\
	".\UpDown.h"\
	

"$(INTDIR)\UpDown.obj" : $(SOURCE) $(DEP_CPP_UPDOW) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\3D.cpp
DEP_CPP_3D_CP=\
	".\3D.h"\
	".\ControlExplorer.h"\
	".\copy.h"\
	".\Is95.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\3D.obj" : $(SOURCE) $(DEP_CPP_3D_CP) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Demo.cpp
DEP_CPP_DEMO_=\
	".\demo.h"\
	".\msglog.h"\
	".\Resize.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\Tree.h"\
	

"$(INTDIR)\Demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mystatic.cpp
DEP_CPP_MYSTA=\
	".\ControlExplorer.h"\
	".\dlgcodes.h"\
	".\MyStatic.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Mystatic.obj" : $(SOURCE) $(DEP_CPP_MYSTA) "$(INTDIR)"\
 "$(INTDIR)\ControlExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
