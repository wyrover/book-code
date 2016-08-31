#ifndef __UIANIMATION_H__
#define __UIANIMATION_H__

#pragma once

//class vector;
#include <vector>

namespace DuiLib {

	class CUIAnimation;

	class CControlUI;

	class UILIB_API IUIAnimation
	{
	public:
		virtual ~IUIAnimation() { NULL; }

		virtual BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE) = 0;
		virtual VOID StopAnimation(int nAnimationID = 0) = 0;
		virtual BOOL IsAnimationRunning(int nAnimationID) = 0;
		virtual int GetCurrentFrame(int nAnimationID = 0) = 0;
		virtual BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0) = 0;

		virtual VOID OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) = 0;
		virtual VOID OnAnimationStart(int nAnimationID, BOOL bFirstLoop) = 0;
		virtual VOID OnAnimationStop(int nAnimationID) = 0;

		virtual VOID OnAnimationElapse(int nAnimationID) = 0;
	};

	struct UILIB_API AnimationData
	{
	public:
		AnimationData(){}
		AnimationData(int nElipse, int nFrame, int nID, BOOL bLoop)
		{
			m_bFirstLoop = TRUE;
			m_nCurFrame = 0;
			m_nElapse = nElipse;
			m_nTotalFrame = nFrame;
			m_bLoop = bLoop;
			m_nAnimationID = nID;
		}

	//protected:
	public:
		friend class CDUIAnimation;

		int m_nAnimationID;
		int m_nElapse;

		int m_nTotalFrame;
		int m_nCurFrame;

		BOOL m_bLoop;
		BOOL m_bFirstLoop;
	};

#pragma warning(push)
#pragma warning(disable: 4100)
	class UILIB_API CUIAnimation: public IUIAnimation
	{
	public:
		CUIAnimation(CControlUI* pOwner);

		~CUIAnimation();

		virtual BOOL StartAnimation(int nElapse, int nTotalFrame, int nAnimationID = 0, BOOL bLoop = FALSE);
		virtual VOID StopAnimation(int nAnimationID = 0);
		virtual BOOL IsAnimationRunning(int nAnimationID);
		virtual int GetCurrentFrame(int nAnimationID = 0);
		virtual BOOL SetCurrentFrame(int nFrame, int nAnimationID = 0);

		virtual VOID OnAnimationStart(int nAnimationID, BOOL bFirstLoop) {};
		virtual VOID OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) {};
		virtual VOID OnAnimationStop(int nAnimationID) {};

		virtual VOID OnAnimationElapse(int nAnimationID);

	protected:
		BOOL GetAnimationDataByID(int nAnimationID, AnimationData& data);
		void RemoveAni(int nAnimationID);
	protected:
		CControlUI* m_pControl;
		std::vector<AnimationData> m_arAnimations;
	};
#pragma warning(pop)

} // namespace DuiLib

#endif // __UIANIMATION_H__