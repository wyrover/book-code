// [!output CODE_FILE_NAME].h : interface of the [!output CODE_CLASS_NAME] class
//
/////////////////////////////////////////////////////////////////////////////
[!if CODE_USE_GUIDSYMBOL]
#if !defined([!output FILE_NAME_SYMBOL])
#define [!output FILE_NAME_SYMBOL]
[!else]
#ifndef [!output FILE_NAME_SYMBOL]
#define [!output FILE_NAME_SYMBOL]
[!endif]

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class [!output CODE_CLASS_NAME] : public ui_page_dialog_t<[!output CODE_CLASS_NAME],[!output CODE_DIALOG_ID]>,
				   public page_base_t<[!output CODE_CLASS_NAME]>
{
public:
	void OnInit()
	{
	}
	void OnProjectChange(project_dsp *)
	{
	}
	void OnClassChange(Token * tkcls)
	{
	}
	void OnResDialogChange(CResDialog * dlg)
	{
	}
	void OnResMenuChange(CResMenu * menu)
	{
	}
	void OnCloseProject()
	{
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]
[!if CODE_USE_GUIDSYMBOL]
#endif // !defined([!output FILE_NAME_SYMBOL])
[!else]
#endif // [!output FILE_NAME_SYMBOL]
[!endif]