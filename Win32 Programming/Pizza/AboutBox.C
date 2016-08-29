#include "StdSDK.h"
#include "resource.h"
#include "dlgtemplate.h"


LPDLGTEMPLATE makeAboutBoxTemplate()
    {
      // IDD_ABOUTBOX DIALOG DISCARDABLE  22, 17, 204, 83
      // STYLE DS_MODALFRAME | DS_CONTEXTHELP | WS_POPUP | WS_VISIBLE | 
      //       WS_CAPTION | WS_SYSMENU
      // CAPTION "About "
      // FONT 8, "System"
      // BEGIN
      //    DEFPUSHBUTTON   "OK",IDOK,166,63,32,14,WS_GROUP
      //    ICON            IDR_MAINFRAME,IDC_STATIC,3,2,18,20
      //    LTEXT           "FileDescription",IDC_ABOUT_FILEDESCRIPTION,
      //			30,2,118,8
      //    RTEXT           "ProductVersion",IDC_ABOUT_VERSION,180,2,17,8
      //    LTEXT           "LegalCopyright",IDC_ABOUT_LEGALCOPYRIGHT,
      //			30,10,168,8
      //    LTEXT           "Comments",IDC_ABOUT_COMMENTS,30,18,168,8
      //    CONTROL         "",IDC_STATIC,"Static",SS_BLACKRECT,2,31,200,1
      //    LTEXT           "OSVERSION",IDC_ABOUT_OSVERSION,4,34,196,8
      //    LTEXT           "ProcessorVersion",IDC_ABOUT_PROCESSORVERSION,
      //			4,42,196,8
      //    CONTROL         "",IDC_STATIC,"Static",SS_BLACKRECT,2,52,200,1
      //    LTEXT           "LegalTrademarks",IDC_ABOUT_LEGALTRADEMARKS,
      //			3,57,156,18
      //    LTEXT           "Version ",IDC_STATIC,152,2,28,8
      //END
     
#define DLGTEMPLATE_WORKING_SIZE 4096
      LPDLGTEMPLATE templ;
      LPDLGITEMTEMPLATE item;

      //================================================================
      // IDD_ABOUTBOX DIALOG DISCARDABLE  22, 17, 204, 83
      // STYLE DS_MODALFRAME | DS_CONTEXTHELP | WS_POPUP | WS_VISIBLE | 
      //       WS_CAPTION | WS_SYSMENU
      // CAPTION "About "
      // FONT 8, "System"
      //================================================================
      templ = DIALOG(&item, DLGTEMPLATE_WORKING_SIZE,
      			22, 17, 204, 83,
			DS_MODALFRAME | DS_CONTEXTHELP | WS_POPUP |
			WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
			0,
			NULL,		// menu
			NULL,		// class
			L"About ",	// caption
			L"System",	// font
			8);		// font height

       if(templ == NULL)
	  return NULL;


//*****************************************************************************
//*****************************************************************************

      //================================================================
      //    DEFPUSHBUTTON   "OK",IDOK,166,63,32,14,WS_GROUP
      //================================================================

      templ->cdit++;
      item = DEFPUSHBUTTON(item, L"OK", IDOK, 166, 63, 32, 14, 
      			WS_GROUP | WS_TABSTOP | WS_VISIBLE, 0);

      //================================================================
      //    ICON            IDR_MAINFRAME,IDC_STATIC,3,2,18,20
      //================================================================

      templ->cdit++;
      item = ICON(item, MAKEINTRESOURCEW(IDR_MAINFRAME), IDC_STATIC, 3, 2, 18, 20, 
      				WS_VISIBLE, 0);

      //================================================================
      //    LTEXT           "FileDescription",IDC_ABOUT_FILEDESCRIPTION,
      //			30,2,118,8
      //================================================================

      templ->cdit++;
      item = LTEXT(item, L"FileDescription", IDC_ABOUT_FILEDESCRIPTION,
      				30, 2, 118, 8, WS_VISIBLE, 0);
      //================================================================
      //    RTEXT           "ProductVersion",IDC_ABOUT_VERSION,180,2,17,8
      //================================================================

      templ->cdit++;
      item = RTEXT(item, L"ProductVersion", IDC_ABOUT_VERSION, 
      				180, 2, 17, 8, WS_VISIBLE, 0);

      //================================================================
      //    LTEXT           "LegalCopyright",IDC_ABOUT_LEGALCOPYRIGHT,
      //			30,10,168,16
      //================================================================

      templ->cdit++;
      item = LTEXT(item, L"LegalCopyright", IDC_ABOUT_LEGALCOPYRIGHT,
      				30, 10, 168, 16, WS_VISIBLE, 0);

      //================================================================
      //    LTEXT           "Comments",IDC_ABOUT_COMMENTS,30,26,168,8
      //================================================================

      templ->cdit++;
      item = LTEXT(item, L"Comments", IDC_ABOUT_COMMENTS,
      				30, 26, 168, 8, WS_VISIBLE, 0);

      //================================================================
      //    CONTROL         "",IDC_STATIC,"Static",SS_BLACKRECT,2,39,200,1
      //================================================================

      templ->cdit++;
      item = CONTROL(item, L"", IDC_STATIC, L"Static", 
      					SS_BLACKRECT | WS_VISIBLE, 
      					2, 39, 200, 1, 0);

      //================================================================
      //    LTEXT           "OSVERSION",IDC_ABOUT_OSVERSION,4,42,196,8
      //================================================================


      templ->cdit++;
      item = LTEXT(item, L"OSVERSION", IDC_ABOUT_OSVERSION,
      				4, 42, 196, 8, WS_VISIBLE, 0);

      //================================================================
      //    LTEXT           "ProcessorVersion",IDC_ABOUT_PROCESSORVERSION,
      //			4,50,196,8
      //================================================================


      templ->cdit++;
      item = LTEXT(item, L"ProcessorVersion", IDC_ABOUT_PROCESSORVERSION,
      				4, 50, 196, 8, WS_VISIBLE, 0);

      //================================================================
      //    CONTROL         "",IDC_STATIC,"Static",SS_BLACKRECT,2,60,200,1
      //================================================================

      templ->cdit++;
      item = CONTROL(item, L"", IDC_STATIC, L"Static", 
      					SS_BLACKRECT | WS_VISIBLE, 
      					2, 60, 200, 1, 0);

      //================================================================
      //    LTEXT           "LegalTrademarks",IDC_ABOUT_LEGALTRADEMARKS,
      //			3,65,156,18
      //================================================================

      templ->cdit++;
      item = LTEXT(item, L"LegalTrademarks", IDC_ABOUT_LEGALTRADEMARKS,
      				3, 65, 156, 18, WS_VISIBLE, 0);

      //================================================================
      //    LTEXT           "Version ",IDC_STATIC,152,2,28,8
      //================================================================

      templ->cdit++;
      item = LTEXT(item, L"Version", IDC_STATIC,
      				152, 2, 28, 8, WS_VISIBLE, 0);

      return templ;
    }
