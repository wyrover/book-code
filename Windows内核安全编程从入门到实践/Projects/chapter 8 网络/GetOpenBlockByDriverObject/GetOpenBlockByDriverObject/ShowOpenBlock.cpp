//
//Support: Xp sp3(NDIS 5.1)¡¢Win7 7600(NDIS 6.20)
//
//Create: 2010/7/31
//
//LastWrite:2010/7/31


#include "stdafx.h"



VOID
ShowOpenBlock(PDRIVER_OBJECT DriverObject)
{
    BOOLEAN                     bRet = FALSE;
    GlobolData                  gData = {0};

    do {
        bRet = InitGlobolData(&gData);

        if (FALSE == bRet) {
            break;
        }

        bRet = ValidateLegacyVerion(&gData.Ver);

        if (FALSE == bRet) {
            break;
        }

        bRet = GetMDriverBlockList(&gData);

        if (FALSE == bRet) {
            break;
        }

        bRet = (gData.Ver.MajorNdisVersion > 5) ? ShowOpenBlockBy60MDriverBlock(&gData) : ShowOpenBlockBy50MDriverBlock(&gData);

        if (FALSE == bRet) {
            break;
        }

        bRet = DeInitGlobolData(&gData);

        if (FALSE == bRet) {
            break;
        }
    } while (FALSE);

    return ;
}


BOOLEAN
InitGlobolData(GlobolData *gpData)
{
    BOOLEAN                 bRet = FALSE;
    PRTL_OSVERSIONINFOW     lpVersionInformation = NULL;
    UINT                    uNdisVer = 0;

    do {
        lpVersionInformation = (PRTL_OSVERSIONINFOW)ExAllocatePoolWithTag(
                                   NonPagedPool,
                                   sizeof(RTL_OSVERSIONINFOW),
                                   'wjl'
                               );

        if (lpVersionInformation == NULL) {
            bRet = FALSE;
            break;
        }

        RtlGetVersion(lpVersionInformation);
        gpData->Ver.OsMajorVersion = lpVersionInformation->dwMajorVersion;
        gpData->Ver.OsMinorVersion = lpVersionInformation->dwMinorVersion;
        ExFreePool(lpVersionInformation);
        uNdisVer = NdisGetVersion();
        gpData->Ver.MajorNdisVersion = uNdisVer >> 16;
        gpData->Ver.MiniorNdisVersion = uNdisVer & 0x1111;
        KdPrintEx((xx, "[InitGlobolData]\nOsMajorVersion:%d OsMinorVersion:%d\
					  \nMajorNdisVersion: %d MiniorNdisVersion:%d\n",
                   gpData->Ver.OsMajorVersion, gpData->Ver.OsMinorVersion,
                   gpData->Ver.MajorNdisVersion, gpData->Ver.MiniorNdisVersion));
        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOLEAN
ValidateLegacyVerion(Version *Ver)
{
    BOOLEAN     bRet = FALSE;

    do {
        if (Ver->OsMajorVersion < 5 || Ver->OsMajorVersion > 6) {
            bRet = FALSE;
            break;
        }

        if (Ver->MajorNdisVersion < 4 || Ver->MajorNdisVersion > 6) {
            bRet = FALSE;
            break;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}


BOOLEAN
GetMDriverBlockList(GlobolData *gpData)
{
    BOOLEAN                     bRet = FALSE;
    NTSTATUS                    ntStatus = STATUS_SUCCESS;
    UNICODE_STRING              usDriverName, usFuncName;
    PVOID                       pObj = NULL, pEntry = NULL;
    OBJECT_DIRECTORY_ENTRY      *pEntryItem = NULL;
    PDRIVER_OBJECT              pDrvObj = NULL;
    ULONG                       ClientDriverExtension = 0, ClientIdentificationAddress = 0;
    ULONG                       uDirectoryObjectType = 0;
    PVOID                       pObGetObjectType = NULL, pFakeDirObj = NULL;
    PVOID                       pMDriverBlock = NULL;
    ndisMiniDriverList_Item     *pItem = NULL;

    do {
        if (gpData->Ver.OsMajorVersion > 5) {
            RtlInitUnicodeString(&usFuncName, L"ObGetObjectType");
            pObGetObjectType = MmGetSystemRoutineAddress(&usFuncName);

            if (NULL == pObGetObjectType) {
                bRet = FALSE;
                break;
            }

            pfnObGetObjectType _ObGetObjectType = (pfnObGetObjectType)pObGetObjectType;
            pFakeDirObj = ExAllocatePoolWithTag(NonPagedPool, 0x10, 'wjl');

            if (NULL == pFakeDirObj) {
                bRet = FALSE;
                break;
            }

            *(UCHAR*)pFakeDirObj = ObpDirectoryObjectTypeIndex;
            uDirectoryObjectType =
                _ObGetObjectType((PVOID)ObjectHeaderTypeIndexToObject((ULONG)pFakeDirObj));
            ExFreePool(pFakeDirObj);

            if (uDirectoryObjectType == 0) {
                bRet = FALSE;
                break ;
            }

            KdPrintEx((xx, "[GetMDriverBlockList] uDirectoryObjectType: 0x%08x\n", uDirectoryObjectType));
        } else {
            uDirectoryObjectType = (ULONG) * IoFileObjectType;
        }

        RtlInitUnicodeString(&usDriverName, L"\\Driver");
        ntStatus = ObReferenceObjectByName(&usDriverName, OBJ_CASE_INSENSITIVE, NULL,
                                           0, (POBJECT_TYPE)uDirectoryObjectType, KernelMode, NULL, (PVOID*)&pObj);

        if (!NT_SUCCESS(ntStatus)) {
            KdPrintEx((xx, "[GetMDriverBlockList]ObReferenceObjectByName %wZ Failure,Status:0x%08x!\n",
                       &usDriverName, ntStatus));
            break ;
        }

        pEntry = pObj;

        for (int i = 0; i < ObjectDirectoryEntryCount; ++i) {
            pEntryItem = *(OBJECT_DIRECTORY_ENTRY**)pEntry;

            while (pEntryItem != NULL) {
                do {
                    pDrvObj = (PDRIVER_OBJECT)pEntryItem->Object;

                    if (pDrvObj == NULL) {
                        break;
                    }

                    //KdPrintEx((xx,"[GetMDriverBlockList]%wZ\n",&pDrvObj->DriverName));
                    ClientDriverExtension = *(ULONG*)((ULONG)pDrvObj->DriverExtension + ClientDriverExtensionOffset);

                    if (ClientDriverExtension == 0) {
                        break;
                    }

                    ClientIdentificationAddress = *(ULONG*)(ClientDriverExtension + ClientIdentificationAddressOffset);

                    if (ClientIdentificationAddress == MinoprtDriverSignal) {
                        pMDriverBlock = (PVOID)ClientDriverExtensionToMDriverBlock(ClientDriverExtension);
                        //KdPrintEx((xx,"[GetMDriverBlockList]it is a miniport driver!M_DRIVER_BLOCK: 0x%08x\n",
                        //  pMDriverBlock));
                        pItem = (ndisMiniDriverList_Item*)ExAllocatePoolWithTag(
                                    NonPagedPool,
                                    sizeof(ndisMiniDriverList_Item),
                                    'wjl'
                                );

                        if (NULL == pItem) {
                            break;
                        }

                        pItem->Next = gpData->ndisMiniDriverList;
                        pItem->MDriverBlock = pMDriverBlock;
                        gpData->ndisMiniDriverList = (PVOID)pItem;
                    }
                } while (FALSE);

                pEntryItem = (OBJECT_DIRECTORY_ENTRY*)pEntryItem->ChainLink;
            }

            pEntry = (PVOID)((ULONG)pEntry + 4);
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}

BOOLEAN
ShowOpenBlockBy50MDriverBlock(GlobolData *gpData)
{
    BOOLEAN                 bRet = FALSE;
    ndisMiniDriverList_Item *pListItem = NULL;
    NDIS_50_M_Driver_Block  *pMDriverBlock = NULL;
    NDIS_50_Miniport_Block  *pMiniportBlock = NULL;
    NDIS_50_Open_Block      *pOpenBlock = NULL;
    CHAR MediumArray[][30] = {
        "NdisMedium802_3", "NdisMedium802_5", "NdisMediumFddi", "NdisMediumWan",
        "NdisMediumLocalTalk", "NdisMediumDix", "NdisMediumArcnetRaw", "NdisMediumArcnet878_2",
        "NdisMediumAtm", "NdisMediumWirelessWan", "NdisMediumIrda", "NdisMediumBpc",
        "NdisMediumCoWan", "NdisMedium1394", "NdisMediumInfiniBand", "NdisMediumTunnel",
        "NdisMediumNative802_11", "NdisMediumLoopback", "NdisMediumWiMAX", "NdisMediumIP"
    };

    do {
        KdPrintEx((xx, "[ShowOpenBlockBy50MDriverBlock]\n"));
        pListItem = (ndisMiniDriverList_Item*)gpData->ndisMiniDriverList;

        while (NULL != pListItem) {
            pMDriverBlock = (NDIS_50_M_Driver_Block*)pListItem->MDriverBlock;

            if (NULL != pMDriverBlock) {
                KdPrintEx((xx, "\n_M_DRIVER_BLOCK(0x%08x):\n", pMDriverBlock));
                KdPrintEx((xx, "ServiceRegPath:%wZ\n", (ULONG)pMDriverBlock->NdisDriverInfo + 0x04));

                if (pMDriverBlock->AssociatedProtocol) {
                    KdPrintEx((xx, "AssociatedProtocol(0x%08x): %wZ\n",
                               pMDriverBlock->AssociatedProtocol,
                               (ULONG)pMDriverBlock->AssociatedProtocol + NdisProtocolBlock50NameOffset));
                }

                pMiniportBlock = (NDIS_50_Miniport_Block*)pMDriverBlock->MiniportQueue;

                while (pMiniportBlock) {
                    KdPrintEx((xx, "\tMiniport_Block(0x%08x):\n", pMiniportBlock));
                    KdPrintEx((xx, "\tMiniportName: %wZ MediaType: %s\n",
                               &pMiniportBlock->MiniportName, MediumArray[pMiniportBlock->MediaType]));
                    pOpenBlock = (NDIS_50_Open_Block*)pMiniportBlock->OpenQueue;

                    while (pOpenBlock) {
                        KdPrintEx((xx, "\t\tOpenQueue(0x%08x):\n", pOpenBlock));
                        KdPrintEx((xx, "\t\tBindDeviceName: %wZ\n", pOpenBlock->BindDeviceName));
                        KdPrintEx((xx, "\t\tRootDeviceName:%wZ\n", pOpenBlock->RootDeviceName));
                        KdPrintEx((xx, "\t\tProtocolHandle:0x%08x MiniportHandle:0x%08x\n",
                                   pOpenBlock->ProtocolHandle, pOpenBlock->MiniportHandle));
                        KdPrintEx((xx, "\t\tProtocolName: %wZ\n",
                                   (ULONG)pOpenBlock->ProtocolHandle + NdisProtocolBlock50NameOffset));
                        pOpenBlock = (NDIS_50_Open_Block*)pOpenBlock->MiniportNextOpen;
                    }

                    pMiniportBlock = (NDIS_50_Miniport_Block*)pMiniportBlock->NextMiniport;
                }
            }

            pListItem = (ndisMiniDriverList_Item*)pListItem->Next;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}



BOOLEAN
ShowOpenBlockBy60MDriverBlock(GlobolData *gpData)
{
    BOOLEAN                 bRet = FALSE;
    ndisMiniDriverList_Item *pListItem = NULL;
    NDIS_60_M_Driver_Block  *pMDriverBlock = NULL;
    NDIS_60_Miniport_Block  *pMiniportBlock = NULL;
    NDIS_60_Open_Block      *pOpenBlock = NULL;
    CHAR MediumArray[][30] = {
        "NdisMedium802_3", "NdisMedium802_5", "NdisMediumFddi", "NdisMediumWan",
        "NdisMediumLocalTalk", "NdisMediumDix", "NdisMediumArcnetRaw", "NdisMediumArcnet878_2",
        "NdisMediumAtm", "NdisMediumWirelessWan", "NdisMediumIrda", "NdisMediumBpc",
        "NdisMediumCoWan", "NdisMedium1394", "NdisMediumInfiniBand", "NdisMediumTunnel",
        "NdisMediumNative802_11", "NdisMediumLoopback", "NdisMediumWiMAX", "NdisMediumIP"
    };

    do {
        KdPrintEx((xx, "[ShowOpenBlockBy60MDriverBlock]\n"));
        pListItem = (ndisMiniDriverList_Item*)gpData->ndisMiniDriverList;

        while (NULL != pListItem) {
            pMDriverBlock = (NDIS_60_M_Driver_Block*)pListItem->MDriverBlock;

            if (NULL != pMDriverBlock) {
                KdPrintEx((xx, "\n_M_DRIVER_BLOCK(0x%08x):\n", pMDriverBlock));

                if (NULL != pMDriverBlock->ServiceRegPath.Buffer) {
                    KdPrintEx((xx, "ServiceRegPath: %wZ\n", &pMDriverBlock->ServiceRegPath));
                } else {
                    KdPrintEx((xx, "ServiceRegPath:%wZ\n", (ULONG)pMDriverBlock->NdisDriverInfo + 0x04));
                }

                if (pMDriverBlock->AssociatedProtocol) {
                    KdPrintEx((xx, "AssociatedProtocol(0x%08x): %wZ\n",
                               pMDriverBlock->AssociatedProtocol,
                               (ULONG)pMDriverBlock->AssociatedProtocol + NdisProtocolBlock60NameOffset));
                }

                pMiniportBlock = (NDIS_60_Miniport_Block*)pMDriverBlock->MiniportQueue;

                while (pMiniportBlock) {
                    KdPrintEx((xx, "\tMiniport_Block(0x%08x):\n", pMiniportBlock));
                    KdPrintEx((xx, "\tMiniportName: %wZ MediaType: %s\n",
                               &pMiniportBlock->MiniportName, MediumArray[pMiniportBlock->MediaType]));
                    pOpenBlock = (NDIS_60_Open_Block*)pMiniportBlock->OpenQueue;

                    while (pOpenBlock) {
                        KdPrintEx((xx, "\t\tOpenQueue(0x%08x):\n", pOpenBlock));
                        KdPrintEx((xx, "\t\tBindDeviceName: %wZ\n", pOpenBlock->BindDeviceName));
                        KdPrintEx((xx, "\t\tRootDeviceName:%wZ\n", pOpenBlock->RootDeviceName));
                        KdPrintEx((xx, "\t\tProtocolHandle:0x%08x MiniportHandle:0x%08x\n",
                                   pOpenBlock->ProtocolHandle, pOpenBlock->MiniportHandle));
                        KdPrintEx((xx, "\t\tProtocolName: %wZ\n",
                                   (ULONG)pOpenBlock->ProtocolHandle + NdisProtocolBlock60NameOffset));
                        pOpenBlock = (NDIS_60_Open_Block*)pOpenBlock->MiniportNextOpen;
                    }

                    pMiniportBlock = (NDIS_60_Miniport_Block*)pMiniportBlock->NextMiniport;
                }
            }

            pListItem = (ndisMiniDriverList_Item*)pListItem->Next;
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}


BOOLEAN
DeInitGlobolData(GlobolData *gpData)
{
    BOOLEAN     bRet = FALSE;
    PVOID       pItem = NULL;

    do {
        while (gpData->ndisMiniDriverList) {
            pItem = gpData->ndisMiniDriverList;
            gpData->ndisMiniDriverList = ((ndisMiniDriverList_Item*)pItem)->Next;
            ExFreePool(pItem);
        }

        bRet = TRUE;
    } while (FALSE);

    return bRet;
}