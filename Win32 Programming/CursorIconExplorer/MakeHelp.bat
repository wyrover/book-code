@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CURSORICONEXPLORER.HPJ. >"hlp\CursorIconExplorer.hm"
echo. >>"hlp\CursorIconExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\CursorIconExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\CursorIconExplorer.hm"
echo. >>"hlp\CursorIconExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\CursorIconExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\CursorIconExplorer.hm"
echo. >>"hlp\CursorIconExplorer.hm"
echo // Resources (IDR_*) >>"hlp\CursorIconExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\CursorIconExplorer.hm"
echo. >>"hlp\CursorIconExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\CursorIconExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\CursorIconExplorer.hm"
echo. >>"hlp\CursorIconExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\CursorIconExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\CursorIconExplorer.hm"
REM -- Make help for Project CURSORICONEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\CursorIconExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\CursorIconExplorer.hlp" Debug
if exist Debug\nul copy "hlp\CursorIconExplorer.cnt" Debug
if exist Release\nul copy "hlp\CursorIconExplorer.hlp" Release
if exist Release\nul copy "hlp\CursorIconExplorer.cnt" Release
echo.


