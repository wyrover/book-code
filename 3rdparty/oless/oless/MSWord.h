#ifndef MSWORD_H
#define MSWORD_H


struct FibBase{
	unsigned short wIdent; // 2 bytes
	unsigned short nFib;
	unsigned short unused1;
	unsigned short lid;
	unsigned short pnNext;
	unsigned short flags;
	unsigned short nFibBack; //must be 0x000BF or 0x00C1
	unsigned char lKey[4];
	short flags2;
	char reserved[12];
};

struct FibRgW97{
	char buffer[28];
};

struct FibRgLw97{
	char buffer[88];
};

struct Fib {
	FibBase base;
	unsigned short csw;
	FibRgW97 fibRgW;
	unsigned short cslw;
	FibRgLw97 fibRgLw;
	unsigned short cbRgFcLcb;
};

struct Fib2 {
	unsigned short cswNew;
	unsigned short nFibNew;
};

#endif