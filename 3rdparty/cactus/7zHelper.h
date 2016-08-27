#pragma once

#include <cactus/cactus_types.h>
#include <7z.headers/7zip/Archive/IArchive.h>
#include <7z.headers/7zip/Common/FileStreams.h>
#include <string>
#include <vector>

class SevenZipArchive;

class CInFile : //public IUnknown,
    public IInStream
{

private:

    HANDLE m_hFile;
    int m_nRefCount;
    string16 m_lpFileName;

public:

    CInFile(const char *lpFileName);
    ~CInFile();

    bool Open();
    bool Create();
    void Close();

    unsigned __int64 GetSize();
    const char *GetName();

    virtual HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    virtual HRESULT __stdcall Read(void *data, unsigned int size, unsigned int *processedSize);
    virtual HRESULT __stdcall Seek(__int64 offset, unsigned int seekOrigin, unsigned __int64 *newPosition);
};


class COutFile : public IOutStream
{

private:
    HANDLE m_hFile;
    int m_nRefCount;
    string16 m_lpFileName;

public:
    COutFile(const char *lpFileName);
    ~COutFile();

    bool Open();
    void Close();

    bool SetTime(const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime);
    bool SetAttributes(DWORD dwFileAttributes);

    virtual HRESULT __stdcall QueryInterface(const IID &iid, void ** ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    //ISequentialOutStream
    virtual HRESULT __stdcall Write(const void *data, unsigned int size, unsigned int* processedSize);

    //IOutStream

    virtual HRESULT __stdcall Seek(__int64 offset, unsigned int seekOrigin, unsigned __int64 *newPosition);
    virtual HRESULT __stdcall SetSize(__int64 newSize);
};

class CArchiveOpenVolumeCallback : public IArchiveOpenVolumeCallback
{
private:

    int m_nRefCount;
    SevenZipArchive *m_pArchive;

    CInFile *m_pVolumeFile;

public:

    CArchiveOpenVolumeCallback(SevenZipArchive *pArchive);
    ~CArchiveOpenVolumeCallback();


    virtual HRESULT __stdcall QueryInterface(const IID &iid, void ** ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    // IArchiveOpenVolumeCallback

    virtual HRESULT __stdcall GetProperty(PROPID propID, PROPVARIANT *value);
    virtual HRESULT __stdcall GetStream(const wchar_t *name, IInStream **inStream);

};

#define TYPE_FILE       1
#define TYPE_LISTING    2

class CArchiveOpenCallback : public IArchiveOpenCallback/*, public IArchiveOpenVolumeCallback*/
{

private:
    int m_nRefCount;
    CArchiveOpenVolumeCallback *m_pArchiveOpenVolumeCallback;

    bool m_bProgressMessage;
    DWORD m_dwStartTime;

    SevenZipArchive *m_pArchive;

    HANDLE m_hScreen;

public:

    CArchiveOpenCallback(SevenZipArchive *pArchive);
    ~CArchiveOpenCallback();


    virtual HRESULT __stdcall QueryInterface(const IID &iid, void ** ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    // IArchiveOpenCallback
    virtual HRESULT __stdcall SetTotal(const UInt64 *files, const UInt64 *bytes);
    virtual HRESULT __stdcall SetCompleted(const UInt64 *files, const UInt64 *bytes);
};

class CArchiveExtractCallback : public IArchiveExtractCallback
{

public:
    int m_nRefCount;
    SevenZipArchive *m_pArchive;

    int m_nItemsNumber;
    std::string m_lpDestPath;
    unsigned __int64 m_nLastProcessed;
    int m_nState; //0 - skip, 1 - extract

    std::vector<std::string> m_files;

public:
    CArchiveExtractCallback(SevenZipArchive *pArchive, const char *lpDestPath);
    ~CArchiveExtractCallback();

    virtual HRESULT __stdcall QueryInterface(const IID &iid, void ** ppvObject);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    //IProgress
    virtual HRESULT __stdcall SetTotal(unsigned __int64 total);
    virtual HRESULT __stdcall SetCompleted(const unsigned __int64* completeValue);

    //IArchiveExtractCallback
    virtual HRESULT __stdcall GetStream(unsigned int index, ISequentialOutStream **outStream, int askExtractMode);
    // GetStream OUT: S_OK - OK, S_FALSE - skeep this file
    virtual HRESULT __stdcall PrepareOperation(int askExtractMode);
    virtual HRESULT __stdcall SetOperationResult(int resultEOperationResult);
};


