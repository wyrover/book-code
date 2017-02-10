#pragma once

//
// Forward declarations for the ref count database 
// operations ("public" interface)
//
HRESULT CRefCountDatabase_Init();
VOID CRefCountDatabase_AddRef(const VOID*);
VOID CRefCountDatabase_Release(const VOID*);
VOID CRefCountDatabase_DetectLeaks();

//
// Base class implementation for IUnknown
//
template
<
    bool bUseRefCountDB
>
class CIUnknownImplT : 
    public IUnknown
{
public:
    //
    // Declare a virtual destructor to ensure that derived classes
    // will be destroyed properly
    //
    virtual 
    ~CIUnknownImplT(
        VOID
        )
    {
    }

    CIUnknownImplT(
        VOID
        )
    {
        bUseRefCountDB ? CRefCountDatabase_AddRef(this) : 0;
        m_lRefCount = 1;
    }

    STDMETHOD_(ULONG, AddRef)(
        VOID
        )
    {
        bUseRefCountDB ? CRefCountDatabase_AddRef(this) : 0;
        return InterlockedIncrement(&m_lRefCount);
    }

    STDMETHOD_(ULONG, Release)(
        VOID
        )
    {
        LONG lRef;

        bUseRefCountDB ? CRefCountDatabase_Release(this) : 0;
        lRef = InterlockedDecrement(&m_lRefCount);
        if (lRef == 0)
        {
            delete this;
        }

        return lRef;
    }

    //
    // Can be overridden in derived classes, as needed
    //
    STDMETHOD(QueryInterface)(
        __in REFIID iid,
        __deref_out PVOID* ppvObject
        )
    {
        return E_NOTIMPL;
        UNREFERENCED_PARAMETER(iid);
        UNREFERENCED_PARAMETER(ppvObject);
    }

private:
    volatile LONG m_lRefCount;
};

typedef CIUnknownImplT<true> CIUnknownImpl;


class CRefCountDatabase :
    CZeroInit<CRefCountDatabase>
{
private:
    //
    // Global interface to the singleton
    //
    friend HRESULT CRefCountDatabase_Init();
    friend VOID CRefCountDatabase_AddRef(const VOID*);
    friend VOID CRefCountDatabase_Release(const VOID*);
    friend VOID CRefCountDatabase_DetectLeaks();

    struct CObjRef : 
        CZeroInit<CObjRef>, 
        public CIUnknownImplT<false>
    {
        VOID
        Init(
            __in const VOID* pObj
            )
        {
            USHORT nCount, n;
            PVOID pStackFrames[MAX_CAPTURED_STACK_DEPTH];

            ASSERT(pObj);

            m_pObj = pObj;
            m_nStackFrames = 0;

            nCount = CaptureStackBackTrace(
                1,
                ARRAYSIZE(pStackFrames),
                pStackFrames,
                NULL
                );
            ASSERT(nCount <= ARRAYSIZE(pStackFrames));

            m_spStackFrames.Attach((PVOID*)new BYTE[nCount * sizeof(PVOID)]);
            if (m_spStackFrames)
            {
                for (n = 0; n < nCount; n++)
                {
                    m_spStackFrames[n] = pStackFrames[n];
                }
                m_nStackFrames = nCount;
            }
        }

        VOID
        Dump(
            VOID
            ) const
        {
            int n;

            OutputDebugString(L"  OBJECT LEAK: \n");
            OutputDebugString(L"   Address: 0x%p\n", m_pObj);
            OutputDebugString(L"   StackTrace: dps 0x%p L0x%x\n",
                m_spStackFrames, m_nStackFrames);

            for (n = 0; n < m_nStackFrames; n++)
            {
                OutputDebugString(L"    %d: 0x%p\n", n, m_spStackFrames[n]);
            }
        }

private:
        const VOID* m_pObj;
        CAutoPtr<PVOID> m_spStackFrames;
        int m_nStackFrames;
    };

    //
    // Prevent creation of second interface of the database
    //
    CRefCountDatabase() {}

    ~CRefCountDatabase(
        VOID
        )
    {
        Done();
    }

    VOID
    AddRef(
        __in const VOID* pObj
        )
    {
        CComPtr<CObjRef> spObjRef;
        CObjRef* pObjRef;
        const CObjRefArray::CPair* pPair;

        ASSERT(pObj);

        if (!m_bInited)
        {
            return;
        }

        EnterCriticalSection(&m_cs);

        pPair = m_objs.Lookup((ULONG_PTR) pObj);
        if (pPair)
        {
            pObjRef = static_cast<CObjRef*>(pPair->m_value);
            pObjRef->AddRef();
        }
        else
        {
            spObjRef.Attach(new CObjRef());
            if (spObjRef)
            {
                spObjRef->Init(pObj);
                pObjRef = static_cast<CObjRef*>(spObjRef);
                pObjRef->AddRef();
                m_objs.SetAt((ULONG_PTR) pObj, spObjRef);
            }
        }

        LeaveCriticalSection(&m_cs);
    }

    VOID
    Release(
        __in const VOID* pObj
        )
    {
        const CObjRefArray::CPair* pPair;
        CObjRef* pObjRef;
        LONG lRef;

        ASSERT(pObj);

        if (!m_bInited)
        {
            return;
        }

        EnterCriticalSection(&m_cs);

        pPair = m_objs.Lookup((ULONG_PTR) pObj);
        if (pPair)
        {
            pObjRef = static_cast<CObjRef*>(pPair->m_value);
            lRef = pObjRef->Release();
            if (lRef == 1)
            {
                m_objs.RemoveKey((ULONG_PTR) pObj);
            }
        }

        LeaveCriticalSection(&m_cs);
    }

    VOID
    DetectLeaks(
        VOID
        )
    {
        if (!m_bInited)
        {
            return;
        }

        EnterCriticalSection(&m_cs);

        const CObjRefArray::CPair* pPair;
        POSITION pos = m_objs.GetStartPosition();
        while (pos)
        {
            pPair = m_objs.GetAt(pos);
            if (pPair)
            {
                pPair->m_value->Dump();
                DebugBreak();
            }
            m_objs.GetNext(pos);
        }

        LeaveCriticalSection(&m_cs);
    }

    HRESULT
    Init(
        VOID
        )
    {
        ChkProlog();

        ChkWin32(InitializeCriticalSectionAndSpinCount(&m_cs, 0));
        m_bInited = TRUE;

        ChkNoCleanup();
    }

    VOID
    Done(
        VOID
        )
    {
        if (m_bInited)
        {
            DeleteCriticalSection(&m_cs);
            m_bInited = FALSE;
        }
    }

    COREDEFS_INLINE
    static
    CRefCountDatabase&
    GetGlobalDB(
        VOID
        )
    {
        static CRefCountDatabase g_refCountDB;
        return g_refCountDB;
    }

    //
    // A slightly-smarter version of OutputDebugString
    //
    COREDEFS_INLINE
    static
    VOID
    OutputDebugString(
        __in LPCWSTR pwszFormat,
        ...
        )
    {
        WCHAR wszBuf[MAX_PATH];
        va_list list;

        ChkProlog();

        ASSERT(pwszFormat);

        va_start(list, pwszFormat);

        ChkHr(StringCchVPrintfW(
            wszBuf,
            ARRAYSIZE(wszBuf),
            pwszFormat,
            list
            ));

        ::OutputDebugStringW(wszBuf);

        va_end(list);

        ChkCleanup();
    }

    typedef CAtlMap<ULONG_PTR, CComPtr<CObjRef> > CObjRefArray;

    CObjRefArray m_objs;
    CRITICAL_SECTION m_cs;
    BOOL m_bInited;

    static const MAX_CAPTURED_STACK_DEPTH = 64;
};

//
// Refcount database interface
//
COREDEFS_INLINE HRESULT CRefCountDatabase_Init()
{
    return CRefCountDatabase::GetGlobalDB().Init();
}

COREDEFS_INLINE VOID CRefCountDatabase_AddRef(const VOID* pObj)
{
    CRefCountDatabase::GetGlobalDB().AddRef(pObj);
}

COREDEFS_INLINE VOID CRefCountDatabase_Release(const VOID* pObj)
{
    CRefCountDatabase::GetGlobalDB().Release(pObj);
}

COREDEFS_INLINE VOID CRefCountDatabase_DetectLeaks()
{
    CRefCountDatabase::GetGlobalDB().DetectLeaks();
}
