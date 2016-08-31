
#include <cactus/7zHelper.h>
#include <cactus/cactus_7z.h>
#include <cactus/PropVariant.h>
#include <cactus/cactus_string.h>
#include <cactus/cactus_file.h>



using namespace cactus;

bool CInFile::Open()
{
    HANDLE hFile = CreateFile(
                       m_lpFileName.c_str(),
                       GENERIC_READ,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL
                   );

    if (hFile != INVALID_HANDLE_VALUE) {
        m_hFile = hFile;
        return true;
    }

    return false;
}

bool CInFile::Create()
{
    HANDLE hFile = CreateFile(
                       m_lpFileName.c_str(),
                       GENERIC_WRITE,
                       FILE_SHARE_READ,
                       NULL,
                       CREATE_ALWAYS,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL
                   );

    if (hFile != INVALID_HANDLE_VALUE) {
        m_hFile = hFile;
        return true;
    }

    return false;
}

void CInFile::Close()
{
    if (m_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

CInFile::CInFile(const char *lpFileName)
{
    m_nRefCount = 1;
    m_hFile = INVALID_HANDLE_VALUE;
    m_lpFileName = cactus::convert_to<string16>(lpFileName);
}

CInFile::~CInFile()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
        CloseHandle(m_hFile);
}

unsigned __int64 CInFile::GetSize()
{
    DWORD dwLoPart, dwHiPart;
    dwLoPart = GetFileSize(m_hFile, &dwHiPart);
    return ((unsigned __int64)dwHiPart) * 0x100000000ull + (unsigned __int64)dwLoPart;
}

const char *CInFile::GetName()
{
    return convert_to<string8>(m_lpFileName).c_str();
}


HRESULT __stdcall CInFile::QueryInterface(const IID &iid, void ** ppvObject)
{
    *ppvObject = NULL;

    if (iid == IID_IInStream) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

ULONG __stdcall CInFile::AddRef()
{
    return ++m_nRefCount;
}

ULONG __stdcall CInFile::Release()
{
    if (--m_nRefCount == 0) {
        delete this;
        return 0;
    }

    return m_nRefCount;
}



HRESULT __stdcall CInFile::Read(void *data, unsigned int size, unsigned int *processedSize)
{
    DWORD dwRead;

    if (ReadFile(m_hFile, data, size, &dwRead, NULL)) {
        if (processedSize)
            *processedSize = dwRead;

        return S_OK;
    }

    return E_FAIL;
}

HRESULT __stdcall CInFile::Seek(__int64 offset, unsigned int seekOrigin, unsigned __int64 *newPosition)
{
    DWORD hi, lo;
    hi = (DWORD)(offset >> 32);
    lo = (DWORD)offset;
    lo = SetFilePointer(m_hFile, lo, (PLONG)&hi, seekOrigin);

    if ((lo == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))
        return E_FAIL;
    else {
        if (newPosition)
            *newPosition = ((unsigned __int64)hi) * 0x100000000ull + (unsigned __int64)lo;

        return S_OK;
    }
}


bool COutFile::Open()
{
    HANDLE hFile = CreateFile(
                       m_lpFileName.c_str(),
                       GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       CREATE_ALWAYS,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL
                   );

    if (hFile != INVALID_HANDLE_VALUE) {
        m_hFile = hFile;
        return true;
    }

    return false;
}

void COutFile::Close()
{
    if (m_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

bool COutFile::SetTime(const FILETIME* lpCreationTime, const FILETIME* lpLastAccessTime, const FILETIME* lpLastWriteTime)
{
    return (bool)SetFileTime(m_hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
}

bool COutFile::SetAttributes(DWORD dwFileAttributes)
{
    return (bool)SetFileAttributes(m_lpFileName.c_str(), dwFileAttributes);
}

COutFile::COutFile(const char *lpFileName)
{
    //USES_CONVERSION;
    m_nRefCount = 1;
    m_hFile = INVALID_HANDLE_VALUE;
    //m_lpFileName = CA2CT(lpFileName);
    m_lpFileName = cactus::convert_to<string16>(lpFileName);
}

COutFile::~COutFile()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
        CloseHandle(m_hFile);
}


HRESULT __stdcall COutFile::QueryInterface(const IID &iid, void ** ppvObject)
{
    *ppvObject = NULL;

    if (iid == IID_ISequentialOutStream) {
        *ppvObject = (void*)(ISequentialOutStream*)this;
        AddRef();
        return S_OK;
    }

    if (iid == IID_IOutStream) {
        *ppvObject = (void*)(IOutStream*)this;
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

ULONG __stdcall COutFile::AddRef()
{
    return ++m_nRefCount;
}

ULONG __stdcall COutFile::Release()
{
    if (--m_nRefCount == 0) {
        delete this;
        return 0;
    }

    return m_nRefCount;
}


HRESULT __stdcall COutFile::Write(const void *data, unsigned int size, unsigned int *processedSize)
{
    DWORD dwWritten;

    if (WriteFile(m_hFile, data, size, &dwWritten, NULL)) {
        if (processedSize)
            *processedSize = dwWritten;

        return S_OK;
    }

    return E_FAIL;
}

HRESULT __stdcall COutFile::Seek(__int64 offset, unsigned int seekOrigin, unsigned __int64 *newPosition)
{
    DWORD hi, lo;
    hi = (DWORD)(offset >> 32);
    lo = (DWORD)offset;
    lo = SetFilePointer(m_hFile, lo, (PLONG)&hi, seekOrigin);

    if ((lo == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR))
        return E_FAIL;
    else {
        if (newPosition)
            *newPosition = ((unsigned __int64)hi) * 0x100000000ull + (unsigned __int64)lo;

        return S_OK;
    }
}

HRESULT __stdcall COutFile::SetSize(__int64 newSize)
{
    return S_OK;
}




CArchiveExtractCallback::CArchiveExtractCallback(
    SevenZipArchive *pArchive,
    const char *lpDestPath)
{
    m_pArchive = pArchive;
    m_nRefCount = 1;
    m_lpDestPath = lpDestPath;
    m_nLastProcessed = (unsigned __int64) - 1;
    m_nState = -1;
}


CArchiveExtractCallback::~CArchiveExtractCallback()
{
}

ULONG __stdcall CArchiveExtractCallback::AddRef()
{
    return ++m_nRefCount;
}

ULONG __stdcall CArchiveExtractCallback::Release()
{
    if (--m_nRefCount == 0) {
        delete this;
        return 0;
    }

    return m_nRefCount;
}


HRESULT __stdcall CArchiveExtractCallback::QueryInterface(const IID &iid, void ** ppvObject)
{
    *ppvObject = NULL;

    if (iid == IID_IArchiveExtractCallback) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}


HRESULT __stdcall CArchiveExtractCallback::SetTotal(unsigned __int64 total)
{
    m_nLastProcessed = (unsigned __int64) - 1;
    return S_OK;
}

HRESULT CArchiveExtractCallback::SetCompleted(const unsigned __int64* completeValue)
{
    if ((__int64)m_nLastProcessed != -1) {
        unsigned __int64 diff = *completeValue - m_nLastProcessed;
        m_nLastProcessed = *completeValue;
    }

    return S_OK;
}


HRESULT __stdcall CArchiveExtractCallback::GetStream(
    unsigned int index,
    ISequentialOutStream **outStream,
    int askExtractMode
)
{
    CPropVariant value;
    IInArchive *archive = m_pArchive->m_pArchive;

    if (askExtractMode == 0) { //extract
        if (archive->GetProperty(index, kpidPath, &value) != S_OK)
            return S_OK; //!!! to return error

        char szArcFileName[MAX_PATH];
        char szFullName[MAX_PATH];

        if (value.vt == VT_BSTR)
            WideCharToMultiByte(CP_OEMCP, 0, value.bstrVal, -1, szArcFileName, MAX_PATH, NULL, NULL);
        else {
            //strcpy (szArcFileName, FSF.PointToName (m_pArchive->m_lpFileName));
            //CutTo (szArcFileName, '.', true);
            ATLASSERT(FALSE);
        }

        strcpy(szFullName, m_lpDestPath.c_str());
        PathAppendA(szFullName, szArcFileName);

        if ((int)m_nLastProcessed == -1)
            m_nLastProcessed = 0;

        FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
        DWORD dwFileAttributes = 0;
        memset(&ftCreationTime, 0, sizeof(FILETIME));
        memset(&ftLastAccessTime, 0, sizeof(FILETIME));
        memset(&ftLastWriteTime, 0, sizeof(FILETIME));

        if (archive->GetProperty(index, kpidAttrib, &value) == S_OK) {
            if (value.vt == VT_UI4)
                dwFileAttributes = value.ulVal;
        }

        if (archive->GetProperty(index, kpidCTime, &value) == S_OK) {
            if (value.vt == VT_FILETIME)
                memcpy(&ftCreationTime, &value.filetime, sizeof(FILETIME));
        }

        if (archive->GetProperty(index, kpidATime, &value) == S_OK) {
            if (value.vt == VT_FILETIME)
                memcpy(&ftLastAccessTime, &value.filetime, sizeof(FILETIME));
        }

        if (archive->GetProperty(index, kpidMTime, &value) == S_OK) {
            if (value.vt == VT_FILETIME)
                memcpy(&ftLastWriteTime, &value.filetime, sizeof(FILETIME));
        }

        bool bIsFolder = false;

        if (archive->GetProperty(index, kpidIsDir, &value) == S_OK) {
            if (value.vt == VT_BOOL)
                bIsFolder = (value.boolVal == VARIANT_TRUE);
        }

        if (bIsFolder || dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            *outStream = NULL;
            //CreateDirEx (szFullName, dwFileAttributes);
            Path::CreateDirEx(szFullName);
        } else {
            Path::CreateDirs(szFullName);
            COutFile *file = new COutFile(szFullName);

            if (file->Open()) {
                file->SetAttributes(dwFileAttributes);
                file->SetTime(&ftCreationTime, &ftLastAccessTime, &ftLastWriteTime);
                *outStream = file;
                m_files.push_back(szFullName);
            } else
                delete file;
        }
    } else
        *outStream = NULL;

    return S_OK;
}

HRESULT __stdcall CArchiveExtractCallback::PrepareOperation(int askExtractMode)
{
    return S_OK;
}

HRESULT __stdcall CArchiveExtractCallback::SetOperationResult(int resultEOperationResult)
{
    return S_OK;
}

ULONG __stdcall CArchiveOpenCallback::AddRef()
{
    return ++m_nRefCount;
}

ULONG __stdcall CArchiveOpenCallback::Release()
{
    if (--m_nRefCount == 0) {
        delete this;
        return 0;
    }

    return m_nRefCount;
}


HRESULT __stdcall CArchiveOpenCallback::QueryInterface(const IID &iid, void ** ppvObject)
{
    *ppvObject = NULL;

    if (iid == IID_IArchiveOpenCallback) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    if (iid == IID_IArchiveOpenVolumeCallback) {
        if (!m_pArchiveOpenVolumeCallback)
            m_pArchiveOpenVolumeCallback = new CArchiveOpenVolumeCallback(m_pArchive);

        m_pArchiveOpenVolumeCallback->AddRef();
        *ppvObject = m_pArchiveOpenVolumeCallback;
        return S_OK;
    }

    return E_NOINTERFACE;
}

CArchiveOpenCallback::CArchiveOpenCallback(SevenZipArchive *pArchive)
{
    m_nRefCount = 1;
    m_pArchive = pArchive;
    m_pArchiveOpenVolumeCallback = NULL;
    m_bProgressMessage = false;
    m_dwStartTime = GetTickCount();
}

CArchiveOpenCallback::~CArchiveOpenCallback()
{
    if (m_pArchiveOpenVolumeCallback)
        m_pArchiveOpenVolumeCallback->Release();
}

HRESULT __stdcall CArchiveOpenCallback::SetTotal(const UInt64 *files, const UInt64 *bytes)
{
    return S_OK;
}


bool CheckForEsc()
{
    bool EC = false;
    INPUT_RECORD rec;
    DWORD ReadCount;

    while (true) {
        PeekConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &ReadCount);

        if (ReadCount == 0)
            break;

        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &ReadCount);

        if (rec.EventType == KEY_EVENT) {
            if ((rec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) &&
                rec.Event.KeyEvent.bKeyDown)
                EC = true;
        }
    }

    return EC;
}


HRESULT __stdcall CArchiveOpenCallback::SetCompleted(const UInt64 *files, const UInt64 *bytes)
{
    if (CheckForEsc())
        return E_FAIL;

    if (!(*files & 0x1f)  && (GetTickCount() - m_dwStartTime > 500)) {
        m_bProgressMessage = true;
    }

    return S_OK;
}

CArchiveOpenVolumeCallback::CArchiveOpenVolumeCallback(SevenZipArchive *pArchive)
{
    m_nRefCount = 1;
    m_pArchive = pArchive;
    m_pVolumeFile = NULL;
}

CArchiveOpenVolumeCallback::~CArchiveOpenVolumeCallback()
{
}


ULONG __stdcall CArchiveOpenVolumeCallback::AddRef()
{
    return ++m_nRefCount;
}

ULONG __stdcall CArchiveOpenVolumeCallback::Release()
{
    if (--m_nRefCount == 0) {
        delete this;
        return 0;
    }

    return m_nRefCount;
}


HRESULT __stdcall CArchiveOpenVolumeCallback::QueryInterface(const IID &iid, void ** ppvObject)
{
    *ppvObject = NULL;

    if (iid == IID_IArchiveOpenVolumeCallback) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}


HRESULT __stdcall CArchiveOpenVolumeCallback::GetProperty(PROPID propID, PROPVARIANT *value)
{
    if (propID == kpidName) {
        wchar_t wszNameOnly[MAX_PATH];
#if 0

        if (m_pVolumeFile)
            MultiByteToWideChar(CP_OEMCP, 0, FSF.PointToName(m_pVolumeFile->GetName()), -1, wszNameOnly, MAX_PATH);
        else
            MultiByteToWideChar(CP_OEMCP, 0, FSF.PointToName(m_pArchive->m_pInFile->GetName()), -1, wszNameOnly, MAX_PATH);

#endif
        value->vt = VT_BSTR;
        value->bstrVal = SysAllocString(wszNameOnly);
    }

    if (propID == kpidSize) {
        value->vt = VT_UI8;

        if (m_pVolumeFile)
            value->uhVal.QuadPart = m_pVolumeFile->GetSize();
        else
            value->uhVal.QuadPart = m_pArchive->m_pInFile->GetSize();
    }

    return S_OK;
}

HRESULT __stdcall CArchiveOpenVolumeCallback::GetStream(const wchar_t *name, IInStream **inStream)
{
    char szFullName[MAX_PATH];
    char szFileName[MAX_PATH];
    WideCharToMultiByte(CP_OEMCP, 0, name, -1, szFileName, MAX_PATH, NULL, NULL);
    strcpy(szFullName, m_pArchive->m_lpFileName.c_str());
    strcat(szFullName, szFileName);
    CInFile *file = new CInFile(szFullName);
    bool bResult = file->Open();
    *inStream = file;
    m_pVolumeFile = file;
    return bResult ? S_OK : S_FALSE;
}

