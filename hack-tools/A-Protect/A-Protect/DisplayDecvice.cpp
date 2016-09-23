#include <windows.h>
#include <stdio.h>

unsigned char szQueryValue[256];

BOOL MyRegQueryValue(char *strKey,char *strValue,char *VenKey,char *lpDecviceKey)
{
	HKEY hKey,hKey1;
	DWORD size;
	char data[256];
	int i=0;
	char szQueryKey[256];
	DWORD dwBufSize = 256;
	DWORD dwDataType = REG_SZ;

	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,strKey,0,KEY_ENUMERATE_SUB_KEYS,&hKey)==ERROR_SUCCESS)
	{
		size = sizeof(data);
		while(RegEnumKeyExA(hKey,i,data,&size,NULL,NULL,NULL,NULL)!=ERROR_NO_MORE_ITEMS)
		{
			memset(szQueryKey,0,sizeof(szQueryKey));
			strcat(szQueryKey,strKey);
			strcat(szQueryKey,"\\");
			strcat(szQueryKey,data);

			if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,szQueryKey,NULL,KEY_QUERY_VALUE,&hKey1) == ERROR_SUCCESS)
			{
				memset(szQueryValue,0,sizeof(szQueryValue));
				RegQueryValueExA(hKey1,strValue,NULL,&dwDataType,szQueryValue,&dwBufSize);
				RegCloseKey(hKey1);
			}
			if (strcmp((char *)szQueryValue,"Display") == 0){
				memset(lpDecviceKey,0,sizeof(lpDecviceKey));
				wsprintfA(lpDecviceKey,"@\"PCI\\%s\\%s\"",VenKey,data);
				//MessageBoxA(0,lpDecviceKey,0,0);
				//printf("Key2:%s %s\r\n",lpDecviceKey,szQueryValue);
			}
			i++;
			size = sizeof(data);
			memset(data,0,sizeof(data));
		}
		RegCloseKey(hKey);
	}
	return FALSE;
}
BOOL QueryDisplayDecvicePath(char *lpDecviceKey)
{
	HKEY hKey;
	char *strKey = "SYSTEM\\CurrentControlSet\\Enum\\PCI";
	DWORD size;
	char data[260];
	int i=0;
	char szQueryKey[260];
	
	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,strKey,0,KEY_ENUMERATE_SUB_KEYS,&hKey)==ERROR_SUCCESS)
	{
		size = sizeof(data);
		while(RegEnumKeyExA(hKey,i,data,&size,NULL,NULL,NULL,NULL)!=ERROR_NO_MORE_ITEMS)
		{
			memset(szQueryKey,0,sizeof(szQueryKey));
			strcat(szQueryKey,strKey);
			strcat(szQueryKey,"\\");
			strcat(szQueryKey,data);
			//printf("Key:%s\r\n",szQueryKey);
			MyRegQueryValue(szQueryKey,"Class",data,lpDecviceKey);
			i++;
			size = sizeof(data);
			memset(data,0,sizeof(data));
		}
		RegCloseKey(hKey);
	}
	return FALSE;
}
/*
int main()
{
	QueryDisplayDecvicePath();
	return 0;
}
*/