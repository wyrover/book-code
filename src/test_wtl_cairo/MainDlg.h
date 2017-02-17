// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

inline std::string to_utf8(const std::wstring& string)
{
	int required_size =
		WideCharToMultiByte(CP_UTF8, 0,
		string.c_str(), (int)string.size(),
		NULL, 0, NULL, NULL);

	if (required_size == 0)
		return "";

	std::vector<char> buf(++required_size);
	WideCharToMultiByte(CP_UTF8, 0,
		string.c_str(), (int)string.size(),
		&buf[0], required_size,
		NULL, NULL);
	return std::string(&buf[0]);
}

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		Invalidate(FALSE);


		


		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
	
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CRect clientrc;
		CSize size;
		static HBITMAP hBitmap = 0;
		HBITMAP hOldBitmap;
		GetClientRect(&clientrc);
		CPaintDC dc(m_hWnd);
		CDC memdc;
		if (memdc.CreateCompatibleDC(dc.m_hDC))
		{
			hBitmap = CreateCompatibleBitmap(dc.m_hDC, clientrc.Width(), clientrc.Height());
			hOldBitmap = memdc.SelectBitmap(hBitmap);

			

			cairo_surface_t *surface = cairo_win32_surface_create (memdc.m_hDC);
			cairo_t *cr = cairo_create(surface);

			// Draw on the cairo context.
			gradientExample(cr);
			
			// Cleanup.
			cairo_destroy(cr);
			cairo_surface_destroy(surface);
			memdc.DrawText( _T( "I'm custom drawn."), 0, &clientrc,
				DT_CENTER | DT_SINGLELINE | DT_VCENTER );
			dc.BitBlt(0, 0, clientrc.Width(), clientrc.Height(), memdc, 0, 0, SRCCOPY);
			memdc.SelectBitmap(hOldBitmap);
		}

		return 0;
	}
	
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;
	}
	
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	void gradientExample( cairo_t* cr )
	{
		CRect clientrc;		
		GetClientRect(&clientrc);


		

		cairo_pattern_t *pat;
		pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, 256.0);
		cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
		cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
		cairo_rectangle (cr, 0, 0, clientrc.Width(), clientrc.Height());
		cairo_set_source (cr, pat);
		cairo_fill (cr);
		cairo_pattern_destroy (pat);


		


		pat = cairo_pattern_create_radial (115.2, 102.4, 25.6,
			102.4,  102.4, 128.0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
		cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
		cairo_set_source (cr, pat);
		cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
		cairo_fill (cr);
		cairo_pattern_destroy (pat);



		int i;
		for ( i = 1; i <= 10; i++) {
			cairo_set_source_rgba(cr, 0, 0, 1, i*0.1);
			cairo_rectangle(cr, 50*i, 20, 40, 40);
			cairo_fill(cr);  
		}      

		/************************************************************************/
		/* 平移变换                                                                     */
		/************************************************************************/
		cairo_set_source_rgb(cr, 0.2, 0.3, 0.8);
		cairo_rectangle(cr, 10, 10, 30, 30);
		cairo_fill(cr);

		cairo_translate(cr, 20, 20);
		cairo_set_source_rgb(cr, 0.8, 0.3, 0.2);
		cairo_rectangle(cr, 0, 0, 30, 30);
		cairo_fill(cr);

		cairo_translate(cr, 30, 30);
		cairo_set_source_rgb(cr, 0.8, 0.8, 0.2);
		cairo_rectangle(cr, 0, 0, 30, 30);
		cairo_fill(cr);

		cairo_translate(cr, 40, 40);
		cairo_set_source_rgb(cr, 0.3, 0.8, 0.8);
		cairo_rectangle(cr, 0, 0, 30, 30);
		cairo_fill(cr);    


		cairo_select_font_face(cr, to_utf8(L"宋体").c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, 22);

		cairo_set_source_rgb(cr, 0.9 , 0.9 , 0.9);
		cairo_move_to(cr, 100, 0);
		cairo_show_text(cr, to_utf8(L" Beckov 2012 , (c) Jan Bodnar 中文测试").c_str());
		cairo_stroke(cr); 


		

		/************************************************************************/
		/* 仿射变换                                                                     */
		/************************************************************************/
		cairo_matrix_t matrix;

		cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
		cairo_rectangle(cr, 20, 30, 80, 50);
		cairo_fill(cr);
		cairo_matrix_init(&matrix,
			1.0, 0.5,
			0.0, 1.0,
			0.0, 0.0);

		cairo_transform(cr, &matrix);
		cairo_rectangle(cr, 130, 30, 80, 50);
		cairo_fill(cr);


		/************************************************************************/
		/* 缩放变换                                                                     */
		/************************************************************************/
		cairo_set_source_rgb(cr, 0.2, 0.3, 0.8);
		cairo_rectangle(cr, 10, 10, 90, 90);    
		cairo_fill(cr);

		cairo_scale(cr, 0.6, 0.6);
		cairo_set_source_rgb(cr, 0.8, 0.3, 0.2);
		cairo_rectangle(cr, 30, 30, 90, 90);    
		cairo_fill(cr);  

		cairo_scale(cr, 0.8, 0.8);
		cairo_set_source_rgb(cr, 0.8, 0.8, 0.2);
		cairo_rectangle(cr, 50, 50, 90, 90);    
		cairo_fill(cr);      

		/************************************************************************/
		/* 位图                                                                     */
		/************************************************************************/
		/*cairo_surface_t *image;  
		image = cairo_image_surface_create_from_png("png-0070.png");
		cairo_set_source_surface(cr, image, 10, 10);
		cairo_surface_destroy(image);*/


		
		
	}
};
