#define _WIN32_WINNT 0x0400     // WM_MOUSEWHEEL support
#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <tchar.h>
#include <ctype.h>
#include <Aclapi.h>
#include <crtdbg.h>     // ASSERT
#include <comdef.h>     // bstr_t support
#include <lm.h>
#include <ShlObj.h>
#include <ShellAPI.h>
#include <CommDlg.h>

#include <tchar.h>
#include "AccessEnum.h"
#include "resource.h"
#include "resizer.h"
#include "listview.h"
#include "Enumeration.h"
#include "registry.h"


PSID DomainAdminSid();
bool IsDomainAdmin();
int GetAccountName(TCHAR * buf, const TCHAR * host, PSID sid);


const TCHAR ALL_DOMAINS[] = _T("<All domains>");

struct _globals {
    HINSTANCE           hInst;
    HWND                Abort;      // 'abort in progress' window
    DWORD               ShowAllFiles;
    DWORD               ShowLocalSystemAccount;
    CShare          *   ShareList;
    long                ThreadCount;
    HWND                hMainDlg;
    _bstr_t             Exclude;
} g;

REG_SETTING RegSettings[] = {
    { _T("ShowAllFiles"), SETTING_TYPE_DWORD, sizeof g.ShowAllFiles , &g.ShowAllFiles, 0 },
    { _T("ShowLocalSystem"), SETTING_TYPE_DWORD, sizeof g.ShowLocalSystemAccount , &g.ShowLocalSystemAccount, 0 },
    { NULL, SETTING_TYPE_DWORD, 0, NULL, 0 }
};




const DWORD CFileEnumeration::READ_MASK     =   GENERIC_ALL | GENERIC_READ | GENERIC_EXECUTE | READ_CONTROL | FILE_READ_ATTRIBUTES | FILE_READ_DATA | FILE_READ_EA;
const DWORD CFileEnumeration::WRITE_MASK    =   GENERIC_ALL | GENERIC_WRITE | DELETE | WRITE_DAC | WRITE_OWNER | FILE_APPEND_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_DATA | FILE_WRITE_EA;

const DWORD CRegEnumeration::READ_MASK      =   GENERIC_ALL | GENERIC_READ | GENERIC_EXECUTE | READ_CONTROL | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY;
const DWORD CRegEnumeration::WRITE_MASK     =   GENERIC_ALL | GENERIC_WRITE | DELETE | WRITE_DAC | WRITE_OWNER | KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_CREATE_LINK;


// Everyone
static const SID    SID_EVERYONE            = { 1, 1, SECURITY_WORLD_SID_AUTHORITY,     SECURITY_NULL_RID                   };
// creator owner
static const SID    SID_CREATOR_OWNER       = { 1, 1, SECURITY_CREATOR_SID_AUTHORITY,   SECURITY_CREATOR_OWNER_RID          };
static const SID    SID_CREATOR_GROUP       = { 1, 1, SECURITY_CREATOR_SID_AUTHORITY,   SECURITY_CREATOR_GROUP_RID          };
static const SID    SID_CREATOR_OWNERSERVER = { 1, 1, SECURITY_CREATOR_SID_AUTHORITY,   SECURITY_CREATOR_OWNER_SERVER_RID   };
static const SID    SID_CREATOR_GROUPSERVER = { 1, 1, SECURITY_CREATOR_SID_AUTHORITY,   SECURITY_CREATOR_GROUP_SERVER_RID   };
// local system
static const SID    SID_NTAUTHORITY_SYSTEM  = { 1, 1, SECURITY_NT_AUTHORITY,            SECURITY_LOCAL_SYSTEM_RID           };
// alias sids
#if 0
static const SID    SID_BUILTIN_ADMIN       = { 1, 1, SECURITY_BUILTIN_DOMAIN_RID,      DOMAIN_ALIAS_RID_ADMINS             };
static const SID    SID_BUILTIN_POWERUSERS  = { 1, 1, SECURITY_BUILTIN_DOMAIN_RID,      DOMAIN_ALIAS_RID_POWER_USERS        };
static const SID    SID_BUILTIN_GUESTS      = { 1, 1, SECURITY_BUILTIN_DOMAIN_RID,      DOMAIN_ALIAS_RID_GUESTS             };
static const SID    SID_BUILTIN_USERS       = { 1, 1, SECURITY_BUILTIN_DOMAIN_RID,      DOMAIN_ALIAS_RID_USERS              };
#endif



static struct LISTVIEW_COLUMN Columns[] = {
    { TEXT("Path"),         240,    DATATYPE_TEXT   },
    { TEXT("Read"),         120,    DATATYPE_TEXT   },
    { TEXT("Write"),        120,    DATATYPE_TEXT   },
    { TEXT("Deny"),         80,     DATATYPE_TEXT   },
    { NULL,                 0, (DATATYPE) - 1    }
};



// Power Users
PSID SidLocalPowerUserAlias()
{
    static PSID sid = NULL;

    if (sid == NULL)  {
        SID_IDENTIFIER_AUTHORITY    SystemSidAuthority = SECURITY_NT_AUTHORITY;
        AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_POWER_USERS,
                                 0, 0, 0, 0, 0, 0, &sid);
    }

    return sid;
}

// None
PSID SidNone()
{
    static PSID sid = NULL;

    if (sid == NULL)  {
        SID_IDENTIFIER_AUTHORITY    SystemSidAuthority = SECURITY_NT_AUTHORITY;
        AllocateAndInitializeSid(&SystemSidAuthority, 5, SECURITY_NT_NON_UNIQUE, 0x56252AE1, 0xAEF28A35, 0x7D8EE907, 0x00000201, 0, 0, 0, &sid);
    }

    return sid;
}

// Local Administrators
PSID SidLocalAdminAlias()
{
    static PSID sid = NULL;

    if (sid == NULL)  {
        SID_IDENTIFIER_AUTHORITY ntauth = SECURITY_NT_AUTHORITY;
        AllocateAndInitializeSid(&ntauth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                                 0, 0, 0, 0, 0, 0, &sid);
    }

    return sid;
}


const TCHAR * FormattedMessage(DWORD status)
{
    static TCHAR msg[ 1024 ];
    msg[0] = 0;
    FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                  status, 0, msg, sizeof msg / sizeof msg[0], NULL);

    if (msg[0] != 0)  {
        TCHAR * ptr = _tcschr(msg, 0);

        while (ptr > msg  &&  isspace(*--ptr))
            *ptr = 0;
    } else {
        _stprintf(msg, _T("Error %d"), status);
    }

    return msg;
}


bool ShowFileProperties(HWND hDlg, const TCHAR * Path)
{
    SHELLEXECUTEINFO    shellExecuteInfo = { 0 };
    shellExecuteInfo.cbSize = sizeof shellExecuteInfo;
    shellExecuteInfo.fMask  = SEE_MASK_INVOKEIDLIST;
    shellExecuteInfo.hwnd   = hDlg;
    shellExecuteInfo.lpVerb = _T("properties");
    shellExecuteInfo.lpFile = Path;
    return ShellExecuteEx(&shellExecuteInfo) != FALSE;
}

bool ExploreFile(HWND hDlg, const TCHAR * Path)
{
    if (_tcsnicmp(Path, _T("HK"), 2) == 0)  {
        // registry key
        RegeditJump(hDlg, Path);
        return true;
    } else {
        // file
        SHELLEXECUTEINFO    shellExecuteInfo = { 0 };
        shellExecuteInfo.cbSize = sizeof shellExecuteInfo;
        shellExecuteInfo.fMask  = SEE_MASK_FLAG_DDEWAIT;
        shellExecuteInfo.hwnd   = hDlg;
        shellExecuteInfo.lpVerb = _T("explore");
        shellExecuteInfo.lpFile = Path;
        shellExecuteInfo.nShow  = SW_SHOWNORMAL;
        BOOL ok = ShellExecuteEx(&shellExecuteInfo);
        return ok == TRUE;
    }
}




TCHAR * GetTextualSid(PSID pSid)
{
    PSID_IDENTIFIER_AUTHORITY psia;
    DWORD dwSubAuthorities;
    DWORD dwSidRev = SID_REVISION;
    DWORD dwCounter;
    DWORD dwSidSize;

    // Validate the binary SID.
    if (! IsValidSid(pSid))
        return NULL;

    // Get the identifier authority value from the SID.
    psia = GetSidIdentifierAuthority(pSid);
    // Get the number of subauthorities in the SID.
    dwSubAuthorities = *GetSidSubAuthorityCount(pSid);
    // Compute the buffer length.
    // S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
    dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);
    // Check input buffer length.
    // If too small, indicate the proper size and set last error.
    TCHAR   *   TextualSid = new TCHAR[ dwSidSize ];
    // Add 'S' prefix and revision number to the string.
    dwSidSize = wsprintf(TextualSid, TEXT("S-%lu-"), dwSidRev);

    // Add SID identifier authority to the string.
    if ((psia->Value[0] != 0) || (psia->Value[1] != 0))  {
        dwSidSize += wsprintf(TextualSid + dwSidSize,
                              TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
                              (USHORT)psia->Value[0],
                              (USHORT)psia->Value[1],
                              (USHORT)psia->Value[2],
                              (USHORT)psia->Value[3],
                              (USHORT)psia->Value[4],
                              (USHORT)psia->Value[5]);
    } else {
        dwSidSize += wsprintf(TextualSid + dwSidSize,
                              TEXT("%lu"),
                              (ULONG)(psia->Value[5])   +
                              (ULONG)(psia->Value[4] <<  8)   +
                              (ULONG)(psia->Value[3] << 16)   +
                              (ULONG)(psia->Value[2] << 24));
    }

    // Add SID subauthorities to the string.
    //
    for (dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)  {
        dwSidSize += wsprintf(TextualSid + dwSidSize, TEXT("-%lu"), *GetSidSubAuthority(pSid, dwCounter));
    }

    return TextualSid;
}




struct ACCOUNT {
    PSID                                Sid;
    const TCHAR                     *   Name;
    const GROUP_USERS_INFO_0        *   GroupNetList;
    DWORD                               GroupNetCnt;
    const LOCALGROUP_USERS_INFO_0   *   GroupLocalList;
    DWORD                               GroupLocalCnt;
    SID_NAME_USE                        Use;
};
#define UNDEFINED_ACCOUNT   ((ACCOUNT *)-1)


const ACCOUNT * LookupSid(PSID pSid, const TCHAR * host)
{
    static  ACCOUNT **  AccountList = NULL;
    static  DWORD       AccountCnt  = 0;
    static  DWORD       AccountMax  = 0;

    // Check if sid is one we've already looked up and cached.
    for (DWORD idx = 0; idx < AccountCnt; ++idx)
        if (EqualSid(pSid, AccountList[idx]->Sid))
            return AccountList[idx];

    // Its a new sid, so create a new account structure for it
    ACCOUNT *   act = new ACCOUNT;
    // get copy of sid
    DWORD   sidlen = GetLengthSid(pSid);
    act->Sid = new BYTE[ sidlen ];
    memcpy(act->Sid, pSid, sidlen);
    // Get account name for sid.
    TCHAR       domainName[MAX_PATH + 1 + 2] = _T("\\\\");
    TCHAR       userName[MAX_PATH + 1];
    DWORD       userLength = MAX_PATH;
    DWORD       domainLength = MAX_PATH;
    TCHAR       fullName[ 2 * MAX_PATH ];

    if (LookupAccountSid(host, pSid, userName, &userLength, domainName + 2, &domainLength, &act->Use))  {
        // found account on remote system
        switch (act->Use)  {
        case SidTypeAlias:
            _stprintf(fullName, _T("%s"), userName);
            break;

        case SidTypeUser:
            _stprintf(fullName, _T("%s\\%s"), domainName + 2, userName);
            break;

        case SidTypeDeletedAccount:
            _stprintf(fullName, _T("%s\\%s(Deleted)"), domainName + 2, userName);
            break;

        default:
            _ASSERT(false);

        // fall through
        case SidTypeGroup:
        case SidTypeWellKnownGroup:
            if (domainName[2])
                _stprintf(fullName, _T("%s\\%s"), domainName + 2, userName);
            else
                _stprintf(fullName, _T("%s"), userName);

            break;
        }

        act->Name = _tcsdup(fullName);
    } else if (host  &&  LookupAccountSid(NULL, pSid, userName, &userLength, domainName + 2, &domainLength, &act->Use))  {
        _ASSERT(false);

        // found account on local system
        switch (act->Use)  {
        case SidTypeAlias:
            _stprintf(fullName, _T("%s"), userName);
            break;

        case SidTypeUser:
            _stprintf(fullName, _T("%s\\%s"), domainName + 2, userName);
            break;

        case SidTypeDeletedAccount:
            _stprintf(fullName, _T("%s\\%s(Deleted)"), domainName + 2, userName);
            break;

        default:
            _ASSERT(false);

        // fall through
        case SidTypeGroup:
        case SidTypeWellKnownGroup:
            if (domainName[2])
                _stprintf(fullName, _T("%s\\%s"), domainName + 2, userName);
            else
                _stprintf(fullName, _T("%s"), userName);

            break;
        }

        act->Name = _tcsdup(fullName);
    } else {
        if (EqualSid(pSid, SidLocalPowerUserAlias()))  {
            _tcscpy(fullName, _T("Power Users"));
            act->Name = _tcsdup(fullName);
        } else {
            // Use plain SID
            act->Name   = GetTextualSid(pSid);
            act->Use    = SidTypeUnknown;
            fullName[0] = 0;
        }
    }

    act->GroupNetList   = NULL;
    act->GroupLocalList = NULL;
    act->GroupNetCnt    = 0;
    act->GroupLocalCnt  = 0;

    if (act->Use == SidTypeUser)  {
        NET_API_STATUS  status;
        // see what groups account belongs to
        DWORD   total = 0;
        status = NetUserGetGroups(domainName, userName, 0, (BYTE **)&act->GroupNetList, MAX_PREFERRED_LENGTH, &act->GroupNetCnt, &total);
        _ASSERT(status == 0 || status == ERROR_ACCESS_DENIED);   // ERROR_INVALID_FUNCTION, NERR_InvalidComputer
        total = 0;
        TCHAR server[ MAX_PATH ];

        if (host)  {
            _stprintf(server, _T("\\\\%s"), host);
            host = server;
        }

        status = NetUserGetLocalGroups(host, fullName, 0, LG_INCLUDE_INDIRECT, (BYTE **)&act->GroupLocalList, MAX_PREFERRED_LENGTH, &act->GroupLocalCnt, &total);

        if (status == ERROR_NO_SUCH_DOMAIN)  {
            // if it complains about the domain try removing that portion of the name
            if (host  &&  _tcsicmp(domainName, host) == 0)
                status = NetUserGetLocalGroups(host, userName, 0, LG_INCLUDE_INDIRECT, (BYTE **)&act->GroupLocalList, MAX_PREFERRED_LENGTH, &act->GroupLocalCnt, &total);
        }

        _ASSERT(status == 0 || status == ERROR_ACCESS_DENIED);   // ERROR_INVALID_FUNCTION, NERR_InvalidComputer, ERROR_NO_SUCH_DOMAIN
    }

    // add account to list
    if (AccountCnt >= AccountMax)  {
        AccountMax = AccountMax ? 2 * AccountMax : 1024;
        AccountList = (ACCOUNT **)realloc(AccountList, AccountMax * sizeof AccountList[0]);
    }

    AccountList[ AccountCnt++ ] = act;
    return act;
}


struct ACCOUNT_RIGHTS {
    const ACCOUNT   *   Account;
    DWORD               AllowMask;
    DWORD               DenyMask;
};

inline int CompareAccount(const ACCOUNT * pa, const ACCOUNT * pb)
{
    if (pa == pb)
        return 0;

    return _tcsicmp(pa->Name, pb->Name);
}
int __cdecl CompareAccountRights(const void * pa, const void * pb)
{
    return CompareAccount(((const ACCOUNT_RIGHTS *)pa)->Account, ((const ACCOUNT_RIGHTS *)pb)->Account);
}

class CEnumeration;
bool GetCannonicalRights(const TCHAR * host, const PSECURITY_DESCRIPTOR sd, const CEnumeration * root, const ACCOUNT * Read[], const ACCOUNT * Write[], const ACCOUNT * Deny[])
{
    BOOL    present, defaulted;
    PACL    acl = NULL;

    if (host  &&  host[0] == 0)
        host = NULL;

    // Get DACL
    if (!IsValidSecurityDescriptor(sd)  ||
        !GetSecurityDescriptorDacl(sd, &present, &acl, &defaulted)  ||
        !present) {
        // can't say anything about the rights
        Read[0]     = UNDEFINED_ACCOUNT;
        Write[0]    = UNDEFINED_ACCOUNT;
        Deny[0]     = UNDEFINED_ACCOUNT;
        Read[1]     = NULL;
        Write[1]    = NULL;
        Deny[1]     = NULL;
        return false;
    }

    if (acl == NULL)  {
        // NULL dacl grants full access to everyone
        Read[0]     = LookupSid((PSID)&SID_EVERYONE, host);
        Write[0]    = Read[0];
        Deny[0]     = NULL;
        Read[1]     = NULL;
        Write[1]    = NULL;
        Deny[1]     = NULL;
        return true;
    }

    // First we'll get a list of all accounts and the rights each has.
    ACCOUNT_RIGHTS  AccountList[ MAX_ACCOUNTS_PER_ACL ];
    DWORD           AccountCnt = 0;

    // Iterate over aces and acculate a list of accounts
    for (int idx = 0; idx < acl->AceCount; ++idx)  {
        ACCESS_ALLOWED_ACE * ace;
        // Get the ace
        GetAce(acl, idx, (void **)&ace);

        if (ace->Header.AceType != ACCESS_ALLOWED_ACE_TYPE  &&
            ace->Header.AceType != ACCESS_DENIED_ACE_TYPE) {
            continue;
        }

        if (ace->Header.AceFlags & INHERIT_ONLY_ACE)
            continue;

        // Get SID for ace
        PSID pSid = &ace->SidStart;

        if (! IsValidSid(pSid))
            continue;

        // translate CREATOR_OWNER sid to owner sid
        if (EqualSid(pSid, (PSID)&SID_CREATOR_OWNER))  {
            PSID    pOwner = NULL;
            BOOL    defaulted;

            if (! GetSecurityDescriptorOwner(sd, &pOwner, &defaulted))
                continue;

            if (pOwner == NULL)
                continue;   // no owner, therefore no permissions to report

            pSid = pOwner;
        } else if (EqualSid(pSid, (PSID)&SID_CREATOR_GROUP))  {
            PSID    pOwner = NULL;
            BOOL    defaulted;

            if (! GetSecurityDescriptorGroup(sd, &pOwner, &defaulted))
                continue;

            if (pOwner == NULL)
                continue;   // no owner, therefore no permissions to report

            pSid = pOwner;
        }

        // Skip LocalSystem account
        if (!g.ShowLocalSystemAccount  &&  EqualSid(pSid, (PSID)&SID_NTAUTHORITY_SYSTEM))  {
            // who cares?
            continue;
        }

        // Check if SID already is in our list
        DWORD i;

        for (i = 0; i < AccountCnt; ++i)
            if (EqualSid(pSid, AccountList[i].Account->Sid))
                break;

        // Create new list entry if not already present
        if (i >= AccountCnt)  {
            if (AccountCnt + 1 >= MAX_ACCOUNTS_PER_ACL)  {
                MessageBox(g.hMainDlg, _T("Internal error: Too many accounts in ACL"), APPNAME, MB_OK | MB_ICONSTOP);
                return false;
            }

            ++AccountCnt;
            AccountList[i].Account      = LookupSid(pSid, host);
            AccountList[i].AllowMask    = 0;
            AccountList[i].DenyMask     = 0;
        }

#if 0

        // Skip None group
        if (EqualSid(pSid, SidNone()))  {
            // who cares?
            continue;
        }

#endif

        // Merge rights for this ace into existing account rights
        // (the other ace types were filtered out above)
        if (ace->Header.AceType == ACCESS_ALLOWED_ACE_TYPE)
            AccountList[i].AllowMask |= ace->Mask;
        else
            AccountList[i].DenyMask  |= ace->Mask;
    }

    DWORD actIdx;

    // Convert rights for each account to generic rights. This
    // simplifies the bit arithmetic we do later.
    for (actIdx = 0; actIdx < AccountCnt; ++actIdx)  {
        DWORD   & mask = AccountList[actIdx].AllowMask;
        mask = (root->AllowRead(mask)  ? GENERIC_READ  : 0)
               | (root->AllowWrite(mask) ? GENERIC_WRITE : 0);
    }

#if 1
    // Check group membership for each account and
    // subtract rights granted by group(s) from account rights.

    for (actIdx = 0; actIdx < AccountCnt; ++actIdx)  {
        for (DWORD other = 0; other < AccountCnt; ++other)  {
            // make sure other item is a group, and not us
            if (actIdx == other)
                continue;

            if (AccountList[other].Account->Use != SidTypeGroup         &&
                AccountList[other].Account->Use != SidTypeWellKnownGroup   &&
                AccountList[other].Account->Use != SidTypeAlias)
                continue;

            // If Everyone is present then always assume membership
            if (EqualSid(AccountList[other].Account->Sid, (PSID)&SID_EVERYONE))  {
                // Everyone is a member of Everyone
                AccountList[actIdx].AllowMask &= ~AccountList[other].AllowMask;
                AccountList[actIdx].DenyMask  &= ~AccountList[other].DenyMask;
                continue;
            }

            DWORD actGrp;

            // check each of our groups to see if we belong to other
            for (actGrp = 0; actGrp < AccountList[actIdx].Account->GroupLocalCnt; ++actGrp)  {
                if (_tcsicmp(AccountList[actIdx].Account->GroupLocalList[actGrp].lgrui0_name,
                             AccountList[other].Account->Name) == 0) {
                    // There is another group that we are a member of.
                    // Subtract any rights they grant from our rights.
                    AccountList[actIdx].AllowMask &= ~AccountList[other].AllowMask;
                    AccountList[actIdx].DenyMask  &= ~AccountList[other].DenyMask;
                }
            }

            for (actGrp = 0; actGrp < AccountList[actIdx].Account->GroupNetCnt; ++actGrp)  {
                if (_tcsicmp(AccountList[actIdx].Account->GroupNetList[actGrp].grui0_name,
                             AccountList[other].Account->Name) == 0) {
                    // There is another group that we are a member of
                    // Subtract any rights they grant from our rights.
                    AccountList[actIdx].AllowMask &= ~AccountList[other].AllowMask;
                    AccountList[actIdx].DenyMask  &= ~AccountList[other].DenyMask;
                }
            }
        }
    }

#endif
    // sort list of accounts
    qsort(AccountList, AccountCnt, sizeof AccountList[0], CompareAccountRights);
    // Generate lists of the accounts with Read, Write and Deny rights present.
    DWORD   ReadCnt     = 0;
    DWORD   WriteCnt    = 0;
    DWORD   DenyCnt     = 0;

    for (actIdx = 0; actIdx < AccountCnt; ++actIdx)  {
        if (AccountList[actIdx].AllowMask & GENERIC_READ)  {
            Read[ReadCnt++] = AccountList[actIdx].Account;
        }

        if (AccountList[actIdx].AllowMask & GENERIC_WRITE)  {
            Write[WriteCnt++] = AccountList[actIdx].Account;
        }

        if (AccountList[actIdx].DenyMask)  {
            Deny[DenyCnt++] = AccountList[actIdx].Account;
        }
    }

    // set null terminator
    Read[ReadCnt]   = NULL;
    Write[WriteCnt] = NULL;
    Deny[DenyCnt]   = NULL;
    return true;
}


// Both account lists must be sorted
bool AccountListEqual(const ACCOUNT * Account1[], const ACCOUNT * Account2[])
{
    if (Account1[0] == UNDEFINED_ACCOUNT  ||  Account2[0] == UNDEFINED_ACCOUNT)  {
        // if either is undefined, then the undefined one is the superset
        return Account2[0] == Account1[0];
    }

    int idx;

    // Compare each account in turn, using the same comparison function used to sort the lists.
    for (idx = 0; Account1[idx] && Account2[idx]; ++idx)  {
        int diff = CompareAccount(Account1[idx], Account2[idx]);

        if (diff)
            return false;
    }

    return Account1[idx] == Account2[idx];
}

bool AccountListSubset(const ACCOUNT * Sub[], const ACCOUNT * Super[])
{
    // if either is undefined, then the undefined one is the superset
    if (Sub[0] == UNDEFINED_ACCOUNT)
        return Super[0] == UNDEFINED_ACCOUNT;

    if (Super[0] == UNDEFINED_ACCOUNT)
        return false;

    int iSub;
    int iSuper;

    // Compare each account in turn, using the same comparison function used to sort the lists.
    for (iSub = 0, iSuper = 0; Sub[iSub] && Super[iSuper];)  {
        int diff = CompareAccount(Sub[iSub], Super[iSuper]);

        if (diff == 0)  {
            ++iSub;
            ++iSuper;
        } else if (diff > 0)  {
            ++iSuper;
        } else {
            return false;
        }
    }

    return Sub[iSub] == NULL;
}



int AddListviewRow(HWND hListview, const TCHAR * path,
                   const TCHAR * OtherRead, const TCHAR * OtherWrite,
                   const TCHAR * deny)
{
    CShare * share = new CShare(path, OtherRead, OtherWrite, deny, ',');
    share->InsertInList(&g.ShareList);
    // get icon
    int iImage;

    if (_tcsnicmp(path, _T("HK"), 2) == 0)  {
        // registry key
        HICON       hIcon = LoadIcon(g.hInst, MAKEINTRESOURCE(IDI_FOLDERCLOSED));
        HIMAGELIST  hImageList = ListView_GetImageList(hListview, LVSIL_SMALL);
        iImage = ImageList_AddIcon(hImageList, hIcon);
        DestroyIcon(hIcon);
    } else {
        // file
        SHFILEINFO  info = { 0 };
        SHGetFileInfo(path, 0, &info, sizeof info, SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
        HIMAGELIST hImageList = ListView_GetImageList(hListview, LVSIL_SMALL);
        iImage = ImageList_AddIcon(hImageList, info.hIcon);
        DestroyIcon(info.hIcon);
    }

    // Add to listview
    LVITEM  item;
    item.mask = LVIF_TEXT | LVIF_IMAGE;
    item.iItem = 0x7FFFFFFF;
    item.iSubItem = 0;
    item.iImage = iImage;
    item.pszText = (TCHAR *) path;
    item.iItem = ListView_InsertItem(hListview, &item);
    // set param to index, so we can look ourself up during sorting
    item.mask = LVIF_PARAM;
    item.lParam = item.iItem;
    ListView_SetItem(hListview, &item);
    item.mask = LVIF_TEXT;
    item.iSubItem += 1;
    item.pszText = (TCHAR *) OtherRead;
    ListView_SetItem(hListview, &item);
    item.iSubItem += 1;
    item.pszText = (TCHAR *) OtherWrite;
    ListView_SetItem(hListview, &item);
    item.iSubItem += 1;
    item.pszText = (TCHAR *) deny;
    ListView_SetItem(hListview, &item);
    return item.iItem;
}





//==============================================================
//
// EnumerateDomains
//
// Create a list of computer domains accessible to this system
//
//==============================================================

class CWorker
{
public:
    void Start()
    {
        InterlockedIncrement(&g.ThreadCount);
        UpdateStatus();
#if _MT
        DWORD   id;
        HANDLE  hThread = CreateThread(NULL, 0, Work, this, CREATE_SUSPENDED, &id);
        SetThreadPriority(hThread, THREAD_PRIORITY_LOWEST);
        ResumeThread(hThread);
#else
        Work();
#endif
    }

    virtual ~CWorker()
    {
    }

protected:
    void UpdateStatus()
    {
        TCHAR * msg = g.ThreadCount ? _T("Scanning...") : _T("");
        SetWindowText(GetDlgItem(g.hMainDlg, IDC_STATUS), msg);
    }

    static int ProcessException(int code, const EXCEPTION_POINTERS * ep)
    {
        TCHAR buf[ MAX_PATH ];
        _stprintf(buf, _T("An internal error occured: Exception #0x%X"), code);
        MessageBox(g.hMainDlg, buf, APPNAME, MB_OK);
        EXCEPTION_RECORD    *   ExceptionRecord = ep->ExceptionRecord;
        CONTEXT             *   ContextRecord   = ep->ContextRecord;
        return EXCEPTION_EXECUTE_HANDLER;
    }

    static DWORD WINAPI Work(void * This)
    {
        CWorker * obj = (CWorker *)This;

        __try {
            obj->Work();
        } __except (ProcessException(GetExceptionCode(), GetExceptionInformation()))  {
        }

        return 0;
    }

    virtual void Work()
    {
        if (InterlockedDecrement(&g.ThreadCount) == 0)  {
            // We're done enumerating
            PostMessage(g.hMainDlg, WM_APP_ENUM_COMPLETE, 0, 0);
        }

        UpdateStatus();
        delete this;
    }
};






class CTraversal : public CWorker
{
    HWND                    m_hListview;
    const bstr_t            m_Host;
    CEnumeration        *   m_Root;

#define MISSING_DACL ((PACL) 0xFFFFFFFF)

    inline bool EXPLICIT_DACL(PACL d)
    {
        return d != NULL  &&  d != MISSING_DACL;
    }

public:
    CTraversal(HWND hListview, const TCHAR * Host, CEnumeration * Root) :
        m_hListview(hListview), m_Host(Host), m_Root(Root)
    {
    }

    ~CTraversal()
    {
    }

private:

    void GetAccountNames(TCHAR  * Text, const ACCOUNT * Account[])
    {
        // Analyze DACL
        if (Account[0] == UNDEFINED_ACCOUNT)  {
            _tcscpy(Text, _T("???"));
        } else {
            // convert acl to text
            TCHAR * TextStart = Text;

            for (int i = 0; Account[i]; ++i)  {
                Text += _stprintf(Text, _T("%s, "), Account[i]->Name);
            }

            // Trim trailing commas in lists
            if (Text == TextStart)  {
                Text[0] = 0;
            } else {
                Text[-2] = 0;
            }
        }
    }

    void ShowAccounts(const TCHAR * Path, const ACCOUNT * Read[], const ACCOUNT * Write[], const ACCOUNT * Deny[])
    {
        // Get permissions
        TCHAR       szRead[ MAX_TEXT ];
        TCHAR       szWrite[ MAX_TEXT ];
        TCHAR       szDeny[ MAX_TEXT ];
        GetAccountNames(szRead,  Read);
        GetAccountNames(szWrite, Write);
        GetAccountNames(szDeny,  Deny);
        // Add to list view
        AddListviewRow(m_hListview, Path, szRead, szWrite, szDeny);
    }


    void Enumerate(CEnumeration * Root, const ACCOUNT * ParentRead[], const ACCOUNT * ParentWrite[], const ACCOUNT * ParentDeny[])
    {
        // If path is a subdirectory of Exclude then skip it
        if (!!g.Exclude)  {
            if (_tcsnicmp(Root->Path(), g.Exclude, _tcslen(g.Exclude)) == 0)
                return;
            else
                g.Exclude = (TCHAR *) NULL;
        }

#if 0

        if (_tcsicmp(Root->Path(), _T("HKLM\\Software\\Classes\\Installer\\Components")) == 0)
            __asm int 3;

#endif
        // Show progress
        SetDlgItemText(GetParent(m_hListview), IDC_PATH, Root->Path());
#if 0

        if (wcslen(Root->Path()) > 100)
            Path = (TCHAR *)9999999999;

#endif
        // Get security descriptor
        PSECURITY_DESCRIPTOR    sd = Root->GetSecurity();
        // Convert acl to rights list
        const ACCOUNT * Read[ MAX_ACCOUNTS_PER_ACL ];
        const ACCOUNT * Write[ MAX_ACCOUNTS_PER_ACL ];
        const ACCOUNT * Deny[ MAX_ACCOUNTS_PER_ACL ];
        GetCannonicalRights(m_Host, sd, Root, Read, Write, Deny);
        bool show;

        if (Root->IsDir()  ||  g.ShowAllFiles)  {
            // directory permissions must match exactly
            show =  !AccountListEqual(Read,  ParentRead)      ||
                    !AccountListEqual(Write, ParentWrite)      ||
                    !AccountListEqual(Deny,  ParentDeny);
        } else {
            // file permissions its okay to be a subset of parent
            show =  !AccountListSubset(Read,  ParentRead) ||
                    !AccountListSubset(Write, ParentWrite) ||
                    !AccountListSubset(Deny,  ParentDeny);
        }

        if (show)  {
            // Display permissions
            ShowAccounts(Root->Path(), Read, Write, Deny);
        }

        // Recurse if a directory
        if (Root->IsDir())  {
            DWORD   status;
            CEnumeration * Child = Root->FindNext(&status);

            if (status)  {
                // indicate error while reading children
                const TCHAR * msg = FormattedMessage(status);
                _bstr_t fullpath = Root->Path() + _T("\\*");
                AddListviewRow(m_hListview, fullpath, msg, _T(""), _T(""));
            } else {
                while (Child) {
                    if (g.Abort)
                        break;

                    // Compute full path and evaluate
#if 0

                    if (end - Path + _tcslen(info.cFileName) + 1 >= MAX_UNCPATH)  {
                        MessageBox(NULL, _T("Fatal error: MAX_UNCPATH exceeded"), APPNAME, MB_ICONSTOP);
                        exit(1);
                    }

                    _tcscpy(end + 1, info.cFileName);
#endif
#if 0
                    static TCHAR oldPath[ 2 * MAX_UNCPATH ];
                    _tcscpy(oldPath, Path);

                    if (_tcscmp(oldPath, _T("C:\\WINDOWS\\Help\\SBSI\\Training\\Database\\bookmrk.CDX")) == 0)
                        __asm int 3;

#endif
                    Enumerate(Child, Read, Write, Deny);
                    delete Child;
                    Child = Root->FindNext(&status);
                }
            }
        }

        delete sd;
    }

    void Work()
    {
        // Enumerate files in the directory
        bool    isDir = m_Root->IsDir();
        // Get permissions
        const ACCOUNT * Account[2] = { UNDEFINED_ACCOUNT, NULL };
        Enumerate(m_Root, Account, Account, Account);
        // restore original path
        SetDlgItemText(GetParent(m_hListview), IDC_PATH, m_Root->Path());
        CWorker::Work();
    } // Work

}; // CTraversal






bool ExportFile(HWND hListView, const TCHAR * path)
{
    HANDLE  hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    LV_ITEM     lvi;
    DWORD       nb;
    // Mark file as unicode
#if UNICODE
    WORD uni = 0xFEFF;
    WriteFile(hFile, &uni, sizeof uni, &nb, NULL);
#endif

    // print headers
    for (lvi.iSubItem = 0; lvi.iSubItem < sizeof(Columns) / sizeof(Columns[0]) - 1; ++lvi.iSubItem)  {
        WriteFile(hFile, _T("\""), sizeof(TCHAR), &nb, NULL);
        WriteFile(hFile, Columns[lvi.iSubItem].Title, _tcslen(Columns[lvi.iSubItem].Title)*sizeof(TCHAR), &nb, NULL);
        WriteFile(hFile, _T("\"\t"), 2 * sizeof(TCHAR), &nb, NULL);
    }

    WriteFile(hFile, _T("\r\n"), 2 * sizeof(TCHAR), &nb, NULL);
    // Get text
    int max = ListView_GetItemCount(hListView);

    for (lvi.iItem = 0; lvi.iItem < max; ++lvi.iItem)  {
        // Get item
        lvi.mask = TVIF_TEXT;

        for (lvi.iSubItem = 0; lvi.iSubItem < sizeof(Columns) / sizeof(Columns[0]) - 1; ++lvi.iSubItem)  {
            TCHAR Item[ MAX_TEXT ];
            lvi.pszText     = Item;
            lvi.cchTextMax  = MAX_TEXT;

            if (! ListView_GetItem(hListView, &lvi))
                *Item = 0;

#if 0

            for (int i = 0; Item[i]; ++i)
                if (Item[i] == ',' && Item[i + 1] == ' ')  {
                    Item[i] = ' ';
                    Item[i + 1] = 0x0A;
                }

#endif
            WriteFile(hFile, _T("\""), sizeof(TCHAR), &nb, NULL);
            WriteFile(hFile, Item, _tcslen(Item)*sizeof(TCHAR), &nb, NULL);
            WriteFile(hFile, _T("\"\t"), 2 * sizeof(TCHAR), &nb, NULL);
        }

        WriteFile(hFile, _T("\r\n"), 2 * sizeof(TCHAR), &nb, NULL);
    }

    CloseHandle(hFile);
    return true;
}




void PrintAllRecurse(CEnumeration * parent)
{
    PSECURITY_DESCRIPTOR sd = parent->GetSecurity();
    delete sd;

    for (;;)  {
        DWORD   status;
        CEnumeration * child = parent->FindNext(&status);

        if (child == NULL)
            break;

        PrintAllRecurse(child);
        delete child;
    }
}
void PrintAll()
{
    CFileEnumeration    file(_T("C:"), true);
    CRegEnumeration     reg(_T("SOFTWARE"), true);
    PrintAllRecurse(&reg);
    PrintAllRecurse(&file);
}








INT_PTR CALLBACK AbortDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    RECT    parentRc, childRc;

    switch (message)  {
    case WM_INITDIALOG:
        GetWindowRect(GetParent(hDlg), &parentRc);
        GetWindowRect(hDlg, &childRc);
        parentRc.left = ((parentRc.left + parentRc.right) - (childRc.right - childRc.left)) / 2;
        parentRc.top  = ((parentRc.top + parentRc.bottom) - (childRc.bottom - childRc.top)) / 2;
        MoveWindow(hDlg, parentRc.left, parentRc.top, childRc.right - childRc.left, childRc.bottom - childRc.top, TRUE);
        return TRUE;

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;

    default:
        break;
    }

    return FALSE;
}


INT_PTR CALLBACK AboutDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    RECT    parentRc, childRc;
    static HWND     hLink;
    static BOOL     underline_link;
    static HFONT    hFontNormal = NULL;
    static HFONT    hFontUnderline = NULL;
    static HCURSOR  hHandCursor = NULL;
    static HCURSOR  hRegularCursor;
    LOGFONT         logfont;

    switch (message)  {
    case WM_INITDIALOG:
        GetWindowRect(GetParent(hDlg), &parentRc);
        GetWindowRect(hDlg, &childRc);
        parentRc.left += 70;
        parentRc.top  += 60;
        MoveWindow(hDlg, parentRc.left, parentRc.top, childRc.right - childRc.left, childRc.bottom - childRc.top, TRUE);
        underline_link = TRUE;
        hLink = GetDlgItem(hDlg, IDC_LINK);
        // get link fonts
        hFontNormal = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
        GetObject(hFontNormal, sizeof logfont, &logfont);
        logfont.lfUnderline = TRUE;
        hFontUnderline = CreateFontIndirect(&logfont);
        // get hand
        hHandCursor = LoadCursor(g.hInst, TEXT("HAND"));
        hRegularCursor = LoadCursor(NULL, IDC_ARROW);
        return TRUE;

    case WM_CTLCOLORSTATIC:
        if ((HWND)lParam == hLink)  {
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);

            if (GetSysColorBrush(26/*COLOR_HOTLIGHT*/))
                SetTextColor(hdc, GetSysColor(26/*COLOR_HOTLIGHT*/));
            else
                SetTextColor(hdc, RGB(0, 0, 255));

            SelectObject(hdc, underline_link ? hFontUnderline : hFontNormal);
            return (LONG)GetSysColorBrush(COLOR_BTNFACE);
        }

        break;

    case WM_MOUSEMOVE: {
        POINT   pt = { LOWORD(lParam), HIWORD(lParam) };
        HWND    hChild = ChildWindowFromPoint(hDlg, pt);

        if (underline_link == (hChild == hLink))  {
            underline_link = !underline_link;
            InvalidateRect(hLink, NULL, FALSE);
        }

        if (underline_link)
            SetCursor(hRegularCursor);
        else
            SetCursor(hHandCursor);

        break;
    }

    case WM_LBUTTONDOWN: {
        POINT       pt = { LOWORD(lParam), HIWORD(lParam) };
        HWND        hChild = ChildWindowFromPoint(hDlg, pt);

        if (hChild == hLink)  {
            ShellExecute(hDlg, TEXT("open"), TEXT("http://www.sysinternals.com"), NULL, NULL, SW_SHOWNORMAL);
        }

        break;
    }

    case WM_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, 0);
            return TRUE;
        }

        break;

    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE;

    default:
        break;
    }

    return FALSE;
}

INT_PTR CALLBACK FilePermDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)  {
    case WM_INITDIALOG:
        CheckDlgButton(hDlg, IDYES, g.ShowAllFiles ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hDlg, IDNO, !g.ShowAllFiles ? BST_CHECKED : BST_UNCHECKED);
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam))  {
        case IDOK:
            g.ShowAllFiles = IsDlgButtonChecked(hDlg, IDYES) == BST_CHECKED;

        case IDCANCEL:
            EndDialog(hDlg, 0);
            break;
        }

        break;
    }

    return false;
}

INT_PTR CALLBACK HelpDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    static const TCHAR HelpText[] =
        _T("AccessEnum displays who has access to the files and folders within a directory. Although every ")
        _T("file/directory is examined, AccessEnum displays only those with permissions that differ from ")
        _T("their parent folder, allowing you to quickly determine deviations in your security policy.")
        _T("\r\n\r\n")
        _T("AccessEnum abstracts Window's access-control model to just Read, Write and Deny permissions. ")
        _T("A file is shown as granting Write permission whether it grants just a single write right (such ")
        _T("as Write Owner) or the full suite of write rights via Full Control. Read and Deny permissions ")
        _T("are handled similarly.")
        _T("\r\n\r\n")
        _T("When AccessEnum compares a file/folder and its parent to determine whether their permissions ")
        _T("are equivalent, it looks only at whether the same set of accounts are granted Read, Write and ")
        _T("Deny access respectively. If a file grants just Write Owner access, and its parent just Delete ")
        _T("access, the two will still be considered equivalent since both allow some form of writing.")
        _T("\r\n\r\n")
        _T("AccessEnum condenses the number of accounts displayed as having access to a file/folder by ")
        _T("hiding accounts with permissions that are duplicated by a group to which the account belongs. ")
        _T("For example, if a file grants Read access to both user Bob and group Marketing, and Bob is a ")
        _T("member of the Marketing group, then only Marketing will be shown in the list of accounts having ")
        _T("Read access.")
        _T("\r\n\r\n")
        _T("AccessEnum handles files slightly differently than folders. When comparing the permissions of a ")
        _T("file to its parent the file is displayed only when its permissions are less strict than the parent ")
        _T("folder. You can change this behavior using the Options menu.");

    switch (message)  {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_EDIT, HelpText);
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam))  {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, 0);
            break;
        }

        break;
    }

    return false;
}



_bstr_t GetTreePath(HWND hTree, HTREEITEM hItem)
{
    _bstr_t Path;

    for (;;)  {
        TVITEM  tvi;
        TCHAR   Name[ MAX_UNCPATH ];
        tvi.mask        = TVIF_TEXT;
        tvi.hItem       = hItem;
        tvi.pszText     = Name;
        tvi.cchTextMax  = MAX_UNCPATH;
        TreeView_GetItem(hTree, &tvi);
        Path = Name + Path;
        hItem = TreeView_GetParent(hTree, hItem);

        if (hItem == NULL)
            return Path;

        Path = _T("\\") + Path;
    }
}


INT_PTR CALLBACK BrowseRegDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    HWND                hTree;
    TVINSERTSTRUCT      tvi;
    int                 i;
    NMHDR           *   nmhdr;
    HIMAGELIST          hImageList;
    static _bstr_t  *   ReturnPath;
    static const TCHAR  *   Root[] = {
        _T("HKEY_CLASSES_ROOT"),
        _T("HKEY_CURRENT_USER"),
        _T("HKEY_LOCAL_MACHINE"),
        _T("HKEY_USERS"),
        _T("HKEY_CURRENT_CONFIG")
    };

    switch (message)  {
    case WM_INITDIALOG:
        new CResizer(hDlg);
        ReturnPath = (_bstr_t *) lParam;
        hTree = GetDlgItem(hDlg, IDC_TREE);
        // create images for nodes
        hImageList = ImageList_Create(16, 16, ILC_COLOR | ILC_MASK, 4, 4);
        ImageList_AddIcon(hImageList, LoadIcon(g.hInst, MAKEINTRESOURCE(IDI_FOLDERCLOSED)));
        ImageList_AddIcon(hImageList, LoadIcon(g.hInst, MAKEINTRESOURCE(IDI_FOLDEROPEN)));
        TreeView_SetImageList(hTree, hImageList, TVSIL_NORMAL);

        // add root entries
        for (i = 0; i < 5; ++i)  {
            tvi.hParent             = TVI_ROOT;
            tvi.hInsertAfter        = TVI_LAST;
            tvi.item.mask           = TVIF_TEXT | TVIF_CHILDREN | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvi.item.pszText        = (TCHAR *) Root[i];
            tvi.item.cChildren      = I_CHILDRENCALLBACK;
            tvi.item.iImage         = 0;
            tvi.item.iSelectedImage = 1;
            tvi.item.lParam         = false;    // not expanded yet
            TreeView_InsertItem(hTree, &tvi);
        }

        EnableWindow(GetDlgItem(hDlg, IDOK), false);
        return false;

    case WM_NOTIFY:
        nmhdr = (NMHDR *) lParam;

        if (nmhdr->code == TVN_SELCHANGED)  {
            EnableWindow(GetDlgItem(hDlg, IDOK), true);
        }

        if (nmhdr->code == TVN_GETDISPINFO)  {
            NMTVDISPINFO    *   tvnm = (NMTVDISPINFO *) nmhdr;

            if (tvnm->item.mask & TVIF_CHILDREN)  {
                // Get number of children in key
                _bstr_t Path = GetTreePath(GetDlgItem(hDlg, IDC_TREE), tvnm->item.hItem);
                HKEY hKey = CRegEnumeration::OpenPath(Path, NULL);
                tvnm->item.cChildren = 0;
                RegQueryInfoKey(hKey, NULL, NULL, NULL, (DWORD *)&tvnm->item.cChildren, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                RegCloseKey(hKey);
            }
        }

        if (nmhdr->code == TVN_ITEMEXPANDING)  {
            NMTREEVIEW *    tvnm = (NMTREEVIEW *) nmhdr;

            if (tvnm->action == TVE_EXPAND  &&  tvnm->itemNew.lParam == 0)  {
                // add children to parent
                hTree = GetDlgItem(hDlg, IDC_TREE);
                _bstr_t Path = GetTreePath(hTree, tvnm->itemNew.hItem);
                HKEY hKey = CRegEnumeration::OpenPath(Path, NULL);
                TCHAR   Name[ MAX_PATH ];

                for (int i = 0; RegEnumKey(hKey, i, Name, MAX_PATH) == 0; ++i)  {
                    TVINSERTSTRUCT  tvi;
                    tvi.hParent             = tvnm->itemNew.hItem;
                    tvi.hInsertAfter        = TVI_LAST;
                    tvi.item.mask           = TVIF_TEXT | TVIF_CHILDREN | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
                    tvi.item.pszText        = Name;
                    tvi.item.cChildren      = I_CHILDRENCALLBACK;
                    tvi.item.iImage         = 0;
                    tvi.item.iSelectedImage = 1;
                    tvi.item.lParam         = false;    // not expanded yet
                    TreeView_InsertItem(hTree, &tvi);
                }

                RegCloseKey(hKey);
                // mark parent as expanded
                TVITEM  tvi;
                tvi.mask        = TVIF_CHILDREN | TVIF_PARAM;
                tvi.hItem       = tvnm->itemNew.hItem;
                tvi.cChildren   = i;
                tvi.lParam      = true; // expanded
                TreeView_SetItem(hTree, &tvi);
            }
        }

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))  {
        case IDOK:
            hTree = GetDlgItem(hDlg, IDC_TREE);
            *ReturnPath = GetTreePath(hTree, TreeView_GetSelection(hTree));
            EndDialog(hDlg, 1);
            break;

        case IDCANCEL:
            EndDialog(hDlg, 0);
            break;
        }

        break;
    }

    return false;
}

_bstr_t GetTreeItemText(HWND hTree, int Item, int SubItem)
{
    // apparently there is no way to determine the length of the text in a list view
    TCHAR   tmp[ 64 * 1024 ];
    LVITEM  lvi;
    lvi.mask        = LVIF_TEXT;
    lvi.iItem       = Item;
    lvi.iSubItem    = SubItem;
    lvi.pszText     = tmp;
    lvi.cchTextMax  = sizeof tmp / sizeof tmp[0];
    SendMessage(hTree, LVM_GETITEMTEXT, Item, (LPARAM) &lvi);
    _bstr_t Text(tmp);
    return Text;
}


//----------------------------------------------------------------------
//
// MainDialog
//
// Main interface for editing output
//
//----------------------------------------------------------------------
LRESULT CALLBACK MainDialog(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    int                 idx;
    OPENFILENAME        open;
    static TCHAR        exportPath[ MAX_PATH ];
    TCHAR           *   p;
    bool                ok;
    DWORD               style;
    static bool         Starting = true;
    static HCURSOR      hCursor;
    HIMAGELIST          himlSmall;
    HICON               hIcon;
    DWORD   dwStyle;

    switch (message) {
    case WM_INITDIALOG: {
        // set anchor points
        {
            CResizer * resizer = new CResizer(hDlg);
            resizer->SetHorz(GetDlgItem(hDlg, IDC_REFRESH),      ANCHOR_LEFT);
            resizer->SetHorz(GetDlgItem(hDlg, IDC_EXPORT),       ANCHOR_LEFT);
            resizer->SetVert(GetDlgItem(hDlg, IDC_LIST),         ANCHOR_BOTH);
            resizer->SetHorz(GetDlgItem(hDlg, IDC_PATH),         ANCHOR_BOTH);
            resizer->SetHorz(GetDlgItem(hDlg, IDC_DESCRIPTION),  ANCHOR_LEFT);
            resizer->SetHorz(GetDlgItem(hDlg, IDC_STATUS),       ANCHOR_BOTH);
        }
        CRegistry   reg(_T("Software\\Sysinternals\\AccessEnum"));
        reg.ReadRegSettings(RegSettings);
        CheckMenuItem(GetMenu(hDlg), IDC_LOCAL_SYSTEM, MF_BYCOMMAND | (g.ShowLocalSystemAccount ? MF_CHECKED : 0));
        // clear status text
        SetWindowText(GetDlgItem(hDlg, IDC_STATUS),  _T(""));
        _tcscpy(exportPath, _T("AccessEnum"));
        // Set cursor for window
        hCursor = LoadCursor(NULL, IDC_ARROW);
        SetClassLong(hDlg, GCL_HCURSOR, (LONG)hCursor);
        SetCursor(hCursor);
        // Create listview columns
        InitListViewColumns(GetDlgItem(hDlg, IDC_LIST), Columns, sizeof Columns / sizeof Columns[0] - 1);
        style = GetWindowLong(GetDlgItem(hDlg, IDC_LIST), GWL_STYLE);
        style |= LVS_SHOWSELALWAYS;
        SetWindowLong(GetDlgItem(hDlg, IDC_LIST), GWL_STYLE, style);
        ListView_SetExtendedListViewStyleEx(GetDlgItem(hDlg, IDC_LIST), LVS_EX_LABELTIP, LVS_EX_LABELTIP);
        // Set the arrow image list
        himlSmall = ImageList_Create(16, 16, ILC_COLOR | ILC_MASK, 256, 256);
        hIcon = LoadIcon(g.hInst, MAKEINTRESOURCE(IDI_DOWNARROW));
        ImageList_AddIcon(himlSmall, hIcon);
        hIcon = LoadIcon(g.hInst, MAKEINTRESOURCE(IDI_UPARROW));
        ImageList_AddIcon(himlSmall, hIcon);
        dwStyle = GetWindowLong(GetDlgItem(hDlg, IDC_LIST), GWL_STYLE);
        dwStyle |= LVS_SHAREIMAGELISTS;
        SetWindowLong(GetDlgItem(hDlg, IDC_LIST), GWL_STYLE, dwStyle);
        ListView_SetImageList(GetDlgItem(hDlg, IDC_LIST), himlSmall, LVSIL_SMALL);
#if 0

        if (! IsDomainAdmin())  {
            MessageBox(hDlg, _T("Information may be incomplete because you are not a domain administrator"), APPNAME, MB_OK | MB_ICONWARNING);
        }

#endif
        {
            TCHAR Path[ MAX_PATH ] = _T("");
            GetEnvironmentVariable(_T("SYSTEMROOT"), Path, MAX_PATH);
            SetDlgItemText(hDlg, IDC_PATH, Path);
        }
    }

    return TRUE;

    case WM_SETCURSOR:
        switch (LOWORD(lParam))  {
        case HTTOP:
        case HTBOTTOM:
        case HTLEFT:
        case HTRIGHT:
        case HTTOPLEFT:
        case HTTOPRIGHT:
        case HTBOTTOMLEFT:
        case HTBOTTOMRIGHT:
            break;

        default:
            SetCursor(hCursor);
            return true;
        }

        break;

    case WM_APP_ENUM_COMPLETE:
        EnableWindow(GetDlgItem(hDlg, IDC_REFRESH), true);
        EnableWindow(GetDlgItem(hDlg, IDC_PATH), true);
        EnableWindow(GetDlgItem(hDlg, IDC_BROWSE_FILE), true);
        EnableWindow(GetDlgItem(hDlg, IDC_BROWSE_REG), true);
        EnableWindow(GetDlgItem(hDlg, IDCANCEL), false);
        EnableWindow(GetDlgItem(hDlg, IDC_EXPORT), true);
        hCursor = LoadCursor(NULL, IDC_ARROW);
        SetClassLong(hDlg, GCL_HCURSOR, (LONG)hCursor);
        SetCursor(hCursor);

        if (g.Abort)  {
            SendMessage(g.Abort, WM_CLOSE, 0, 0);
//              MessageBox( hDlg, _T("Scan cancelled by user"), APPNAME, MB_OK|MB_ICONINFORMATION );
        }

        break;

    case WM_CONTEXTMENU: {
        // Get window that was clicked
        POINT   pt;
        HWND    hChild;

        if (lParam != 0xFFFFFFFF)  {
            // triggered by right-click
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            MapWindowPoints(NULL, hDlg, &pt, 1);
            hChild = ChildWindowFromPointEx(hDlg, pt, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED);

            if (hChild != GetDlgItem(hDlg, IDC_LIST))
                break;

            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
        } else {
            // triggered by hotkey
            hChild = GetFocus();

            if (hChild != GetDlgItem(hDlg, IDC_LIST))
                break;

            idx = ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_LIST));

            if (idx < 0)
                break;

            RECT    rc;
            ListView_GetItemRect(GetDlgItem(hDlg, IDC_LIST), idx, &rc, LVIR_LABEL);
            pt.x = rc.left;
            pt.y = rc.bottom;
            MapWindowPoints(GetDlgItem(hDlg, IDC_LIST), NULL, &pt, 1);
        }

        {
            // Create pop-up menu
            idx = ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_LIST));

            if (idx >= 0)  {
                TCHAR   path[ MAX_UNCPATH ];
                ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST), idx, 0, path, MAX_UNCPATH);
                idx = _tcsnicmp(path, _T("HK"), 2) ? MF_ENABLED : MF_GRAYED;
            } else {
                idx = MF_ENABLED;
            }

            HMENU hMenu     = LoadMenu(g.hInst, _T("POPUP"));
            HMENU hSubMenu  = GetSubMenu(hMenu, 0);
            // Enable properties item for files, but not registry items.
            EnableMenuItem(hSubMenu, IDC_PROPERTIES, MF_BYCOMMAND | idx);
            // Set default menu item to Properties for files, Explore for registry
            MENUITEMINFO    info = { 0 };
            info.cbSize = sizeof info;
            info.fMask = MIIM_STATE;
            info.fState = MFS_DEFAULT;
            SetMenuItemInfo(hSubMenu, idx == MF_ENABLED ? IDC_PROPERTIES : IDC_EXPLORE,
                            false, &info);
            // Display menu
            TrackPopupMenu(hSubMenu,
                           TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                           pt.x, pt.y, 0, hDlg, NULL);
            DestroyMenu(hMenu);
        }
    }
    break;

    case WM_NOTIFY:
        if (LOWORD(wParam) == IDC_LIST)  {
            NMLISTVIEW  * nm = (NMLISTVIEW *) lParam;

            switch (nm->hdr.code)  {
            case LVN_COLUMNCLICK:
                // sort column
                SortListView(nm->hdr.hwndFrom, nm->iSubItem, Columns);
                break;

            case NM_DBLCLK:
                // double click item, so display device properties
                idx = ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_LIST));

                if (idx >= 0)  {
                    TCHAR   path[ MAX_UNCPATH ];
                    ListView_GetItemText(GetDlgItem(hDlg, IDC_LIST), idx, 0, path, MAX_UNCPATH);

                    if (_tcsnicmp(path, _T("HK"), 2) == 0)
                        ExploreFile(hDlg, path);
                    else
                        ShowFileProperties(hDlg, path);
                }

                break;
            }
        }

        break;

    case WM_COMMAND:

        // Normal notifications
        switch (LOWORD(wParam)) {
        case IDC_QUIT:
            // quit
            SendMessage(hDlg, WM_CLOSE, 0, 0);
            break;

        case IDCANCEL:

            // is an enumeration in progress?
            if (! IsWindowEnabled(GetDlgItem(hDlg, IDC_REFRESH)))  {
                // have we already cancelled it?
                if (g.Abort == NULL)  {
                    g.Abort = CreateDialog(g.hInst, _T("ABORT"), hDlg, AbortDialog);
                }
            }

            break;

        case IDC_ABOUT:
            DialogBox(g.hInst, _T("ABOUT"), hDlg, AboutDialog);
            break;

        case IDC_FILEOPTIONS:
            DialogBox(g.hInst, _T("FILEPERM"), hDlg, FilePermDialog);
            break;

        case IDC_HELPCONTENTS:
            DialogBox(g.hInst, _T("HELP"), hDlg, HelpDialog);
            break;

        case IDC_LOCAL_SYSTEM:
            // should we show localsystem rights?
        {
            MENUITEMINFO    info = { sizeof info, 0 };
            info.fMask = MIIM_STATE;
            GetMenuItemInfo(GetMenu(hDlg), IDC_LOCAL_SYSTEM, false, &info);
            g.ShowLocalSystemAccount = (info.fState & MFS_CHECKED) == 0;
            CheckMenuItem(GetMenu(hDlg), IDC_LOCAL_SYSTEM, MF_BYCOMMAND | (g.ShowLocalSystemAccount ? MF_CHECKED : 0));
        }
        break;

        case IDOK:
            // convert to IDC_REFRESH
            return SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_REFRESH, HIWORD(wParam)), lParam);

        case IDC_BROWSE_FILE: {
            BROWSEINFO  bi  = { 0 };
            bi.hwndOwner    = hDlg;
            bi.ulFlags      = BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS ;
            bi.lpszTitle    = _T("Select the folder to enumerate:");
            ITEMIDLIST * pidl = SHBrowseForFolder(&bi);
            TCHAR       Path[ MAX_UNCPATH ];

            if (pidl  &&  SHGetPathFromIDList(pidl, Path))  {
                SetDlgItemText(hDlg, IDC_PATH, Path);
                LPMALLOC pMalloc = NULL;
                SHGetMalloc(&pMalloc);
                pMalloc->Free(pidl);
            }
        }
        break;

        case IDC_BROWSE_REG: {
            _bstr_t Path;

            if (DialogBoxParam(g.hInst, _T("REGBROWSE"), hDlg, BrowseRegDialog, (LPARAM)&Path))
                SetDlgItemText(hDlg, IDC_PATH, Path);
        }
        break;

        case IDC_REFRESH:
            if (IsWindowEnabled(GetDlgItem(hDlg, IDC_REFRESH)))  {
                // Create a list of all file permissions
                EnableWindow(GetDlgItem(hDlg, IDC_REFRESH), false);
                EnableWindow(GetDlgItem(hDlg, IDC_PATH), false);
                EnableWindow(GetDlgItem(hDlg, IDC_BROWSE_FILE), false);
                EnableWindow(GetDlgItem(hDlg, IDC_BROWSE_REG), false);
                EnableWindow(GetDlgItem(hDlg, IDCANCEL), true);
                EnableWindow(GetDlgItem(hDlg, IDC_EXPORT), false);
                hCursor = LoadCursor(NULL, IDC_APPSTARTING);
                SetClassLong(hDlg, GCL_HCURSOR, (LONG)hCursor);
                SetCursor(hCursor);
                delete g.ShareList;
                g.ShareList = NULL;
                g.Abort = NULL;
                ListView_DeleteAllItems(GetDlgItem(hDlg, IDC_LIST));
                g.Exclude = (TCHAR *)NULL;
                // get path to enumerate
                TCHAR Path[ MAX_PATH ];
                GetDlgItemText(hDlg, IDC_PATH, Path, MAX_PATH);
                // if it is on a network then get name of system it is on
                TCHAR   NetworkPath[ MAX_PATH ] = _T("\0\0\0");

                if (Path[0] == '\\' && Path[1] == '\\') {
                    _tcscpy(NetworkPath, Path);
                } else if (Path[1] == ':')  {
                    // convert drive to unc path, if share
                    TCHAR   DevicePath[ 3 ] = { Path[0], ':', 0 };
                    DWORD   nb = MAX_PATH;
                    WNetGetConnection(DevicePath, NetworkPath, &nb);
                }

                if (NetworkPath[0])  {
                    // get file server name
                    TCHAR * ptr = _tcschr(NetworkPath + 2, '\\');

                    if (ptr)
                        *ptr = 0;
                    else
                        NetworkPath[0] = 0;
                }

                CEnumeration * root = NULL;

                if (_tcsnicmp(Path, _T("HK"), 2) == 0)  {
                    // traverse registry
                    root = new CRegEnumeration(Path, true);
                } else {
                    // traverse directory
                    DWORD   attr = GetFileAttributes(Path);
                    bool    isDir = (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
                    root = new CFileEnumeration(Path, isDir);
                }

                if (root == NULL)  {
                    MessageBox(hDlg, _T("Path not found"), APPNAME, MB_OK | MB_ICONWARNING);
                    break;
                }

                CTraversal * e = new CTraversal(GetDlgItem(hDlg, IDC_LIST),
                                                NetworkPath[0] ? NetworkPath + 2 : _T(""),
                                                root);
                e->Start();
                // delete root;
                // Add path to list of paths in combo box
                idx = SendMessage(GetDlgItem(hDlg, IDC_PATH), CB_FINDSTRINGEXACT, -1, (LPARAM) Path);

                if (idx != CB_ERR)
                    SendMessage(GetDlgItem(hDlg, IDC_PATH), CB_DELETESTRING, idx, 0);

                SendMessage(GetDlgItem(hDlg, IDC_PATH), CB_INSERTSTRING, 0, (LPARAM) Path);
            }

            break;

        case IDC_EXCLUDE:
            idx = ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_LIST));

            if (idx >= 0)  {
                // make sure enumeration thread has finished with last exclude:
                while (!!g.Exclude  &&  !IsWindowEnabled(GetDlgItem(hDlg, IDC_REFRESH)))  {
                    Sleep(100);
                }

                HWND    hList = GetDlgItem(hDlg, IDC_LIST);
                _bstr_t Path = GetTreeItemText(hList, idx, 0);
                // copy path to where enumeration thread can see it:
                g.Exclude = Path;
                // remove any items with this path
                LVFINDINFO  find;
                find.flags  = LVFI_STRING | LVFI_PARTIAL;
                find.psz    = Path;
                idx = -1;

                for (;;)  {
                    idx = ListView_FindItem(hList, idx, &find);

                    if (idx < 0)
                        break;

                    ListView_DeleteItem(hList, idx);
                    --idx;
                }
            }

            break;

        case IDC_PROPERTIES:
            idx = ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_LIST));

            if (idx >= 0)  {
                HWND    hList = GetDlgItem(hDlg, IDC_LIST);
                TCHAR   Path[ MAX_UNCPATH ];
                ListView_GetItemText(hList, idx, 0, Path, MAX_UNCPATH);
                ShowFileProperties(hDlg, Path);
            }

            break;

        case IDC_EXPLORE:
            idx = ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_LIST));

            if (idx >= 0)  {
                HWND    hList = GetDlgItem(hDlg, IDC_LIST);
                TCHAR   Path[ MAX_UNCPATH ];
                ListView_GetItemText(hList, idx, 0, Path, MAX_UNCPATH);
                ExploreFile(hDlg, Path);
            }

            break;

        case IDC_EXPORT:
            // get export file name
            memset(&open, 0, sizeof open);
            open.lStructSize = sizeof open;
            open.hwndOwner = hDlg;
            open.lpstrFilter = TEXT("Unicode Text (*.txt)\0*.txt\0");
            open.lpstrFile = exportPath;
            open.nMaxFile = sizeof exportPath / sizeof(TCHAR);
            open.Flags = OFN_HIDEREADONLY;
            open.lpstrTitle = TEXT("Save as");

            if (! GetSaveFileName(&open))
                break;

            p = _tcschr(exportPath, 0);
            p -= 4;

            if (p < exportPath  ||  _tcsicmp(p, TEXT(".txt")) != 0)
                _tcscat(exportPath, TEXT(".txt"));

            DeleteFile(exportPath);
            ok = ExportFile(GetDlgItem(hDlg, IDC_LIST), exportPath);

            if (! ok)
                MessageBox(hDlg, TEXT("Error exporting settings"), APPNAME, MB_OK | MB_ICONSTOP);

            break;

        case IDC_COMPARE:
            CreateDialogParam(g.hInst, _T("COMPARE"), hDlg, CompareDialog, (LPARAM)g.ShareList);
            break;
        }

        break;

    case WM_CLOSE: {
        // save connection info to registry
        CRegistry reg(_T("Software\\Sysinternals\\AccessEnum"));
        reg.WriteRegSettings(RegSettings);
        SendMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
        SaveListViewColumns(GetDlgItem(hDlg, IDC_LIST));
        PostQuitMessage(0);
        break;
    }
    }

    return DefWindowProc(hDlg, message, wParam, lParam);
}



//----------------------------------------------------------------------
//
// WinMain
//
// Initialize and start application
//
//----------------------------------------------------------------------
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
#if 0
    PrintAll();
    return 0;
#endif
    // initialize things
    InitCommonControls();
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    g.hInst = hInstance;
    // register window class
    WNDCLASSEX  wndclass = { 0 };
    wndclass.cbSize         = sizeof wndclass;
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = MainDialog;
    wndclass.hInstance      = hInstance;
    wndclass.cbWndExtra     = DLGWINDOWEXTRA;
    wndclass.hIcon          = LoadIcon(hInstance, TEXT("APPICON"));
    wndclass.hIconSm        = LoadIcon(hInstance, TEXT("APPICON"));
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszClassName  = TEXT("ShareEnumClass");

    if (RegisterClassEx(&wndclass) == 0)
        GetLastError();

    g.hMainDlg = CreateDialog(hInstance, TEXT("MAIN"), NULL, (DLGPROC)MainDialog);

    if (g.hMainDlg == NULL)  {
        GetLastError();
        return 1;
    }

    // make top window
    //SetWindowPos( hMainDlg, HWND_TOP, 1, 1, 0, 0, SWP_NOSIZE );
    // Make the window visible
    ShowWindow(g.hMainDlg, nCmdShow);
    // Paint window
    UpdateWindow(g.hMainDlg);
    // Keyboard accelerators
    HACCEL hAccel = LoadAccelerators(hInstance, TEXT("ACCELERATORS"));
    // Message loop
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (TranslateAccelerator(g.hMainDlg, hAccel, &msg))
            continue;

        if (IsDialogMessage(g.hMainDlg, &msg))
            continue;

        // process message
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
