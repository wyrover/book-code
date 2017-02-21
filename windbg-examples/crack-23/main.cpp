#include "windows.h"
#include "tchar.h"
#include <stdio.h>


int _tmain(int argc, TCHAR *argv[])
{
	
    int i;
    struct information
    {
        int cpu_num;
        char cpu_name[50];
        unsigned long cpu_speed;
        int cpu_price;
    }Info[3] = {
        { 1, "AMD 1.2G 1987", 1200, 120 },
        { 2, "INTEL 2.5G 1998", 2500, 220 },
        { 3, "AMD 2.2G 2008", 3000, 420 }
    };

    Info[0].cpu_num += 2;
    Info[0].cpu_speed += 3;
    Info[0].cpu_price -= 20;

    Info[1].cpu_num += 2;
    Info[1].cpu_speed += 35;
    Info[1].cpu_price -= 32;

    Info[2].cpu_num += 2;
    Info[2].cpu_speed += 13;
    Info[2].cpu_price -= 55;

    for (i = 0; i < 3; i++)
    {
        printf("%d %s %d %d\n\n", Info[i].cpu_num, Info[i].cpu_name, Info[i].cpu_speed, Info[i].cpu_price);
    }

	return 0;
}