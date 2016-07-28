@echo off

@echo V-ASIO Driver Installation
@echo .
@echo .

if "%1"=="-k" goto K:

%SystemRoot%\System32\rundll32.exe setupapi,InstallHinfSection DefaultInstall 132 .\bin\V-ASIO.inf
@echo successfully installed!
goto E:

:K
net start v-asio
@echo if failed to start V-ASIO, please verify that install.bat has been executed.

:E
@pause
