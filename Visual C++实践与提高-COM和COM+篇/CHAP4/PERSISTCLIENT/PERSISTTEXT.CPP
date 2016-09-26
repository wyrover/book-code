// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "persisttext.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IText properties

VARIANT IText::GetText()
{
    VARIANT result;
    GetProperty(0x1, VT_VARIANT, (void*)&result);
    return result;
}

void IText::SetText(const VARIANT& propVal)
{
    SetProperty(0x1, VT_VARIANT, &propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IText operations
