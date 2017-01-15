#ifndef MSEXCEL_H
#define MSEXCEL_H


struct BOF{
	unsigned short vers;
	unsigned short dt;
	unsigned short rubBuild;
	unsigned short rupYear;
	unsigned short fWin : 1;
	unsigned short fRisc : 1;
	unsigned short fBeta : 1;
	unsigned short fWinAny : 1;
	unsigned short fMacAny : 1;
	unsigned short fBetaAny : 1;
	unsigned short unused1 : 2;
	unsigned short fRiscAny : 1;
	unsigned short fOOM : 1;
	unsigned short fGIJmp : 1;
	unsigned short unused2 : 2;
	unsigned short fFontLimit : 1;
	unsigned short verXLHigh : 1;
	unsigned short unused3 : 1;
	unsigned short reserved1 : 13;
	unsigned short verLowestBiff : 4;
	unsigned long reserved2 : 20;
};

#endif