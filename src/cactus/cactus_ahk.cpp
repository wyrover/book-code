/*!
* \file cactus_ahk.cpp
* \brief AHK DLL包装实现
*
* AHK DLL包装实现
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include "cactus/cactus_ahk.h"

namespace cactus
{

CAHKDLL::CAHKDLL(LPCTSTR pszDll /*= NULL*/)
{
}

CAHKDLL::~CAHKDLL()
{
}


UINT_PTR CAHKDLL::ahkdll(LPTSTR fileName, LPTSTR argv, LPTSTR args)
{
    return ahkdll_(fileName, argv, args);
}

UINT_PTR CAHKDLL::ahktextdll(LPTSTR textstr, LPTSTR argv, LPTSTR args)
{
    return ahktext_dll_(textstr, argv, args);
}

bool CAHKDLL::Load(LPCTSTR pszDll)
{
    if (__super::Load(pszDll)) {
        ahkdll_ = (LPFN_AHKDLL)this->GetProcAddress("ahkdll");
        ahktext_dll_ = (LPFN_AHKTEXTDLL)this->GetProcAddress("ahktextdll");
    }

    if (ahkdll_ && ahktext_dll_)
        return true;

    return false;
}

} // namespace cactus