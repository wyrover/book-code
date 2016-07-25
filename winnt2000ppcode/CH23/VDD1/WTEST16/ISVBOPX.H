// ISVBOPX.H - version of ISVBOP.H that works with MS VC++
//
#define RegisterModule    \
   _asm _emit 0xC4 _asm _emit 0xC4 _asm _emit 0x58 _asm _emit 0 
#define UnregisterModule  \
   _asm _emit 0xC4 _asm _emit 0xC4 _asm _emit 0x58 _asm _emit 1 
#define DispatchCall      \
   _asm _emit 0xC4 _asm _emit 0xC4 _asm _emit 0x58 _asm _emit 2 
#define VDDUnsimulate16   \
   _asm _emit 0xC4 _asm _emit 0xC4 _asm _emit 0x58 _asm _emit 3 

// end of file
   
