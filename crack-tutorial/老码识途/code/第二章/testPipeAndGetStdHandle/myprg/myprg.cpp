// myprg.cpp : 定义控制台应用程序的入口点。
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE eh = GetStdHandle(STD_ERROR_HANDLE);
	FILE * fp = fopen("c:\\result.txt", "w");
	char str[10];
	itoa((int)ih, str, 10);
	fwrite(str, strlen(str), 1, fp);
	fwrite("\r\n", 2, 1, fp);
	itoa((int)oh, str, 10);
	fwrite(str, strlen(str), 1, fp);
	fwrite("\r\n", 2, 1, fp);
	itoa((int)eh, str, 10);
	fwrite(str, strlen(str), 1, fp);
	fwrite("\r\n", 2, 1, fp);
	fclose(fp);	
}

