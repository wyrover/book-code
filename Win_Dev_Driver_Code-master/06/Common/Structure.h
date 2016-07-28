#ifndef _STRUCTURE_H___
#define _STRUCTURE_H___

#define CHUNK_SIZE	64

// 应用程序用这个结构体传递upload请求给驱动
typedef struct{
	WORD  addr;// 其实地址
	WORD  len; // 缓冲长度
	//UCHAR data[1];// 缓冲区
}FIRMWARE_UPLOAD, *PFIRMWARE_UPLOAD;

// 取得string描述符
typedef struct _GET_STRING_DESCRIPTOR
{
   UCHAR    Index;
   USHORT   LanguageId;
} GET_STRING_DESCRIPTOR, *PGET_STRING_DESCRIPTOR;

// App通过这个结构体向驱动传递Set Feature命令。
typedef struct _SET_FEATURE_CONTROL
{
	USHORT FeatureSelector;
	USHORT Index;
	BOOLEAN bSetOrClear;// TRUE: Set; FALSE: Clear.
} SET_FEATURE_CONTROL, *PSET_FEATURE_CONTROL;

typedef enum{
	ENTERD0,
	EXITD0,
	SURPRISE_REMOVE,
	DRIVER_SYNC_EXIT,
	DRIVER_SYNC_OTHER
}DRIVER_SYNC_ORDER_TYPE;

// 同步信息数据结构
typedef struct{
	DRIVER_SYNC_ORDER_TYPE type; // 信息类型
	int info; // 信息内容
}DriverSyncPackt, *PDriverSyncPackt;

// 下面的三个结构用来获取驱动程序的版本信息
typedef enum {
	DR_WDF,
	DR_WDM
}DRIVER_TYPE;

typedef enum {
	FW_CY001,
	FW_NOT_CY001
}FIRMWARE_TYPE;

typedef struct{
	DRIVER_TYPE DriverType; // WDF or WDM
	FIRMWARE_TYPE FirmwareType;// USB or Firmware
}DRIVER_VERSION, *PDRIVER_VERSION;

// 特殊类/Vendor自定义请求控制块
typedef struct 
{
	union {
      struct {
        BYTE  recepient:2;
        BYTE  Reserved:3;
        BYTE  Type:2;
        BYTE  bDirInput:1;
      } Request;
      BYTE  type;
    } type;

	UCHAR req;
	USHORT value;
	USHORT index;
	USHORT length;
	char   *buf;// 长度由length决定
}USB_CTL_REQ, *PUSB_CTL_REQ;

#endif