#include "PhoneInfo.h"
#include "utils.h"

char gPhoneInfoPath[100];

PhoneInfoArray gPhoneInfoList;

void usage()
{
	printf("cmd as follows:\n");
	printf("load notebook: -ld path\n");
	printf("save notebook as file: -sa filename\n");
	printf("save notebook: -se \n");
	printf("add phone info: -ad\n");
	printf("quit program: -qt path\n");	
	printf("query name: -qy name\n");
}

BOOLEAN hasDefaultPhoneInfo()
{
	FILE * p = fopen(gPhoneInfoPath, "r");
	if (p != NULL)
	{
		fclose(p);
		return TRUE;
	}
	return FALSE;
}

void loadPhoneInfo(char * file)
{
	FILE * fp;
	char line[128];	
	int i;
	PhoneInfo * pInfo;

	fp = fopen(file, "r");
	if (fp == NULL)
	{
		printf("cannot find file %s\n", file);
		return;
	}
	
	initInfo();	
	PhoneInfo info;
	while (feof(fp) == 0)
	{	
		if (fread(&info, sizeof(PhoneInfo), 1, fp) == 0)
			break;
		i = gPhoneInfoList.getLength();
		gPhoneInfoList.setLength(i + 1);				
		pInfo = gPhoneInfoList.getItem(i);
		* pInfo = info;
		printf("%s   %s\n", pInfo->name, pInfo->phone);		
	}
	fclose(fp);
}

void getDefaultFilePath(char* exePath)
{
	//find directory
	int i;
	i = strlen(exePath) - 1;
	strcpy(gPhoneInfoPath, exePath);
	while (i >= 0)
	{
		if (gPhoneInfoPath[i] == '\\')
		{
			i++;
			break;
		}	
		i--;
	}
	if (i < 0)
	{
		return;
	}
	//make the path
	strcpy(gPhoneInfoPath + i, DEFAULT_PHONE_INFO);
}


void addPhoneInfo(PhoneInfo info)
{
	FILE * pf;
	
	pf = fopen(gPhoneInfoPath, "a");
	if (pf == NULL)
	{
		return;
	}
	fwrite(&info, sizeof(PhoneInfo), 1, pf);	
	gPhoneInfoList.setLength(gPhoneInfoList.getLength() + 1);
	gPhoneInfoList.setItem(gPhoneInfoList.getLength() - 1, &info);
	fclose(pf);
}

void initInfo()
{
	gPhoneInfoList.setLength(0);
}

void queryName(char * name)
{
	int i;
	BOOLEAN found = FALSE;
	PhoneInfo * pInfo;
	int count = gPhoneInfoList.getLength();
	for (i = 0; i < count; i++)
	{
		pInfo = gPhoneInfoList.getItem(i);
		if (strcmp(pInfo->name, name) == 0)
		{
			printf("%s  %s\n", pInfo->name, pInfo->phone);
			found = TRUE;
		}
	}

	if (found == FALSE)
	{
		printf("cannot find the name: %s\n", name);
	}
}

PhoneInfoArray::PhoneInfoArray()
{
	m_array = new DynamicArray(sizeof(PhoneInfo));
}

PhoneInfoArray::~PhoneInfoArray()
{
	delete m_array;
}

void PhoneInfoArray::setLength(int size)
{
	m_array->setArrayLength(size);
}

int PhoneInfoArray::getLength()
{
	return m_array->count;
}

PhoneInfo * PhoneInfoArray::getItem(int index)
{
	return (PhoneInfo *) m_array->getArrayItem(index);
}

void PhoneInfoArray::setItem(int index, PhoneInfo * pValue)
{
	m_array->setArrayItem(index, pValue);
}