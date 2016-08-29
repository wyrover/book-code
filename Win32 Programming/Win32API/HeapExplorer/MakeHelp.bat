@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by HEAPEXPLORER.HPJ. >"hlp\HeapExplorer.hm"
echo. >>"hlp\HeapExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\HeapExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\HeapExplorer.hm"
echo. >>"hlp\HeapExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\HeapExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\HeapExplorer.hm"
echo. >>"hlp\HeapExplorer.hm"
echo // Resources (IDR_*) >>"hlp\HeapExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\HeapExplorer.hm"
echo. >>"hlp\HeapExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\HeapExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\HeapExplorer.hm"
echo. >>"hlp\HeapExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\HeapExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\HeapExplorer.hm"
REM -- Make help for Project HEAPEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\HeapExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\HeapExplorer.hlp" Debug
if exist Debug\nul copy "hlp\HeapExplorer.cnt" Debug
if exist Release\nul copy "hlp\HeapExplorer.hlp" Release
if exist Release\nul copy "hlp\HeapExplorer.cnt" Release
echo.


