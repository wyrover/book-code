#include <Windows.h>
#include <tchar.h>
#include <initguid.h>  
#include <ole2.h>  
#include <mstask.h>  
#include <msterr.h>  
#include <iostream>  


using namespace std;  


#define TASKS_TO_RETRIEVE          5  

int _tmain(int argc, _TCHAR* argv[])  
{  
	HRESULT hr = S_OK;  
	ITaskScheduler *pITS;  

	std::wcout.imbue(std::locale("chs"));  

	// 初始化 com 接口  
	hr = CoInitialize(NULL);  
	if (SUCCEEDED(hr))  
	{  
		hr = CoCreateInstance(CLSID_CTaskScheduler,  
			NULL,  
			CLSCTX_INPROC_SERVER,  
			IID_ITaskScheduler,  
			(void **) &pITS);  
		if (FAILED(hr))  
		{  
			wcout << L"CoCreateInstance Error!" << endl;  
			CoUninitialize();  
			return hr;  
		}  
	}  
	else  
	{  
		wcout << L"CoInitialize Error!" << endl;  
		return hr;  
	}  

	// 开始枚举  
	IEnumWorkItems *pIEnum;  
	hr = pITS->Enum(&pIEnum);  
	if (FAILED(hr))  
	{  
		wcout << L"Enum Error!" << endl;  
		CoUninitialize();  
		return hr;  
	}  

	// 取出枚举后得到的数据  
	LPWSTR *lpwszNames;  
	DWORD dwFetchedTasks = 0;  
	while (SUCCEEDED( pIEnum->Next(TASKS_TO_RETRIEVE, &lpwszNames, &dwFetchedTasks)) && (dwFetchedTasks != 0) )  
	{  
		while (dwFetchedTasks)  
		{  
			ITask *pITask;  
			wchar_t strTemp[MAX_PATH] = {0};  
			SIZE_T len;  

			len = wcslen(lpwszNames[--dwFetchedTasks]);  
			while (len)  
			{  
				if (lpwszNames[dwFetchedTasks][--len] == L'.')  
				{  
					wcsncpy(strTemp, lpwszNames[dwFetchedTasks], len);  
					break;  
				}  
			}  

			hr = pITS->Activate(strTemp, IID_ITask, (IUnknown**) &pITask);  
			if (FAILED(hr))  
			{  
				wcout <<lpwszNames[dwFetchedTasks] << L"calling ITaskScheduler::Activate Error!" <<endl;  
				continue;  
			}  

			LPWSTR lpwszApplicationName;  
			hr = pITask->GetApplicationName(&lpwszApplicationName);  
			if (FAILED(hr))  
			{  
				wcout <<lpwszNames[dwFetchedTasks] << L"calling ITask::GetApplicationName Error!" <<endl;  
				continue;  
			}  

			wcout <<lpwszNames[dwFetchedTasks] << endl << lpwszApplicationName << endl << endl;  
			pITask->Release();  
			CoTaskMemFree(lpwszApplicationName);  
			CoTaskMemFree(lpwszNames[dwFetchedTasks]);  
		}  

		CoTaskMemFree(lpwszNames);  
	}  

	pITS->Release();  
	pIEnum->Release();  
	CoUninitialize();  

	system("pause");  

	return 0;  
}  