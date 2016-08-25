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
[!if CODE_USE_BASE_CLASS]
class [!output CODE_CLASS_NAME] : public [!output CODE_BASE_CLASS]
[!else]
class [!output CODE_CLASS_NAME]
[!endif]
{
public:
[!if CODE_USE_CONSTRUCTION]
[!if CODE_USE_CPP_FILES]
	[!output CODE_CLASS_NAME]();
[!else]
	[!output CODE_CLASS_NAME]()
	{
	}
[!endif]
[!endif]
[!if CODE_USE_DESTRUCTION]
[!if CODE_USE_CPP_FILES]
	virtual ~[!output CODE_CLASS_NAME]();
[!else]
	virtual ~[!output CODE_CLASS_NAME]()
	{
	}
[!endif]
[!endif]
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]
[!if CODE_USE_GUIDSYMBOL]
#endif // !defined([!output FILE_NAME_SYMBOL])
[!else]
#endif // [!output FILE_NAME_SYMBOL]
[!endif]