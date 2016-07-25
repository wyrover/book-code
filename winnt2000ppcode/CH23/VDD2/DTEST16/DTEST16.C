// DTEST16.C - Sample DOS App that calls DOSDRV16.SYS

#include <stdio.h>         /* prototype for printf */
#include <io.h>            /* prototype for file routines */
#include <dos.h>           /* prototype for int86, REGS */
#include <fcntl.h>         /* O_BINARY, O_RDWR, O_RDONLY */

/**------------------------------------------------------**/
void main(void)
{
   int hDriver=0;
   union _REGS inregs, outregs;
   struct _SREGS sregs;
   char szData[128];
   unsigned char ucData=0;

   // open the dos device driver by name and set to raw mode
   if ((hDriver = _open("DRV16", O_BINARY | O_RDWR)) == -1) {
      printf("Couldn't open device driver\n");
      return;
   } else printf("Open succeeded.\n");

   inregs.x.ax = 0x4400;
   inregs.x.bx = hDriver;
   _intdos(&inregs, &outregs);       // int21 DOS ioctl

   if ((outregs.x.dx & 0x80) > 0) {  // if character device
      inregs.h.al = 1;
      inregs.h.dh = 0;
      inregs.h.dl = outregs.h.dl | 0x20;
      _intdos(&inregs, &outregs);    // set to raw mode
   }

   // could read and write to the device using 
   // _write(hDriver,..) and _read(hDriver,..)

   // do a device I/O Control Input from the device
   inregs.x.ax = 0x4402;         
   inregs.x.bx = hDriver;                   
   inregs.x.cx = 1;           // just one byte
   _segread(&sregs);                               
   inregs.x.dx = (unsigned int)(&ucData);
   _intdosx(&inregs,&outregs,&sregs);           

   printf("Device IOCTL data = %d\n", ucData);
   _close(hDriver);
} // main 

// end of file
