/*--------------------------------------------------
      GLOBAL.C
      Declares and defines global variables for
      SECTIONS.DLL. Variables declared here will
      be shared by all the DLL's clients--all clients
      will use the same instance of these variables.

      To make the variables shared, the DEF file must
      assign memory attributes for this data segment.

      To assign attributes, the DEF file must be
      given a name for the data segment.

      In order for this segment to have a different
      name than the default _TEXT segment, it must
      be placed in a different file that begins with
      the -z compiler options for setting segment,
      class, and group names.

      written by Brian G. Myers      
  --------------------------------------------------*/
#pragma option -zR.GLOBALDATA
#pragma option -zS.GLOBALGROUP
#pragma option -zT.GLOBALCLASS

/*  Total number of clients attached to the DLL */
unsigned int uNumClients = 0;

/* Total number of threads in all clients */
/* attached to the DLL                    */
unsigned int uNumThreadsGlobal = 0;

