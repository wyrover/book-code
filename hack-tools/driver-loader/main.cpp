#include <SDKDDKVer.h>
#ifndef _WIN32_WINNT               
    #define _WIN32_WINNT 0x0501     
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define Cleanup(x, y, z) {x = y; goto z;}
#define FLUSH fflush(stdin);
#define DRIVER_LOADED       0x00000001
#define DRIVER_STARTED      0x00000002
#define DRIVER_STOPPED      0x00000003
#define DRIVER_UNLOADED     0x00000004
#define DRIVER_CANT_LOAD    0x00000010
#define DRIVER_CANT_START   0x00000020
#define DRIVER_CANT_STOP    0x00000030
#define DRIVER_CANT_UNLOAD  0x00000040

typedef struct {
    DWORD driverstatus;
} DRIVER_LOADER, *PDRIVER_LOADER;

int FileExists(const TCHAR *driverpath)
{
    
    FILE *fExists = _wfopen(driverpath, L"r");

    if (!fExists)
        return 0;

    fclose(fExists);
    return 1;
}

int myLoadDriver(const TCHAR *drivername, const TCHAR *driverpath)
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    int ret = 1;

    if (!FileExists(driverpath)) {
        printf("FileExists failed\n");
        Cleanup(ret, -1, c);
    }

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!hSCManager) {
        printf("OpenSCManager failed\n");
        Cleanup(ret, -2, c);
    }

    hService = CreateService(
                   hSCManager,
                   drivername,
                   drivername,
                   SERVICE_ALL_ACCESS,
                   SERVICE_KERNEL_DRIVER,
                   SERVICE_DEMAND_START,//SERVICE_DEMAND_START,
                   SERVICE_ERROR_NORMAL,
                   driverpath,
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   NULL);

    if (!hService) {
        printf("CreateService failed\n");
        Cleanup(ret, -3, c);
    }

c:

    if (hService) CloseServiceHandle(hService);

    if (hSCManager) CloseServiceHandle(hSCManager);

    return ret;
}

int myStartDriver(wchar_t *drivername)
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    int ret = 1;
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!hSCManager)
        Cleanup(ret, -1, c);

    hService = OpenService(hSCManager, drivername, SERVICE_ALL_ACCESS);

    if (!hService)
        Cleanup(ret, -2, c);

    if (!StartService(hService, 0, NULL))
        Cleanup(ret, -3, c);

c:

    if (hService) CloseServiceHandle(hService);

    if (hSCManager) CloseServiceHandle(hSCManager);

    return ret;
}

int myStopDriver(wchar_t* drivername)
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    SERVICE_STATUS ss;
    int ret = 1;
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!hSCManager) {
        Cleanup(ret, -1, c);
    }

    hService = OpenService(hSCManager, drivername, SERVICE_ALL_ACCESS);

    if (!hService) {
        Cleanup(ret, -2, c);
    }

    if (!ControlService(hService, SERVICE_CONTROL_STOP, &ss)) {
        Cleanup(ret, -3, c);
    }

c:

    if (hService) CloseServiceHandle(hService);

    if (hSCManager) CloseServiceHandle(hSCManager);

    return ret;
}

int myUnloadDriver(const wchar_t* drivername)
{
    SC_HANDLE hSCManager;
    SC_HANDLE hService;
    SERVICE_STATUS ss;
    int ret = 1;
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!hSCManager)
        Cleanup(ret, -1, c);

    hService = OpenService(hSCManager, drivername, SERVICE_ALL_ACCESS);

    if (!hService)
        Cleanup(ret, -2, c);

    // try to stop service first
    ControlService(hService, SERVICE_CONTROL_STOP, &ss);

    if (!DeleteService(hService))
        Cleanup(ret, -4, c);

c:

    if (hService) CloseServiceHandle(hService);

    if (hSCManager) CloseServiceHandle(hSCManager);

    return ret;
}

void funcLoadDriver(DRIVER_LOADER *dl)
{
    wchar_t drivername[256 + 1];
    wchar_t driverpath[256 + 1];
    wchar_t selection;
    int err;

    for (; ;) {
        printf(" Enter driver's name\n - ");
        FLUSH;
        scanf("%256[^\n]", drivername);
        printf(" Enter driver's full path\n - ");
        FLUSH;
        scanf("%256[^\n]", driverpath);
        printf(" Confirm (Y - yes | N - no | B - back): ");
        FLUSH;
        scanf("%c", &selection);

        switch (selection) {
        case 'y':
        case 'Y':
            printf(" Performing  : myLoadDriver\n");
            err = myLoadDriver(drivername, driverpath);
            dl->driverstatus = DRIVER_LOADED;

            if (err != 1) {
                dl->driverstatus = DRIVER_CANT_LOAD;
                printf(" Error       : myLoadDriver (%d)\n", err);
                printf(" GetLastError: (%d)\n", GetLastError());
                return;
            }

            printf(" Success     : myLoadDriver\n");
            return;

        case 'n':
        case 'N':
            break;

        case 'b':
        case 'B':
            return;

        default:
            printf(" Wrong option selected, default to N\n");
            break;
        }

        printf("\n");
    }
}

void funcStartDriver(DRIVER_LOADER *dl)
{
    wchar_t drivername[256 + 1];
    wchar_t selection;
    int err;

    for (; ;) {
        printf(" Enter driver's name\n -  ");
        FLUSH;
        scanf("%256[^\n]", drivername);
        printf(" Confirm (Y - yes | N - no | B - back): ");
        FLUSH;
        scanf("%c", &selection);

        switch (selection) {
        case 'y':
        case 'Y':
            printf(" Performing  : myStartDriver\n");
            err = myStartDriver(drivername);
            dl->driverstatus = DRIVER_STARTED;

            if (err != 1) {
                dl->driverstatus = DRIVER_CANT_START;
                printf(" Error       : myStartDriver (%d)\n", err);
                printf(" GetLastError: (%d)\n", GetLastError());
                return;
            }

            printf(" Success     : myStartDriver\n");
            return;

        case 'n':
        case 'N':
            break;

        case 'b':
        case 'B':
            return;

        default:
            printf(" Wrong option selected, default to N\n");
            break;
        }

        printf("\n");
    }
}

void funcStopDriver(DRIVER_LOADER *dl)
{
    wchar_t drivername[256 + 1];
    wchar_t selection;
    int err;

    for (; ;) {
        printf(" Enter driver's name\n -  ");
        FLUSH;
        scanf("%256[^\n]", drivername);
        printf(" Confirm (Y - yes | N - no | B - back): ");
        FLUSH;
        scanf("%c", &selection);

        switch (selection) {
        case 'y':
        case 'Y':
            printf(" Performing  : myStopDriver\n");
            err = myStopDriver(drivername);
            dl->driverstatus = DRIVER_STOPPED;

            if (err != 1) {
                dl->driverstatus = DRIVER_CANT_STOP;
                printf(" Error       : myStopDriver (%d)\n", err);
                printf(" GetLastError: (%d)\n", GetLastError());
                return;
            }

            printf(" Success     : myStopDriver\n");
            return;

        case 'n':
        case 'N':
            break;

        case 'b':
        case 'B':
            return;

        default:
            printf(" Wrong option selected, default to N\n");
            break;
        }

        printf("\n");
    }
}

void funcUnloadDriver(DRIVER_LOADER *dl)
{
    wchar_t drivername[256 + 1];
    wchar_t selection;
    int err;

    for (; ;) {
        printf(" Enter driver's name\n -  ");
        FLUSH;
        scanf("%256[^\n]", drivername);
        printf(" Confirm (Y - yes | N - no | B - back): ");
        FLUSH;
        scanf("%c", &selection);

        switch (selection) {
        case 'y':
        case 'Y':
            printf(" Performing  : myUnloadDriver\n");
            err = myUnloadDriver(drivername);
            dl->driverstatus = DRIVER_UNLOADED;

            if (err != 1) {
                dl->driverstatus = DRIVER_CANT_UNLOAD;
                printf(" Error       : myUnloadDriver (%d)\n", err);
                printf(" GetLastError: (%d)\n", GetLastError());
                return;
            }

            printf(" Success     : myUnloadDriver\n");
            return;

        case 'n':
        case 'N':
            break;

        case 'b':
        case 'B':
            return;

        default:
            printf(" Wrong option selected, default to N\n");
            break;
        }

        printf("\n");
    }
}

int main(int argc, wchar_t **argv)
{
    DRIVER_LOADER dl;
    int selection;

    for (; ;) {
        printf(" 1 - Load a driver\n"
               " 2 - Start service\n"
               " 3 - Stop service\n"
               " 4 - Unload a driver\n"
               " 0 - Exit\n"
               "\n"
               " Select an option: ");
        FLUSH;
        scanf("%d", &selection);

        switch (selection) {
        case 1:
            funcLoadDriver(&dl);
            break;

        case 2:
            funcStartDriver(&dl);
            break;

        case 3:
            funcStopDriver(&dl);
            break;

        case 4:
            funcUnloadDriver(&dl);
            break;

        case 0:
            printf(" Thanks for using...XXX \n");
            FLUSH;
            getchar();
            return 0;

        default:
            break;
        }

        printf("\n");
    }

    FLUSH;
    getchar();
    return 0;
}