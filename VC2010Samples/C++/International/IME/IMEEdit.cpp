/******************************************************************************\
*	IMEEdit.cpp: 实现文件
*
*	这是 Microsoft 源代码示例的一部分。
*	版权所有(C) Microsoft Corporation。
*	保留所有权利。
*
*	此源代码仅用作 Microsoft 
*	开发工具和/或 WinHelp 文档的补充。
*	有关 Microsoft 示例程序的详细信息，
*	请参见这些来源。
\******************************************************************************/

#include "stdafx.h"
#include "imm.h"
#include "IMEEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 声明枚举类型语言
enum LANGFLAG           
{
	DEFAULT,				
	TRADITIONAL_CHINESE,	
	JAPANESE,
	KOREAN,
	SIMPLIFIED_CHINESE
} LangFlag;  

// 定义代码页 
const int nCodePage[5] = {
	0,		// 默认值 
	950,		// 繁体中文
	932,		// 日语
	949,		// 朝鲜语 
	936			// 简体中文
};

// 定义字符集
const BYTE nCharSet[5] = {
	DEFAULT_CHARSET,		// 默认值 
	CHINESEBIG5_CHARSET,	// 繁体中文
	SHIFTJIS_CHARSET,		// 日语
	HANGUL_CHARSET,			// 朝鲜语
	GB2312_CHARSET			// 简体中文
};

// 定义默认字体
const char szDefaultFontName[5][19] = {
	"\x54\x00\x61\x00\x68\x00\x6F\x00\x6D\x00\x61\x00\x00\x00",					// 默认字体
	"\xB0\x65\x30\x7D\x0E\x66\xD4\x9A\x00\x00",									// 繁体中文字体
	"\x2D\xFF\x33\xFF\x20\x00\x30\xFF\xB4\x30\xB7\x30\xC3\x30\xAF\x30\x00\x00",	// 日语字体
	"\x74\xAD\xBC\xB9\x00\x00",													// 朝鲜语字体
	"\xB0\x65\x8B\x5B\x53\x4F\x00\x00"											// 简体中文字体
};

const TCHAR szEnglishFontName[5][11] = {
	_T("Tahoma"),		_T("MingLiU"),		_T("MS PGothic"), 
	_T("Gulim"),		_T("NSimSun")};

/////////////////////////////////////////////////////////////////////////////
// CIMEEdit

CIMEEdit::CIMEEdit()
{
	OSVERSIONINFO osVI;

	// 初始化缓冲区位置
	m_xEndPos = m_xInsertPos = m_xCaretPos = 0;

	// 初始化缓冲区
	memset(m_szBuffer, 0, WCHARSIZE * (BUFFERSIZE + 1));

	// 初始化成员变量
	m_fStat = FALSE;				// 不显示虚下划线
	m_fShowInvert = FALSE;			// 不转换当前撰写字符串
	m_nComSize = 0;					// 清除以字节为单位的撰写字符串大小的变量
	m_nCharSet =  DEFAULT_CHARSET;		
	m_hFont = NULL;		
	m_fIsNT = TRUE;
	m_fIsXP = TRUE;
}

CIMEEdit::~CIMEEdit()
{
	if (m_hFont)
		DeleteObject(m_hFont);
}

BEGIN_MESSAGE_MAP(CIMEEdit, CEdit)
	//{{AFX_MSG_MAP(CIMEEdit)
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMEEdit 消息处理程序

void CIMEEdit::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	TEXTMETRIC	tm;
	int			nXStart, nXEnd;
	CRect		cRect;
	HFONT		pOldFont = (HFONT)SelectObject(dc, m_hFont);
	CPen		*pPen, *pOldPen;
	wchar_t		szTempStr[BUFFERSIZE+1];
	RECT		rcBounds;

	HideCaret();

	// 用白色填充背景
	GetClientRect(&rcBounds);
	dc.FillRect(&rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	// 显示缓冲区中的文本
	TextOutW(dc, X_INIT, Y_INIT, m_szBuffer, (int)wcslen(m_szBuffer));

	dc.GetTextMetrics(&tm);
	// 在撰写字符串下绘制虚线
	if ((m_fStat) && (m_dwProperty & IME_PROP_AT_CARET) && (m_nLanguage != KOREAN))
	{
		// 计算撰写字符串的起始位置并将钢笔移动到起始位置
		memcpy(szTempStr, m_szBuffer, WCHARSIZE * m_xInsertPos);	 
		szTempStr[m_xInsertPos] = L'\0';
		nXStart = X_INIT + GetWidthOfString(szTempStr);
		dc.MoveTo(nXStart, Y_INIT + tm.tmHeight);

		// 计算撰写字符串的结尾位置并绘制虚线
		m_szComStr[m_nComSize / WCHARSIZE] = L'\0';

		if (m_nLanguage == JAPANESE)
		{
			for (int i = 0; ; i++)
			{
				if (m_dwCompCls[i] == m_nComAttrSize)
					break;
				memcpy(szTempStr, &m_szComStr[m_dwCompCls[i]], WCHARSIZE * (m_dwCompCls[i+1] - m_dwCompCls[i]));	 
				szTempStr[m_dwCompCls[i+1] - m_dwCompCls[i]] = L'\0';
				nXEnd = nXStart + GetWidthOfString(szTempStr);
				
				if (m_bComAttr[m_dwCompCls[i]] == 0x00)
					pPen = new CPen(PS_DOT, 1, RGB(0, 0, 0));	// 针对输入字符
				else if (m_bComAttr[m_dwCompCls[i]] == 0x01)
					pPen = new CPen(PS_SOLID, 2, RGB(0, 0, 0));	// 针对可编辑词组
				else 
					pPen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));	// 针对转换的字符

				pOldPen = dc.SelectObject(pPen);
				dc.LineTo(nXEnd - 1, Y_INIT + tm.tmHeight);
				dc.MoveTo(nXEnd + 1, Y_INIT + tm.tmHeight);
				dc.SelectObject(pOldPen);
				delete pPen;
				nXStart = nXEnd;
			}
		}
		else
		{
			nXEnd = nXStart + GetWidthOfString(m_szComStr);
			pPen = new CPen(PS_DOT, 1, RGB(0, 0, 0));
			pOldPen = dc.SelectObject(pPen);
			dc.LineTo(nXEnd, Y_INIT + tm.tmHeight);
			dc.SelectObject(pOldPen);
			delete pPen;
		}
	}

	// 反转当前撰写字符串(繁体中文与简体中文)
	if (m_fShowInvert)
	{
		// 计算反转字符串的起始位置
		errno_t err;
		err = memcpy_s(szTempStr, sizeof(szTempStr) - WCHARSIZE, m_szBuffer, WCHARSIZE * m_nInvertStart);
		ASSERT(!err);
		szTempStr[m_nInvertStart] = L'\0';
		cRect.top = Y_INIT;
		cRect.left = X_INIT + GetWidthOfString(szTempStr);

		// 计算反转字符串的结尾位置
		cRect.bottom = cRect.top + tm.tmHeight;
		err = memcpy_s(szTempStr, sizeof(szTempStr) - WCHARSIZE, &m_szBuffer[m_nInvertStart], WCHARSIZE * (m_nInvertEnd - m_nInvertStart));	 
		ASSERT(!err);
		szTempStr[m_nInvertEnd - m_nInvertStart] = L'\0';
		cRect.right = cRect.left + GetWidthOfString(szTempStr);

		dc.InvertRect(cRect);  
	}

	ShowCaret();
	SelectObject(dc, pOldFont);
}

void CIMEEdit::OnSetFocus(CWnd*) 
{
	CreateSolidCaret(1,FONT_WIDTH);		

	// 设置插入符号位置
	ShowCaretOnView();	

	ShowCaret();

}

void CIMEEdit::OnKillFocus(CWnd*) 
{
	HIMC	hIMC;
	
	HideCaret();

	// 删除幻像插入符号
	Invalidate();

	// 如果撰写未结束，则完成撰写字符串
	if ((m_fStat) && (m_nLanguage != KOREAN))
	{
		hIMC = ImmGetContext(this->m_hWnd);
		if (hIMC) 
		{
			ImmNotifyIME(hIMC, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
			ImmReleaseContext(this->m_hWnd,hIMC);
		}
	}	
}

/*****************************************************************************\
* 函数: ShowCaretOnView
*
* 计算插入符号位置并重新显示插入符号
\*****************************************************************************/
void CIMEEdit::ShowCaretOnView()
{
	wchar_t *szTmpStr = new(wchar_t[m_xCaretPos + 1]);
	errno_t err;

	HideCaret();

	// 计算插入符号的位置并设置插入符号位置
	err = memcpy_s(szTmpStr, WCHARSIZE * m_xCaretPos, m_szBuffer, WCHARSIZE * (m_xCaretPos));
	ASSERT(!err);
	szTmpStr[m_xCaretPos] = L'\0';
	SetCaretPos(CPoint(X_INIT + GetWidthOfString(szTmpStr), Y_INIT));
	ShowCaret();
	
	// 如果 IME 属性没有 IME_PROP_AT_CARET，则更改撰写窗口
	// 的位置
	if (!(m_dwProperty & IME_PROP_AT_CARET) )
		SetCompositionWindowPos();

	delete [] szTmpStr;
}

/*****************************************************************************\
* 函数: GetWidthOfString
*
* 计算字符串宽度(以像素为单位) 
*
* 参数: 
*	LPCTSTR szStr - 字符串 
\*****************************************************************************/
int CIMEEdit::GetWidthOfString(wchar_t *szStr)
{
	SIZE		Size;
	CClientDC	dc(this);
	HFONT		pOld = (HFONT)SelectObject(dc, m_hFont);

	GetTextExtentPoint32W(dc, szStr, (int)wcslen(szStr), &Size);

	SelectObject(dc, pOld);

	return Size.cx;
}

/*****************************************************************************\
* 函数: OnImeStartComposition
*
* 接收 WM_IME_STARTCOMPOSITION 消息并开始撰写
*
* 参数: 
*	WPARAM wParam - 未使用
*	LPARAM lParam - 未使用
\*****************************************************************************/
void CIMEEdit::OnImeStartComposition(WPARAM, LPARAM)
{
	// 检查缓冲区。如果已满，则清除缓冲区 
	if (m_xEndPos >= (BUFFERSIZE-1))	// 检查缓冲区 
		ClearBuffer();

	// 如果为朝鲜语字符集，则更改插入符号 
	if (m_nLanguage == KOREAN)		
	{
		CreateSolidCaret(FONT_WIDTH, FONT_HEIGHT);	// 创建宽插入符号
		ShowCaret();
	}

	// 备份字符串缓冲区 
	memcpy(m_szBackup, m_szBuffer, WCHARSIZE * (BUFFERSIZE+1));
}

/*****************************************************************************\
* 函数: OnImeStartComposition
*
* 接收 WM_IME_COMPOSITION 消息和撰写字符串
*
* 参数: 
*	WPARAM wParam - 表示对撰写字符串的最新更改的
*					 DBCS 字符
*	LPARAM lParam - 更改标志
\*****************************************************************************/
BOOL CIMEEdit::OnImeComposition(WPARAM, LPARAM lParam)
{
	HIMC	hIMC;
	int		i, nTmpInc;

	hIMC = ImmGetContext(this->m_hWnd);
	if (hIMC == NULL) 
		return TRUE;

	// 还原字符串缓冲区
	if (m_fStat)
		memcpy(m_szBuffer, m_szBackup, WCHARSIZE * (BUFFERSIZE+1));

	if (lParam & GCS_RESULTSTR)
	{
		m_fShowInvert = FALSE;

		// 获取结果字符串
		m_nComSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, (LPVOID)m_szComStr, WCHARSIZE * (BUFFERSIZE+1));

		if (m_nComSize > 0)
		{
			// 将撰写字符串插入到字符串缓冲区中
			nTmpInc = InsertCompStr();

			// 移动插入/插入符号位置并增加结尾位置
			memcpy(m_szBackup, m_szBuffer, WCHARSIZE * (BUFFERSIZE+1));
			m_xEndPos += nTmpInc;
			m_xInsertPos += nTmpInc;
			m_xCaretPos = m_xInsertPos;
		}
	}
	else if (lParam & GCS_COMPSTR)
	{
		// 如果 IME 属性没有 IME_PROP_AT_CARET，则忽略级别 3 功能
		if (!(m_dwProperty & IME_PROP_AT_CARET) )
			return FALSE;

		// 获取撰写字符串
		m_nComSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, (LPVOID)m_szComStr, WCHARSIZE * (BUFFERSIZE+1));
		if (m_nLanguage != KOREAN)
		{
			// 获取撰写特性和撰写字符串中的光标位置
			if (lParam & GCS_COMPATTR)
				m_nComAttrSize = ImmGetCompositionStringW(hIMC, GCS_COMPATTR, m_bComAttr, sizeof(m_bComAttr));

			if (lParam & GCS_CURSORPOS)
				m_nComCursorPos = ImmGetCompositionStringW(hIMC, GCS_CURSORPOS, NULL, 0);

			if (lParam & GCS_COMPCLAUSE)
				ImmGetCompositionStringW(hIMC, GCS_COMPCLAUSE, m_dwCompCls,sizeof(m_dwCompCls));
			
			if ((m_nLanguage == TRADITIONAL_CHINESE) || (m_nLanguage == SIMPLIFIED_CHINESE && !m_fIsXP))
			{
				m_fShowInvert = TRUE;
				nTmpInc = 0;
				for (i = m_nComCursorPos; i < (int)m_nComAttrSize; i++)
				{
					if (m_bComAttr[i] == 1)
						nTmpInc++;
					else
						break;
				}
				m_nInvertStart = m_xInsertPos + m_nComCursorPos;
				m_nInvertEnd = m_nInvertStart + nTmpInc;
			}
 		}
		if (m_nComSize > 0)
		{
			// 将撰写字符串插入到字符串缓冲区中
			nTmpInc = InsertCompStr();
		
			// 移动插入符号位置
			if (m_nLanguage != KOREAN)
			{
				if (m_nLanguage == JAPANESE)
					m_xCaretPos = m_xInsertPos + m_nComSize / WCHARSIZE;
				else
					m_xCaretPos = m_xInsertPos + m_nComCursorPos;
			}
		}
	}
	else
	{
		// 无撰写字符串
		m_nComSize = 0;
	}

	ImmReleaseContext(this->m_hWnd, hIMC);

	ShowCaretOnView();
	Invalidate();	
	return TRUE;
}

/*****************************************************************************\
* 函数: OnImeEndComposition
*
* 接收 WM_IME_ENDCOMPOSITION 消息并结束撰写
*
* 参数: 
*	WPARAM wParam - 未使用 
*	LPARAM lParam - 未使用
\*****************************************************************************/
void CIMEEdit::OnImeEndComposition(WPARAM, LPARAM)
{
	// 关闭反转标志
	m_fShowInvert = FALSE;

	// 如果为朝鲜语字符集，则将插入符号更改为常规样式
	if (m_nLanguage == KOREAN)		
		CreateSolidCaret(1,FONT_WIDTH);		

	ShowCaret();
}

/*****************************************************************************\
* 函数: InsertCompStr
*
* 将撰写字符串插入到字符串缓冲区中
*
* 参数: 
*	WPARAM wParam - 未使用 
*	LPARAM lParam - 未使用
*
* 返回值: 
*	TCHAR 中撰写字符串的大小
\*****************************************************************************/
int CIMEEdit::InsertCompStr()
{
	HIMC	hIMC;
	int		nTmpPos, nTmpInc;
	wchar_t	szTmpStr[BUFFERSIZE+1];
	errno_t err;

	// 备份插入位置之后的子字符串 
	nTmpPos = m_xEndPos - m_xInsertPos;
	err = memcpy_s(szTmpStr, sizeof(szTmpStr) - WCHARSIZE, &m_szBuffer[m_xInsertPos], WCHARSIZE * nTmpPos);
	ASSERT(!err);
	szTmpStr[nTmpPos] = L'\0';

	// 检查缓冲区溢出
	if ((m_nComSize % 2) != 0)
		m_nComSize++;
	nTmpInc = m_nComSize / WCHARSIZE;

	if ((m_xEndPos + nTmpInc) < 0)
		return 0;

	if ((m_xEndPos + nTmpInc) >= BUFFERSIZE)
	{
		if (m_xInsertPos > 0)
		{
			// 删除插入位置之前的子字符串
			memcpy(m_szBackup, szTmpStr, WCHARSIZE * (BUFFERSIZE+1));
			ClearBuffer();
			m_xEndPos = nTmpPos;
		}
		else
		{
			// 取消撰写字符串 
			hIMC = ImmGetContext(this->m_hWnd);
			if (hIMC) 
			{
				ImmNotifyIME(hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
				ImmReleaseContext(this->m_hWnd,hIMC);
				ClearBuffer();
			}
		}
	}

	// 将撰写字符串复制到字符串缓冲区
	err = memcpy_s(&m_szBuffer[m_xInsertPos], sizeof(m_szBuffer) - (m_xInsertPos+1)*WCHARSIZE, m_szComStr, m_nComSize);
	ASSERT(!err);

	// 还原插入位置之后的子字符串
	err = memcpy_s(&m_szBuffer[m_xInsertPos+nTmpInc], sizeof(m_szBuffer) - (m_xInsertPos+nTmpInc+1)*WCHARSIZE, szTmpStr, WCHARSIZE * nTmpPos);
	ASSERT(!err);
	m_szBuffer[m_xEndPos + nTmpInc] = L'\0';

	return nTmpInc;
}

void CIMEEdit::OnChar(UINT nChar, UINT, UINT) 
{
	int		nTmpDec;
	wchar_t wChar, szTmpStr[BUFFERSIZE+1];
	errno_t err;

	// 检查当前状态
	if (m_fStat)
		return;

	switch(nChar)
	{
		// 返回键
		case _T('\r') :	
				// 删除插入位置之前的子字符串
				err = memcpy_s(m_szBuffer, sizeof(m_szBuffer) - WCHARSIZE, &m_szBuffer[m_xInsertPos], WCHARSIZE * (m_xEndPos - m_xInsertPos));
				ASSERT(!err);
				m_xEndPos = m_xEndPos - m_xInsertPos;
				m_xInsertPos = m_xCaretPos = 0;
				m_szBuffer[m_xEndPos] = L'\0';
				break;

		// Back Space 键
		case _T('\b') :		
				// 删除一个字符
				if(m_xInsertPos == 0) break;
				nTmpDec = GetCombinedCharLength(m_xInsertPos-1);
				err = memcpy_s(&m_szBuffer[m_xInsertPos-nTmpDec], sizeof(m_szBuffer) -(m_xInsertPos-nTmpDec+1)*WCHARSIZE, &m_szBuffer[m_xInsertPos], WCHARSIZE * (m_xEndPos - m_xInsertPos));
				ASSERT(!err);
				m_xEndPos -= nTmpDec;
				m_xInsertPos -= nTmpDec;
				m_xCaretPos -= nTmpDec;
				m_szBuffer[m_xEndPos] = L'\0';
				break;

		default :

				if (nChar < TCHAR(0x20))
					break;
#ifdef _UNICODE
				wChar = nChar;
#else
				MultiByteToWideChar(m_nCodePage, 0, (char *)&nChar, 1, &wChar, 2);
#endif
				// 检查缓冲区溢出
				if(m_xEndPos > (BUFFERSIZE-1) )	
					ClearBuffer();

				// 将字符插入到字符串缓冲区
				err = memcpy_s(szTmpStr, sizeof(szTmpStr) - WCHARSIZE, &m_szBuffer[m_xInsertPos], WCHARSIZE * (m_xEndPos - m_xInsertPos));
				ASSERT(!err);
				m_szBuffer[m_xInsertPos] = wChar;
				err = memcpy_s(&m_szBuffer[m_xInsertPos+1], sizeof(m_szBuffer) - (m_xInsertPos+1+1)*WCHARSIZE,  szTmpStr, WCHARSIZE * (m_xEndPos - m_xInsertPos));
				ASSERT(!err);
				m_xEndPos++;
				m_xInsertPos++;
				m_xCaretPos++;
				//上面的 if(m_xEndPos > (BUFFERSIZE-1) ) 确保在下面的行中不会发生缓冲区溢出
				#pragma warning (suppress: 6386) 
				m_szBuffer[m_xEndPos] = L'\0';
				break;
	}

	ShowCaretOnView();
	Invalidate();

}

void CIMEEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int		nTmpDec;
	errno_t err;

	// 如果 IME 属性没有 IME_PROP_AT_CARET，则在撰写过程中不接受字符
	if (!(m_dwProperty & IME_PROP_AT_CARET) && m_fStat)
		return;

	switch(nChar)
	{
		// 向左键
		case VK_LEFT:
			if(m_xInsertPos == 0) 
				break;
			m_xInsertPos -= GetCombinedCharLength(m_xInsertPos-1);
			m_xCaretPos = m_xInsertPos;
			ShowCaretOnView();
			break;

		// 向右键
		case VK_RIGHT:
			if(m_xInsertPos == m_xEndPos) break;
			m_xInsertPos +=  GetCombinedCharLength(m_xInsertPos);
			m_xCaretPos =  m_xInsertPos;
			ShowCaretOnView();
			break;

		// Delete 键
		case VK_DELETE:
			if ((m_xInsertPos == m_xEndPos) || (m_xEndPos > BUFFERSIZE)) break;
			nTmpDec = GetCombinedCharLength(m_xInsertPos);
			err = memcpy_s(&m_szBuffer[m_xInsertPos], sizeof(m_szBuffer) - (m_xInsertPos+1)*WCHARSIZE, &m_szBuffer[m_xInsertPos+nTmpDec], WCHARSIZE * (m_xEndPos - m_xInsertPos - nTmpDec));
			ASSERT(!err);
			m_xEndPos -= nTmpDec;
			m_szBuffer[m_xEndPos] = L'\0';
			Invalidate();
			break;

		// Home 键
		case VK_HOME:
			if (m_xInsertPos == 0) break;
			m_xInsertPos = m_xCaretPos = 0;
			ShowCaretOnView();
			break;

		// End 键
		case VK_END:
			if (m_xInsertPos == m_xEndPos) break;
			m_xInsertPos = m_xCaretPos = m_xEndPos;
			ShowCaretOnView();
			break;
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


/*****************************************************************************\
* 函数: SetFont
*
* 从 IME 窗口接收 WM_INPUTLANGCHANGE 消息
* 
* 此示例根据键盘布局确定用于 ANSI 到 Unicode 
* 转换的字体和代码页，以便可以对所有东亚语言尝试四种不同的 
* IME，而不必更改字体或代码页。它不是支持 IME 级别 3 
* 的必备条件
*
* 参数: 
*	HKL  hKeyboardLayout - 指定新键盘布局的字符集
*	wchar_t *szSelectedFont - 字体名称。 
\*****************************************************************************/
void CIMEEdit::SetFont(HKL  hKeyboardLayout, LPCTSTR szSelectedFont)
{
	if (szSelectedFont)		
	{
		// 应用选择的字体 
#ifdef _UNICODE
		wcscpy_s(m_szWFontName, wcslen(szSelectedFont)+1, szSelectedFont);
		WideCharToMultiByte(m_nCodePage, 0, m_szWFontName, -1, (char *)m_szMBFontName, 50, NULL, NULL);
#else
		strcpy_s(m_szMBFontName, strlen(szSelectedFont)+1, szSelectedFont);
		MultiByteToWideChar(m_nCodePage, 0, (char *)szSelectedFont, (int)strlen(szSelectedFont)+1, m_szWFontName, 50);
#endif
	}
	else
	{
		// 检查键盘布局
		if (hKeyboardLayout == 0)
			hKeyboardLayout = GetKeyboardLayout(0);
		
		

		switch (LOWORD(hKeyboardLayout))
		{
			// 繁体中文 
			case LID_TRADITIONAL_CHINESE:	
				m_nLanguage = TRADITIONAL_CHINESE;
   				break;

			// 日语
			case LID_JAPANESE:				
				m_nLanguage = JAPANESE;
				break;

			// 朝鲜语
			case LID_KOREAN:				
				m_nLanguage = KOREAN;
				break;

			// 简体中文
			case LID_SIMPLIFIED_CHINESE:	
				m_nLanguage = SIMPLIFIED_CHINESE;
				break;

			default:
				m_nLanguage = DEFAULT;
				break;
		}
		
		// 设置代码页和字符集
		if (m_nLanguage)
			m_nCodePage = nCodePage[m_nLanguage];
		else
			m_nCodePage = GetACP();
		m_nCharSet = nCharSet[m_nLanguage];		

		size_t jdh = wcslen((wchar_t *)szDefaultFontName[m_nLanguage]);
		// 应用默认字体
		wcscpy_s(m_szWFontName, wcslen((wchar_t *)szDefaultFontName[m_nLanguage])+1, (wchar_t *)szDefaultFontName[m_nLanguage]);
		WideCharToMultiByte(m_nCodePage, 0, m_szWFontName, -1, (char *)m_szMBFontName, 50, NULL, NULL);

		// 获取 IME 属性
		m_dwProperty = ImmGetProperty(hKeyboardLayout, IGP_PROPERTY );
	}

	// 删除当前字体
	if (m_hFont)
		DeleteObject(m_hFont);

	// 创建字体
	if (m_fIsNT)
		m_hFont = CreateFontW(FONT_WIDTH, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, m_nCharSet, 
					 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, m_szWFontName);
	else
		m_hFont = CreateFontA(FONT_WIDTH, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, m_nCharSet, 
					 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, m_szMBFontName);
	
	// 如果未能创建选择的字体或默认字体，则创建默认字体
	if (!m_hFont)
	{
		// 应用默认字体
		wcscpy_s(m_szWFontName, wcslen((wchar_t *)szDefaultFontName[DEFAULT])+1, (wchar_t *)szDefaultFontName[DEFAULT]);
		WideCharToMultiByte(m_nCodePage, 0, m_szWFontName, -1, (char *)m_szMBFontName, 50, NULL, NULL);

		if (m_fIsNT)
			m_hFont = CreateFontW(FONT_WIDTH, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, m_nCharSet, 
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, m_szWFontName);
		else
			m_hFont = CreateFontA(FONT_WIDTH, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, m_nCharSet, 
						 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, m_szMBFontName);
	}

	ShowCaretOnView();
	Invalidate();
}

/*****************************************************************************\
* 函数: GetCombinedCharLength
*
* 获取组合字符的长度。 
*
* 此实现仅支持代理项。需要根据需要处理
* 组合的字符。  
*
* 返回值:
*	如果为 UNICODE，
*		如果为代理项字符，则返回 2，否则返回 1
*	如果不为 UNICODE
*		如果为 DBCS 字符，则返回 2，否则返回 1
\*****************************************************************************/
int CIMEEdit::GetCombinedCharLength(int nTmpPos) 
{
	int	i, nRet = 1;

	// 检查代理项字符
	for (i = 0; i <= nTmpPos; i++)
	{
		if ((0xD800 <= m_szBuffer[i]) && (m_szBuffer[i] <= 0xDBFF))
		{
			nRet = 2;
			++i;
		}
		else
			nRet = 1;
	}

	return nRet;
}

/*****************************************************************************\
* 函数: ClearBuffer
*
* 清除字符串缓冲区
\*****************************************************************************/
void CIMEEdit::ClearBuffer()
{
	memset(m_szBuffer, 0, WCHARSIZE * (BUFFERSIZE + 1));
	m_xEndPos = m_xInsertPos = m_xCaretPos = 0;
}


/*****************************************************************************\
* 函数: SetCandiDateWindowPos
*
* 设置日语/朝鲜语 IME 的候选窗口位置
\*****************************************************************************/
void CIMEEdit::SetCandiDateWindowPos() 
{
	HIMC		hIMC;
	CClientDC	dc(this);
	TEXTMETRIC	tm;
	CANDIDATEFORM Candidate;
	errno_t err;
	
	hIMC = ImmGetContext(this->m_hWnd);
	if (hIMC) 
	{
		dc.GetTextMetrics(&tm);
		
		Candidate.dwIndex = 0;
		Candidate.dwStyle = CFS_FORCE_POSITION;

		if (m_nLanguage == JAPANESE) 
		{
			// 在可编辑字符附近设置候选窗口位置
			wchar_t *szTmpStr = new(wchar_t[m_xInsertPos + m_nComCursorPos + 1]);

			err = memcpy_s(szTmpStr, WCHARSIZE* (m_xInsertPos + m_nComCursorPos), m_szBuffer, WCHARSIZE * (m_xInsertPos + m_nComCursorPos));	 
			ASSERT(!err);
			szTmpStr[m_xInsertPos + m_nComCursorPos] = L'\0';
			Candidate.ptCurrentPos.x = X_INIT + GetWidthOfString(szTmpStr);
		}
		else
		{
			// 在插入符号位置附近设置候选窗口位置
			CPoint		point;

			point = GetCaretPos();
			Candidate.ptCurrentPos.x = point.x;
		}
		Candidate.ptCurrentPos.y = Y_INIT + tm.tmHeight + 1;
		ImmSetCandidateWindow(hIMC, &Candidate);

		ImmReleaseContext(this->m_hWnd,hIMC);
	}
}

/*****************************************************************************\
* 函数: SetCompositionWindowPos
*
* 设置繁体中文 IME 的撰写窗口位置
\*****************************************************************************/
void CIMEEdit::SetCompositionWindowPos() 
{
	HIMC		hIMC;
    CPoint		point;
    COMPOSITIONFORM Composition;

	hIMC = ImmGetContext(this->m_hWnd);
	if (hIMC) 
	{
		// 在插入符号位置附近设置撰写窗口位置
		point = GetCaretPos();
		Composition.dwStyle = CFS_POINT;
		Composition.ptCurrentPos.x = point.x;
		Composition.ptCurrentPos.y = point.y;
		ImmSetCompositionWindow(hIMC, &Composition);

		ImmReleaseContext(this->m_hWnd,hIMC);
	}
}

LRESULT CIMEEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
		// 如果输入语言发生更改，则更改此设置
		case WM_INPUTLANGCHANGE:
			ClearBuffer();

			// 为更改的输入语言创建新字体
			SetFont((HKL) lParam, NULL);
			Invalidate();
			ShowCaretOnView();

			// 发送更改 IME 模式的消息
			::PostMessage(GetParent()->m_hWnd, WM_SETINPUTLANG, 0L, 0L);
			break;

		// 开始撰写
		case WM_IME_STARTCOMPOSITION:
			// 打开撰写标志
			m_fStat = TRUE;

			// 如果 IME 属性没有 IME_PROP_AT_CARET，则忽略级别 3 功能
			if (!(m_dwProperty & IME_PROP_AT_CARET) )
				break;

			OnImeStartComposition(wParam, lParam);			
			return 0l;

		// 撰写字符
		case WM_IME_COMPOSITION:

			if (OnImeComposition(wParam, lParam))
				return 0l;
			// 调用 CEdit::WinProc 以显示撰写窗口
			break;
		// 结束撰写
		case WM_IME_ENDCOMPOSITION:

			// 关闭撰写标志
			m_fStat = FALSE;

			// 如果 IME 属性没有 IME_PROP_AT_CARET，则忽略级别 3 功能
			if (!(m_dwProperty & IME_PROP_AT_CARET) )
				break;

			OnImeEndComposition(wParam, lParam);			
			return 0l;

		case WM_IME_NOTIFY:
			switch (wParam)
			{
				// 设置候选窗口位置
				case IMN_OPENCANDIDATE:
					if (m_nLanguage == JAPANESE)
						SetCandiDateWindowPos();
					break;

				// 检测 IME 模式的更改 
				case IMN_SETCONVERSIONMODE:
				case IMN_SETSENTENCEMODE:
					::PostMessage(GetParent()->m_hWnd, WM_RESETMODE, wParam, lParam);
					break;

				// 检测日语 IME 的切换
				case IMN_SETOPENSTATUS:   
					::PostMessage(GetParent()->m_hWnd, WM_TOGGLE, wParam, lParam);
					break;
			}	
			break;

		case WM_IME_SETCONTEXT:
			// 除了日文版 Win9x 中的 IME 模式控件外，无需发送此消息
			if ((m_nLanguage ==  JAPANESE) && !m_fIsNT) 				
			{
				::PostMessage(GetParent()->m_hWnd, WM_SETMODE, wParam, lParam);
			}
			break;
	}		
	return CEdit::WindowProc(message, wParam, lParam);
}
