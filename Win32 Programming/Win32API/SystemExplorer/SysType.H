WORD getSystemType();

// Returns one of the following bit values
#define SYSTYPE_API4 0x01          // feature supported in API4 only
#define SYSTYPE_API3 0x02          // feature supported in API3 only
#define SYSTYPE_W32s 0x04          // feature supported in Win32s

// The following codes can be entered in the table, but are not returned
// by getSystemType
#define SYSTYPE_W32 (SYSTYPE_API3 | SYSTYPE_API4 ) // feature supported in all Win32 variants
#define SYSTYPE_ALL (SYSTYPE_W32 | SYSTYPE_W32s) // feature supported in all Win32
#define SYSTYPE_OBSOLETE4 0x10     // Name is obsolete in API4
