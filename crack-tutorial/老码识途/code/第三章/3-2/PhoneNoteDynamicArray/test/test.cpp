// test.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <process.h>
#include "../PhoneNote1/PhoneInfo.h"
#include "../PhoneNote1/types.h"
#include <memory.h>

void showError(char * msg)
{
	printf("%s", msg);
	exit(0);
};

void clearFile(char * file)
{
	FILE * p = fopen(gPhoneInfoPath, "w");
	if (p != NULL)
	{
		fclose(p);		
	}	
}

PhoneInfo gInfo;

BOOLEAN isCallShowInfo;

void clearShowInfo()
{
	isCallShowInfo = FALSE;
	memset(&gInfo, 0, sizeof(PhoneInfo));
}

void mockShowInfo(PhoneInfo * info)
{
	isCallShowInfo = TRUE;
	gInfo = *info;
};

PhoneInfo showItems[20];
int showCount;

void mockShowInfo2(PhoneInfo * info)
{
	showItems[showCount] = * info;
	showCount++;
}

void initTestEnv()
{
	memset(showItems, 0, 20 * sizeof(PhoneInfo));
	setShowPhoneInfo(mockShowInfo);
	clearShowInfo();
	initInfo();
}

void testQueryName_CanFind()
{	
	initTestEnv();	
	/*strcpy(gPhoneInfoList[0].name, "hh");
	strcpy(gPhoneInfoList[0].phone, "123");
	strcpy(gPhoneInfoList[1].name, "ww");
	strcpy(gPhoneInfoList[1].phone, "456");
	gCount = 2;
	*/
	PhoneInfo info;
	strcpy(info.name, "hh");
	strcpy(info.phone, "123");
	addInfo(&info);
	strcpy(info.name, "ww");
	strcpy(info.phone, "456");
	addInfo(&info);	

	if (queryName("hh") == FALSE)
	{
		showError("!!!error: testQueryName_CanFind: cannot find hh\n");		
	}
	if (isCallShowInfo == FALSE)
	{
		showError("!!!error:  testQueryName_CanFind: not call show\n");		
	}
	if ((strcmp(gInfo.name, "hh") != 0) || (strcmp(gInfo.phone, "123") != 0))
	{
		showError("!!!error:  testQueryName_CanFind: not show info correctly\n");		
	}
}

void testQueryName_CanNotFind()
{
	initTestEnv();
	PhoneInfo info;
	strcpy(info.name, "hh");
	strcpy(info.phone, "123");
	addInfo(&info);
	strcpy(info.name, "ww");
	strcpy(info.phone, "456");
	addInfo(&info);	
	if (queryName("dd") == TRUE)
	{
		showError("!!!error:  testQueryName_CanNotFind: should not find, but say ok\n");		
	}	
}

void testloadPhoneInfo_addPhoneInfo()
{
	initTestEnv();
	setShowPhoneInfo(mockShowInfo2);
	showCount = 0;
	getDefaultFilePath("test.dat");
	clearFile("test.dat");
	PhoneInfo info;
	strcpy(info.name, "hh");
	strcpy(info.phone, "123");
	addPhoneInfo(info);
	strcpy(info.name, "ww");
	strcpy(info.phone, "456"); 
	addPhoneInfo(info);
	loadPhoneInfo(gPhoneInfoPath);
	int count = getInfoCount();
	if (count != 2)
	{
		showError("!!!error:  testloadPhoneInfo_addPhoneInfo: should find 2 item\n");
	}
	info = getInfo(0);
	if ((strcmp(info.name, "hh") != 0) || (strcmp(info.phone, "123") != 0))
	{
		showError("!!!error:  testloadPhoneInfo_addPhoneInfo: item0 error\n");		
	}
	info = getInfo(1);
	if ((strcmp(info.name, "ww") != 0) || (strcmp(info.phone, "456") != 0))
	{
		showError("!!!error:  testloadPhoneInfo_addPhoneInfo: item1 error\n");		
	}
	if (showCount != 2)
	{
		showError("!!!error:  testloadPhoneInfo_addPhoneInfo: should find 2 item\n");
	}
	info = showItems[0];
	if ((strcmp(info.name, "hh") != 0) || (strcmp(info.phone, "123") != 0))
	{
		showError("!!!error:  testloadPhoneInfo_addPhoneInfo: item0 error\n");		
	}
	info = showItems[1];
	if ((strcmp(info.name, "ww") != 0) || (strcmp(info.phone, "456") != 0))
	{
		showError("!!!error:  testloadPhoneInfo_addPhoneInfo: item1 error\n");		
	}
}


void main()
{
	printf("begin test\n");
	testQueryName_CanFind();
	testQueryName_CanNotFind();
	testloadPhoneInfo_addPhoneInfo();
	printf("end test\n");
}
