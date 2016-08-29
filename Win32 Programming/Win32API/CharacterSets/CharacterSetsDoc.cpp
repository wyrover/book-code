// CharacterSetsDoc.cpp : implementation of the CCharacterSetsDoc class
//

#include "stdafx.h"
#include "CharacterSets.h"

#include "CharacterSetsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsDoc

IMPLEMENT_DYNCREATE(CCharacterSetsDoc, CDocument)

BEGIN_MESSAGE_MAP(CCharacterSetsDoc, CDocument)
        //{{AFX_MSG_MAP(CCharacterSetsDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsDoc construction/destruction

CCharacterSetsDoc::CCharacterSetsDoc()
{
        // TODO: add one-time construction code here

}

CCharacterSetsDoc::~CCharacterSetsDoc()
{
}

BOOL CCharacterSetsDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsDoc serialization

void CCharacterSetsDoc::Serialize(CArchive& ar)
{
        if (ar.IsStoring())
        {
                // TODO: add storing code here
        }
        else
        {
                // TODO: add loading code here
        }
}

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsDoc diagnostics

#ifdef _DEBUG
void CCharacterSetsDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CCharacterSetsDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsDoc commands
