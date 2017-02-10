echo Compile the Windows Installer (WIX) source file (WXS) into an obj file using the WIX compiler (candle)
"%ProgramFiles%\Windows Installer XML v3.5\bin\candle.exe" -ext WixUIExtension setup.wxs

echo Link the WIX obj file into the final MSI using the WIX linker (light)
"%ProgramFiles%\Windows Installer XML v3.5\bin\light.exe" -ext WixUIExtension setup.wixobj
