#include "utils.h"

int readLn(FILE * fp, char * ln)
{
	int c, ret;
	ret = 0;

	while (1)
	{
		c = fgetc(fp);
		if (c == EOF)
		{
			*ln = 0;
			return ret;
		}
		*ln = (char)c;
		ln++;
		ret++;
		if (c == '\n')
		{
			if (*(ln - 2) == '\r')
			{
				*(ln - 2) = 0;
				ret--;
			}
			else
				*(ln - 1) = 0;
			return ret;
		}
	}
	return ret;
}

char * getArg(char * start)
{
	int i = 0;
	//strip space before
	while (start[i] != ' ')
	{
		i++;
		break;
	}
	if (start[i] == NULL)
	{
		return NULL;
	}
	return &start[i];
}
