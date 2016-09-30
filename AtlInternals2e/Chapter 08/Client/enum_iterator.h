// enum_iterator.h: IEnumXxx container to produce STL-compatible iterators
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1997-1998, Chris Sells
// All rights reserved.
//
// Revisions:
// 8/03/98  - Revised to use atlcopies.h to leverage shared _Copy<> specializations.
// 7/01/98  - Put back ocslen usage and provided defines for ocslen and ocscpy.
//            #ifdef'd out ocidl related typedefs if ocidl.h not included
//            (thanks again Justin Rudd).
// 6/30/98  - Fixed Component Category typedefs (thanks to Justin Rudd)
//            Removed need for ocslen for clients not using ATL.
// 11/03/97 - Initial release
//
// NO WARRANTIES ARE EXTENDED. USE AT YOUR OWN RISK.
//
// To contact the author with suggestions or comments, use csells@sellsbrothers.com.
/////////////////////////////////////////////////////////////////////////////
// The following is a list of enumerations interfaces and the cooresponding
// client-side STL-like iterator classes:
//
// IEnumCATEGORYINFO      catinfo_iterator
// IEnumConnectionPoints  connectionpoint_iterator
// IEnumConnections       connection_iterator
// IEnumFORMATETC         formatetc_iterator
// IEnumGUID              guid_iterator
// IEnumHLITEM            hlitem_iterator
// IEnumMoniker           mk_iterator
// IEnumOleDocumentViews  oledocview_iterator
// IEnumOleUndoUnits      oleundo_iterator
// IEnumOLEVERB           oleverb_iterator
// IEnumSTATDATA          statdata_iterator
// IEnumSTATPROPSTG       statpropstg_iterator
// IEnumSTATSTG           statstg_iterator
// IEnumString            olestr_iterator
// IEnumUnknown           unk_iterator
// IEnumVARIANT           variant_iterator
/////////////////////////////////////////////////////////////////////////////
// Usage:
/*

void EnumVariants(IEnumVARIANT* pevar)
{
    typedef enum_iterator<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT> EVI;
    for( EVI i = EVI(pevar, 64); i != EVI(); ++i )
    {
        VARIANT&    v = *i;
        // Do something with v
    }
}

*/
// or using the typedefs for the standard enumerators
/*

void EnumVariants(IEnumVARIANT* pevar)
{
    for( variant_iterator i = variant_iterator(pevar, 64); i != variant_iterator(); ++i )
    {
        VARIANT&    v = *i;
        // Do something with v
    }
}

*/
// or using STL algorithms (this is my personal favorite)
/*

struct DoSomethingWithVariant
{
    void operator()(const VARIANT& v)
    {
        // Do something with v
    }
};

void EnumVariants(IEnumVARIANT* pevar)
{
    for_each(enum_variant(pevar, 128), enum_variant(), DoSomethingWithVariant());
}

*/
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// The following block was copied out of atlconv.h to avoid
// requiring a client to use ATL.
#ifndef __ATLCONV_H__
#if defined(_UNICODE)
inline size_t ocslen(LPCOLESTR x)
{
    return lstrlenW(x);
}
inline OLECHAR* ocscpy(LPOLESTR dest, LPCOLESTR src)
{
    return lstrcpyW(dest, src);
}
#else
inline size_t ocslen(LPCOLESTR x)
{
    return lstrlenW(x);
}
//lstrcpyW doesn't work on Win95, so we do this
inline OLECHAR* ocscpy(LPOLESTR dest, LPCOLESTR src)
{
    return (LPOLESTR) memcpy(dest, src, (lstrlenW(src) + 1) * sizeof(WCHAR));
}
#endif
#endif // __ATLCONV_H__

/////////////////////////////////////////////////////////////////////////////
// The following block was copied out of atlcom.h to avoid
// requiring a client not using ATL to provide a _Module.
#ifndef __ATLCOM_H__

// This is a part of the Active Template Library.
// Copyright (C) 1996-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#ifndef ATL_NO_NAMESPACE
namespace ATL
{
#endif

// These _CopyXXX classes are used with enumerators in order to control
// how enumerated items are initialized, copied, and deleted

// Default is shallow copy with no special init or cleanup
template <class T>
class _Copy
{
public:
    static void copy(T* p1, T* p2)
    {
        memcpy(p1, p2, sizeof(T));
    }
    static void init(T*) {}
    static void destroy(T*) {}
};

#if _MSC_VER>1020
    template<>
#endif
class _Copy<VARIANT>
{
public:
    static void copy(VARIANT* p1, VARIANT* p2)
    {
        VariantCopy(p1, p2);
    }
    static void init(VARIANT* p)
    {
        VariantInit(p);
    }
    static void destroy(VARIANT* p)
    {
        VariantClear(p);
    }
};

#if _MSC_VER>1020
    template<>
#endif
class _Copy<LPOLESTR>
{
public:
    static void copy(LPOLESTR* p1, LPOLESTR* p2)
    {
        (*p1) = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(*p2) + 1));
        ocscpy(*p1, *p2);
    }
    static void init(LPOLESTR* p)
    {
        *p = NULL;
    }
    static void destroy(LPOLESTR* p)
    {
        CoTaskMemFree(*p);
    }
};

#if _MSC_VER>1020
    template<>
#endif
class _Copy<OLEVERB>
{
public:
    static void copy(OLEVERB* p1, OLEVERB* p2)
    {
        *p1 = *p2;

        if (p1->lpszVerbName == NULL)
            return;

        p1->lpszVerbName = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->lpszVerbName) + 1));
        ocscpy(p1->lpszVerbName, p2->lpszVerbName);
    }
    static void init(OLEVERB* p)
    {
        p->lpszVerbName = NULL;
    }
    static void destroy(OLEVERB* p)
    {
        if (p->lpszVerbName) CoTaskMemFree(p->lpszVerbName);
    }
};

#ifdef __ocidl_h__
#if _MSC_VER>1020
    template<>
#endif
class _Copy<CONNECTDATA>
{
public:
    static void copy(CONNECTDATA* p1, CONNECTDATA* p2)
    {
        *p1 = *p2;

        if (p1->pUnk)
            p1->pUnk->AddRef();
    }
    static void init(CONNECTDATA*) {}
    static void destroy(CONNECTDATA* p)
    {
        if (p->pUnk) p->pUnk->Release();
    }
};
#endif // __ocidl_h__

template <class T>
class _CopyInterface
{
public:
    static void copy(T** p1, T** p2)
    {
        *p1 = *p2;

        if (*p1)(*p1)->AddRef();
    }
    static void init(T**) {}
    static void destroy(T** p)
    {
        if (*p)(*p)->Release();
    }
};

#ifndef ATL_NO_NAMESPACE
}
#endif
#endif  // __ATLCOM_H__

#include "atlcopies.h"  // More _Copy<> specializations

/////////////////////////////////////////////////////////////////////////////
// enum_iterator

#ifndef ENUM_CHUNK
    #define ENUM_CHUNK 64
#endif

template <typename EnumItf, const IID* pIIDEnumItf,
          typename EnumType, typename CopyClass = _Copy<EnumType> >
class enum_iterator
{
public:
    enum_iterator(IUnknown* punkEnum = 0, ULONG nChunk = ENUM_CHUNK) : m_pRep(0)
    {
        if (punkEnum) {
            m_pRep = new EnumRep(punkEnum, nChunk);

            if (m_pRep) {
                m_pRep->AddRef();

                if (!m_pRep->First()) {
                    Destroy();
                }
            }
        }
    }

    enum_iterator(const enum_iterator& i) : m_pRep(i.m_pRep)
    {
        if (m_pRep) m_pRep->AddRef();
    }

    ~enum_iterator()
    {
        Destroy();
    }

    enum_iterator& operator=(const enum_iterator& rhs)
    {
        if (*this != rhs) {
            Destroy();

            if (m_pRep = rhs.m_pRep) m_pRep->AddRef();
        }

        return *this;
    }

    bool operator!=(const enum_iterator& rhs)
    {
        return !(*this == rhs);
    }

    bool operator==(const enum_iterator& rhs)
    {
        return m_pRep == rhs.m_pRep;
    }

    // ++i
    enum_iterator& operator++()
    {
        if (m_pRep && !m_pRep->Next()) {
            Destroy();  // We're at the end of the collection
        }

        return *this;
    }

    // i++
    enum_iterator operator++(int)
    {
        enum_iterator   tmp = *this;
        ++(*this);  // Forward to ++i
        return tmp;
    }

    EnumType& operator*()
    {
        assert(m_pRep && "Past the end of the enumeration");
        return m_pRep->Get();
    }

private:
    void Destroy()
    {
        if (m_pRep) m_pRep->Release();

        m_pRep = 0;
    }

    class EnumRep
    {
    public:
        EnumRep(IUnknown* punkEnum, ULONG nChunk)
            : m_pEnum(0), m_rgEnum(0), m_nChunk(nChunk), m_cRef(0), m_n(0), m_cElems(0)
        {
            if (punkEnum) {
                punkEnum->QueryInterface(*pIIDEnumItf, (void**)&m_pEnum);

                if (m_pEnum) {
                    m_rgEnum = new EnumType[m_nChunk];

                    if (m_rgEnum) {
                        InitChunk();
                    } else {
                        m_pEnum->Release();
                        m_pEnum = 0;
                    }
                }
            }
        }

        ~EnumRep()
        {
            if (m_rgEnum) {
                DestroyChunk();
                delete[] m_rgEnum;
            }

            if (m_pEnum) m_pEnum->Release();
        }

        void AddRef()
        {
            ++m_cRef;
        }

        void Release()
        {
            if (!--m_cRef) delete this;
        }

        bool First()
        {
            return NextChunk();
        }

        bool Next()
        {
            if (++m_n >= m_cElems) { // We're at the end of this chunk
                if (m_cElems < m_nChunk ||  // Last m_pEnum->Next() returned S_FALSE
                    !NextChunk()) {         // Last m_pEnum->Next() return S_OK
                    return false;   // We're at the end of the collection
                } else {
                    m_n = 0;    // We're at the beginning of a new chunk
                }
            }

            return true;
        }

        EnumType& Get()
        {
            assert(m_pEnum && "No elements cached");
            assert(m_n < m_cElems && "No elements left");
            return m_rgEnum[m_n];
        }

    private:
        EnumItf*    m_pEnum;
        EnumType*   m_rgEnum;
        ULONG       m_cElems;
        const ULONG m_nChunk;
        ULONG       m_cRef;
        ULONG       m_n;

        void InitChunk()
        {
            for (EnumType* p = &m_rgEnum[0]; p != &m_rgEnum[m_nChunk]; ++p) {
                CopyClass::init(p);
            }
        }

        // Use the CopyClass::destroy but leave data available for next chunk
        void DestroyChunk()
        {
            for (EnumType* p = &m_rgEnum[0]; p < &m_rgEnum[m_cElems]; ++p) {
                CopyClass::destroy(p);
            }

            m_cElems = 0;
        }

        bool NextChunk()
        {
            DestroyChunk();
            HRESULT hr = (m_pEnum ? m_pEnum->Next(m_nChunk, m_rgEnum, &m_cElems) : E_UNEXPECTED);

            if (hr == S_OK) m_cElems = m_nChunk;
            else if (FAILED(hr)) m_cElems = 0;

            // Coerce m_cElems == 0 to a failure
            // so iterator knows it's stepped off the end
            if (SUCCEEDED(hr) && m_cElems == 0) hr = E_FAIL;

            return (SUCCEEDED(hr) ? true : false);
        }
    };

    EnumRep*    m_pRep;
};

// Typedefs for standard enumerators
typedef enum_iterator<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT>  variant_iterator;
typedef enum_iterator<IEnumMoniker, &IID_IEnumMoniker, IMoniker*, _CopyInterface<IMoniker> > mk_iterator;
typedef enum_iterator<IEnumString, &IID_IEnumString, LPOLESTR> olestr_iterator;
typedef enum_iterator<IEnumUnknown, &IID_IEnumUnknown, IUnknown*, _CopyInterface<IUnknown> > unk_iterator;
typedef enum_iterator<IEnumSTATSTG, &IID_IEnumSTATSTG, STATSTG> statstg_iterator;
typedef enum_iterator<IEnumFORMATETC, &IID_IEnumFORMATETC, FORMATETC> formatetc_iterator;
typedef enum_iterator<IEnumSTATDATA, &IID_IEnumSTATDATA, STATDATA> statdata_iterator;
typedef enum_iterator<IEnumSTATPROPSTG, &IID_IEnumSTATPROPSTG, STATPROPSTG> statpropstg_iterator;
typedef enum_iterator<IEnumOLEVERB, &IID_IEnumOLEVERB, OLEVERB> oleverb_iterator;

// TODO: Requires _Copy<> specialization
//typedef enum_iterator<IEnumSTATPROPSETSTG, &IID_IEnumSTATPROPSETSTG, STATPROPSETSTG> statpropsetstg_iterator;

#ifdef __hlink_h__
    typedef enum_iterator<IEnumHLITEM, &IID_IEnumHLITEM, HLITEM> hlitem_iterator;
#endif // __hlink_h__

#ifdef __ocidl_h__
    typedef enum_iterator<IEnumConnections, &IID_IEnumConnections, CONNECTDATA> connection_iterator;
    typedef enum_iterator<IEnumConnectionPoints, &IID_IEnumConnectionPoints, IConnectionPoint*, _CopyInterface<IConnectionPoint> > connectionpoint_iterator;
    typedef enum_iterator<IEnumOleUndoUnits, &IID_IEnumOleUndoUnits, IOleUndoUnit*, _CopyInterface<IOleUndoUnit> > oleundo_iterator;
#endif

#ifdef __comcat_h__
    typedef enum_iterator<IEnumGUID, &IID_IEnumGUID, GUID> guid_iterator;
    typedef guid_iterator clsid_iterator;
    typedef guid_iterator catid_iterator;
    typedef enum_iterator<IEnumCATEGORYINFO, &IID_IEnumCATEGORYINFO, CATEGORYINFO> catinfo_iterator;
#endif  // __comcat_h__

#ifdef __docobj_h__
    typedef enum_iterator<IEnumOleDocumentViews, &IID_IEnumOleDocumentViews, IOleDocumentView*, _CopyInterface<IOleDocumentView> > oledocview_iterator;
#endif

