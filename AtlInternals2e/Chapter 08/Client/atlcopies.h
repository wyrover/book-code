// atlcopies.h: ATL copy policy classes, including _Copy<> specializations.
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1998 Chris Sells
// All rights reserved.
// 8/03/98 - Initial release.
//
// NO WARRANTIES ARE EXTENDED. USE AT YOUR OWN RISK.
//
// To contact the author with suggestions or comments, use csells@sellsbrothers.com.
/////////////////////////////////////////////////////////////////////////////
// The file provides a set of _Copy specializations for various enumerated data types.
//
// template<> class _Copy<CHANNELENUMINFO>;
// template<> class _Copy<HLITEM>;
// template<> class _Copy<STATSTG>;
// template<> class _Copy<STATPROPSTG>;
// template<> class _Copy<FORMATETC>;
// template<> class _Copy<STATDATA>;
//
// Not all the _Copy specializations needed have been provided in this file,
// and therefore not all existing enumeration interfaces have corresponding typedefs.
//
// This file also contains the following extra copy policy class:
//
// class _CopyBSTR;
//
/////////////////////////////////////////////////////////////////////////////
// Usage: This file intended for use by client-side and server-side enumeration
// classes, e.g. CComEnum and enum_iterator.

#pragma once
#ifndef _ATLCOPIES_H_
#define _ATLCOPIES_H_

#ifndef ATL_NO_NAMESPACE
namespace ATL
{
#endif

/////////////////////////////////////////////////////////////////////////////
// _Copy specializations for standard enums defined in windows.h et al

template<>
class _Copy<FORMATETC>
{
public:
    static HRESULT copy(FORMATETC* p1, FORMATETC* p2)
    {
        HRESULT hr = E_OUTOFMEMORY;
        ZeroMemory(p1, sizeof(*p1));
        *p1 = *p2;

        if ((!p2->ptd) || (p1->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE)))) {
            *(p1->ptd) = *(p2->ptd);
            hr = S_OK;
        }

        return hr;
    }
    static void init(FORMATETC* p)
    {
        ZeroMemory(p, sizeof(*p));
    }
    static void destroy(FORMATETC* p)
    {
        CoTaskMemFree(p->ptd);
        ZeroMemory(p, sizeof(*p));
    }
};

template<>
class _Copy<STATDATA>
{
public:
    static HRESULT copy(STATDATA* p1, STATDATA* p2)
    {
        HRESULT hr = E_OUTOFMEMORY;
        ZeroMemory(p1, sizeof(*p1));
        hr = _Copy<FORMATETC>::copy(&(p1->formatetc), &(p2->formatetc));

        if (SUCCEEDED(hr)) {
            p1->advf = p2->advf;
            p1->dwConnection = p2->dwConnection;

            if (p1->pAdvSink = p2->pAdvSink) p1->pAdvSink->AddRef();
        }

        return hr;
    }
    static void init(STATDATA* p)
    {
        ZeroMemory(p, sizeof(*p));
    }
    static void destroy(STATDATA* p)
    {
        _Copy<FORMATETC>::destroy(&(p->formatetc));

        if (p->pAdvSink) p->pAdvSink->Release();

        ZeroMemory(p, sizeof(*p));
    }
};

template<>
class _Copy<STATPROPSTG>
{
public:
    static HRESULT copy(STATPROPSTG* p1, STATPROPSTG* p2)
    {
        HRESULT hr = E_OUTOFMEMORY;
        ZeroMemory(p1, sizeof(*p1));

        if (p1->lpwstrName = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->lpwstrName) + 1))) {
            *p1 = *p2;
            ocscpy(p1->lpwstrName, p2->lpwstrName);
            hr = S_OK;
        }

        return hr;
    }
    static void init(STATPROPSTG* p)
    {
        ZeroMemory(p, sizeof(*p));
    }
    static void destroy(STATPROPSTG* p)
    {
        CoTaskMemFree(p->lpwstrName);
        ZeroMemory(p, sizeof(*p));
    }
};

template<>
class _Copy<STATSTG>
{
public:
    static HRESULT copy(STATSTG* p1, STATSTG* p2)
    {
        HRESULT hr = E_OUTOFMEMORY;
        ZeroMemory(p1, sizeof(*p1));

        if (p1->pwcsName = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->pwcsName) + 1))) {
            *p1 = *p2;
            ocscpy(p1->pwcsName, p2->pwcsName);
            hr = S_OK;
        }

        return hr;
    }
    static void init(STATSTG* p)
    {
        ZeroMemory(p, sizeof(*p));
    }
    static void destroy(STATSTG* p)
    {
        CoTaskMemFree(p->pwcsName);
        ZeroMemory(p, sizeof(*p));
    }
};

class _CopyBSTR
{
public:
    static HRESULT copy(BSTR* p1, BSTR* p2)
    {
        HRESULT hr = S_OK;
        (*p1) = (LPOLESTR)SysAllocString(*p2);

        if (*p1 == NULL)
            hr = E_OUTOFMEMORY;

        return hr;
    }
    static void init(LPOLESTR* p)
    {
        *p = NULL;
    }
    static void destroy(LPOLESTR* p)
    {
        SysFreeString(*p);
    }
};

// TODO: _Copy<PACKAGEDETAIL> specialization
// TODO: _Copy<CLASSDETAIL> specialization

/////////////////////////////////////////////////////////////////////////////
// _Copy specializations for standard enums defined outside windows.h

#ifdef __urlhist_h__
    // TODO: _Copy<STATURL> specialization
#endif  // __urlhist_h__

#ifdef __strmif_h__
    // TODO: _Copy<AM_MEDIA_TYPE*> specialization
    // TODO: _Copy<REGFILTER*> specialization
#endif  // __strmif_h__

#ifdef __chanmgr_h__
template<>
class _Copy<CHANNELENUMINFO>
{
public:
    static HRESULT copy(CHANNELENUMINFO* p1, CHANNELENUMINFO* p2)
    {
        HRESULT hr = E_OUTOFMEMORY;
        ZeroMemory(p1, sizeof(*p1));
        p1->stSubscriptionState = p2->stSubscriptionState;

        if ((p1->pszTitle = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->pszTitle) + 1))) &&
            (p1->pszPath = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->pszPath) + 1))) &&
            (p1->pszURL = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->pszURL) + 1)))) {
            ocscpy(p1->pszTitle, p2->pszTitle);
            ocscpy(p1->pszPath, p2->pszPath);
            ocscpy(p1->pszURL, p2->pszURL);
            hr = S_OK;
        } else {
            CoTaskMemFree(p1->pszTitle);
            CoTaskMemFree(p1->pszPath);
            CoTaskMemFree(p1->pszURL);
        }

        return hr;
    }
    static void init(CHANNELENUMINFO* p)
    {
        ZeroMemory(p, sizeof(*p));
    }
    static void destroy(CHANNELENUMINFO* p)
    {
        CoTaskMemFree(p->pszTitle);
        CoTaskMemFree(p->pszPath);
        CoTaskMemFree(p->pszURL);
        ZeroMemory(p, sizeof(*p));
    }
};
#endif  // __chanmgr_h__

#ifdef __hlink_h__
template<>
class _Copy<HLITEM>
{
public:
    static HRESULT copy(HLITEM* p1, HLITEM* p2)
    {
        HRESULT hr = E_OUTOFMEMORY;
        ZeroMemory(p1, sizeof(*p1));

        if (p1->pwzFriendlyName = (LPOLESTR)CoTaskMemAlloc(sizeof(OLECHAR) * (ocslen(p2->pwzFriendlyName) + 1))) {
            p1->uHLID = p2->uHLID;
            ocscpy(p1->pwzFriendlyName, p2->pwzFriendlyName);
            hr = S_OK;
        }

        return hr;
    }
    static void init(HLITEM* p)
    {
        ZeroMemory(p, sizeof(*p));
    }
    static void destroy(HLITEM* p)
    {
        CoTaskMemFree(p->pwzFriendlyName);
        ZeroMemory(p, sizeof(*p));
    }
};
#endif  // __hlink_h__

#ifdef __mstask_h__
    // TODO: _Copy<LPWSTR*> specialization
#endif  // __mstask_h__

#ifndef ATL_NO_NAMESPACE
}
#endif

#endif  // _ATLCOPIES_H_
