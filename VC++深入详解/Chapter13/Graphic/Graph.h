// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__88A49E60_5896_491F_8200_965FF4362629__INCLUDED_)
#define AFX_GRAPH_H__88A49E60_5896_491F_8200_965FF4362629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraph : public CObject  
{
	DECLARE_SERIAL(CGraph)
public:
	void Draw(CDC* pDC);
	CPoint m_ptOrigin;
	CPoint m_ptEnd;
	UINT m_nDrawType;

	
	CGraph();
	CGraph(UINT m_nDrawType,CPoint m_ptOrigin,CPoint m_ptEnd);
	void Serialize( CArchive& archive );
	virtual ~CGraph();

};

#endif // !defined(AFX_GRAPH_H__88A49E60_5896_491F_8200_965FF4362629__INCLUDED_)
