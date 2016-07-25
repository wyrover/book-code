// REGVDD.H - Include file for sample Windows NT VDD
//
typedef struct {
   DWORD dwCmd;
   DWORD dwRootKey;
   DWORD dwVolatility;
   DWORD dwAccess;
   char szSubkey[128];
   DWORD dwValueType;
   char szValueName[128];
   BYTE szValueData[128]; 
   DWORD dwValueDataSize;
   DWORD dwStatus;
} REGVDD_INFO;

// REGVDD Dispatch Commands
#define REGVDD_CMD_CREATESUBKEY     1
#define REGVDD_CMD_DELETESUBKEY     2
#define REGVDD_CMD_WRITEVALUE       3
#define REGVDD_CMD_READVALUE        4
#define REGVDD_CMD_DELETEVALUE      5

// end of file
