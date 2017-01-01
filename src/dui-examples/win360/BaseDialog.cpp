#include "StdAfx.h"
#include "BaseDialog.h"

#include <memory>


//消息通告
void BaseDialog::Notify(TNotifyUI& msg)
{
    //如果是点击消息那么通过控件名字判断是哪个控件
    if (msg.sType == _T("click")) {
        if (msg.pSender == static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn"))))
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);

        if (msg.pSender == static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn"))))
            PostQuitMessage(0);

        if (msg.pSender == static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn"))))
            ::IsZoomed(*this) ? SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0) : SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

        if (msg.pSender == static_cast<CButtonUI*>(m_pm.FindControl(_T("check_normal")))) {
            ::MessageBox(NULL, L"开始扫描", L"提示", MB_OK);
        }
    }
}

//首先启动消息循环会进入此虚函数进行消息处理
LRESULT BaseDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //初始化消息映射MAP  利用auto_ptr维护指针 static保证只创建一次
    static std::auto_ptr<MessageMap> customMessageMap(InitMessageMap());
    BOOL bHandled = TRUE;
    LRESULT lRes = 0;

    //将消息在消息映射map中进行查找 找到响应的消息处理函数
    if (customMessageMap->find(uMsg) != customMessageMap->end()) {
        //typedef HRESULT (BaseDialog::*CustomMsgHandler)(WPARAM, LPARAM, BOOL&);
        //如果找到 查找响应的消息响应函数
        CustomMsgHandler handler = (*customMessageMap)[uMsg];
        //通过this->(*handler)进行消息响应函数的调用
        lRes = (this->*handler)(wParam, lParam, bHandled);

        //如果 bHandled返回True没有被修改那么说明消息已经被处理 返回
        if (bHandled) return lRes;
    }

    //CPaintManagerUI丢给PaintManagerUI进行处理 如果处理了 那么会返回True 否则返回false继续走
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

    //最后丢给默认的windows消息处理函数
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

//初始化消息循环对应的消息响应函数
BaseDialog::MessageMap* BaseDialog::InitMessageMap()
{
    MessageMap* map = new MessageMap;
    (*map)[WM_CREATE] = &BaseDialog::OnCreate;
    (*map)[WM_DESTROY] = &BaseDialog::OnDestory;
    (*map)[WM_ERASEBKGND] = &BaseDialog::OnErasebkgnd;
    (*map)[WM_SIZE] = &BaseDialog::OnSize;
    (*map)[WM_NCPAINT] = &BaseDialog::OnNcPaint;
    //以下三个消息用于屏蔽系统标题栏
    (*map)[WM_NCACTIVATE] = &BaseDialog::OnNcActive;
    (*map)[WM_NCCALCSIZE] = &BaseDialog::OnNcCalSize;
    (*map)[WM_NCHITTEST] = &BaseDialog::OnNcHitTest;
    (*map)[WM_SYSCOMMAND] = &BaseDialog::OnSysCommand;
    (*map)[WM_GETMINMAXINFO] = &BaseDialog::OnGetMinMaxInfo;
    return map;
}

//窗口创建时候
HRESULT BaseDialog::OnCreate(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //获取当前窗口风格
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    //设置STYLE
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    //初始化界面渲染器
    m_pm.Init(m_hWnd);
    //D
    CDialogBuilder builder;
    //通过xml 以及渲染器渲染界面UI
    CControlUI* pRoot = builder.Create(_T("main_dlg.xml"), (UINT)0, NULL, &m_pm);
    //附加界面UI到对话框容器
    m_pm.AttachDialog(pRoot);
    //增加消息处理  因为实现了 INotifyUI接口
    m_pm.AddNotifier(this);
    return 0;
}


HRESULT BaseDialog::OnDestory(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //发送退出消息
    ::PostQuitMessage(0L);
    return 0;
}

//擦除背景
HRESULT BaseDialog::OnErasebkgnd(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 1;
}

//当窗口的尺寸发生改编的时候
HRESULT BaseDialog::OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //在窗口大小改变的时候设置圆角
    SIZE szRoundCorner = m_pm.GetRoundCorner();

    if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++;
        rcWnd.bottom++;
        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right,
                                         rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
        ::SetWindowRgn(*this, hRgn, TRUE);
        ::DeleteObject(hRgn);
        return 0;
    }

    bHandled = FALSE;
    return 0;
}

HRESULT BaseDialog::OnNcPaint(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

HRESULT BaseDialog::OnNcActive(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (::IsIconic(*this)) bHandled = FALSE;

    return (wParam == 0) ? TRUE : FALSE;
}


//如果不处理那么就会导致DUILIB 不停调用系统消息进行处理
// 屏蔽系统标题栏 似乎不屏蔽一定会出问题
HRESULT BaseDialog::OnNcCalSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

HRESULT BaseDialog::OnNcHitTest(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //获取客户区相对坐标
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);
    //获取客户区域
    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    //如果窗口没有最大化
    if (!::IsZoomed(*this)) {
        //获取非客户区域sizebox
        RECT rcSizeBox = m_pm.GetSizeBox();

        //如果y<窗口区域top+sizebox.top 算上非客户区
        if (pt.y < rcClient.top + rcSizeBox.top) {
            //判断是否在左上边
            if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;

            //判断是否在右上边
            if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;

            //返回顶端测试
            return HTTOP;
        }
        //否则在bottom
        else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
            //左下
            if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;

            //右下
            if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;

            //默认下边
            return HTBOTTOM;
        }

        //如果不再 top 或者bottom 那么就是在左边 右边的非客户区
        if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;

        if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
    }

    //获取标题栏的矩形区域
    //并且判断鼠标是否在该区域中 如果在返回
    RECT rcCaption = m_pm.GetCaptionRect();

    if (pt.x >= rcClient.left + rcCaption.left
        && pt.x < rcClient.right - rcCaption.right
        && pt.y >= rcCaption.top
        && pt.y < rcCaption.bottom) {
        //ButtonUI OptionUI 只有这两种类型当作标题栏
        CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));

        if (pControl
            && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0
            && _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0) {
            return HTCAPTION;
        }
    }

    //其余部分是客户区
    return HTCLIENT;
}


//系统命令处理
LRESULT BaseDialog::OnSysCommand(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == SC_CLOSE) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }

    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(WM_SYSCOMMAND, wParam, lParam);
    return 1L;
}

LRESULT BaseDialog::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
    rcWork.Offset(-rcWork.left, -rcWork.top);
    /// 窗口最大化时裁剪阴影所占区域
    LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
    lpMMI->ptMaxPosition.x  = rcWork.left - 5;
    lpMMI->ptMaxPosition.y  = rcWork.top - 3;
    lpMMI->ptMaxSize.x      = rcWork.right + 10;
    lpMMI->ptMaxSize.y      = rcWork.bottom + 10;
    bHandled = FALSE;
    return 0;
}