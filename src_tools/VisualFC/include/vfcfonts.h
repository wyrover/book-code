// Windows Template Library - WTL version 8.0
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// This file is a part of the Windows Template Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/osi3.0/licenses/cpl1.0.php)
// which can be found in the file CPL.TXT at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or
// any other, from this software.

// Module: vfcfonts.h
// Creator: visualfc 
// Email: visualfc@gmail.com
// open source: http://sf.net/projects/visualfc
// Date: 2007-12-06

#ifndef _VFCFONTS_H__
#define _VFCFONTS_H__

namespace WTL
{
/*
typedef struct tagLOGFONTA/W
{
    LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    CHAR/WCAHR     lfFaceName[LF_FACESIZE];
} 
*/

#define BEGIN_DLGFONT_MAP(thisClass) \
	static const CDialogFontBase::_AtlDlgFontMap* GetDlgFontMap() \
	{ \
		static const _AtlDlgFontMap theMap[] = \
		{
#define END_DLGFONT_MAP() \
			{ -1, {0} }, \
		}; \
		return theMap; \
	}

#define DLGFONT_CONTROL(id, lfHeight,lfFaceName) \
		{ id, {lfHeight,0,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,lfFaceName} },

#define DLGFONT_CONTROL_EX(id,he,wi,es,or,we,it,un,st,ch,ou,cl,qu,pi,fa) \
		{ id, {he,wi,es,or,we,it,un,st,ch,ou,cl,qu,pi,fa} },

#define DLGFONT_DIALOG(lfHeight,lfFaceName) \
		{ -2, {lfHeight,0,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,lfFaceName} },

#define DLGFONT_DIALOG_EX(he,wi,es,or,we,it,un,st,ch,ou,cl,qu,pi,fa) \
		{ -2, {he,wi,es,or,we,it,un,st,ch,ou,cl,qu,pi,fa} },

class CDialogFontBase
{
public:
	CDialogFontBase()
	{
	}
	~CDialogFontBase()
	{
		for (int i = 0; i < m_arrData.GetSize(); i++)
		{
			m_arrData[i].m_font.DeleteObject();
		}
	}
	struct _AtlDlgFontMap
	{
		int		  m_nCtlID;
		LOGFONT   m_logFont;
	};		
	struct _AtlDlgResizeData
	{
		int m_nCtlID;
		CFontHandle m_font;
	};
	CSimpleArray<_AtlDlgResizeData>	m_arrData;
	
	void AddDlgFontMap(const _AtlDlgFontMap * pMap)
	{
		for(int nCount = 1; !(pMap->m_nCtlID == -1); nCount++, pMap++)
		{
			_AtlDlgResizeData data;
			data.m_nCtlID = pMap->m_nCtlID;
			data.m_font.CreateFontIndirect(&pMap->m_logFont);
			m_arrData.Add(data);
		}	
	}

	void SetDlgFont(HWND hDlg)
	{
		int i = 0;
		for (i = 0; i < m_arrData.GetSize(); i++)
		{
			if (m_arrData[i].m_nCtlID == -2)
			{
				CWindow(hDlg).SetFont(m_arrData[i].m_font.m_hFont);
				::EnumChildWindows(hDlg,&CDialogFontBase::EnumChildProc,(LPARAM)m_arrData[i].m_font.m_hFont);
			}
		}
		for (i = 0; i < m_arrData.GetSize(); i++)
		{
			if (m_arrData[i].m_nCtlID == -2)
			{
				continue;
			}
			CWindow(::GetDlgItem(hDlg,m_arrData[i].m_nCtlID)).SetFont(m_arrData[i].m_font,FALSE);
		}
	}
	
	static BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
	{
		HFONT hFont = (HFONT)lParam;
		if (hwnd)
		{
			CWindow(hwnd).SetFont(hFont,FALSE);
		}
		return TRUE;
	}
};

template <class T>
class CDialogFont : public CDialogFontBase
{
public:
	void DlgFont_Init()
	{
		T * pT = static_cast<T*>(this);
		const _AtlDlgFontMap* pMap = pT->GetDlgFontMap();
		AddDlgFontMap(pMap);
		SetDlgFont(pT->m_hWnd);
	}
};

} //namespace WTL

namespace winx
{

#define WINX_DLGFONT_BEGIN()\
	struct _winxDialogFont : public WTL::CDialogFontBase\
{\
	BEGIN_DLGFONT_MAP(0)

#define WINX_DLGFONT DLGFONT_CONTROL
#define WINX_DLGFONT_EX DLGFONT_CONTROL_EX

#define WINX_DLGFONT_DIALOG	DLGFONT_DIALOG
#define WINX_DLGFONT_DIALOG_EX	DLGFONT_DIALOG_EX

#define WINX_DLGFONT_END()\
	END_DLGFONT_MAP()\
};\
_winxDialogFont _winx_dlgfont;\
void DlgFont_Init()\
{\
	_winx_dlgfont.AddDlgFontMap(_winxDialogFont::GetDlgFontMap());\
	_winx_dlgfont.SetDlgFont(m_hWnd);\
}

} //namesapce winx

#endif //_VFCFONTS_H__