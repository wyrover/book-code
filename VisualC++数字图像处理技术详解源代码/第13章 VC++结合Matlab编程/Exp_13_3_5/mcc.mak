all : bin_target

myeig.cpp : D:\VC2005\Exp_13_3_5\myeig.m
	d:\matlab6p5\bin\win32\mcc -L Cpp libmmfile.mlib -A line:on -G -A debugline:on -O fold_scalar_mxarrays:off -O fold_non_scalar_mxarrays:off -O optimize_integer_for_loops:off -O array_indexing:off -O optimize_conditionals:off -O speculate:off libmmfile.mlib -v -I D:\VC2005\Exp_13_3_5 -I D:\VC2005\Exp_13_3_5 -t -T codegen D:\VC2005\Exp_13_3_5\myeig.m

mymagic.cpp : D:\VC2005\Exp_13_3_5\mymagic.m
	d:\matlab6p5\bin\win32\mcc -L Cpp libmmfile.mlib -A line:on -G -A debugline:on -O fold_scalar_mxarrays:off -O fold_non_scalar_mxarrays:off -O optimize_integer_for_loops:off -O array_indexing:off -O optimize_conditionals:off -O speculate:off libmmfile.mlib -v -I D:\VC2005\Exp_13_3_5 -I D:\VC2005\Exp_13_3_5 -t -T codegen D:\VC2005\Exp_13_3_5\mymagic.m

myeig_main.cpp : D:\VC2005\Exp_13_3_5\myeig.m D:\VC2005\Exp_13_3_5\mymagic.m
	d:\matlab6p5\bin\win32\mcc  -L Cpp libmmfile.mlib -A line:on -G -A debugline:on -O fold_scalar_mxarrays:off -O fold_non_scalar_mxarrays:off -O optimize_integer_for_loops:off -O array_indexing:off -O optimize_conditionals:off -O speculate:off libmmfile.mlib -v -I D:\VC2005\Exp_13_3_5 -I D:\VC2005\Exp_13_3_5 -W main  D:\VC2005\Exp_13_3_5\myeig.m D:\VC2005\Exp_13_3_5\mymagic.m

bin_target : myeig.exe

myeig.obj : myeig.cpp
	cl  -c -Zp8 -G5 -W3 -nologo -Zm500 -GX -MD -I"d:\MATLAB6p5\extern\include\cpp" -DMSVC -DIBMPC -DMSWIND /Fomyeig.obj -Id:\MATLAB6p5\extern\include -Id:\MATLAB6p5\simulink\include -Zi -Fd"myeig.pdb" myeig.cpp

mymagic.obj : mymagic.cpp
	cl  -c -Zp8 -G5 -W3 -nologo -Zm500 -GX -MD -I"d:\MATLAB6p5\extern\include\cpp" -DMSVC -DIBMPC -DMSWIND /Fomymagic.obj -Id:\MATLAB6p5\extern\include -Id:\MATLAB6p5\simulink\include -Zi -Fd"myeig.pdb" mymagic.cpp

myeig_main.obj : myeig_main.cpp
	cl  -c -Zp8 -G5 -W3 -nologo -Zm500 -GX -MD -I"d:\MATLAB6p5\extern\include\cpp" -DMSVC -DIBMPC -DMSWIND /Fomyeig_main.obj -Id:\MATLAB6p5\extern\include -Id:\MATLAB6p5\simulink\include -Zi -Fd"myeig.pdb" myeig_main.cpp

myeig.exe : myeig.obj mymagic.obj myeig_main.obj
	link "/out:myeig.exe" /debug kernel32.lib user32.lib gdi32.lib advapi32.lib oleaut32.lib ole32.lib /LIBPATH:"d:\MATLAB6p5\extern\lib\win32\microsoft\msvc60" libmmfile.lib libmatlb.lib /nologo libmx.lib libmat.lib libmwservices.lib libmex.lib libut.lib d:\MATLAB6p5\extern\lib\win32\libmatpm.lib  @myeig_master.rsp 
	if exist _lib2456.def del _lib2456.def
	if exist ".res" del ".res"

