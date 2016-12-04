#ifndef PHONEINFO_H_XXXXX
#define PHONEINFO_H_XXXXX

#include <stdio.h>
#include "../include/types.h"
#include "../include/diskarray.h"
#include <string.h>


#define DEFAULT_PHONE_INFO "phoneinfo.dat"

#define MAX_PHONE_NUM 100

struct PhoneInfo{
	char name[30];
	char phone[30];
};

typedef void (*ShowPhoneInfoPtr)(PhoneInfo * info);


#ifndef PHONEINFOLOGICLAYER_EXPORTS
  extern char __declspec(dllimport) gPhoneInfoPath[100];
#else
  extern char __declspec(dllexport) gPhoneInfoPath[100];
#endif

//extern char  gPhoneInfoPath[100];

void addInfo(PhoneInfo * info);

void setShowPhoneInfo(ShowPhoneInfoPtr pfunc);

int getInfoCount();

PhoneInfo getInfo(int index);


BOOLEAN hasDefaultPhoneInfo();

void loadPhoneInfo(char * file);

void getDefaultFilePath(char* exePath);

void addPhoneInfo(PhoneInfo info);

void initInfo();

BOOLEAN queryName(char * name);

void initSys(char * exeFileName);

#endif