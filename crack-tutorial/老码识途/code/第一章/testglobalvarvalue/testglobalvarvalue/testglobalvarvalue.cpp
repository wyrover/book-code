// testglobalvarvalue.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>

int gi;

void usage()
{
	printf("please input s to set gi or d to dispaly gi\n");
}

void main(int argCount, char ** args)
{	
	if (argCount != 2)
	{
		usage();
		return;
	}
	if (args[1][0] == 's')
	{
		gi = 12;
		printf("gi = %d\n", gi);
		getchar();
	}
	else if (args[1][0] == 'd')
	{	
		printf("gi = %d\n", gi);
	}
	else
		usage();
}

