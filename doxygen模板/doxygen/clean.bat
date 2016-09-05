@echo off
echo 清空以前输出
if exist refman.chm del /f /q refman.chm
if exist output\html del /f /q output\html\*.*
if exist output\latex del /f /q output\latex\*.*
if exist output\rtf del /f /q output\rtf\*.*
if exist output del /f /q output\*.*
if exist build.log del /f /q build.log
