///////////////////////////////////////////////////////////////////////////
//CListCtrl类的一个派生类
//该类的几点功能：
//1、可以使用多线程
//2、可以在第一列插入图标
//3、可以改变字体颜色
//说明：用过几个CListCtrl的派生类
//有些类可以使用多线程，但是由于重绘的原因始终加不上图标
//有些类可以加入图标，但由于Windows消息的原因，使用多线程就出错
//由于数据量太多，而且需要大量时间去计算，又不想使用户觉得界面卡，只能用多线程
//于是就综合了一下，写了这个类，功能简单，容易理解。
//由于是第一次写类，还希望大家多多提建议
//by l0g1n-------2012年6月12日   17：44     QQ:519710391
///////////////////////////////////////////////////////////////////////////
#pragma once

// CMyList
class CMyList : public CListCtrl
{
	DECLARE_DYNAMIC(CMyList)

public:
	CMyList();
	virtual ~CMyList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	//设置图标图标关联
	CImageList* SetImageList(CImageList *pImageList);
	//设置图标id
	BOOL SetItemImageId(int nItem,int nImageId);
	//插入数据，可设置字体颜色
	int InsertItem(int nItem,LPCTSTR lpText,COLORREF fontcolor=RGB(0,0,0));
};
