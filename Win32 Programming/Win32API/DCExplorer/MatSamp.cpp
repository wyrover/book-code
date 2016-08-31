// MatSamp.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "MatSamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatrixSample

CMatrixSample::CMatrixSample()
{
 XFORM identity = {1.0f, 0.0f, 
 	           0.0f, 1.0f,
		   0.0f, 0.0f};
 matrix = identity;
}

CMatrixSample::~CMatrixSample()
{
}


BEGIN_MESSAGE_MAP(CMatrixSample, CStatic)
	//{{AFX_MSG_MAP(CMatrixSample)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixSample message handlers

void CMatrixSample::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}
