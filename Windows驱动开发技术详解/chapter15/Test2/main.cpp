#include <Windows.h>
#include <stdio.h>

#include ".\winiolib\WinIo.h"

int main()
{
	//打开WinIO驱动
	bool bRet = InitializeWinIo();
	if (bRet)
	{
		printf("Load Dirver successfully!\n");

		//对0x378端口进行输出操作,8位操作
		SetPortVal(0x378,0,1);

		//关闭WinIO驱动
		ShutdownWinIo();
	}

	
	return 0;
}
