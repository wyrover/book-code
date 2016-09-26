// Text.cpp : implementation file
//

#include "stdafx.h"
#include "PersistText.h"
#include "Text.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CText

IMPLEMENT_DYNCREATE(CText, CCmdTarget)

CText::CText()
{
    EnableAutomation();
    // To keep the application running as long as an OLE automation
    //  object is active, the constructor calls AfxOleLockApp.
    AfxOleLockApp();
    m_pchText = NULL;
}

CText::~CText()
{
    // To terminate the application when all objects created with
    //  with OLE automation, the destructor calls AfxOleUnlockApp.
    AfxOleUnlockApp();

    if (m_pchText != NULL)
        delete [] m_pchText;
}


void CText::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.
    CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CText, CCmdTarget)
    //{{AFX_MSG_MAP(CText)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CText, CCmdTarget)
    //{{AFX_DISPATCH_MAP(CText)
    DISP_PROPERTY_EX(CText, "Text", GetText, SetText, VT_VARIANT)
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IText to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {5310B303-76A5-11D4-8BDA-0EB916000000}
static const IID IID_IText =
{ 0x5310b303, 0x76a5, 0x11d4, { 0x8b, 0xda, 0xe, 0xb9, 0x16, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(CText, CCmdTarget)
INTERFACE_PART(CText, IID_IPersistStream, PersistStream)
INTERFACE_PART(CText, IID_IText, Dispatch)
END_INTERFACE_MAP()

// {5310B304-76A5-11D4-8BDA-0EB916000000}
IMPLEMENT_OLECREATE(CText, "PersistText.Text", 0x5310b304, 0x76a5, 0x11d4, 0x8b, 0xda, 0xe, 0xb9, 0x16, 0x0, 0x0, 0x0)

/////////////////////////////////////////////////////////////////////////////
// CText message handlers

VARIANT CText::GetText()
{
    return COleVariant(m_pchText).Detach();
}

void CText::SetText(const VARIANT FAR& newValue)
{
    CString strTemp;
    ASSERT(newValue.vt == VT_BSTR);

    if (m_pchText != NULL) {
        delete [] m_pchText;
    }

    strTemp = newValue.bstrVal; // converts to narrow chars
    m_pchText = new char[strTemp.GetLength() + 1];
    strcpy(m_pchText, strTemp);
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) CText::XPersistStream::AddRef()
{
    METHOD_PROLOGUE(CText, PersistStream)
    return (ULONG) pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CText::XPersistStream::Release()
{
    METHOD_PROLOGUE(CText, PersistStream)
    return (ULONG) pThis->ExternalRelease();
}

STDMETHODIMP CText::XPersistStream::QueryInterface(REFIID iid,
        void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(CText, PersistStream)
    // ExternalQueryInterface looks up iid in the macro-generated tables
    return (HRESULT) pThis->ExternalQueryInterface(&iid, ppvObj);
}
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CText::XPersistStream::GetClassID(LPCLSID lpClassID)
{
    TRACE("Entering CText::XPersistStream::GetClassID\n");
    METHOD_PROLOGUE(CText, PersistStream)
    ASSERT_VALID(pThis);
    *lpClassID = CText::guid;
    return NOERROR;
}

STDMETHODIMP CText::XPersistStream::IsDirty()
{
    TRACE("Entering CText::XPersistStream::IsDirty\n");
    METHOD_PROLOGUE(CText, PersistStream)
    ASSERT_VALID(pThis);
    return NOERROR;
}

STDMETHODIMP CText::XPersistStream::Load(LPSTREAM pStm)
{
    ULONG nLength;
    STATSTG statstg;
    METHOD_PROLOGUE(CText, PersistStream)
    ASSERT_VALID(pThis);

    if (pThis->m_pchText != NULL) {
        delete [] pThis->m_pchText;
    }

    // don't need to free statstg.pwcsName because of NONAME flag
    VERIFY(pStm->Stat(&statstg, STATFLAG_NONAME) == NOERROR);
    int nSize = statstg.cbSize.LowPart; // assume < 4 GB

    if (nSize > 0) {
        pThis->m_pchText = new char[nSize];
        pStm->Read(pThis->m_pchText, nSize, &nLength);
    }

    return NOERROR;
}

STDMETHODIMP CText::XPersistStream::Save(LPSTREAM pStm, BOOL fClearDirty)
{
    METHOD_PROLOGUE(CText, PersistStream)
    ASSERT_VALID(pThis);
    int nSize = strlen(pThis->m_pchText) + 1;
    pStm->Write(pThis->m_pchText, nSize, NULL);
    return NOERROR;
}

STDMETHODIMP CText::XPersistStream::GetSizeMax(ULARGE_INTEGER FAR* pcbSize)
{
    TRACE("Entering CText::XPersistStream::GetSizeMax\n");
    METHOD_PROLOGUE(CText, PersistStream)
    ASSERT_VALID(pThis);
    pcbSize->LowPart = strlen(pThis->m_pchText) + 1;
    pcbSize->HighPart = 0; // assume < 4 GB
    return NOERROR;
}
