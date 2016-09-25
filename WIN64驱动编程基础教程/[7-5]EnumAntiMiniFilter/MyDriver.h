
ULONG FltFilterOperationsOffset = 0x188; //WIN7 OFFSET of fltmgr!_FLT_FILTER->PFLT_OPERATION_REGISTRATION

typedef struct _FLT_OPERATION_REGISTRATION {
    UCHAR   MajorFunction;
    ULONG   Flags;
    PVOID   PreOperation;
    PVOID   PostOperation;
    PVOID   Reserved1;
} FLT_OPERATION_REGISTRATION, *PFLT_OPERATION_REGISTRATION;

typedef struct _FLT_FILTER {
    UCHAR buffer[1024];
} FLT_FILTER, *PFLT_FILTER;

NTSTATUS
__fastcall
FltEnumerateFilters
(
    PFLT_FILTER *FilterList,
    ULONG FilterListSize,
    PULONG NumberFiltersReturned
);

NTSTATUS
__fastcall
FltObjectDereference
(
    PVOID FltObject
);

ULONG EnumMiniFilter()
{
    long    ntStatus;
    ULONG   uNumber;
    PVOID   pBuffer = NULL;
    ULONG   uIndex = 0, DrvCount = 0;
    PVOID   pCallBacks, pFilter;
    PFLT_OPERATION_REGISTRATION pNode;

    do {
        if (pBuffer != NULL) {
            ExFreePool(pBuffer);
            pBuffer = NULL;
        }

        ntStatus = FltEnumerateFilters(NULL,  0, &uNumber);

        if (ntStatus != STATUS_BUFFER_TOO_SMALL)
            break;

        pBuffer = ExAllocatePoolWithTag(NonPagedPool, sizeof(PFLT_FILTER) * uNumber, 'mnft');

        if (pBuffer == NULL) {
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        ntStatus = FltEnumerateFilters(pBuffer, uNumber, &uNumber);
    } while (ntStatus == STATUS_BUFFER_TOO_SMALL);

    if (! NT_SUCCESS(ntStatus)) {
        if (pBuffer != NULL)
            ExFreePool(pBuffer);

        return 0;
    }

    DbgPrint("MiniFilter Count: %ld\n", uNumber);
    DbgPrint("------\n");

    __try {
        while (DrvCount < uNumber) {
            pFilter = (PVOID)(*(PULONG64)((PUCHAR)pBuffer + DrvCount * 8));
            pCallBacks = (PVOID)((PUCHAR)pFilter + FltFilterOperationsOffset);
            pNode = (PFLT_OPERATION_REGISTRATION)(*(PULONG64)pCallBacks);

            __try {
                while (pNode->MajorFunction != 0x80) { //IRP_MJ_OPERATION_END
                    if (pNode->MajorFunction < 28) { //MajorFunction id is 0~27
                        DbgPrint("Object=%p\tPreFunc=%p\tPostFunc=%p\tIRP=%d\n",
                                 pFilter,
                                 pNode->PreOperation,
                                 pNode->PostOperation,
                                 pNode->MajorFunction);
                    }

                    pNode++;
                }
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                FltObjectDereference(pFilter);
                DbgPrint("[EnumMiniFilter]EXCEPTION_EXECUTE_HANDLER: pNode->MajorFunction\n");
                ntStatus = GetExceptionCode();
                ExFreePool(pBuffer);
                return uIndex;
            }

            DrvCount++;
            FltObjectDereference(pFilter);
            DbgPrint("------\n");
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        FltObjectDereference(pFilter);
        DbgPrint("[EnumMiniFilter]EXCEPTION_EXECUTE_HANDLER\n");
        ntStatus = GetExceptionCode();
        ExFreePool(pBuffer);
        return uIndex;
    }

    if (pBuffer != NULL) {
        ExFreePool(pBuffer);
        ntStatus = STATUS_SUCCESS;
    }

    return uIndex;
}