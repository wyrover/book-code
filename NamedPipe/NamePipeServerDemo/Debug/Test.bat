for /l %%i in (1,1,20) do (
@echo off
cd E:\Project\NamedPipe\NamePipeServerDemo\Debug\
start NamePipeClientDemo.exe
)