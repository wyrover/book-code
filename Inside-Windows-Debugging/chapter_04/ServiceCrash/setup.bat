reg delete "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug" /f
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\AeDebug" /f
IF "%_BUILDARCH%" == "x86" copy %~dp0\obj%_BuildType%_%DDK_TARGET_OS%_%_BUILDARCH%\i386\servicewithcrash.exe %systemroot%\system32\.
IF "%_BUILDARCH%" == "AMD64" copy %~dp0\obj%_BuildType%_%DDK_TARGET_OS%_%_BUILDARCH%\amd64\servicewithcrash.exe %systemroot%\system32\.
call "%~dp0\scm\deleteservice.cmd
call "%~dp0\scm\createservice.cmd
