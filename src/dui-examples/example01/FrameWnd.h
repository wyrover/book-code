#pragma once

class CFrameWnd: public WindowImplBase
{
public:
    explicit CFrameWnd(LPCTSTR pszXMLPath);

    LPCTSTR GetWindowClassName() const;
    CDuiString GetSkinFile();

    void InitWindow();
    void Notify(TNotifyUI& msg);
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    // CControlUI* CreateControl(LPCTSTR pstrClassName);
private:
    CDuiString      m_strXMLPath;

    map<CDuiString, bool> m_MenuCheckInfo; //保存菜单的单选复选信息
};