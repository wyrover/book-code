@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CTLCOLOREXPLORER.HPJ. >"hlp\CtlColorExplorer.hm"
echo. >>"hlp\CtlColorExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\CtlColorExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\CtlColorExplorer.hm"
echo. >>"hlp\CtlColorExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\CtlColorExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\CtlColorExplorer.hm"
echo. >>"hlp\CtlColorExplorer.hm"
echo // Resources (IDR_*) >>"hlp\CtlColorExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\CtlColorExplorer.hm"
echo. >>"hlp\CtlColorExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\CtlColorExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\CtlColorExplorer.hm"
echo. >>"hlp\CtlColorExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\CtlColorExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\CtlColorExplorer.hm"
REM -- Make help for Project CTLCOLOREXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\CtlColorExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\CtlColorExplorer.hlp" Debug
if exist Debug\nul copy "hlp\CtlColorExplorer.cnt" Debug
if exist Release\nul copy "hlp\CtlColorExplorer.hlp" Release
if exist Release\nul copy "hlp\CtlColorExplorer.cnt" Release
echo.


