// PropView.cpp : implementation of the CPropView class
//

#include "stdafx.h"
#include "Prop.h"

#include "PropDoc.h"
#include "PropView.h"

#include "PropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropView

IMPLEMENT_DYNCREATE(CPropView, CView)

BEGIN_MESSAGE_MAP(CPropView, CView)
	//{{AFX_MSG_MAP(CPropView)
	ON_COMMAND(IDM_PROPERTYSHEET, OnPropertysheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropView construction/destruction

CPropView::CPropView()
{
	// TODO: add construction code here
	m_iOccupation=-1;
	m_strWorkAddr="";
	memset(m_bLike,0,sizeof(m_bLike));
	m_strSalary="";
}

CPropView::~CPropView()
{
}

BOOL CPropView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPropView drawing

void CPropView::OnDraw(CDC* pDC)
{
	CPropDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CFont font;
	font.CreatePointFont(300,"华文行楷");

	CFont *pOldFont;
	pOldFont=pDC->SelectObject(&font);

	CString strTemp;
	strTemp="你的职业：";

	switch(m_iOccupation)
	{
	case 0:
		strTemp+="程序员";
		break;
	case 1:
		strTemp+="系统工程师";
		break;
	case 2:
		strTemp+="项目经理";
		break;
	default:
		break;
	}
	pDC->TextOut(0,0,strTemp);

	strTemp="你的工作地点：";
	strTemp+=m_strWorkAddr;

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	pDC->TextOut(0,tm.tmHeight,strTemp);

	strTemp="你的兴趣爱好：";
	if(m_bLike[0])
	{
		strTemp+="足球 ";
	}
	if(m_bLike[1])
	{
		strTemp+="篮球 ";
	}
	if(m_bLike[2])
	{
		strTemp+="排球 ";
	}
	if(m_bLike[3])
	{
		strTemp+="游泳 ";
	}
	pDC->TextOut(0,tm.tmHeight*2,strTemp);

	strTemp="你的薪资水平：";
	strTemp+=m_strSalary;
	pDC->TextOut(0,tm.tmHeight*3,strTemp);

	pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CPropView diagnostics

#ifdef _DEBUG
void CPropView::AssertValid() const
{
	CView::AssertValid();
}

void CPropView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPropDoc* CPropView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPropDoc)));
	return (CPropDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPropView message handlers

void CPropView::OnPropertysheet() 
{
	// TODO: Add your command handler code here
	CPropSheet propSheet("维新属性表单程序");
	propSheet.SetWizardMode();
	if(ID_WIZFINISH==propSheet.DoModal())
	{
		m_iOccupation=propSheet.m_prop1.m_occupation;
		m_strWorkAddr=propSheet.m_prop1.m_workAddr;
		m_bLike[0]=propSheet.m_prop2.m_football;
		m_bLike[1]=propSheet.m_prop2.m_basketball;
		m_bLike[2]=propSheet.m_prop2.m_volleyball;
		m_bLike[3]=propSheet.m_prop2.m_swim;
		m_strSalary=propSheet.m_prop3.m_strSalary;
		Invalidate();
	}
}
