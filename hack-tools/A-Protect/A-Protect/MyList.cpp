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
#include "stdafx.h"
#include "MyList.h"

// CMyList

IMPLEMENT_DYNAMIC(CMyList, CListCtrl)

CMyList::CMyList()
{
}

CMyList::~CMyList()
{
}

BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyList::OnNMCustomdraw)
END_MESSAGE_MAP()

CMap<DWORD , DWORD& , COLORREF , COLORREF&> MapItemColor;

void CMyList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
	    *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
	    *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if (lplvcd->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
    {
		COLORREF ItemColor;
		if(MapItemColor.Lookup((lplvcd->nmcd.dwItemSpec), ItemColor))
		{
                //lplvdr->clrText = RGB(0,0,0);//ItemColor;
				lplvcd->clrText = ItemColor;
                *pResult = CDRF_DODEFAULT;
        }
	}
}
// CMyList 消息处理程序

//设置图标图标关联
CImageList* CMyList::SetImageList(CImageList *pImageList)
{
	return CListCtrl::SetImageList(pImageList,LVSIL_SMALL);
}
//设置图标id
BOOL CMyList::SetItemImageId(int nItem,int nImageId)
{
	return CListCtrl::SetItem(nItem,0,LVIF_IMAGE,NULL,nImageId,0,0,0);
}
//插入数据，可设置字体颜色
int CMyList::InsertItem(int nItem,LPCTSTR lpText,COLORREF fontcolor)
{
	const int IDX = CListCtrl::InsertItem(nItem, lpText);
	//改变颜色
	DWORD iItem=(DWORD)nItem;
	MapItemColor.SetAt(iItem, fontcolor);
	CListCtrl::RedrawItems(iItem,iItem);
	CListCtrl::Update(iItem);
	return IDX;
}