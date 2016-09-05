// GraphShowerView.h : CGraphShowerView 类的接口
//

#pragma once

class CGraphShowerView : public CView
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CGraphShowerView)

public:
	bool	m_bWidhtOut;				// 显示图像是否超出视图宽度
	bool	m_bHeightOut;				// 显示图像是否超出视图高度
	int		m_nShowType;				// 显示类型

	virtual ~CGraphShowerView();

	void ShowPicture(CDC* pDC, Image& image, int nShowType);
										// 以指定模式显示图像
	void SetShowType(int nType);		// 设置显示模式
	CSize GetShowPicSize(Image& image, int nShowType);
										// 获得指定模式下图像的大小

	void OpenFile(const CString& strPath);	// 打开图像文件

	/******************************************************************************
	*	作用:		清除图像的偏移坐标
	******************************************************************************/
	void ClearOffset(void)
	{
		m_nXX = m_nYY = m_nXXMax = m_nYYMax = 0;
	}

	void FixOffset(void);				// 修正图像的偏移坐标

	/***************************************************************************
	*	作用:		获得指定索引文件的全路径
	***************************************************************************/
	CString GetFilePath(int nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nPicNum);
		return m_strPath + L"\\" + m_FilesNameAry[nIndex];
	}

	void RefreshIndicator();			// 更新状态栏的指示器信息

	CGraphShowerDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);		// 重写以绘制该视图

	// 消息响应函数
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
										// 鼠标移动的响应函数
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
										// 单击鼠标左键的响应函数
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
										// 设置鼠标光标
	afx_msg void OnFileOpen();			// "打开"的命令响应函数
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);// 禁止重绘背景

	// "显示模式"菜单相关函数
	afx_msg void OnModeNormal();
	afx_msg void OnModeWidth();
	afx_msg void OnModeHeight();
	afx_msg void OnModeScreen();
	afx_msg void OnUpdateModeNormal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeWidth(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeHeight(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeScreen(CCmdUI *pCmdUI);
	// "视图"下"图像"菜单相关函数
	afx_msg void OnImagePrev();
	afx_msg void OnImageNext();
	afx_msg void OnImageFirst();
	afx_msg void OnImageLast();
	afx_msg void OnUpdateImagePrev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageNext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageFirst(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageLast(CCmdUI *pCmdUI);
	// "显示特效"菜单相关函数
	afx_msg void OnEffectNone();
	afx_msg void OnEffectRand();
	afx_msg void OnEffectScan();
	afx_msg void OnEffectDscan();
	afx_msg void OnEffectMove();
	afx_msg void OnEffectVBlind();
	afx_msg void OnEffectHRaster();
	afx_msg void OnEffectMosaic();
	afx_msg void OnEffectHsmove();
	afx_msg void OnEffectSblind();
	afx_msg void OnEffectVraster();
	afx_msg void OnEffectRaindrop();
	afx_msg void OnUpdateEffectNone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectScan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectDscan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectMove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectBlind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRaster(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectMosaic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectHsmove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectSblind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectVraster(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRaindrop(CCmdUI *pCmdUI);

protected:
	CGraphShowerView();

	virtual void OnInitialUpdate();		// 更新函数
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	int				m_nEffectDisplayType;// 当前特效类型
	bool			m_bEffectDraw;		// 是否使用特效显示
	Bitmap*			m_pBitmap;			// 双缓存绘图
	int				m_nPicNum;			// 当前目录的文件个数
	CString			m_strPath;			// 当前打开目录
	CStringArray	m_FilesNameAry;		// 保存当前目录下所有文件名的容器
	int				m_nPos;				// 当前文件在容器中的索引
	int				m_nShowPicHeight;	// 显示图片的高度
	int				m_nShowPicWidth;	// 显示图片的宽度

	CPoint			m_psMove;			// 记录移动前鼠标的位置
	int				m_nXX;				// 图片X坐标的偏移量
	int				m_nYY;				// 图片Y坐标的偏移量
	int				m_nXXMax;			// X坐标最大偏移量
	int				m_nYYMax;			// Y坐标最大偏移量

	void PreDrawImage(void);			// 在内存中预先绘制图像
	void EffectDisplayImage(CDC* pDC, CDC* pMemDC);
										// 使用特效显示图像

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#ifndef _DEBUG  // GraphShowerView.cpp 中的调试版本
inline CGraphShowerDoc* CGraphShowerView::GetDocument() const
   { return reinterpret_cast<CGraphShowerDoc*>(m_pDocument); }
#endif

