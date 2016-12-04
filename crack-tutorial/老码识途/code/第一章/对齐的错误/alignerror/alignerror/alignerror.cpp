// alignerror.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>

struct Hdr
{
	char flag;
	int len;
};

void decodeHdr1(char * buf)
{
	struct Hdr * phdr;
	phdr = (struct Hdr *)buf;
	printf("version1 len = %d\n", phdr->len);
}

void decodeHdr2(char * buf)
{
	int * pdataLen = (int *)&buf[1];
	printf("version2 len = %d\n", * pdataLen);
}

void main()
{
	char buf[64];
	buf[0] = 1;//填写标志位 
	*((int *)&buf[1]) = 12;
	decodeHdr1(buf);
	decodeHdr2(buf);
}

