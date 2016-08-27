#pragma once
#include <Windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

enum SyncObjectTypes {
    syncObjNone     = 0,
    syncObjRequest  = 1,
    syncObjBoth     = 2
};

//////////////////////////////////////////////////////////////////////////////

template<typename T>
class SharedMemory
{
public:

    SharedMemory();
    SharedMemory(const wstring& strName, DWORD dwSize, SyncObjectTypes syncObjects, bool bCreate = true);

    ~SharedMemory();

    void Create(const wstring& strName, DWORD dwSize/* = 1*/, SyncObjectTypes syncObjects, const wstring& strUser);
    void Open(const wstring& strName, SyncObjectTypes syncObjects/* = syncObjNone*/);

    inline void Lock();
    inline void Release();
    inline void SetReqEvent();
    inline void SetRespEvent();

    inline T* Get() const;
    inline HANDLE GetReqEvent() const;
    inline HANDLE GetRespEvent() const;

    inline T& operator[](size_t index) const;
    inline T* operator->() const;
    inline T& operator*() const;
    inline SharedMemory& operator=(const T& val);

private:

    void CreateSyncObjects(const boost::shared_ptr<SECURITY_ATTRIBUTES>& sa, SyncObjectTypes syncObjects, const wstring& strName);

private:

    wstring             m_strName;
    DWORD               m_dwSize;

    boost::shared_ptr<void> m_hSharedMem;
    boost::shared_ptr<T>        m_pSharedMem;

    boost::shared_ptr<void> m_hSharedMutex;
    boost::shared_ptr<void> m_hSharedReqEvent;
    boost::shared_ptr<void> m_hSharedRespEvent;
};

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

class SharedMemoryLock
{
public:

    template <typename T> explicit SharedMemoryLock(SharedMemory<T>& sharedMem)
        : m_lock((sharedMem.Lock(), &sharedMem), boost::mem_fn(&SharedMemory<T>::Release))
    {
    }

private:

    boost::shared_ptr<void> m_lock;
};

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
SharedMemory<T>::SharedMemory()
    : m_strName(L"")
    , m_dwSize(0)
    , m_hSharedMem()
    , m_pSharedMem()
    , m_hSharedMutex()
    , m_hSharedReqEvent()
    , m_hSharedRespEvent()
{
}


template<typename T>
SharedMemory<T>::SharedMemory(const wstring& strName, DWORD dwSize, SyncObjectTypes syncObjects, bool bCreate)
    : m_strName(strName)
    , m_dwSize(dwSize)
    , m_hSharedMem()
    , m_pSharedMem()
    , m_hSharedMutex()
    , m_hSharedReqEvent()
    , m_hSharedRespEvent()
{
    if (bCreate) {
        Create(strName, dwSize, syncObjects);
    } else {
        Open(strName, syncObjects);
    }
}


template<typename T>
SharedMemory<T>::~SharedMemory()
{
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::Create(const wstring& strName, DWORD dwSize, SyncObjectTypes syncObjects, const wstring& strUser)
{
    m_strName   = strName;
    m_dwSize    = dwSize;
    boost::shared_ptr<SECURITY_ATTRIBUTES>  sa;
    EXPLICIT_ACCESS                 ea[2];
    SID_IDENTIFIER_AUTHORITY    SIDAuthCreator  = SECURITY_CREATOR_SID_AUTHORITY;
    PSID                        tmpSID = NULL;
    boost::shared_ptr<void>         creatorSID;     // PSID
    PACL                        tmpACL = NULL;
    boost::shared_ptr<ACL>              acl;
    boost::shared_ptr<void>         sd;             // PSECURITY_DESCRIPTOR
    ::ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));

    if (strUser.length() > 0) {
        // initialize an EXPLICIT_ACCESS structure for an ACE
        // the ACE will allow Everyone full access
        ea[0].grfAccessPermissions  = GENERIC_ALL;
        ea[0].grfAccessMode         = SET_ACCESS;
        ea[0].grfInheritance        = NO_INHERITANCE;
        ea[0].Trustee.TrusteeForm   = TRUSTEE_IS_NAME;
        ea[0].Trustee.TrusteeType   = TRUSTEE_IS_USER;
        ea[0].Trustee.ptstrName     = (LPTSTR)strUser.c_str();

        // create a SID for the BUILTIN\Administrators group
        if (!::AllocateAndInitializeSid(
                &SIDAuthCreator,
                1,
                SECURITY_CREATOR_OWNER_RID,
                0, 0, 0, 0, 0, 0, 0,
                &tmpSID)) {
            // TODO: error handling
            return;
        }

        creatorSID.reset(tmpSID, ::FreeSid);
        // initialize an EXPLICIT_ACCESS structure for an ACE
        // the ACE will allow the Administrators group full access
        ea[1].grfAccessPermissions  = GENERIC_ALL;
        ea[1].grfAccessMode         = SET_ACCESS;
        ea[1].grfInheritance        = NO_INHERITANCE;
        ea[1].Trustee.TrusteeForm   = TRUSTEE_IS_SID;
        ea[1].Trustee.TrusteeType   = TRUSTEE_IS_WELL_KNOWN_GROUP;
        ea[1].Trustee.ptstrName     = (LPTSTR)creatorSID.get();

        if (::SetEntriesInAcl(2, ea, NULL, &tmpACL) != ERROR_SUCCESS) {
            // TODO: error handling
            return;
        }

        acl.reset(tmpACL, ::LocalFree);
        // initialize a security descriptor
        sd.reset(::LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH), ::LocalFree);

        if (!sd) {
            // TODO: error handling
            return;
        }

        if (!::InitializeSecurityDescriptor(sd.get(), SECURITY_DESCRIPTOR_REVISION)) {
            // TODO: error handling
            return;
        }

        // add the ACL to the security descriptor
        if (!::SetSecurityDescriptorDacl(
                sd.get(),
                TRUE,       // bDaclPresent flag
                acl.get(),
                FALSE)) {   // not a default DACL
            // TODO: error handling
            return;
        }

        // initialize a security attributes structure
        sa.reset(new SECURITY_ATTRIBUTES);
        sa->nLength             = sizeof(SECURITY_ATTRIBUTES);
        sa->lpSecurityDescriptor = sd.get();
        sa->bInheritHandle      = FALSE;
    }

    m_hSharedMem = boost::shared_ptr<void>(::CreateFileMapping(
            INVALID_HANDLE_VALUE,
            sa.get(),
            PAGE_READWRITE,
            0,
            m_dwSize * sizeof(T),
            m_strName.c_str()),
                                           ::CloseHandle);
    // TODO: error handling
    //if (!m_hSharedMem) return false;
    m_pSharedMem = boost::shared_ptr<T>(static_cast<T*>(::MapViewOfFile(
                                            m_hSharedMem.get(),
                                            FILE_MAP_ALL_ACCESS,
                                            0,
                                            0,
                                            0)),
                                        ::UnmapViewOfFile);
    ::ZeroMemory(m_pSharedMem.get(), m_dwSize * sizeof(T));

    if (syncObjects > syncObjNone) CreateSyncObjects(sa, syncObjects, strName);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::Open(const wstring& strName, SyncObjectTypes syncObjects)
{
    m_strName   = strName;
    m_hSharedMem = boost::shared_ptr<void>(::OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            m_strName.c_str()),
                                           ::CloseHandle);

    // TODO: error handling
    //if (!m_hSharedMem) return false;
    if (!m_hSharedMem || (m_hSharedMem.get() == INVALID_HANDLE_VALUE)) OutputDebugString(boost::str(boost::wformat(L"Error opening shared mem %1%, error: %2%\n") % m_strName % ::GetLastError()).c_str());

    m_pSharedMem = boost::shared_ptr<T>(static_cast<T*>(::MapViewOfFile(
                                            m_hSharedMem.get(),
                                            FILE_MAP_ALL_ACCESS,
                                            0,
                                            0,
                                            0)),
                                        ::UnmapViewOfFile);

    if (!m_pSharedMem) OutputDebugString(boost::str(boost::wformat(L"Error mapping shared mem %1%, error: %2%\n") % m_strName % ::GetLastError()).c_str());

    if (syncObjects > syncObjNone) CreateSyncObjects(boost::shared_ptr<SECURITY_ATTRIBUTES>(), syncObjects, strName);

    //if (!m_pSharedMem) return false;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::Lock()
{
    if (!m_hSharedMutex) return;

    ::WaitForSingleObject(m_hSharedMutex.get(), INFINITE);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::Release()
{
    if (!m_hSharedMutex) return;

    ::ReleaseMutex(m_hSharedMutex.get());
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::SetReqEvent()
{
    if (!m_hSharedReqEvent) {
        OutputDebugString(boost::str(boost::wformat(L"Req Event %1% is null!") % m_strName).c_str());
        return;
    }

    if (!::SetEvent(m_hSharedReqEvent.get())) {
        OutputDebugString(boost::str(boost::wformat(L"SetEvent %1% failed: %2%!\n") % m_strName % ::GetLastError()).c_str());
    }
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::SetRespEvent()
{
    if (!m_hSharedRespEvent) {
        OutputDebugString(boost::str(boost::wformat(L"Resp Event %1% is null!") % m_strName).c_str());
        return;
    }

    ::SetEvent(m_hSharedRespEvent.get());
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
T* SharedMemory<T>::Get() const
{
    return m_pSharedMem.get();
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
HANDLE SharedMemory<T>::GetReqEvent() const
{
    return m_hSharedReqEvent.get();
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
HANDLE SharedMemory<T>::GetRespEvent() const
{
    return m_hSharedRespEvent.get();
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
T& SharedMemory<T>::operator[](size_t index) const
{
    return *(m_pSharedMem.get() + index);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
T* SharedMemory<T>::operator->() const
{
    return m_pSharedMem.get();
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
T& SharedMemory<T>::operator*() const
{
    return *m_pSharedMem;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
SharedMemory<T>& SharedMemory<T>::operator=(const T& val)
{
    *m_pSharedMem = val;
    return *this;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<typename T>
void SharedMemory<T>::CreateSyncObjects(const boost::shared_ptr<SECURITY_ATTRIBUTES>& sa, SyncObjectTypes syncObjects, const wstring& strName)
{
    if (syncObjects >= syncObjRequest) {
        m_hSharedMutex = boost::shared_ptr<void>(
                             ::CreateMutex(sa.get(), FALSE, (wstring(L"") + strName + wstring(L"_mutex")).c_str()),
                             ::CloseHandle);
        OutputDebugString(boost::str(boost::wformat(L"m_hSharedMutex %1%: %2%\n") % m_strName % (DWORD)(m_hSharedMutex.get())).c_str());
        m_hSharedReqEvent = boost::shared_ptr<void>(
                                ::CreateEvent(sa.get(), FALSE, FALSE, (wstring(L"") + strName + wstring(L"_req_event")).c_str()),
                                ::CloseHandle);
        OutputDebugString(boost::str(boost::wformat(L"m_hSharedReqEvent %1%: %2%\n") % m_strName % (DWORD)(m_hSharedReqEvent.get())).c_str());
    }

    if (syncObjects >= syncObjBoth) {
        m_hSharedRespEvent = boost::shared_ptr<void>(
                                 ::CreateEvent(sa.get(), FALSE, FALSE, (wstring(L"") + strName + wstring(L"_resp_event")).c_str()),
                                 ::CloseHandle);
        OutputDebugString(boost::str(boost::wformat(L"m_hSharedRespEvent %1%: %2%\n") % m_strName % (DWORD)(m_hSharedRespEvent.get())).c_str());
    }
}

//////////////////////////////////////////////////////////////////////////////
