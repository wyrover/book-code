
#define APPNAME                 _T("Share Enum")
#define WM_APP_ENUM_COMPLETE    (WM_APP+0)

#define MAX_UNCPATH                 1024

const DWORD MAX_ACCOUNTS_PER_ACL    = 1024;
const DWORD MAX_TEXT                = 30 * MAX_ACCOUNTS_PER_ACL;

extern INT_PTR CALLBACK CompareDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam) ;

extern void RegeditJump(HWND hWnd, const TCHAR * path);



class CUser
{
public:
    TCHAR   *   m_Acct;
    CUser   *   m_Next;

    CUser(const TCHAR * text, int len)
    {
        while (len > 0  &&  iswspace(text[len - 1]))
            --len;

        m_Acct = new TCHAR[ len + 1 ];
        _tcsncpy(m_Acct, text, len);
        m_Acct[ len ] = 0;
        m_Next = NULL;
    }

    ~CUser()
    {
        delete m_Acct;
        delete m_Next;
    }

    void InsertInList(CUser ** list)
    {
        while (*list  &&  _tcsicmp(this->m_Acct, (*list)->m_Acct) > 0)
            list = &(*list)->m_Next;

        this->m_Next = *list;
        *list = this;
    }

    static CUser * MakeList(const TCHAR * list, TCHAR delim)
    {
        CUser * head = NULL;

        for (;;)  {
            while (*list == ' ')
                ++list;

            if (*list == 0)
                break;

            const TCHAR * next = _tcschr(list, delim);

            if (next == NULL)
                next = _tcschr(list, 0);

            CUser * newitem = new CUser(list, next - list);
            newitem->InsertInList(&head);

            if (*next == 0)
                break;

            list = next + 1;
        }

        return head;
    }
};

class CShare
{
public:
    TCHAR   *   m_path;
    CUser   *   m_OtherRead;
    CUser   *   m_OtherWrite;
    CUser   *   m_deny;
    CShare  *   m_Next;

    CShare(const TCHAR * path,
           const TCHAR * OtherRead, const TCHAR * OtherWrite, const TCHAR * deny, TCHAR delim)
    {
        m_Next          = NULL;
        m_path          = _tcsdup(path);
        m_OtherWrite    = CUser::MakeList(OtherWrite, delim);
        m_OtherRead     = CUser::MakeList(OtherRead, delim);
        m_deny          = CUser::MakeList(deny, delim);
    }

    ~CShare()
    {
        delete m_path;
        delete m_OtherWrite;
        delete m_OtherRead;
        delete m_deny;
        delete m_Next;
    }

    void InsertInList(CShare ** list)
    {
        while (*list  &&  _tcsicmp(this->m_path, (*list)->m_path) > 0)
            list = &(*list)->m_Next;

        m_Next = *list;
        *list = this;
    }
};
