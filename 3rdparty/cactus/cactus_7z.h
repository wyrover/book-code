#pragma once
#include <string>
#include <7z.headers/7zip/Archive/IArchive.h>
#include <7z.headers/7zip/Common/FileStreams.h>
#include <cactus/7zHelper.h>
#include <vector>


typedef unsigned int (__stdcall *CREATEOBJECT)(const GUID *, const GUID *, void **);
typedef HRESULT(__stdcall *GETHANDLERPROPERTY)(PROPID propID, PROPVARIANT *value);
typedef HRESULT(__stdcall *GETHANDLERPROPERTY2)(unsigned int formatIndex, PROPID propID, PROPVARIANT *value);
typedef HRESULT(__stdcall *GETNUMBEROFFORMATS)(unsigned int *numFormats);

typedef UInt32 (WINAPI *GetMethodPropertyFunc)(UInt32 index, PROPID propID, PROPVARIANT *value);
typedef UInt32 (WINAPI *CreateObjectFunc)(const GUID *clsID, const GUID *interfaceID, void **outObject);
typedef UInt32 (WINAPI *GetNumberOfMethodsFunc)(UInt32 *numMethods);
typedef UInt32 (WINAPI *GetNumberOfFormatsFunc)(UInt32 *numFormats);
typedef UInt32 (WINAPI *GetHandlerPropertyFunc)(PROPID propID, PROPVARIANT *value);
typedef UInt32 (WINAPI *GetHandlerPropertyFunc2)(UInt32 index, PROPID propID, PROPVARIANT *value);
typedef UInt32 (WINAPI *CreateObjectFunc)(const GUID *clsID, const GUID *iid, void **outObject);
typedef UInt32 (WINAPI *SetLargePageModeFunc)();

struct CDllCodecInfo {
    CLSID Encoder;
    CLSID Decoder;
    bool EncoderIsAssigned;
    bool DecoderIsAssigned;
    int LibIndex;
    UInt32 CodecIndex;
};

struct FormatInfoInternal {
    GUID uid;
    char *lpSignature;
    int nSignatureLength;
    char *lpDefaultExt;
    bool bUpdate;
    char *lpName;
};

class SevenZipModule
{

public:
    HMODULE m_hModule;

    CREATEOBJECT m_pfnCreateObject;
    GETHANDLERPROPERTY m_pfnGetHandlerProperty;
    GETHANDLERPROPERTY2 m_pfnGetHandlerProperty2;
    GETNUMBEROFFORMATS m_pfnGetNumberOfFormats;

    unsigned int m_nNumberOfFormats;
    FormatInfoInternal *m_pInfo;

    std::vector<CDllCodecInfo> m_codecs;
public:
    SevenZipModule();
    bool Initialize(LPCTSTR lpFileName);
    ~SevenZipModule();

    void GetArchiveFormatInfo(unsigned int nFormatIndex);
};


class SevenZipArchive
{

public:
    const SevenZipModule *m_pModule;
    IInArchive *m_pArchive;
    CInFile *m_pInFile;

    DWORD m_nItemsNumber;
    bool m_bOpened;
    std::vector<std::string> m_files;
    std::string m_lpFileName;

public:
    SevenZipArchive(const SevenZipModule *pModule);
    virtual ~SevenZipArchive();
    virtual bool __stdcall pOpenArchive(const char *lpFileName, int nOpMode);
    virtual void __stdcall pCloseArchive();
    virtual bool __stdcall pExtract(const char *lpDestPath);
};
