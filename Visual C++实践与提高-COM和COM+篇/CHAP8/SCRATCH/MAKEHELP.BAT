@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SCRIBBLE.HPJ. >"hlp\Scribble.hm"
echo. >>"hlp\Scribble.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Scribble.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Scribble.hm"
echo. >>"hlp\Scribble.hm"
echo // Prompts (IDP_*) >>"hlp\Scribble.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Scribble.hm"
echo. >>"hlp\Scribble.hm"
echo // Resources (IDR_*) >>"hlp\Scribble.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Scribble.hm"
echo. >>"hlp\Scribble.hm"
echo // Dialogs (IDD_*) >>"hlp\Scribble.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Scribble.hm"
echo. >>"hlp\Scribble.hm"
echo // Frame Controls (IDW_*) >>"hlp\Scribble.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Scribble.hm"
REM -- Make help for Project SCRIBBLE


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Scribble.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Scribble.hlp" goto :Error
if not exist "hlp\Scribble.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Scribble.hlp" Debug
if exist Debug\nul copy "hlp\Scribble.cnt" Debug
if exist Release\nul copy "hlp\Scribble.hlp" Release
if exist Release\nul copy "hlp\Scribble.cnt" Release
echo.
goto :done

:Error
echo hlp\Scribble.hpj(1) : error: Problem encountered creating help file

:done
echo.
