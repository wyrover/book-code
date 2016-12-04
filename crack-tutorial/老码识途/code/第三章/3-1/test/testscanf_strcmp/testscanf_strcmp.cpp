#include<stdio.h>
#include <string.h>

void main()
{
	char cmd[30];
	while (1)
	{
		scanf("%[^\n]", cmd);
		fflush(stdin);
		if (strcmp(cmd, "-ld") == 0) 
			printf("ok\n");
		else
			break;
	}
}

