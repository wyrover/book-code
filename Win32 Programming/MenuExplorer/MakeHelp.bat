@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by MENUEXPLORER.HPJ. >"hlp\MenuExplorer.hm"
echo. >>"hlp\MenuExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\MenuExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\MenuExplorer.hm"
echo. >>"hlp\MenuExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\MenuExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\MenuExplorer.hm"
echo. >>"hlp\MenuExplorer.hm"
echo // Resources (IDR_*) >>"hlp\MenuExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\MenuExplorer.hm"
echo. >>"hlp\MenuExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\MenuExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\MenuExplorer.hm"
echo. >>"hlp\MenuExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\MenuExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\MenuExplorer.hm"
REM -- Make help for Project MENUEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\MenuExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\MenuExplorer.hlp" Debug
if exist Debug\nul copy "hlp\MenuExplorer.cnt" Debug
if exist Release\nul copy "hlp\MenuExplorer.hlp" Release
if exist Release\nul copy "hlp\MenuExplorer.cnt" Release
echo.


