// Sink.cpp: implementation of the CSink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConnectionClient.h"
#include "Sink.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSink::CSink()
{
    m_dwRefCount = 0;
}

CSink::~CSink()
{
}
