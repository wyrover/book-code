REM bldver.bat - Build wtest.c.
REM    ***this file only for 16-bit environment***
REM ----------------------------------------------
@set CCFILES=wtest.c wversion.c
@if NOT "%cc%"=="bcc" goto NOTBCC
rc -r wtest.rc
bcc -DSTRICT -WSE -vi %CCFILES%
brc wtest.res wtest.exe
@goto END
:NOTBCC
@if NOT "%cc%"=="msc" goto NOTMSC
rc -r wtest.rc
cl /c /DSTRICT /nologo /W3 /Zp /GAs /GEsme %CCFILES%
link /nologo /NOE /align:16 /NOD /MAP:FULL /LINE wtest+wversion,,wtest,slibcew+libw+ver.lib,wtest;
rc wtest.res wtest.exe
@goto END
:NOTMSC
@if NOT "%cc%"=="wcc"  goto NOTWC
wrc -r wtest.rc
wcl -dSTRICT -w3 -zw -bt=windows -ms %CCFILES% ver.lib
wrc wtest.res wtest.exe
@goto END
:NOTWC
@if NOT "%cc%"=="sc"  goto NOTSC
rc -r wtest.rc
sc -DSTRICT -a1 -ms -W3 %CCFILES% ver.lib
rem Note: due to bug in Symantec 10.0 rtl, this rc produces error message
rc wtest.res wtest.exe
@goto END
:NOTSC
@echo To use this batch file, set your CC environment variable to:
@echo   bcc     - if you are using Borland C++ v4.5
@echo   msc     - if you are using Microsoft Visual C++ v1.5
@echo   wcc     - if you are using Watcom C++ v10.0
@echo   sc      - if you are using Symantec C++ v6.10
:END
@set CCFILES=

