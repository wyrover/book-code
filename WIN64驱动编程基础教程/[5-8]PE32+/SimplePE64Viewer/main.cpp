#include <stdio.h>
#include <Windows.h>
#include <IMAGEHLP.H>
#pragma comment(lib,"ImageHlp.lib")

void MyCls(HANDLE hConsole)
{
    COORD   coordScreen = {0, 0}; //设置清屏后光标返回的屏幕左上角坐标
    BOOL    bSuccess;
    DWORD   cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO   csbi;//保存缓冲区信息
    DWORD   dwConSize;//当前缓冲区可容纳的字符数
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi); //获得缓冲区信息
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y; //缓冲区容纳字符数目
    bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi); //获得缓冲区信息
    bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
    return;
}

void clrscr(void)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    MyCls(hStdOut);
    return;
}

DWORD FileLen(char *filename)
{
    WIN32_FIND_DATAA fileInfo = {0};
    DWORD fileSize = 0;
    HANDLE hFind;
    hFind = FindFirstFileA(filename , &fileInfo);

    if (hFind != INVALID_HANDLE_VALUE) {
        fileSize = fileInfo.nFileSizeLow;
        FindClose(hFind);
    }

    return fileSize;
}

CHAR *LoadFile(char *filename)
{
    DWORD dwReadWrite, LenOfFile = FileLen(filename);
    HANDLE hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

    if (hFile != INVALID_HANDLE_VALUE) {
        PCHAR buffer = (PCHAR)malloc(LenOfFile);
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        ReadFile(hFile, buffer, LenOfFile, &dwReadWrite, 0);
        CloseHandle(hFile);
        return buffer;
    }

    return NULL;
}

VOID ShowPE64Info(char *filename)
{
    PIMAGE_NT_HEADERS64 pinths64;
    PIMAGE_DOS_HEADER pdih;
    char *filedata;
    filedata = LoadFile(filename);
    pdih = (PIMAGE_DOS_HEADER)filedata;
    pinths64 = (PIMAGE_NT_HEADERS64)(filedata + pdih->e_lfanew);

    if (pinths64->Signature != 0x00004550) {
        printf("无效的PE文件！\n");
        return ;
    }

    if (pinths64->OptionalHeader.Magic != 0x20b) {
        printf("不是PE32+格式的文件！\n");
        return ;
    }

    printf("\n");
    printf("入口点：          %llx\n", pinths64->OptionalHeader.AddressOfEntryPoint);
    printf("镜像基址：        %llx\n", pinths64->OptionalHeader.ImageBase);
    printf("镜像大小：        %llx\n", pinths64->OptionalHeader.SizeOfImage);
    printf("代码基址：        %llx\n", pinths64->OptionalHeader.BaseOfCode);
    printf("块对齐：          %llx\n", pinths64->OptionalHeader.SectionAlignment);
    printf("文件块对齐：      %llx\n", pinths64->OptionalHeader.FileAlignment);
    printf("子系统：          %llx\n", pinths64->OptionalHeader.Subsystem);
    printf("区段数目：        %llx\n", pinths64->FileHeader.NumberOfSections);
    printf("时间日期标志：    %llx\n", pinths64->FileHeader.TimeDateStamp);
    printf("首部大小：        %llx\n", pinths64->OptionalHeader.SizeOfHeaders);
    printf("特征值：          %llx\n", pinths64->FileHeader.Characteristics);
    printf("校验和：          %llx\n", pinths64->OptionalHeader.CheckSum);
    printf("可选头部大小：    %llx\n", pinths64->FileHeader.SizeOfOptionalHeader);
    printf("RVA 数及大小：    %llx\n", pinths64->OptionalHeader.NumberOfRvaAndSizes);
    getchar();
    printf("\n");
    printf("输出表：\n");
    printf("Ordinal\tRVA\t\tName\n");
    PIMAGE_EXPORT_DIRECTORY pied;

    if (pinths64, pdih, pinths64->OptionalHeader.DataDirectory[0].VirtualAddress == 0) goto imp;

    pied = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, pinths64->OptionalHeader.DataDirectory[0].VirtualAddress, NULL);
    DWORD i = 0;
    DWORD NumberOfNames = pied->NumberOfNames;
    ULONGLONG **ppdwNames = (ULONGLONG **)pied->AddressOfNames;
    ppdwNames = (PULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (ULONG)ppdwNames, NULL);
    ULONGLONG **ppdwAddr  = (ULONGLONG **)pied->AddressOfFunctions;
    ppdwAddr = (PULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (DWORD)ppdwAddr, NULL);
    ULONGLONG *ppdwOrdin = (ULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (DWORD)pied->AddressOfNameOrdinals, NULL);
    char* szFun = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (ULONG) * ppdwNames, NULL);

    for (i = 0; i < NumberOfNames; i++) {
        printf("%0.4x\t%0.8x\t%s\n", i + 1, *ppdwAddr, szFun);
        szFun = szFun + strlen(szFun) + 1;
        ppdwAddr++;

        if (i % 200 == 0 && i / 200 >= 1) {
            printf("{Press [ENTER] to continue...}");
            getchar();
        }
    }

imp:
    printf("\n\n输入表：\n");
    printf("\tHint\tThunkRVA\tName\n");
    PIMAGE_IMPORT_DESCRIPTOR piid;
    PIMAGE_THUNK_DATA _pThunk = NULL;
    DWORD dwThunk = NULL;
    USHORT Hint;

    if (pinths64->OptionalHeader.DataDirectory[1].VirtualAddress == 0) return;

    piid = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, pinths64->OptionalHeader.DataDirectory[1].VirtualAddress, NULL);

    for (; piid->Name != NULL;) {
        char *szName = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (ULONG)piid->Name, 0);
        printf("%s\n", szName);

        if (piid->OriginalFirstThunk != 0) {
            dwThunk = piid->OriginalFirstThunk;
            _pThunk = (PIMAGE_THUNK_DATA)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (ULONG)piid->OriginalFirstThunk, NULL);
        } else {
            dwThunk = piid->FirstThunk;
            _pThunk = (PIMAGE_THUNK_DATA)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (ULONG)piid->FirstThunk, NULL);
        }

        for (; _pThunk->u1.AddressOfData != NULL;) {
            char *szFun = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pinths64, pdih, (ULONG)(((PIMAGE_IMPORT_BY_NAME)_pThunk->u1.AddressOfData)->Name), 0);

            if (szFun != NULL)
                memcpy(&Hint, szFun - 2, 2);
            else
                Hint = -1;

            printf("\t%0.4x\t%0.8x\t%s\n", Hint, dwThunk, szFun);
            dwThunk += 8;
            _pThunk++;
        }

        piid++;
        printf("{Press [ENTER] to continue...}");
        getchar();
    }
}

int main()
{
    char filename[MAX_PATH] = {0};
    SetConsoleTitleA("Simple PE64 Viewer");
bgn:
    printf("Simple PE64 Viewer\n====================\nAuthor: Tesla.Angela\nVersion: 0.01\nSupport: PE32+ file\n\n\n");
    printf("输入文件名（支持文件拖拽，直接按回车则默认打开ntoskrnl.exe，输入exit退出）：");
    gets(filename);

    if (FileLen(filename) == 0) {
        if (stricmp(filename, "exit")) {
            CopyFileA("c:\\windows\\system32\\ntoskrnl.exe", "c:\\ntoskrnl.exe", 0);
            strcpy(filename, "c:\\ntoskrnl.exe");
            printf("c:\\ntoskrnl.exe\n");
        } else
            goto end;
    }

    ShowPE64Info(filename);
    clrscr();
    goto bgn;
end:
    DeleteFileA("c:\\ntoskrnl.exe");
    return 0;
}