/*--------------------------------------------------
      TEST.C      
      Calls routines from the Sections DLL.
      Run several instances of Test at once to
      see the totals change.

      written by Brian G. Myers      
  --------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "sections.h"

int main ( )
{
   /* display data from the DLL */
   printf("Totals:\n\r-------\n\r");
   
   printf( "clients: %d\n\r",
            GetNumClients() );
   printf( "threads local: %d\n\r",
            GetNumThreadsLocal() );
   printf( "threads global: %d\n\r",
            GetNumThreadsGlobal() );

   /* wait for the user to press a key */
   getch( );
   return( 0 );
}
