#ifndef _KDOSNAMES_H_
#define _KDOSNAMES_H_
#include <vector>


class KDosNames
{
public:
	KDosNames()
	{
		m_bInited = FALSE;
	}
	~KDosNames()
	{

	}

	BOOL Init()
	{
		if (m_bInited)
			return TRUE;

		m_bInited = GetDosDeviceNames();

		return m_bInited;
	}

	BOOL DevicePathToDosPath(CStringW& strPath)
	{
		for (size_t i = 0; i < m_vecDosDeviceNames.size(); i += 2)
		{
			
			if (wcsnicmp(strPath, m_vecDosDeviceNames[i], m_vecDosDeviceNames[i].GetLength()) == 0)
			{
				strPath.Replace(m_vecDosDeviceNames[i], m_vecDosDeviceNames[i + 1]);
				return TRUE;
			}
		}

		return FALSE;
	}

protected:
	BOOL GetDosDeviceNames(void)
	{
		WCHAR szDevicesName[MAX_LOGICAL_DRIVE_LENGTH+1] ={0};
		int nLen = GetLogicalDriveStringsW(MAX_LOGICAL_DRIVE_LENGTH,szDevicesName);
		if (!nLen) return FALSE;

		int nCount = nLen / LOGICAL_DRIVE_NAME_LEN;
		WCHAR szTargetPath[MAX_PATH+1] = {0};
		WCHAR *szCurrentDevice = szDevicesName;
		szTargetPath[MAX_PATH] = L'\0';

		m_vecDosDeviceNames.clear();
		for (int i = 0; i < nCount; i++)
		{
			szCurrentDevice[2] = L'\0';

			if(QueryDosDeviceW(szCurrentDevice, szTargetPath, MAX_PATH))
			{
				m_vecDosDeviceNames.push_back(szTargetPath);
				m_vecDosDeviceNames.push_back(szCurrentDevice);
			}
			szCurrentDevice += LOGICAL_DRIVE_NAME_LEN;
		}

		return TRUE;
	}

private:
	static const int LOGICAL_DRIVE_NAME_LEN = 4;
	static const int MAX_LOGICAL_DRIVE_LENGTH = 	(LOGICAL_DRIVE_NAME_LEN*26 + 1);

	BOOL m_bInited;
	std::vector<CStringW> m_vecDosDeviceNames;
};
#endif