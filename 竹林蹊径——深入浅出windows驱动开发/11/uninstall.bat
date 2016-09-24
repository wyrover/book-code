@echo off

@echo V-ASIO Driver Uninstallation
@echo .
@echo .

%SystemRoot%\System32\rundll32.exe setupapi,InstallHinfSection DefaultUninstall 132 .\bin\V-ASIO.inf

@echo V-ASIO driver is successfully uninstalled!
@pause
