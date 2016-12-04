// WrongDestine.cpp : 定义控制台应用程序的入口点。
//
#include <malloc.h>
#include <string.h>

void enc(char * src, char * des, char key)
{
	char * ptr = src;
	char * tem = (char *)malloc(strlen(src) + 1);
	int i = 0;
	while (* ptr != 0)
	{
		tem[i] = *ptr ^ key;
		ptr++;
		i++;
	}
	tem[strlen(src)] = 0;
	strcpy(des, tem);
}

void main()
{
	char* pDes = "12345678";
	char* encryed = "hello";
	enc(encryed, pDes, 0);
}

