/*!
* \file cactus_device.h
* \brief 设备枚举声明 
* 
* 设备枚举声明
* 
* \author wangyang 
* \date 2015/01/30 
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_DEVICE_H__
#define __CACTUS_CACTUS_DEVICE_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <SetupAPI.h>

namespace cactus
{

    enum DrvStatsOnDev : UINT32
    {
        DrvInstedOnDev = 0,		///> 已安装
        DrvNeedInstOnDev,		///> 未安装
        DrvFailedInstOnDev,		///> 安装失败
        DrvFinishInstOnDev,		///> 安装结束（需要重启来完成）
    };

	BOOL CACTUS_API GetPhysicalDeviceObjectNameBySearchCarmera(
		string16& strPhysiceDeviceObjcetName,
		string16& strDeviceDesc);


    UINT32 CACTUS_API Setupapi_GetDrvStatsOnDev(int nProblemCode, const string16& strGuidValue);
    UINT32 CACTUS_API Setupapi_GetDrvStatsOnDev(int nProblemCode, GUID& guid );

    int CACTUS_API Cmapi_GetDeviceProblemCode(const string16& instance_id);
    DWORD CACTUS_API GetDeviceProblemCodeByInstanceID(const string16& instance_id, ULONG& uProblem);

    DWORD CACTUS_API RefreshDevice();

	
}

namespace cactus
{
	class CACTUS_API Device
	{
	public:
		Device(GUID guid, string16 name, string16 instance_id, string16 path, int wIndex, int wOrder)			
			: guid_(guid), name_(name), instance_id_(instance_id), path_(path), wIndex_(wIndex), wOrder_(wOrder)
		{
		}
		~Device() {};
	public:
		GUID	guid_;
		int	wOrder_;
		int	wIndex_;
		string16 instance_id_;
		string16 name_;
		string16 path_;
	};

	BOOL AddDevice(std::vector<Device>& device_list, HDEVINFO hDevInfo, int index, SP_DEVINFO_DATA& DevInfoData);
	BOOL InitDeviceList(std::vector<Device>& device_list);
}

#endif // __CACTUS_CACTUS_DEVICE_H__
