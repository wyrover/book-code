#include "StdAfx.h"
#include "UIGifAnim.h"

namespace DuiLib
{
	//write by wangji
	struct CGifAnimUI::Imp
	{
		bool				m_bRealStop			;//外部停止了
		bool				m_bLoadImg			;//是否加载过图片
		bool				m_bTimer			;//是否启动定时器
		bool				m_bAutoStart		;//是否自动开始
		int					m_nDelay			;//循环毫秒数
		UINT				m_nFrameCount		;//gif图片总帧数
		UINT				m_nFramePosition	;//当前放到第几帧
		Image*				m_pGifImage			;//gif对象
		PropertyItem*		m_pPropertyItem		;//帧与帧之间间隔时间
		CPaintManagerUI*&	m_pManager			;
		CGifAnimUI*			m_pOwer				;//拥有者
		Imp(CPaintManagerUI* & pManager):m_pManager(pManager),
		m_bLoadImg(false),m_bTimer(false),
		m_nDelay(100),m_pGifImage(NULL),m_nFrameCount(0U),
		m_pPropertyItem(NULL),m_nFramePosition(0U),
		m_pOwer(NULL),m_bRealStop(false),m_bAutoStart(true)
		{
		}
		void SetOwer(CGifAnimUI *pOwer)
		{
			m_pOwer = pOwer;
		}
		~Imp()
		{
			if ( m_pGifImage != NULL )
			{
				delete m_pGifImage;
				m_pGifImage = NULL;
			}
			if ( m_pPropertyItem != NULL )
			{
				free( m_pPropertyItem );
				m_pPropertyItem = NULL;
			}
		}
		inline void CheckLoadImage()
		{
			if(!m_bLoadImg)
				LoadGifImage();
		}
		inline bool IsLoadImage(){return m_bLoadImg;}
		virtual void LoadGifImage()
		{
			CDuiString sImag = m_pOwer->GetBkImage();
			m_bLoadImg = true;
			m_pGifImage	=	CRenderEngine::LoadGifImage(STRINGorID(sImag),0, 0);
			if ( NULL == m_pGifImage ) return;
			const UINT nCount	=	m_pGifImage->GetFrameDimensionsCount();
			ASSERT(nCount > 0);
			if(0 == nCount)return;
			GUID* pDimensionIDs	=	new GUID[ nCount ];
			m_pGifImage->GetFrameDimensionsList( pDimensionIDs, nCount );
			m_nFrameCount	=	m_pGifImage->GetFrameCount( &pDimensionIDs[0] );
			const int nSize		=	m_pGifImage->GetPropertyItemSize( PropertyTagFrameDelay );
			if(0 == nSize)
			{
				delete  []pDimensionIDs;
				return;
			}
			m_pPropertyItem	= (PropertyItem*) malloc( nSize );
			m_pGifImage->GetPropertyItem( PropertyTagFrameDelay, nSize, m_pPropertyItem );
			delete  []pDimensionIDs;
			pDimensionIDs = NULL;

			m_nDelay = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
			if (m_nDelay <= 0 ) 
				m_nDelay = 100;
			if(!m_bAutoStart)
				m_bRealStop = true;
			if(m_bAutoStart && m_pOwer->IsVisible())
				StartAnim();
		}
		void SetAutoStart(bool bAuto)
		{
			m_bAutoStart = bAuto;
		}
		void StartAnim()
		{
			if(!m_bTimer)
			{
				if(!IsLoadImage())
					LoadGifImage();
				if(m_pGifImage)
					m_pManager->SetTimer( m_pOwer, EVENT_TIEM_ID, m_nDelay );
				m_bTimer = true;
			}
		}
		void StopAnim(bool bGoFirstFrame)//bGoFirstFrame 是否跑到第一帧
		{
			if(m_bTimer)
			{
				if(bGoFirstFrame)
				{
					m_nFramePosition = 0U;
					m_pOwer->Invalidate();
				}
				m_pManager->KillTimer( m_pOwer, EVENT_TIEM_ID );
				m_bTimer = false;
			}
		}
		void EventOnTimer(const WPARAM idEvent )
		{
			if ( idEvent != EVENT_TIEM_ID )
				return;
			++m_nFramePosition;
			if(m_nFramePosition >= m_nFrameCount)
				m_nFramePosition = 0;
			if(!m_pOwer->IsVisible())return;
			m_pOwer->Invalidate();
		}
		void DrawFrame( HDC hDC,const RECT& rcPaint,const RECT &rcItem)
		{
			if ( NULL == hDC || NULL == m_pGifImage ) return;
			GUID pageGuid = Gdiplus::FrameDimensionTime;
			Graphics graphics( hDC );
			graphics.DrawImage( m_pGifImage, m_pOwer->m_rcItem.left, m_pOwer->m_rcItem.top );
			m_pGifImage->SelectActiveFrame( &pageGuid, m_nFramePosition );
		}
		void EventSetVisible(bool bVisible)
		{
			if(bVisible)
			{
				if(!m_bRealStop)
					StartAnim();
			}
			else
				StopAnim(true);
		}
	};
	CGifAnimUI::CGifAnimUI(void):m_pImp(new CGifAnimUI::Imp(m_pManager))
	{
		this;
		m_pImp->SetOwer(this);
	}
	CGifAnimUI::~CGifAnimUI(void)
	{
		m_pImp->StopAnim(false);
		delete m_pImp;
		m_pImp = NULL;
	}

	//************************************
	// Method:    GetClass
	// FullName:  CGifAnimUI::GetClass
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier: const
	// Note:	  
	//************************************
	LPCTSTR CGifAnimUI::GetClass() const
	{
		return _T("GifAnimUI");
	}

	//************************************
	// Method:    GetInterface
	// FullName:  CGifAnimUI::GetInterface
	// Access:    public 
	// Returns:   LPVOID
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Note:	  
	//************************************
	LPVOID CGifAnimUI::GetInterface( LPCTSTR pstrName )
	{
			if( _tcscmp(pstrName, _T("GifAnim")) == 0 ) 
				return static_cast<CGifAnimUI*>(this);
			return CLabelUI::GetInterface(pstrName);
	}
	void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("auto")) == 0 ) 
			m_pImp->SetAutoStart(_tcscmp(pstrValue, _T("true")) == 0);
		else
			__super::SetAttribute(pstrName, pstrValue);
	}
	void CGifAnimUI::Init()
	{
		__super::Init();
		m_pImp->CheckLoadImage();
	}
	void CGifAnimUI::SetVisible(bool bVisible /*= true*/)
	{
		__super::SetVisible(bVisible);
		m_pImp->EventSetVisible(bVisible);
	}
	void CGifAnimUI::SetInternVisible(bool bVisible/* = true*/)
	{
		__super::SetInternVisible(bVisible);
		m_pImp->EventSetVisible(bVisible);
	}
	//************************************
	// Method:    DoPaint
	// FullName:  CGifAnimUI::DoPaint
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: HDC hDC
	// Parameter: const RECT & rcPaint
	// Note:	  
	//************************************
	void CGifAnimUI::DoPaint( HDC hDC, const RECT& rcPaint )
	{
		if( !::IntersectRect( &m_rcPaint, &rcPaint, &m_rcItem ) ) return;
		m_pImp->DrawFrame( hDC,rcPaint,m_rcItem);
	}

	//************************************
	// Method:    DoEvent
	// FullName:  CGifAnimUI::DoEvent
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: TEventUI & event
	// Note:	  
	//************************************
	void CGifAnimUI::DoEvent( TEventUI& event )
	{
		this;
		WPARAM nID = event.wParam;
		if( event.Type == UIEVENT_TIMER )
		{
			m_pImp->EventOnTimer(nID);
			return;
		}
			

		__super::DoEvent(event);
	}
	void CGifAnimUI::StartAnim()
	{
		m_pImp->m_bRealStop = false;
		m_pImp->StartAnim();
	}
	void CGifAnimUI::StopAnim()
	{
		m_pImp->m_bRealStop = true;
		m_pImp->StopAnim(true);
	}
}
