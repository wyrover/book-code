// testfile.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <string.h>

int readLn(FILE * fp, char * ln)
{
	int c, ret;
	ret = 0;	
	while (1){
		c = fgetc(fp);
		if (c == EOF){
			*ln = 0;
			return ret;
		}
		*ln = (char)c;
		ln++;
		ret++;
		if (c == '\n'){
			if (*(ln - 2) == '\r')	{
				*(ln - 2) = 0;
				ret--;
			}
			else *(ln - 1) = 0;
			return ret;
		}
	}
	return ret;
}


void main(int argc, char * argv[]){
	FILE * f;
	char buf[30];
	char retc[2];
	retc[0] = 0x0d;
	retc[1] = 0x0a;
	f = fopen("c:\\temp.txt", "wb");//in windows, file path ‘\’ should be ‘\\’
	if (f != NULL) 
	{
		fwrite("han hong", strlen("han hong"), 1, f);
		fwrite(retc, 2, 1, f);
		fwrite("han heee", strlen("han heee"), 1, f);
		fwrite(retc, 2, 1, f);
		fclose(f);
	}
	f = fopen("c:\\temp.txt", "r");
	if (f != NULL) 
	{
		while (readLn(f, buf) != 0)
			printf("%s\n", buf);
		if (feof(f) != 0) 
			printf("********file end********\n"); 
		fclose(f);
	}
	getchar();
	return;
}