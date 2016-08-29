@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by FONTEXPLORER.HPJ. >"hlp\FontExplorer.hm"
echo. >>"hlp\FontExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\FontExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\FontExplorer.hm"
echo. >>"hlp\FontExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\FontExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\FontExplorer.hm"
echo. >>"hlp\FontExplorer.hm"
echo // Resources (IDR_*) >>"hlp\FontExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\FontExplorer.hm"
echo. >>"hlp\FontExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\FontExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\FontExplorer.hm"
echo. >>"hlp\FontExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\FontExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\FontExplorer.hm"
REM -- Make help for Project FONTEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\FontExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\FontExplorer.hlp" Debug
if exist Debug\nul copy "hlp\FontExplorer.cnt" Debug
if exist Release\nul copy "hlp\FontExplorer.hlp" Release
if exist Release\nul copy "hlp\FontExplorer.cnt" Release
echo.


