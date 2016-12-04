// FileRead.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <string.h>
#include "FormatFile.h"


void usage()
{
	printf("please input file path like this: fileread c:\temp \n");
}



void main(int argc, char* argv[])
{
	FormatFile ff;
	ff.doFormat();
}

