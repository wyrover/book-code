// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Graphic.h"
#include "Graph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CGraph, CObject, 1 )

CGraph::CGraph()
{

}

CGraph::CGraph(UINT m_nDrawType,CPoint m_ptOrigin,CPoint m_ptEnd)
{
	this->m_nDrawType=m_nDrawType;
	this->m_ptOrigin=m_ptOrigin;
	this->m_ptEnd=m_ptEnd;
}

CGraph::~CGraph()
{

}

void CGraph::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_nDrawType<<m_ptOrigin<<m_ptEnd;
	}
	else
	{
		ar>>m_nDrawType>>m_ptOrigin>>m_ptEnd;
	}
}

void CGraph::Draw(CDC *pDC)
{
	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush=pDC->SelectObject(pBrush);
	switch(m_nDrawType)
	{
	case 1:
		pDC->SetPixel(m_ptEnd,RGB(0,0,0));
		break;
	case 2:
		pDC->MoveTo(m_ptOrigin);
		pDC->LineTo(m_ptEnd);
		break;
	case 3:
		pDC->Rectangle(CRect(m_ptOrigin,m_ptEnd));
		break;
	case 4:
		pDC->Ellipse(CRect(m_ptOrigin,m_ptEnd));
		break;
	}
	pDC->SelectObject(pOldBrush);
}
