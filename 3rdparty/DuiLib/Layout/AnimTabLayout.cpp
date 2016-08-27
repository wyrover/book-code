#include "stdafx.h"
#include "AnimTabLayout.h"

namespace DuiLib{


///////////////////////////////////////////////////////////////////////////////////////
CAnimationTabLayoutUI::CAnimationTabLayoutUI() : 
	CUIAnimation( this ), 
	m_bIsVerticalDirection( false ), 
	m_nPositiveDirection( 1 ),
	m_nAnimId(1111),
	m_nAnimEllapse(0),		//20
	m_nAnimFrame(0){	//15

}


LPCTSTR CAnimationTabLayoutUI::GetClass() const
{
	return _T("AnimationTabLayoutUI");
}

LPVOID CAnimationTabLayoutUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("AnimationTabLayoutUI")) == 0 ) 
		return static_cast<CAnimationTabLayoutUI*>(this);
	return CTabLayoutUI::GetInterface(pstrName);
}

bool CAnimationTabLayoutUI::SelectItem( int iIndex )
{
	if (!(m_nAnimFrame && m_nAnimEllapse)){
		return __super::SelectItem(iIndex);
	}

	if( iIndex < 0 || iIndex >= m_items.GetSize() ) return false;
	if( iIndex == m_iCurSel ) return true;
	if( iIndex > m_iCurSel ) m_nPositiveDirection = -1;
	if( iIndex < m_iCurSel ) m_nPositiveDirection = 1;

	int iOldSel = m_iCurSel;
	m_iCurSel = iIndex;
	for( int it = 0; it < m_items.GetSize(); it++ )
	{
		if( it == iIndex ) {
			//GetItemAt(it)->SetVisible(true);
			//GetItemAt(it)->SetFocus();
			AnimationSwitch();
		}
		else GetItemAt(it)->SetVisible(false);
	}
	NeedParentUpdate();

	if( m_pManager != NULL ) {
		m_pManager->SetNextTabControl();
		m_pManager->SendNotify(this, _T("tabselect"), m_iCurSel, iOldSel);
	}
	return true;
}


void CAnimationTabLayoutUI::AnimationSwitch()
{
	m_rcItemOld = m_rcItem;
	if( !m_bIsVerticalDirection )
	{
		m_rcCurPos.top = m_rcItem.top;
		m_rcCurPos.bottom = m_rcItem.bottom;
		m_rcCurPos.left = m_rcItem.left - ( m_rcItem.right - m_rcItem.left ) * m_nPositiveDirection;
		m_rcCurPos.right = m_rcItem.right - ( m_rcItem.right - m_rcItem.left ) * m_nPositiveDirection;				
	}
	else
	{
		m_rcCurPos.left = m_rcItem.left;
		m_rcCurPos.right = m_rcItem.right;
		m_rcCurPos.top = m_rcItem.top - ( m_rcItem.bottom - m_rcItem.top ) * m_nPositiveDirection;
		m_rcCurPos.bottom = m_rcItem.bottom - ( m_rcItem.bottom - m_rcItem.top ) * m_nPositiveDirection;		
	}

	StopAnimation( m_nAnimId );
	StartAnimation( GetEllapse(), GetFrameSize(), m_nAnimId );
}

void CAnimationTabLayoutUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_TIMER ) 
	{
		OnTimer(  event.wParam );
	}
	__super::DoEvent( event );
}

void CAnimationTabLayoutUI::OnTimer( int nTimerID )
{
	OnAnimationElapse( nTimerID );
}

//////////////////////////////////////////////////////////////////////////
void CAnimationTabLayoutUI::OnAnimationStart(INT nAnimationID, BOOL bFirstLoop)
{
	if( ! (GetItemAt(m_iCurSel)->IsVisible())) {
		GetItemAt(m_iCurSel)->SetVisible(true);
		GetItemAt(m_iCurSel)->SetFocus();
	}
}

void CAnimationTabLayoutUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
{
	int iStepLen = 0;
	if( !m_bIsVerticalDirection ){
		iStepLen = ( m_rcItemOld.right - m_rcItemOld.left ) * m_nPositiveDirection / nTotalFrame;
		if( nCurFrame != nTotalFrame ){
			m_rcCurPos.left = m_rcCurPos.left + iStepLen;
			m_rcCurPos.right = m_rcCurPos.right +iStepLen;			
		}else{
			m_rcItem = m_rcCurPos = m_rcItemOld;
		}
	}else{
		iStepLen = ( m_rcItemOld.bottom - m_rcItemOld.top ) * m_nPositiveDirection / nTotalFrame;
		if( nCurFrame != nTotalFrame ){
			m_rcCurPos.top = m_rcCurPos.top + iStepLen;
			m_rcCurPos.bottom = m_rcCurPos.bottom +iStepLen;			
		}else{
			m_rcItem = m_rcCurPos = m_rcItemOld;	
		}	
	}

	DUITRACE(_T("Animation Pos : (%d, %d, %d, %d)"), m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
	DWORD dwTick = GetTickCount();
	SetPos(m_rcCurPos);
	dwTick = GetTickCount() - dwTick;
	DUITRACE(_T("Animation Tick : %d"), dwTick);
}

void CAnimationTabLayoutUI::OnAnimationStop(INT nAnimationID) 
{
	NeedParentUpdate();
}

//////////////////////////////////////////////////////////////////////////

void CAnimationTabLayoutUI::SetEllapse(int nVal){
	if (nVal > 0 && nVal < 1000){
		m_nAnimEllapse = nVal;
	}
}

void CAnimationTabLayoutUI::SetFrameSize(int nVal){
	if (nVal > 0 && nVal < 1000){
		m_nAnimFrame = nVal;
	}
}

int CAnimationTabLayoutUI::GetEllapse(void){
	return m_nAnimEllapse;
}

int CAnimationTabLayoutUI::GetFrameSize(void){
	return m_nAnimFrame;
}


void CAnimationTabLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("direction")) == 0 && _tcscmp( pstrValue, _T("vertical")) == 0 ) {
		m_bIsVerticalDirection = true; // pstrValue = "vertical" or "horizontal"
		return;
	}else if( _tcscmp(pstrName, _T("frame")) == 0  ){
		LPTSTR pstr = NULL;
		long lVal = _tcstoul(pstrValue, &pstr, 10);
		SetFrameSize(lVal);
		return;
	}else if( _tcscmp(pstrName, _T("ellapse")) == 0  ){
		LPTSTR pstr = NULL;
		long lVal = _tcstoul(pstrValue, &pstr, 10);
		SetEllapse(lVal);
		return;
	}

	return CTabLayoutUI::SetAttribute(pstrName, pstrValue);
}


}// 