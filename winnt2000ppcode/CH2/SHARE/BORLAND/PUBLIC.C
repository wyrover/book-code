/*--------------------------------------------------
      PUBLIC.C
      Declares and defines a global variable for
      SHARE.DLL. The variable declared here will
      be shared by all the DLL's clients--all clients
      will use the same instance of dwOffset.

      To make the variable shared, the DEF file must
      assign memory attributes for this data segment.

      To assign attributes, the DEF file must be
      given a name for the data segment.

      To name this segment, giving it an identity
      separate from the default _TEXT, it must be
      placed in a different file that begins with
      the -z compiler options for setting segment,
      class, and group names.

      written by Brian G. Myers      
  --------------------------------------------------*/
#pragma option -zR.PUBLICDATA
#pragma option -zS.PUBLICGROUP
#pragma option -zT.PUBLICCLASS

/* #include must come after -z options */
#include <wtypes.h>         /* for DWORD */

/* position in buffer */
DWORD dwOffset = 0;           

