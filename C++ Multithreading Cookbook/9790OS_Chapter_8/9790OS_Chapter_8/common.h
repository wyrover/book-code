#include <windows.h>
#include <tchar.h>

#define MAPPING_NAME  _T("__comm_61524_map__")
#define EVENT_NAME    _T("__evnt_68435_rst__")

typedef struct {
    HMODULE hLibrary;
} COMM_OBJECT, *PCOMM_OBJECT;
