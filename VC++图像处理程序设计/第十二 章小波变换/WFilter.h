// WFilter.h: interface for the CWFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WFILTER_H__5AAF2511_8AC6_4CCE_A5F5_4E5CEDD36DCC__INCLUDED_)
#define AFX_WFILTER_H__5AAF2511_8AC6_4CCE_A5F5_4E5CEDD36DCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Cdib.h"
class CWFilter :public CDib 
{
public:
	short **m_FilterData;
	CWFilter();
	virtual ~CWFilter();

};

#endif // !defined(AFX_WFILTER_H__5AAF2511_8AC6_4CCE_A5F5_4E5CEDD36DCC__INCLUDED_)
