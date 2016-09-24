/**
 * @file tdi_fw_lib
 * @author tan wen
 * @brief I add this file to simplise the including of tdi_fw.lib's 
 *		  header file. tdi_fw.lib is a library based on tdi_fw. You
 *		  can link this library to build a tdi filter driver easily.
 */

#ifndef _TDI_FW_LIB_HEADER_
#define _TDI_FW_LIB_HEADER_

#include <ntddk.h>
#include <tdikrnl.h>
#include "net.h"
#include "sock.h"
#include "ipc.h"

/** If you use tdi_fw.lib to build your tdi filter driver, you must write 
 *	tdifw_driver_entry(), tdifw_driver_unload(), tdifw_user_device_dispatch(),
 *  tdifw_filter(). You can't change these function's name or use their name
 *	to define other functions or vals. These functions' prototypes are as below.
*/

/** tdifw_driver_entry is just like DriverEntry. tdi_fw.lib already havs the 
 *  function which named DriverEntry. So when you is using tdi_fw.lib to develop
 *	a tdi neckwork filter, use tdifw_driver_entry instead. Don't to set driver's
 *	dispatch function here. tdi_fw has used them. But if you create any device
 *	here, you may use tdifw_register_user_device() to resigter them. When any
 *  irp is sending to your devices, tdifw_user_device_dispatch() will be called.
 *	You can dispatch your irp here.
*/
extern NTSTATUS
tdifw_driver_entry(
			IN PDRIVER_OBJECT theDriverObject,
            IN PUNICODE_STRING theRegistryPath);

/** tdifw_driver_unload is just like DriverUnload. It will be called when this
 *  driver is unloading. Delete all devices you created and free memory you 
 *  allocated here.
*/
extern VOID
tdifw_driver_unload(
			IN PDRIVER_OBJECT DriverObject);

/** tdifw_user_device_dispatch() will be called when any device which you 
 *	have created and used tdifw_register_user_device() to register got any
 *	irp. You should complete any such irp here. Don't passthru.
*/
extern NTSTATUS tdifw_user_device_dispatch(
	IN PDEVICE_OBJECT DeviceObject, IN PIRP irp);

/** These function will be call when any network event happends. You can choose
 *	interested event to filter. return FLT_ALLOW to passthru or FLT_DENY to 
 *  refuse this event. See the strcut flt_request for more details.
*/
extern int tdifw_filter(struct flt_request *request);

BOOLEAN tdifw_register_user_device(PDEVICE_OBJECT dev);

#endif // _TDI_FW_LIB_HEADER_
