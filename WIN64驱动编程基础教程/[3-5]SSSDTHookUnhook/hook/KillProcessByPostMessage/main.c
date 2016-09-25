#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
    DWORD pid, wpid, i, j;
    HWND hWnd;
st:
    system("cls");
    printf("Input pid: ");
    scanf("%ld", &pid);

    for (i = 100; i < 0xffffff; i += 2) {
        GetWindowThreadProcessId(i, &wpid);

        if (wpid == pid && IsWindowVisible((HWND)i) == 1) {
            hWnd = i;

            for (j = 0; j < 0x10000; j++) {
                PostMessage(hWnd, j, 0, 0);
            }
        }
    }

    printf("OK!");
    getchar();
    getchar();
    goto st;
    return 0;
}
