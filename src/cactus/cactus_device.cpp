/*!
* \file cactus_device.cpp
* \brief 设备枚举实现
* 
* 设备枚举实现
* 
* \author wangyang 
* \date 2015/01/30 
* \version 1.0
*/

#include "cactus/cactus_device.h"
#include "cactus/cactus_string.h"
#include <devguid.h>
#include <regstr.h>
#include <cfgmgr32.h>
#include <setupapi.h>
#pragma comment(lib,"Setupapi.lib")

namespace cactus
{
	SP_CLASSIMAGELIST_DATA _spImageData = {0};

	BOOL GetPhysicalDeviceObjectNameByProperty(
		HDEVINFO			hDevInfo, 
		PSP_DEVINFO_DATA	pDeviceInfoData, 
		std::wstring&		strPhysiceDeviceObjcetName)
	{
		BOOL bRet = TRUE;
		DWORD DataT;
		LPTSTR lpBuffer = NULL;
		DWORD dwBufSize = 0;
		// 
		// Call function with null to begin with, 
		// then use the returned buffer size 
		// to Alloc the buffer. Keep calling until
		// success or an unknown failure.
		// 
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			pDeviceInfoData,
			SPDRP_PHYSICAL_DEVICE_OBJECT_NAME,
			&DataT, (PBYTE)lpBuffer, dwBufSize, &dwBufSize))
		{
			DWORD dwLastErr = GetLastError();
			if (ERROR_INSUFFICIENT_BUFFER == dwLastErr)
			{
				// Change the buffer size.
				if (lpBuffer) LocalFree(lpBuffer);
				lpBuffer = (LPTSTR)LocalAlloc(LPTR, dwBufSize + 1);
			}
			else
			{
				// Insert error handling here.
				bRet = FALSE;
				break;
			}
		}
		if (bRet && lpBuffer)
		{
			strPhysiceDeviceObjcetName = lpBuffer;
		}
		if (lpBuffer) LocalFree(lpBuffer);

		return bRet;
	}


	BOOL GetPhysicalDeviceObjectNameBySearchCarmera(string16& strPhysiceDeviceObjcetName, string16& strDeviceDesc)
	{
		BOOL bRet = FALSE;
		HDEVINFO hDevInfo;
		SP_DEVINFO_DATA DeviceInfoData = {0};
		std::wstring strDeviceObjcetName;

		// Create a HDEVINFO with all present devices.
		hDevInfo = SetupDiGetClassDevs(
			(LPGUID)&GUID_DEVCLASS_IMAGE, 0, 0,DIGCF_PRESENT);
		if (hDevInfo == INVALID_HANDLE_VALUE)
		{
			// Insert error handling here.
			return bRet;
		}

		// Enumerate through all devices in Set.
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
		{
			if(GetPhysicalDeviceObjectNameByProperty(
				hDevInfo, 
				&DeviceInfoData, 
				strDeviceObjcetName))
			{
				if (!strDeviceObjcetName.empty())
				{
					strPhysiceDeviceObjcetName = strDeviceObjcetName;
					bRet = TRUE;
					break;
				}
			}
		}

		//  Cleanup
		SetupDiDestroyDeviceInfoList(hDevInfo);
		return bRet;
	}


    UINT32 Setupapi_GetDrvStatsOnDev(int nProblemCode, const string16& strGuidValue)
    {
        if ( 0 == nProblemCode )
        {
            return DrvInstedOnDev;
        }

        GUID guid;
        GuidFromString(strGuidValue, guid );
        return Setupapi_GetDrvStatsOnDev( nProblemCode, guid );
    }


    UINT32 Setupapi_GetDrvStatsOnDev(int nProblemCode, GUID& guid )
    {
        if ( 0 == nProblemCode )
        {
            return DrvInstedOnDev;
        }

        HDEVINFO hDevInfo = SetupDiGetClassDevs( &guid, NULL, NULL, 0 );
        if ( INVALID_HANDLE_VALUE == hDevInfo )
        {
            return DrvInstedOnDev;
        }

        SP_DEVINFO_DATA devInfoData;
        ZeroMemory( &devInfoData, sizeof( devInfoData ) );
        devInfoData.cbSize = sizeof( devInfoData );

        if ( !SetupDiEnumDeviceInfo( hDevInfo, 0, &devInfoData ) )
        {
            return DrvInstedOnDev;
        }

        DWORD dwDrvStats = 0, dwRequreSize = 0;
        if ( !SetupDiGetDeviceRegistryProperty( 
            hDevInfo, 
            &devInfoData, 
            SPDRP_INSTALL_STATE,
            NULL,
            (PBYTE)( &dwDrvStats ),
            sizeof( dwDrvStats ),
            &dwRequreSize)
            )
        {
            return DrvInstedOnDev;
        }

        switch ( dwDrvStats )
        {
        case 0/*CM_INSTALL_STATE_INSTALLED*/:		return DrvInstedOnDev;		break;
        case 1/*CM_INSTALL_STATE_NEEDS_REINSTALL*/:	return DrvNeedInstOnDev;	break;
        case 2/*CM_INSTALL_STATE_FAILED_INSTALL*/:	return DrvFailedInstOnDev;	break;
        case 3/*CM_INSTALL_STATE_FINISH_INSTALL*/:	return DrvFinishInstOnDev;	break;
        }

        return DrvInstedOnDev;
    }

    int Cmapi_GetDeviceProblemCode(const string16& instance_id)
    {
        CONFIGRET dwRet = 0;
        DEVINST devInst;
        ULONG uStatus = DN_HAS_PROBLEM, uProblem = 0;
        dwRet = CM_Locate_DevNode( &devInst, (DEVINSTID_W)instance_id.c_str(), CM_LOCATE_DEVNODE_NORMAL );
        if ( CR_SUCCESS == dwRet )
        {
            dwRet = CM_Get_DevNode_Status( &uStatus, &uProblem, devInst, 0 );
            if ( CR_SUCCESS == dwRet )
            {
                return uProblem;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return -2;
        }
    }

    DWORD GetDeviceProblemCodeByInstanceID(const string16& instance_id, ULONG& uProblem)
    {
        CONFIGRET dwRet = 0;
        DEVINST devInst;
        ULONG uStatus = DN_HAS_PROBLEM;
        dwRet = CM_Locate_DevNode(&devInst, (DEVINSTID_W)instance_id.c_str(), CM_LOCATE_DEVNODE_NORMAL);
        if ( CR_SUCCESS == dwRet )
        {
            dwRet = CM_Get_DevNode_Status( &uStatus, &uProblem, devInst, 0);
        }
        return dwRet;
    }

    DWORD RefreshDevice()
    {
        DEVINST devInst = 0;
        DWORD dwStatus = 0; 

        ///>得到设备管理树的根结点
        dwStatus = CM_Locate_DevNode(&devInst, NULL, CM_LOCATE_DEVNODE_NORMAL);  
        if (dwStatus == CR_SUCCESS)
        {
            ///>刷新
            dwStatus = CM_Reenumerate_DevNode(devInst, 0); 
        }
        return dwStatus;
    }

	BOOL AddDevice(std::vector<Device>& device_list, HDEVINFO hDevInfo, int index, SP_DEVINFO_DATA& DevInfoData)
	{
		char16 szClassName[MAX_PATH] = {0};
		char16 szInstanceID[LINE_LEN]  = {0};		
		char16 szName[64]      = {0};
		string16 classname;
		string16 instance_id;
		string16 name;
		string16 path;		
		int wImageIdx       = 0;		
		short                  wOrder;
		DWORD                  dwRequireSize;
		char  szBuf[MAX_PATH] = {0};

		if (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DevInfoData, SPDRP_CLASS, //SPDRP_DEVICEDESC,
			0L, (PBYTE)szBuf, 2048, 0))
		{
			return FALSE;
		}

		if (!SetupDiGetClassImageIndex(&_spImageData, &DevInfoData.ClassGuid, &wImageIdx)) {
			return FALSE;	
		}

		if (SetupDiGetClassDescription(&DevInfoData.ClassGuid, szClassName, MAX_PATH, &dwRequireSize)) {
			classname = szClassName;	
		}

		

		if (SetupDiGetDeviceInstanceId(hDevInfo, &DevInfoData, szInstanceID, LINE_LEN, 0)) {
			instance_id = szInstanceID;
		}


		SP_DEVICE_INTERFACE_DATA spDevInterfaceData = {0};	
		spDevInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);


		if (SetupDiCreateDeviceInterface(hDevInfo,
			&DevInfoData,
			&DevInfoData.ClassGuid,
			0L,
			0L,
			&spDevInterfaceData))			
		
		{
			
			DWORD dwRequire = 0;
			
			if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &spDevInterfaceData,	0L, 0, &dwRequire, 0L)) {
				SP_DEVICE_INTERFACE_DETAIL_DATA *pspDevInterfaceDetail = 0L;
				pspDevInterfaceDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA*)LocalAlloc(LPTR,
					sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA)*dwRequire);
				pspDevInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

				if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &spDevInterfaceData, pspDevInterfaceDetail, dwRequire, &dwRequire, 0L)) {
					path = string16(pspDevInterfaceDetail->DevicePath);									
				}

				if (pspDevInterfaceDetail)
					LocalFree(pspDevInterfaceDetail);
			}				
		}


		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DevInfoData, SPDRP_FRIENDLYNAME, 0L, (PBYTE)szName, 63, 0)) {
			name = string16(szName);
		} else if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DevInfoData, SPDRP_DEVICEDESC, 0L, (PBYTE)szName, 63, 0)) {
			name = string16(szName);
		}

		
		device_list.push_back(Device(DevInfoData.ClassGuid, name, instance_id, path, index, wOrder));
		
		return TRUE;
		
	}


	void InitialImageData()
	{		
		SetupDiDestroyClassImageList(&_spImageData);
		RtlZeroMemory(&_spImageData, sizeof(SP_CLASSIMAGELIST_DATA));
		_spImageData.cbSize = sizeof(SP_CLASSIMAGELIST_DATA);
		SetupDiGetClassImageList(&_spImageData);
	};


	BOOL InitDeviceList(std::vector<Device>& device_list)
	{
		HDEVINFO        hDevInfo         = NULL;
		SP_DEVINFO_DATA spDevInfoData    = {0};
		int				wIndex           = 0;		

		InitialImageData();

		hDevInfo = SetupDiGetClassDevs(0L, 0L, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES | DIGCF_PROFILE);
		if (hDevInfo == (void*)-1)
		{		
			return FALSE;
		}

		spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		while (SetupDiEnumDeviceInfo(hDevInfo, wIndex++, &spDevInfoData))
		{
			AddDevice(device_list, hDevInfo, wIndex, spDevInfoData);			
		}		

		return TRUE;
		
	}

}