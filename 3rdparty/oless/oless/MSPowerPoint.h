#ifndef MSPOWERPOINT_H
#define MSPOWERPOINT_H


static const char PP95SIG[] = "Microsoft (R) PowerPoint (R) Windows  "; //38
struct RecordHeader {
	unsigned short recVer : 4;
	unsigned short recInstance : 12;
	unsigned short recType : 2;
	unsigned long recLen;
};
struct PointStruct {
	unsigned long x;
	unsigned long y;
};
struct RatioStruct {
	unsigned long numer;
	unsigned long denom;
};
struct DocumentAtom {
	RecordHeader rh;
	PointStruct slideSize;
	PointStruct notesSize;
	RatioStruct serverZoom;
	unsigned long notesMasterPersistIdRef;
	unsigned long handoutMasterPersistIdRef;
	unsigned short firstSlideNumber;
	unsigned short slideSizeType;
	unsigned char fSaveWithFonts;
	unsigned char fOmitTitlePlace;
	unsigned char fRightToLeft;
	unsigned char fShowComments;
};
struct DocContainer {
	RecordHeader rh;
	DocumentAtom documentAtom;
};

struct CurrentUserAtom {
	RecordHeader rh;
	unsigned long size;
	unsigned long headerToken;
	unsigned long offsetToCurrentEdit;
	unsigned short lenUserName;
	unsigned short docFileVersion;
	unsigned char majorVersion;
	unsigned char minorVersion;
	unsigned short build;
};
struct UserEditAtom {
	RecordHeader rh;
	unsigned long lastSlideIdRef;
	unsigned short version;
	unsigned char minorVersion;
	unsigned char majorVersion;
	unsigned long offsetLastEdit;
	unsigned long offsetPersistDirectory;
	unsigned long docPersistIdRef;
	unsigned long persistIdSeed;
	unsigned short lastView;
	unsigned short unused;
};

#endif