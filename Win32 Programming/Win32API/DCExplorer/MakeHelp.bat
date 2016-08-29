@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by DC EXPLORER.HPJ. >"hlp\DCExplorer.hm"
echo. >>"hlp\DCExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\DCExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\DCExplorer.hm"
echo. >>"hlp\DCExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\DCExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\DCExplorer.hm"
echo. >>"hlp\DCExplorer.hm"
echo // Resources (IDR_*) >>"hlp\DCExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\DCExplorer.hm"
echo. >>"hlp\DCExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\DCExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\DCExplorer.hm"
echo. >>"hlp\DCExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\DCExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\DCExplorer.hm"
REM -- Make help for Project DC EXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\DCExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\DCExplorer.hlp" Debug
if exist Debug\nul copy "hlp\DCExplorer.cnt" Debug
if exist Release\nul copy "hlp\DCExplorer.hlp" Release
if exist Release\nul copy "hlp\DCExplorer.cnt" Release
echo.


