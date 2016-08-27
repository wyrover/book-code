#ifndef __UIANIMTABLAYOUT_H__
#define __UIANIMTABLAYOUT_H__

#pragma once

#include "../Control/UIAnimation.h"

namespace DuiLib
{

class UILIB_API CAnimationTabLayoutUI : public CTabLayoutUI, public CUIAnimation
{
public:
	CAnimationTabLayoutUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual bool SelectItem( int iIndex );
	void AnimationSwitch();
	void DoEvent(TEventUI& event);
	void OnTimer( int nTimerID );

	virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop);
	virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
	virtual void OnAnimationStop(INT nAnimationID);

	void SetEllapse(int);
	void SetFrameSize(int);

	int GetEllapse(void);
	int GetFrameSize(void);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	bool m_bIsVerticalDirection;
	int m_nPositiveDirection;
	RECT m_rcCurPos;
	RECT m_rcItemOld;
	int m_nAnimId;
	int m_nAnimEllapse;
	int m_nAnimFrame;
	enum
	{
		TAB_ANIMATION_ID = 1,

		TAB_ANIMATION_ELLAPSE = 15,
		TAB_ANIMATION_FRAME_COUNT = 15,
	};
};

} //namespace DuiLib


#endif //__UIANIMTABLAYOUT_H__