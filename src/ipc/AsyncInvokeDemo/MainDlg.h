// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlmisc.h>
#include <atltime.h>

class CMainDlg :
    public CDialogImpl<CMainDlg>,
    public CWinDataExchange<CMainDlg>,
    public CMessageFilter,
    public CSynchronizeInvoke
{
public:
    enum { IDD = IDD_MAINDLG };

    virtual BOOL PreTranslateMessage(MSG *pMsg)
    {
        // Process thread callback messages
        if (CSynchronizeInvoke::IsThreadCallbackMessage(pMsg))
            return TRUE;

        return CWindow::IsDialogMessage(pMsg);
    }

    BEGIN_DDX_MAP(CMainDlg)
    DDX_CONTROL_HANDLE(IDC_LSTVWCTRLEVENTS, m_cEventsListViewCtrl)
    DDX_CONTROL_HANDLE(IDC_BTNTEST, m_cTestButton)
    END_DDX_MAP()

    BEGIN_MSG_MAP(CMainDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
    COMMAND_ID_HANDLER(IDC_BTNTEST, OnTest)
    END_MSG_MAP()

    // Handler prototypes (uncomment arguments if needed):
    //LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    //LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    //LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        HMODULE hModule = ::LoadLibrary(_T("advapi32.dll"));
        ATLASSERT(hModule != NULL);
        RtlGenRandom = (RTLGENRANDOM)::GetProcAddress(hModule, "SystemFunction036");
        ATLASSERT(RtlGenRandom != NULL);
        // set icons
        HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
                                         IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
        SetIcon(hIcon, TRUE);
        HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
                                              IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
        SetIcon(hIconSmall, FALSE);
        // register object for message filtering
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);
        pLoop->AddMessageFilter(this);
        // DDX
        DoDataExchange();
        InitializeListViewCtrl();
        return TRUE;
    }

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        // unregister message filtering
        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);
        pLoop->RemoveMessageFilter(this);
        return 0;
    }

    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        DestroyWindow();
        ::PostQuitMessage(0);
        return 0;
    }

    LRESULT OnWindowPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
    {
        // Simulating OnShown event
        if (((LPWINDOWPOS)lParam)->flags & SWP_SHOWWINDOW) {
            AppendEvent(_T("Initialized."));
        }

        return 0;
    }

    LRESULT OnTest(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        UINT n = 0;
        // Generate a random number to pass as test parameter
        BOOLEAN bRes = RtlGenRandom((PVOID)&n, sizeof(n));
        ATLASSERT(bRes != FALSE);
        CString szMsg;
        szMsg.Format(_T("Calling \"Test1(ulParam = %u)\" on the thread pool..."), n);
        AppendEvent(szMsg);
        MAKECLSDELEGATE(CMainDlg, Test1)->BeginInvoke(n, MAKECLSDELEGATE(CMainDlg, Test1Callback));
        return 0;
    }

private:
    VOID AppendEvent(LPCTSTR lpszMessage, INT nIcon = 0)
    {
        INT nCount = m_cEventsListViewCtrl.GetItemCount();
        CString szThreadId;
        szThreadId.Format(_T("%ld"), ::GetCurrentThreadId());
        m_cEventsListViewCtrl.AddItem(nCount, 0, _T(""), nIcon);
        m_cEventsListViewCtrl.AddItem(nCount, 1, CTime::GetCurrentTime().Format(_T("%I:%M:%S %p")));
        m_cEventsListViewCtrl.AddItem(nCount, 2, szThreadId);
        m_cEventsListViewCtrl.AddItem(nCount, 3, lpszMessage);
        m_cEventsListViewCtrl.EnsureVisible(nCount, FALSE);
    }

    ULONG_PTR Test1(ULONG_PTR ulParam)
    {
        CString szMsg;
        szMsg.Format(_T("This is \"Test1(ulParam = %u)\" running on the thread pool. Using \"ulParam\" as return value."), (UINT)ulParam);
        AppendEvent(szMsg);
        return ulParam;
    }

    ULONG_PTR Test1Callback(ULONG_PTR ulParam)
    {
        // Retreiving Test1()'s return value
        CAsyncResult *pAsyncResult = (CAsyncResult *)ulParam;
        IDelegate *pDelegate = pAsyncResult->GetAsyncDelegate();
        UINT nRes = (UINT)pDelegate->EndInvoke((IAsyncResult **)&pAsyncResult);
        ATLASSERT(pAsyncResult == NULL);
        // NEVER use the async result after calling EndInvoke!
        CString szMsg;
        szMsg.Format(_T("This is \"Test1Callback()\". Return value of \"Test1()\" is %u."), nRes);
        AppendEvent(szMsg);
        szMsg.Format(_T("Now calling \"Test2(ulParam = %u)\" on the main thread..."), nRes);
        AppendEvent(szMsg);
        // Call Test2() as "fire & forget", meaning its return value is never retrieved
        BeginInvoke(MAKECLSDELEGATE(CMainDlg, Test2), (ULONG_PTR)nRes);
        // Return value of a callback is never used
        return 0;
    }

    ULONG_PTR Test2(ULONG_PTR ulParam)
    {
        CString szMsg;
        szMsg.Format(_T("This is \"Test2(ulParam = %u)\" called as \"fire & forget\"."), (UINT)ulParam);
        AppendEvent(szMsg);
        // Return value not used as this was called as a "fire & forget"
        return 0;
    }

private:
    VOID InitializeListViewCtrl()
    {
        INT cx = ::GetSystemMetrics(SM_CXSMICON);
        INT cy = ::GetSystemMetrics(SM_CYSMICON);
        m_cEventsImageList.Create(cx, cy, ILC_COLOR32, 3, 1);
        m_cEventsImageList.AddIcon(::AtlLoadSysIconImage(IDI_INFORMATION, LR_SHARED));
        m_cEventsImageList.AddIcon(::AtlLoadSysIconImage(IDI_WARNING, LR_SHARED));
        m_cEventsImageList.AddIcon(::AtlLoadSysIconImage(IDI_ERROR, LR_SHARED));
        m_cEventsListViewCtrl.SetImageList(m_cEventsImageList, LVSIL_SMALL);
        m_cEventsListViewCtrl.ModifyStyle(0, LVS_REPORT | LVS_NOSORTHEADER);
        m_cEventsListViewCtrl.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
        m_cEventsListViewCtrl.AddColumn(_T(""), 0);
        m_cEventsListViewCtrl.SetColumnWidth(0, 26);
        m_cEventsListViewCtrl.AddColumn(_T("Time"), 1);
        m_cEventsListViewCtrl.SetColumnWidth(1, 80);
        m_cEventsListViewCtrl.AddColumn(_T("Thread ID"), 2);
        m_cEventsListViewCtrl.SetColumnWidth(2, 75);
        m_cEventsListViewCtrl.AddColumn(_T("Message"), 3);
        m_cEventsListViewCtrl.SetColumnWidth(3, 500);
    }

private:
    typedef BOOLEAN(WINAPI *RTLGENRANDOM)(PVOID, ULONG);
    RTLGENRANDOM RtlGenRandom;

    CImageList m_cEventsImageList;
    CListViewCtrl m_cEventsListViewCtrl;
    CButton m_cTestButton;
};
