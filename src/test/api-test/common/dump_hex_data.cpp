#include "dump_hex_data.h"
#include <cstdio>

void dumpHexData(char *tmp, int len)
{
    printf("char pkt[] = {\n");
    int i;

    for (i = 0; i < len; i++) {
        printf("0x%02x, ", tmp[i] & 0xFF);

        if (i % 16 == 15) {
            printf("\n");
        }
    }

    if (i % 16 != 0) {
        printf("\n");
    }

    printf("};\n");
}

void printHexData(char *tmp, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        printf("%02x ", tmp[i] & 0xFF);

        if (i % 16 == 15) {
            printf("\n");
        }
    }

    if (i % 16 != 0) {
        printf("\n");
    }
}