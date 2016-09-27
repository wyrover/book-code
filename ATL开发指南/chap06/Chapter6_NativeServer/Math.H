//
// Math.h
//

// access to the global variables
extern DWORD g_dwObjs;
extern DWORD g_dwLocks;

DEFINE_GUID(CLSID_Math,
            0xA988BD40, 0x9F1A, 0x11CE, 0x8B, 0x9F, 0x10, 0x00, 0x5A, 0xFB, 0x7D, 0x30);

const DISPID_ADD = 1;
const DISPID_SUBTRACT = 2;
const DISPID_MULTIPLY = 3;
const DISPID_DIVIDE = 4;

class Math : public IDispatch
{
protected:
    // Reference count
    DWORD          m_dwRef;

public:
    Math();
    ~Math();

public:
    // IUnknown
    STDMETHOD(QueryInterface)(REFIID, void**);
    STDMETHOD_(ULONG, AddRef());
    STDMETHOD_(ULONG, Release());

    // IDispatch
    STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
    STDMETHOD(GetTypeInfo)(UINT itinfo,
                           LCID lcid,
                           ITypeInfo** pptinfo);
    STDMETHOD(GetIDsOfNames)(REFIID riid,
                             OLECHAR** rgszNames,
                             UINT cNames,
                             LCID lcid,
                             DISPID* rgdispid);
    STDMETHOD(Invoke)(DISPID dispid,
                      REFIID riid,
                      LCID lcid,
                      WORD wFlags,
                      DISPPARAMS FAR* pDispParams,
                      VARIANT FAR* pvarResult,
                      EXCEPINFO FAR* pExcepInfo,
                      unsigned int FAR* puArgErr);

};

class MathClassFactory : public IClassFactory
{
protected:
    DWORD           m_dwRef;

public:
    MathClassFactory();
    ~MathClassFactory();

    // IUnknown
    STDMETHOD(QueryInterface(REFIID, void**));
    STDMETHOD_(ULONG, AddRef());
    STDMETHOD_(ULONG, Release());

    // IClassFactory
    STDMETHOD(CreateInstance(LPUNKNOWN, REFIID, void**));
    STDMETHOD(LockServer(BOOL));
};

