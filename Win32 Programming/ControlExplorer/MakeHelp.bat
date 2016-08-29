@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CONTROLEXPLORER.HPJ. >"hlp\ControlExplorer.hm"
echo. >>"hlp\ControlExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\ControlExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\ControlExplorer.hm"
echo. >>"hlp\ControlExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\ControlExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\ControlExplorer.hm"
echo. >>"hlp\ControlExplorer.hm"
echo // Resources (IDR_*) >>"hlp\ControlExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\ControlExplorer.hm"
echo. >>"hlp\ControlExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\ControlExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\ControlExplorer.hm"
echo. >>"hlp\ControlExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\ControlExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\ControlExplorer.hm"
REM -- Make help for Project CONTROLEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\ControlExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\ControlExplorer.hlp" Debug
if exist Debug\nul copy "hlp\ControlExplorer.cnt" Debug
if exist Release\nul copy "hlp\ControlExplorer.hlp" Release
if exist Release\nul copy "hlp\ControlExplorer.cnt" Release
echo.


