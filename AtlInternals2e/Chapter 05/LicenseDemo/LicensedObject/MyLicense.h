
const OLECHAR rlk[] = OLESTR("Some run-time license key") ;

class CMyLicense
{
protected:

    static BOOL VerifyLicenseKey(BSTR bstr)
    {
        return wcscmp(bstr, rlk) == 0;
    }

    static BOOL GetLicenseKey(DWORD dwReserved, BSTR* pBstr)
    {
        *pBstr = SysAllocString(rlk);
        return TRUE;
    }

    static BOOL IsLicenseValid()
    {
        return FALSE;
    }
};
