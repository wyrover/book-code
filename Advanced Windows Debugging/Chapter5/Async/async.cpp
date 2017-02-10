/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE_NAME              256 
#define MAX_REG_VALUE_COUNT    2
#define MAX_LEN                             256 
#define ENUM_TIME_LEN                 3000

class CRegValue
{
public: 
    CRegValue() : m_pwszName(NULL), m_dwValue(0) {};
    ~CRegValue()
    {
        if(m_pwszName)
        {
            delete[] m_pwszName;
            m_pwszName=NULL;
        }
    }
    const WCHAR* GetName() const { return m_pwszName; }
    DWORD GetValue() const { return m_dwValue; }
    
    VOID SetProperties(WCHAR* pwszName, DWORD dwValue)
    {
        m_pwszName=pwszName;
        m_dwValue = dwValue;
    }

private:
    WCHAR* m_pwszName;
    DWORD m_dwValue ;
} ;


typedef struct 
{
    CRegValue* pRegValues;
    DWORD dwRegValuesCount;
    HKEY hKey;
} CRegEnumData;

BOOL RegEnum(WCHAR* pwszPath, DWORD dwTimeout);
HANDLE RegEnumAsync(CRegEnumData* pRegData);
DWORD WINAPI RegThreadProc(LPVOID lpParameter);
VOID DisplayError(WCHAR* pwszPath, DWORD dwType, DWORD dwTimeout, BOOL bFullError);


int __cdecl wmain (int argc, WCHAR* args[])
{
    WCHAR wszRegPath[MAX_LEN] ;
    int iTimeout=0;
    BOOL bEnd = FALSE;
    
    while(!bEnd)
    {
        wprintf(L"Enter registry key path (\"quit\" to quit): ");
        wscanf(L"%s", wszRegPath, MAX_LEN);

        if(!_wcsicmp(wszRegPath, L"quit"))
        {
            bEnd=TRUE;
            continue;
        }

        wprintf(L"Enter timeout for enumeration: ");
        wscanf(L"%d", &iTimeout);
        
        if(iTimeout==0)
        {
            wprintf(L"Invalid timeout specified...\n");
            bEnd=TRUE;
        }
        else
        {
            //
            // Enumerate 
            //
            if ( RegEnum(wszRegPath, iTimeout) == FALSE )
            {
                DisplayError(wszRegPath, REG_DWORD, iTimeout, TRUE);
            }

        }
    }
    return 0;
}


BOOL RegEnum(WCHAR* pwszPath, DWORD dwTimeout)
{
    CRegValue regValues[MAX_REG_VALUE_COUNT];
    CRegEnumData* pData=NULL;
    BOOL bRet=FALSE;
    HANDLE hWaitHandle = NULL;
    DWORD dwRet=0;
    HKEY hKey=NULL;
    LONG lRes=0;

    if(!pwszPath)
    {
        return FALSE;
    }

    lRes=RegOpenKeyEx(HKEY_CURRENT_USER, pwszPath, 0, KEY_QUERY_VALUE, &hKey);
    if(lRes==ERROR_SUCCESS)
    {
        pData=new CRegEnumData;
        if(pData)
        {
            pData->pRegValues=regValues;
            pData->dwRegValuesCount=MAX_REG_VALUE_COUNT;
            pData->hKey=hKey;

            hWaitHandle=RegEnumAsync(pData);
            if(hWaitHandle!=NULL)
            {
                dwRet=WaitForSingleObject(hWaitHandle, dwTimeout);
                if(dwRet==WAIT_TIMEOUT)
                {
                    wprintf(L"Timeout occured...\n");
                }
                else
                {
                    for(int i=0; i<MAX_REG_VALUE_COUNT; i++)
                    {
                        if(regValues[i].GetName()!=NULL)
                        {
                            wprintf(L"Value %d Name: %s\n", i+1, regValues[i].GetName());
                            wprintf(L"Value %d Data: %d\n", i+1, regValues[i].GetValue());
                            bRet=TRUE;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                CloseHandle(hWaitHandle);
            }
            else
            {
                wprintf(L"Async registry enumeration failed...");
            }
        }
    }

    return bRet;
}

HANDLE RegEnumAsync(CRegEnumData* pRegData)
{
    return CreateThread(NULL, 0, RegThreadProc, pRegData, 0, NULL);
}

DWORD WINAPI RegThreadProc(LPVOID lpParameter)
{
    DWORD dwIndex=0;
    BOOL bRet=FALSE;

    Sleep(ENUM_TIME_LEN);

    CRegEnumData* pRegData=reinterpret_cast<CRegEnumData*>(lpParameter);

    while(!bRet && dwIndex<pRegData->dwRegValuesCount)
    {
        DWORD dwType=0;
        LONG lRes=0;
        DWORD dwData=0; // Only get DWORD values
        DWORD dwDataSize=sizeof(DWORD);
        DWORD dwNameLen=MAX_VALUE_NAME;
        WCHAR* pwszValueName=new WCHAR[MAX_VALUE_NAME];
        if(pwszValueName)
        {
            lRes=RegEnumValue(pRegData->hKey, dwIndex, pwszValueName, &dwNameLen, NULL, &dwType, (LPBYTE)&dwData, &dwDataSize);
            if(lRes==ERROR_SUCCESS && dwType==REG_DWORD)
            {
                pRegData->pRegValues[dwIndex].SetProperties(pwszValueName, dwData);
                dwIndex++;
            }
            else 
            {
                delete[] pwszValueName;
                bRet=TRUE;
            }
        }
        else 
        {
            bRet=TRUE;
        }
    }

    RegCloseKey(pRegData->hKey);
    delete pRegData;
    return 0 ;
}

VOID DisplayError(WCHAR* pwszPath, DWORD dwType, DWORD dwTimeout, BOOL bFullError)
{
    if(bFullError)
    {
        if(dwType==REG_DWORD)
        {
            wprintf(L"Error enumerating DWORDS in HKEY_CURRENT_USER\\%s within %d ms!\n", pwszPath, dwTimeout);
        }
        else
        {
            wprintf(L"Error enumerating <unknown type> in HKEY_CURRENT_USER\\%s within %d ms!\n", pwszPath, dwTimeout);
        }
    }
    else
    {
        wprintf(L"Error enumerating key values!\n");
    }

    //
    // Simulate wait for user confirmation
    //
    Sleep(6000); 
}