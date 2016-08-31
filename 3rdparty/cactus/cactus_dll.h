/*!
* \file cactus_dll.h
* \brief DLL引用 声明
*
* DLL引用 声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#ifndef __CATCUS_CATCUS_DLL_H__
#define __CATCUS_CATCUS_DLL_H__

#include <cactus/cactus.h>
#include "cactus_types.h"

/****************************************************************************
*                                 DEFINE_PROC
* Inputs:
*       result: The result type; if a linkage type such as WINAPI, CALLBACK,
*       etc. is required, use DEFINE_PROC_LINKAGE
*   name: The function name
*   args: The argument list, enclosed in parentheses
* Notes:
*       There are as many of these as required for your methods to be called
****************************************************************************/

#define DEFINE_PROC(result, name, args) \
    protected: \
    typedef result (*name##_PROC) args; \
    public: \
    name##_PROC name;

/****************************************************************************
*                             DEFINE_PROC_LINKAGE
* Inputs:
*       result: Result type
*   linkage: Linkage type, e.g., CALLBACK, WINAPI
*   name: Function name
*   args: Argument list enclosed in ( )s
* Notes:
*       There are as many of these as required for your methods to be called
****************************************************************************/

#define DEFINE_PROC_LINKAGE(result, linkage, name, args) \
    protected: \
    typedef result (linkage * name##_PROC) args; \
    public: \
    name##_PROC name;

/****************************************************************************
*                               IMPLEMENT_PROC
* Inputs:
*       name: The name of the function, as used as the second argument of
*       DEFINE_PROC.
* Notes:
*       This must appear between a BEGIN_PROC_TABLE and END_PROC_TABLE
****************************************************************************/

#define IMPLEMENT_PROC(name) \
    IMPLEMENT_PROC_SPECIAL(name, #name)

/****************************************************************************
*                           IMPLEMENT_PROC_SPECIAL
* Inputs:
*       name: The name as defined by IMPLEMENT_PROC or IMPLEMENT_PROC_LINKAGE
*   externalname: The external name
* Notes:
*       This must appear between a BEGIN_PROC_TABLE and END_PROC_TABLE
****************************************************************************/

#define IMPLEMENT_PROC_SPECIAL(name, externalname) \
    name = loading ? (name##_PROC)GetProcAddress(library, externalname) : NULL; \
    { \
        DWORD err = ::GetLastError(); \
        if(loading) \
            ASSERT(name != NULL); \
        if(loading && name == NULL) \
        { \
            ::SetLastError(err); \
        } \
    }

/****************************************************************************
*                              BEGIN_PROC_TABLE
*               END_PROC_TABLE
* Effect:
*       This declares the method that initializes the method pointers.
*   There can be only one BEGIN_PROC_TABLE declaration per class.
*   Following this are some number of IMPLEMENT_PROC lines,
*   followed by END_PROC_TABLE
****************************************************************************/

#define BEGIN_PROC_TABLE() protected: virtual BOOL Define(BOOL loading) { ::SetLastError(ERROR_SUCCESS);
#define END_PROC_TABLE()   return TRUE; }

/****************************************************************************
*                              DECLARE_PROC_DLL
* Inputs:
*       myclass: The name of your subclass. This must be the same as
*         the class name you used to declare the class
*   superclass: This allows you to derive additional subclasses;
*       normally you will use the DynamicDLL class here
* Effect:
*       Defines the constructors and destructors for the subclass
****************************************************************************/

#define DECLARE_PROC_DLL(myclass, superclass) \
    public: \
    myclass() : superclass() {} \
    myclass(LPCTSTR module) : superclass(module) { Load(module); } \
    virtual ~myclass() { Free(); }


/****************************************************************************
*                                 DynamicDLL
*       This is the abstract superclass on which all other classes are based
*   It defines the basic behavior of the class.
****************************************************************************/

class CACTUS_API DynamicDLL
{
public:
    DynamicDLL()
    {
        Init();
    }
    DynamicDLL(LPCTSTR name)
    {
        Init();
    }
    virtual ~DynamicDLL() { }
    BOOL Load(LPCTSTR name)
    {
        ::SetLastError(ERROR_SUCCESS);
        ASSERT(library == NULL); // Attempt to load twice?

        if (library != NULL) {
            /* already loaded */
            //::SetLastError(ERROR_INVALID_HANDLE);
            return FALSE;
        } /* already loaded */

        library = ::LoadLibrary(name);

        if (library != NULL)
            return Define(TRUE);

        Free();
        return FALSE;
    }
    void Free()
    {
        if (library != NULL)
            ::FreeLibrary(library);

        library = NULL;
        Define(FALSE);
    }
    BOOL IsLoaded()
    {
        return library != NULL;
    }
protected:
    void Init()
    {
        library = NULL;
    }
    HINSTANCE library;
    virtual BOOL Define(BOOL loading)
    {
        ASSERT(FALSE);    // must define in subclass
        return FALSE;
    }
    // Did you forget to do a BEGIN_PROC_TABLE?
};

/****************************************************************************
*                                   Example
****************************************************************************/
//class TestDynDLL: public DynamicDLL {
//   DECLARE_PROC_DLL(TestDynDLL, DynamicDLL)
//
//   DEFINE_PROC(LRESULT, Test, (LPCTSTR, int))
//   DEFINE_PROC_LINKAGE(BOOL, WINAPI, Test2, ( ) )
//
//   BEGIN_PROC_TABLE()
//       IMPLEMENT_PROC(Test)
//       IMPLEMENT_PROC_SPECIAL(Test2, "_Test2@0")
//   END_PROC_TABLE()
//
//};


namespace cactus
{
class CACTUS_API vmsDLL
{
public:
    virtual bool is_Loaded();
    FARPROC GetProcAddress(LPCSTR pszProcName);
    virtual void Free();
    virtual bool Load(LPCTSTR pszDll);
    vmsDLL(LPCTSTR pszDll = NULL);
    virtual ~vmsDLL();

protected:
    HMODULE m_hDLL;
    static KLock _lock;
};

class CACTUS_API vmsComDLL : public vmsDLL
{
public:

    HRESULT RegisterServer(bool bRegister);

    virtual bool Load(LPCTSTR pszDll);

    vmsComDLL();
    virtual ~vmsComDLL();

protected:

    typedef HRESULT(_stdcall *FNDLLRS)(void);

    FNDLLRS m_pfnRegServer, m_pfnUnregServer;

};
} // namespace cactus

#endif