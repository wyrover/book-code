#ifndef PHONEINFO_H_XXXXX
#define PHONEINFO_H_XXXXX

#include <stdio.h>
#include "types.h"
#include <string.h>



#define DEFAULT_PHONE_INFO "phoneinfo.dat"

#define MAX_PHONE_NUM 100

struct PhoneInfo{
	char name[30];
	char phone[30];
};

extern char gPhoneInfoPath[100];

void usage();

BOOLEAN hasDefaultPhoneInfo();

void loadPhoneInfo(char * file);

void getDefaultFilePath(char* exePath);

void addPhoneInfo(PhoneInfo info);

void initInfo();

void queryName(char * name);

#endif