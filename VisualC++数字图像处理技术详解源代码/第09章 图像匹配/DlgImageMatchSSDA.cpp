// DlgImageMatchSSDA.cpp : implementation file
//

#include "stdafx.h"
#include "imagematching.h"
#include "DlgImageMatchSSDA.h"

#include "CDib.h"
#include "GlobalApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchSSDA dialog


CDlgImageMatchSSDA::CDlgImageMatchSSDA(CWnd* pParent /*=NULL*/, CImageMatchingDoc* pDoc)
	: CDialog(CDlgImageMatchSSDA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImageMatchSSDA)
	m_lTk = 1;
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
	m_pDibResult = new CDib;
}


void CDlgImageMatchSSDA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImageMatchSSDA)
	DDX_Text(pDX, IDC_THRESHOLD, m_lTk);
	DDV_MinMaxLong(pDX, m_lTk, 1, 2147483647);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImageMatchSSDA, CDialog)
	//{{AFX_MSG_MAP(CDlgImageMatchSSDA)
	ON_BN_CLICKED(IDC_OPENTEMPLATE, OnOpentemplate)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_IMAGEMATCH, OnImagematch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImageMatchSSDA message handlers

void CDlgImageMatchSSDA::CalImageLocation()
{
	// ------------------------------------------------------
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

	// ------------------------------------------------------
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
	
	// 设置控件IDC_IMAGEMATCH的位置大小
	pWnd=GetDlgItem(IDC_IMAGEMATCH);
	pWnd->GetWindowPlacement(winPlacement);
	winPlacement->rcNormalPosition.top = nIniImgBottom +15;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 60;
	pWnd->SetWindowPlacement(winPlacement);
		
	// 调整此对话框的大小	
	this->GetWindowPlacement(winPlacement);
	//winPlacement->rcNormalPosition.top = nIniImgtop -50;
	winPlacement->rcNormalPosition.bottom = nIniImgBottom + 272;
	winPlacement->rcNormalPosition.left   = nIniImgLeft   - 20;
	winPlacement->rcNormalPosition.right  = nIniImgRight  + 20;
	this->SetWindowPlacement(winPlacement);

	// 释放已分配内存
	delete winPlacement;

	// 设置计算图象控件位置标志位为TRUE
	m_bCalImgLoc = TRUE;

}

BOOL CDlgImageMatchSSDA::ImageMatchSSDA(CDib *pDibSrc, CDib *pDibTemplate)
{
	// 指向源图像的指针
	LPBYTE	lpSrc,lpTemplateSrc;
	
	// 指向缓存图像的指针
	LPBYTE	lpDst;

	//循环变量
	long i;
	long j;
	long m;
	long n;

	//中间结果
	double dSigmaS;
	double dSigmaT;
    
	//误差
	double dbr;

	//阈值
    long lTk = m_lTk; 

	//累加误差次数
	long lR = 0;

	//最大累加误加次数
	long  dbMaxR = 0;

    //最小累加误加，dbr<lTk时，最小的r
	double dbMinr;

	//序贯匹配的位置
	int nMaxWidth;
	int nMaxHeight;
	
	//是否有dbr<lTk出现，有br=true.
	bool br=false;

	//像素值
	unsigned char unchPixel;
	unsigned char unchTemplatePixel;

	// 获得图象数据存储的高度和宽度
	CSize sizeSaveImage;
	sizeSaveImage = pDibSrc->GetDibSaveDim();

	// 获得模板图象数据存储的高度和宽度
	CSize sizeSaveTemplate;
	sizeSaveTemplate = pDibTemplate->GetDibSaveDim();


	// 暂时分配内存，以保存新图像
	CDib* pDibNew;
	pDibNew = new CDib;

	// 如果分配内存失败，则推出
	if(!CopyDIB(pDibSrc,pDibNew)){
		// 释放已分配内存
		pDibNew->Empty();

		// 返回
		return FALSE;
	}
	

	// 初始化新分配的内存
	lpDst = (LPBYTE)pDibNew->m_lpImage; 
	
	// 图象的高度
	int nImageHeight ;
	nImageHeight = pDibSrc->m_lpBMIH->biHeight;

	// 图象的宽度
	int nImageWidth;
	nImageWidth = pDibSrc->m_lpBMIH->biWidth;

	// 模板图象的高度
	int nTemplateHeight;
	nTemplateHeight = pDibTemplate->m_lpBMIH->biHeight;

	// 模板图象的宽度
	int nTemplateWidth;
	nTemplateWidth = pDibTemplate->m_lpBMIH->biWidth;

	//计算阈值
	dbMinr=lTk;

	//计算dSigmaT
	dSigmaT = 0;
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			// 指向模板图像倒数第j行，第i个象素的指针,为什么是倒数？			
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			unchTemplatePixel = (unsigned char)*lpTemplateSrc;
			dSigmaT += (double)unchTemplatePixel; //修改
		}
	}
    dSigmaT = dSigmaT/(nTemplateHeight*nTemplateWidth);//我加的

	//序贯算法
	for (j = 0;j < nImageHeight - nTemplateHeight +1 ;j++)
	{
		for(i = 0;i < nImageWidth - nTemplateWidth + 1;i++)
		{
			dSigmaS = 0;
			lR = 0;
			dbr = 0;
			
	        //计算dSigmaS
			for (n = 0;n < nTemplateHeight ;n++)
			{
				for(m = 0;m < nTemplateWidth ;m++)
				{
					// 指向源图像倒数第j+n行，第i+m个象素的指针			
					lpSrc  = (LPBYTE)pDibSrc->m_lpImage + sizeSaveImage.cx * (j+n) + (i+m);
			
					// 指向模板图像倒数第n行，第m个象素的指针			
					lpTemplateSrc  = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;

					unchPixel = (unsigned char)*lpSrc;	
					dSigmaS += (double)unchPixel;//修改	
				}	
			}
			dSigmaS = dSigmaS/(nTemplateHeight*nTemplateWidth);
			//计算误差dbr,lR
            for (n = 0;n < nTemplateHeight ;n++)
			{				
				for(m = 0;m < nTemplateWidth ;m++)
				{
					// 指向源图像倒数第j+n行，第i+m个象素的指针			
					lpSrc  = (LPBYTE)pDibSrc->m_lpImage + sizeSaveImage.cx * (j+n) + (i+m);

					// 指向模板图像倒数第n行，第m个象素的指针			
					lpTemplateSrc  = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;

					unchPixel = (unsigned char)*lpSrc;
					unchTemplatePixel = (unsigned char)*lpTemplateSrc;

					dbr += abs(unchPixel - dSigmaS - unchTemplatePixel+dSigmaT);
					if (dbr >= lTk)
					{
						lR = (m+1)*(n+1);						
						break;//跳出一次循环
					}
				}
				//再跳出一次循环
				if (dbr >= lTk)
					{
						break;
					}
			}
			
			if(dbr<dbMinr)
			{
				br=true;
				dbMinr=dbr;

				nMaxWidth = i;
				nMaxHeight = j;
			}

			if (dbr >= lTk && lR > dbMaxR && br==false)
			{
				 dbMaxR = lR;
				nMaxWidth = i;
				nMaxHeight = j;
			}
		}
	}

	// 对目标图象的象素进行赋值
	for(i=0; i<nImageHeight; i++)
		for( j=0; j<nImageWidth; j++){
			lpDst[i*sizeSaveImage.cx +j] /=2;			
		}

	//将最大相似性出现区域部分复制到目标图像
	for (n = 0;n < nTemplateHeight ;n++)
	{
		for(m = 0;m < nTemplateWidth ;m++)
		{
			lpTemplateSrc = (LPBYTE)pDibTemplate->m_lpImage + sizeSaveTemplate.cx * n + m;
			lpDst = (LPBYTE)pDibNew->m_lpImage + sizeSaveImage.cx * (n+nMaxHeight) + (m+nMaxWidth);
			*lpDst = *lpTemplateSrc;
		}
	}
	
	// 复制图像
	memcpy(pDibSrc->m_lpImage, pDibNew->m_lpImage, nImageWidth * nImageHeight);

	// 释放内存
	pDibNew->Empty();
	
	// 返回
	return TRUE;

}

void CDlgImageMatchSSDA::OnCancel() 
{
	// TODO: Add extra cleanup here
	// 释放已分配内存
	if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}	
	
	CDialog::OnCancel();
}

void CDlgImageMatchSSDA::OnOK() 
{
    // 释放已分配内存
	if(!m_pDibModel->IsEmpty()){
		m_pDibModel->Empty();
		m_pDibModel = NULL;
	}
	if(!m_pDibResult->IsEmpty()){
		m_pDibResult->Empty();
		m_pDibResult = NULL;
	}		
	CDialog::OnOK();
}

void CDlgImageMatchSSDA::OnOpentemplate() 
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

void CDlgImageMatchSSDA::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
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
	if(!m_pDibResult->IsEmpty()){
		sizeDisplay.cx=m_pDibResult->m_lpBMIH->biWidth;
		sizeDisplay.cy=m_pDibResult->m_lpBMIH->biHeight;
		pointDisplay.x = m_rectResltImage.left;
		pointDisplay.y = m_rectResltImage.top;
		m_pDibResult->Draw(&dc,pointDisplay,sizeDisplay);		
	}
	// Do not call CDialog::OnPaint() for painting messages	
}

void CDlgImageMatchSSDA::OnImagematch() 
{
	// TODO: Add your control notification handler code here
	// 更改光标形状
	BeginWaitCursor();

		// 分配结果图象内存
	if(!m_pDibInit->IsEmpty()){
		// 如果分配内存失败，则推出
		if(!CopyDIB(m_pDibInit,m_pDibResult)){
			
			// 释放已分配内存
			m_pDibResult->Empty();

			// 返回
			return ;
		}	
	}

	// 设置计算图象控件位置标志位为FALSE以重新设置图象控件位置
	m_bCalImgLoc = FALSE;

	// 调用ImageMatchSSDA()函数进行模板匹配
	if (ImageMatchSSDA(m_pDibResult, m_pDibModel))
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
