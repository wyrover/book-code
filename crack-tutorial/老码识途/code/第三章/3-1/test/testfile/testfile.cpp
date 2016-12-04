// testfile.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
void main(int argc, char * argv[]){
	FILE * f;
	char buf[30];
	f = fopen("c:\\temp.txt", "w");//in windows, file path ‘\’ should be ‘\\’
	if (f != NULL) 
	{
		fputs("han hong\n", f);
		fputs("han heee\n", f);
		fclose(f);
	}
	f = fopen("c:\\temp.txt", "r");
	if (f != NULL) 
	{
		while (fgets(buf, sizeof(buf), f) != NULL)
			printf("%s", buf);
		if (feof(f) != 0) 
			printf("********file end********\n"); 
		fclose(f);
	}
	getchar();
	return;
}