#ifndef PHONEINFO_H_XXXXX
#define PHONEINFO_H_XXXXX

#include <stdio.h>
#include "types.h"
#include <string.h>

 extern char gPhoneInfoPath[100];

#define DEFAULT_PHONE_INFO "phoneinfo.txt"

void usage();

BOOLEAN hasDefaultPhoneInfo();

void loadPhoneInfo(char * file);

void getDefaultFilePath(char* exePath);

void addPhoneInfo(char * phoneInfo);

#endif