// Sink.h: interface for the CSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINK_H__3618F855_A175_41A3_AF69_3C0DEA0152A7__INCLUDED_)
#define AFX_SINK_H__3618F855_A175_41A3_AF69_3C0DEA0152A7__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#include "ConnectionCOM.h"
#import "ConnectionCOM.tlb" named_guids raw_interfaces_only

class CSink : public _IAddEvents
{
private:
    DWORD       m_dwRefCount;
public:
    CSink();
    virtual ~CSink();
    STDMETHODIMP ExecutionOver(int Result)
    {
        CString    strTemp;
        strTemp.Format(L"The result is %d", Result);
        AfxMessageBox(strTemp);
        return S_OK;;
    };


    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject)
    {
        if (iid == IID__IAddEvents) {
            m_dwRefCount++;
            *ppvObject = (void *)this;
            return S_OK;
        }

        if (iid == IID_IUnknown) {
            m_dwRefCount++;
            *ppvObject = (void *)this;
            return S_OK;
        }

        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
        m_dwRefCount++;
        return m_dwRefCount;
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG l;
        l  = m_dwRefCount--;

        if (0 == m_dwRefCount) {
            delete this;
        }

        return l;
    }

};

#endif // !defined(AFX_SINK_H__3618F855_A175_41A3_AF69_3C0DEA0152A7__INCLUDED_)
