// DlgImageMatchMoment.cpp : implementation file
//

#include "stdafx.h"
#include "imagematching.h"
#include "DlgImageMatchMoment.h"

#include "CDib.h"
#include "GlobalApi.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchMoment dialog


CDlgImageMatchMoment::CDlgImageMatchMoment(CWnd* pParent /*=NULL*/, CImageMatchingDoc* pDoc)
	: CDialog(CDlgImageMatchMoment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImageMatchMoment)
	m_a1_inimage = 0.0;
	m_a1_template = 0.0;
	m_a2_inimage = 0.0;
	m_a2_template = 0.0;
	m_a3_inimage = 0.0;
	m_a3_template = 0.0;
	m_a4_inimage = 0.0;
	m_a4_template = 0.0;
	m_a5_inimage = 0.0;
	m_a5_template = 0.0;
	m_a6_inimage = 0.0;
	m_a6_template = 0.0;
	m_a7_inimage = 0.0;
	m_a7_template = 0.0;
	m_R = 0.0;
	//}}AFX_DATA_INIT
	// 设置计算图象控件位置标志位为FALSE
	m_bCalImgLoc = FALSE;

	// 设置初始图象
	m_pDibInit = pDoc->m_pDibInit;

	// 获得文档指针
	m_pDoc = pDoc;

	// 分配模板图象内存
	m_pDibModel = new CDib;
		
	// 分配结果图象
//	m_pDibResult = new CDib;
}


void CDlgImageMatchMoment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImageMatchMoment)
	DDX_Text(pDX, IDC_A1_INIIMAGE, m_a1_inimage);
	DDX_Text(pDX, IDC_A1_TEMPLATE, m_a1_template);
	DDX_Text(pDX, IDC_A2_INIIMAGE, m_a2_inimage);
	DDX_Text(pDX, IDC_A2_TEMPLATE, m_a2_template);
	DDX_Text(pDX, IDC_A3_INIIMAGE, m_a3_inimage);
	DDX_Text(pDX, IDC_A3_TEMPLATE, m_a3_template);
	DDX_Text(pDX, IDC_A4_INIIMAGE, m_a4_inimage);
	DDX_Text(pDX, IDC_A4_TEMPLATE, m_a4_template);
	DDX_Text(pDX, IDC_A5_INIIMAGE, m_a5_inimage);
	DDX_Text(pDX, IDC_A5_TEMPLATE, m_a5_template);
	DDX_Text(pDX, IDC_A6_INIIMAGE, m_a6_inimage);
	DDX_Text(pDX, IDC_A6_TEMPLATE, m_a6_template);
	DDX_Text(pDX, IDC_A7_INIIMAGE, m_a7_inimage);
	DDX_Text(pDX, IDC_A7_TEMPLATE, m_a7_template);
	DDX_Text(pDX, IDC_SIMULATE, m_R);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImageMatchMoment, CDialog)
	//{{AFX_MSG_MAP(CDlgImageMatchMoment)
	ON_BN_CLICKED(IDC_CALMOMENT, OnCalmoment)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OPENTEMPLATE, OnOpentemplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchMoment message handlers

void CDlgImageMatchMoment::CalImageLocation()
{
	// 获得控件IDC_INIIMAGE的句柄，并获得控件的初始位置信息
	CWnd* pWnd=GetDlgItem(IDC_INIIMAGE);
	WINDOWPLACEMENT *winPlacement;
	winPlacement=new WINDOWPLACEMENT;
	pWnd->GetWindowPlacement(winPlacement);
	
	// 图象宽度
	int nImageWidth;
	nImageWidth = m_pDibInit->m_lpBMIH->biWidth;

	// 图象高度
	int nImageHeight;
	nImageHeight = m_pDibInit->m_lpBMIH->biHeight;

	// -----------------------------------------------------------------
	// 调整控件IDC_INIIMAGE的大小位置，并同时设置显示基准图象的位置
	if(nImageHeight > 256){
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 256;
		m_rectInitImage.bottom = winPlacement->rcNormalPosition.top + 128 + nImageHeight/2;
		m_rectInitImage.top    = winPlacement->rcNormalPosition.top + 128 - nImageHeight/2;
	}
	if(nImageWidth > 256){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectInitImage.right = winPlacement->rcNormalPosition.right;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 256;
		m_rectInitImage.right = winPlacement->rcNormalPosition.left + 128 + nImageWidth/2;
		m_rectInitImage.left  = winPlacement->rcNormalPosition.left + 128 - nImageWidth/2;
	}

	// 设置IDC_INIIMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);	
			
	// 获得显示模板图象控件的右边位置，以便确认显示模板图象控件的位置
	int nIniImgRight;
	nIniImgRight = winPlacement->rcNormalPosition.right;
	
	int  nIniImgLeft;
	nIniImgLeft   = winPlacement->rcNormalPosition.left;

	// 获得IDC_INIIMAGEE控件的下边位置，以便调整其他控件的位置
	int nIniImgBottom;
	nIniImgBottom = winPlacement->rcNormalPosition.bottom;

	// 获得IDC_INIIMAGE控件的下边位置，以便调整其他控件的位置
	int nIniImgtop = winPlacement->rcNormalPosition.top;

	// ------------------------------------------------------
	// 获得控件IDC_TEMPLATE的句柄，并获得初始位置信息
	pWnd=GetDlgItem(IDC_TEMPLATE);
	pWnd->GetWindowPlacement(winPlacement);
	
	// 如果还未打开模板图象，则设置结果图象大小和初始图象大小相等
	if(!m_pDibModel->IsEmpty()){
		nImageWidth  = m_pDibModel->m_lpBMIH->biWidth;
		nImageHeight = m_pDibModel->m_lpBMIH->biHeight;
	}
	
	// 调整控件IDC_TEMPLATE的大小位置，并同时设置显示结果图象的位置

	// 先调整控件的左边位置，和IDC_INIIMAGE控件相隔15个象素
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	if(nImageHeight > 256){		
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;
		m_rectModelImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectModelImage.top    = nIniImgtop;		
	}
	else{
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 256;
		m_rectModelImage.bottom = winPlacement->rcNormalPosition.top + 128 + nImageHeight/2;
		m_rectModelImage.top    = winPlacement->rcNormalPosition.top + 128 - nImageHeight/2;
	}
	if(nImageWidth > 256){		
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectModelImage.right = winPlacement->rcNormalPosition.right;
		m_rectModelImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{		
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 256;
		m_rectModelImage.right = winPlacement->rcNormalPosition.left + 128 + nImageWidth/2;
		m_rectModelImage.left  = winPlacement->rcNormalPosition.left + 128 - nImageWidth/2;
	}

	// 设置IDC_TEMPLATE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);
		
	// 获得IDC_TEMPLATE控件的右边位置，以便调整其他控件的位置
	nIniImgRight  = winPlacement->rcNormalPosition.right;

	/* ------------------------------------------------------
	// 获得控件IDC_RESULTIMAGE的句柄，并获得初始位置信息
	pWnd=GetDlgItem(IDC_RESULTIMAGE);
	pWnd->GetWindowPlacement(winPlacement);
	
	// 如果还未生成结果图象，则设置结果图象大小和初始图象大小相等
	if(!m_pDibResult->IsEmpty()){
		nImageWidth  = m_pDibResult->m_lpBMIH->biWidth;
		nImageHeight = m_pDibResult->m_lpBMIH->biHeight;
	}
	
	// 调整控件IDC_RESULTIMAGE的大小位置，并同时设置显示结果图象的位置

	// 先调整控件的左边位置，和IDC_TEMPLATE控件相隔15个象素
	winPlacement->rcNormalPosition.left = nIniImgRight + 15;

	if(nImageHeight > 256){		
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + nImageHeight;	
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.bottom;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top;		
	}
	else{
		winPlacement->rcNormalPosition.top = nIniImgtop;
		winPlacement->rcNormalPosition.bottom = winPlacement->rcNormalPosition.top + 256;
		m_rectResltImage.bottom = winPlacement->rcNormalPosition.top + 128 + nImageHeight/2;
		m_rectResltImage.top    = winPlacement->rcNormalPosition.top + 128 - nImageHeight/2;
	}
	if(nImageWidth > 256){
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + nImageWidth;
		m_rectResltImage.right = winPlacement->rcNormalPosition.right;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left;			
	}
	else{
		winPlacement->rcNormalPosition.right = winPlacement->rcNormalPosition.left + 256;
		m_rectResltImage.right = winPlacement->rcNormalPosition.left + 128 + nImageWidth/2;
		m_rectResltImage.left  = winPlacement->rcNormalPosition.left + 128 - nImageWidth/2;
	}

	// 设置IDC_RESULTIMAGE控件的大小位置
	pWnd->SetWindowPlacement(winPlacement);
	
	// ------------------------------------------------------
	if(nIniImgBottom > winPlacement->rcNormalPosition.bottom)
		nIniImgBottom = winPlacement->rcNormalPosition.bottom;
	
	nIniImgRight  = winPlacement->rcNormalPosition.right;
	*/
	// 设置控件IDOK的位置大小
	pWnd=GetDlgItem(IDOK);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDCANCEL的位置大小
	pWnd=GetDlgItem(IDCANCEL);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);

	// 设置控件IDC_OPENTEMPLATE的位置大小
	pWnd=GetDlgItem(IDC_OPENTEMPLATE);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
	
	// 设置控件IDC_CALMOMENT的位置大小
	pWnd=GetDlgItem(IDC_CALMOMENT);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
		
	// 调整此对话框的大小	
	this->GetWindowPlacement(winPlacement);
	//winPlacement->rcNormalPosition.top = nIniImgtop -50;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 172;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 350;
	this->SetWindowPlacement(winPlacement);

	// 释放已分配内存
	delete winPlacement;

	// 设置计算图象控件位置标志位为TRUE
	m_bCalImgLoc = TRUE;

}

void CDlgImageMatchMoment::OnCalmoment() 
{
	// TODO: Add your control notification handler code here
	// 更改光标形状
	BeginWaitCursor();

		// 分配结果图象内存
/*	if(!m_pDibInit->IsEmpty()){
		// 如果分配内存失败，则推出
		if(!CopyDIB(m_pDibInit,m_pDibResult)){
			
			// 释放已分配内存
			m_pDibResult->Empty();

			// 返回
			return ;
		}	
	}*/

	// 设置计算图象控件位置标志位为FALSE以重新设置图象控件位置
	m_bCalImgLoc = FALSE;

	// 调用CalMoment()函数进行模板匹配
	if (CalMoment(m_pDibInit, m_pDibModel))
	{
		// 更新显示
		Invalidate();
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 恢复光标
	EndWaitCursor();

	// 更新视图
	Invalidate();
}

void CDlgImageMatchMoment::OnOK() 
{
	// TODO: Add extra validation here
	if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
/*	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}*/	

	CDialog::OnOK();
}

void CDlgImageMatchMoment::OnCancel() 
{
    if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
/*	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}*/		
	CDialog::OnCancel();
}

void CDlgImageMatchMoment::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// 如果还没有计算图象的位置，则进行计算
	if(!m_bCalImgLoc){
		CalImageLocation();
	}

	// 显示大小
	CSize sizeDisplay;

	// 显示位置
	CPoint pointDisplay;
	
	// 显示初始图象
	if(!m_pDibInit->IsEmpty()){
		sizeDisplay.cx=m_pDibInit->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibInit->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectInitImage.left;
		pointDisplay.y = m_rectInitImage.top;
		m_pDibInit->Draw(&dc,pointDisplay,sizeDisplay);		
	}
	
	// 显示模板图象
	if(!m_pDibModel->IsEmpty()){
		sizeDisplay.cx=m_pDibModel->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibModel->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectModelImage.left;
		pointDisplay.y = m_rectModelImage.top;
		m_pDibModel->Draw(&dc,pointDisplay,sizeDisplay);		
	}

	// 显示结果图象
/*	if(!m_pDibResult->IsEmpty()){
		sizeDisplay.cx=m_pDibResult->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibResult->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;
		m_pDibResult->Draw(&dc,pointDisplay,sizeDisplay);		
	}*/
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgImageMatchMoment::OnOpentemplate() 
{
    CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CFile file;
	 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		// 打开文件
		if( !file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite))
		{
			// 返回	
			return ;
		}

		// 读入模板图象
		if(!m_pDibModel->Read(&file)){
			// 恢复光标形状
			EndWaitCursor();

			// 清空已分配内存
			m_pDibModel->Empty();

			// 返回
 			return;
		}
	}
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模板配准，其它的可以类推）
	if(m_pDibModel->m_nColorTableEntries != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 清空已分配内存
		m_pDibModel->Empty();

		// 返回
		return;
	}

	// 初始图象的长宽大小
	CSize sizeImage			= m_pDibInit->GetDimensions();
	int nImageWidth			= sizeImage.cx		;
	int nImageHeight			= sizeImage.cy		;
	
	// 模板图象的长宽大小
	CSize sizeModelImage	= m_pDibModel->GetDimensions()	;
	int nModelWidth		= sizeImage.cx		;
	int nModelHeight	= sizeImage.cy		;

	// 判断模板尺寸和初始图象的大小，如果模板大于初始图象，则推出
	if(nModelHeight > nImageHeight || nModelWidth > nImageWidth )
	{
		// 提示用户
		MessageBox("模板尺寸大于源图像尺寸！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 释放已分配内存
		m_pDibModel->Empty();

		// 返回
		return;

	}

	// 如果打开新的待配准文件，将图象位置设置标志位设为FALSE，以便再次调整位置
	m_bCalImgLoc = FALSE;
	
	// 更新显示	
	this->Invalidate();		
}


BOOL CDlgImageMatchMoment::CalMoment(CDib *pDibSrc, CDib *pDibTemplate)
{
	// 原图，模板重心x，y坐标值
	int nBarycenterX,nBarycenterY;
	int nTBarycenterX, nTBarycenterY;
	
	// 循环变量
	int i,j;

	// 临时变量
	double temp;

	//原图，模板的七个不变矩，二阶，三阶重心矩
	double Sa[8], Ta[8];
	double Su00, Su02, Su20, Su11, Su30, Su12, Su21, Su03;
	double Tu00, Tu02, Tu20, Tu11, Tu30, Tu12, Tu21, Tu03;

	//原图，模板的相似度
	double dbR;
	//中间结果
	double dSigmaST=0;
	double dSigmaS=0;
	double dSigmaT=0;

	//计算重心x,y坐标
	CalBarycenter(pDibSrc, &nBarycenterX, &nBarycenterY);
	CalBarycenter(pDibTemplate, &nTBarycenterX, &nTBarycenterY);

	//计算二阶，三阶重心矩
/*	Su02 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 0, 2);
	Su20 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 2, 0);
	Su11 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 1, 1);
	Su30 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 3, 0);
	Su12 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 1, 2);
	Su21 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 2, 1);
	Su03 = BarycenterMoment(pDibSrc, nBarycenterX, nBarycenterY, 0, 3);

	Tu02 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 0, 2);
	Tu20 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 2, 0);
	Tu11 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 1, 1);
	Tu30 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 3, 0);
	Tu12 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 1, 2);
	Tu21 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 2, 1);
	Tu03 = BarycenterMoment(pDibTemplate, nTBarycenterX, nTBarycenterY, 0, 3);
*/
	Su00 = BarycenterMoment(pDibSrc, 0, 0, 0, 0);
	Su02 = BarycenterMoment(pDibSrc, 0, 0, 0, 2)/pow(Su00,2);
	Su20 = BarycenterMoment(pDibSrc, 0, 0, 2, 0)/pow(Su00,2);
	Su11 = BarycenterMoment(pDibSrc, 0, 0, 1, 1)/pow(Su00,2);
	Su30 = BarycenterMoment(pDibSrc, 0, 0, 3, 0)/pow(Su00,2.5);
	Su12 = BarycenterMoment(pDibSrc, 0, 0, 1, 2)/pow(Su00,2.5);
	Su21 = BarycenterMoment(pDibSrc, 0, 0, 2, 1)/pow(Su00,2.5);
	Su03 = BarycenterMoment(pDibSrc, 0, 0, 0, 3)/pow(Su00,2.5);

	Tu00 = BarycenterMoment(pDibTemplate, 0, 0, 0, 0);
	Tu02 = BarycenterMoment(pDibTemplate, 0, 0, 0, 2)/pow(Tu00,2);
	Tu20 = BarycenterMoment(pDibTemplate, 0, 0, 2, 0)/pow(Tu00,2);
	Tu11 = BarycenterMoment(pDibTemplate, 0, 0, 1, 1)/pow(Tu00,2);
	Tu30 = BarycenterMoment(pDibTemplate, 0, 0, 3, 0)/pow(Tu00,2.5);
	Tu12 = BarycenterMoment(pDibTemplate, 0, 0, 1, 2)/pow(Tu00,2.5);
	Tu21 = BarycenterMoment(pDibTemplate, 0, 0, 2, 1)/pow(Tu00,2.5);
	Tu03 = BarycenterMoment(pDibTemplate, 0, 0, 0, 3)/pow(Tu00,2.5);

	Sa[1] = Su20 + Su02;
	Sa[2] = (Su20 - Su02)*(Su20 - Su02)+4*Su11*Su11;
	Sa[3] = pow((Su30 - 3*Su12), 2) + pow((3*Su21 - Su03), 2);
	Sa[4] = pow((Su30 + Su12), 2) + pow((Su21 + Su03), 2);
	Sa[5] = (Su30 - 3*Su12)*(Su30 + Su12)*(pow((Su30 + Su12), 2)-3*pow((Su21+Su03),2))
		+3*(Su21-Su03)*(Su21+Su03)*(3*pow((Su30+Su12),2)-pow((Su21+Su03),2));
	Sa[6] = (Su20-Su02)*(pow((Su30+Su12),2)-pow((Su21+Su03),2))+
		4*Su11*(Su30+Su12)*(Su21+Su03);
	Sa[7] = (3*Su21-Su03)*(Su30+Su12)*(pow((Su30+Su12),2)-3*pow((Su21+Su03),2))+
		(Su30-3*Su12)*(Su21+Su03)*(3*pow((Su30+Su12),2)-pow((Su21+Su03),2));

	Ta[1] = Tu20 + Tu02;
	Ta[2] = (Tu20 - Tu02)*(Tu20 - Tu02)+4*Tu11*Tu11;
	Ta[3] = pow((Tu30 - 3*Tu12), 2) + pow((3*Tu21 - Tu03), 2);
	Ta[4] = pow((Tu30 + Tu12), 2) + pow((Tu21 + Tu03), 2);
	Ta[5] = (Tu30 - 3*Tu12)*(Tu30 + Tu12)*(pow((Tu30 + Tu12), 2)-3*pow((Tu21+Tu03),2))
		+3*(Tu21-Tu03)*(Tu21+Tu03)*(3*pow((Tu30+Tu12),2)-pow((Tu21+Tu03),2));
	Ta[6] = (Tu20-Tu02)*(pow((Tu30+Tu12),2)-pow((Tu21+Tu03),2))+
		4*Tu11*(Tu30+Tu12)*(Tu21+Tu03);
	Ta[7] = (3*Tu21-Tu03)*(Tu30+Tu12)*(pow((Tu30+Tu12),2)-3*pow((Tu21+Tu03),2))+
		(Tu30-3*Tu12)*(Tu21+Tu03)*(3*pow((Tu30+Tu12),2)-pow((Tu21+Tu03),2));

	for(i=1;i<8;i++)
	{
		temp=Sa[i]*Ta[i];
		dSigmaST+=temp;
		
		dSigmaS+=pow(Sa[i],2);

		dSigmaT+=pow(Ta[i],2);
	}
	dbR=dSigmaST/( sqrt(dSigmaS)*sqrt(dSigmaT));

	m_a1_inimage=Sa[1];
	m_a1_template=Ta[1];
	m_a2_inimage=Sa[2];
	m_a2_template=Ta[2];
	m_a3_inimage=Sa[3];
	m_a3_template=Ta[3];
	m_a4_inimage=Sa[4];
	m_a4_template=Ta[4];
	m_a5_inimage=Sa[5];
	m_a5_template=Ta[5];
	m_a6_inimage=Sa[6];
	m_a6_template=Ta[6];
	m_a7_inimage=Sa[7];
	m_a7_template=Ta[7];
	m_R=dbR;
	UpdateData(FALSE);

	return true;

}

void CDlgImageMatchMoment::CalBarycenter(CDib *pDib, int *nBarycenterX, int *nBarycenterY)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 图象象素值
	int  nPixelValue;

	// 0次矩m00,x方向的一次矩m01和y方向的一次矩m10
	double m00, m10, m01;
	
	// 循环变量
	int i,j;

	// 临时变量
	double temp;

	// 赋初值为零
	m00 = 0;
	m01 = 0;
	m10 = 0;

	// 求0次矩m00,x方向的一次矩m01和y方向的一次矩m10
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;
			nPixelValue = *lpSrc;

			m00 = m00 + nPixelValue;

			temp = i * nPixelValue;
			m10  = m10 + temp;

			temp = j * nPixelValue;
			m01  = m01 + temp;
		}
	}

	// 重心x，y坐标值
	*nBarycenterX = (int)(m10 / m00 + 0.5);
	*nBarycenterY = (int)(m01 / m00 + 0.5);
}

double CDlgImageMatchMoment::BarycenterMoment(CDib *pDib, int nBarycenterX, int nBarycenterY, int ip, int jq)
{
	// 指向源图像的指针
	BYTE *	lpSrc;

	//图象的宽度和高度
	LONG    lWidth;
	LONG    lHeight;

	// 图像每行的字节数
	LONG	lLineBytes;
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth  = SizeDim.cx;
	lHeight = SizeDim.cy;	
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图像每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	//图像数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 图象的矩
	double dbImageMoment;

	// 循环变量
	int i,j;

	// 临时变量
	double temp;

	dbImageMoment = 0;

	// 力矩的计算
	for (j = 0; j < lHeight; j++)
	{
		for(i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * j + i;

			temp =pow((double)(i-nBarycenterX),ip)*pow((double)(j-nBarycenterY),jq);
			temp = temp * (*lpSrc);

			dbImageMoment = dbImageMoment + temp;
		}
	}
	
	return dbImageMoment;
}
