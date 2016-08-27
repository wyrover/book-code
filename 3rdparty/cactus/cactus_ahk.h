/*!
* \file cactus_ahk.h
* \brief AHK DLL包装声明
*
* AHK DLL包装声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include <cactus/cactus.h>
#include "cactus/cactus_string.h"
#include "cactus/cactus_dll.h"

namespace cactus
{

class CACTUS_API CAHKDLL: public vmsDLL
{
public:
    CAHKDLL(LPCTSTR pszDll = NULL);
    virtual ~CAHKDLL();
public:
    virtual bool Load(LPCTSTR pszDll);
    UINT_PTR ahkdll(LPTSTR fileName, LPTSTR argv, LPTSTR args);
    UINT_PTR ahktextdll(LPTSTR textstr, LPTSTR argv, LPTSTR args);
protected:
    typedef UINT_PTR(*LPFN_AHKDLL)(LPTSTR fileName, LPTSTR argv, LPTSTR args);
    typedef UINT_PTR(*LPFN_AHKTEXTDLL)(LPTSTR textstr, LPTSTR argv, LPTSTR args);

protected:
    LPFN_AHKDLL ahkdll_;
    LPFN_AHKTEXTDLL ahktext_dll_;

};

} // namespace cactus