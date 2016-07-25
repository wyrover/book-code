/******** CONS32.C - source file for cons32.exe *********/

#include <windows.h>
#include <stdio.h>

/* DOS Device Driver Command Codes */
#define CMD_READ               4
#define CMD_WRITE              8
#define CMD_WRITE_VFY          9
#define CMD_OUT_IOCTL          12

/* DOS Device Driver SubCommand Codes */
#define CMD_IOCTL_READBUFFER   0x09
#define CMD_IOCTL_WRITEBUFFER  0x0A
#define CMD_IOCTL_SCSIINQ      0x0D

/* 32-bit API in HPSCAN32.DLL (VDD) */
ULONG APIENTRY VDDScannerCommand(USHORT, PCHAR, ULONG);

VOID main(VOID)
{
   char Buffer[64+1], i;

   printf("NT Console Test program for HP Scanners.\r\n");
   printf("----------------------------------------\r\n");

   VDDScannerCommand(CMD_IOCTL_SCSIINQ, Buffer, 64);
   printf("Device Type:        %c\r\n", Buffer[0]+'0');

   VDDScannerCommand(CMD_WRITE, "\x01B*s10E", 64);
   VDDScannerCommand(CMD_READ, Buffer, (ULONG)64);
   printf("HP Scanner Model #: ");
   for (i=8; i <= 12; i++) printf("%c", Buffer[i]);
   printf("\r\n");

} /* main */
