#pragma once

#define	MAX_CHARS			1024*4
//#define MAKELONG(a, b) ((unsigned long) (((unsigned short) (a)) | ((unsigned long) ((unsigned short) (b))) << 16))

// status register bits
#define IBUFFER_FULL		0x02
#define OBUFFER_FULL		0x01

// flags for keyboard status
#define	S_SHIFT				1
#define	S_CAPS				2
#define	S_NUM				4

#define REPLACE_MAKECODE 0x27 //';'
void __stdcall print_keystroke();