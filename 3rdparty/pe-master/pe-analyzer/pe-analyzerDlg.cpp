/*
 * Copyright 2010 JiJie Shi
 *
 * This file is part of PEMaster.
 *
 * PEMaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PEMaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PEMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// pe-analyzerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "pe-analyzerDlg.h"
#include "commctrl.h"

#pragma comment( lib, "pe-master.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FLAG_SHOW_DATA 1
#define FLAG_SHOW_STRING 0

#define MAX_DESC_INFO_LEN 4096
#define MAX_FILTER_LEN 1024
CHAR g_szFilter[ MAX_FILTER_LEN ] = { 0 };
#define ptr_is_struct_infos( ptr ) ( 0xffff0000 & ( dword )ptr ) 

#define FIND_SUB_TREE_TRAVERSE 0x01
#define STRUCT_TYPE_PE_FILE 0x4d5a
#define STRUCT_TYPE_LIB_FILE 0x600a
#define STRUCT_TYPE_COFF_FILE 0x041c

#define PE_FILE_TITLE "PE FILE [RCLICK]"
#define PE_DOS_HEADER_TITLE "PE DOS HEADER"
#define PE_DOS_STUB_TITLE "PE DOS STUB"
#define PE_NT_HEADER_TITLE "PE NT HEADER"
#define PE_OPTIONAL_HEADER_TITLE "PE OPTIONAL HEADER"
#define PE_SECTION_HEADER_TITLE "PE SECTION HEADER"
#define LIB_FILE_TITLE "LIB FILE"
#define LIB_SECTION1_TITLE "SECTION1"
#define LIB_SECTION2_TITLE "SECTION2"
#define LIB_LONGNAME_SECTION_TITLE "LONG NAME SECTION"
#define LIB_OBJ_FILE_SECTION_TITLE "OBJ FILE SECTION [RCLICK ME]"
#define LIB_SECTION1_SYM_TABLE_TITLE "Symbol table"
#define LIB_SECTION1_STR_TABLE_TITLE "String table"
#define LIB_SECTION2_OBJ_OFFSETS_TITLE "Object section offsets"
#define LIB_SECTION2_SYM_INDEXES_TITLE "Symbol indexes"
#define LIB_SECTION2_STR_TABLE_TITLE "String table"
#define LIB_SECTION_LONGNAME_STR_TABLE_TITLE "String table"
#define COFF_FILE_HEADER_TITLE "Coff file header"
#define COFF_FILE_SECTION_TITLE "Coff section"
#define COFF_FILE_STR_TABLE_TITLE "String table"

#define COFF_FILE_TITLE "COFF FILE"

typedef struct __sym_org_data
{
	byte *sym_data;
	dword sym_data_len;
} sym_org_data;

typedef struct __analyze_data
{
	byte *data;
	analyze_context *context;
} analyze_data;

//dword screen_dpi = USER_DEFAULT_SCREEN_DPI;
//dword dpi_scale_factor_x = 1;
//#include <math.h>
//int compensate_xdpi ( int val )
//{
//	if (screen_dpi == USER_DEFAULT_SCREEN_DPI)
//		return val;
//	else
//	{
//		double tmp_val = (double) val * dpi_scale_factor_x;
//
//		if (tmp_val > 0)
//			return (int) floor(tmp_val);
//		else
//			return (int) ceil(tmp_val);
//	}
//}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CpeanalyzerDlg 对话框


CpeanalyzerDlg *g_pDlg;
CpeanalyzerDlg::CpeanalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpeanalyzerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpeanalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpeanalyzerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEL_FILE, OnBnClickedButtonSelFile)
	ON_BN_CLICKED(IDC_STOP_ANALYZE, OnBnClickedStopAnalyze)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MAIN, OnTvnSelchangedTreeMain)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_MAIN, OnRClientTreeMain )
	ON_MESSAGE( WM_DO_UI_WORK, OnDoUIWork )
	ON_WM_DROPFILES()
	ON_WM_MEASUREITEM()
	ON_WM_INITMENUPOPUP()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DETAIL, OnLvnItemchangedListDetail)
END_MESSAGE_MAP()


// CpeanalyzerDlg 消息处理程序

void CpeanalyzerDlg::OnDropFiles( HDROP drop_info )   
{ 
	char file_name[ MAX_PATH ];
	char file_count_str[10];
	HWND edit_path;
	int32 file_count;
	int32 ret;
	int32 i; 

	file_count = ::DragQueryFile( drop_info, 0xFFFFFFFF, NULL, 512 ); 
	
	for( i = 0; i < file_count; i ++ ) 
	{ 
		ret =::DragQueryFile( drop_info, i, file_name, MAX_PATH );
		if( 0 != ret )
		{
			edit_path = ::GetDlgItem( m_hWnd, IDC_EDIT_FILE_PATH );
			ASSERT( NULL != edit_path );

			::SetWindowText( edit_path, file_name );
			OnBnClickedOk();
			break;
		}
	} 
	::DragFinish( drop_info );

	//CDialog::OnDropFiles( drop_info );   
} 

dword create_context_menu( HWND parent, char *menu_text )
{
	int32 ret;
	dword seled_menu;
	HMENU menu_popup;
	POINT cur_pt;
 
	ASSERT( NULL != parent );
	GetCursorPos( &cur_pt );

	menu_popup = CreatePopupMenu(); 
	ret = AppendMenu( menu_popup, MF_STRING, 
		(UINT)MENU_ITEM_ID_DUMP_FILE, menu_text );

	seled_menu = TrackPopupMenuEx( menu_popup, 
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, 
		cur_pt.x, cur_pt.y, parent, NULL); 

	//dword err = GetLastError();
	DestroyMenu( menu_popup ); 

    return seled_menu;
}

int32 init_wnd_feature( HWND hwnd )
{
	int32 ret;
	int32 i;
	HWND tree_main;
	HWND list_detail;
	char *columns_title[] = { "Address", "Raw data", "Description" };
	dword columns_len[] = { 120, 300, 250 };
	LVCOLUMN lvc;

	ASSERT( NULL != hwnd );

	tree_main = ( HWND )::GetDlgItem( hwnd, IDC_TREE_MAIN );
	list_detail = ( HWND )::GetDlgItem( hwnd, IDC_LIST_DETAIL );
	
	::SendMessage( tree_main, TVM_SETBKCOLOR, 0, 0x00e1f0ff );
	::SendMessage( tree_main, TVM_SETTEXTCOLOR, 0, 0x00000000 );
	::SendMessage( tree_main, TVM_SETLINECOLOR, 0, 0x00eeeee0 );
	::SendMessage( tree_main, TVM_SETINSERTMARKCOLOR, 0, 0x00eeeee0 );

	::SendMessage( list_detail, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, 
		LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );

	for( ; ; )
	{
		ret = ListView_DeleteColumn( list_detail, 1 );
		if( FALSE == ret )
		{
			break;
		}
	}

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	for ( i = 0; i < sizeof( columns_title ) / sizeof( columns_title[ 0 ] ); i++ ) 
	{ 
		lvc.pszText = columns_title[ i ];    
		lvc.cx = columns_len[ i ];

		lvc.fmt = LVCFMT_LEFT;                             

		if (ListView_InsertColumn( list_detail, i, &lvc ) == -1) 
			return FALSE; 
	}

	return 0;
}

HTREEITEM insert_text_in_tree( HWND tree, HTREEITEM tree_item, const char *str_insert, byte *data )
{
	TV_INSERTSTRUCT tvis;
	HTREEITEM sub_tree;

	ASSERT( NULL != tree );
	ASSERT( NULL != str_insert );

	if( TVI_ROOT == tree_item )
	{
		tvis.hParent = NULL;
		tvis.hInsertAfter = tree_item;
	}
	else
	{
		tvis.hParent = tree_item;
		tvis.hInsertAfter = NULL;
	}

	tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvis.item.pszText = ( char* )str_insert;
	tvis.item.iImage = -1;
	tvis.item.iSelectedImage = -1;
	tvis.item.lParam = ( DWORD )data;

	sub_tree = ( HTREEITEM )SendMessage( tree, TVM_INSERTITEM, 0, ( LPARAM )&tvis );
	return sub_tree;
}

int32 all_one_line_to_list( HWND list_detail, dword item_index, byte *data, byte *base_addr, dword data_len, char *descrition, dword flags )
{
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	LV_ITEM lv_item;
	dword out_str_len;

	memset( &lv_item, 0, sizeof( lv_item ) );
	lv_item.mask = LVIF_TEXT;

	sprintf( desc, "0x%0.8x", ( data - base_addr ) );
	lv_item.pszText = desc;
	lv_item.iItem = item_index;
	lv_item.iSubItem = 0;
	ListView_InsertItem( list_detail, &lv_item );

	out_str_len = MAX_DESC_INFO_LEN;
	if( flags ==  FLAG_SHOW_DATA )
	{
		dump_mem( data, data_len, desc, &out_str_len );
		lv_item.pszText = desc;
	}
	else if( flags == FLAG_SHOW_STRING )
	{
		lv_item.pszText = ( char* )data;
	}
	
	lv_item.iItem = item_index;
	lv_item.iSubItem = 1;
	ListView_SetItem( list_detail, &lv_item );

	lv_item.pszText = descrition;
	lv_item.iItem = item_index;
	lv_item.iSubItem = 2;
	ListView_SetItem( list_detail, &lv_item );

	return 0;
}

int32 add_pe_dos_header( struct_infos *info, file_analyzer *analyzer )
{
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	PIMAGE_DOS_HEADER dos_hdr;
	HWND list_detail;
	LV_ITEM lv_item;
	dword out_str_len;
	dword item_index;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	dos_hdr = ( PIMAGE_DOS_HEADER )info->struct_data;

	ListView_DeleteAllItems( list_detail );

	//typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
 //   WORD   e_magic;                     // Magic number
 //   WORD   e_cblp;                      // Bytes on last page of file
 //   WORD   e_cp;                        // Pages in file
 //   WORD   e_crlc;                      // Relocations
 //   WORD   e_cparhdr;                   // Size of header in paragraphs
 //   WORD   e_minalloc;                  // Minimum extra paragraphs needed
 //   WORD   e_maxalloc;                  // Maximum extra paragraphs needed
 //   WORD   e_ss;                        // Initial (relative) SS value
 //   WORD   e_sp;                        // Initial SP value
 //   WORD   e_csum;                      // Checksum
 //   WORD   e_ip;                        // Initial IP value
 //   WORD   e_cs;                        // Initial (relative) CS value
 //   WORD   e_lfarlc;                    // File address of relocation table
 //   WORD   e_ovno;                      // Overlay number
 //   WORD   e_res[4];                    // Reserved words
 //   WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
 //   WORD   e_oeminfo;                   // OEM information; e_oemid specific
 //   WORD   e_res2[10];                  // Reserved words
 //   LONG   e_lfanew;                    // File address of new exe header
 // } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

	item_index = 0;
	sprintf( desc, "Magic number: 0x%0.4x", dos_hdr->e_magic );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_magic, file_data, sizeof( dos_hdr->e_magic ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Bytes on last page of file: 0x%0.4x", dos_hdr->e_cblp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cblp, file_data, sizeof( dos_hdr->e_cblp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Pages in file: 0x%0.4x", dos_hdr->e_cp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cp, file_data, sizeof( dos_hdr->e_cp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Relocations: 0x%0.4x", dos_hdr->e_crlc  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_crlc, file_data, sizeof( dos_hdr->e_crlc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of header in paragraphs: 0x%0.4x", dos_hdr->e_cparhdr  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cparhdr, file_data, sizeof( dos_hdr->e_cparhdr ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Minimum extra paragraphs needed: 0x%0.4x", dos_hdr->e_minalloc  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_minalloc, file_data, sizeof( dos_hdr->e_minalloc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Maximum extra paragraphs needed: 0x%0.4x", dos_hdr->e_maxalloc );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_maxalloc, file_data, sizeof( dos_hdr->e_maxalloc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial (relative) SS value: 0x%0.4x", dos_hdr->e_ss  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_ss, file_data, sizeof( dos_hdr->e_ss ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial SP value: 0x%0.4x", dos_hdr->e_sp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_sp, file_data, sizeof( dos_hdr->e_sp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Checksum: 0x%0.4x", dos_hdr->e_csum  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_csum, file_data, sizeof( dos_hdr->e_csum ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial IP value: 0x%0.4x", dos_hdr->e_ip  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_ip, file_data, sizeof( dos_hdr->e_ip ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial (relative) CS value: 0x%0.4x", dos_hdr->e_cs  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cs, file_data, sizeof( dos_hdr->e_cs ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "File address of relocation table: 0x%0.4x", dos_hdr->e_lfarlc );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_lfarlc, file_data, sizeof( dos_hdr->e_lfarlc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Overlay number: %d", dos_hdr->e_ovno  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_ovno, file_data, sizeof( dos_hdr->e_ovno ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Reserved words: 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x", 
		dos_hdr->e_res[ 0 ], 
		dos_hdr->e_res[ 1 ], 
		dos_hdr->e_res[ 2 ], 
		dos_hdr->e_res[ 3 ]  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )dos_hdr->e_res, file_data, sizeof( dos_hdr->e_res ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "OEM id: 0x%0.4x", dos_hdr->e_oemid  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_oemid, file_data, sizeof( dos_hdr->e_oemid ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "OEM info: 0x%0.4x", dos_hdr->e_oeminfo  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_oeminfo, file_data, sizeof( dos_hdr->e_oeminfo ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Reserved words: 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x ", 
		dos_hdr->e_res2[ 0 ], 
		dos_hdr->e_res2[ 1 ],
		dos_hdr->e_res2[ 2 ],
		dos_hdr->e_res2[ 3 ],
		dos_hdr->e_res2[ 4 ], 
		dos_hdr->e_res2[ 5 ], 
		dos_hdr->e_res2[ 6 ],
		dos_hdr->e_res2[ 7 ],
		dos_hdr->e_res2[ 8 ],
		dos_hdr->e_res2[ 9 ] );
	all_one_line_to_list( list_detail, item_index++, ( byte* )dos_hdr->e_res2, file_data, sizeof( dos_hdr->e_res2 ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "NT header offset: 0x%0.8x", dos_hdr->e_lfanew );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_lfanew, file_data, sizeof( dos_hdr->e_lfanew ), desc, FLAG_SHOW_DATA );
	return 0;
}

int32 add_pe_dos_stub( struct_infos *info, file_analyzer *analyzer )
{
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	PIMAGE_DOS_HEADER dos_hdr;
	HWND list_detail;
	LV_ITEM lv_item;
	dword out_str_len;
	dword item_index;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	dos_hdr = ( PIMAGE_DOS_HEADER )info->struct_data;

	ListView_DeleteAllItems( list_detail );

	//typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
 //   WORD   e_magic;                     // Magic number
 //   WORD   e_cblp;                      // Bytes on last page of file
 //   WORD   e_cp;                        // Pages in file
 //   WORD   e_crlc;                      // Relocations
 //   WORD   e_cparhdr;                   // Size of header in paragraphs
 //   WORD   e_minalloc;                  // Minimum extra paragraphs needed
 //   WORD   e_maxalloc;                  // Maximum extra paragraphs needed
 //   WORD   e_ss;                        // Initial (relative) SS value
 //   WORD   e_sp;                        // Initial SP value
 //   WORD   e_csum;                      // Checksum
 //   WORD   e_ip;                        // Initial IP value
 //   WORD   e_cs;                        // Initial (relative) CS value
 //   WORD   e_lfarlc;                    // File address of relocation table
 //   WORD   e_ovno;                      // Overlay number
 //   WORD   e_res[4];                    // Reserved words
 //   WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
 //   WORD   e_oeminfo;                   // OEM information; e_oemid specific
 //   WORD   e_res2[10];                  // Reserved words
 //   LONG   e_lfanew;                    // File address of new exe header
 // } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

	item_index = 0;
	sprintf( desc, "Magic number: 0x%0.4x", dos_hdr->e_magic );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_magic, file_data, sizeof( dos_hdr->e_magic ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Bytes on last page of file: 0x%0.4x", dos_hdr->e_cblp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cblp, file_data, sizeof( dos_hdr->e_cblp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Pages in file: 0x%0.4x", dos_hdr->e_cp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cp, file_data, sizeof( dos_hdr->e_cp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Relocations: 0x%0.4x", dos_hdr->e_crlc  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_crlc, file_data, sizeof( dos_hdr->e_crlc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of header in paragraphs: 0x%0.4x", dos_hdr->e_cparhdr  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cparhdr, file_data, sizeof( dos_hdr->e_cparhdr ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Minimum extra paragraphs needed: 0x%0.4x", dos_hdr->e_minalloc  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_minalloc, file_data, sizeof( dos_hdr->e_minalloc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Maximum extra paragraphs needed: 0x%0.4x", dos_hdr->e_maxalloc );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_maxalloc, file_data, sizeof( dos_hdr->e_maxalloc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial (relative) SS value: 0x%0.4x", dos_hdr->e_ss  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_ss, file_data, sizeof( dos_hdr->e_ss ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial SP value: 0x%0.4x", dos_hdr->e_sp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_sp, file_data, sizeof( dos_hdr->e_sp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Checksum: 0x%0.4x", dos_hdr->e_csum  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_csum, file_data, sizeof( dos_hdr->e_csum ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial IP value: 0x%0.4x", dos_hdr->e_ip  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_ip, file_data, sizeof( dos_hdr->e_ip ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Initial (relative) CS value: 0x%0.4x", dos_hdr->e_cs  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_cs, file_data, sizeof( dos_hdr->e_cs ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "File address of relocation table: 0x%0.4x", dos_hdr->e_lfarlc );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_lfarlc, file_data, sizeof( dos_hdr->e_lfarlc ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Overlay number: %d", dos_hdr->e_ovno  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_ovno, file_data, sizeof( dos_hdr->e_ovno ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Reserved words: 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x", 
		dos_hdr->e_res[ 0 ], 
		dos_hdr->e_res[ 1 ], 
		dos_hdr->e_res[ 2 ], 
		dos_hdr->e_res[ 3 ]  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )dos_hdr->e_res, file_data, sizeof( dos_hdr->e_res ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "OEM id: 0x%0.4x", dos_hdr->e_oemid  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_oemid, file_data, sizeof( dos_hdr->e_oemid ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "OEM info: 0x%0.4x", dos_hdr->e_oeminfo  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_oeminfo, file_data, sizeof( dos_hdr->e_oeminfo ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Reserved words: 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x ", 
		dos_hdr->e_res2[ 0 ], 
		dos_hdr->e_res2[ 1 ],
		dos_hdr->e_res2[ 2 ],
		dos_hdr->e_res2[ 3 ],
		dos_hdr->e_res2[ 4 ], 
		dos_hdr->e_res2[ 5 ], 
		dos_hdr->e_res2[ 6 ],
		dos_hdr->e_res2[ 7 ],
		dos_hdr->e_res2[ 8 ],
		dos_hdr->e_res2[ 9 ] );
	all_one_line_to_list( list_detail, item_index++, ( byte* )dos_hdr->e_res2, file_data, sizeof( dos_hdr->e_res2 ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "NT header offset: 0x%0.8x", dos_hdr->e_lfanew );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&dos_hdr->e_lfanew, file_data, sizeof( dos_hdr->e_lfanew ), desc, FLAG_SHOW_DATA );
	return 0;
}

int32 add_pe_nt_header( struct_infos *info, file_analyzer *analyzer )
{
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	PIMAGE_FILE_HEADER file_hdr;;
	HWND list_detail;
	dword out_str_len;
	dword item_index;
//
//	typedef struct _IMAGE_FILE_HEADER {
//    WORD    Machine;
//    WORD    NumberOfSections;
//    DWORD   TimeDateStamp;
//    DWORD   PointerToSymbolTable;
//    DWORD   NumberOfSymbols;
//    WORD    SizeOfOptionalHeader;
//    WORD    Characteristics;
//} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	file_hdr = ( PIMAGE_FILE_HEADER )info->struct_data;

	ListView_DeleteAllItems( list_detail );

	item_index = 0;
	sprintf( desc, "Compatible machine: 0x%0.4x", file_hdr->Machine );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->Machine, file_data, sizeof( file_hdr->Machine ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Number of section: 0x%0.4x", file_hdr->NumberOfSections  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->NumberOfSections, file_data, sizeof( file_hdr->NumberOfSections ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Time stamp: 0x%0.8x", file_hdr->TimeDateStamp  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->TimeDateStamp, file_data, sizeof( file_hdr->TimeDateStamp ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Pointer of symbol table: 0x%0.8x", file_hdr->PointerToSymbolTable  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->PointerToSymbolTable, file_data, sizeof( file_hdr->PointerToSymbolTable ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Number of symbols: 0x%0.8x", file_hdr->NumberOfSymbols  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->NumberOfSymbols, file_data, sizeof( file_hdr->NumberOfSymbols ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of optional header: 0x%0.4x", file_hdr->SizeOfOptionalHeader  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->SizeOfOptionalHeader, file_data, sizeof( file_hdr->SizeOfOptionalHeader ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Characteristics: 0x%0.4x", file_hdr->Characteristics );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->Characteristics, file_data, sizeof( file_hdr->Characteristics ), desc, FLAG_SHOW_DATA );

	return 0;
}

int32 add_pe_section_info( struct_infos *info, file_analyzer *analyzer )
{
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	PIMAGE_SECTION_HEADER sect_hdr;;
	HWND list_detail;
	dword out_str_len;
	dword item_index;

	//typedef struct _IMAGE_SECTION_HEADER {
	//	BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
	//	union {
	//		DWORD   PhysicalAddress;
	//		DWORD   VirtualSize;
	//	} Misc;
	//	DWORD   VirtualAddress;
	//	DWORD   SizeOfRawData;
	//	DWORD   PointerToRawData;
	//	DWORD   PointerToRelocations;
	//	DWORD   PointerToLinenumbers;
	//	WORD    NumberOfRelocations;
	//	WORD    NumberOfLinenumbers;
	//	DWORD   Characteristics;
	//} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;
	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	sect_hdr = ( PIMAGE_SECTION_HEADER )info->struct_data;

	ListView_DeleteAllItems( list_detail );

	item_index = 0;
	sprintf( desc, "Name: %s", sect_hdr->Name );
	all_one_line_to_list( list_detail, item_index++, ( byte* )sect_hdr->Name, file_data, sizeof( sect_hdr->Name ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Physical address: 0x%0.8x", sect_hdr->Misc.PhysicalAddress  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->Misc.PhysicalAddress, file_data, sizeof( sect_hdr->Misc.PhysicalAddress ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Misc( Virtual address/Virtual Size ): 0x%0.8x", sect_hdr->VirtualAddress  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->VirtualAddress, file_data, sizeof( sect_hdr->VirtualAddress ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of raw data: 0x%0.8x", sect_hdr->SizeOfRawData  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->SizeOfRawData, file_data, sizeof( sect_hdr->SizeOfRawData ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Pointer of raw data: 0x%0.8x", sect_hdr->PointerToRawData  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->PointerToRawData, file_data, sizeof( sect_hdr->PointerToRawData ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Pointer of relocations: 0x%0.8x", sect_hdr->PointerToRelocations  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->PointerToRelocations, file_data, sizeof( sect_hdr->PointerToRelocations ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Pointer of line numbers: 0x%0.8x", sect_hdr->PointerToLinenumbers );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->PointerToLinenumbers, file_data, sizeof( sect_hdr->PointerToLinenumbers ), desc, FLAG_SHOW_DATA );
	
	sprintf( desc, "Number of relocations: 0x%0.4x", sect_hdr->NumberOfRelocations );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->NumberOfRelocations, file_data, sizeof( sect_hdr->NumberOfRelocations ), desc, FLAG_SHOW_DATA );
	
	sprintf( desc, "Characteristics: 0x%0.8x", sect_hdr->Characteristics );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->Characteristics, file_data, sizeof( sect_hdr->Characteristics ), desc, FLAG_SHOW_DATA );

	return 0;
}

int32 add_pe_optional_header( struct_infos *info, file_analyzer *analyzer )
{
	int32 i;
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	PIMAGE_OPTIONAL_HEADER opt_hdr;;
	HWND list_detail;
	LV_ITEM lv_item;
	dword out_str_len;
	dword item_index;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	opt_hdr = ( PIMAGE_OPTIONAL_HEADER )info->struct_data;

	ListView_DeleteAllItems( list_detail );
//
//typedef struct _IMAGE_OPTIONAL_HEADER {
//    //
//    // Standard fields.
//    //
//
//    WORD    Magic;
//    BYTE    MajorLinkerVersion;
//    BYTE    MinorLinkerVersion;
//    DWORD   SizeOfCode;
//    DWORD   SizeOfInitializedData;
//    DWORD   SizeOfUninitializedData;
//    DWORD   AddressOfEntryPoint;
//    DWORD   BaseOfCode;
//    DWORD   BaseOfData;
//
//    //
//    // NT additional fields.
//    //
//
//    DWORD   ImageBase;
//    DWORD   SectionAlignment;
//    DWORD   FileAlignment;
//    WORD    MajorOperatingSystemVersion;
//    WORD    MinorOperatingSystemVersion;
//    WORD    MajorImageVersion;
//    WORD    MinorImageVersion;
//    WORD    MajorSubsystemVersion;
//    WORD    MinorSubsystemVersion;
//    DWORD   Win32VersionValue;
//    DWORD   SizeOfImage;
//    DWORD   SizeOfHeaders;
//    DWORD   CheckSum;
//    WORD    Subsystem;
//    WORD    DllCharacteristics;
//    DWORD   SizeOfStackReserve;
//    DWORD   SizeOfStackCommit;
//    DWORD   SizeOfHeapReserve;
//    DWORD   SizeOfHeapCommit;
//    DWORD   LoaderFlags;
//    DWORD   NumberOfRvaAndSizes;
//    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
//} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

	item_index = 0;
	sprintf( desc, "Magic number: 0x%0.4x", opt_hdr->Magic );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->Magic, file_data, sizeof( opt_hdr->Magic ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Major linker version: 0x%0.2x", opt_hdr->MajorLinkerVersion  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MajorLinkerVersion, file_data, sizeof( opt_hdr->MajorLinkerVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Minor linker version: 0x%0.2x", opt_hdr->MinorLinkerVersion  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MinorLinkerVersion, file_data, sizeof( opt_hdr->MinorLinkerVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of code: %d", opt_hdr->SizeOfCode  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfCode, file_data, sizeof( opt_hdr->SizeOfCode ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of initialized data: %d", opt_hdr->SizeOfInitializedData  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfInitializedData, file_data, sizeof( opt_hdr->SizeOfInitializedData ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of uninitialized data: %d", opt_hdr->SizeOfUninitializedData  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfUninitializedData, file_data, sizeof( opt_hdr->SizeOfUninitializedData ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Address of entry point: 0x%0.8x", opt_hdr->AddressOfEntryPoint );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->AddressOfEntryPoint, file_data, sizeof( opt_hdr->AddressOfEntryPoint ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Base address of code: 0x%0.8x", opt_hdr->BaseOfCode  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->BaseOfCode, file_data, sizeof( opt_hdr->BaseOfCode ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Base address of data: 0x%0.8x", opt_hdr->BaseOfData  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->BaseOfData, file_data, sizeof( opt_hdr->BaseOfData ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Image base address: 0x%0.8x", opt_hdr->ImageBase  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->ImageBase, file_data, sizeof( opt_hdr->ImageBase ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section alignment: 0x%0.8x", opt_hdr->SectionAlignment  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SectionAlignment, file_data, sizeof( opt_hdr->SectionAlignment ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "File alignment: 0x%0.8x", opt_hdr->FileAlignment  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->FileAlignment, file_data, sizeof( opt_hdr->FileAlignment ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Major OS version: 0x%0.4x", opt_hdr->MajorOperatingSystemVersion );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MajorOperatingSystemVersion, file_data, sizeof( opt_hdr->MajorOperatingSystemVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Minor OS version: 0x%0.4x", opt_hdr->MinorOperatingSystemVersion  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MinorOperatingSystemVersion, file_data, sizeof( opt_hdr->MinorOperatingSystemVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Major image version: 0x%0.4x", opt_hdr->MajorImageVersion );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MajorImageVersion, file_data, sizeof( opt_hdr->MajorImageVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Minor image version: 0x%0.4x", opt_hdr->MinorImageVersion  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MinorImageVersion, file_data, sizeof( opt_hdr->MinorImageVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Major subsystem version: 0x%0.4x", opt_hdr->MajorSubsystemVersion  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MajorSubsystemVersion, file_data, sizeof( opt_hdr->MajorSubsystemVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Minor subsystem version: 0x%0.4x", opt_hdr->MinorSubsystemVersion );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->MinorSubsystemVersion, file_data, sizeof( opt_hdr->MinorSubsystemVersion ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Win32 version: 0x%0.8x", opt_hdr->Win32VersionValue );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->Win32VersionValue, file_data, sizeof( opt_hdr->Win32VersionValue ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of image: 0x%0.8x", opt_hdr->SizeOfImage );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfImage, file_data, sizeof( opt_hdr->SizeOfImage ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of headers: 0x%0.8x", opt_hdr->SizeOfHeaders );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfHeaders, file_data, sizeof( opt_hdr->SizeOfHeaders ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Check sum: 0x%0.8x", opt_hdr->CheckSum );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->CheckSum, file_data, sizeof( opt_hdr->CheckSum ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Subsystem: 0x%0.4x", opt_hdr->Subsystem );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->Subsystem, file_data, sizeof( opt_hdr->Subsystem ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Dll characteristics: 0x%0.4x", opt_hdr->DllCharacteristics );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->DllCharacteristics, file_data, sizeof( opt_hdr->DllCharacteristics ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of stack reserve: 0x%0.8x", opt_hdr->SizeOfStackReserve );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfStackReserve, file_data, sizeof( opt_hdr->SizeOfStackReserve ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of stack commit: 0x%0.8x", opt_hdr->SizeOfStackCommit );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfStackCommit, file_data, sizeof( opt_hdr->SizeOfStackCommit ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of heap reserve: 0x%0.8x", opt_hdr->SizeOfHeapReserve );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfHeapReserve, file_data, sizeof( opt_hdr->SizeOfHeapReserve ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Size of heap commit: 0x%0.8x", opt_hdr->SizeOfHeapCommit );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->SizeOfHeapCommit, file_data, sizeof( opt_hdr->SizeOfHeapCommit ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "PE loader flags: 0x%0.8x", opt_hdr->LoaderFlags );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->LoaderFlags, file_data, sizeof( opt_hdr->LoaderFlags ), desc, FLAG_SHOW_DATA );

	for( i = 0; i < sizeof( opt_hdr->DataDirectory ) / sizeof( opt_hdr->DataDirectory[ 0 ] ); i ++ )
	{
		sprintf( desc, "Data directory %d", i );
		all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->DataDirectory, file_data, sizeof( opt_hdr->DataDirectory[ i ] ), desc, FLAG_SHOW_DATA );

		sprintf( desc, "Virtual address: 0z%0.8x", opt_hdr->DataDirectory[ i ].VirtualAddress );
		all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->DataDirectory[ i ].VirtualAddress, file_data, sizeof( opt_hdr->DataDirectory[ i ].VirtualAddress ), desc, FLAG_SHOW_DATA );

		sprintf( desc, "Size: %d", opt_hdr->DataDirectory[ i ].Size );
		all_one_line_to_list( list_detail, item_index++, ( byte* )&opt_hdr->DataDirectory[ i ].Size, file_data, sizeof( opt_hdr->DataDirectory[ i ].Size ), desc, FLAG_SHOW_DATA );
	}
	return 0;
}

int32 add_lib_section_desc( struct_infos *info, file_analyzer *analyzer )
{
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	lib_section_hdr *section;
	lib_section_hdr sect_copy;
	HWND list_detail;
	dword out_str_len;
	dword item_index;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	section = ( lib_section_hdr* )info->struct_data;

	sect_copy = *section;
	clean_hdr_filled_bytes( &sect_copy );

	ListView_DeleteAllItems( list_detail );

	//char name[16];
	//char time[12];
	//char user_id[6];
	//char group_id[6];
	//char mode[8];
	//char size[10];
	//char end_of_header[2];

	item_index = 0;
	sprintf( desc, "Section name: %s", sect_copy.name  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->name, file_data, sizeof( section->name ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section time: %s", sect_copy.time  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->time, file_data, sizeof( section->time ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section user id: %s", sect_copy.user_id  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->user_id, file_data, sizeof( section->user_id ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section group id: %s", sect_copy.group_id  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->group_id, file_data, sizeof( section->group_id ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section mode: %s", sect_copy.mode  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->mode, file_data, sizeof( section->mode ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section size: %s", sect_copy.size  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->size, file_data, sizeof( section->size ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Section end of header: %c%c", sect_copy.end_of_header[0], sect_copy.end_of_header[1] );
	all_one_line_to_list( list_detail, item_index++, ( byte* )section->end_of_header, file_data, sizeof( section->end_of_header ), desc, FLAG_SHOW_DATA );

	return 0;
}

int32 add_index_info( struct_infos *info, file_analyzer *analyzer, char *mask_str )
{
	int32 i;
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	dword *sym_offset;
	dword sym_off_num;
	HWND list_detail;
	dword out_str_len;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	sym_offset = ( dword* )info->struct_data;
	sym_off_num = info->data_len;

	ListView_DeleteAllItems( list_detail );

	for( i = 0; i < sym_off_num; i ++ )
	{
		sprintf( desc, mask_str, i + 1 );
		all_one_line_to_list( list_detail, i, ( byte* )( sym_offset + i ), file_data, sizeof( *sym_offset ), desc, FLAG_SHOW_DATA );
	}

	return 0;
}



int32 add_str_table_info( struct_infos *info, file_analyzer *analyzer )
{
	int ret;
	char *syms_str_table;
	dword syms_str_table_len;
	dword syms_str_offset;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	HWND list_detail;
	dword item_index;
	dword out_str_len;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	syms_str_table = ( char* )info->struct_data;
	syms_str_table_len = info->data_len;

	ListView_DeleteAllItems( list_detail );

	item_index = 0;
	syms_str_offset = 0;
	for(;; )
	{
		sprintf( desc, "String %d at 0x%0.8x", item_index, syms_str_offset );
		all_one_line_to_list( list_detail, item_index++, ( byte* )( syms_str_table ), file_data, strlen( syms_str_table ), desc, FLAG_SHOW_STRING );
		;
		syms_str_offset += strlen( syms_str_table ) + sizeof( char );
		syms_str_table += strlen( syms_str_table ) + sizeof( char );

		ASSERT( syms_str_offset <= syms_str_table_len );
		if( syms_str_offset == syms_str_table_len )
		{
			break;
		}
	}

	return 0;
}

int32 add_sym_table_info( struct_infos *info, file_analyzer *analyzer )
{
	return add_index_info( info, analyzer, "Symbol offset %d" );
}

int32 add_obj_offset_info( struct_infos *info, file_analyzer *analyzer )
{
	return add_index_info( info, analyzer, "Object section offset: %d" );
}

int32 add_sym_index_info( struct_infos *info, file_analyzer *analyzer )
{
	return add_index_info( info, analyzer, "Object symbol index: %d" );
}

int32 add_coff_file_hdr_info( struct_infos *info, file_analyzer *analyzer )
{
	int32 i;
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	byte *file_data;
	analyze_context *context;
	coff_file_hdr *file_hdr;
	HWND list_detail;
	dword out_str_len;
	dword item_index;

	//typedef struct __coff_file_hdr{
	//	unsigned short magic;
	//	unsigned short sect_num;
	//	unsigned long  time;
	//	unsigned long  syms_offset;
	//	unsigned long  syms_num;
	//	unsigned short opt_hdr_size;
	//	unsigned short flags;
	//} coff_file_hdr;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	file_hdr = ( coff_file_hdr* )info->struct_data;

	ListView_DeleteAllItems( list_detail );

	item_index = 0;
	sprintf( desc, "Coff file magic number: 0x%0.2x", file_hdr->magic  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->magic, file_data, sizeof( file_hdr->magic ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file section nubmer: %d", file_hdr->sect_num  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->sect_num, file_data, sizeof( file_hdr->sect_num ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file time: %d", file_hdr->time  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->time, file_data, sizeof( file_hdr->time ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file symbol offset: 0x%0.8x", file_hdr->syms_offset  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->syms_offset, file_data, sizeof( file_hdr->syms_offset ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file symbol number: %d", file_hdr->syms_num  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->syms_num, file_data, sizeof( file_hdr->syms_num ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file optional header sizee: %d", file_hdr->opt_hdr_size  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->opt_hdr_size, file_data, sizeof( file_hdr->opt_hdr_size ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file flag: 0x%0.8x", file_hdr->flags );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&file_hdr->flags, file_data, sizeof( file_hdr->flags ), desc, FLAG_SHOW_DATA );

	return 0;
}

int32 add_coff_optional_28_hdr_info( struct_infos *info, file_analyzer *analyzer )
{
	return 0;
}

int32 add_coff_section_hdr_info( struct_infos *info, file_analyzer *analyzer )
{
	int32 i;
	int32 ret;
	char desc[ MAX_DESC_INFO_LEN ];
	char sect_name[ 8 ];
	byte *file_data;
	analyze_context *context;
	coff_sect_hdr *sect_hdr;
	HWND list_detail;
	dword out_str_len;
	dword item_index;

	file_data = analyzer->all_file_data;

	context = ( analyze_context* )analyzer->context;
	list_detail = context->list_detail;
	sect_hdr = ( coff_sect_hdr* )info->struct_data;

	ListView_DeleteAllItems( list_detail );

	//char           name[8];
	//unsigned long  virt_size;
	//unsigned long  virt_addr;
	//unsigned long  size;
	//unsigned long  sect_offset;
	//unsigned long  sect_rel_offset;
	//unsigned long  ln_table_offset;
	//unsigned short rel_offset_num;
	//unsigned short ln_num;
	//unsigned long  flags;

	item_index = 0;

	memcpy( sect_name, sect_hdr->name, sizeof( sect_hdr->name ) );

	sect_name[ sizeof( sect_hdr->name ) - 1 ] = '\0';

	sprintf( desc, "Coff file name: %s", sect_name  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )sect_hdr->name, file_data, sizeof( sect_hdr->name ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file section nubmer: %d", sect_hdr->virt_size  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->virt_size, file_data, sizeof( sect_hdr->virt_size ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file time: %d", sect_hdr->virt_addr  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->virt_addr, file_data, sizeof( sect_hdr->virt_addr ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file symbol offset: %d", sect_hdr->size  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->size, file_data, sizeof( sect_hdr->size ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file symbol number: %d", sect_hdr->sect_rel_offset  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->sect_rel_offset, file_data, sizeof( sect_hdr->sect_rel_offset ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file optional header sizee: %d", sect_hdr->ln_table_offset  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->ln_table_offset, file_data, sizeof( sect_hdr->ln_table_offset ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file optional header sizee: %d", sect_hdr->ln_num  );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->ln_num, file_data, sizeof( sect_hdr->ln_num ), desc, FLAG_SHOW_DATA );

	sprintf( desc, "Coff file flag: 0x%0.8x", sect_hdr->flags );
	all_one_line_to_list( list_detail, item_index++, ( byte* )&sect_hdr->flags, file_data, sizeof( sect_hdr->flags ), desc, FLAG_SHOW_DATA );

	return 0;
}

HTREEITEM find_sub_tree_in_tree( HWND tree_main, HTREEITEM tree_item, dword struct_type, dword struct_index, dword flags )
{
#define MAX_TREE_ITEM_TITLE_LEN 512
	int ret;
	struct_infos *info;
	HTREEITEM sub_tree;
	HTREEITEM sib_tree;
	HTREEITEM ret_tree;
	TVITEM tv_item;
	char geted_str[ MAX_TREE_ITEM_TITLE_LEN ];

	ASSERT( NULL != tree_item );

	if( TVI_ROOT != tree_item )
	{
		memset( &tv_item, 0, sizeof( tv_item ) );
		tv_item.mask = TVIF_PARAM;
		//tv_item.pszText = geted_str;
		//tv_item.cchTextMax = MAX_TREE_ITEM_TITLE_LEN;
		tv_item.hItem = tree_item;
		ret = TreeView_GetItem( tree_main, &tv_item );
		if( FALSE == ret )
		{
			return NULL;
		}
		
		if( STRUCT_TYPE_PE_FILE == struct_type )
		{
			if( STRUCT_TYPE_PE_FILE == tv_item.lParam )
			{
				return sub_tree;
			}
		}
		else if( STRUCT_TYPE_LIB_FILE == struct_type )
		{
			if( STRUCT_TYPE_LIB_FILE == tv_item.lParam )
			{
				return sub_tree;
			}
		}
		else if( STRUCT_TYPE_COFF_FILE == struct_type )
		{
			if( STRUCT_TYPE_COFF_FILE == tv_item.lParam )
			{
				return sub_tree;
			}
		}

		info = ( struct_infos* )tv_item.lParam;
		if( NULL != info )
		{
			if( info->struct_id == struct_type &&
				info->struct_index == struct_index )
			{
				return sub_tree;
			}
		}
	}

	sub_tree = TreeView_GetChild( tree_main, tree_item );
	if( NULL == sub_tree )
	{
		return NULL;
	}

	memset( &tv_item, 0, sizeof( tv_item ) );
	tv_item.mask = TVIF_PARAM;
	//tv_item.pszText = geted_str;
	//tv_item.cchTextMax = MAX_TREE_ITEM_TITLE_LEN;
	tv_item.hItem = sub_tree;
	ret = TreeView_GetItem( tree_main, &tv_item );
	if( FALSE == ret )
	{
		return NULL;
	}

	if( STRUCT_TYPE_PE_FILE == struct_type )
	{
		if( STRUCT_TYPE_PE_FILE == tv_item.lParam )
		{
			return sub_tree;
		}
	}
	else if( STRUCT_TYPE_LIB_FILE == struct_type )
	{
		if( STRUCT_TYPE_LIB_FILE == tv_item.lParam )
		{
			return sub_tree;
		}
	}
	else if( STRUCT_TYPE_COFF_FILE == struct_type )
	{
		if( STRUCT_TYPE_COFF_FILE == tv_item.lParam )
		{
			return sub_tree;
		}
	}

	info = ( struct_infos* )tv_item.lParam;
	if( NULL != info )
	{
		if( info->struct_id == struct_type &&
			info->struct_index == struct_index )
		{
			return sub_tree;
		}
	}

	if( flags & FIND_SUB_TREE_TRAVERSE )
	{
		ret_tree = find_sub_tree_in_tree( tree_main, sub_tree, struct_type, struct_index, flags );
		if( NULL != ret_tree )
		{
			return ret_tree;
		}
	}

	sib_tree = TreeView_GetNextSibling( tree_main, sub_tree );
	if( NULL == sib_tree )
	{
		return NULL;
	}

	ret_tree = find_sub_tree_in_tree( tree_main, sib_tree, struct_type, struct_index, flags );
	if( NULL != ret_tree )
	{
		return ret_tree;
	}

	return NULL;
}

BOOL CpeanalyzerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	memset( &analyzing_context, 0, sizeof( analyzing_context ) );
	//GetDlgItem( IDC_EDIT_FILTER )->SetWindowText( "CString" );
	//GetDlgItem( IDC_EDIT_PE_FILE_PATH )->SetWindowText( "C:\\WINDDK\\2600.1106\\lib\\wxp\\i386\\mfc42.lib" ); /*"E:\\Visual C++ 6.0 SP6简体中文版\\VC98\\LIB\\MSUILSTF.DLL" );*///"lib_sample.lib" );
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	init_wnd_feature( m_hWnd );

	g_pDlg = this;
	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CpeanalyzerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpeanalyzerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CpeanalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int sum( int num1, int num2 )
{
	return num1 + num2;
}

void dummy()
{
	int dummy = 0;
}

int ret_sample_func_code( byte **org_func_code, dword *func_code_len )
{
	int sum_val;
	dword func_addr_begin;
	dword func_addr_end;
	byte *func_code;

	sum_val = sum( 3, 6 );

	func_addr_begin = ( DWORD )( VOID* )sum;
	func_addr_end = ( DWORD )( VOID* )dummy;

	ASSERT( func_addr_end > func_addr_begin );

	func_code = ( BYTE* )malloc( func_addr_end - func_addr_begin );

	ASSERT( NULL != func_code );
	memcpy( func_code, ( void* )func_addr_begin, func_addr_end - func_addr_begin );

	if( *func_code == 0xe9 )
	{
		dword offset;
		offset = *( dword* )( func_code + sizeof( byte ) );
		offset += sizeof( dword ) + sizeof( byte );
		func_addr_begin += offset;
		func_addr_end += offset;
		func_addr_end = ( dword )memchr( ( void* )func_addr_begin, 0xcc, ( dword )( func_addr_end - func_addr_begin ) );
		if( NULL == func_addr_end )
		{
			return -1;
		}
	}

	memcpy( func_code, ( void* )func_addr_begin, func_addr_end - func_addr_begin );

	*org_func_code = func_code;
	*func_code_len = func_addr_end - func_addr_begin;

	return 0;
}

BOOL g_bStop;


int32 error_handle( error_infos *info )
{
	switch( info->err_code )
	{
	default:
		break;
	}

	return 0;
}

#define MAX_PE_OPTIONAL_DESC_LEN 4096
int32 analyze_pe_optional_hdr( PIMAGE_OPTIONAL_HEADER option_hdr, analyze_context *context )
{
	//HWND tree_main;
	//HWND list_detail;
	//HWND sub_tree;
	//HWND ret;

	//char opt_hdr_desc[ MAX_PE_OPTIONAL_DESC_LEN ];
	//ASSERT( NULL != option_hdr );
	//ASSERT( NULL != context );
	//ASSERT( NULL != context->tree_main );
	//ASSERT( NULL != context->list_detail );
	//
	//tree_main = context->tree_main;
	//list_detail = context->list_detail;

	//option_hdr->AddressOfEntryPoint;

	//sub_tree = insert_text_in_tree( tree_main, "PE Optional Header", ( byte* )option_hdr );

	//printf( opt_hdr_desc, "PE Image base : 0x%0.8x", option_hdr->ImageBase );
	//ret = insert_text_in_tree( sub_tree, opt_hdr_desc, NULL );

	//printf( opt_hdr_desc, "PE optional header signature: 0x%0.4x", option_hdr->Magic );
	//ret = insert_text_in_tree( sub_tree, opt_hdr_desc, NULL );

	//printf( opt_hdr_desc, "PE Section alignment: 0x%0.8x", option_hdr->SectionAlignment );
	//ret = insert_text_in_tree( sub_tree, opt_hdr_desc, NULL );

	//printf( opt_hdr_desc, "PE Section alignment: 0x%0.8x", option_hdr->FileAlignment );
	//ret = insert_text_in_tree( sub_tree, opt_hdr_desc, NULL );

	//printf( opt_hdr_desc, "PE Minor subsytem version: 0x%0.8x", option_hdr->MinorSubsystemVersion );
	//ret = insert_text_in_tree( sub_tree, opt_hdr_desc, NULL );

	//printf( opt_hdr_desc, "PE Minor subsytem version: 0x%0.8x", option_hdr->MinorSubsystemVersion );
	//ret = insert_text_in_tree( sub_tree, opt_hdr_desc, NULL );

	//option_hdr->SizeOfImage;
	//option_hdr->SizeOfHeaders;
	//option_hdr->CheckSum; //CheckSumMappedFile()
	//option_hdr->SizeOfStackReserve;
	//option_hdr->SizeOfStackCommit;
	//option_hdr->SizeOfHeapReserve;
	//option_hdr->SizeOfHeapCommit;
	return 0;
}

int32 dump_obj_file( char *org_file_name, byte *file_data, dword file_data_len )
{
	return write_to_new_file_by_name( org_file_name, file_data, file_data_len );
}

int32 analyze_obj_file( obj_file_info *info, void *context )
{
	char file_path[ MAX_PATH ];
	char *path_delim;
	analyze_context *__context;

	__context = ( analyze_context* )context;
	
	strcpy( file_path, __context->file_path );

	path_delim = strrchr( file_path, '\\' );
	
	if( path_delim == NULL )
	{
		return -1;
	}

	*path_delim = '\0';

	return write_to_new_file( path_delim, info->file_name, info->file_data, info->file_data_len );
}

int32 insert_text_in_main_tree( char *text, analyze_context *context, dword id )
{
	HWND tree_main;
	HTREEITEM tree_self;

	ASSERT( NULL != context );

	tree_main = context->tree_main;

	tree_self = insert_text_in_tree( tree_main, TVI_ROOT, text, ( byte* )id );
	if( NULL == tree_self )
	{
		return -1;
	}

	return 0;
}

int32 insert_item_in_seted_item( dword struct_type, dword index, char* text, struct_infos *info, analyze_context *context )
{ 
	int32 ret;
	HWND tree_main;
	struct_infos *find_info;
	HTREEITEM tree_target;
	HTREEITEM tree_self;
	HTREEITEM tree_ret;

	ASSERT( NULL != info );
	ASSERT( NULL != context );

	tree_main = context->tree_main;
	info->param2 = NULL;

	if( STRUCT_TYPE_PE_FILE == struct_type || 
		STRUCT_TYPE_LIB_FILE == struct_type || 
		STRUCT_TYPE_COFF_FILE == struct_type )
	{
		tree_target = find_sub_tree_in_tree( tree_main, TVI_ROOT, struct_type, index, 0 );
		if( NULL == tree_target )
		{
			return -1;
		}
	}
	else
	{
		find_info = find_struct_info_by_id( struct_type, index );
		if( NULL == find_info )
		{
			return -1;
		}

		if( NULL == find_info->param2 )
		{
			ASSERT( FALSE );
			return -1;
		}

		tree_target = ( HTREEITEM )find_info->param2;
	}

	

	tree_self = insert_text_in_tree( tree_main, tree_target, text, ( byte* )info );
	if( NULL == tree_self )
	{
		return -1;
	}

	info->param2 = ( dword )tree_self;
	return 0;
}


int32 analyze_lib_section_longname( lib_section_hdr *section, struct_infos *info, analyze_context *costext )
{

	char desc[ MAX_DESC_INFO_LEN ];

	HWND tree_main;
	HTREEITEM tree_target;
	HTREEITEM tree_sub;

	tree_main = costext->tree_main;

	tree_target = find_sub_tree_in_tree( tree_main, TVI_ROOT, STRUCT_TYPE_LIB_FILE, 0, 0 );
	if( NULL == tree_target )
	{
		return -1;
	}

	tree_sub = insert_text_in_tree( tree_main, tree_target, LIB_LONGNAME_SECTION_TITLE, ( byte* )info );
	if( NULL == tree_sub )
	{
		return -1;
	}

	return 0;
}

int32 analyze_lib_section_obj_file( lib_section_hdr *obj_file_sect, struct_infos *info, analyze_context *costext )
{

	char desc[ MAX_DESC_INFO_LEN ];

	HWND tree_main;
	HTREEITEM tree_target;
	HTREEITEM tree_sub;

	tree_main = costext->tree_main;

	tree_target = find_sub_tree_in_tree( tree_main, TVI_ROOT, STRUCT_TYPE_LIB_FILE, 0, 0 );
	if( NULL == tree_target )
	{
		return -1;
	}

	sprintf( desc, "%s%d", LIB_OBJ_FILE_SECTION_TITLE, info->struct_index + 1 );
	tree_sub = insert_text_in_tree( tree_main, tree_target, desc, ( byte* )info );

	if( NULL == tree_sub )
	{
		return -1;
	}

	return 0;
}

int32 analyze_lib_section1( lib_section_hdr *section1, struct_infos *info, analyze_context *context )
{
	//char desc[ MAX_DESC_INFO_LEN ];

	int32 ret;
	HWND tree_main;
	HTREEITEM tree_target;
	HTREEITEM tree_sub;
	HTREEITEM tree_self;
	HTREEITEM tree_ret;

	tree_main = context->tree_main;

	tree_sub = insert_text_in_tree( tree_main, TVI_ROOT, LIB_FILE_TITLE, ( byte* )STRUCT_TYPE_LIB_FILE );

	if( NULL == tree_sub )
	{
		return -1;
	}

	tree_self = insert_text_in_tree( tree_main, tree_sub, LIB_SECTION1_TITLE, ( byte* )info );
	if( NULL == tree_self )
	{
		return -1;
	}

	tree_ret = insert_text_in_tree( tree_main, tree_self, "Lib section symbol table", ( byte* )info );
	if( NULL == tree_ret )
	{
		return -1;
	}
	return 0;
}

int32 analzye_all_struct( struct_infos *struct_info, void *context )
{
	int ret;
	analyze_context *__context;
	struct_infos *__struct_info;
	ASSERT( NULL != struct_info );
	ASSERT( NULL != context );

	ret = 0;

	__context = ( analyze_context* )context;
	__struct_info = struct_info;

	switch( __struct_info->struct_id )
	{
	case STRUCT_TYPE_PE_DOS_HEADER:
		ret = insert_text_in_main_tree( PE_FILE_TITLE, __context, STRUCT_TYPE_PE_FILE );
		if( 0 > ret )
		{
			goto __error;
		}
		ret = insert_item_in_seted_item( STRUCT_TYPE_PE_FILE, 0, PE_DOS_HEADER_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_PE_DOS_STUB:
		ret = insert_item_in_seted_item( STRUCT_TYPE_PE_FILE, 0, PE_DOS_STUB_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_PE_NT_HEADER:
		ret = insert_item_in_seted_item( STRUCT_TYPE_PE_FILE, 0, PE_NT_HEADER_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_PE_OPTIONAL_HEADER:
		ret = insert_item_in_seted_item( STRUCT_TYPE_PE_FILE, 0, PE_OPTIONAL_HEADER_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_PE_SECTION:
		ret = insert_item_in_seted_item( STRUCT_TYPE_PE_FILE, 0, PE_SECTION_HEADER_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_LIB_SECTION1:
		ret = insert_text_in_main_tree( LIB_FILE_TITLE, __context, STRUCT_TYPE_LIB_FILE );
		if( 0 > ret )
		{
			goto __error;
		}
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_FILE, 0, LIB_SECTION1_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_LIB_SECTION2:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_FILE, 0, LIB_SECTION2_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_LIB_SECTION_LONGNAME:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_FILE, 0, LIB_LONGNAME_SECTION_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_LIB_SECTION_OBJ_FILE:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_FILE, 0, LIB_OBJ_FILE_SECTION_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_SYM_TABLE:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION1, 0, LIB_SECTION1_SYM_TABLE_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_SECTION1_STR_TABLE:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION1, 0, LIB_SECTION1_STR_TABLE_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_OBJ_OFFSETS:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION2, 0, LIB_SECTION2_OBJ_OFFSETS_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_SYM_INDEXES:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION2, 0, LIB_SECTION2_SYM_INDEXES_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_SECTION2_STR_TABLE:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION2, 0, LIB_SECTION2_STR_TABLE_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_LONGNAME_SECTION_STR_TABLE:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION_LONGNAME, 0, LIB_SECTION_LONGNAME_STR_TABLE_TITLE, __struct_info, __context );
		break;
	case STRUCT_TYPE_COFF_FILE_HEADER:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION_OBJ_FILE, __struct_info->param3, COFF_FILE_HEADER_TITLE, __struct_info, __context );
		if( 0 > ret )
		{
			ret = insert_text_in_main_tree( COFF_FILE_TITLE, __context, STRUCT_TYPE_COFF_FILE );
			if( 0 > ret )
			{
				goto __error;
			}

			ret = insert_item_in_seted_item( STRUCT_TYPE_COFF_FILE, 0, COFF_FILE_HEADER_TITLE, __struct_info, __context );
		}
		break;
	case STRUCT_TYPE_COFF_SECTION_HEADER:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION_OBJ_FILE, __struct_info->param3, COFF_FILE_SECTION_TITLE, __struct_info, __context );
		if( 0 > ret )
		{
			ret = insert_item_in_seted_item( STRUCT_TYPE_COFF_FILE, 0, COFF_FILE_SECTION_TITLE, __struct_info, __context );
		}
		break;
	case STRUCT_TYPE_COFF_STR_TABLE:
		ret = insert_item_in_seted_item( STRUCT_TYPE_LIB_SECTION_OBJ_FILE, __struct_info->param3, COFF_FILE_STR_TABLE_TITLE, __struct_info, __context );
		if( 0 > ret )
		{
			ret = insert_item_in_seted_item( STRUCT_TYPE_COFF_FILE, 0, COFF_FILE_STR_TABLE_TITLE, __struct_info, __context );
		}
		break;
	case STRUCT_TYPE_PE_FILL_DATA:
		break; 
	default:
		ASSERT( FALSE );	
		ret = -1;
		break;
	}

__error:
	return ret;
}

int when_func_code_finded( code_infos* code_info, void *context )
{
	ASSERT( NULL != context );
	sym_org_data *org_code = ( sym_org_data* )context;

	if( NULL != org_code->sym_data && 0 != org_code->sym_data_len )
	{
		if( 0 <= mem_submem( org_code->sym_data, org_code->sym_data_len, code_info->func_code, code_info->func_code_len ) )
		{
			::MessageBox( NULL, code_info->func_name, NULL, NULL );
		}
	}
	return 0;
}

int when_find_lib_func_name( sym_infos* sym_info, void *context )
{
	CString strAddLine;
	CString strText;
	CRichEditCtrl *pRichEdit;
	ASSERT( NULL != sym_info );
	if( NULL == sym_info->sym_name )
	{
		return -1;
	}
	
	if( NULL == strstr( sym_info->sym_name, g_szFilter ) )
	{
		return -1;
	}	

	//	if( NULL != strstr( sym_info->sym_name, "/" ) )
	//{
	//	int kk = 0;
	//	//return -1;
	//}	
	//pRichEdit = ( CRichEditCtrl *)g_pDlg->GetDlgItem( IDC_EDIT_OUTPUT );
	//
	//pRichEdit->GetWindowText( strText );
	//strAddLine.Format( "%s #%d\r\n", sym_info->sym_name, 0 );

	//strText += strAddLine;

	//pRichEdit->SetWindowText( strText );
	return 0;
}

#include "addsectiondlg.h"
#include "pe_writer.h"
int32 on_main_tree_item_rclick( file_analyzer *analyzer )
{
	int32 ret;
	dword seled_menu_id;
	TVITEM tvi;
	HTREEITEM seled_item;
	char seled_file_path[ MAX_PATH ];
	analyze_context *context;
	struct_infos *info;

	context = ( analyze_context* )analyzer->context;

	seled_item = TreeView_GetSelection( context->tree_main );
	if( NULL == seled_item )
	{
		return 0;
	}

	memset( &tvi, 0, sizeof( tvi ) );
	tvi.mask = TVIF_PARAM;
	tvi.hItem = seled_item;

	ret = TreeView_GetItem( context->tree_main, &tvi );
	if( FALSE == ret )
	{
		return -1;
	}

	info = ( struct_infos* )tvi.lParam;

/*	if( NULL != strstr( str_geted, LIB_SECTION1_TITLE ) )
	{

	}
	else if( NULL != strstr( str_geted, LIB_SECTION2_TITLE ) )
	{

	}
	else if( NULL != strstr( str_geted, LIB_LONGNAME_SECTION_TITLE ) )
	{

	}
	else */
	if( ( dword )info == STRUCT_TYPE_PE_FILE )
	{
		IMAGE_SECTION_HEADER sect_hdr; 
		CaddsectionDlg Dlg( &sect_hdr, NULL ); 

		PBYTE pe_file;

		if( analyzer->pe_write_info == NULL )
		{
			return -1; 
		}

		seled_menu_id = SendMessage( context->main_wnd, WM_DO_UI_WORK, 0, ( LPARAM )( PVOID )"&add new section" );

		if( seled_menu_id != MENU_ITEM_ID_DUMP_FILE )
		{
			return 0;
		}

		if( IDOK != Dlg.DoModal() )
		{
			return 0; 
		}

		ret = open_file_dlg( context->main_wnd, seled_file_path, MAX_PATH, 1 );
		if( 0 > ret )
		{
			return -1;
		}

		ret = add_new_section( &sect_hdr, analyzer->pe_write_info, analyzer ); 
		if( 0 > ret )
		{
			return ret; 
		}

		return write_pe_structs_to_file( analyzer->pe_write_info, seled_file_path, ( ( ( analyze_context* )analyzer->context )->file_path ) );

	}
	
	if( FALSE == ptr_is_struct_infos( tvi.lParam ) )
	{
		return 0;
	}

	if( info->struct_id == STRUCT_TYPE_LIB_SECTION_OBJ_FILE )
	{
		lib_section_hdr *section;
		seled_menu_id = SendMessage( context->main_wnd, WM_DO_UI_WORK, 0, ( LPARAM )( PVOID )"&dump this .obj file" );

		if( seled_menu_id != MENU_ITEM_ID_DUMP_FILE )
		{
			return 0;
		}

		ret = open_file_dlg( context->main_wnd, seled_file_path, MAX_PATH, 1 );
		if( 0 > ret )
		{
			return -1;
		}
		
		ASSERT( NULL != info->struct_data );
		section = ( lib_section_hdr* )info->struct_data;
		dump_obj_file( seled_file_path, ( byte* )section + sizeof( lib_section_hdr ), atoi( section->size ) );
	}

	return 0;
}

int32 on_detail_tree_item_seled( HTREEITEM item_seled, file_analyzer *analyzer )
{
	int32 ret;
	TVITEM tvi;
	char str_geted[ MAX_TREE_ITEM_TITLE_LEN ];
	analyze_context *context;

	context = ( analyze_context* )analyzer->context;

	memset( &tvi, 0, sizeof( tvi ) );
	tvi.hItem = item_seled;
	tvi.pszText = str_geted;
	tvi.cchTextMax = MAX_TREE_ITEM_TITLE_LEN;

	ret = TreeView_GetItem( context->tree_main, &tvi );
	if( FALSE == ret )
	{
		return -1;
	}

	return 0;
}

int32 on_main_tree_item_seled( HTREEITEM item_seled, file_analyzer *analyzer )
{
	int32 ret;
	HWND tree_main;
	TVITEM tv_item;
	analyze_context *context;
	struct_infos *info;

	ASSERT( NULL != item_seled );
	ASSERT( TVI_ROOT != item_seled );
	ASSERT( NULL != analyzer );
	ASSERT( NULL != analyzer->context );

	context = ( analyze_context* )analyzer->context;

	tree_main = context->tree_main;

	memset( &tv_item, 0, sizeof( tv_item ) );
	tv_item.mask = TVIF_PARAM;
	//tv_item.pszText = geted_str;
	//tv_item.cchTextMax = MAX_TREE_ITEM_TITLE_LEN;
	tv_item.hItem = item_seled;
	ret = TreeView_GetItem( tree_main, &tv_item );
	if( FALSE == ret )
	{
		return -1;
	}

	info = ( struct_infos* )tv_item.lParam;

	if( NULL == info )
	{
		return -1;
	}

	if( ptr_is_struct_infos( info ) )
	{
		switch( info->struct_id )
		{
		case STRUCT_TYPE_PE_DOS_HEADER:
			ret = add_pe_dos_header( info, analyzer );
			break;
		case STRUCT_TYPE_PE_DOS_STUB:
			ret = add_pe_dos_stub( info, analyzer );
			break;
		case STRUCT_TYPE_PE_NT_HEADER:
			ret = add_pe_nt_header( info, analyzer );
			break;
		case STRUCT_TYPE_PE_OPTIONAL_HEADER:
			ret = add_pe_optional_header( info, analyzer );
			break;
		case STRUCT_TYPE_PE_SECTION:
			ret = add_pe_section_info( info, analyzer );
			break;
		case STRUCT_TYPE_LIB_SECTION1:
			ret = add_lib_section_desc( info, analyzer );
			break;
		case STRUCT_TYPE_LIB_SECTION2:
			ret = add_lib_section_desc( info, analyzer );
			break;
		case STRUCT_TYPE_SYM_TABLE:
			ret = add_sym_table_info( info, analyzer );
			break;
		case STRUCT_TYPE_SECTION1_STR_TABLE:
			ret = add_str_table_info( info, analyzer );
			break;
		case STRUCT_TYPE_OBJ_OFFSETS:
			ret = add_obj_offset_info( info, analyzer );
			break;
		case STRUCT_TYPE_SYM_INDEXES:
			ret = add_sym_index_info( info, analyzer );
			break;
		case STRUCT_TYPE_SECTION2_STR_TABLE:
			ret = add_str_table_info( info, analyzer );
			break;
		case STRUCT_TYPE_LONGNAME_SECTION_STR_TABLE:
			ret = add_str_table_info( info, analyzer );
			break;
		case STRUCT_TYPE_COFF_FILE_HEADER:
			ret = add_coff_file_hdr_info( info, analyzer );
			break;
		case STRUCT_TYPE_COFF_OPTIONAL_28_HEADER:
			ret = add_coff_optional_28_hdr_info( info, analyzer );
			break;
		case STRUCT_TYPE_COFF_SECTION_HEADER:
			ret = add_coff_section_hdr_info( info, analyzer );
			break;
		case STRUCT_TYPE_COFF_STR_TABLE:
			ret = add_str_table_info( info, analyzer );
			break;
		default:
			break;
		}
	}

	return ret;
}

dword CALLBACK thread_analyze_file( LPVOID param )
{
	int32 ret;
	MSG msg;
	analyze_context *context;

	ASSERT( NULL != param );
	context = ( analyze_context* )param;

	ASSERT( '\0' != context->file_path[ 0 ] );

	SetEvent( context->start_event );

	context->analyzer.strs_analyze = when_find_lib_func_name;
	context->analyzer.code_analyze = when_func_code_finded;
	context->analyzer.syms_analyze = when_find_lib_func_name;
	context->analyzer.struct_analyze = analzye_all_struct;
	context->analyzer.obj_file_analyze = analyze_obj_file;
	context->analyzer.error_handler = error_handle;
	context->analyzer.context = context;

	while( TRUE )
	{
		ret = ::GetMessage( &msg, ( HWND )0xffffffff, 0, NULL );
		if( FALSE == ret )
		{
			goto exit_thread;
		}

		ASSERT( NULL == msg.hwnd );

		switch( msg.message )
		{
		case WM_START_FILE_ANALYZE:
			init_analyzing( context->file_path, &context->analyzer );
			break;
		case WM_MAIN_TREE_ITEM_SELED:
			on_main_tree_item_seled( ( HTREEITEM )msg.lParam, &context->analyzer );
			break;
		case WM_LIST_DETAIL_ITEM_SELED:
			on_detail_tree_item_seled( ( HTREEITEM )msg.lParam, &context->analyzer );
			break;
		case WM_MAIN_TREE_ITEM_RCLICK:
			on_main_tree_item_rclick( &context->analyzer );
			break;
		default:
			break;
		}
	}


exit_thread:
	if( NULL != context->analyzer.all_file_data )
	{
		release_file_data( &context->analyzer.all_file_data );
	}
	ExitThread( 0 );
	return 0;
}

int32 exit_work_thread( analyze_context *context )
{
	int32 i;
	int32 ret;
	dword wait_ret;
	g_bStop = TRUE;
	
	ASSERT( NULL != context );
	if( NULL == context->analyze_thread )
	{
		return 0;
	}

	for( i = 0; i < 30; i ++ )
	{
		ret = PostThreadMessage( context->thread_id, WM_CLOSE, 0, 0 );
		if( TRUE == ret )
		{
			break;
		}
	}
	ASSERT( FALSE != ret );

	wait_ret = WaitForSingleObject( context->analyze_thread, 500 );
	if( wait_ret != WAIT_OBJECT_0 )
	{
		if( NULL != context->analyzer.pe_write_info )
		{
			destroy_pe_writer( context->analyzer.pe_write_info ); 
		}

		TerminateThread( context->analyze_thread, 0 );
		context->analyze_thread = NULL;
		if( NULL != context->analyzer.all_file_data )
		{
			release_file_data( &context->analyzer.all_file_data );
		}
	}

	ASSERT( NULL == context->analyzer.all_file_data );
	return 0;
}

#include <Wininet.h>
void CpeanalyzerDlg::OnBnClickedOk()
{
	int32 i;
	int32 ret;
	dword wait_ret;
	HWND tree_main;
	HWND list_detail;
	HWND edit_file_path;

	exit_work_thread( &analyzing_context );

	tree_main = ( HWND )::GetDlgItem( m_hWnd, IDC_TREE_MAIN );
	list_detail = ( HWND )::GetDlgItem( m_hWnd, IDC_LIST_DETAIL );
	edit_file_path = ( HWND )::GetDlgItem( m_hWnd, IDC_EDIT_FILE_PATH );

	TreeView_DeleteAllItems( tree_main );
	ListView_DeleteAllItems( list_detail );

	analyzing_context.main_wnd = m_hWnd;
	analyzing_context.tree_main = tree_main;
	analyzing_context.list_detail = list_detail;
	::GetWindowText( edit_file_path, analyzing_context.file_path, sizeof( analyzing_context.file_path ) );
	if( '\0' == analyzing_context.file_path[ 0 ] )
	{
		::MessageBox( NULL, "Please input the name the analyzing file\n", "Error", NULL );
		return;
	}

	analyzing_context.start_event = CreateEvent( NULL, FALSE, FALSE, NULL );
	if( NULL == analyzing_context.start_event )
	{
		return;
	}

	analyzing_context.analyze_thread = CreateThread( NULL, 0, thread_analyze_file, &analyzing_context, NULL, &analyzing_context.thread_id );
	if( NULL == analyzing_context.analyze_thread )
	{
		return;
	}

	wait_ret = WaitForSingleObject( analyzing_context.start_event, INFINITE );
	if( WAIT_OBJECT_0 != wait_ret )
	{
		return;
	}

	for( ; ; )
	{
		ret = ::PostThreadMessage( analyzing_context.thread_id, WM_START_FILE_ANALYZE, 0, 0 );
		if( TRUE == ret )
		{
			break;
		}
	}

	if( FALSE == ret )
	{
		exit_work_thread( &analyzing_context );
	}
}

void CpeanalyzerDlg::OnBnClickedButtonSelFile()
{
	int32 ret;
	HWND edit;
	char file_name[ MAX_PATH ];
	
	edit = ::GetDlgItem( m_hWnd, IDC_EDIT_FILE_PATH );
	ASSERT( NULL != edit );

	ret = open_file_dlg( m_hWnd, file_name, MAX_PATH, 0 );
	if( 0 > ret )
	{
		::SetWindowText( edit, "" );
	}

	::SetWindowText( edit, file_name );
	OnBnClickedOk(); 
}

void CpeanalyzerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	
	if( analyzing_context.analyze_thread != NULL )
	{
		exit_work_thread( &analyzing_context );
	}
	OnCancel();
}

void CpeanalyzerDlg::OnBnClickedStopAnalyze()
{
	// TODO: 在此添加控件通知处理程序代码

	if( analyzing_context.analyze_thread == NULL )
		return;

	exit_work_thread( &analyzing_context );
}

void CpeanalyzerDlg::OnTvnSelchangedTreeMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	int32 ret;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	ret = PostThreadMessage( analyzing_context.thread_id, WM_MAIN_TREE_ITEM_SELED, ( WPARAM )NULL, ( LPARAM )pNMTreeView->itemNew.hItem );
	if( FALSE == ret )
	{
		exit_work_thread( &analyzing_context );
	}
	*pResult = 0;
}

LRESULT CpeanalyzerDlg::OnDoUIWork( WPARAM wParam, LPARAM lParam )
{
	return create_context_menu( m_hWnd, ( char* )lParam );
}

void CpeanalyzerDlg::OnRClientTreeMain( NMHDR *pNMHDR, LRESULT *pResult )
{
	POINT cur_pt;
	int32 ret;
	//TVHITTESTINFO HitTestInfo;

	//GetCursorPos( &HitTestInfo.pt );
	//::ScreenToClient( pNMHDR->hwndFrom, &HitTestInfo.pt );
	//TreeView_HitTest( pNMHDR->hwndFrom, &HitTestInfo );

	//if( NULL == HitTestInfo.hItem || 0 == ( TVHT_ONITEM & HitTestInfo.flags ) )
	//{
	//	goto __return;
	//}

	ret = PostThreadMessage( analyzing_context.thread_id, WM_MAIN_TREE_ITEM_RCLICK, ( WPARAM )NULL, ( LPARAM )NULL );
	if( FALSE == ret )
	{
		exit_work_thread( &analyzing_context );
	}
	
__return:
	*pResult = 0;
}

void CpeanalyzerDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//lpMeasureItemStruct->itemHeight = 10;
	//lpMeasureItemStruct->itemWidth = 30;
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CpeanalyzerDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
}

void CpeanalyzerDlg::OnLvnItemchangedListDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
