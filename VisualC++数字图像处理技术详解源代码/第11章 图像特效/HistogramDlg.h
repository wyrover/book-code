#pragma once
#include "afxwin.h"

// CHistogramDlg 对话框

class CHistogramDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CStatic m_stiHistogram;		// 直方图显示区域
	int		m_nLimitLow;		// 显示灰度的下限
	int		m_nLimitUp;			// 显示灰度的上限
	long	m_lCounts[256];		// 各级灰度出现的个数
	long	m_nPixelCount;		// 图像像素总数
	CPoint	m_psMove;			// 记录托拽时的鼠标位置
	int		m_nIsDraging;		// 鼠标是否正在托拽
	int		m_nGray;			// 当前鼠标位置的灰度级数
	float	m_dPer;				// 出现概率


	CHistogramDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CHistogramDlg();

	afx_msg void OnEnChangeLimitLower();	// 灰度下限改变的响应函数
	afx_msg void OnEnChangeLimitUp();		// 灰度上限改变的响应函数
	afx_msg void OnPaint();					// 绘制对话框
	virtual BOOL OnInitDialog();			// 对话框初始化时计算各级灰度数量
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
											// 鼠标移动时响应托拽动作
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
											// 托拽时改变鼠标光标
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
											// 鼠标按下时判断是否在灰度上下限直线范围中
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
											// 释放鼠标的响应函数

	// 对话框数据
	enum { IDD = IDD_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	void Refresh(void);	// 刷新直方图

	// 拖动枚举
	enum DragingType
	{
		DT_NULL,		// 无拖动
		DT_LOW,			// 拖动下限
		DT_UP			// 拖动上限
	};
};
