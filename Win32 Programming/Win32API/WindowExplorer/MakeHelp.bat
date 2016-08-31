@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by WINDOW EXPLORER.HPJ. >"hlp\WindowExplorer.hm"
echo. >>"hlp\WindowExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\WindowExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\WindowExplorer.hm"
echo. >>"hlp\WindowExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\WindowExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\WindowExplorer.hm"
echo. >>"hlp\WindowExplorer.hm"
echo // Resources (IDR_*) >>"hlp\WindowExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\WindowExplorer.hm"
echo. >>"hlp\WindowExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\WindowExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\WindowExplorer.hm"
echo. >>"hlp\WindowExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\WindowExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\WindowExplorer.hm"
REM -- Make help for Project WINDOW EXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\WindowExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\WindowExplorer.hlp" Debug
if exist Debug\nul copy "hlp\WindowExplorer.cnt" Debug
if exist Release\nul copy "hlp\WindowExplorer.hlp" Release
if exist Release\nul copy "hlp\WindowExplorer.cnt" Release
echo.


