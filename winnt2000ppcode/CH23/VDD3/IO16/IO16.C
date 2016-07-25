#include "stdio.h"
#include "memory.h"

#define PORT_IDENTIFY   0x300
#define PORT_READ       0x301
#define PORT_WRITE      0x302
#define PORT_COUNT      0x303
#define PORT_STATUS     0x304
#define MIO_SEG         0xCC00
#define MIO_RANGE       128

void main(void)
{
	unsigned char byteID, byteCount, i;
   char dataBuffer[64];
   char far *pMIOAddress;

   // initialize output data buffer
   pMIOAddress = (char far *)(((unsigned long)MIO_SEG) << 16);
   for (i=0; i < 64; i++) dataBuffer[i]=i;

   // Check that the card is responding
	_asm  mov  dx, PORT_IDENTIFY
	_asm  in   al, dx
	_asm  mov  byteID, al  

   printf("Port I/O: ID %d read from port 0x%x\n",
      byteID, PORT_IDENTIFY); 

   //--------------------------------------------
   // Perform a "write" operation
   //--------------------------------------------

   // copy to memory-mapped I/O area
   _fmemcpy(pMIOAddress, dataBuffer, 64);

   // set the data byte count
   _asm  mov  dx, PORT_COUNT
   _asm  mov  al, 64
   _asm  out  dx, al

   // send the write command signifying data ready
   _asm  mov  dx, PORT_WRITE
   _asm  mov  al, 1
   _asm  out  dx, al

   // wait until transfer complete
   _asm  mov  dx, PORT_STATUS
   _asm  WriteLoop:
   _asm  in   al, dx
   _asm  cmp  al, 1
   _asm  jne  WriteLoop

   // check how many bytes actually written
   _asm  mov  dx, PORT_COUNT
   _asm  in   al, dx
   _asm  mov  byteCount, al

   printf("%d bytes written.\n", byteCount);


   //--------------------------------------------
   // Perform a "read" operation
   //--------------------------------------------

   // set the data byte count
   _asm  mov  dx, PORT_COUNT 
   _asm  mov  al, 64
   _asm  out  dx, al

   // send the read command
   _asm  mov  dx, PORT_READ
   _asm  mov  al, 1
   _asm  out  dx, al

   // wait until transfer complete
   _asm  mov  dx, PORT_STATUS
   _asm  ReadLoop:
   _asm  in   al, dx
   _asm  cmp  al, 1
   _asm  jne  ReadLoop

   // check how many bytes actually available
   _asm  mov  dx, PORT_COUNT
   _asm  in   al, dx
   _asm  mov  byteCount, al

   // copy memory-mapped I/O area to local buffer
   _fmemcpy(dataBuffer, pMIOAddress, byteCount);

   printf("%d bytes read.\n", byteCount);
   for (i=0; i < byteCount; i++) printf("%c", dataBuffer[i]);
   printf("\n");

} // main
  
