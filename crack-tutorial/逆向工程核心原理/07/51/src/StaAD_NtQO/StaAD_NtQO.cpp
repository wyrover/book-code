#include "stdio.h"
#include "windows.h"
#include "tchar.h"

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectAllTypesInformation,
    ObjectHandleInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

void MyNtQueryObject()
{
    typedef struct _LSA_UNICODE_STRING {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR Buffer;
    } LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

    typedef NTSTATUS (WINAPI *NTQUERYOBJECT)(
        HANDLE Handle,
        OBJECT_INFORMATION_CLASS ObjectInformationClass,
        PVOID ObjectInformation,
        ULONG ObjectInformationLength,
        PULONG ReturnLength
    );
    
    #pragma pack(1)
    typedef struct _OBJECT_TYPE_INFORMATION {
        UNICODE_STRING TypeName;
        ULONG TotalNumberOfHandles;
        ULONG TotalNumberOfObjects;
    }OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

    typedef struct _OBJECT_ALL_INFORMATION {
        ULONG                   NumberOfObjectsTypes;
        OBJECT_TYPE_INFORMATION ObjectTypeInformation[1];
    } OBJECT_ALL_INFORMATION, *POBJECT_ALL_INFORMATION;
    #pragma pack()
       
    POBJECT_ALL_INFORMATION pObjectAllInfo = NULL;
    void *pBuf = NULL;
    ULONG lSize = 0;
    BOOL bDebugging = FALSE;

    NTQUERYOBJECT pNtQueryObject = (NTQUERYOBJECT)
                                    GetProcAddress(GetModuleHandle(L"ntdll.dll"), 
                                                   "NtQueryObject");

    // Get the size of the list
    pNtQueryObject(NULL, ObjectAllTypesInformation, &lSize, sizeof(lSize), &lSize);

    // Allocate list buffer
    pBuf = VirtualAlloc(NULL, lSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // Get the actual list
    pNtQueryObject((HANDLE)0xFFFFFFFF, ObjectAllTypesInformation, pBuf, lSize, NULL);

    pObjectAllInfo = (POBJECT_ALL_INFORMATION)pBuf;

    UCHAR *pObjInfoLocation = (UCHAR *)pObjectAllInfo->ObjectTypeInformation;
    POBJECT_TYPE_INFORMATION pObjectTypeInfo = NULL;
    for( UINT i = 0; i < pObjectAllInfo->NumberOfObjectsTypes; i++ )
    {
        pObjectTypeInfo = (POBJECT_TYPE_INFORMATION)pObjInfoLocation;
        if( wcscmp(L"DebugObject", pObjectTypeInfo->TypeName.Buffer) == 0 )
        {
            bDebugging = (pObjectTypeInfo->TotalNumberOfObjects > 0) ? TRUE : FALSE;
            break;
        }
        
        // calculate next struct
        pObjInfoLocation = (UCHAR*)pObjectTypeInfo->TypeName.Buffer;
        pObjInfoLocation += pObjectTypeInfo->TypeName.Length;
        pObjInfoLocation = (UCHAR*)(((ULONG)pObjInfoLocation & 0xFFFFFFFC) + sizeof(ULONG));
    }

    if( pBuf )
    VirtualFree(pBuf, 0, MEM_RELEASE);

    printf("NtQueryObject(ObjectAllTypesInformation)\n");
    if( bDebugging )  printf("  => Debugging!!!\n\n");
    else              printf("  => Not debugging...\n\n");
}

int _tmain(int argc, TCHAR* argv[])
{
    MyNtQueryObject();

    printf("\npress any key to quit...\n");
    _gettch();

    return 0;
}