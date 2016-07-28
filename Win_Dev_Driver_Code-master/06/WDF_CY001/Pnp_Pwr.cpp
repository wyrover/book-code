/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：Pnp_Pwr.c
	说明：PNP与电源处理
	历史：
	创建：
*/
#ifdef __cplusplus
extern "C"{
#endif

#include "public.h"

#ifdef __cplusplus
}
#endif

#include "DrvClass.h"
#pragma code_seg("PAGE")

//下面两个Power回调，和WDM中的PnpSetPower类似。
NTSTATUS DrvClass::PwrD0Entry(IN WDF_POWER_DEVICE_STATE  PreviousState)
{

}

// 离开D0状态
NTSTATUS DrvClass::PwrD0Exit(IN WDF_POWER_DEVICE_STATE  TargetState)
{
}

VOID DrvClass::PnpSurpriseRemove(IN WDFDEVICE  Device)
{
	CompleteSyncRequest(Device, SURPRISE_REMOVE, 0);
}

