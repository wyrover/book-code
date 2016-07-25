// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__79E5F761_60DB_455C_A13B_DB53D271A0B8__INCLUDED_)
#define AFX_GRAPH_H__79E5F761_60DB_455C_A13B_DB53D271A0B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraph:public CObject
{
	DECLARE_SERIAL(CGraph)
public:
	void Draw(CDC* pDC);
	CPoint m_ptOrigin;
	CPoint m_ptEnd;
	UINT m_nDrawType;
	CGraph();
	CGraph(UINT m_nDrawType,CPoint m_ptOrigin,CPoint m_ptEnd);
	void Serialize(CArchive& ar);
	virtual ~CGraph();

};

#endif // !defined(AFX_GRAPH_H__79E5F761_60DB_455C_A13B_DB53D271A0B8__INCLUDED_)
