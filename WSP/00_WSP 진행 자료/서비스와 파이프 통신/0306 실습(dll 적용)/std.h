//std.h
//#define MY_DEBUG
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>	
#include <stdio.h>
#include "resource.h"
#include "mysock.h"
#pragma comment(lib, "mysock.lib")

#define PORT 9000

#include "pack.h"
#include "handler.h"
#include "ui.h"


extern CRITICAL_SECTION cs; 
