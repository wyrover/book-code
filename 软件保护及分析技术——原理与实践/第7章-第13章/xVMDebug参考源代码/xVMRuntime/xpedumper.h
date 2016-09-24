#ifndef XPEDUMPER_H
#define XPEDUMPER_H
#include <tchar.h>
#include <map>
#include "../../../nCom/nautolargebuffer.h"




class xpedumper
{
public:
    xpedumper();
    ~xpedumper();

    int InitDumpFromMemory(HMODULE hBase,PVOID toAddress);
    int SaveToFile(const TCHAR* filename);
    int AppendSection(BYTE* lpMem,int rawSize,DWORD VirtualAddr,DWORD VirtualSize,BOOL placeHold);
private:
    NAutoBufferA m_nla;
};

#endif // XPEDUMPER_H
