/******** TEST16.C - source file for cons32.exe ***********/

#include <stdio.h>         /* prototype for printf */
#include <io.h>            /* prototype for file routines */
#include <dos.h>           /* prototype for int86, REGS */
#include <fcntl.h>         /* O_BINARY, O_RDWR, O_RDONLY */

typedef struct {
   unsigned int command, status, data, offset, segment;
} IOCTLDATA;
IOCTLDATA ioctl_block;

#define SCANNER_DRV_NAME   "HPSCAN"
#define LoWord(l)  ((unsigned short)(unsigned long)(l))
#define HiWord(l)  ((unsigned short)((((unsigned long)(l)) \
   >> 16) & 0xFFFF))

int OpenScanner(void);
unsigned int IOCTLInquiry(int, char far *, int *);

/**------------------------------------------------------**/
void main(void)
{
   int hScanner, status, len=64, i;
   char Buffer[64+1];

   printf("16-bit DOS Test program for HP Scanners.\r\n");
   printf("----------------------------------------\r\n");
   if ((hScanner = OpenScanner()) <= 0) return;
   status = IOCTLInquiry(hScanner, (char far *)Buffer, &len);
   printf("Device Type:        %c\r\n", Buffer[0]+'0');
   _write(hScanner, "\x01B*s10E", 6);
   _read(hScanner, Buffer, 64);
   printf("HP Scanner Model #: ");
   for (i=8; i <= 12; i++) printf("%c", Buffer[i]);
   printf("\r\n");
   _close(hScanner);
} /* main */

/**-------------------- OpenScanner ---------------------**/
int OpenScanner(void)
{
   int handle=0;
   union _REGS inregs, outregs;
	
   if ((handle = _open(SCANNER_DRV_NAME, O_BINARY | O_RDWR))
      == -1) return handle;       /* couldn't open device */

   inregs.x.ax = 0x4400;
   inregs.x.bx = handle;
   _intdos(&inregs, &outregs);         /* int21 DOS ioctl */

   if ((outregs.x.dx & 0x80) > 0)  /* if character device */
   {
      inregs.h.al = 1;
      inregs.h.dh = 0;
      inregs.h.dl = outregs.h.dl | 0x20;
      _intdos(&inregs, &outregs);      /* set to raw mode */
   }
   return handle;
}	/* OpenScanner */

/**-------------------- IOCTLInquiry --------------------**/
unsigned int IOCTLInquiry(int handle, char far *buf, int *len)
{
   union _REGS regs;
   struct _SREGS sregs;
   int status = 0;

   regs.x.ax = 0x4403;       /* DOS output IOCTL function */
   regs.x.bx = handle;                  /* Scanner handle */
   regs.x.cx = 10;                       /* size of ioctl */
   regs.x.dx = (unsigned int)(&ioctl_block);

   _segread(&sregs);             /* set ds to String seg */
   regs.x.dx = (unsigned int)(&ioctl_block); /* near data */
   ioctl_block.command = 0x0d;            /* SCSI inquiry */
   ioctl_block.status = 0;
   ioctl_block.data = (int)*len;
   ioctl_block.segment = HiWord(buf);         /* far data */
   ioctl_block.offset = LoWord(buf);          /* far data */
   _intdosx(&regs,&regs,&sregs);            /* call int21 */

   if (regs.x.cflag) status = regs.x.ax;
   else
   {                           /* device not ready = 0xf5 */
      if (regs.h.al == 0x00) status = 0xf5;
      else status = ioctl_block.status;
   }
   *len = (long)ioctl_block.data;
   return status;
}	/* IOCTLInquiry */
