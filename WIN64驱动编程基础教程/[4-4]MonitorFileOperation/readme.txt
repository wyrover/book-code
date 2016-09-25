打印出实时文件操作。
同时保护了readme.txt不被删除、修改、重命名。

手工安装方法（首先打开CMD）：
安装驱动：InfDefaultInstall INF文件完整路径
开启保护：net start MFTest
关闭保护：net stop MFTest

自动安装方法：
运行WdmDrvLoader.exe

批处理加载代码：
@title 保护文件[readme.txt]测试
@copy mftest.sys c:\
@copy setup.inf c:\
@InfDefaultInstall "c:\setup.inf"
@net start mftest
@echo 文件保护成功！现在可以测试文件保护效果！
@echo 按任意键取消文件保护。
@pause
@del c:\mftest.sys
@del c:\setup.inf
@del c:\windows\system32\drivers\mftest.sys
@net stop mftest
@echo 打扫垃圾完毕。
@pause