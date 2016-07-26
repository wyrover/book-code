#if !defined(AFX_IMEEDIT_H__F425503C_0F3C_4C7D_A2E5_A9A7E35AC07B__INCLUDED_)
#define AFX_IMEEDIT_H__F425503C_0F3C_4C7D_A2E5_A9A7E35AC07B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IMEEdit.h: 头文件
//

// 定义 CIMEEdit 的常量
#define X_INIT					5		
#define Y_INIT					5
#define FONT_HEIGHT				16
#define FONT_WIDTH				16
#define BUFFERSIZE				30
#define WCHARSIZE				2

// 定义语言 ID
#define LID_TRADITIONAL_CHINESE	0x0404
#define LID_JAPANESE			0x0411
#define LID_KOREAN				0x0412
#define LID_SIMPLIFIED_CHINESE	0x0804

//定义自定义消息
#define WM_SETINPUTLANG	WM_USER + 1
#define WM_SETMODE		WM_USER + 2
#define WM_RESETMODE	WM_USER + 3
#define WM_TOGGLE		WM_USER + 4

/////////////////////////////////////////////////////////////////////////////
// CIMEEdit 窗口

class CIMEEdit : public CEdit
{
// 构造
public:
	CIMEEdit();

// 特性
protected:
	int		m_xEndPos;					// 缓冲区的结尾位置 
	int		m_xInsertPos;				// 缓冲区的插入位置
	int		m_xCaretPos;				// 缓冲区的插入符号位置
	int		m_nComSize;					// 撰写字符串大小
	int		m_nComCursorPos;			// 撰写字符串中的当前光标位置
	DWORD	m_nComAttrSize;				// 撰写字符串的特性的大小
	int		m_nInvertStart;				// 反转字符串的起始位置
	int		m_nInvertEnd;				// 反转字符串的结尾位置
	DWORD	m_dwProperty;				// IME 的属性
	UINT	m_nCodePage;				// 代码页
	BYTE	m_nCharSet;					// 字符集
	HFONT	m_hFont;					// 字体
	BOOL	m_fShowInvert;				// 如果为 true，则示例显示反转字符串 
	BOOL	m_fStat;					// 如果为 true，则为 IME 撰写字符串
	BYTE	m_bComAttr[BUFFERSIZE+1];	// 撰写字符串的特性
	DWORD	m_dwCompCls[BUFFERSIZE+1];	// 撰写 
	wchar_t	m_szBuffer[BUFFERSIZE+1];	// 输入字符串的缓冲区
	wchar_t	m_szComStr[BUFFERSIZE+1];	// 撰写字符串的缓冲区
	wchar_t	m_szBackup[BUFFERSIZE+1];	// 用于备份最后一个字符串的缓冲区

public:
	wchar_t m_szWFontName[50];			// UNICODE 字符的字体名称
	char	m_szMBFontName[50];			// 多字节字符的字体名称
	int		m_nLanguage;				// 语言的内部索引
	BOOL	m_fIsNT;					// 如果为 true，则为 NT。否则为 Win9X
	BOOL	m_fIsXP;						// 如果为 true，则为 Windows XP 或更高版本
// 操作
public:
	BOOL OnImeComposition(WPARAM wParam,LPARAM lParam);
	void OnImeStartComposition(WPARAM wParam,LPARAM lParam);	
	void OnImeEndComposition(WPARAM wParam,LPARAM lParam);
	int	 InsertCompStr();
	void ShowCaretOnView();
	int  GetWidthOfString(wchar_t*);
	void SetFont(HKL hKeyboardLayout, LPCTSTR szSelectedFont);
	void ClearBuffer();
	int  GetCombinedCharLength(int); 
	void SetCandiDateWindowPos();
	void SetCompositionWindowPos();

// 重写
	// ClassWizard 生成的虚函数重写
	//{{AFX_VIRTUAL(CIMEEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// 实现
public:
	virtual ~CIMEEdit();

	// 生成的消息映射函数
protected:
	//{{AFX_MSG(CIMEEdit)
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ 将紧挨上一行之前插入附加声明。

#endif // !defined(AFX_IMEEDIT_H__F425503C_0F3C_4C7D_A2E5_A9A7E35AC07B__INCLUDED_)
