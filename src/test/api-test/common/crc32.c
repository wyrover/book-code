//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   CRC32.C
//  Creater     :   Freeway Chen
//  Date        :   2000-6-15 18:25:43
//  Comment     :   Get source code from Zlib Project
//
//////////////////////////////////////////////////////////////////////////////////////

/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-1998 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include <windows.h>


typedef unsigned __int32 CRC_UINT32;
typedef unsigned __int8  CRC_BYTE;

#ifdef DYNAMIC_CRC_TABLE

static int CRCTableEmptyFlag = 1;
static CRC_UINT32 CRC_Table[256];
static void MakeCRCTable(void);

/*
  Generate a table for a byte-wise 32-bit CRC calculation on the polynomial:
  x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1.

  Polynomials over GF(2) are represented in binary, one bit per coefficient,
  with the lowest powers in the most significant bit.  Then adding polynomials
  is just exclusive-or, and multiplying a polynomial by x is a right shift by
  one.  If we call the above polynomial p, and represent a byte as the
  polynomial q, also with the lowest power in the most significant bit (so the
  byte 0xb1 is the polynomial x^7+x^3+x+1), then the CRC is (q*x^32) mod p,
  where a mod b means the remainder after dividing a by b.

  This calculation is done using the shift-register method of multiplying and
  taking the remainder.  The register is initialized to zero, and for each
  incoming bit, x^32 is added mod p to the register if the bit is a one (where
  x^32 mod p is p+x^32 = x^26+...+1), and the register is multiplied mod p by
  x (which is shifting right by one and adding x^32 mod p if the bit shifted
  out is a one).  We start with the highest power (least significant bit) of
  q and repeat for all eight bits of q.

  The table is simply the CRC of all possible eight bit values.  This is all
  the information needed to generate CRC's on data a byte at a time for all
  combinations of CRC register values and incoming bytes.
*/
static void MakeCRCTable()
{
    CRC_UINT32 CRC;
    CRC_UINT32 n, k;
    CRC_UINT32 poly;            /* polynomial exclusive-or pattern */
    /* terms of polynomial defining this crc (except x^32): */
    static const CRC_BYTE p[] = {0, 1, 2, 4, 5, 7, 8, 10, 11, 12, 16, 22, 23, 26};
    /* make exclusive-or pattern from polynomial (0xedb88320L) */
    poly = 0L;

    for (n = 0; n < sizeof(p) / sizeof(CRC_BYTE); n++)
        poly |= 1L << (31 - p[n]);

    for (n = 0; n < 256; n++) {
        CRC = n;

        for (k = 0; k < 8; k++) {
            if (CRC & 1)
                CRC = poly ^ (CRC >> 1);
            else
                CRC = CRC >> 1;
        }

        CRC_Table[n] = CRC;
    }

    CRCTableEmptyFlag = 0;
}
#else

// ========================================================================
// Table of CRC-32's of all single-byte values (made by make_crc_table)
//

static const CRC_UINT32 CRC_Table[256] = {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL
};
#endif  // DYNAMIC_CRC_TABLE

// =========================================================================
// This function can be used by asm versions of crc32()
//
const CRC_UINT32 * GetCRCTable()
{
#ifdef DYNAMIC_CRC_TABLE

    if (CRCTableEmptyFlag)
        MakeCRCTable();

#endif
    return (const CRC_UINT32 *)CRC_Table;
}



// Use internal CRC32 function
// =========================================================================
#define DO1BYTE(pBuf) CRC = CRC_Table[(CRC ^ (*pBuf++)) & 0xff] ^ (CRC >> 8);
//#define DO2(pBuf)  DO1(pBuf); DO1(pBuf);
//#define DO4(pBuf)  DO2(pBuf); DO2(pBuf);
//#define DO8(pBuf)  DO4(pBuf); DO4(pBuf);
// =========================================================================

unsigned CRC32_C(unsigned CRC, const void *pvBuf, int nLen)
//unsigned CRC32(unsigned CRC, const void *pvBuf, int nLen)
{
    const CRC_BYTE *pBuf = (CRC_BYTE*)pvBuf;

    if (pvBuf == (void *)0)
        return 0L;

#ifdef DYNAMIC_CRC_TABLE

    if (CRCTableEmptyFlag)
        MakeCRCTable();

#endif  // DYNAMIC_CRC_TABLE
    CRC = CRC ^ 0xffffffffL;    // equal CRC = ~CRC

    while (nLen >= 8) {
        // It's easy to debug
        DO1BYTE(pBuf);          // 0
        DO1BYTE(pBuf);          // 1
        DO1BYTE(pBuf);          // 2
        DO1BYTE(pBuf);          // 3
        DO1BYTE(pBuf);          // 4
        DO1BYTE(pBuf);          // 5
        DO1BYTE(pBuf);          // 6
        DO1BYTE(pBuf);          // 7
        nLen -= 8;
    }

    if (nLen) {
        do {
            DO1BYTE(pBuf);
        } while (--nLen);
    }

    return (CRC ^ 0xffffffffL);   // equal return ~CRC
}

// ============================================================================
//
// ASM Part in Visual C++
//
// ============================================================================

//#define DO1BYTE(pBuf) CRC = CRC_Table[(CRC ^ (*pBuf++)) & 0xff] ^ (CRC >> 8);
#define DO1BYTEASM() \
    __asm{ mov bl, [esi] } \
    __asm{ inc esi } \
    __asm{ xor ebx, eax } \
    __asm{ shr eax, 8 } \
    __asm{ and ebx, 0xff } \
    __asm{ xor eax, CRC_Table[ebx * 4] }


#pragma warning(push)
#pragma warning(disable: 4731)
unsigned int CRC32(unsigned int CRC, void const *pvBuf, unsigned int nLen)
{
    unsigned RetCode = 0;

    if (!pvBuf)
        return RetCode;

    __asm mov esi, [pvBuf]
    __asm mov ecx, [nLen]
    __asm mov eax, [CRC]
    __asm shr ecx, 3
    __asm xor eax, 0xffffffff
    __asm test ecx, ecx
    __asm jz CRC_Last7Bytes
    __asm push ebp
    __asm mov ebp, ecx
    __asm xor edx, edx
    __asm xor ecx, ecx
    __asm align 4
CRC_Loop1:
    // eax = CRC
    // edx first TableIndex
    // ecx second TableIndex
    // Process 4 bytes
    __asm mov ebx, [esi]
    __asm add esi, 4
    __asm mov dl, bl
    __asm mov cl, bh
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]
    __asm shr ebx, 16
    __asm xor eax, edi
    __asm xor cl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[ecx * 4]
    __asm mov dl, bl
    __asm xor eax, edi
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]
    __asm mov cl, bh
    __asm xor eax, edi
    __asm mov ebx, [esi]
    __asm xor cl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[ecx * 4]
    __asm add esi, 4
    __asm xor eax, edi
    __asm mov dl, bl
    __asm mov cl, bh
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]
    __asm shr ebx, 16
    __asm xor eax, edi
    __asm xor cl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[ecx * 4]
    __asm mov dl, bl
    __asm xor eax, edi
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]
    __asm mov cl, bh
    __asm xor eax, edi
    __asm xor cl, al
    __asm shr eax, 8
    __asm xor eax, CRC_Table[ecx * 4]
    __asm dec ebp
    __asm jnz CRC_Loop1
    __asm pop ebp
CRC_Last7Bytes:
    __asm mov ecx, [nLen]
    __asm xor edx, edx
    __asm and ecx, 0x7
    __asm jz CRC_Exit
CRC_Loop2:
    __asm mov dl, [esi]
    __asm inc esi
    __asm xor dl, al
    __asm shr eax, 8
    __asm xor eax, CRC_Table[edx * 4]
    __asm dec ecx
    __asm jnz CRC_Loop2
CRC_Exit:
    __asm xor eax, 0xffffffff
    __asm mov [RetCode], eax
    return RetCode;
}
#pragma warning(pop)

unsigned int CRC32_file(unsigned int CRC, LPCTSTR lpFileName)
{
    const unsigned long c_ulLimit = 2 * 1024 * 1024;
    HANDLE  hFile           = NULL;
    DWORD   dwFileSizeHigh  = 0;
    DWORD   dwFileSizeLow   = 0;
    PVOID   lpBuffer        = NULL;

    __try {
        hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
            return 0;

        dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);

        if (dwFileSizeLow == INVALID_FILE_SIZE)
            return 0;

        if (0 != SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
            return 0;

        if (dwFileSizeHigh > 0 || dwFileSizeLow > c_ulLimit)
            dwFileSizeLow = c_ulLimit;

        if ((lpBuffer = malloc(dwFileSizeLow)) == NULL)
            return 0;

        if (!ReadFile(hFile, lpBuffer, dwFileSizeLow, &dwFileSizeHigh, NULL) ||
            dwFileSizeHigh != dwFileSizeLow)
            return 0;

        if ((dwFileSizeHigh = CRC32(CRC, lpBuffer, dwFileSizeLow)) == 0)
            return 0;
    } __finally {
        if (lpBuffer != NULL) {
            free(lpBuffer);
            lpBuffer = NULL;
        }

        if (hFile) {
            CloseHandle(hFile);
            hFile = NULL;
        }
    }

    return dwFileSizeHigh;
}


/*
unsigned CRC32(unsigned CRC, const void *pvBuf, int nLen)
{
    unsigned RetCode = 0;

    if (!pvBuf)
        return RetCode;

    __asm mov esi, [pvBuf]
    __asm mov ecx, [nLen]
    __asm mov eax, [CRC]
    __asm shr ecx, 3
    __asm xor eax, 0xffffffff

    __asm test ecx, ecx
    __asm jz CRC_Last7Bytes

CRC_Loop1:
    __asm xor edx, edx

    // Process 4 bytes
    __asm mov ebx, [esi]
    __asm add esi, 4

    __asm mov dl, bl
    __asm shr ebx, 8

    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm shr ebx, 8
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm shr ebx, 8
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm xor dl, al
    __asm shr eax, 8
    __asm xor eax, CRC_Table[edx * 4]


    // Process 4 bytes
    __asm mov ebx, [esi]
    __asm add esi, 4

    __asm mov dl, bl
    __asm shr ebx, 8

    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm shr ebx, 8
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm shr ebx, 8
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm xor dl, al
    __asm shr eax, 8
    __asm xor eax, CRC_Table[edx * 4]

    __asm dec ecx
    __asm jnz CRC_Loop1

CRC_Last7Bytes:

    __asm mov ecx, [nLen]
    __asm xor edx, edx

    __asm test ecx, 0x4
    __asm jz CRC_Last3Bytes


    // Process 4 bytes
    __asm mov ebx, [esi]
    __asm add esi, 4

    __asm mov dl, bl
    __asm shr ebx, 8

    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm shr ebx, 8
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm shr ebx, 8
    __asm xor dl, al
    __asm shr eax, 8
    __asm mov edi, CRC_Table[edx * 4]

    __asm mov dl, bl
    __asm xor eax, edi

    __asm xor dl, al
    __asm shr eax, 8
    __asm xor eax, CRC_Table[edx * 4]

CRC_Last3Bytes:

    __asm xor edx, edx
    __asm and ecx, 0x3
    __asm jz CRC_Exit

CRC_Loop2:

    __asm mov dl, [esi]
    __asm inc esi
    __asm xor dl, al
    __asm shr eax, 8
    __asm xor eax, CRC_Table[edx * 4]

    __asm dec ecx
    __asm jnz CRC_Loop2

CRC_Exit:

    __asm xor eax, 0xffffffff
    __asm mov [RetCode], eax

    return RetCode;
}
*/