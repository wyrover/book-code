#include "StdAfx.h"
#include <algorithm>
#include "UIAnimation.h"


namespace DuiLib {

	CUIAnimation::CUIAnimation(CControlUI* pOwner)
	{
		ASSERT(pOwner != NULL);
		m_pControl = pOwner;
	}

	CUIAnimation::~CUIAnimation()
	{
		m_pControl = NULL;
	}

	BOOL CUIAnimation::StartAnimation(int nElapse, int nTotalFrame, int nAnimationID /*= 0*/, BOOL bLoop/* = FALSE*/)
	{
		AnimationData pData;
		if(GetAnimationDataByID(nAnimationID, pData)){
			return FALSE;
		}

		AnimationData aniData(nElapse, nTotalFrame, nAnimationID, bLoop);

		if(m_pControl->GetManager()->SetTimer( m_pControl, nAnimationID, nElapse )){
			m_arAnimations.push_back(aniData);
			return TRUE;
		}
		return FALSE;
	}

	VOID CUIAnimation::StopAnimation(int nAnimationID /*= 0*/)
	{
		if(nAnimationID != 0){
			m_pControl->GetManager()->KillTimer( m_pControl, nAnimationID );
			RemoveAni(nAnimationID);
		}
	}

	BOOL CUIAnimation::IsAnimationRunning(int nAnimationID)
	{
		AnimationData data;
		return GetAnimationDataByID(nAnimationID, data);
	}

	int CUIAnimation::GetCurrentFrame(int nAnimationID/* = 0*/)
	{
		AnimationData data;
		
		if( GetAnimationDataByID(nAnimationID, data) ){
			ASSERT(FALSE);
			return data.m_nCurFrame;
		}
		return 0;
	}

	BOOL CUIAnimation::SetCurrentFrame(int nFrame, int nAnimationID/* = 0*/)
	{
		if(nFrame >= 0){
			std::vector<AnimationData>::iterator itr;
			for (itr = m_arAnimations.begin(); itr != m_arAnimations.end(); ++itr){
				AnimationData& data = (*itr);
				if (data.m_nAnimationID == nAnimationID){
					if (nFrame <= data.m_nTotalFrame){
					 	data.m_nCurFrame = nFrame;
					}
					break;
				}
			}
			return TRUE;
		}
		return FALSE;
	}

	VOID CUIAnimation::OnAnimationElapse(int nAnimationID)
	{
		if(m_pControl == NULL) return;

		AnimationData data;
		if( !GetAnimationDataByID(nAnimationID, data) ){
			return;
		}

		int nCurFrame = data.m_nCurFrame;
		if(nCurFrame == 0)
		{
			OnAnimationStart(nAnimationID, data.m_bFirstLoop);
			data.m_bFirstLoop = FALSE;
		}

		OnAnimationStep(data.m_nTotalFrame, nCurFrame, nAnimationID);

		if(nCurFrame >= data.m_nTotalFrame){
			OnAnimationStop(nAnimationID);
			if(data.m_bLoop){
				data.m_nCurFrame = 0;
			}else{
				StopAnimation(nAnimationID);
			}
		}else{
			SetCurrentFrame(++nCurFrame, nAnimationID);
		}
	}

	BOOL CUIAnimation::GetAnimationDataByID(int nAnimationID, AnimationData& data)
	{
		int nCount = m_arAnimations.size();
		for(int i=0; i<nCount; ++i)
		{
			if((m_arAnimations)[i].m_nAnimationID == nAnimationID)
			{
				data = m_arAnimations[i];
				return true;
			}
		}

		return false;
	}


	void CUIAnimation::RemoveAni(int nAnimationID)
	{
		std::vector<AnimationData>::iterator itr;
		for (itr = m_arAnimations.begin(); itr != m_arAnimations.end(); ++itr){
			AnimationData& data = (*itr);
			if (data.m_nAnimationID == nAnimationID){
				m_arAnimations.erase(itr);
				break;
			}
		}
	}
} // namespace DuiLib