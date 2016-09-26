// ASPComp.cpp : Implementation of CASPComp
#include "stdafx.h"
#include "ASPComSamp.h"
#include "ASPComp.h"
/////////////////////////////////////////////////////////////////////////////
// CASPComp


STDMETHODIMP CASPComp::ProcessRequest()
{
    HRESULT hr = E_FAIL;
    CComPtr<IObjectContext> pObjContext;
    //get the object context
    hr = ::GetObjectContext(&pObjContext);
    CComPtr<IRequest>pRequest = NULL;
    CComPtr<IResponse>pResponse = NULL;

    if (SUCCEEDED(hr)) {
        //get context properties
        CComPtr<IGetContextProperties> pProps;
        hr = pObjContext->QueryInterface(IID_IGetContextProperties, (void**)&pProps);

        if (SUCCEEDED(hr)) {
            CComBSTR bstrASPObj = "Request";
            CComVariant vt;

            //get the Request object
            if (SUCCEEDED(pProps->GetProperty(bstrASPObj, &vt))) {
                if (V_VT(&vt) == VT_DISPATCH) {
                    IDispatch* pDispatch = V_DISPATCH(&vt);

                    //use the returned IDispatch to QI for
                    //an IRequest
                    if (pDispatch)
                        hr = pDispatch->QueryInterface(IID_IRequest, (void **)&pRequest);
                }
            }

            bstrASPObj = "Response";

            //get the Response object
            if (SUCCEEDED(pProps->GetProperty(bstrASPObj, &vt))) {
                if (V_VT(&vt) == VT_DISPATCH) {
                    IDispatch* pDispatch = V_DISPATCH(&vt);

                    if (!pDispatch)
                        return E_FAIL;
                    else
                        //QI for the IResponse interface
                        hr = pDispatch->QueryInterface(IID_IResponse, (void **)&pResponse);

                    if (!pResponse)
                        return E_FAIL;
                    else {
                        CComPtr<IRequestDictionary>pReqDict = NULL;
                        CComVariant vRequestInfo;
                        //get the ServerVariables dictionary
                        hr = pRequest->get_ServerVariables(&pReqDict);

                        if (pReqDict) {
                            //send some text out to the client
                            hr = pResponse->Write(CComVariant(L"<H2>Hello World!</H2><UL>"));
                            //get the browser type
                            hr = pReqDict->get_Item(CComVariant(L"HTTP_USER_AGENT"), &vRequestInfo);
                            //write out the browser type
                            hr = pResponse->Write(CComVariant(L"<LI>Browser: "));
                            hr = pResponse->Write(vRequestInfo);
                            hr = pResponse->Write(CComVariant(L"</LI>"));
                            //get the client's address
                            hr = pReqDict->get_Item(CComVariant(L"REMOTE_ADDR"), &vRequestInfo);
                            //write out the client's address
                            hr = pResponse->Write(CComVariant(L"<LI>Address: "));
                            hr = pResponse->Write(vRequestInfo);
                            hr = pResponse->Write(CComVariant(L"</LI>"));
                            //terminate the list
                            hr = pResponse->Write(CComVariant(L"</UL>"));
                        }
                    }
                }
            }
        }
    }

    return S_OK;
}
