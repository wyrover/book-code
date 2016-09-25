#include <ntddk.h>

/*
    以上代码为字符串操作测试
*/

//1.字符串初始化
VOID StringInitTest()
{
    //(1)用RtlInitAnsiString初始化字符串
    {
        ANSI_STRING  AnsiString1;
        CHAR * string1 = "hello";
        //初始化ANSI_STRING字符串
        RtlInitAnsiString(&AnsiString1, string1);
        DbgPrint("AnsiString1:%Z\n", &AnsiString1); //打印hello
        string1[0] = 'H';
        string1[1] = 'E';
        string1[2] = 'L';
        string1[3] = 'L';
        string1[4] = 'O';
        //改变string1，AnsiString1同样会导致变化
        DbgPrint("AnsiString1:%Z\n", &AnsiString1); //打印HELLO
    }
    //(2)程序员自己初始化字符串
    {
#define BUFFER_SIZE 1024
        UNICODE_STRING UnicodeString1 = {0};
        WCHAR* wideString = L"hello";
        //设置缓冲区大小
        UnicodeString1.MaximumLength = BUFFER_SIZE;
        //分配内存
        UnicodeString1.Buffer = (PWSTR)ExAllocatePool(PagedPool, BUFFER_SIZE);
        //设置字符长度,因为是宽字符，所以是字符长度的2倍
        UnicodeString1.Length = 2 * wcslen(wideString);
        //保证缓冲区足够大，否则程序终止
        ASSERT(UnicodeString1.MaximumLength >= UnicodeString1.Length);
        //内存拷贝，
        RtlCopyMemory(UnicodeString1.Buffer, wideString, UnicodeString1.Length);
        //设置字符长度
        UnicodeString1.Length = 2 * wcslen(wideString);
        DbgPrint("UnicodeString:%wZ\n", &UnicodeString1);
        //清理内存
        ExFreePool(UnicodeString1.Buffer);
        UnicodeString1.Buffer = NULL;
        UnicodeString1.Length = UnicodeString1.MaximumLength = 0;
    }
}

//2.字符串拷贝
VOID StringCopyTest()
{
    UNICODE_STRING UnicodeString1;
    UNICODE_STRING UnicodeString2 = {0};
    //初始化UnicodeString1
    RtlInitUnicodeString(&UnicodeString1, L"Hello World");
    //初始化UnicodeString2
    UnicodeString2.Buffer = (PWSTR)ExAllocatePool(PagedPool, BUFFER_SIZE);
    UnicodeString2.MaximumLength = BUFFER_SIZE;
    //将初始化UnicodeString2拷贝到UnicodeString1
    RtlCopyUnicodeString(&UnicodeString2, &UnicodeString1);
    //分别显示UnicodeString1和UnicodeString2
    DbgPrint("UnicodeString1:%wZ\n", &UnicodeString1);
    DbgPrint("UnicodeString2:%wZ\n", &UnicodeString2);
    //销毁UnicodeString2（UnicodeString1不用销毁）
    RtlFreeUnicodeString(&UnicodeString2);
}

//3.字符串比较
VOID StringCompareTest()
{
    UNICODE_STRING UnicodeString1;
    UNICODE_STRING UnicodeString2;
    //初始化UnicodeString1
    RtlInitUnicodeString(&UnicodeString1, L"Hello World");
    //初始化UnicodeString2
    RtlInitUnicodeString(&UnicodeString1, L"Hello");

    if (RtlEqualUnicodeString(&UnicodeString1, &UnicodeString2, TRUE))
        DbgPrint("UnicodeString1 and UnicodeString2 are equal\n");
    else
        DbgPrint("UnicodeString1 and UnicodeString2 are NOT equal\n");
}

//4.字符串变大写
VOID StringToUpperTest()
{
    //初始化UnicodeString1
    UNICODE_STRING UnicodeString1;
    UNICODE_STRING UnicodeString2;
    RtlInitUnicodeString(&UnicodeString1, L"Hello World");
    //变化前
    DbgPrint("UnicodeString1:%wZ\n", &UnicodeString1);
    //变大写
    RtlUpcaseUnicodeString(&UnicodeString2, &UnicodeString1, TRUE);
    //变化后
    DbgPrint("UnicodeString2:%wZ\n", &UnicodeString2);
    //销毁UnicodeString2（UnicodeString1不用销毁）
    RtlFreeUnicodeString(&UnicodeString2);
}

//5.字符串与整型相互转化
VOID StringToIntegerTest()
{
    //(1)字符串转换成数字
    {
        UNICODE_STRING UnicodeString1;
        ULONG lNumber;
        NTSTATUS nStatus;
        //初始化UnicodeString1
        RtlInitUnicodeString(&UnicodeString1, L"-100");
        nStatus = RtlUnicodeStringToInteger(&UnicodeString1, 10, &lNumber);

        if (NT_SUCCESS(nStatus)) {
            DbgPrint("Conver to integer succussfully!\n");
            DbgPrint("Result:%d\n", lNumber);
        } else {
            DbgPrint("Conver to integer unsuccessfully!\n");
        }
    }
    //(2)数字转换成字符串
    {
        NTSTATUS nStatus;
        UNICODE_STRING UnicodeString2 = {0};
        //初始化UnicodeString2
        UnicodeString2.Buffer = (PWSTR)ExAllocatePool(PagedPool, BUFFER_SIZE);
        UnicodeString2.MaximumLength = BUFFER_SIZE;
        nStatus = RtlIntegerToUnicodeString(200, 10, &UnicodeString2);

        if (NT_SUCCESS(nStatus)) {
            DbgPrint("Conver to string succussfully!\n");
            DbgPrint("Result:%wZ\n", &UnicodeString2);
        } else {
            DbgPrint("Conver to string unsuccessfully!\n");
        }

        //销毁UnicodeString2
        //注意!!UnicodeString1不用销毁
        RtlFreeUnicodeString(&UnicodeString2);
    }
}

//6. ANSI_STRING字符串与UNICODE_STRING字符串相互转换
VOID StringConverTest()
{
    //(1)将UNICODE_STRING字符串转换成ANSI_STRING字符串
    UNICODE_STRING UnicodeString1;
    ANSI_STRING AnsiString1;
    NTSTATUS nStatus;
    //初始化UnicodeString1
    RtlInitUnicodeString(&UnicodeString1, L"Hello World");
    nStatus = RtlUnicodeStringToAnsiString(&AnsiString1, &UnicodeString1, TRUE);

    if (NT_SUCCESS(nStatus)) {
        DbgPrint("Conver succussfully!\n");
        DbgPrint("Result:%Z\n", &AnsiString1);
    } else {
        DbgPrint("Conver unsuccessfully!\n");
    }

    //销毁AnsiString1
    RtlFreeAnsiString(&AnsiString1);
    //(2)将ANSI_STRING字符串转换成UNICODE_STRING字符串
    {
        ANSI_STRING AnsiString2;
        UNICODE_STRING UnicodeString2;
        NTSTATUS nStatus;
        //初始化AnsiString2
        RtlInitString(&AnsiString2, "Hello World");
        nStatus = RtlAnsiStringToUnicodeString(&UnicodeString2, &AnsiString2, TRUE);

        if (NT_SUCCESS(nStatus)) {
            DbgPrint("Conver succussfully!\n");
            DbgPrint("Result:%wZ\n", &UnicodeString2);
        } else {
            DbgPrint("Conver unsuccessfully!\n");
        }

        //销毁UnicodeString2
        RtlFreeUnicodeString(&UnicodeString2);
    }
}


/*
    以下代码为字符串转换代码
*/

//UNICODE_STRINGz转换为CHAR*
//输入UNICODE_STRING的指针，输出窄字符串，BUFFER需要已经分配好空间
VOID UnicodeToChar(PUNICODE_STRING dst, char *src)
{
    ANSI_STRING string;
    RtlUnicodeStringToAnsiString(&string, dst, TRUE);
    strcpy(src, string.Buffer);
    RtlFreeAnsiString(&string);
}
//WCHAR*转换为CHAR*
//输入宽字符串首地址，输出窄字符串，BUFFER需要已经分配好空间
VOID WcharToChar(PWCHAR src, PCHAR dst)
{
    UNICODE_STRING uString;
    ANSI_STRING aString;
    RtlInitUnicodeString(&uString, src);
    RtlUnicodeStringToAnsiString(&aString, &uString, TRUE);
    strcpy(dst, aString.Buffer);
    RtlFreeAnsiString(&aString);
}
//CHAR*转WCHAR*
//输入窄字符串首地址，输出宽字符串，BUFFER需要已经分配好空间
VOID CharToWchar(PCHAR src, PWCHAR dst)
{
    UNICODE_STRING uString;
    ANSI_STRING aString;
    RtlInitAnsiString(&aString, src);
    RtlAnsiStringToUnicodeString(&uString, &aString, TRUE);
    wcscpy(dst, uString.Buffer);
    RtlFreeUnicodeString(&uString);
}


/*
    以下代码为自动生成
*/

VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    DbgPrint("[MyDriver]Unloaded...\n");
    return;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;
    DbgPrint("[MyDriver]Loaded...\n");
    StringInitTest();
    StringCopyTest();
    StringCompareTest();
    StringToUpperTest();
    StringToIntegerTest();
    StringConverTest();
    return STATUS_SUCCESS;
}