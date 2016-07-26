/***
*dbgrpt.c - Debug CRT Reporting Functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#ifdef _DEBUG

#include <internal.h>
#include <crtdbg.h>
#include <mtdll.h>
#include <malloc.h>

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention

volatile _CRT_REPORT_HOOK_M __declspec(appdomain) __pfnReportHookManaged = (_CRT_REPORT_HOOK_M) _encoded_null();

static int __cdecl _CrtReportHook_managed_thunk(int i, char *pCh, int *pInt)
{
    // Avoid thread-sync issues
    _CRT_REPORT_HOOK_M pfnReportHookManaged = (_CRT_REPORT_HOOK_M) _decode_pointer(__pfnReportHookManaged);

    if (pfnReportHookManaged != NULL)
    {
        return (*pfnReportHookManaged)(i, pCh, pInt);
    }
    else
    {
        return 0;
    }
}

static void __clrcall _CrtReportHook_managed_cleanup(void)
{
    _CRT_REPORT_HOOK pfnReportHook=_CrtGetReportHook();
    _CRT_REPORT_HOOK_M enull = (_CRT_REPORT_HOOK_M) _encoded_null();
    if(pfnReportHook==_CrtReportHook_managed_thunk && __pfnReportHookManaged!=enull)
    {
        _CrtSetReportHook((_CRT_REPORT_HOOK)NULL);
    }
    __pfnReportHookManaged=enull;
}

/***
*_CRT_REPORT_HOOK_M _CrtSetReportHook() - set client report hook
*
*Purpose:
*       set client report hook
*
*Entry:
*       _CRT_REPORT_HOOK_M pfnNewHook - new report hook
*
*Exit:
*       return previous hook
*
*Exceptions:
*
*******************************************************************************/
_MRTIMP _CRT_REPORT_HOOK_M __cdecl _CrtSetReportHook(
        _CRT_REPORT_HOOK_M pfnNewHook
        )
{
    if(_atexit_m_appdomain(_CrtReportHook_managed_cleanup)!=0)
    {
        return NULL;
    }

    _CRT_REPORT_HOOK_M pfnOldHook = (_CRT_REPORT_HOOK_M) _decode_pointer(__pfnReportHookManaged);
    __pfnReportHookManaged = (_CRT_REPORT_HOOK_M) _encode_pointer(pfnNewHook);
    _CrtSetReportHook((_CRT_REPORT_HOOK)_CrtReportHook_managed_thunk);
    return pfnOldHook;
}

_MRTIMP _CRT_REPORT_HOOK __cdecl _CrtSetReportHook(
        int pfnNewHook
        )
{
    _VALIDATE_RETURN(pfnNewHook == NULL, EINVAL, NULL);

    return _CrtSetReportHook((_CRT_REPORT_HOOK)NULL);
}

/***
static int __cdecl _CrtAllocHook_managed_thunk
(
    int allocType,
    void *userData,
    size_t size,
    int blockType,
    long requestNumber,
    const unsigned char *filename,
    int lineNumber
)
*
*Purpose:
*       thunk m-> client alloc hook
*
*Exceptions:
*
*******************************************************************************/
volatile _CRT_ALLOC_HOOK_M __declspec(appdomain) __pfnAllocHookManaged = (_CRT_ALLOC_HOOK_M) _encoded_null();

static int __cdecl _CrtAllocHook_managed_thunk
(
    int allocType,
    void *userData,
    size_t size,
    int blockType,
    long requestNumber,
    const unsigned char *filename,
    int lineNumber
)
{
    // Avoid thread-sync issues
    _CRT_ALLOC_HOOK_M pfnAllocHookManaged= (_CRT_ALLOC_HOOK_M) _decode_pointer(__pfnAllocHookManaged);

    if (pfnAllocHookManaged != NULL)
    {
        return (*pfnAllocHookManaged)(allocType, userData, size, blockType, requestNumber, filename, lineNumber);
    }
    else
    {
        return 0;
    }
}

static void __clrcall _CrtAllocHook_managed_cleanup(void)
{
    _CRT_ALLOC_HOOK pfnAllocHook=_CrtGetAllocHook();
    _CRT_ALLOC_HOOK_M enull = (_CRT_ALLOC_HOOK_M) _encoded_null();
    if(pfnAllocHook==_CrtAllocHook_managed_thunk && __pfnAllocHookManaged!=enull)
    {
        _CrtSetAllocHook((_CRT_ALLOC_HOOK)NULL);
    }
    __pfnAllocHookManaged=enull;
}

/***
*_CRT_ALLOC_HOOK_M _CrtSetAllocHook() - set client report hook
*
*Purpose:
*       set client alloc hook
*
*Entry:
*       _CRT_ALLOC_HOOK_M pfnNewHook - new alloc hook
*
*Exit:
*       return previous hook
*
*Exceptions:
*
*******************************************************************************/
_MRTIMP _CRT_ALLOC_HOOK_M __cdecl _CrtSetAllocHook
(
    _CRT_ALLOC_HOOK_M pfnNewHook
)
{
    if(_atexit_m_appdomain(_CrtAllocHook_managed_cleanup)!=0)
    {
        return NULL;
    }

    _CRT_ALLOC_HOOK_M pfnOldHook = (_CRT_ALLOC_HOOK_M) _decode_pointer(__pfnAllocHookManaged);
    __pfnAllocHookManaged = (_CRT_ALLOC_HOOK_M) _encode_pointer(pfnNewHook);
    _CrtSetAllocHook((_CRT_ALLOC_HOOK)_CrtReportHook_managed_thunk);
    return pfnOldHook;
}

_MRTIMP _CRT_ALLOC_HOOK __cdecl _CrtSetAllocHook
(
    int pfnNewHook
)
{
    _VALIDATE_RETURN(pfnNewHook == NULL, EINVAL, NULL);

    return _CrtSetAllocHook((_CRT_ALLOC_HOOK)NULL);
}

/***
static void __cdecl _CrtDumpClient_managed_thunk
(
    void *userPortion,
    size_t blockSize
)
*
*Purpose:
*       thunk m-> client dump hook
*
*Exceptions:
*
*******************************************************************************/
volatile _CRT_DUMP_CLIENT_M __declspec(appdomain) __pfnDumpClientManaged = (_CRT_DUMP_CLIENT_M) _encoded_null();

static void __cdecl _CrtDumpClient_managed_thunk
(
    void *userPortion,
    size_t blockSize
)
{
    // Avoid thread-sync issues
    _CRT_DUMP_CLIENT_M pfnDumpClientManaged = (_CRT_DUMP_CLIENT_M) _decode_pointer(__pfnDumpClientManaged);

    if (pfnDumpClientManaged != NULL)
    {
        (*pfnDumpClientManaged)(userPortion, blockSize);
    }
}

static void __clrcall _CrtDumpClient_managed_cleanup(void)
{
    _CRT_DUMP_CLIENT pfnDumpClient=_CrtGetDumpClient();
    _CRT_DUMP_CLIENT_M enull = (_CRT_DUMP_CLIENT_M) _encoded_null();
    if(pfnDumpClient==_CrtDumpClient_managed_thunk && __pfnDumpClientManaged!=enull)
    {
        _CrtSetDumpClient((_CRT_DUMP_CLIENT)NULL);
    }
    __pfnDumpClientManaged=enull;
}

/***
*_CRT_DUMP_CLIENT_M _CrtSetDumpClient() - set client dump hook
*
*Purpose:
*       set client dump hook
*
*Entry:
*       _CRT_DUMP_CLIENT_M pfnNewHook - new dump hook
*
*Exit:
*       return previous hook
*
*Exceptions:
*
*******************************************************************************/
_MRTIMP _CRT_DUMP_CLIENT_M __cdecl _CrtSetDumpClient
(
    _CRT_DUMP_CLIENT_M pfnNewHook
)
{
    if(_atexit_m_appdomain(_CrtDumpClient_managed_cleanup)!=0)
    {
        return NULL;
    }

    _CRT_DUMP_CLIENT_M pfnOldHook = (_CRT_DUMP_CLIENT_M) _decode_pointer(__pfnDumpClientManaged);
    __pfnDumpClientManaged = (_CRT_DUMP_CLIENT_M) _encode_pointer(pfnNewHook);
    _CrtSetDumpClient((_CRT_DUMP_CLIENT)_CrtDumpClient_managed_thunk);
    return pfnOldHook;
}

_MRTIMP _CRT_DUMP_CLIENT __cdecl _CrtSetDumpClient
(
    int pfnNewHook
)
{
    _VALIDATE_RETURN(pfnNewHook == NULL, EINVAL, NULL);

    return _CrtSetDumpClient((_CRT_DUMP_CLIENT)NULL);
}

/*
 * The implementation here is not ideal. The problem is when to free the list.
 * If we free it before process shutdown, any problem after that could not be
 * reported.
 */

/***
* CCrtReportHook2DB Class for keeping track of all the report hooks.
*******************************************************************************/

/*
FUTURE: I did a trivial templatisation of this code to support W hooks. Cleanup to bind into a single template class would be nice.
*/

template<typename HookType>
static int __cdecl _CrtSetReportHook2_thunk
(
    int mode,
    HookType pfnNewHook
);

template<>
static int __cdecl _CrtSetReportHook2_thunk
(
    int mode,
    _CRT_REPORT_HOOK pfnNewHook
)
{
    return _CrtSetReportHook2(mode, pfnNewHook);
}

template<>
static int __cdecl _CrtSetReportHook2_thunk
(
    int mode,
    _CRT_REPORT_HOOKW pfnNewHook
)
{
    return _CrtSetReportHookW2(mode, pfnNewHook);
}

template<typename HookType, typename CharType>
class CCrtReportHook2DB
{
public:
    struct CReportHookList
    {
        CReportHookList *m_pNext;
        CReportHookList *m_pPrev;
        HookType m_Hook;
        int m_Refcount;

        CReportHookList()
            : m_pNext(NULL), m_pPrev(NULL), m_Hook(NULL), m_Refcount(0)
        {
        }

        static CReportHookList* create()
        {
            return reinterpret_cast<CReportHookList*>(calloc(1, sizeof(CReportHookList)));
        }

        static void destroy(CReportHookList* pReportHookList)
        {
            free(pReportHookList);
        }
    };

    CReportHookList *findHook(HookType pfnHook)
    {
        CReportHookList *retval=NULL;

        for (retval = m_HeadNode.m_pNext;
                retval != NULL && retval->m_Hook != pfnHook;
                retval = retval->m_pNext);

        return retval;
    }

    int updateHook(CReportHookList *pCReportHookList)
    {
        int retval = 0;
        if (pCReportHookList->m_pPrev != NULL)
        {
            disconnect(pCReportHookList);
            setHead(pCReportHookList);
            retval = pCReportHookList->m_Refcount++;
        }
        else
        {
            setHead(pCReportHookList);
            retval = pCReportHookList->m_Refcount = 1;
        }
        return retval;
    }

    int deleteHook(CReportHookList *pCReportHookList)
    {
        int retval = 0;
        if ((retval = --pCReportHookList->m_Refcount) == 0)
        {
            disconnect(pCReportHookList);
            CReportHookList::destroy(pCReportHookList);
        }
        return retval;
    }

    /* lock must be already held */
    int callReportHooks(int i, CharType *pCh, int *pInt)
    {
        int retval = 0;
        for (CReportHookList *ptmp = m_HeadNode.m_pNext;
                ptmp != NULL;
                ptmp = ptmp->m_pNext)
        {
            if (ptmp->m_Hook)
            {
                if (retval = (*(ptmp->m_Hook))(i, pCh, pInt))
                    break;
            }
        }
        return retval;
    }

    /* lock must be already held */
    void removeReportHooks()
    {
        CReportHookList* ptmp = m_HeadNode.m_pNext;
        while (ptmp != NULL)
        {
            CReportHookList* current = ptmp;
            ptmp = ptmp->m_pNext;
            CReportHookList::destroy(current);
        }
    }

    CCrtReportHook2DB()
    {
    }

        static __declspec(appdomain) bool callback_registered;
        static __declspec(appdomain) class CCrtReportHook2DB<HookType, CharType> __ReportHookDB;

private:
    void disconnect(CReportHookList* pCReportHookList)
    {
        // Remove an item from the list

        // Assume item is in list
        pCReportHookList->m_pPrev->m_pNext = pCReportHookList->m_pNext;

        // Check whether item is at end of list
        if (pCReportHookList->m_pNext)
        {
            pCReportHookList->m_pNext->m_pPrev = pCReportHookList->m_pPrev;
        }
    }

    void setHead(CReportHookList* pCReportHookList)
    {
        pCReportHookList->m_pNext = m_HeadNode.m_pNext;
        if (pCReportHookList->m_pNext != NULL)
        {
            pCReportHookList->m_pNext->m_pPrev = pCReportHookList;
        }
        m_HeadNode.m_pNext = pCReportHookList;
        pCReportHookList->m_pPrev = &m_HeadNode;
    }

    // Data members
private:
        CReportHookList m_HeadNode;
};

template<typename HookType, typename CharType>
__declspec(appdomain) bool CCrtReportHook2DB<HookType, CharType>::callback_registered=false;

template<typename HookType, typename CharType>
__declspec(appdomain) CCrtReportHook2DB<HookType, CharType> CCrtReportHook2DB<HookType, CharType>::__ReportHookDB;

template<typename CharType, typename DBType>
static int __cdecl _CrtReportHook2_managed_thunk(int i, CharType *pCh, int *pInt)
{
    /* lock was already taken by caller */
        return DBType::__ReportHookDB.callReportHooks(i, pCh, pInt);
}

template<typename NativeHookType, typename CharType, typename DBType>
static void __clrcall _CrtReportHook2_managed_cleanup(void)
{
    _CrtSetReportHook2_thunk(_CRT_RPTHOOK_REMOVE, static_cast<NativeHookType>(_CrtReportHook2_managed_thunk<CharType, DBType>) );

    _mlock(_DEBUG_LOCK);
    __try
    {
        DBType::__ReportHookDB.removeReportHooks();
    }
    __finally
    {
        _munlock(_DEBUG_LOCK);
    }
}

/***
*_CRT_REPORT_HOOK_M _CrtSetReportHook2() - configure client report hook in list
*
*Purpose:
*       Install or remove a client report hook from the report list.  Exists
*       separately from _CrtSetReportHook because the older function doesn't
*       work well in an environment where DLLs that are loaded and unloaded
*       dynamically out of LIFO order want to install report hooks.
*
*Entry:
*       int mode - _CRT_RPTHOOK_INSTALL or _CRT_RPTHOOK_REMOVE
*       _CRT_REPORT_HOOK_M pfnNewHook - report hook to install/remove/query
*
*Exit:
*       Returns -1 if an error was encountered, with EINVAL or ENOMEM set,
*       else returns the reference count of pfnNewHook after the call.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

template<typename NativeHookType, typename HookType, typename CharType, typename DBType>
int __cdecl _CrtSetReportHook2_impl
(
    int mode,
    HookType pfnNewHook,
    int (* pfnHook)(int, NativeHookType)
)
{
    DBType::CReportHookList *pHook=NULL;
    int ret=0;

    /* validation section */
    _VALIDATE_RETURN(mode == _CRT_RPTHOOK_INSTALL || mode == _CRT_RPTHOOK_REMOVE, EINVAL, -1);
    _VALIDATE_RETURN(pfnNewHook != NULL, EINVAL, -1);

    _mlock(_DEBUG_LOCK);
    __try
    {

        /* Search for new hook function to see if it's already installed */
        pHook = DBType::__ReportHookDB.findHook(pfnNewHook);

        if (mode == _CRT_RPTHOOK_REMOVE)
        {
            /* Remove request - free list node if refcount goes to zero */
            if (pHook != NULL)
            {
                DBType::__ReportHookDB.deleteHook(pHook);
            }
            else
            {
                _RPTF0(_CRT_ASSERT, "The hook function is not in the list!");
                ret = -1;
                errno = EINVAL;
            }
        }
        else
        {
            /* Insert request */
            if (pHook != NULL)
            {
                /* Hook function already registered, move to head of list */
                ret = DBType::__ReportHookDB.updateHook(pHook);
            }
            else
            {
                /* Hook function not already registered, insert new node */
                pHook = DBType::CReportHookList::create();
                if (pHook == NULL)
                {
                    /* malloc fails: we do not assert here */
                    ret = -1;
                    errno = ENOMEM;
                }
                else
                {
                    pHook->m_Hook = pfnNewHook;
                    ret = DBType::__ReportHookDB.updateHook(pHook);
                                        if (DBType::callback_registered == false)
                    {
                        if(_atexit_m_appdomain(_CrtReportHook2_managed_cleanup<NativeHookType, CharType, DBType>)!=0)
                        {
                            ret=-1;
                            errno=ENOMEM;
                        }
                        else
                        {
                            pfnHook(_CRT_RPTHOOK_INSTALL, static_cast<NativeHookType>(_CrtReportHook2_managed_thunk<CharType, DBType>));

                            DBType::callback_registered=true;
                        }
                    }
                }
            }
        }

    }
    __finally
    {
        _munlock(_DEBUG_LOCK);
    }

    return ret;
}

_MRTIMP int __cdecl _CrtSetReportHook2
(
    int mode,
    _CRT_REPORT_HOOK_M pfnNewHook
)
{
    return _CrtSetReportHook2_impl<_CRT_REPORT_HOOK, _CRT_REPORT_HOOK_M, char, CCrtReportHook2DB<_CRT_REPORT_HOOK_M, char> >(mode, pfnNewHook, _CrtSetReportHook2_thunk );
}

_MRTIMP int __cdecl _CrtSetReportHookW2
(
    int mode,
    _CRT_REPORT_HOOKW_M pfnNewHook
)
{
    return _CrtSetReportHook2_impl<_CRT_REPORT_HOOKW, _CRT_REPORT_HOOKW_M, wchar_t, CCrtReportHook2DB<_CRT_REPORT_HOOKW_M, wchar_t> >(mode, pfnNewHook, _CrtSetReportHook2_thunk);
}
#endif  /* _DEBUG */
