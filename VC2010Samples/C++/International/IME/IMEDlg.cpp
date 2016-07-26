/******************************************************************************\
*	IMEDlg.cpp: 实现文件
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
#include "IMM.h"
#include "IMEEdit.h"
#include "IME.h"
#include "IMEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 声明枚举类型语言
extern enum LANGFLAG
{
   ENGLISH,				
   TRADITIONAL_CHINESE,	
   JAPANESE,
   KOREAN,
   SIMPLIFIED_CHINESE
} LangFlag;  

// 转换模式中有效选项的标志 
const BOOL fConvMode[5][13] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
	{1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0}, 
	{1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1}, 
	{1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0}};

// 语句模式中的有效选项的标志 
const BOOL fSentMode[5][6] = {
	{0, 0, 0, 0, 0, 0}, 
	{0, 1, 0, 0, 0, 0}, 
	{1, 1, 1, 1, 1, 1}, 
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}};

// 转换模式消息 
const DWORD dwConvModeMsg[13] = {
	IME_CMODE_ALPHANUMERIC,		IME_CMODE_NATIVE,		IME_CMODE_KATAKANA, 
	IME_CMODE_LANGUAGE,			IME_CMODE_FULLSHAPE,	IME_CMODE_ROMAN, 
	IME_CMODE_CHARCODE,			IME_CMODE_HANJACONVERT, IME_CMODE_SOFTKBD, 
	IME_CMODE_NOCONVERSION,		IME_CMODE_EUDC,			IME_CMODE_SYMBOL, 
	IME_CMODE_FIXED};

// 语句模式消息 
const DWORD dwSentModeMsg[6] = {
	IME_SMODE_NONE,			IME_SMODE_PLAURALCLAUSE,	IME_SMODE_SINGLECONVERT,	
	IME_SMODE_AUTOMATIC,	IME_SMODE_PHRASEPREDICT,	IME_SMODE_CONVERSATION };

const TCHAR szEnglishFontName[5][11] = {
	_T("Tahoma"),		_T("MingLiU"),		_T("MS PGothic"), 
	_T("Gulim"),		_T("NSimSun")};


/////////////////////////////////////////////////////////////////////////////
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard 生成的虚函数重写
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//}}AFX_VIRTUAL

// 实现
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// 无消息处理程序
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMEDlg 对话框

CIMEDlg::CIMEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIMEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIMEDlg)
	//}}AFX_DATA_INIT
	// 注意，LoadIcon 在 Win32 中不需要后续的 DestroyIcon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSelectedID = 0;
}

void CIMEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIMEDlg)
	DDX_Control(pDX, IDC_COMBO_FONT, m_FontList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIMEDlg, CDialog)
	//{{AFX_MSG_MAP(CIMEDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_IME_ENABLE, OnRadioImeEnable)
	ON_BN_CLICKED(IDC_RADIO_IME_DISABLE, OnRadioImeDisable)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT, OnSelchangeComboFont)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETINPUTLANG, OnSetInputLang)
	ON_MESSAGE(WM_SETMODE, OnSetIME)
	ON_MESSAGE(WM_RESETMODE, OnReSetIME)
	ON_MESSAGE(WM_TOGGLE, OnToggleJPNIME)
	ON_COMMAND_RANGE(IDC_CHECK_IME_CMODE_ALPHANUMERIC, IDC_CHECK_IME_SMODE_CONVERSATION, OnChangeOption)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMEDlg 消息处理程序

BOOL CIMEDlg::OnInitDialog()
{
	CClientDC	dc(this);
	LOGFONT lf;

	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，
	//  框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// 使用子类获取 WM_INPUTLANGCHANGE/WM_IME_NOTIFY 消息
	m_IMEEdit.SubclassDlgItem(IDC_IME_EDIT, this);

	// 选中“IME Enable”(启用 IME)按钮作为默认设置
	CheckDlgButton(IDC_RADIO_IME_ENABLE, BST_CHECKED);


	// 获取当前键盘布局并启用/禁用复选框
	m_IMEEdit.SetFont(0, NULL);

	// 获取系统中的字体名称
	if (dc)
	{
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfCharSet = DEFAULT_CHARSET;

		::EnumFontFamiliesEx(dc, &lf, (FONTENUMPROC) EnumFontFamExProc, (LPARAM) &m_FontList, NULL);
	}

	OnSetInputLang(0L, 0L);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIMEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIMEDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 当用户拖动最小化窗口时系统调用此函数
//  取得要显示的光标。
HCURSOR CIMEDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*****************************************************************************\
* 函数: OnSetInputLang
*
* 从测试窗口接收 WM_INPUTLANGCHANGE 消息，并设置语言和
* 创建字体
*
* 参数: 
*	WPARAM wParam - 未使用
*	LPARAM lParam - 未使用
\*****************************************************************************/
LRESULT CIMEDlg::OnSetInputLang(WPARAM, LPARAM)
{
	HWND	hWnd; 

	// 启用有效选项 
	SetOption();

	// 获取 IME 中的默认设置并显示默认设置
	hWnd = GetDlgItem(IDC_IME_EDIT)->m_hWnd;
	if (hWnd && IsWindow(hWnd))
	{
		// 获取 IME 状态并设置示例中的转换模式选项
		GetIMEMode(hWnd);
		::SetFocus(hWnd);
	}

#ifdef _UNICODE
	if (m_FontList.SelectString(0, m_IMEEdit.m_szWFontName) == CB_ERR)
#else
	if (m_FontList.SelectString(0, m_IMEEdit.m_szMBFontName) == CB_ERR)
#endif
		m_FontList.SelectString(0, szEnglishFontName[m_IMEEdit.m_nLanguage]);

	return 0L;
}

/*****************************************************************************\
* 函数:SetOption
*
* 使用键盘 ID 启用/禁用每个复选框
*
* 参数:
*   WORD wID - 键盘 ID
\*****************************************************************************/
void CIMEDlg::SetOption()
{
	int nID;

	// 启用/禁用转换模式的复选框
	for (nID = IDC_CHECK_IME_CMODE_ALPHANUMERIC; nID <= IDC_CHECK_IME_SMODE_CONVERSATION; nID++)
	{
		((CButton*)GetDlgItem(nID))->SetCheck(0);
		GetDlgItem(nID)->EnableWindow(fConvMode[m_IMEEdit.m_nLanguage][nID - IDC_CHECK_IME_CMODE_ALPHANUMERIC]);	
	} 

	// 启用/禁用语句模式的复选框
	for (nID = IDC_CHECK_IME_SMODE_NONE; nID <= IDC_CHECK_IME_SMODE_CONVERSATION; nID++)
	{
		((CButton*)GetDlgItem(nID))->SetCheck(0);
		GetDlgItem(nID)->EnableWindow(fSentMode[m_IMEEdit.m_nLanguage][nID - IDC_CHECK_IME_SMODE_NONE]);	
	}
}

/*****************************************************************************\
* 函数: GetIMEMode
*
* 设置 IME 窗口的 IME 模式并设置示例中的选项
*
* 参数: 
*	HWND hWnd - 测试窗口的句柄
\*****************************************************************************/
void CIMEDlg::GetIMEMode(HWND hWnd)
{
	HIMC	hImc;
	UINT	nID;
	DWORD	dwConvMode, dwSentMode;

	// 获取输入上下文
	hImc = ImmGetContext(hWnd);
	if (hImc)
	{
		// 获取当前 IME 状态
		ImmGetConversionStatus(hImc, &dwConvMode, &dwSentMode);
		if (m_IMEEdit.m_nLanguage == JAPANESE)
		{
			if (ImmGetOpenStatus(hImc))
				dwConvMode |= 1;
			else
				dwConvMode &= ~1;
		}

		// 更改示例中的转换模式选项
		for (nID = IDC_CHECK_IME_CMODE_NATIVE; nID <= IDC_CHECK_IME_CMODE_FIXED; nID++)
		{
			if (fConvMode[m_IMEEdit.m_nLanguage][nID - IDC_CHECK_IME_CMODE_ALPHANUMERIC])
			{
				// IME_CMODE_LANGUAGE 选项是组合选项 
				if (nID == IDC_CHECK_IME_CMODE_LANGUAGE)	
					((CButton*)GetDlgItem(nID))->SetCheck( (dwConvMode & dwConvModeMsg[1]) && (dwConvMode & dwConvModeMsg[2]));
				else
					((CButton*)GetDlgItem(nID))->SetCheck(dwConvMode & dwConvModeMsg[nID - IDC_CHECK_IME_CMODE_ALPHANUMERIC]);
				// IME_CMODE_ALPHANUMERIC 和 IME_CMODE_NATIVE 是切换选项
				if (nID == IDC_CHECK_IME_CMODE_NATIVE)	
					((CButton*)GetDlgItem(IDC_CHECK_IME_CMODE_ALPHANUMERIC))->SetCheck(!(dwConvMode & dwConvModeMsg[nID - IDC_CHECK_IME_CMODE_ALPHANUMERIC]));
			}
		}

		// 更改示例中的语句模式选项
		// 首先设置 IME_SMODE_NONE 选项。 
		if ((fSentMode[m_IMEEdit.m_nLanguage][0]) && (dwSentMode == IME_SMODE_NONE))
			((CButton*)GetDlgItem(IDC_CHECK_IME_SMODE_NONE))->SetCheck(1);
		else
			((CButton*)GetDlgItem(IDC_CHECK_IME_SMODE_NONE))->SetCheck(0);

		// 设置其他转换选项。如果 IME_SMODE_NONE 被激活，则忽略其他选项
		for (nID = IDC_CHECK_IME_SMODE_PLAURALCLAUSE; nID <= IDC_CHECK_IME_SMODE_CONVERSATION; nID++)
		{
			if ((fSentMode[m_IMEEdit.m_nLanguage][nID - IDC_CHECK_IME_SMODE_NONE]) && (dwSentMode != IDC_CHECK_IME_SMODE_NONE))
				((CButton*)GetDlgItem(nID))->SetCheck(dwSentMode & dwSentModeMsg[nID - IDC_CHECK_IME_SMODE_NONE]);
		}

		// 释放输入上下文
		ImmReleaseContext(hWnd, hImc);
	}
}

/*****************************************************************************\
* 函数: OnChangeOption
*
* 在用户单击复选框时更改 IME 模式
*
* 参数:
*   UINT nID - 复选框的 ID
\*****************************************************************************/
void CIMEDlg::OnChangeOption(UINT nID)
{
	// 在编辑窗口中设置焦点
	::SetFocus(GetDlgItem(IDC_IME_EDIT)->m_hWnd);
	
	// 保存选择的选项
	m_nSelectedID = nID;
	
	// 对于日文版 Win98 中的软键盘，OnSetIME 在编辑时必须调用 
	// 窗口获取焦点
	if ((m_IMEEdit.m_nLanguage != JAPANESE) || m_IMEEdit.m_fIsNT)
		OnSetIME(0, 0);
}

/*****************************************************************************\
* 函数: SetIMEMode
*
* 设置 IME 模式
*
* 参数:
*	HWND hWnd - 句柄测试窗口
*	DWORD dwNewConvMode - 用于更改转换模式的选项
*	DWORD dwNewSentMode - 用于更改语句模式的旋项
*	BOOL fFlag - 如果为 TRUE 则打开传递的选项。否则关闭该选项
\*****************************************************************************/
void CIMEDlg::SetIMEMode(HWND hWnd, DWORD dwNewConvMode, DWORD dwNewSentMode, BOOL fFlag)
{
	HIMC	hImc;
	DWORD	dwConvMode, dwSentMode;
	BOOL	fRet;

	// 获取输入上下文
	hImc = ImmGetContext(hWnd);
	if (hImc)
	{
		// 获取当前 IME 状态
		ImmGetConversionStatus(hImc, &dwConvMode, &dwSentMode);

		// 使用传递的选项更改 IME 状态
		if (fFlag)
		{
			fRet = ImmSetConversionStatus(hImc, dwConvMode | dwNewConvMode, dwSentMode | dwNewSentMode);
			if ((m_IMEEdit.m_nLanguage == JAPANESE) && (dwNewConvMode & IME_CMODE_NATIVE))
				ImmSetOpenStatus(hImc, fFlag);
 
		}
		else
		{
			ImmSetConversionStatus(hImc, dwConvMode & ~dwNewConvMode, dwSentMode & ~dwNewSentMode);
			if ((m_IMEEdit.m_nLanguage == JAPANESE) && (dwNewConvMode & IME_CMODE_NATIVE))
				ImmSetOpenStatus(hImc, fFlag);
		}

		// 释放输入上下文
		ImmReleaseContext(hWnd, hImc);
	}
}

/*****************************************************************************\
* 函数: OnSetIME
*
* 从测试窗口接收 WM_INT_NOTIFY(IMN_SETCOMPOSITIONWINDOW) 消息
* 或是在选项更改时被调用
*
* 参数:
*	WPARAM wParam - 通知命令
*	LPARAM lParam - 特定于命令的数据
\*****************************************************************************/
LRESULT CIMEDlg::OnSetIME(WPARAM, LPARAM)
{
	HWND	hWnd; 
	BOOL	fFlag;

	// 选中保存的选项
	if (m_nSelectedID)
	{
		hWnd = GetDlgItem(IDC_IME_EDIT)->m_hWnd;

		if (hWnd && IsWindow(hWnd))
		{
			// 获取复选框状态
			fFlag = ((CButton*)GetDlgItem(m_nSelectedID))->GetCheck();

			// 设置示例中的转换模式选项
			if (m_nSelectedID < IDC_CHECK_IME_SMODE_NONE)
			{	
				// IME_CMODE_ALPHANUMERIC 和 IME_CMODE_NATIVE 是切换选项
				if (m_nSelectedID == IDC_CHECK_IME_CMODE_ALPHANUMERIC)
					SetIMEMode(hWnd, dwConvModeMsg[1], 0, !fFlag);
				else
					SetIMEMode(hWnd, dwConvModeMsg[m_nSelectedID - IDC_CHECK_IME_CMODE_ALPHANUMERIC], 0, fFlag);
			}
			// 设置示例中的语句模式选项
			else if (m_nSelectedID == IDC_CHECK_IME_SMODE_NONE)
			{
				// 若要设置 IME_SMODE_NONE，转换模式必须为 0 
				SetIMEMode(hWnd, 0, ~dwSentModeMsg[m_nSelectedID - IDC_CHECK_IME_SMODE_NONE], !fFlag);
			}
			else
				SetIMEMode(hWnd, 0, dwSentModeMsg[m_nSelectedID - IDC_CHECK_IME_SMODE_NONE], fFlag);

			// 获取 IME 状态并设置示例中的转换模式选项
			GetIMEMode(hWnd);
		}
		m_nSelectedID = 0;			
	}

	return 0L;
}

/*****************************************************************************\
* 函数: OnReSetIME
*
* 从测试窗口接收 WM_INT_NOTIFY(IMN_SETCONVERSIONMODE, IMN_SETSENTENCEMODE) 
* 消息
*
* 参数:
*	WPARAM wParam - 通知命令
*	LPARAM lParam - 特定于命令的数据
\*****************************************************************************/
LRESULT CIMEDlg::OnReSetIME(WPARAM, LPARAM)
{
	HWND	hWnd; 

	// 使 IME 选项与 IME 窗口同步
	hWnd = GetDlgItem(IDC_IME_EDIT)->m_hWnd;
	if (hWnd && IsWindow(hWnd))
	{
		// 获取 IME 状态并设置示例中的转换模式选项
		GetIMEMode(hWnd);
		::SetFocus(hWnd);
	}

	return 0L;
}

/*****************************************************************************\
* 函数: OnToggleJPNIME
*
* 设置日语 IME 的 Open 状态
*
* 参数: 
*	WPARAM wParam - 通知命令
*	LPARAM lParam - 特定于命令的数据
\*****************************************************************************/
LRESULT CIMEDlg::OnToggleJPNIME(WPARAM, LPARAM)
{
	HIMC	hImc;

	if (m_IMEEdit.m_nLanguage != JAPANESE)
		return 0L;

	// 获取输入上下文
	hImc = ImmGetContext(this->m_hWnd);
	if (hImc)
	{
		if (ImmGetOpenStatus(hImc))
		{
			((CButton*)GetDlgItem(IDC_CHECK_IME_CMODE_NATIVE))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_CHECK_IME_CMODE_ALPHANUMERIC))->SetCheck(0);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_IME_CMODE_NATIVE))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_CHECK_IME_CMODE_ALPHANUMERIC))->SetCheck(1);
		}

		// 释放输入上下文
		ImmReleaseContext(this->m_hWnd, hImc);
	}

	return 0L;
}

/*****************************************************************************\
* 函数: OnRadioImeEnable
*
* “IME Enable”(启用 IME)按钮的事件 
\*****************************************************************************/
void CIMEDlg::OnRadioImeEnable() 
{
	HWND hWnd; 

	hWnd = GetDlgItem(IDC_IME_EDIT)->m_hWnd;
	if (hWnd && IsWindow(hWnd))
	{
		// 启用 IME
		if (m_hImc)
		{
			// 使输入上下文与测试窗口相关联
			ImmAssociateContext(hWnd, m_hImc);
			m_hImc = NULL;
		}

		// 同步示例和 IME 窗口中的当前设置
		OnReSetIME(0, 0);

		::SetFocus(hWnd);
	}	
}

/*****************************************************************************\
* 函数: OnRadioImeEnable
*
* “IME Disable”(禁用 IME)按钮的事件 
\*****************************************************************************/
void CIMEDlg::OnRadioImeDisable() 
{
	HWND	hWnd; 

	hWnd = GetDlgItem(IDC_IME_EDIT)->m_hWnd;
	if (hWnd && IsWindow(hWnd))
	{
		// 获取备份的输入上下文。 
		m_hImc = ImmGetContext(hWnd);

		// 移除关联测试 
		if (m_hImc)
			ImmAssociateContext(hWnd, NULL);

		// 释放输入上下文
		ImmReleaseContext(hWnd, m_hImc);
		::SetFocus(hWnd);
	}	
}

/*****************************************************************************\
* 函数: OnSelchangeComboFont
*
* 更改字体的事件 
\*****************************************************************************/
void CIMEDlg::OnSelchangeComboFont() 
{	
	CString	szFontName;

	// 获取选择的字体名称并设置字体
	m_FontList.GetLBText(m_FontList.GetCurSel(), szFontName);
	m_IMEEdit.SetFont(0, (LPCTSTR)szFontName);
	
	::SetFocus(GetDlgItem(IDC_IME_EDIT)->m_hWnd);		
}

/*****************************************************************************\
* 函数: EnumFamScreenCallBackEx
*
* 更改字体的事件 
\*****************************************************************************/
BOOL CALLBACK AFX_EXPORT CIMEDlg::EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX*, int FontType, LPVOID pThis)
{
	// 不要放置在非打印机光栅字体中
	if (FontType & TRUETYPE_FONTTYPE)
	{
		if (((CComboBox *) (pThis))->FindString(0, (LPCTSTR) pelf->elfLogFont.lfFaceName) == CB_ERR)
			((CComboBox *) (pThis))->AddString( (LPCTSTR) pelf->elfLogFont.lfFaceName);
	}
	return 1;
}

void CIMEDlg::OnOk() 
{
	PostQuitMessage(0);	
}
