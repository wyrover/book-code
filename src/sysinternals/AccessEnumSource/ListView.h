#pragma once

enum {
//  LISTVIEW_IMG_FILE,          // default icon
    LISTVIEW_IMG_DESCENDING,
    LISTVIEW_IMG_ASCENDING,
};

enum DATATYPE {
    DATATYPE_TEXT,
    DATATYPE_NUMBER,
    DATATYPE_HEX,
    DATATYPE_DATE,
};

enum SORTDIR {
    SORTDIR_UNDEF,
    SORTDIR_UP,
    SORTDIR_DOWN,
};

struct LISTVIEW_COLUMN {
    const TCHAR     *   Title;
    int                 Width;
    DATATYPE            Type;
    SORTDIR             SortDir;
};

struct LISTVIEW_SORT {
    HWND                hList;
    LISTVIEW_COLUMN *   ColumnList;
    int                 Column;
    bool                UsingEx;
};


void InitListViewColumns(HWND hListView, const LISTVIEW_COLUMN * Cols, int numColumns, long style = LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
void SortListView(HWND hListview, int iSubItem, LISTVIEW_COLUMN * ColumnList, bool Reverse = true);
LPARAM ListView_GetParam(HWND hListView, UINT iItem);
void PopupListviewColumnsMenu(HWND hListview, LISTVIEW_COLUMN * ColumnList);
int GetListViewSortColumn(LISTVIEW_COLUMN * ColumnList);
void SaveListViewColumns(HWND hListView);
