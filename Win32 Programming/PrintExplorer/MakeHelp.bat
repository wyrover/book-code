@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by PRINTEXPLORER.HPJ. >"hlp\PrintExplorer.hm"
echo. >>"hlp\PrintExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\PrintExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\PrintExplorer.hm"
echo. >>"hlp\PrintExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\PrintExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\PrintExplorer.hm"
echo. >>"hlp\PrintExplorer.hm"
echo // Resources (IDR_*) >>"hlp\PrintExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\PrintExplorer.hm"
echo. >>"hlp\PrintExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\PrintExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\PrintExplorer.hm"
echo. >>"hlp\PrintExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\PrintExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\PrintExplorer.hm"
REM -- Make help for Project PRINTEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\PrintExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\PrintExplorer.hlp" Debug
if exist Debug\nul copy "hlp\PrintExplorer.cnt" Debug
if exist Release\nul copy "hlp\PrintExplorer.hlp" Release
if exist Release\nul copy "hlp\PrintExplorer.cnt" Release
echo.


