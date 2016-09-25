#include <stdio.h>
#include <Windows.h>

#pragma comment(lib,"Advapi32.lib")

char *cs(char *str1, char *str2) //connect string
{
    SIZE_T newstrlen = strlen(str1) + strlen(str2) + 1;
    char *newstr = (char*)malloc(newstrlen);
    memcpy(newstr, str1, strlen(str1));
    memcpy(newstr + strlen(str1), str2, strlen(str2) + 1);
    return newstr;
}

BOOLEAN LoadWdmDrv(char *inf, char *szDrvSvcName)
{
    CHAR exe[] = "c:\\windows\\system32\\InfDefaultInstall.exe ";
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    char *cmd = cs(exe, inf);

    if (!CreateProcessA(NULL, cmd, NULL, NULL, 0, 0, NULL, NULL, &si, &pi))
        return FALSE;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    SC_HANDLE hSvcHandle = OpenServiceA(hSCM, szDrvSvcName, SERVICE_ALL_ACCESS);

    if (hSvcHandle) {
        CloseServiceHandle(hSCM);
        CloseServiceHandle(hSvcHandle);
        return TRUE;
    } else {
        CloseServiceHandle(hSCM);
        return FALSE;
    }
}

BOOLEAN net_start(char *szDrvSvcName)
{
    BOOLEAN b = FALSE;
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    SC_HANDLE hSvcHandle = OpenServiceA(hSCM, szDrvSvcName, SERVICE_ALL_ACCESS);

    if (hSvcHandle) {
        b = StartServiceA(hSvcHandle, NULL, NULL);
        CloseServiceHandle(hSvcHandle);
    }

    CloseServiceHandle(hSCM);
    return b;
}

BOOLEAN net_stop(char *szDrvSvcName)
{
    BOOLEAN b = FALSE;
    SERVICE_STATUS lpSt;
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    SC_HANDLE hSvcHandle = OpenServiceA(hSCM, szDrvSvcName, SERVICE_ALL_ACCESS);

    if (hSvcHandle) {
        b = ControlService(hSvcHandle, SERVICE_CONTROL_STOP, &lpSt);
        CloseServiceHandle(hSvcHandle);
    }

    CloseServiceHandle(hSCM);
    return b;
}

BOOLEAN UnloadWdmDrv(char *szDrvSvcName)
{
    BOOLEAN b = FALSE;
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    SC_HANDLE hSvcHandle = OpenServiceA(hSCM, szDrvSvcName, SERVICE_ALL_ACCESS);

    if (hSvcHandle) {
        b = DeleteService(hSvcHandle);
        CloseServiceHandle(hSvcHandle);
    }

    CloseServiceHandle(hSCM);
    return b;
}

int main()
{
    BOOLEAN b;
    //copy file
    CopyFileA("setup.inf", "c:\\setup.inf", FALSE);
    CopyFileA("mftest.sys", "c:\\mftest.sys", FALSE);
    //load WDM DRIVER
    b = LoadWdmDrv("c:\\setup.inf", "mftest");
    printf("LoadWdmDrv: %d\n", b);
    //start driver
    b = net_start("mftest");
    printf("net_start: %d\n", b);
    printf("press ENTER to unload driver...");
    getchar();
    //stop driver
    net_stop("mftest");
    //unload driver
    UnloadWdmDrv("mftest");
    //exit
    printf("press ENTER to exit...");
    getchar();
    //delete file
    DeleteFileA("c:\\setup.inf");
    DeleteFileA("c:\\mftest.sys");
}