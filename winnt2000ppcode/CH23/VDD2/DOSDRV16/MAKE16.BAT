masm /mx dosdrv16.asm,,nul.lst,nul.crf;
link /M /L dosdrv16;
exe2bin dosdrv16.exe dosdrv16.sys
