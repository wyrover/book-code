#ifndef BaseDialog_H
#define BaseDialog_H

#include "UIlib.h"

#include <map>
using namespace DuiLib;

//继承自CWindowWnd  INotifyUI
//拥有创建窗口和接受窗口事件通知的能力
class BaseDialog : public CWindowWnd, public INotifyUI
{

public:
    BaseDialog() {};
protected:
    //虚函数 用于设置窗口的CLASS NAME
    virtual LPCTSTR GetWindowClassName() const
    {
        return _T("USHER TEST DLG");
    }
    //用于时间通告消息回调
    virtual void Notify(TNotifyUI& msg);
    //处理
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    // 自定义消息映射
    typedef HRESULT(BaseDialog::*CustomMsgHandler)(WPARAM, LPARAM, BOOL&);
    typedef std::map<UINT, CustomMsgHandler> MessageMap;
    virtual MessageMap* InitMessageMap();
    // 自定义消息处理 在窗口创建的时候
    HRESULT OnCreate(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //在窗口销毁的时候
    HRESULT OnDestory(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //擦除窗口背景
    HRESULT OnErasebkgnd(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //尺寸改变的时候
    HRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //非客户区重绘
    HRESULT OnNcPaint(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //非客户区激活
    HRESULT OnNcActive(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //非客户区计算大小
    HRESULT OnNcCalSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //非客户区点击测试
    HRESULT OnNcHitTest(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //系统命令处理
    LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //最大最小化信息
    LRESULT OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    CPaintManagerUI m_pm;
};
#endif // BaseDialog_H