
#include <windows.h>
#include <string.h>

/*
 * Resource defines
 */
#define WINDOWMENU  1

#ifdef RC_INVOKED
#define ID(id) id
#else
#define ID(id) MAKEINTRESOURCE(id)
#endif

/*
 * Menu ID's
 */
#define MM_ABOUT        8000
#define MM_SERVER       8001
#define MM_CLIENT       8002
#define MM_RESERVE      8003

#define MM_OPT_1        7001
#define MM_OPT_2        7002
#define MM_OPT_3        7003
#define MM_OPT_4        7004
#define MM_OPT_5        7005
#define MM_OPT_6        7006
#define MM_OPT_7        7007
#define MM_OPT_8        7008

#define IDM_CASCADE     30
#define IDM_TILE        31
#define IDM_ARRANGE     32
#define IDM_CLOSEALL    33

#define SIZEOFCAPTIONTEXT       20

/*
 * Dialog ID's
 */
#define IDBTN_PAGE              101
#define IDBTN_MAP               102
#define IDBTN_OK                103

#define IDEDIT_MAPFILE          104
#define IDEDIT_MAPNAME          105


/*
 * String resource ID's
 */

#define  IDS_ERR_INITAPP_FAILED         300
#define  IDS_ERR_CREATE_CHILD_FAILED    301
#define  IDS_ERR_CREATE_DLG_FAILED      302
#define  IDS_ERR_MAPFILE_FAILED         303
#define  IDS_ERR_MAPPING_FAILED         304
#define  IDS_ERR_MAPVIEW_FAILED         305
#define  IDS_ERR_ACCESS_WRITE_FAILED    306
#define  IDS_ERR_ACCESS_READ_FAILED     307
#define  IDS_ERR_OPEN_MAPPING_FAILED    308
#define  IDS_SERVER                     309
#define  IDS_CLIENT                     310
#define  IDS_CREATE_MAP_FILE            311
#define  IDS_CREATE_MAPPING             312
#define  IDS_MAPPING_VIEW               313
#define  IDS_ACCESSING_SERVER_WRITE     314
#define  IDS_OPENING_FILE               315
#define  IDS_ACCESSING_SERVER_READ      316
#define  IDS_SEL_CREATE_FILE_MAPPING    317
#define  IDS_SEL_MAP_VIEW               318
#define  IDS_SEL_ACCESS                 319
#define  IDS_SEL_CREATE_FILE            320
#define  IDS_SEL_ACCESS_READ            321
#define  IDS_SEL_OPEN_FILE              322
#define  IDS_ERROR                      323


extern HWND ghwndClient;
extern HWND ghwndMain;


LPTSTR GetStringRes(int id);
void ErrorOut(LPSTR errstring);
HANDLE CreateMapFile (char *MapFileName);
HANDLE CreateMap (HANDLE FileToBeMapped, LPSTR MapName);
LPVOID MapView (HANDLE hMap);
HANDLE OpenMap(LPSTR MapName);
