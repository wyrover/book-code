all:test test_static test_dynamic

#默认:多线程静态链接
test:
 cl test.cpp /D_CRT_SECURE_NO_DEPRECATE /EHsc /link user32.lib /out:test.exe
 
#多线程静态链接
#libcmt.lib C运行时静态库
#libcpmt.lib C++运行时静态库
test_static:
 cl /MT test.cpp /D_CRT_SECURE_NO_DEPRECATE /EHsc /link /NODEFAULTLIB libcmt.lib libcpmt.lib \
 Kernel32.lib user32.lib /out:test_static.exe
 
#多线程动态链接
#msvcrt.lib C运行时导入库,执行期依赖于MSVCR90.dll
#msvcprt.lib C++运行时导入库,执行期依赖于MSVCP90.dll
test_dynamic:
 cl /MD test.cpp /D_CRT_SECURE_NO_DEPRECATE /EHsc /link /NODEFAULTLIB msvcrt.lib msvcprt.lib \
 Kernel32.lib user32.lib /out:test_dynamic.exe

clean:
 del test.exe
 del test_static.exe
 del test_dynamic.exe
 del test_dynamic.exe.manifest
 del test.obj



#备注：
# /NODEFAULTLIB 选项忽略所有编译器默认链接的库。
# 用 dumpbin /DEPENDENTS test.exe 命令查看可执行文件的依赖关系。
# 示例程序使用 Visual Studio 2008 SP1 命令行编译。