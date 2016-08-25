#include <windows.h>
#include <stdio.h>
//使用CTL_CODE必须加入winioctl.h
#include <winioctl.h>
#include "..\NT_Driver\Ioctls.h"


DWORD In_32(HANDLE hDevice,USHORT port)
{
	DWORD dwOutput ;
	DWORD inputBuffer[2] =
	{
		port,//对port进行操作
		4//1代表8位操作，2代表16位操作，4代表32位操作
	};
	DWORD dResult;

	DeviceIoControl(hDevice, READ_PORT, inputBuffer, sizeof(inputBuffer), &dResult, sizeof(DWORD), &dwOutput, NULL);

	return dResult;
	
}
void Out_32(HANDLE hDevice,USHORT port,DWORD value)
{
	DWORD dwOutput ;
	DWORD inputBuffer[3] =
	{
		port,//对port进行操作
		4,//1代表8位操作，2代表16位操作，4代表32位操作
		value//输出字节
	};

	DeviceIoControl(hDevice, WRITE_PORT, inputBuffer, sizeof(inputBuffer), NULL, 0, &dwOutput, NULL);
}

/* PCI配置空间寄存器 */
#define PCI_CONFIG_ADDRESS      0xCF8
#define PCI_CONFIG_DATA         0xCFC


#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2
#define PCI_TYPE2_ADDRESSES             5

typedef struct _PCI_COMMON_CONFIG {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test

    union {
        struct _PCI_HEADER_TYPE_0 {
            ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
            ULONG   CIS;
            USHORT  SubVendorID;
            USHORT  SubSystemID;
            ULONG   ROMBaseAddress;
            UCHAR   CapabilitiesPtr;
            UCHAR   Reserved1[3];
            ULONG   Reserved2;
            UCHAR   InterruptLine;      //
            UCHAR   InterruptPin;       // (ro)
            UCHAR   MinimumGrant;       // (ro)
            UCHAR   MaximumLatency;     // (ro)
        } type0;

// end_wdm end_ntminiport end_ntndis

        //
        // PCI to PCI Bridge
        //

        struct _PCI_HEADER_TYPE_1 {
            ULONG   BaseAddresses[PCI_TYPE1_ADDRESSES];
            UCHAR   PrimaryBus;
            UCHAR   SecondaryBus;
            UCHAR   SubordinateBus;
            UCHAR   SecondaryLatency;
            UCHAR   IOBase;
            UCHAR   IOLimit;
            USHORT  SecondaryStatus;
            USHORT  MemoryBase;
            USHORT  MemoryLimit;
            USHORT  PrefetchBase;
            USHORT  PrefetchLimit;
            ULONG   PrefetchBaseUpper32;
            ULONG   PrefetchLimitUpper32;
            USHORT  IOBaseUpper16;
            USHORT  IOLimitUpper16;
            UCHAR   CapabilitiesPtr;
            UCHAR   Reserved1[3];
            ULONG   ROMBaseAddress;
            UCHAR   InterruptLine;
            UCHAR   InterruptPin;
            USHORT  BridgeControl;
        } type1;

        //
        // PCI to CARDBUS Bridge
        //

        struct _PCI_HEADER_TYPE_2 {
            ULONG   SocketRegistersBaseAddress;
            UCHAR   CapabilitiesPtr;
            UCHAR   Reserved;
            USHORT  SecondaryStatus;
            UCHAR   PrimaryBus;
            UCHAR   SecondaryBus;
            UCHAR   SubordinateBus;
            UCHAR   SecondaryLatency;
            struct  {
                ULONG   Base;
                ULONG   Limit;
            }       Range[PCI_TYPE2_ADDRESSES-1];
            UCHAR   InterruptLine;
            UCHAR   InterruptPin;
            USHORT  BridgeControl;
        } type2;

// begin_wdm begin_ntminiport begin_ntndis

    } u;

    UCHAR   DeviceSpecific[192];

} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;

typedef struct _PCI_SLOT_NUMBER {
    union {
        struct {
            ULONG   FunctionNumber:3;
			ULONG   DeviceNumber:5;
            ULONG   Reserved:24;
        } bits;
        ULONG   AsULONG;
    } u;
} PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;

void DisplayPCIConfiguation(HANDLE hDevice,int bus,int dev,int func)
{
	DWORD	dwAddr;
	DWORD	dwData;

	PCI_COMMON_CONFIG pci_config;
	PCI_SLOT_NUMBER SlotNumber;

	SlotNumber.u.AsULONG = 0;
	SlotNumber.u.bits.DeviceNumber = dev;
	SlotNumber.u.bits.FunctionNumber = func;

	dwAddr = 0x80000000 | (bus <<16) | (SlotNumber.u.AsULONG<<8);
	
	/* 256字节的PCI配置空间 */
	for (int i = 0; i < 0x100; i += 4)	
	{
		/* Read */
		Out_32(hDevice,PCI_CONFIG_ADDRESS, dwAddr | i);
		dwData = In_32(hDevice,PCI_CONFIG_DATA);
		memcpy( ((PUCHAR)&pci_config)+i,&dwData,4);
	}
	
	printf("bus:%d\tdev:%d\tfunc:%d\n",bus,dev,func);

	printf("VendorID:%x\n",pci_config.VendorID);
	printf("DeviceID:%x\n",pci_config.DeviceID);
	printf("Command:%x\n",pci_config.Command);
	printf("Status:%x\n",pci_config.Status);
	printf("RevisionID:%x\n",pci_config.RevisionID);
	printf("ProgIf:%x\n",pci_config.ProgIf);
	printf("SubClass:%x\n",pci_config.SubClass);
	printf("BaseClass:%x\n",pci_config.BaseClass);
	printf("CacheLineSize:%x\n",pci_config.CacheLineSize);
	printf("LatencyTimer:%x\n",pci_config.LatencyTimer);
	printf("HeaderType:%x\n",pci_config.HeaderType);
	printf("BIST:%x\n",pci_config.BIST);
	for (i=0;i<6;i++)
	{
		printf("BaseAddresses[%d]:0X%08X\n",i,pci_config.u.type0.BaseAddresses[i]);
	}
	printf("InterruptLine:%d\n",pci_config.u.type0.InterruptLine);
	printf("InterruptPin:%d\n",pci_config.u.type0.InterruptPin);
}

int main()
{
	HANDLE hDevice = 
		CreateFile("\\\\.\\HelloDDK",
					GENERIC_READ | GENERIC_WRITE,
					0,		// share mode none
					NULL,	// no security
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL );		// no template

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Failed to obtain file handle to device: "
			"%s with Win32 error code: %d\n",
			"MyWDMDevice", GetLastError() );
		return 1;
	}

	DisplayPCIConfiguation(hDevice,2,1,0);

	CloseHandle(hDevice);

	return 0;
}