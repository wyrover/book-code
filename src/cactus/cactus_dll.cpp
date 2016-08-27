/*!
* \file cactus_dll.cpp
* \brief DLL引用 实现
*
* DLL引用 实现
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include "cactus/cactus_dll.h"

namespace cactus
{
cactus::KLock vmsDLL::_lock;

vmsDLL::vmsDLL(LPCTSTR pszDll)
{
    m_hDLL = NULL;

    if (pszDll)
        Load(pszDll);
}

vmsDLL::~vmsDLL()
{
}

bool vmsDLL::Load(LPCTSTR pszDll)
{
    KLocker locker(_lock);
    Free();
    m_hDLL = LoadLibrary(pszDll);

    if (m_hDLL == NULL)
        return false;

    return true;
}

void vmsDLL::Free()
{
    KLocker locker(_lock);

    if (m_hDLL) {
        FreeLibrary(m_hDLL);
        m_hDLL = NULL;
    }
}

FARPROC vmsDLL::GetProcAddress(LPCSTR pszProcName)
{
    KLocker locker(_lock);

    if (m_hDLL == NULL)
        return NULL;

    return ::GetProcAddress(m_hDLL, pszProcName);
}

bool vmsDLL::is_Loaded()
{
    return m_hDLL != NULL;
}




} // namespace cactus

namespace cactus
{
vmsComDLL::vmsComDLL()
{
    CoInitialize(NULL);
    m_pfnRegServer = m_pfnUnregServer = NULL;
}

vmsComDLL::~vmsComDLL()
{
    CoUninitialize();
}

bool vmsComDLL::Load(LPCTSTR pszDll)
{
    KLocker locker(_lock);

    if (false == vmsDLL::Load(pszDll))
        return false;

    m_pfnRegServer = (FNDLLRS) GetProcAddress("DllRegisterServer");
    m_pfnUnregServer = (FNDLLRS) GetProcAddress("DllUnregisterServer");
    return m_pfnRegServer != NULL && m_pfnUnregServer != NULL;
}

HRESULT vmsComDLL::RegisterServer(bool bRegister)
{
    KLocker locker(_lock);
    FNDLLRS pfn = bRegister ? m_pfnRegServer : m_pfnUnregServer;

    if (pfn == NULL)
        return E_NOTIMPL;

    return pfn();
}

} // namespace cactus

