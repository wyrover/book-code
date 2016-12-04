// testfiledir.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <string.h>
#include <memory.h>

void main(int argCount, char ** args)
{
	FILE * fp;
	char buf[30];
	printf("第一个参数是: %s\n", args[0]);
	int i;
	i = strlen(args[0]) - 1;	
	while (i >= 0)
	{
		if (args[0][i] == '\\')
		{
			break;
		}	
		i--;
	}
	char * fileName = "temp.txt";
	if (i < 0)
	{
		printf("第一个参数无路径\n");		
		fp = fopen(fileName, "r");		
	}
	else
	{		
		char file[256];
		memset(file, 0, sizeof(file));		
		strcpy(file, args[0]);
		file[i] = 0;
		printf("dir is %s\n", file);
		file[i] = '\\';
		strcpy(file + i + 1, fileName);
		printf("open file %s\n", file);
		fp = fopen(file, "r");
	}
	if (fp == NULL)
	{
		return;
	}
	while (fgets(buf, sizeof(buf), fp) != NULL)
		printf("%s", buf);
	fclose(fp);
}

