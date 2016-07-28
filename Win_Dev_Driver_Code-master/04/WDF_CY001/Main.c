/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：Main.c
	说明：入口函数
	历史：
	创建：
*/

#include "public.h"

#pragma alloc_text(INIT, DriverEntry)

#define WDF_USB

NTSTATUS DriverEntry(
    IN PDRIVER_OBJECT  DriverObject, 
    IN PUNICODE_STRING  RegistryPath
    )
{
  WDF_DRIVER_CONFIG config;
  NTSTATUS status = STATUS_SUCCESS;

  // 编译变量__DATE__, __TIME__在Checked环境中有效
  // 表示编译时候的时间戳
  KDBG(DPFLTR_INFO_LEVEL, "[***CY001.sys DriverEntry ***]");
  KDBG(DPFLTR_INFO_LEVEL, "Compiled time: %s (%s)", __DATE__, __TIME__);

  //
  // 我这里使用了一个编译判断条件语句。用意是在同一个驱动中，既使用WDF有使用WDM方式编程。

#ifdef WDF_USB

  // 创建WDF框架驱动对象。这个对象我们永远用不到，也不被暴露。但他确实存在着。
  // 我们可以把它看成一只隐藏着的手吧。
  // DriverEntry里面的这种初始化工作，和所有的小端口驱动一模一样，简直别无二致。
  // 读者可以据此参看诸如Scasi、文件过滤、NDIS、StreamPort等类型的小端口驱动，是如何运作的。
  WDF_DRIVER_CONFIG_INIT(&config, PnpAdd);

  status = WdfDriverCreate(DriverObject, // WDF驱动对象
                      RegistryPath,
                      WDF_NO_OBJECT_ATTRIBUTES,
                      &config, // 配置参数
                      WDF_NO_HANDLE);

  //gDriverObj = DriverObject; // 如果需要，可以把驱动对象保存在全局变量中
  
#else
	//在此处定义WDM驱动分发回调函数
#endif

  return status;
}
